/***
This is a derivative work of ADMS.  Original code is copyight the original authors.
Modifications Copyright Luther T. Cat

ADMS
Copyright (C) 2015  Luther T. Cat
Original code copyright the original authors.

This file is part of ADMS.

ADMS is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, version 2 of the License.

ADMS is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with ADMS.  If not, see <http://www.gnu.org/licenses/>.
***/

#include "adms.h"
#include "adms_message.h"
#include "adms_admsmain.h"
#include "adms_simulator.h"
#include "json_serializer.h"
#include <cstdlib>
#include <cstdio>
#include <libgen.h>
#include <list>
#include <string>
#include <iostream>
#include <fstream>
/* examples: /a/b => \0,a,b and a/b/ => a,b,\0*/
static std::string mydirname (std::string myname)
{
  char *myin = strdup(myname.c_str());
  std::string ret = dirname(myin);
  free(myin);
  return ret;
}
static std::string mybasename(std::string myname)
{
  char *myin = strdup(myname.c_str());
  std::string ret = basename(myin);
  free(myin);
  return ret;
}
static std::string filename (std::string myname)
{
  std::string myfilename;
  std::string mymybasename=mybasename(myname);
  myfilename += mydirname(myname);
  if(!mymybasename.empty())
  {
    myfilename += ADMS_PATH_SEPARATOR;
    myfilename += mymybasename;
  }
  return myfilename;
}

static std::list<std::string> getlist_from_argv (const int argc,const char* *argv,const char* opt,const char* argtype)
{
  std::list<std::string> mylist;
  for(size_t i=1; i<argc; i++)
  {
    std::string myargvalue;
    const char* value=argv[i];
    {
      if(!strcmp(opt,"-f"))
      {
        if((strlen(value)>2)&&(value[0]=='-'))
        {}
        else if((strlen(value)==2)&&(value[0]=='-'))
          i++;
        else
          myargvalue=argv[i];
      }
      if(!strcmp(opt,value))
      {
        if(i<argc-1)
          myargvalue=argv[++i];
        else
          adms_message_fatal("option %s: no value found - expecting %s\n",opt,argtype);
      }
      else if((strlen(value)>2)&&!strncmp(opt,value,2))
      {
        myargvalue=std::string(value).substr(2);
      }

      if(!myargvalue.empty())
      {
        std::string myunixpath=myargvalue;
        if(!strcmp(argtype,"file"))
        {
          if(!adms_file_isregular(myunixpath))
            adms_message_fatal("option %s: cannot read file '%s'\n",opt,myunixpath.c_str());
        }
        else if(!strcmp(argtype,"directory"))
        {
          if(!adms_file_isdirectory(myunixpath))
            adms_message_fatal("option %s: cannot read directory '%s'\n",opt,myunixpath.c_str());
        }
        mylist.push_back(myunixpath);
        adms_message_verbose("shift: %s %s\n",opt,myunixpath.c_str());
      }
    }
  }
  return mylist;
}

