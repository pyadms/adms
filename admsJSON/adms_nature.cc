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

#include "adms_nature.h"

p_nature adms_nature_new (std::string myaccess)
{
  p_nature mynewnature = new s_nature(myaccess);
  return mynewnature;
}

std::string s_nature::get_uid_implemented()
{
  return _access;
}

void s_nature::get_reference_list_implemented(references_list_t &rlist)
{
  rlist = {
    mytup1{"abstol", create_adms_list(_abstol)},
    mytup1{"base",   create_adms_list(_base)},
    mytup1{"ddt_nature", create_adms_list(_ddt_nature)},
    mytup1{"idt_nature", create_adms_list(_idt_nature)},
  };
}

void s_nature::get_attribute_list_implemented(attributes_list_t &alist)
{
}

void s_nature::get_string_list_implemented(strings_list_t &slist)
{
  slist = {
    mytup2{"access", _access},
    mytup2{"name", _name},
    mytup2{"ddt_name", _ddt_name},
    mytup2{"idt_name", _idt_name},
    mytup2{"units", _units},
  };
}



