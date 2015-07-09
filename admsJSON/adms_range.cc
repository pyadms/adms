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

#include "adms_range.h"
#include "adms_math.h"

int adms_range_cmp (p_range myrange,p_range refrange)
{
  return
  (
    (myrange->_module != refrange->_module)
    ||
    (myrange->_infexpr != refrange->_infexpr)
    ||
    (myrange->_supexpr != refrange->_supexpr)
  );
}

p_range adms_range_new (p_module mymodule,p_expression myinfexpr,p_expression mysupexpr)
{
  p_range mynewrange = new s_range(mymodule, myinfexpr, mysupexpr);
  return mynewrange;
}

std::string s_range::get_uid_implemented()
{
  std::string myuid;
  switch (_type) {
    case admse_include:
      myuid = "from";
      break;
    case admse_include_value:
      myuid = "from";
      break;
    case admse_exclude:
    {
      myuid = "exclude";
      break;
    }
    case admse_exclude_value:
    {
      myuid = "exclude";
      break;
    }
    default:
      break;
  }

  myuid += " ";

  switch (_infboundtype) {
    case admse_range_bound_include:
      myuid += "[";
      break;
    case admse_range_bound_exclude:
      myuid += "(";
      break;
    case admse_range_bound_value:
      myuid += "[";
      break;
    default:
      break;
  };

  myuid += _infexpr->get_uid();
  myuid += ":";
  myuid += _supexpr->get_uid();

  switch (_supboundtype) {
    case admse_range_bound_include:
      myuid += "]";
      break;
    case admse_range_bound_exclude:
      myuid += ")";
      break;
    case admse_range_bound_value:
      myuid += "]";
      break;
    default:
      break;
  };
  return myuid;
}

void s_range::get_reference_list_implemented(references_list_t &rlist)
{
  rlist = {
    mytup1{"module", create_adms_list(_module)},
    mytup1{"infexpr", create_adms_list(_infexpr)},
    mytup1{"supexpr", create_adms_list(_supexpr)},
  };
}

void s_range::get_attribute_list_implemented(attributes_list_t &alist)
{
}

void s_range::get_string_list_implemented(strings_list_t &slist)
{
  slist = {
    mytup2{"name", _name},
    mytup2{"infboundtype", get_admse_string(_infboundtype)},
    mytup2{"supboundtype", get_admse_string(_supboundtype)},
    mytup2{"type", get_admse_string(_type)},
  };
}

