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

#include "admsPreprocessor.h"
#include "adms_message.h"
#include "adms_slist.h"
#include "adms_admsmain.h"
#include <cstdlib>
#include <cassert>

extern int adms_preprocessor_leng;
static p_preprocessor_main adms_preprocessor_main;
void adms_preprocessor_valueto_main (p_preprocessor_main myadms_preprocessor_main)
{
  adms_preprocessor_main=myadms_preprocessor_main;
}
p_preprocessor_main pproot ()
{
  return adms_preprocessor_main;
}
void adms_preprocessor_get_define_from_argv (const int argc,const char** argv)
{
  int i;
  for (i=1; i<argc; i++)
  {
    std::string value=argv[i];
    std::string defineName;
    std::string defineValue;
    if ( (value.size()>2) && (value[0]=='-') && (value[1]=='D') )
    {
      std::string arg = value.substr(2);
      auto epos = arg.find('=');

      if (epos == std::string::npos)
      {
        adms_message_verbose("at command line argument, bad syntax ... '%s'\n",value.c_str());
      }
      else if (epos == (arg.size() - 1))
      {
        defineName = arg.substr(0, arg.size() - 1);
      }
      else if (epos == 0)
      {
        defineName = arg;
      }
      else
      {
        defineName  = arg.substr(0, epos);
        defineValue = arg.substr(epos+1);
      }
  
      if (!defineName.empty())
      {
        if (!defineValue.empty())
        {
          adms_preprocessor_define_add_default_with_text (defineName, defineValue);
        }
        else
        {
          adms_preprocessor_define_add_default (defineName);
        }
      }
    }
    if (!defineName.empty())
      adms_message_verbose("set pragma ... '%s'\n",defineName.c_str());
  }
}
int adms_preprocessor_add_message (std::string message)
{
  adms_message_info(message.c_str());
  return 0;
}
p_preprocessor_text adms_preprocessor_new_text_as_string(std::string name)
{
  p_preprocessor_text text= new s_preprocessor_text;
  text->_str = name;
  text->_isarg = 0;
  return text;
}
static int adms_preprocessor_pragma_substitute_text (p_preprocessor_substitutor substitutor)
{
  if (adms_preprocessor_pragma_define_is_def (substitutor->_pragma))
  {
    p_preprocessor_text space = adms_preprocessor_new_text_as_string(" ");
    p_slist l; for(l=substitutor->_pragma->text;l;l=l->next)
    {
      p_preprocessor_text text=(p_preprocessor_text)(l->data);
      p_slist arg=adms_slist_copy(substitutor->_pragma->arg);
      adms_slist_inreverse(&arg);
      substitutor->_cr_text=text;
      substitutor->_cr_newarg=adms_slist_copy(substitutor->_newarg);
      adms_slist_inreverse(&substitutor->_cr_newarg);
      if (text->_isarg==1)
      {
        p_slist ll; for(ll=arg;ll;ll=ll->next)
        {
          char* arg=(char*)ll->data;
          if(substitutor->_cr_text->_str == arg)
          {
            if(substitutor->_cr_newarg)
            {
              p_slist newarg=adms_slist_copy((p_slist)(substitutor->_cr_newarg->data));
              adms_slist_inreverse(&newarg);
              adms_slist_concat(&newarg,substitutor->_newtext);
              substitutor->_newtext=newarg;
            }
            else
              adms_slist_push(&(substitutor->_newtext),(p_adms)substitutor->_cr_text);
          }
          substitutor->_cr_newarg = substitutor->_cr_newarg->next;
        }
      }
      else
      {
        p_preprocessor_text newtext = adms_preprocessor_new_text_as_string(text->_str);
        newtext->_isarg = -1;
        adms_slist_push(&(substitutor->_newtext),(p_adms)newtext);
      }
      adms_slist_free(arg);
      adms_slist_free(substitutor->_cr_newarg);
    }
    adms_slist_push(&(substitutor->_newtext),(p_adms)space);
  }
  else
  {
    p_preprocessor_text name = adms_preprocessor_new_text_as_string(substitutor->_pragma->name);
    adms_slist_push(&(substitutor->_newtext),(p_adms)name);
  }
  return 0;
}
p_slist adms_preprocessor_new_text_as_substitutor (p_preprocessor_pragma_define define, p_slist newarg)
{
  p_preprocessor_substitutor substitutor=new s_preprocessor_substitutor;
  substitutor->_newtext=NULL;
  substitutor->_cr_text=NULL;
  substitutor->_cr_newarg=NULL;
  substitutor->_pragma = define;
  substitutor->_newarg = newarg;
  adms_preprocessor_pragma_substitute_text(substitutor);
  adms_slist_inreverse(&substitutor->_newtext);
  return substitutor->_newtext;
}
int adms_preprocessor_pragma_define_has_arg (p_preprocessor_pragma_define pragma)
{
  return (pragma->hasArg==1);
}
int adms_preprocessor_pragma_define_is_def (p_preprocessor_pragma_define pragma)
{
  return (pragma->isDefined==1);
}
int adms_preprocessor_identifier_is_def (char * name)
{
  int test;
  p_preprocessor_pragma_define pragma;
  if(
      (pragma = adms_preprocessor_pragma_define_exists (name))
      &&
      adms_preprocessor_pragma_define_is_def(pragma)
    )
    test = ( pragma->isDefined == 1 );
  else
    test = 0;
  return test;
}
p_preprocessor_pragma_define adms_preprocessor_identifer_set_undef (char * name)
{
  p_preprocessor_pragma_define pragma = adms_preprocessor_pragma_define_exists (name);
  if(!pragma)
    pragma = adms_preprocessor_define_add (name);
  pragma->isDefined = 0;
  return pragma;
}
int adms_preprocessor_identifier_is_ndef (char * name)
{
  return (!adms_preprocessor_identifier_is_def (name) );
}
int adms_preprocessor_pragma_define_is_ndef (p_preprocessor_pragma_define pragma)
{
  return (!adms_preprocessor_pragma_define_is_def(pragma));
}
int adms_preprocessor_pragma_define_has_noarg (p_preprocessor_pragma_define pragma)
{
  return (!adms_preprocessor_pragma_define_has_arg(pragma));
}
int adms_preprocessor_pragma_define_has_nullarg (p_preprocessor_pragma_define pragma)
{
  return adms_preprocessor_pragma_define_has_arg(pragma) && (adms_slist_length( pragma->arg )==0);
}
int adms_preprocessor_pragma_define_has_text (p_preprocessor_pragma_define pragma)
{
  return ((pragma->text)?1:0);
}
int adms_preprocessor_pragma_define_has_notext (p_preprocessor_pragma_define pragma)
{
  return (!adms_preprocessor_pragma_define_has_text(pragma));
}
int adms_preprocessor_pragma_define_has_noarg_and_notext (p_preprocessor_pragma_define pragma)
{
  int test;
  test = adms_preprocessor_pragma_define_has_noarg(pragma)
         &&
         adms_preprocessor_pragma_define_has_notext(pragma)
         ;
  return test;
}
int adms_preprocessor_pragma_define_has_noarg_and_text (p_preprocessor_pragma_define pragma)
{
  int test;
  test = adms_preprocessor_pragma_define_has_noarg(pragma)
         &&
         adms_preprocessor_pragma_define_has_text(pragma)
         ;
  return test;
}
int adms_preprocessor_pragma_define_has_nullarg_and_notext (p_preprocessor_pragma_define pragma)
{
  int test;
  test = adms_preprocessor_pragma_define_has_nullarg(pragma)
         &&
         adms_preprocessor_pragma_define_has_notext(pragma)
         ;
  return test;
}
int adms_preprocessor_pragma_define_has_nullarg_and_text (p_preprocessor_pragma_define pragma)
{
  int test;
  test = adms_preprocessor_pragma_define_has_nullarg(pragma)
         &&
         adms_preprocessor_pragma_define_has_text(pragma)
         ;
  return test;
}
int adms_preprocessor_pragma_define_has_arg_and_notext (p_preprocessor_pragma_define pragma)
{
  int test;
  test = adms_preprocessor_pragma_define_has_arg(pragma)
         &&
         adms_preprocessor_pragma_define_has_notext(pragma)
         ;
  return test;
}
int adms_preprocessor_pragma_define_has_arg_and_text (p_preprocessor_pragma_define pragma)
{
  int test;
  test = adms_preprocessor_pragma_define_has_arg(pragma)
         &&
         adms_preprocessor_pragma_define_has_text(pragma)
         ;
  return test;
}
static int adms_preprocessor_identifier_is_pragma_cb (p_preprocessor_pragma_define pragma, const char * identifier)
{
  return (!(pragma->name == identifier));
}
typedef int (*Binary) (const void* d1,const void* d2);
static p_slist adms_slist_find_custom (p_slist l,const void* data,Binary func)
{
  while(l)
  {
    if(!func(l->data,data))
       return l;
      l=l->next;
  }
  return nullptr;
}
p_preprocessor_pragma_define adms_preprocessor_pragma_define_exists (std::string name)
{
  p_slist Data;
  p_preprocessor_pragma_define pragma;
  if((Data=adms_slist_find_custom ( pproot()->Defined,name.c_str(),(Binary)adms_preprocessor_identifier_is_pragma_cb)))
    pragma=(p_preprocessor_pragma_define) Data->data;
  else
    pragma=NULL;
  return pragma;
}
p_preprocessor_pragma_define adms_preprocessor_define_add (std::string name)
{
  p_preprocessor_pragma_define pragma;
  if ((pragma = adms_preprocessor_pragma_define_exists(name)))
  {
    if( pragma->isDefined == 1 )
      adms_message_warning("pragma redefined ... '%s'\n",name.c_str());
    pragma->hasArg = 0;
    pragma->arg = NULL;
    pragma->text = NULL;
  }
  else
  {
    pragma=new s_preprocessor_pragma_define;
    pragma->hasArg = 0;
    pragma->arg=NULL;
    pragma->text=NULL;
    pragma->name = name;
    adms_slist_push(&(pproot()->Defined),(p_adms)pragma);
  }
  pragma->isDefined = 1;
  return pragma;
}
int adms_preprocessor_define_add_default (std::string name)
{
  p_preprocessor_pragma_define pragma;
  pragma = adms_preprocessor_define_add (name);
  adms_message_verbose("define macro ... '%s'\n",name.c_str());
  assert(adms_preprocessor_pragma_define_has_noarg_and_notext(pragma));
  return 0;
}
int adms_preprocessor_define_add_default_with_text (std::string name, std::string value)
{
  p_slist Text;
  p_preprocessor_text text;
  p_preprocessor_pragma_define pragma;
  text = adms_preprocessor_new_text_as_string(value);
  Text = adms_slist_new((p_adms)text);
  pragma = adms_preprocessor_define_add_with_text (name, Text);
  adms_message_verbose("define macro with value ... '%s=%s'\n",name.c_str(), value.c_str());
  assert(adms_preprocessor_pragma_define_has_noarg_and_text(pragma));
  return 0;
}
p_preprocessor_pragma_define adms_preprocessor_undefine_add (char * name)
{
  p_preprocessor_pragma_define pragma;
  pragma = adms_preprocessor_pragma_define_exists(name);
  if(!pragma)
    {
      pragma=new s_preprocessor_pragma_define;
      pragma->arg=NULL;
      pragma->text=NULL;
      pragma->name = name;
      adms_slist_push(&(pproot()->Defined),(p_adms)pragma);
    }
  pragma->isDefined = 0;
  return pragma;
}
p_preprocessor_pragma_define adms_preprocessor_define_add_with_arg (char * name, p_slist arg)
{
  p_preprocessor_pragma_define pragma;
  pragma = adms_preprocessor_define_add (name );
  pragma->hasArg = 1;
  pragma->arg = arg;
  pragma->text = NULL;
  return pragma;
}
p_preprocessor_pragma_define adms_preprocessor_define_add_with_text (std::string name, p_slist text)
{
  p_preprocessor_pragma_define pragma;
  pragma = adms_preprocessor_define_add (name );
  pragma->hasArg = 0;
  pragma->arg = NULL;
  pragma->text = text;
  return pragma;
}
p_preprocessor_pragma_define adms_preprocessor_define_add_with_arg_and_text (char * name, p_slist arg, p_slist text)
{
  p_preprocessor_pragma_define pragma;
  p_slist l;
  pragma = adms_preprocessor_define_add_with_arg (name, arg );
  pragma->text = text;
  for(l=pragma->text;l;l=l->next)
  {
    p_preprocessor_text text=(p_preprocessor_text)(l->data);
    p_slist ll; for(ll=pragma->arg;ll;ll=ll->next)
    {
      if((text->_isarg != -1) && (text->_str == (char*)(ll->data)))
        text->_isarg = 1;
    }
  }
  return pragma;
}
static p_continuator adms_preprocessor_get_line_position_cb (std::list<p_continuator> &l,int mycharpos)
{
  p_continuator ret = nullptr;
  auto it = std::find_if(l.begin(), l.end(), [&](p_continuator x)->bool {return mycharpos<=(x->char_position);});
  if (it != l.end())
  {
    ret = *it;
  }
  return ret;
}
int adms_preprocessor_get_line_position (p_preprocessor cr_preprocessor, int char_pos)
{
  int line_pos=cr_preprocessor->cur_line_position;
  p_continuator mycontinuator;
  if((mycontinuator=adms_preprocessor_get_line_position_cb(cr_preprocessor->cur_continuator_position,char_pos)))
    line_pos=mycontinuator->line_position;
  return line_pos;
}

