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

%{

#include "adms.h"
#include "adms_message.h"
#include "admsPreprocessor.h"
#include "preprocessorYacc.hh"
#include "adms_slist.h"
#include "adms_admsmain.h"
#include "adms_attribute.h"

#include <algorithm>

// remove deprecated warning from Apple c++ compiler
#define register

#ifndef INITIAL
#  define INITIAL 0
#endif

#ifdef _WIN32
#include <io.h>
#define YY_NO_UNISTD_H
#else
#include <unistd.h>
#endif

#define yyflex_unput \
{\
  int i;\
  char *yycopy=strdup(yytext);\
  for ( i = yyleng - 1; i >= 0; --i )\
    unput(yycopy[i]);\
  free(yycopy);\
}

#define DONT_SKIPP (pproot()->skipp_text->data==INT2ADMS(0))

void adms_preprocessor_setfile_input (FILE *ifile)
{
  yyin = ifile;
}

FILE *adms_preprocessor_getfile_input (void)
{
  return yyin;
}

void adms_preprocessor_setfile_output (FILE *ifile)
{
  yyout = ifile;
}

FILE *adms_preprocessor_getfile_output (void)
{
  return yyout;
}

#define tkreturn(token) {pproot()->cr_scanner->cur_char_position+=yyleng; return token;}

static std::string adms_preprocessor_lex_remove_leading_quote (char* name)
{
  std::string ret = name;
  return ret.substr(1);
}

static std::string adms_preprocessor_lex_get_path (char* macro, const char begin, const char end)
{
  std::string orig = macro;
  auto b = orig.find(begin);
  auto e = orig.rfind(begin);
  std::string ret = orig.substr(b+1,e-b-1);
  return ret;
}

void preprocessorerror (const char *message)
{
  p_preprocessor cr_preprocessor=pproot()->cr_scanner;
  adms_message_fatal ("[%s:%i]:%s: %s\n",
    cr_preprocessor->filename.c_str(),
    adms_preprocessor_get_line_position(cr_preprocessor,cr_preprocessor->cur_char_position-yyleng),
    message,
    !cr_preprocessor->cur_message.empty() ? cr_preprocessor->cur_message.c_str() : "(unknown)"
  );
}
static void adms_file_fprintf ( char *filename, const char *filedata)
{
  FILE *ofh = fopen (filename,"w");
  if (fprintf (ofh, "%s", filedata)==EOF)
    adms_message_fatal("%s: cannot write file\n", filename);
  fclose (ofh);
}
static FILE* adms_file_open_read_with_path (const char* myfilename, const std::list<std::string> &mypath)
{
  for (auto l : mypath)
  {
    FILE* myfh=NULL;
    std::string mypathname=l;
    mypathname += ADMS_PATH_SEPARATOR;
    mypathname += myfilename;
    if((myfh=fopen(mypathname.c_str(),"r")))
      return myfh;
  }
  return NULL;
}
static void adms_preprocessor_lex_include_file (const char* fileName)
{
  FILE*myfh;
  p_preprocessor scanner;
  std::string message;
  message += pproot()->cr_scanner->filename;
  message += ":";
  message += adms_integertostring(adms_preprocessor_get_line_position (pproot()->cr_scanner, 0));
  pproot()->cr_filename=fileName;
  if(!(myfh=adms_file_open_read_with_path(fileName,(pproot()->includePath))))
  {
    adms_message_fatal("[%s]: failed to open file ... '%s'\n",message.c_str(),fileName);
  }
  scanner=new s_preprocessor;
  adms_message_verbose("include file '%s'\n",fileName);
  pproot()->cr_scanner->buffer = (p_adms)YY_CURRENT_BUFFER;
  pproot()->Scanner.push_back(pproot()->cr_scanner);
  yyin = myfh;
  scanner->fid = yyin;
  scanner->cur_line_position = 1;
  scanner->cur_char_position = 1;
  scanner->filename=pproot()->cr_filename;
  scanner->buffer = (p_adms) yy_create_buffer(yyin,YY_BUF_SIZE );
  pproot()->cr_scanner = scanner;
  yy_switch_to_buffer((YY_BUFFER_STATE)scanner->buffer );
  std::string mystr = "# 1 \"";
  mystr += pproot()->cr_scanner->filename;
  mystr += "\"\n";
  preprocessorlval.mystr = strdup(mystr.c_str());
  BEGIN( INITIAL );
}

static std::string adms_preprocessor_lex_skipp_text()
{
  std::string outstring("");
  std::string mystr = yytext;
  size_t nbline = std::count(mystr.begin(), mystr.end(), '\n');
  pproot()->cr_scanner->cur_line_position += nbline;
  if (nbline > 0)
  {
    auto pos = mystr.rfind('\n');
    pproot()->cr_scanner->cur_char_position = mystr.size() - pos;
    outstring = std::string(nbline, '\n');
  }
  else
  {
    pproot()->cr_scanner->cur_char_position += mystr.size();
  }

  return outstring;
}

%}

