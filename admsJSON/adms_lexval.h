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

#ifndef ADMS_LEXVAL_H
#define ADMS_LEXVAL_H
#include "adms.h"
/* ------- lexval -------------- */
struct s_lexval : s_adms {
  s_lexval (std::string mystring, std::string myf, int myl, int myc)
    : s_adms(admse_lexval), _string(mystring), _f(myf), _l(myl), _c(myc) {}

  std::string get_uid_implemented();
  void get_reference_list_implemented(references_list_t &rlist);
  void get_attribute_list_implemented(attributes_list_t &alist);
  void get_string_list_implemented(strings_list_t &slist);

  std::string _string;
  std::string _f;
  int _l;
  int _c;
};
p_lexval adms_lexval_new (std::string mystring,std::string myf,int myl,int myc);
void adms_lexval_free(p_lexval mylexval);

#endif
