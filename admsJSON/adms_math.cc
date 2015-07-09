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

#include "adms_math.h"
#include "adms_message.h"
#include "adms_lexval.h"
#include "adms_variable.h"
#include "adms_admsmain.h"
#include "adms_source.h"
#include "adms_node.h"
#include "adms_module.h"

int adms_probe_cmp (p_probe myprobe,p_probe refprobe)
{
  return
  (
    (myprobe->_module != refprobe->_module)
    ||
    (myprobe->_branch != refprobe->_branch)
    ||
    (myprobe->_nature != refprobe->_nature)
  );
}

/* ------- string -------------- */
p_string adms_string_new (std::string myvalue)
{
  p_string mynewstring = new s_string(myvalue);
  return mynewstring;
}
std::string s_string::get_uid_implemented()
{
  std::string myuid="\"";
  myuid += _value;
  myuid += "\"";
  return myuid;
}

void s_string::get_reference_list_implemented(references_list_t &rlist)
{
}

void s_string::get_attribute_list_implemented(attributes_list_t &alist)
{
}

void s_string::get_string_list_implemented(strings_list_t &slist)
{
  slist = {mytup2{"value", _value}};
}


/* ------- mapply_unary -------------- */
p_mapply_unary adms_mapply_unary_new (std::string myname, p_adms myarg1)
{
  p_mapply_unary mynewmapply_unary = new s_mapply_unary(myname, myarg1);
  return mynewmapply_unary;
}

std::string s_mapply_unary::get_uid_implemented()
{
  std::string myuid;
  //TODO: case notation
  if(_name=="plus")
  {
    myuid += "(+";
    myuid += _arg1->get_uid();
    myuid += ")";
  }
  else if(_name=="minus")
  {
    myuid += "(-";
    myuid += _arg1->get_uid();
    myuid += ")";
  }
  else if(_name=="not")
  {
    myuid += "(!";
    myuid += _arg1->get_uid();
    myuid += ")";
  }
  else if(_name=="bw_not")
  {
    myuid += "(~";
    myuid += _arg1->get_uid();
    myuid += ")";
  }
  return myuid;
}

void s_mapply_unary::get_reference_list_implemented(references_list_t &rlist)
{
  rlist = {
    mytup1{"arg1", create_adms_list(_arg1)},
  };
}

void s_mapply_unary::get_attribute_list_implemented(attributes_list_t &alist)
{
}

void s_mapply_unary::get_string_list_implemented(strings_list_t &slist)
{
  slist = {mytup2{"name", _name}};
}


