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

#include "adms_analog.h"

p_analog adms_analog_new (p_adms mycode)
{
  p_analog mynewanalog = new s_analog(mycode);
  return mynewanalog;
}
std::string s_analog::get_uid_implemented()
{
  std::string myuid="analog ";
  myuid += _code->get_uid();
  return myuid;
}

void s_analog::get_reference_list_implemented(references_list_t &rlist)
{
  rlist = {
    mytup1{"code", p_adms_list{_code}},
  };
}

void s_analog::get_attribute_list_implemented(attributes_list_t &alist)
{
}

void s_analog::get_string_list_implemented(strings_list_t &slist)
{
}

