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

#include "adms_node.h"

int adms_node_cmp (p_node mynode,p_node refnode)
{
  return
  (
    (mynode->_module != refnode->_module)
    ||
    mynode->_name.compare(refnode->_name)
  );
}

/* ------- nodealias -------------- */
p_nodealias adms_nodealias_new (p_module mymodule, std::string myname)
{
  p_nodealias mynewnodealias = new s_nodealias(mymodule, myname);
  return mynewnodealias;
}
std::string s_nodealias::get_uid_implemented()
{
  return _name;
}

void s_nodealias::get_reference_list_implemented(references_list_t &rlist)
{
  attribute_list _attribute;
  rlist = {
    mytup1{"module", create_adms_list(_module)},
    mytup1{"node", create_adms_list(_node)},
  };
}

void s_nodealias::get_attribute_list_implemented(attributes_list_t &alist)
{
}

void s_nodealias::get_string_list_implemented(strings_list_t &slist)
{
  slist = {
    mytup2{"name", _name},
  };
}


/* ------- node -------------- */
p_node adms_node_new (p_module mymodule, std::string myname)
{
  p_node mynewnode = new s_node(mymodule, myname);
  return mynewnode;
}

std::string s_node::get_uid_implemented()
{
  return _name;
}

void s_node::get_reference_list_implemented(references_list_t &rlist)
{
  rlist = {
    mytup1{"module", create_adms_list(_module)},
    mytup1{"discipline", create_adms_list(_discipline)},
  };
}

void s_node::get_attribute_list_implemented(attributes_list_t &alist)
{
  push_back_attributes(alist, _attribute);
}

void s_node::get_string_list_implemented(strings_list_t &slist)
{
  slist = {
    mytup2{"name", _name},
    mytup2{"direction", get_admse_string(_direction)},
    mytup2{"location", get_admse_string(_location)},
  };
}