/* ------- mapply_binary -------------- */
p_mapply_binary adms_mapply_binary_new (std::string myname,p_adms myarg1,p_adms myarg2)
{
  p_mapply_binary mynewmapply_binary = new s_mapply_binary(myname, myarg1, myarg2);
  return mynewmapply_binary;
}
std::string s_mapply_binary::get_uid_implemented()
{
  std::string myuid;
  if(_name=="bw_equr")
  {
    myuid += "(";
    myuid += _arg1->get_uid();
    myuid += "^~";
    myuid += _arg2->get_uid();
    myuid += ")";
  }
  else if(_name=="bw_equl")
  {
    myuid += "(";
    myuid += _arg1->get_uid();
    myuid += "~^";
    myuid += _arg2->get_uid();
    myuid += ")";
  }
  else if(_name=="bw_xor")
  {
    myuid += "(";
    myuid += _arg1->get_uid();
    myuid += "^";
    myuid += _arg2->get_uid();
    myuid += ")";
  }
  else if(_name=="bw_or")
  {
    myuid += "(";
    myuid += _arg1->get_uid();
    myuid += "|";
    myuid += _arg2->get_uid();
    myuid += ")";
  }
  else if(_name=="bw_and")
  {
    myuid += "(";
    myuid += _arg1->get_uid();
    myuid += "&";
    myuid += _arg2->get_uid();
    myuid += ")";
  }
  else if(_name=="or")
  {
    myuid += "(";
    myuid += _arg1->get_uid();
    myuid += "||";
    myuid += _arg2->get_uid();
    myuid += ")";
  }
  else if(_name=="and")
  {
    myuid += "(";
    myuid += _arg1->get_uid();
    myuid += "&&";
    myuid += _arg2->get_uid();
    myuid += ")";
  }
  else if(_name=="equ")
  {
    myuid += "(";
    myuid += _arg1->get_uid();
    myuid += "==";
    myuid += _arg2->get_uid();
    myuid += ")";
  }
  else if(_name=="notequ")
  {
    myuid += "(";
    myuid += _arg1->get_uid();
    myuid += "!=";
    myuid += _arg2->get_uid();
    myuid += ")";
  }
  else if(_name=="lt")
  {
    myuid += "(";
    myuid += _arg1->get_uid();
    myuid += "<=";
    myuid += _arg2->get_uid();
    myuid += ")";
  }
  else if(_name=="lt_equ")
  {
    myuid += "(";
    myuid += _arg1->get_uid();
    myuid += "<=";
    myuid += _arg2->get_uid();
    myuid += ")";
  }
  else if(_name=="gt")
  {
    myuid += "(";
    myuid += _arg1->get_uid();
    myuid += ">";
    myuid += _arg2->get_uid();
    myuid += ")";
  }
  else if(_name=="gt_equ")
  {
    myuid += "(";
    myuid += _arg1->get_uid();
    myuid += ">=";
    myuid += _arg2->get_uid();
    myuid += ")";
  }
  else if(_name=="shiftr")
  {
    myuid += "(";
    myuid += _arg1->get_uid();
    myuid += ">>";
    myuid += _arg2->get_uid();
    myuid += ")";
  }
  else if(_name=="shiftl")
  {
    myuid += "(";
    myuid += _arg1->get_uid();
    myuid += "<<";
    myuid += _arg2->get_uid();
    myuid += ")";
  }
  else if(_name=="addp")
  {
    myuid += "(";
    myuid += _arg1->get_uid();
    myuid += "+";
    myuid += _arg2->get_uid();
    myuid += ")";
  }
  else if(_name=="addm")
  {
    myuid += "(";
    myuid += _arg1->get_uid();
    myuid += "-";
    myuid += _arg2->get_uid();
    myuid += ")";
  }
  else if(_name=="multtime")
  {
    myuid += "(";
    myuid += _arg1->get_uid();
    myuid += "*";
    myuid += _arg2->get_uid();
    myuid += ")";
  }
  else if(_name=="multdiv")
  {
    myuid += "(";
    myuid += _arg1->get_uid();
    myuid += "/";
    myuid += _arg2->get_uid();
    myuid += ")";
  }
  else if(_name=="multmod")
  {
    myuid += "(";
    myuid += _arg1->get_uid();
    myuid += "%";
    myuid += _arg2->get_uid();
    myuid += ")";
  }
  return myuid;
}

void s_mapply_binary::get_reference_list_implemented(references_list_t &rlist)
{
  rlist = {
    mytup1{"arg1", create_adms_list(_arg1)},
    mytup1{"arg2", create_adms_list(_arg2)},
  };
}

void s_mapply_binary::get_attribute_list_implemented(attributes_list_t &alist)
{
}

void s_mapply_binary::get_string_list_implemented(strings_list_t &slist)
{
  slist = {mytup2{"name", _name}};
}


/* ------- mapply_ternary -------------- */
p_mapply_ternary adms_mapply_ternary_new (std::string myname,p_adms myarg1,p_adms myarg2,p_adms myarg3)
{
  p_mapply_ternary mynewmapply_ternary = new s_mapply_ternary(myname, myarg1, myarg2, myarg3);
  return mynewmapply_ternary;
}
std::string s_mapply_ternary::get_uid_implemented()
{
  std::string myuid;
  if(_name=="conditional")
  {
    myuid += "(";
    myuid += _arg1->get_uid();
    myuid += "?";
    myuid += _arg2->get_uid();
    myuid += ":";
    myuid += _arg3->get_uid();
    myuid += ")";
  }
  return myuid;
}

void s_mapply_ternary::get_reference_list_implemented(references_list_t &rlist)
{
  rlist = {
    mytup1{"arg1", create_adms_list(_arg1)},
    mytup1{"arg2", create_adms_list(_arg2)},
    mytup1{"arg3", create_adms_list(_arg3)},
  };
}

void s_mapply_ternary::get_attribute_list_implemented(attributes_list_t &alist)
{
}

void s_mapply_ternary::get_string_list_implemented(strings_list_t &slist)
{
  slist = {mytup2{"name", _name}};
}

/* ------- number -------------- */
p_number adms_number_new (p_lexval mylexval)
{
  p_number mynewnumber = new s_number(mylexval);
  return mynewnumber;
}

std::string s_number::get_uid_implemented()
{
  std::string myuid;
  myuid += _lexval->get_uid();
  return myuid;
}

