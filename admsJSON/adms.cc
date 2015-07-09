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


/* ------- code automatically created by ./mkelements.pl -------------- */

#include "adms.h"
#include "adms_math.h"
#include "adms_instance.h"
#include "adms_attribute.h"
#include "adms_admsmain.h"
#include "adms_yaccval.h"
#include "adms_lexval.h"
#include "adms_variable.h"
#include "adms_math.h"
#include "adms_source.h"
#include "adms_simulator.h"
#include "adms_module.h"
#include "adms_range.h"
#include "adms_node.h"
#include "adms_analogfunction.h"
#include "adms_analog.h"
#include "adms_block.h"
#include <cstdlib>
#include <limits>
#include <map>



void s_adms::get_reference_list(references_list_t &rlist)
{
  get_reference_list_implemented(rlist);
  if (p_math p = dynamic_cast<p_math>(this))
  {
    p->get_reference_list_math(rlist);
  }
}

void s_adms::get_attribute_list(attributes_list_t &alist)
{
  get_attribute_list_implemented(alist);
  if (p_math p = dynamic_cast<p_math>(this))
  {
    p->get_attribute_list_math(alist);
  }
}

void s_adms::get_string_list(strings_list_t &slist)
{
  get_string_list_implemented(slist);
  if (p_math p = dynamic_cast<p_math>(this))
  {
    p->get_string_list_math(slist);
  }
  const std::string &datatypename = get_admse_string(_datatypename);
}



/*-- Miscellaneous routines --*/
std::string adms_integertostring (int value)
{
  return std::to_string(value);
}


int adms_file_isregular (std::string myfilename)
{
  return((access(myfilename.c_str(),F_OK)==0));
}
int adms_file_isdirectory (std::string myfilename)
{
  struct stat s;
  return((stat(myfilename.c_str(),&s)==0)&&(s.st_mode&S_IFDIR));
}

bool is_valid_scaling_unit(const std::string &lv)
{
  return
  (lv == "E") ||
  (lv == "P") ||
  (lv == "T") ||
  (lv == "G") ||
  (lv == "M") ||
  (lv == "k") ||
  (lv == "h") ||
  (lv == "D") ||
  (lv == "d") ||
  (lv == "c") ||
  (lv == "m") ||
  (lv == "u") ||
  (lv == "n") ||
  (lv == "A") ||
  (lv == "p") ||
  (lv == "f") ||
  (lv == "a") ;
}

void push_back_attributes(attributes_list_t &alist, attribute_list &attribute)
{
  for (auto a : attribute) {
    alist.push_back(mytup2{a->_name, a->_value});
  }
}

std::string get_admse_string(admse x) {
static std::map<admse, std::string>  m= {
  {admse_simulator, "simulator"},
  {admse_admsmain, "admsmain"},
  {admse_list, "list"},
  {admse_nature, "nature"},
  {admse_discipline, "discipline"},
  {admse_module, "module"},
  {admse_analogfunction, "analogfunction"},
  {admse_nodealias, "nodealias"},
  {admse_node, "node"},
  {admse_branchalias, "branchalias"},
  {admse_branch, "branch"},
  {admse_variableprototype, "variableprototype"},
  {admse_source, "source"},
  {admse_range, "range"},
  {admse_analog, "analog"},
  {admse_string, "string"},
  {admse_mapply_unary, "mapply_unary"},
  {admse_mapply_binary, "mapply_binary"},
  {admse_mapply_ternary, "mapply_ternary"},
  {admse_number, "number"},
  {admse_function, "function"},
  {admse_variable, "variable"},
  {admse_array, "array"},
  {admse_probe, "probe"},
  {admse_expression, "expression"},
  {admse_instance, "instance"},
  {admse_instanceparameter, "instanceparameter"},
  {admse_instancenode, "instancenode"},
  {admse_nilled, "nilled"},
  {admse_assignment, "assignment"},
  {admse_contribution, "contribution"},
  {admse_conditional, "conditional"},
  {admse_forloop, "forloop"},
  {admse_whileloop, "whileloop"},
  {admse_case, "case"},
  {admse_caseitem, "caseitem"},
  {admse_blockvariable, "blockvariable"},
  {admse_block, "block"},
  {admse_callfunction, "callfunction"},
  {admse_attribute, "attribute"},
  {admse_lexval, "lexval"},
  {admse_yaccval, "yaccval"},
  {admse_input, "input"},
  {admse_output, "output"},
  {admse_yes, "yes"},
  {admse_no, "no"},
  {admse_continuous, "continuous"},
  {admse_discrete, "discrete"},
  {admse_real, "real"},
  {admse_integer, "integer"},
  {admse_inout, "inout"},
  {admse_internal, "internal"},
  {admse_ground, "ground"},
  {admse_external, "external"},
  {admse_scalar, "scalar"},
  {admse_model, "model"},
  {admse_range_bound_include, "range_bound_include"},
  {admse_range_bound_exclude, "range_bound_exclude"},
  {admse_range_bound_value, "range_bound_value"},
  {admse_include, "include"},
  {admse_include_value, "include_value"},
  {admse_exclude, "exclude"},
  {admse_exclude_value, "exclude_value"},
  {admse_plus, "plus"},
  {admse_minus, "minus"}
};
  return m[x];
}

