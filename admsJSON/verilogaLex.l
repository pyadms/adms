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

#include "adms_lexval.h"
#include "admsVeriloga.h"
#include "verilogaYacc.hh"
#include "adms_message.h"
#include "adms_admsmain.h"
#include <cassert>
#include <string>

#ifdef _WIN32
#include <io.h>
#define YY_NO_UNISTD_H
#else
#include <unistd.h>
#endif


// remove deprecated warning from Apple c++ compiler
#define register

static int adms_strtointeger (const char *mystr)
{
  int val;
  errno=0;
  val=(int)strtol(mystr,NULL,10);
  if(errno)
    adms_message_fatal("%s: strtoint conversion failed\n",mystr);
  return val;
}
void adms_veriloga_setfile_input (FILE *ifile)
{
  yyin=ifile;
}
void verilogaerror (const char *s)
{
  adms_message_fatal("%s: during lexical analysis %s at line %i -- see '%s'\n",root()->_curfilename.c_str(),s,root()->_curline,verilogatext);
}
void POS_UPDATE(const int myyyleng,const char*myyytext)
{
  int c=root()->_fpos;
  adms_message_dbg_vla("%s:%i:%i-%i read token '%s'\n",root()->_curfilename.c_str(),
    root()->_curline,c+1,c+myyyleng,myyytext);
  root()->_fpos = c+myyyleng;
}
void TKRETURN(const char*myyytext, const int myyyleng)
{
  verilogalval._lexval=adms_lexval_new(myyytext,root()->_curfilename,root()->_curline,root()->_fpos+1);
  POS_UPDATE(myyyleng,myyytext);
}
void TKSTRIPPEDRETURN(const char*myyytext, const int myyyleng)
{
  int mynewlen=strlen(myyytext)-2;
  assert(mynewlen>=0);

  std::string mystrippedstring = myyytext;
  mystrippedstring = mystrippedstring.substr(1, mynewlen);

  POS_UPDATE(myyyleng,myyytext);
  verilogalval._lexval=adms_lexval_new(
    mystrippedstring,
    root()->_curfilename,
    root()->_curline,
    root()->_fpos
  );
}
#ifdef YYDEBUG
extern int yydebug;
#endif

%}

whitespace0 [ \t\v\f]*
whitespace [ \t\v\f]+
newline    \r?\n
wn [ \t\v\f\r?\n]
wn0 [ \t\v\f\r?\n]*
wn1 [ \t\v\f\r?\n]+
ident      [A-Za-z_][A-Za-z0-9_]*
attribute  [A-Za-z_][A-Za-z0-9_:.]*
attributevalue {wn0}={wn0}{anystring} 
anystring  \"[^\"]*\"
b8_int     0[0-9]*
b10_uint   [1-9][0-9]*
b16_int    0[xX][0-9A-Fa-f]+
float      [0-9]*\.[0-9]+([eE][+-]?[0-9]+)?|[0-9]+\.?([eE][+-]?[0-9]+)?
char       \'[^\']*[\'\n]

%x insideAttribute 
%x insideAttributeBegin 
%x insideAttributeEqual 
%x insideAttributeValue 
%x insideAttributeEnd 
%x insidePragma 
%option nounput

%%

{newline} {
  root()->_curline=root()->_curline+1;
  root()->_fpos=0;
}

^#{whitespace} {BEGIN(insidePragma);}
{whitespace} {POS_UPDATE(yyleng,yytext);}

<insidePragma>{
{anystring} {
  int mynewlen=strlen(yytext)-2;
  assert(mynewlen>=0);

  std::string mystrippedstring=yytext;
  mystrippedstring = mystrippedstring.substr(1, mynewlen);
  
  root()->_curfilename = mystrippedstring;
}
{b10_uint} {
    root()->_curline=adms_strtointeger(yytext);
    root()->_fpos=0;
}
{newline} {
  BEGIN(INITIAL);
  root()->_fpos=0;
}
{whitespace} {POS_UPDATE(yyleng,yytext);}
. {
  adms_message_fatal("[%s:%i]: inside pragma declaration unexpected character at [%s]\n",root()->_curfilename.c_str(),root()->_curline,yytext);
}
}

{anystring} {TKSTRIPPEDRETURN(yytext,yyleng); return tk_anystring;}

"(*"{wn0} {BEGIN(insideAttribute); {TKRETURN(yytext,yyleng); return tk_beginattribute;}}

