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

#include "adms_branch.h"
#include "adms_node.h"

int adms_branch_cmp (p_branch mybranch,p_branch refbranch)
{
  return
  (
    (mybranch->_module != refbranch->_module)
    ||
    (mybranch->_pnode != refbranch->_pnode)
    ||
    (mybranch->_nnode != refbranch->_nnode)
  );
}

p_branchalias adms_branchalias_new (p_module mymodule, std::string myname)
{
  p_branchalias mynewbranchalias = new s_branchalias(mymodule, myname);
  return mynewbranchalias;
}
std::string s_branchalias::get_uid_implemented()
{
  return _name;
}

void s_branchalias::get_reference_list_implemented(references_list_t &rlist)
{
  rlist = {
    mytup1{"module", create_adms_list(_module)},
    mytup1{"branch", create_adms_list(_branch)},
  };
}

void s_branchalias::get_attribute_list_implemented(attributes_list_t &alist)
{
}

void s_branchalias::get_string_list_implemented(strings_list_t &slist)
{
  slist = {
    mytup2{"name", _name},
  };
}

p_branch adms_branch_new (p_module mymodule,p_node mypnode,p_node mynnode)
{
  p_branch mynewbranch = new s_branch(mymodule, mypnode, mynnode);
  return mynewbranch;
}
std::string s_branch::get_uid_implemented()
{
  std::string myuid = _pnode->get_uid();
  myuid += ",";
  myuid += _nnode->get_uid();
  return myuid;
}

void s_branch::get_reference_list_implemented(references_list_t &rlist)
{
  attribute_list _attribute;
  rlist = {
    mytup1{"module", create_adms_list(_module)},
    mytup1{"pnode", create_adms_list(_pnode)},
    mytup1{"nnode", create_adms_list(_nnode)},
    mytup1{"discipline", create_adms_list(_discipline)},
  };
}

void s_branch::get_attribute_list_implemented(attributes_list_t &alist)
{
  push_back_attributes(alist, _attribute);
}

void s_branch::get_string_list_implemented(strings_list_t &slist)
{
}