#include "admsPreprocessor.h"
#include "admsVeriloga.h"
static void parseva (const int argc,const char** argv, std::string myverilogamsfile)
{
  p_slist myli;

  std::string mytmpverilogamsfile;
  mytmpverilogamsfile += ".";
  mytmpverilogamsfile += mybasename(myverilogamsfile);
  mytmpverilogamsfile += ".adms";

  root()->_filename=mybasename(myverilogamsfile);
  root()->_fullfilename=myverilogamsfile;
  root()->_curfilename=myverilogamsfile;
  adms_message_info("%sJSON-%s\n",PACKAGE_NAME,PACKAGE_VERSION);
  //adms_message_info("%sJSON-%s %s %s\n",PACKAGE_NAME,PACKAGE_VERSION,__DATE__,__TIME__);
  /* preprocess input file */
  {
    p_preprocessor mypreprocessor=new s_preprocessor;
    FILE* myverilogamsfilefh=adms_file_open_read(myverilogamsfile.c_str());
    FILE* ofh=fopen(mytmpverilogamsfile.c_str(),"wb");
    if(!ofh)
      adms_message_fatal("%s: failed to open file [write mode]\n",mytmpverilogamsfile.c_str());
    adms_preprocessor_setfile_input(myverilogamsfilefh);
    mypreprocessor->cur_line_position=1;
    mypreprocessor->cur_char_position=1;
    mypreprocessor->cur_message.clear();
    mypreprocessor->fid=myverilogamsfilefh;
    mypreprocessor->filename=myverilogamsfile;
    adms_preprocessor_valueto_main(new s_preprocessor_main);
    pproot()->Defined=NULL;
    pproot()->Text=NULL;
    pproot()->cr_filename=myverilogamsfile;
    pproot()->cr_scanner=mypreprocessor;
    pproot()->error=0;
    adms_slist_push(&pproot()->skipp_text,(p_adms)(long)(0));

    pproot()->includePath=getlist_from_argv(argc,argv,"-I","directory");

    pproot()->includePath.push_front(".");

    adms_preprocessor_get_define_from_argv(argc,argv);
    adms_preprocessor_define_add_default("insideADMS");
    adms_message_verbose("create temporary file %s\n",mytmpverilogamsfile.c_str());
    preprocessorparse();
    /* save preprocessed Verilog-AMS file */
    fputs("# 1 \"",ofh);
    fputs(pproot()->cr_scanner->filename.c_str(),ofh);
    fputs("\"\n",ofh);
    adms_slist_inreverse(&pproot()->Text);
    for(myli=pproot()->Text;myli;myli=myli->next)
      fputs(((p_preprocessor_text)(myli->data))->_str.c_str(),ofh);
    fclose(ofh);
    /* free preprocessor */
    free(pproot()->cr_scanner);
    fclose(myverilogamsfilefh);
  }
  /* parses temporary file */
  {
    FILE* ifh=adms_file_open_read(mytmpverilogamsfile.c_str());
    if(root()->_dbg_vla==admse_yes)
      adms_veriloga_setint_yydebug(1);
    adms_veriloga_setfile_input(ifh);
    if(verilogaparse())
      adms_message_fatal("parse Verilog-AMS preprocessed file '%s' failed\n",mytmpverilogamsfile.c_str());
    else
      adms_message_verbose("No error found during parsing\n");
    fclose(ifh);
  }
}