template <typename T>
void push_back_mytup1(references_list_t &rlist, std::string name, std::list<T> &list)
{
  p_adms_list x;
  std::copy(list.begin(), list.end(), std::back_inserter(x));
  rlist.push_back(mytup1{name, x});
}

template void push_back_mytup1(references_list_t &, std::string, std::list<s_adms *> &);
template void push_back_mytup1(references_list_t &, std::string, std::list<s_admsmain *> &);
template void push_back_mytup1(references_list_t &, std::string, std::list<s_analog *> &);
template void push_back_mytup1(references_list_t &, std::string, std::list<s_analogfunction *> &);
template void push_back_mytup1(references_list_t &, std::string, std::list<s_array *> &);
template void push_back_mytup1(references_list_t &, std::string, std::list<s_assignment *> &);
template void push_back_mytup1(references_list_t &, std::string, std::list<s_attribute *> &);
template void push_back_mytup1(references_list_t &, std::string, std::list<s_block *> &);
template void push_back_mytup1(references_list_t &, std::string, std::list<s_blockvariable *> &);
template void push_back_mytup1(references_list_t &, std::string, std::list<s_branch *> &);
template void push_back_mytup1(references_list_t &, std::string, std::list<s_branchalias *> &);
template void push_back_mytup1(references_list_t &, std::string, std::list<s_callfunction *> &);
template void push_back_mytup1(references_list_t &, std::string, std::list<s_case *> &);
template void push_back_mytup1(references_list_t &, std::string, std::list<s_caseitem *> &);
template void push_back_mytup1(references_list_t &, std::string, std::list<s_conditional *> &);
//template void push_back_mytup1(references_list_t &, std::string, std::list<s_continuator *> &);
template void push_back_mytup1(references_list_t &, std::string, std::list<s_contribution *> &);
template void push_back_mytup1(references_list_t &, std::string, std::list<s_discipline *> &);
template void push_back_mytup1(references_list_t &, std::string, std::list<s_expression *> &);
template void push_back_mytup1(references_list_t &, std::string, std::list<s_forloop *> &);
template void push_back_mytup1(references_list_t &, std::string, std::list<s_function *> &);
template void push_back_mytup1(references_list_t &, std::string, std::list<s_instance *> &);
template void push_back_mytup1(references_list_t &, std::string, std::list<s_instancenode *> &);
template void push_back_mytup1(references_list_t &, std::string, std::list<s_instanceparameter *> &);
template void push_back_mytup1(references_list_t &, std::string, std::list<s_lexval *> &);
//template void push_back_mytup1(references_list_t &, std::string, std::list<s_list *> &);
template void push_back_mytup1<s_mapply_binary *>(references_list_t &, std::string, std::list<s_mapply_binary *> &);
template void push_back_mytup1<s_mapply_ternary *>(references_list_t &, std::string, std::list<s_mapply_ternary *> &);
template void push_back_mytup1<s_mapply_unary *>(references_list_t &, std::string, std::list<s_mapply_unary *> &);
template void push_back_mytup1(references_list_t &, std::string, std::list<s_math *> &);
template void push_back_mytup1(references_list_t &, std::string, std::list<s_module *> &);
template void push_back_mytup1(references_list_t &, std::string, std::list<s_nature *> &);
template void push_back_mytup1(references_list_t &, std::string, std::list<s_nilled *> &);
template void push_back_mytup1(references_list_t &, std::string, std::list<s_node *> &);
template void push_back_mytup1(references_list_t &, std::string, std::list<s_nodealias *> &);
template void push_back_mytup1(references_list_t &, std::string, std::list<s_number *> &);
//template void push_back_mytup1(references_list_t &, std::string, std::list<s_path *> &);
//template void push_back_mytup1(references_list_t &, std::string, std::list<s_pparse *> &);
//template void push_back_mytup1(references_list_t &, std::string, std::list<s_preprocessor *> &);
//template void push_back_mytup1<s_preprocessor_main *>(references_list_t &, std::string, std::list<s_preprocessor_main *> &);
//template void push_back_mytup1<s_preprocessor_pragma_define *>(references_list_t &, std::string, std::list<s_preprocessor_pragma_define *> &);
//template void push_back_mytup1<s_preprocessor_pragma_define_text *>(references_list_t &, std::string, std::list<s_preprocessor_pragma_define_text *> &);
//template void push_back_mytup1<s_preprocessor_substitutor *>(references_list_t &, std::string, std::list<s_preprocessor_substitutor *> &);
//template void push_back_mytup1<s_preprocessor_text *>(references_list_t &, std::string, std::list<s_preprocessor_text *> &);
template void push_back_mytup1(references_list_t &, std::string, std::list<s_probe *> &);
//template void push_back_mytup1(references_list_t &, std::string, std::list<s_ptraverse *> &);
template void push_back_mytup1(references_list_t &, std::string, std::list<s_range *> &);
template void push_back_mytup1(references_list_t &, std::string, std::list<s_simulator *> &);
//template void push_back_mytup1(references_list_t &, std::string, std::list<s_slist *> &);
template void push_back_mytup1(references_list_t &, std::string, std::list<s_source *> &);
template void push_back_mytup1(references_list_t &, std::string, std::list<s_string *> &);
//template void push_back_mytup1(references_list_t &, std::string, std::list<s_text *> &);
template void push_back_mytup1(references_list_t &, std::string, std::list<s_variable *> &);
template void push_back_mytup1(references_list_t &, std::string, std::list<s_variableprototype *> &);
template void push_back_mytup1(references_list_t &, std::string, std::list<s_whileloop *> &);
template void push_back_mytup1(references_list_t &, std::string, std::list<s_yaccval *> &);


