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

#ifndef ADMS_ANALOGFUNCTION_H
#define ADMS_ANALOGFUNCTION_H

#include "adms.h"
#include "adms_lexval.h"

/* ------- analogfunction -------------- */
struct s_analogfunction : public s_adms {
  s_analogfunction(p_module mymodule, p_lexval mylexval)
    : s_adms(admse_analogfunction), _module(mymodule), _lexval(mylexval), _type(admse_real), _tree(nullptr) {}

  std::string get_uid_implemented();

  void get_reference_list_implemented(references_list_t &rlist);
  void get_attribute_list_implemented(attributes_list_t &alist);
  void get_string_list_implemented(strings_list_t &slist);

  p_module _module;
  p_lexval _lexval;
  admse _type;
  p_adms _tree;
  variableprototype_list _variable;
  attribute_list         _attribute;
};
std::string adms_analogfunction_uid (p_analogfunction myanalogfunction);
p_analogfunction adms_analogfunction_new (p_module mymodule,p_lexval mylexval);
void adms_analogfunction_list_variable_prepend_once_or_abort (p_analogfunction mymyanalogfunction,p_variableprototype myvariable);
p_variableprototype adms_analogfunction_list_variable_lookup_by_id (p_analogfunction mymyanalogfunction,p_module mymodule,p_lexval mylexval,p_adms myblock);
int adms_analogfunction_cmp (p_analogfunction myanalogfunction,p_analogfunction refanalogfunction);
#endif