void s_number::get_reference_list_implemented(references_list_t &rlist)
{
  rlist = {
    mytup1{"lexval", create_adms_list(_lexval)},
  };
}

void s_number::get_attribute_list_implemented(attributes_list_t &alist)
{
}

void s_number::get_string_list_implemented(strings_list_t &slist)
{
  slist = {
    mytup2{"scalingunit", _scalingunit},
    mytup2{"cast", _cast},
  };
}

/* ------- function -------------- */
p_function adms_function_new (p_lexval mylexval,int myunique_id)
{
  p_function mynewfunction = new s_function(mylexval, myunique_id);
  return mynewfunction;
}

std::string s_function::get_uid_implemented()
{
  std::string myuid;
  myuid += _lexval->get_uid();
  myuid += "(";
  for (auto a : _arguments)
  {
    myuid += a->get_uid();
    if (a != _arguments.back())
    {
      myuid += ",";
    }
  }
  myuid += ")";
  return myuid;
}

void s_function::get_reference_list_implemented(references_list_t &rlist)
{
  rlist = {
    mytup1{"lexval", create_adms_list(_lexval)},
    mytup1{"definition", create_adms_list(_definition)},
  };
  push_back_mytup1(rlist, "arguments", _arguments);
}

void s_function::get_attribute_list_implemented(attributes_list_t &alist)
{
}

void s_function::get_string_list_implemented(strings_list_t &slist)
{
  slist = {
    mytup2{"unique_id", std::to_string(_unique_id)}
  };
}



/* ------- variable -------------- */
p_variable adms_variable_new (p_variableprototype myprototype)
{
  p_variable mynewvariable = new s_variable(myprototype);
  return mynewvariable;
}

std::string s_variable::get_uid_implemented()
{
  std::string myuid;
  myuid += _prototype->get_uid();
  return myuid;
}

void s_variable::get_reference_list_implemented(references_list_t &rlist)
{
  rlist = {
    mytup1{"prototype", create_adms_list(_prototype)},
  };
}

void s_variable::get_attribute_list_implemented(attributes_list_t &alist)
{
}

void s_variable::get_string_list_implemented(strings_list_t &slist)
{
}



/* ------- array -------------- */
p_array adms_array_new (p_variable myvariable,p_adms myindex)
{
  p_array mynewarray = new s_array(myvariable, myindex);
  return mynewarray;
}

std::string s_array::get_uid_implemented()
{
  std::string myuid;
  myuid += _variable->get_uid();
  myuid += "[";
  myuid += _index->get_uid();
  myuid += "]";
  return myuid;
}

void s_array::get_reference_list_implemented(references_list_t &rlist)
{
  rlist = {
    mytup1{"variable", create_adms_list(_variable)},
    mytup1{"index", create_adms_list(_index)},
  };
}

void s_array::get_attribute_list_implemented(attributes_list_t &alist)
{
}

void s_array::get_string_list_implemented(strings_list_t &slist)
{
}

/* ------- probe -------------- */
p_probe adms_probe_new (p_module mymodule,p_branch mybranch,p_nature mynature)
{
  p_probe mynewprobe = new s_probe(mymodule, mybranch, mynature);
  return mynewprobe;
}
std::string s_probe::get_uid_implemented()
{
  std::string myuid;
  myuid += _nature->get_uid();
  myuid += "(";
  myuid += _branch->get_uid();
  myuid += ")";
  return myuid;
}

  p_module _module;
  p_branch _branch;
  p_nature _nature;
  p_discipline _discipline;
  p_source _source;

void s_probe::get_reference_list_implemented(references_list_t &rlist)
{
  rlist = {
    mytup1{"module", create_adms_list(_module)},
    mytup1{"branch", create_adms_list(_branch)},
    mytup1{"nature", create_adms_list(_nature)},
  };
}

void s_probe::get_attribute_list_implemented(attributes_list_t &alist)
{
}

void s_probe::get_string_list_implemented(strings_list_t &slist)
{
}



/* ------- expression -------------- */
p_expression adms_expression_new (p_module mymodule,p_adms mytree)
{
  p_expression mynewexpression = new s_expression(mymodule, mytree);
  return mynewexpression;
}

std::string s_expression::get_uid_implemented()
{
  std::string myuid;
  myuid += "(";
  myuid += _tree->get_uid();
  myuid += ")";
  return myuid;
}

void s_expression::get_reference_list_implemented(references_list_t &rlist)
{
  rlist = {
    mytup1{"tree", create_adms_list(_tree)},
  };
}