template <typename T>
std::list<p_adms> create_adms_list(T x)
{
  return std::list<p_adms>{x};
}

template std::list<p_adms> create_adms_list(p_adms x);
template std::list<p_adms> create_adms_list(p_analogfunction x);
template std::list<p_adms> create_adms_list(p_assignment x);
template std::list<p_adms> create_adms_list(p_block x);
template std::list<p_adms> create_adms_list(p_branch x);
template std::list<p_adms> create_adms_list(p_branchalias x);
template std::list<p_adms> create_adms_list(p_discipline x);
template std::list<p_adms> create_adms_list(p_expression x);
template std::list<p_adms> create_adms_list(p_function x);
template std::list<p_adms> create_adms_list(p_instanceparameter x);
template std::list<p_adms> create_adms_list(p_lexval x);
template std::list<p_adms> create_adms_list(p_module x);
template std::list<p_adms> create_adms_list(p_nature x);
template std::list<p_adms> create_adms_list(p_node x);
template std::list<p_adms> create_adms_list(p_simulator x);
template std::list<p_adms> create_adms_list(p_source x);
template std::list<p_adms> create_adms_list(p_variable x);
template std::list<p_adms> create_adms_list(p_variableprototype x);
template std::list<p_adms> create_adms_list(p_analog x);
template std::list<p_adms> create_adms_list(p_number x);


