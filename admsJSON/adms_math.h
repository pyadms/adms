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

#ifndef ADMS_MATH_H
#define ADMS_MATH_H
#include "adms.h"

/* ------- math -------------- */
struct s_math : public s_adms {
  s_math(admse mydatatypename) : s_adms(mydatatypename)
 {}

  void get_reference_list_math(references_list_t &rlist) {}
  void get_attribute_list_math(attributes_list_t &alist) {}
  void get_string_list_math(strings_list_t &slist) {}
};

/* ------- string -------------- */
struct s_string : public s_math {
  s_string(std::string myvalue) : 
    s_math(admse_string)
    , _value(myvalue)
    {}

  std::string get_uid_implemented();
  void get_reference_list_implemented(references_list_t &rlist);
  void get_attribute_list_implemented(attributes_list_t &alist);
  void get_string_list_implemented(strings_list_t &slist);

  std::string _value;
};

p_string adms_string_new (std::string myvalue);

/* ------- mapply_unary -------------- */
struct s_mapply_unary : public s_math {
  s_mapply_unary(std::string myname, p_adms myarg1)
    : s_math(admse_mapply_unary), _name(myname), _arg1(myarg1) {}

  std::string get_uid_implemented();
  void get_reference_list_implemented(references_list_t &rlist);
  void get_attribute_list_implemented(attributes_list_t &alist);
  void get_string_list_implemented(strings_list_t &slist);

  std::string _name;
  p_adms _arg1;
};

p_mapply_unary adms_mapply_unary_new (std::string myname,p_adms myarg1);

/* ------- mapply_binary -------------- */
struct s_mapply_binary : public s_math {
  s_mapply_binary(std::string myname,p_adms myarg1,p_adms myarg2)
    : s_math(admse_mapply_binary), _name(myname), _arg1(myarg1), _arg2(myarg2) {}

  std::string get_uid_implemented();
  void get_reference_list_implemented(references_list_t &rlist);
  void get_attribute_list_implemented(attributes_list_t &alist);
  void get_string_list_implemented(strings_list_t &slist);

  std::string _name;
  p_adms _arg1;
  p_adms _arg2;
};

p_mapply_binary adms_mapply_binary_new (std::string myname,p_adms myarg1,p_adms myarg2);

/* ------- mapply_ternary -------------- */
struct s_mapply_ternary : public s_math {
  s_mapply_ternary(std::string myname,p_adms myarg1,p_adms myarg2, p_adms myarg3)
    : s_math(admse_mapply_ternary), _name(myname), _arg1(myarg1), _arg2(myarg2), _arg3(myarg3) {}

  std::string get_uid_implemented();
  void get_reference_list_implemented(references_list_t &rlist);
  void get_attribute_list_implemented(attributes_list_t &alist);
  void get_string_list_implemented(strings_list_t &slist);

  std::string _name;
  p_adms _arg1;
  p_adms _arg2;
  p_adms _arg3;
};
p_mapply_ternary adms_mapply_ternary_new (std::string myname,p_adms myarg1,p_adms myarg2,p_adms myarg3);

/* ------- number -------------- */
struct s_number : public s_math {
  s_number (p_lexval mylexval) : s_math(admse_number), _lexval(mylexval),
    _scalingunit("1"), _cast("d")
  {}

  std::string get_uid_implemented();
  void get_reference_list_implemented(references_list_t &rlist);
  void get_attribute_list_implemented(attributes_list_t &alist);
  void get_string_list_implemented(strings_list_t &slist);

  p_lexval    _lexval;
  std::string _scalingunit;
  std::string _cast;
};

p_number adms_number_new (p_lexval mylexval);

/* ------- function -------------- */
struct s_function : public s_math {
  s_function (p_lexval mylexval,int myunique_id)
    : s_math(admse_function), _lexval(mylexval), _unique_id(myunique_id), _definition(nullptr)
    {}

  std::string get_uid_implemented();
  void get_reference_list_implemented(references_list_t &rlist);
  void get_attribute_list_implemented(attributes_list_t &alist);
  void get_string_list_implemented(strings_list_t &slist);

  p_lexval _lexval;
  int _unique_id;
  p_analogfunction _definition;
  p_adms_list _arguments;
};
p_function adms_function_new (p_lexval mylexval,int myunique_id);