int main (const int argc,const char**argv)
{
#if 0
  char* myadmsimplicitxmlfile;
  time_t mytime=time(NULL);
  FILE*xfh;
  char* xheader=NULL;
  const char* xinterface=".interface.xml";
#endif

  /*admsmain*/
  rootnew(adms_admsmain_new("admsmain"));
  if(getenv("adms_info")    && !strcmp(getenv("adms_info"),    "no"))  root()->_info=admse_no;
  if(getenv("adms_usage")   && !strcmp(getenv("adms_usage"),   "no"))  root()->_usage=admse_no;
  if(getenv("adms_obsolete")&& !strcmp(getenv("adms_obsolete"),"no"))  root()->_obsolete=admse_no;
  if(getenv("adms_hint")    && !strcmp(getenv("adms_hint"),    "yes")) root()->_hint=admse_yes;
  if(getenv("adms_verbose") && !strcmp(getenv("adms_verbose"), "yes")) root()->_verbose=admse_yes;
  if(getenv("adms_debug")   && !strcmp(getenv("adms_debug"),   "yes")) root()->_debug=admse_yes;
  if(getenv("adms_dbg_vla") && !strcmp(getenv("adms_dbg_vla"), "yes")) root()->_dbg_vla=admse_yes;

/*main.simulator*/
  {
    std::string sfullname;
    sfullname += PACKAGE_NAME;
    sfullname += "JSON-";
    sfullname += PACKAGE_VERSION;
    root()->_simulator=adms_simulator_new(sfullname);
    root()->_simulator->_fullname=sfullname;
  }
/*main.simulator.developer*/
  {
    std::string sdeveloper;
    sdeveloper += PACKAGE_NAME;
    sdeveloper += "JSON";
    sdeveloper += "-";
    sdeveloper += PACKAGE_VERSION;
    /*
    sdeveloper += " (";
    sdeveloper += __DATE__;
    sdeveloper += "-";
    sdeveloper += __TIME__;
    sdeveloper += "";
    */
    root()->_simulator->_developer=sdeveloper;
  }
/*shell*/
  adms_preprocessor_setint_yydebug(0);

  if(
    (argc==1)
    ||
    ((argc==2)&&!strcmp(argv[1],"-h"))
    ||
    ((argc==2)&&!strcmp(argv[1],"--help"))
    ||
    ((argc==3)&&!strcmp(argv[1],"--")&&!strcmp(argv[2],"help"))
  )
  {
    adms_message_usage("%sJSON-%s ",PACKAGE_NAME,PACKAGE_VERSION);
    adms_message_usage_continue("%s %s (os=%s compiler=%s)\n",__DATE__,__TIME__,ADMS_OS,ADMS_COMPILER);
    adms_message_usage("%sJSON source [options]\n",PACKAGE_NAME);
    adms_message_usage("files:\n");
    adms_message_usage("  source: verilog-ams source code\n");
    adms_message_usage("options:\n");
    adms_message_usage("  -D NAME: predefine NAME as a macro, with definition `1'\n");
    adms_message_usage("  -D NAME=DEFINITION: predefine NAME as a macro, with definition DEFINITION\n");
    adms_message_usage("  -I DIR: Add directory DIR to search path for header files\n");
    adms_message_usage("shell variables:\n");
    adms_message_usage("- adms_info=\"yes\"|\"no\": print info [default=yes]\n");
    adms_message_usage("- adms_usage=\"yes\"|\"no\": print usage [default=yes]\n");
    adms_message_usage("- adms_verbose=\"yes\"|\"no\": print messages [default=no]\n");
    adms_message_usage("- adms_debug=\"yes\"|\"no\": print more messages [default=no]\n");
    adms_message_usage("- adms_dbg_vla=\"yes\"|\"no\": debug messages during vla parsing [default=no]\n");
    adms_message_usage("- adms_dbg_pre=\"yes\"|\"no\": debug messages during vla preprocessing [default=no]\n");
#if 0
    adms_message_usage("-i or --info: miscellaneous info (release, web site, mailing list)\n");
#endif
    adms_message_usage("-v or --version: version number\n");
    adms_message_usage("-h or --help: short help\n");
  }
  else if(
    ((argc==2)&&!strcmp(argv[1],"-v"))
    ||
    ((argc==2)&&!strcmp(argv[1],"--version"))
    ||
    ((argc==3)&&!strcmp(argv[1],"--")&&!strcmp (argv[2],"version"))
  )
  {
    adms_message_usage("<release name=\"%sJSON\" version=\"%s\" date=\"%s\" time=\"%s\"/>\n",PACKAGE_NAME,PACKAGE_VERSION,__DATE__,__TIME__);
    return 0;
  }
#if 0
  else if(
    ((argc==2)&&!strcmp(argv[1],"-i"))
    ||
    ((argc==2)&&!strcmp(argv[1],"--info"))
    ||
    ((argc==3)&&!strcmp(argv[1],"--")&&!strcmp(argv[2],"info"))
  )
  {
    adms_message_usage("<info\n",PACKAGE_BUGREPORT);
    adms_message_usage("  author=\"laurent lemaitre\"\n",PACKAGE_BUGREPORT);
    adms_message_usage("  bug-report=\"%s\"\n",PACKAGE_BUGREPORT);
    adms_message_usage("  home-page=\"http://mot-adms.sourceforge.net\"\n");
    adms_message_usage("  mailing-list=\"mot-adms-users@lists.sourceforge.net\">\n");
    adms_message_usage("  <release name=\"%s\" version=\"%s\" ",PACKAGE_NAME,PACKAGE_VERSION);
    adms_message_usage_continue("date=\"%s\" time=\"%s\"/>\n",__DATE__,__TIME__);
    adms_message_usage("</info>\n");
    return 0;
  }
#endif

  if(
    (argc==1)
    ||
    ((argc==2)&&!strcmp(argv[1],"-h"))
    ||
    ((argc==2)&&!strcmp(argv[1],"--help"))
    ||
    ((argc==3)&&!strcmp(argv[1],"--")&&!strcmp(argv[2],"help"))
  )
    return 0;
/*parse verilogams list*/
  {
    std::list<std::string> l=getlist_from_argv(argc,argv,"-f","file");
    for (auto myli : l)
    {
      parseva(argc,argv,myli);
    }
  }

  if (root())
  {
    std::list<std::string> l=getlist_from_argv(argc,argv,"-o","out");
    if (l.size() != 1)
    {
      std::cerr << "No output file specified, please specify using -o\n";
      return -1;
    }
    else
    {
      const std::string &filename = *(l.begin());
      std::ofstream myfile;
      myfile.open(filename );
      if (myfile.is_open())
      {
        json_map_t  json_index;
        json_list_t json_rindex;
        json_serialize(root(), json_index, json_rindex);
        json_write(myfile, json_index, json_rindex);
        myfile.close();
      }
      else
      {
        std::cerr << "Could not write \"" << filename << "\" for output\n.";
        return -1;
      }
    }
    //// TODO: memory cleanup
  }

  return 0;
}

