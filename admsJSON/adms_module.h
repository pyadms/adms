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

#ifndef ADMS_MODULE_H
#define ADMS_MODULE_H
#include "adms.h"
/* ------- module -------------- */
struct s_module : public s_adms {
  s_module(std::string myname) : s_adms(admse_module), _name(myname), _analog(nullptr) {}

  std::string get_uid_implemented();
  void get_reference_list_implemented(references_list_t &rlist);
  void get_attribute_list_implemented(attributes_list_t &alist);
  void get_string_list_implemented(strings_list_t &slist);

  std::string  _name;
  node_list           _node;
  nodealias_list      _nodealias;
  branch_list         _branch;
  branchalias_list    _branchalias;
  analogfunction_list _analogfunction;
  instance_list       _instance;
  variableprototype_list       _variable;
  p_analog _analog;
  block_list          _block;
  blockvariable_list  _blockvariable;
  assignment_list     _assignment;
  callfunction_list   _callfunction;
  contribution_list   _contribution;
  conditional_list    _conditional;
  case_list           _case;
  forloop_list        _forloop;
  whileloop_list      _whileloop;
  expression_list     _expression;
  probe_list          _probe;
  source_list         _source;
  range_list          _range;
  attribute_list      _attribute;
};
p_module adms_module_new (std::string myname);
p_node adms_module_list_node_prepend_by_id_once_or_ignore (p_module mymymodule,p_module mymodule,std::string myname);
p_node adms_module_list_node_prepend_by_id_once_or_abort (p_module mymymodule,p_module mymodule,std::string myname);
p_node adms_module_list_node_lookup_by_id (p_module mymymodule,p_module mymodule,std::string myname);
p_nodealias adms_module_list_nodealias_prepend_by_id_once_or_abort (p_module mymymodule,p_module mymodule,std::string myname);
p_nodealias adms_module_list_nodealias_lookup_by_id (p_module mymymodule,p_module mymodule,std::string myname);
p_branch adms_module_list_branch_prepend_by_id_once_or_ignore (p_module mymymodule,p_module mymodule,p_node mypnode,p_node mynnode);
p_branchalias adms_module_list_branchalias_prepend_by_id_once_or_abort (p_module mymymodule,p_module mymodule,std::string myname);
p_branchalias adms_module_list_branchalias_lookup_by_id (p_module mymymodule,p_module mymodule,std::string myname);
p_instance adms_module_list_instance_prepend_by_id_once_or_abort (p_module mymymodule,p_module myinstantiator,p_module mymodule,std::string myname);
void adms_module_list_variable_prepend_once_or_abort (p_module mymymodule,p_variableprototype myvariable);
p_variableprototype adms_module_list_variable_lookup_by_id (p_module mymymodule,p_module mymodule,p_lexval mylexval,p_adms myblock);
p_probe adms_module_list_probe_prepend_by_id_once_or_ignore (p_module mymymodule,p_module mymodule,p_branch mybranch,p_nature mynature);
p_source adms_module_list_source_prepend_by_id_once_or_ignore (p_module mymymodule,p_module mymodule,p_branch mybranch,p_nature mynature);
p_range adms_module_list_range_prepend_by_id_once_or_abort (p_module mymymodule,p_module mymodule,p_expression myinfexpr,p_expression mysupexpr);
void adms_module_list_analogfunction_prepend_once_or_abort (p_module mymymodule,p_analogfunction myanalogfunction);

#endif