/* ------- variable -------------- */
struct s_variable : public s_math {
  s_variable(p_variableprototype myprototype)
    : s_math(admse_variable),_prototype(myprototype) {}

  std::string get_uid_implemented();
  void get_reference_list_implemented(references_list_t &rlist);
  void get_attribute_list_implemented(attributes_list_t &alist);
  void get_string_list_implemented(strings_list_t &slist);

  p_variableprototype _prototype;
};

p_variable adms_variable_new (p_variableprototype myprototype);

/* ------- array -------------- */
struct s_array : public s_math {
  s_array(p_variable myvariable, p_adms myindex)
    : s_math(admse_array), _variable(myvariable), _index(myindex) {}

  std::string get_uid_implemented();
  void get_reference_list_implemented(references_list_t &rlist);
  void get_attribute_list_implemented(attributes_list_t &alist);
  void get_string_list_implemented(strings_list_t &slist);

  p_variable _variable;
  p_adms _index;
};

p_array adms_array_new (p_variable myvariable,p_adms myindex);

/* ------- probe -------------- */
struct s_probe : public s_math {
  s_probe (p_module mymodule, p_branch mybranch,p_nature mynature)
    : s_math(admse_probe), _module(mymodule), _branch(mybranch), _nature(mynature)
  {}

  std::string get_uid_implemented();
  void get_reference_list_implemented(references_list_t &rlist);
  void get_attribute_list_implemented(attributes_list_t &alist);
  void get_string_list_implemented(strings_list_t &slist);

  p_module _module;
  p_branch _branch;
  p_nature _nature;
};
p_probe adms_probe_new (p_module mymodule,p_branch mybranch,p_nature mynature);

/* ------- expression -------------- */
struct s_expression : public s_math {
  s_expression (p_module mymodule,p_adms mytree)
    : s_math(admse_expression)
  , _tree(mytree),
    _hasspecialnumber("NO"),
    _infinity(admse_no)
  {}

  std::string get_uid_implemented();
  void get_reference_list_implemented(references_list_t &rlist);
  void get_attribute_list_implemented(attributes_list_t &alist);
  void get_string_list_implemented(strings_list_t &slist);

  p_adms _tree;
  std::string _hasspecialnumber;
  admse _infinity;
};

p_expression adms_expression_new (p_module mymodule,p_adms mytree);

/* ------- nilled -------------- */
struct s_nilled : public s_math {
  s_nilled (p_module mymodule)
  : s_math(admse_nilled), _module(mymodule) {}

  std::string get_uid_implemented();
  void get_reference_list_implemented(references_list_t &rlist);
  void get_attribute_list_implemented(attributes_list_t &alist);
  void get_string_list_implemented(strings_list_t &slist);

  p_module _module;
};
p_nilled adms_nilled_new (p_module mymodule);

/* ------- assignment -------------- */
struct s_assignment : public s_math {
  s_assignment (p_module mymodule,p_adms mylhs,p_expression myrhs,p_lexval mylexval)
  : s_math(admse_assignment), _module(mymodule), _lhs(mylhs), _rhs(myrhs), _lexval(mylexval) {}

  std::string get_uid_implemented();
  void get_reference_list_implemented(references_list_t &rlist);
  void get_attribute_list_implemented(attributes_list_t &alist);
  void get_string_list_implemented(strings_list_t &slist);

  p_module _module;
  p_adms _lhs;
  p_expression _rhs;
  p_lexval _lexval;
  attribute_list _attribute;
};
p_assignment adms_assignment_new (p_module mymodule,p_adms mylhs,p_expression myrhs,p_lexval mylexval);

/* ------- contribution -------------- */
struct s_contribution : public s_math {
  s_contribution (p_module mymodule,p_source mylhs,p_expression myrhs,p_lexval mylexval) :
    s_math(admse_contribution), _module(mymodule), _lhs(mylhs), _rhs(myrhs), _lexval(mylexval), _branchalias(nullptr)
  {}

  std::string get_uid_implemented();
  void get_reference_list_implemented(references_list_t &rlist);
  void get_attribute_list_implemented(attributes_list_t &alist);
  void get_string_list_implemented(strings_list_t &slist);