void s_expression::get_attribute_list_implemented(attributes_list_t &alist)
{
}

void s_expression::get_string_list_implemented(strings_list_t &slist)
{
  slist = {
    mytup2{"hasspecialnumber", _hasspecialnumber},
    mytup2{"infinity", get_admse_string(_infinity)},
  };
}


/* ------- nilled -------------- */
p_nilled adms_nilled_new (p_module mymodule)
{
  p_nilled mynewnilled = new s_nilled(mymodule);
  return mynewnilled;
}

std::string s_nilled::get_uid_implemented()
{
  std::string myuid;
  myuid += "nilled operator";
  return myuid;
}

void s_nilled::get_reference_list_implemented(references_list_t &rlist)
{
  rlist = {
    mytup1{"module", create_adms_list(_module)},
  };
}

void s_nilled::get_attribute_list_implemented(attributes_list_t &alist)
{
}

void s_nilled::get_string_list_implemented(strings_list_t &slist)
{
}

/* ------- assignment -------------- */
p_assignment adms_assignment_new (p_module mymodule,p_adms mylhs,p_expression myrhs,p_lexval mylexval)
{
  p_assignment mynewassignment = new s_assignment(mymodule, mylhs, myrhs, mylexval);
  return mynewassignment;
}

std::string s_assignment::get_uid_implemented()
{
  std::string myuid;
  myuid += _lhs->get_uid();
  myuid += "=";
  myuid += _rhs->get_uid();
  myuid += ";";
  return myuid;
}

void s_assignment::get_reference_list_implemented(references_list_t &rlist)
{
  rlist = {
    mytup1{"module", create_adms_list(_module)},
    mytup1{"lhs", create_adms_list(_lhs)},
    mytup1{"rhs", create_adms_list(_rhs)},
    mytup1{"lexval", create_adms_list(_lexval)},
  };
}

void s_assignment::get_attribute_list_implemented(attributes_list_t &alist)
{
  push_back_attributes(alist, _attribute);
}

void s_assignment::get_string_list_implemented(strings_list_t &slist)
{
}



/* ------- contribution -------------- */
p_contribution adms_contribution_new (p_module mymodule,p_source mylhs,p_expression myrhs,p_lexval mylexval)
{
  p_contribution mynewcontribution = new s_contribution(mymodule, mylhs, myrhs, mylexval);
  return mynewcontribution;
}

std::string s_contribution::get_uid_implemented()
{
  std::string myuid;
  myuid += _lhs->get_uid();
  myuid += "<+";
  myuid += _rhs->get_uid();
  myuid += ";";
  return myuid;
}

void s_contribution::get_reference_list_implemented(references_list_t &rlist)
{
  rlist = {
    mytup1{"module", create_adms_list(_module)},
    mytup1{"lhs", create_adms_list(_lhs)},
    mytup1{"rhs", create_adms_list(_rhs)},
    mytup1{"lexval", create_adms_list(_lexval)},
    mytup1{"branchalias", create_adms_list(_branchalias)},
  };
}

void s_contribution::get_attribute_list_implemented(attributes_list_t &alist)
{
  push_back_attributes(alist, _attribute);
}

void s_contribution::get_string_list_implemented(strings_list_t &slist)
{
}


/* ------- conditional -------------- */
p_conditional adms_conditional_new (p_module mymodule,p_expression myif,p_adms mythen,p_adms myelse)
{
  p_conditional mynewconditional = new s_conditional(mymodule, myif, mythen, myelse);
  return mynewconditional;
}
std::string s_conditional::get_uid_implemented()
{
  std::string myuid;
  myuid += "if(";
  myuid += _if->get_uid();
  myuid += ") ";
  myuid += _then->get_uid();
  if(_else)
  {
    myuid += " else ";
    myuid += _else->get_uid();
  }
  return myuid;
}

void s_conditional::get_reference_list_implemented(references_list_t &rlist)
{
  rlist = {
    mytup1{"module", create_adms_list(_module)},
    mytup1{"if", create_adms_list(_if)},
    mytup1{"then", create_adms_list(_then)},
    mytup1{"else", create_adms_list(_else)},
  };
}

void s_conditional::get_attribute_list_implemented(attributes_list_t &alist)
{
}

void s_conditional::get_string_list_implemented(strings_list_t &slist)
{
}