whitespace	 [ \t\v\f]*
Whitespace	 [ \t\v\f]+
newline		 \r?\n
ident		 $?[A-Za-z_][A-Za-z0-9_]*
notident	 [^`a-zA-Z_\r\n]+
notpragma	 [^#`a-zA-Z_\r\n]+
xpath	         \"[^\"]*\"
ipath	         \<[^>]+\>
continuator	 \\\r?\n
anystring  \"[^\"]*\"
b10_uint   [1-9][0-9]*
 
comment          \/\*[^\*]*\*+([^\*\/][^\*]*\*+)*\/
comment_oneline  \/\/[^\r\n]*


%x PRAGMA_PATH
%x PRAGMA_COMMENT
%x PRAGMA_NAME
%x PRAGMA_ERROR
%x PRAGMA_DEFINE_START
%x PRAGMA_DEFINE_IGNORE
%x PRAGMA_DEFINE_NOARG
%x PRAGMA_DEFINE_WITHARG
%x PRAGMA_DEFINE_TEXT_START
%x PRAGMA_DEFINE_TEXT
%x PRAGMA_DEFINE_WITHARG_COMMA
%x PRAGMA_DEFINE_WITHARG_ARGS
%x PRAGMA_SUBSTITUTOR_START
%x PRAGMA_SUBSTITUTOR_ARG
%x PRAGMA_SUBSTITUTOR_PAIR
%x PRAGMA_SUBSTITUTOR_ARGITEM
%x PRAGMA_SUBSTITUTOR_ARGCOMMA
%x PRAGMA_SUBSTITUTOR_ARGEND

%%

\"[^"]*\"  {
  preprocessorlval.mystr = strdup( yytext );
  tkreturn(TK_STRING)
}

{continuator} {
  p_continuator continuator;

  continuator=new s_continuator;
  continuator->char_position = yyleng-1;
  continuator->line_position = pproot()->cr_scanner->cur_line_position;

  pproot()->cr_scanner->cur_line_position += 1;
  pproot()->cr_scanner->cur_char_position = 1;
  pproot()->cr_scanner->cur_continuator_position.push_back(continuator);

  tkreturn(TK_NOPRAGMA_CONTINUATOR)
}

 /* ignored sequences */

^#{Whitespace}{b10_uint}{whitespace}{anystring}{Whitespace}{newline} {
}

\r\n {
  preprocessorlval.mystr=strdup(yytext);
  return(TK_EOL);
}
\n {
  preprocessorlval.mystr=strdup(yytext);
  return(TK_EOL);
}
 
{Whitespace} {
  pproot()->cr_scanner->cur_char_position+=yyleng; 
  preprocessorlval.mystr=strdup(yytext);
  return(TK_SPACE );
}

{comment} {
  preprocessorlval.mystr=strdup(adms_preprocessor_lex_skipp_text().c_str()); return(TK_COMMENT);
}

