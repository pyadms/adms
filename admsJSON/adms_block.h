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

#ifndef ADMS_BLOCK_H
#define ADMS_BLOCK_H
#include "adms.h"
#include "adms_math.h"

/* ------- blockvariable -------------- */
struct s_blockvariable : public s_math {
  s_blockvariable (p_block myblock) :
    s_math(admse_blockvariable), _block(myblock) {}

  std::string get_uid_implemented();
  void get_reference_list_implemented(references_list_t &rlist);
  void get_attribute_list_implemented(attributes_list_t &alist);
  void get_string_list_implemented(strings_list_t &slist);

  p_block _block;
  variableprototype_list _variable;
};

p_blockvariable adms_blockvariable_new (p_block myblock);

/* ------- block -------------- */
struct s_block : public s_math {
  s_block (p_module mymodule,p_lexval mylexval,p_adms myblock,p_adms_list myitem)
    : s_math(admse_block), _module(mymodule), _lexval(mylexval), _block(myblock), _item(myitem) {}

  std::string get_uid_implemented();
  void get_reference_list_implemented(references_list_t &rlist);
  void get_attribute_list_implemented(attributes_list_t &alist);
  void get_string_list_implemented(strings_list_t &slist);

  p_module _module;
  p_lexval _lexval;
  /// can be block, module or analog function
  p_adms _block;
  p_adms_list            _item;
  attribute_list         _attribute;
  variableprototype_list _variable;
};
p_block adms_block_new (p_module mymodule,p_lexval mylexval,p_adms myblock, p_adms_list myitem);
void adms_block_list_variable_prepend_once_or_abort (p_block mymyblock,p_variableprototype myvariable);

#endif