/* ------- forloop -------------- */
p_forloop adms_forloop_new (p_module mymodule,p_assignment myinitial,p_expression mycondition,p_assignment myupdate,p_adms myforblock)
{
  p_forloop mynewforloop = new s_forloop(mymodule, myinitial, mycondition, myupdate, myforblock);
  return mynewforloop;
}
std::string s_forloop::get_uid_implemented()
{
  std::string myuid;
  myuid += "for(";
  myuid += _initial->get_uid();
  myuid += ";";
  myuid += _condition->get_uid();
  myuid += ";";
  myuid += _update->get_uid();
  myuid += ")\n";
  myuid += _forblock->get_uid();
  return myuid;
}

void s_forloop::get_reference_list_implemented(references_list_t &rlist)
{
  rlist = {
    mytup1{"module", create_adms_list(_module)},
    mytup1{"initial", create_adms_list(_initial)},
    mytup1{"condition", create_adms_list(_condition)},
    mytup1{"update", create_adms_list(_update)},
    mytup1{"forblock", create_adms_list(_forblock)},
  };
}

void s_forloop::get_attribute_list_implemented(attributes_list_t &alist)
{
}

void s_forloop::get_string_list_implemented(strings_list_t &slist)
{
}

/* ------- whileloop -------------- */
p_whileloop adms_whileloop_new (p_module mymodule,p_expression mywhile,p_adms mywhileblock)
{
  p_whileloop mynewwhileloop = new s_whileloop( mymodule, mywhile, mywhileblock);
  return mynewwhileloop;
}

std::string s_whileloop::get_uid_implemented()
{
  std::string myuid;
  myuid += "while(";
  myuid += _while->get_uid();
  myuid += ")\n";
  myuid += _whileblock->get_uid();
  return myuid;
}

void s_whileloop::get_reference_list_implemented(references_list_t &rlist)
{
  rlist = {
    mytup1{"module", create_adms_list(_module)},
    mytup1{"while", create_adms_list(_while)},
    mytup1{"whileblock", create_adms_list(_whileblock)},
  };
}

void s_whileloop::get_attribute_list_implemented(attributes_list_t &alist)
{
}

void s_whileloop::get_string_list_implemented(strings_list_t &slist)
{
}


/* ------- case -------------- */
p_case adms_case_new (p_module mymodule,p_expression mycase)
{
  p_case mynewcase = new s_case(mymodule, mycase);
  return mynewcase;
}
std::string s_case::get_uid_implemented()
{
  std::string myuid;
  myuid += "case";
  return myuid;
}

void s_case::get_reference_list_implemented(references_list_t &rlist)
{
  rlist = {
    mytup1{"module", create_adms_list(_module)},
    mytup1{"case", create_adms_list(_case)},
    mytup1{"caseitem", _caseitem},
  };
}

void s_case::get_attribute_list_implemented(attributes_list_t &alist)
{
}

void s_case::get_string_list_implemented(strings_list_t &slist)
{
}

/* ------- caseitem -------------- */
p_caseitem adms_caseitem_new (p_adms mycode)
{
  p_caseitem mynewcaseitem = new s_caseitem(mycode);
  return mynewcaseitem;
}

std::string s_caseitem::get_uid_implemented()
{
  std::string myuid;
  myuid += "caseitem";
  return myuid;
}

void s_caseitem::get_reference_list_implemented(references_list_t &rlist)
{
  rlist = {
    mytup1{"code", create_adms_list(_code)},
  };

  push_back_mytup1(rlist, "condition", _condition);
}

void s_caseitem::get_attribute_list_implemented(attributes_list_t &alist)
{
}

void s_caseitem::get_string_list_implemented(strings_list_t &slist)
{
  slist = {mytup2{"defaultcase", get_admse_string(_defaultcase)}};
}

/* ------- callfunction -------------- */
p_callfunction adms_callfunction_new (p_module mymodule,p_function myfunction)
{
  p_callfunction mynewcallfunction = new s_callfunction(mymodule, myfunction);
  return mynewcallfunction;
}

std::string s_callfunction::get_uid_implemented()
{
  std::string myuid;
  myuid += _function->get_uid();
  return myuid;
}

void s_callfunction::get_reference_list_implemented(references_list_t &rlist)
{
  rlist = {
    mytup1{"module", create_adms_list(_module)},
    mytup1{"function", create_adms_list(_function)},
  };
}

void s_callfunction::get_attribute_list_implemented(attributes_list_t &alist)
{
}

void s_callfunction::get_string_list_implemented(strings_list_t &slist)

{
}