{comment_oneline} {
  preprocessorlval.mystr=strdup(adms_preprocessor_lex_skipp_text().c_str()); return(TK_COMMENT);
}

 /* pragma */

^{whitespace}[`]define {
  pproot()->cr_scanner->cur_char_position+=yyleng; 
  if ( DONT_SKIPP )
    {
      BEGIN( PRAGMA_DEFINE_START );
      tkreturn(TK_DEFINE)
    }
  else
      BEGIN( PRAGMA_DEFINE_IGNORE );
}
<PRAGMA_DEFINE_IGNORE>.* {
  pproot()->cr_scanner->cur_char_position+=yyleng; 
  BEGIN( INITIAL );
}
<PRAGMA_DEFINE_IGNORE>{newline} {
  pproot()->cr_scanner->cur_message="- `define: unexpected end-of-line";
  yyflex_unput
  BEGIN( INITIAL );
}

^{whitespace}[`]undef {
  if ( DONT_SKIPP )
    {
      BEGIN( PRAGMA_NAME );
      tkreturn(TK_UNDEF)
    }
  else
      BEGIN( PRAGMA_DEFINE_IGNORE );
}
^{whitespace}[`]ifdef {
  preprocessorlval.mystr=strdup("");;
  BEGIN( PRAGMA_NAME );
  tkreturn(TK_IFDEF)
}
^{whitespace}[`]ifndef {
  preprocessorlval.mystr=strdup("");;
  BEGIN( PRAGMA_NAME );
  tkreturn(TK_IFNDEF)
}
^{whitespace}[`]else {
  std::string string;
  string += "# ";
  string += adms_integertostring(pproot()->cr_scanner->cur_line_position+1);
  string += " \"";
  string += pproot()->cr_scanner->filename;
  string += "\"";
  preprocessorlval.mystr=strdup(string.c_str());
  BEGIN( PRAGMA_COMMENT );
  tkreturn(TK_ELSE)
}
^{whitespace}[`]endif {
  std::string string;
  string += "# ";
  string += adms_integertostring(pproot()->cr_scanner->cur_line_position+1);
  string += " \"";
  string += pproot()->cr_scanner->filename;
  string += "\"";
  preprocessorlval.mystr=strdup(string.c_str());
  BEGIN( PRAGMA_COMMENT );
  tkreturn(TK_ENDIF)
}
^{whitespace}[`]include {
  if(DONT_SKIPP)
    BEGIN(PRAGMA_PATH);
  else
    BEGIN(PRAGMA_DEFINE_IGNORE);
}