  p_module _module;
  p_source _lhs;
  p_expression _rhs;
  p_lexval _lexval;
  p_branchalias _branchalias;
  attribute_list _attribute;
};
p_contribution adms_contribution_new (p_module mymodule,p_source mylhs,p_expression myrhs,p_lexval mylexval);

/* ------- conditional -------------- */
struct s_conditional : public s_math {
  s_conditional (p_module mymodule,p_expression myif,p_adms mythen,p_adms myelse)
    : s_math(admse_conditional), _module(mymodule), _if(myif), _then(mythen), _else(myelse) {}

  std::string get_uid_implemented();
  void get_reference_list_implemented(references_list_t &rlist);
  void get_attribute_list_implemented(attributes_list_t &alist);
  void get_string_list_implemented(strings_list_t &slist);

  p_module _module;
  p_expression _if;
  p_adms _then;
  p_adms _else;
};
p_conditional adms_conditional_new (p_module mymodule,p_expression myif,p_adms mythen,p_adms myelse);

/* ------- forloop -------------- */
struct s_forloop : public s_math {
  s_forloop (p_module mymodule,p_assignment myinitial,p_expression mycondition,p_assignment myupdate,p_adms myforblock)
    : s_math(admse_forloop), _module(mymodule), _initial(myinitial), _condition(mycondition), _update(myupdate), _forblock(myforblock)
  {
  }

  std::string get_uid_implemented();
  void get_reference_list_implemented(references_list_t &rlist);
  void get_attribute_list_implemented(attributes_list_t &alist);
  void get_string_list_implemented(strings_list_t &slist);

  p_module _module;
  p_assignment _initial;
  p_expression _condition;
  p_assignment _update;
  p_adms _forblock;
};
p_forloop adms_forloop_new (p_module mymodule,p_assignment myinitial,p_expression mycondition,p_assignment myupdate,p_adms myforblock);

/* ------- whileloop -------------- */
struct s_whileloop : public s_math {
  s_whileloop (p_module mymodule,p_expression mywhile,p_adms mywhileblock)
    : s_math(admse_whileloop), _module(mymodule), _while(mywhile), _whileblock(mywhileblock) {}

  std::string get_uid_implemented();
  void get_reference_list_implemented(references_list_t &rlist);
  void get_attribute_list_implemented(attributes_list_t &alist);
  void get_string_list_implemented(strings_list_t &slist);
  
  p_module _module;
  p_expression _while;
  p_adms _whileblock;
};
p_whileloop adms_whileloop_new (p_module mymodule,p_expression mywhile,p_adms mywhileblock);

/* ------- case -------------- */
struct s_case : public s_math {
  s_case (p_module mymodule, p_expression mycase)
    : s_math(admse_case), _module(mymodule), _case(mycase) {}

  std::string get_uid_implemented();
  void get_reference_list_implemented(references_list_t &rlist);
  void get_attribute_list_implemented(attributes_list_t &alist);
  void get_string_list_implemented(strings_list_t &slist);

  p_module _module;
  p_expression _case;
  p_adms_list _caseitem;
};

p_case adms_case_new (p_module mymodule,p_expression mycase);

/* ------- caseitem -------------- */
struct s_caseitem : public s_math {
  s_caseitem  (p_adms mycode)
    : s_math(admse_caseitem), _code(mycode), _defaultcase(admse_no) {}

  std::string get_uid_implemented();
  void get_reference_list_implemented(references_list_t &rlist);
  void get_attribute_list_implemented(attributes_list_t &alist);
  void get_string_list_implemented(strings_list_t &slist);

  p_adms _code;
  admse _defaultcase;
  p_adms_list _condition;
};

p_caseitem adms_caseitem_new (p_adms mycode);

/* ------- callfunction -------------- */
struct s_callfunction : public s_math {
  s_callfunction (p_module mymodule,p_function myfunction) : s_math(admse_callfunction), _module(mymodule), _function(myfunction) {}

  std::string get_uid_implemented();
  void get_reference_list_implemented(references_list_t &rlist);
  void get_attribute_list_implemented(attributes_list_t &alist);
  void get_string_list_implemented(strings_list_t &slist);

  p_module _module;
  p_function _function;
};

p_callfunction adms_callfunction_new (p_module mymodule,p_function myfunction);

int adms_probe_cmp (p_probe myprobe,p_probe refprobe);
#endif

