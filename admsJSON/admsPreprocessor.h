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

#ifndef _preprocessor_main_included
#define _preprocessor_main_included

#include "adms.h"
#include "adms_slist.h"
#include <string>
#include <list>


#define INT2ADMS(a) ((p_adms)(long)(a))
struct s_preprocessor;
typedef s_preprocessor *p_preprocessor;

struct s_preprocessor_pragma_define_text;
typedef s_preprocessor_pragma_define_text *p_preprocessor_pragma_define_text;

struct s_preprocessor_pragma_define;
typedef s_preprocessor_pragma_define *p_preprocessor_pragma_define;

struct s_preprocessor_substitutor;
typedef s_preprocessor_substitutor *p_preprocessor_substitutor;

struct s_preprocessor_text;
typedef s_preprocessor_text *p_preprocessor_text;

struct s_preprocessor_main;
typedef s_preprocessor_main *p_preprocessor_main;

struct s_continuator;
typedef s_continuator *p_continuator;

void adms_preprocessor_get_define_from_argv(const int argc,const char** argv);

int adms_preprocessor_getint_yydebug (void);
void adms_preprocessor_setint_yydebug (const int val);

int adms_preprocessor_registration_printf (const char *filename);

struct s_preprocessor {
  s_preprocessor() : buffer(nullptr), fid(nullptr), cur_line_position(1), cur_char_position(1) {
  }
  void *buffer;
  std::string filename;
  FILE * fid;
  int cur_line_position;
  int cur_char_position;
  std::string cur_message;
  std::list<p_continuator> cur_continuator_position;
};

struct s_preprocessor_pragma_define_text {
  std::string name;
  int isArg;
};

struct s_preprocessor_pragma_define {
  std::string name;
  int hasArg;
  int isDefined;
  p_slist arg;
  p_slist text;
};

struct s_preprocessor_substitutor {
  p_preprocessor_pragma_define _pragma;
  p_slist _newarg;
  p_slist _newtext;
  p_preprocessor_text _cr_text;
  p_slist _cr_newarg;
};

struct s_preprocessor_text {
  int _isarg;
  std::string _str;
};

struct s_preprocessor_main {
  p_preprocessor cr_scanner;
  std::string cr_filename;
  std::list<p_preprocessor> Scanner;
  p_slist Text;
  p_slist Defined;
  std::list<std::string> includePath;
  p_slist skipp_text;
  int error;
};

p_preprocessor_main pproot (void);
void adms_preprocessor_valueto_main (p_preprocessor_main mypreprocessor_main);

void preprocessorerror (const char *s);
int preprocessorlex ();
int preprocessorparse ();

struct s_continuator {
  int line_position;
  int char_position;
};

int adms_preprocessor_get_char_position (p_preprocessor cr_preprocessor);
int adms_preprocessor_get_line_position (p_preprocessor cr_preprocessor, int char_pos);

int adms_preprocessor_add_message ( std::string message);

p_preprocessor_pragma_define adms_preprocessor_define_add (std::string name);
int adms_preprocessor_define_add_default (std::string name);
int adms_preprocessor_define_add_default_with_text (std::string name, std::string value);
p_preprocessor_pragma_define adms_preprocessor_undefine_add (char * name);
p_preprocessor_pragma_define adms_preprocessor_define_add_with_text (std::string name, p_slist text);
p_preprocessor_pragma_define adms_preprocessor_define_add_with_arg (char * name, p_slist arg);
p_preprocessor_pragma_define adms_preprocessor_define_add_with_arg_and_text (char * name, p_slist arg, p_slist text);

p_preprocessor_pragma_define adms_preprocessor_pragma_define_exists (std::string name);

int adms_preprocessor_identifier_is_def (char * name);
int adms_preprocessor_identifier_is_ndef (char * name);

p_preprocessor_pragma_define adms_preprocessor_identifer_set_undef (char * name);

int adms_preprocessor_pragma_define_is_def (p_preprocessor_pragma_define pragma);
int adms_preprocessor_pragma_define_is_ndef (p_preprocessor_pragma_define pragma);
int adms_preprocessor_pragma_define_has_arg (p_preprocessor_pragma_define pragma);
int adms_preprocessor_pragma_define_has_noarg (p_preprocessor_pragma_define pragma);
int adms_preprocessor_pragma_define_has_nullarg (p_preprocessor_pragma_define pragma);
int adms_preprocessor_pragma_define_has_text (p_preprocessor_pragma_define pragma);
int adms_preprocessor_pragma_define_has_notext (p_preprocessor_pragma_define pragma);
int adms_preprocessor_pragma_define_has_noarg_and_notext (p_preprocessor_pragma_define pragma);
int adms_preprocessor_pragma_define_has_noarg_and_text (p_preprocessor_pragma_define pragma);
int adms_preprocessor_pragma_define_has_nullarg_and_notext (p_preprocessor_pragma_define pragma);
int adms_preprocessor_pragma_define_has_nullarg_and_text (p_preprocessor_pragma_define pragma);
int adms_preprocessor_pragma_define_has_arg_and_notext (p_preprocessor_pragma_define pragma);
int adms_preprocessor_pragma_define_has_arg_and_text (p_preprocessor_pragma_define pragma);

p_preprocessor_text adms_preprocessor_new_text_as_string(std::string name);
p_slist adms_preprocessor_new_text_as_substitutor (p_preprocessor_pragma_define define, p_slist newarg);

void adms_preprocessor_setfile_input (FILE *ifile);
FILE *adms_preprocessor_getfile_input (void);
void adms_preprocessor_setfile_output (FILE *ifile);
FILE *adms_preprocessor_getfile_output (void);

#endif /* _preprocessor_main_included */