<PRAGMA_DEFINE_START>{ident}/{newline} {
  BEGIN( PRAGMA_DEFINE_NOARG );
  preprocessorlval.mystr=strdup(yytext );
  tkreturn(TK_PRAGMA_NAME)
}
<PRAGMA_DEFINE_START>{ident}/{Whitespace} {
  BEGIN( PRAGMA_DEFINE_NOARG );
  preprocessorlval.mystr=strdup(yytext );
  tkreturn(TK_PRAGMA_NAME)
}
<PRAGMA_DEFINE_START>{ident}/\( {
  BEGIN( PRAGMA_DEFINE_WITHARG );
  preprocessorlval.mystr=strdup(yytext );
  tkreturn(TK_PRAGMA_NAME)
}
<PRAGMA_DEFINE_START>{Whitespace} {
  pproot()->cr_scanner->cur_char_position+=yyleng; 
}
<PRAGMA_DEFINE_START>{newline} {
  pproot()->cr_scanner->cur_message="- pragma name: unexpected end-of-line";
  yyflex_unput
  BEGIN( PRAGMA_ERROR );
}
<PRAGMA_DEFINE_START>. {
  std::string string;
  string += "at character ";
  string += yytext;
  string += " - identifier was expected";
  pproot()->cr_scanner->cur_message=string;
  yyflex_unput
  BEGIN( PRAGMA_ERROR );
}

<PRAGMA_DEFINE_NOARG>{Whitespace} {
  pproot()->cr_scanner->cur_char_position+=yyleng; 
}
<PRAGMA_DEFINE_NOARG>{newline} {
  pproot()->cr_scanner->cur_message="- `define: unexpected end-of-line)";
  yyflex_unput
  BEGIN( INITIAL );
  tkreturn(TK_DEFINE_END)
}
<PRAGMA_DEFINE_NOARG>{continuator} {
  p_continuator continuator;

  continuator=new s_continuator;
  continuator->char_position = yyleng-1;
  continuator->line_position = pproot()->cr_scanner->cur_line_position;

  pproot()->cr_scanner->cur_line_position += 1;
  pproot()->cr_scanner->cur_char_position = 1;
  pproot()->cr_scanner->cur_continuator_position.push_back(continuator);

  tkreturn(TK_CONTINUATOR)
}
<PRAGMA_DEFINE_NOARG>. {
  std::string string;
  string += "at character ";
  string += yytext;
  string += " - whitespace was expected";
  pproot()->cr_scanner->cur_message=string;
  yyflex_unput
  BEGIN( PRAGMA_DEFINE_TEXT_START );
}

<PRAGMA_DEFINE_WITHARG>\({whitespace}\) {
  pproot()->cr_scanner->cur_char_position+=yyleng; 
  BEGIN( PRAGMA_DEFINE_TEXT_START );
  tkreturn(TK_ARG_NULL)
}
<PRAGMA_DEFINE_WITHARG>\([^)]+{newline} {
  std::string string;
  string += "at character ";
  string += yytext;
  string += " - `define has no closing paren";
  pproot()->cr_scanner->cur_message=string;
  yyflex_unput
  BEGIN( PRAGMA_ERROR );
}
<PRAGMA_DEFINE_WITHARG>\({whitespace} {
  pproot()->cr_scanner->cur_char_position+=yyleng; 
  BEGIN( PRAGMA_DEFINE_WITHARG_ARGS );
}

<PRAGMA_DEFINE_WITHARG_ARGS>{ident} {
  BEGIN( PRAGMA_DEFINE_WITHARG_COMMA );
  preprocessorlval.mystr=strdup(yytext );
  tkreturn(TK_ARG)
}
<PRAGMA_DEFINE_WITHARG_ARGS>. {
  std::string string;
  string += "at character ";
  string += yytext;
  string += " - `define has unexpected syntax";
  pproot()->cr_scanner->cur_message=string;
  yyflex_unput
  BEGIN( PRAGMA_ERROR );
}

<PRAGMA_DEFINE_WITHARG_COMMA>, {
  pproot()->cr_scanner->cur_char_position+=yyleng; 
}
<PRAGMA_DEFINE_WITHARG_COMMA>\) {
  pproot()->cr_scanner->cur_char_position+=yyleng; 
  BEGIN( PRAGMA_DEFINE_TEXT_START );
}
<PRAGMA_DEFINE_WITHARG_COMMA>{Whitespace} {
  pproot()->cr_scanner->cur_char_position+=yyleng; 
}
<PRAGMA_DEFINE_WITHARG_COMMA>. {
  yyflex_unput
  BEGIN( PRAGMA_DEFINE_WITHARG_ARGS );
}

<PRAGMA_DEFINE_TEXT_START>{Whitespace} {
  pproot()->cr_scanner->cur_char_position+=yyleng; 
  BEGIN( PRAGMA_DEFINE_TEXT );
}
<PRAGMA_DEFINE_TEXT_START>{continuator} {
  p_continuator continuator;

  continuator=new s_continuator;
  continuator->char_position = yyleng-1;
  continuator->line_position = pproot()->cr_scanner->cur_line_position;

  pproot()->cr_scanner->cur_line_position += 1;
  pproot()->cr_scanner->cur_char_position = 1;
  pproot()->cr_scanner->cur_continuator_position.push_back(continuator);

  tkreturn(TK_CONTINUATOR)
}
<PRAGMA_DEFINE_TEXT_START>{newline} {
  yyflex_unput
  BEGIN( INITIAL );
  tkreturn(TK_DEFINE_END)
}
<PRAGMA_DEFINE_TEXT_START>. {
  yyflex_unput
  BEGIN( PRAGMA_DEFINE_TEXT );
}

<PRAGMA_DEFINE_TEXT>{Whitespace}/{newline} {
  pproot()->cr_scanner->cur_char_position+=yyleng; 
}

<PRAGMA_DEFINE_TEXT>`{ident}/[ ]*\( {

  preprocessorlval.mystr=strdup(adms_preprocessor_lex_remove_leading_quote( yytext ).c_str());
  if(DONT_SKIPP)
  {
    p_preprocessor_pragma_define Define;
    if ((Define = adms_preprocessor_pragma_define_exists (preprocessorlval.mystr)))
    {
      if(adms_preprocessor_pragma_define_has_noarg(Define))
        tkreturn(TK_SUBSTITUTOR_NOARG)
      else if(adms_preprocessor_pragma_define_has_nullarg(Define))
        tkreturn(TK_SUBSTITUTOR_NULLARG)
      else
        tkreturn(TK_SUBSTITUTOR_WITHARG)
    }
    else
    {
       preprocessorlval.mystr = strdup(yytext);
       pproot()->cr_scanner->cur_message=strdup(yytext);
       tkreturn(TK_ERROR_PRAGMA_NOT_FOUND)
    }
  }
  else
    tkreturn(TK_IDENT)
}

<PRAGMA_DEFINE_TEXT>`{ident} {

  preprocessorlval.mystr=strdup(adms_preprocessor_lex_remove_leading_quote( yytext ).c_str());
  if(DONT_SKIPP)
  {
    p_preprocessor_pragma_define Define;
    if((Define=adms_preprocessor_pragma_define_exists (preprocessorlval.mystr)))
    {
      if(adms_preprocessor_pragma_define_has_noarg(Define))
        tkreturn(TK_SUBSTITUTOR_NOARG)
      else if(adms_preprocessor_pragma_define_has_nullarg(Define))
        tkreturn(TK_SUBSTITUTOR_NULLARG_ALONE)
      else
        tkreturn(TK_SUBSTITUTOR_WITHARG_ALONE)
    }
    else
    {
       preprocessorlval.mystr=strdup(yytext);
       pproot()->cr_scanner->cur_message=yytext;
       tkreturn(TK_ERROR_PRAGMA_NOT_FOUND)
    }
  }
  else
    tkreturn(TK_IDENT)
}

<PRAGMA_DEFINE_TEXT>{ident} {

  preprocessorlval.mystr = strdup(yytext);
  tkreturn(TK_IDENT)
}

<PRAGMA_DEFINE_TEXT>\(  {
  preprocessorlval.mystr = strdup(yytext);
  tkreturn('(')
}

<PRAGMA_DEFINE_TEXT>\)  {
  preprocessorlval.mystr = strdup(yytext);
  tkreturn(')')
}

<PRAGMA_DEFINE_TEXT>\,  {
  preprocessorlval.mystr = strdup(yytext);
  tkreturn(',')
}

<PRAGMA_DEFINE_TEXT>{Whitespace} {
  pproot()->cr_scanner->cur_char_position+=yyleng; 
  preprocessorlval.mystr=strdup(yytext);
  return(TK_SPACE );
}

<PRAGMA_DEFINE_TEXT>{comment_oneline} {
  preprocessorlval.mystr=strdup(adms_preprocessor_lex_skipp_text().c_str()); return(TK_COMMENT);
}

<PRAGMA_DEFINE_TEXT>{comment} {
  preprocessorlval.mystr=strdup(adms_preprocessor_lex_skipp_text().c_str()); return(TK_COMMENT);
}

<PRAGMA_DEFINE_TEXT>.  {
  preprocessorlval.mystr = strdup(yytext);
  tkreturn(TK_NOT_IDENT)
}

<PRAGMA_DEFINE_TEXT>{continuator} {
  p_continuator continuator;

  continuator=new s_continuator;
  continuator->char_position = yyleng-1;
  continuator->line_position = pproot()->cr_scanner->cur_line_position;

  pproot()->cr_scanner->cur_line_position += 1;
  pproot()->cr_scanner->cur_char_position = 1;
  pproot()->cr_scanner->cur_continuator_position.push_back(continuator);

  tkreturn(TK_CONTINUATOR)
}
<PRAGMA_DEFINE_TEXT>{newline} {
  yyflex_unput
  BEGIN( INITIAL );
  tkreturn(TK_DEFINE_END)
}

<PRAGMA_NAME>{ident} {
  BEGIN( PRAGMA_COMMENT );
  preprocessorlval.mystr=strdup(yytext );
  tkreturn(TK_PRAGMA_NAME)
}
<PRAGMA_NAME>{Whitespace} {
  pproot()->cr_scanner->cur_char_position+=yyleng; 
}
<PRAGMA_NAME>{newline} {
  pproot()->cr_scanner->cur_message=strdup("- unexpected end-of-line");
  yyflex_unput
  BEGIN( PRAGMA_ERROR );
}
<PRAGMA_NAME>. {
  std::string string;
  string += "at character ";
  string += yytext;
  string += " - pragma name was expected";
  pproot()->cr_scanner->cur_message=string;
  yyflex_unput
  BEGIN( PRAGMA_ERROR );
}
<PRAGMA_NAME>{continuator} {
  p_continuator continuator;

  continuator=new s_continuator;
  continuator->char_position = yyleng-1;
  continuator->line_position = pproot()->cr_scanner->cur_line_position;

  pproot()->cr_scanner->cur_line_position += 1;
  pproot()->cr_scanner->cur_char_position = 1;
  pproot()->cr_scanner->cur_continuator_position.push_back(continuator);

  tkreturn(TK_CONTINUATOR)
}

<PRAGMA_PATH>{Whitespace} {
  pproot()->cr_scanner->cur_char_position+=yyleng; 
}
<PRAGMA_PATH>{xpath} {
  BEGIN( INITIAL );
  std::string fileName = adms_preprocessor_lex_get_path(yytext,'"','"');
  adms_preprocessor_lex_include_file(fileName.c_str());
  tkreturn(TK_INCLUDE)
}
<PRAGMA_PATH>{ipath} {
  BEGIN( INITIAL );
  std::string fileName = adms_preprocessor_lex_get_path(yytext,'<','>');
  adms_message_info("%s: implicit path handled as explicit path!",fileName.c_str());
  adms_preprocessor_lex_include_file(fileName.c_str());
  tkreturn(TK_INCLUDE)
}
<PRAGMA_PATH>. {
  std::string string;
  string += "at character ";
  string += yytext;
  string += " - bad path";
  pproot()->cr_scanner->cur_message=string;
  yyflex_unput
  BEGIN( PRAGMA_ERROR );
}

<PRAGMA_COMMENT>{Whitespace} {
  pproot()->cr_scanner->cur_char_position+=yyleng; 
}
<PRAGMA_COMMENT>{comment_oneline} {
  preprocessorlval.mystr=strdup(adms_preprocessor_lex_skipp_text().c_str()); return(TK_COMMENT);
}
<PRAGMA_COMMENT>{comment} {
  preprocessorlval.mystr=strdup(adms_preprocessor_lex_skipp_text().c_str()); return(TK_COMMENT);
}
<PRAGMA_COMMENT>{continuator} {
  p_continuator continuator;

  continuator=new s_continuator;
  continuator->char_position = yyleng-1;
  continuator->line_position = pproot()->cr_scanner->cur_line_position;

  pproot()->cr_scanner->cur_line_position += 1;
  pproot()->cr_scanner->cur_char_position = 1;
  pproot()->cr_scanner->cur_continuator_position.push_back(continuator);

  tkreturn(TK_CONTINUATOR)

}
<PRAGMA_COMMENT>{newline} {
  yyflex_unput
  BEGIN( INITIAL );
}
<PRAGMA_COMMENT>. {
  std::string string;
  string += "at character ";
  string += yytext;
  string += " - unknown error";
  pproot()->cr_scanner->cur_message=string;
  yyflex_unput
  BEGIN( PRAGMA_ERROR );
}

<PRAGMA_ERROR>.*{newline} {
  std::string string;
  string += "at character ";
  string += yytext;
  string += " - unknown error";
  pproot()->cr_scanner->cur_message=string;
  tkreturn(TK_ERROR_PRAGMA_DEFINITION)
}

`{ident}/[ ]*\( {

  p_preprocessor_pragma_define Define;
  preprocessorlval.mystr = 
    strdup(adms_preprocessor_lex_remove_leading_quote( yytext).c_str());
  if(DONT_SKIPP)
  {
    if ((Define = adms_preprocessor_pragma_define_exists (preprocessorlval.mystr)))
    {
      if(adms_preprocessor_pragma_define_has_noarg(Define))
        tkreturn(TK_SUBSTITUTOR_NOARG)
      else if(adms_preprocessor_pragma_define_has_nullarg(Define))
        tkreturn(TK_SUBSTITUTOR_NULLARG)
      else
        tkreturn(TK_SUBSTITUTOR_WITHARG)
    }
    else
    {
      preprocessorlval.mystr = strdup(yytext);
      pproot()->cr_scanner->cur_message=strdup(yytext);
      tkreturn(TK_ERROR_PRAGMA_NOT_FOUND)
    }
  }
  else
    tkreturn(TK_IDENT)

}

`{ident} {

  preprocessorlval.mystr = 
    strdup(adms_preprocessor_lex_remove_leading_quote( yytext ).c_str());
  {
    p_preprocessor_pragma_define Define;
    if(DONT_SKIPP)
    {
      if((Define=adms_preprocessor_pragma_define_exists (preprocessorlval.mystr)))
      {
        if(adms_preprocessor_pragma_define_has_noarg(Define))
          tkreturn(TK_SUBSTITUTOR_NOARG)
        else if(adms_preprocessor_pragma_define_has_nullarg(Define))
          tkreturn(TK_SUBSTITUTOR_NULLARG_ALONE)
        else
          tkreturn(TK_SUBSTITUTOR_WITHARG_ALONE)
      }
      else
      {
         preprocessorlval.mystr = strdup(yytext);
         pproot()->cr_scanner->cur_message=strdup(yytext);
         tkreturn(TK_ERROR_PRAGMA_NOT_FOUND)
      }
    }
    else
      tkreturn(TK_IDENT)
  }

}

{ident} {

  preprocessorlval.mystr = strdup(yytext);
  tkreturn(TK_IDENT)

}

\(  {
  preprocessorlval.mystr = strdup(yytext);
  return '(';
}

\)  {
  preprocessorlval.mystr = strdup(yytext);
  return ')';
}

\,  {
  preprocessorlval.mystr = strdup(yytext);
  return ',';
}

.  {
  preprocessorlval.mystr = strdup(yytext);
  return TK_NOT_IDENT;
}

<<EOF>> {
  std::string string;
  if( !pproot()->Scanner.empty() )
    {
      yy_delete_buffer((YY_BUFFER_STATE)(pproot()->cr_scanner->buffer));
      free(pproot()->cr_scanner );
      pproot()->cr_scanner = pproot()->Scanner.back();
      pproot()->Scanner.pop_back();
      yyin = pproot()->cr_scanner->fid;
      yy_switch_to_buffer((YY_BUFFER_STATE)pproot()->cr_scanner->buffer );
    }
  else
    yyterminate ();
  string += "# ";
  string += adms_integertostring(pproot()->cr_scanner->cur_line_position+1);
  string += " \"";
  string += pproot()->cr_scanner->filename;
  string += "\"";
  preprocessorlval.mystr=strdup(string.c_str());
  tkreturn(TK_EOF)
}

%%
int yywrap (void) {return 1;}