<insideAttribute>{
"*)" {BEGIN(INITIAL);TKRETURN(yytext,yyleng);return tk_endattribute;}
{attribute}{attributevalue} {BEGIN(insideAttributeBegin);yyless(0);}
. {
    char myyytext[1000];
    int i=0;
    myyytext[i]=yytext[0];
    for(;;)
    {
      while((myyytext[i]!='*')&&(myyytext[i]!=EOF))
      {
        myyytext[++i]=yyinput();
      }
      if(myyytext[i]=='*')
      {
        while((myyytext[++i]=yyinput())=='*')
        ;
        if(myyytext[i]==')')
        {
          BEGIN(INITIAL);
          i-=2;
          while(myyytext[i]==' ')
            i--;
          myyytext[i+1]='\0';
          POS_UPDATE(yyleng,yytext);
          verilogalval._lexval=adms_lexval_new(
            myyytext,
            root()->_curfilename,
            root()->_curline,
            root()->_fpos
          );
          return tk_anytext;
        }
      }
      if(myyytext[i]==EOF)
      {
        adms_message_fatal("[%s:%i]: inside Verilog-ams EOF found in attribute declaration [%s]\n",root()->_curfilename.c_str(),root()->_curline,yytext);
      }
    }
}
}
<insideAttributeBegin>{
{attribute} {BEGIN(insideAttributeEqual);TKRETURN(yytext,yyleng);return tk_ident;}
. {
  adms_message_fatal("[%s:%i]: inside Verilog-ams attribute declaration unexpected character at [%s]\n",root()->_curfilename.c_str(),root()->_curline,yytext);
}
}
<insideAttributeEqual>{
{wn0}={wn0} {BEGIN(insideAttributeValue);return '=';}
. {
  adms_message_fatal("[%s:%i]: inside Verilog-ams attribute declaration unexpected character at [%s]\n",root()->_curfilename.c_str(),root()->_curline,yytext);
}
}
<insideAttributeValue>{
{anystring} {TKSTRIPPEDRETURN(yytext,yyleng);BEGIN(insideAttributeEnd);return tk_anystring;}
. {
  adms_message_fatal("[%s:%i]: inside Verilog-ams attribute declaration unexpected character at [%s]\n",root()->_curfilename.c_str(),root()->_curline,yytext);
}
}
<insideAttributeEnd>{
{wn0}"*)" {BEGIN(INITIAL);TKRETURN(yytext,yyleng);return tk_endattribute;}
{wn1} {BEGIN(insideAttributeBegin);POS_UPDATE(yyleng,yytext);}
{wn0},{wn0} {BEGIN(insideAttributeBegin);POS_UPDATE(yyleng,yytext);}
{attribute}{attributevalue} {BEGIN(insideAttributeBegin);yyless(0);}
. {
  adms_message_fatal("[%s:%i]: inside Verilog-ams attribute declaration unexpected character at [%s]\n",root()->_curfilename.c_str(),root()->_curline,yytext);
}
}

parameter {TKRETURN(yytext,yyleng); return tk_parameter;}
aliasparameter {TKRETURN(yytext,yyleng); return tk_aliasparameter;}
aliasparam {TKRETURN(yytext,yyleng); return tk_aliasparam;}
module {TKRETURN(yytext,yyleng); return tk_module;}
endmodule {TKRETURN(yytext,yyleng); return tk_endmodule;}
function {TKRETURN(yytext,yyleng); return tk_function;}
endfunction {TKRETURN(yytext,yyleng); return tk_endfunction;}
discipline {TKRETURN(yytext,yyleng); return tk_discipline;}
potential {TKRETURN(yytext,yyleng); return tk_potential;}
flow {TKRETURN(yytext,yyleng); return tk_flow;}
domain {TKRETURN(yytext,yyleng); return tk_domain;}
ground {TKRETURN(yytext,yyleng); return tk_ground;}
enddiscipline {TKRETURN(yytext,yyleng); return tk_enddiscipline;}
nature {TKRETURN(yytext,yyleng); return tk_nature;}
endnature {TKRETURN(yytext,yyleng); return tk_endnature;}
input {TKRETURN(yytext,yyleng); return tk_input;}
output {TKRETURN(yytext,yyleng); return tk_output;}
inout {TKRETURN(yytext,yyleng); return tk_inout;}
branch {TKRETURN(yytext,yyleng); return tk_branch;}
analog {TKRETURN(yytext,yyleng); return tk_analog;}
begin {TKRETURN(yytext,yyleng); return tk_begin;}
end {TKRETURN(yytext,yyleng); return tk_end;}
if {TKRETURN(yytext,yyleng); return tk_if;}
while {TKRETURN(yytext,yyleng); return tk_while;}
case {TKRETURN(yytext,yyleng); return tk_case;}
endcase {TKRETURN(yytext,yyleng); return tk_endcase;}
default {TKRETURN(yytext,yyleng); return tk_default;}
for {TKRETURN(yytext,yyleng); return tk_for;}
else {TKRETURN(yytext,yyleng); return tk_else;}
integer {TKRETURN(yytext,yyleng); return tk_integer;}
real {TKRETURN(yytext,yyleng); return tk_real;}
string {TKRETURN(yytext,yyleng); return tk_string;}
from {TKRETURN(yytext,yyleng); return tk_from;}
exclude {TKRETURN(yytext,yyleng); return tk_exclude;}
inf {TKRETURN(yytext,yyleng); return tk_inf;}
INF {TKRETURN(yytext,yyleng); return tk_inf;}

{ident} {TKRETURN(yytext,yyleng); return tk_ident;}

\>\> {TKRETURN(yytext,yyleng); return tk_op_shr;}
\<\< {TKRETURN(yytext,yyleng); return tk_op_shl;}
\&\& {TKRETURN(yytext,yyleng); return tk_and;}
\|\| {TKRETURN(yytext,yyleng); return tk_or;}
\^\~ {TKRETURN(yytext,yyleng); return tk_bitwise_equr;}

\${ident} {TKRETURN(yytext,yyleng); return tk_dollar_ident;}
{char} {TKSTRIPPEDRETURN(yytext,yyleng); return tk_char;}
{b8_int} {TKRETURN(yytext,yyleng); return tk_integer;}
{b10_uint} {TKRETURN(yytext,yyleng); return tk_integer;}
{b16_int} {TKRETURN(yytext,yyleng); return tk_integer;}
{float} {TKRETURN(yytext,yyleng); return tk_number;}

. {POS_UPDATE(yyleng,yytext);return yytext[0];}

%%

int yywrap (void) {return 1;}
