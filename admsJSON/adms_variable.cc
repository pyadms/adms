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

#include "adms_variable.h"
#include "adms_message.h"
#include "adms_lexval.h"
#include "adms_math.h"
#include "adms_admsmain.h"

int adms_variableprototype_cmp (p_variableprototype myvariableprototype,p_variableprototype refvariableprototype)
{
  return
  (
    (myvariableprototype->_module != refvariableprototype->_module)
    ||
    myvariableprototype->_lexval->_string.compare(refvariableprototype->_lexval->_string)
    ||
    (myvariableprototype->_block != refvariableprototype->_block)
  );
}

void adms_variableprototype_list_alias_prepend_once_or_abort (p_variableprototype mymyvariableprototype,std::string myalias)
{
  auto &list = mymyvariableprototype->_alias;
  for (auto item : list)
  {
    if (item->_value == myalias)
    {
      adms_message_fatal("variableprototype=[%s] alias=[%s] already defined\n",
          mymyvariableprototype->get_uid().c_str(),myalias.c_str());
        return;
    }
  }
  list.push_back(adms_string_new(myalias));
}

p_variableprototype adms_variableprototype_new (p_module mymodule,p_lexval mylexval,p_adms myblock)
{
  p_variableprototype mynewvariableprototype = new s_variableprototype(mymodule, mylexval, myblock);

  return mynewvariableprototype;
}

std::string s_variableprototype::get_uid_implemented()
{
  return _lexval->get_uid();
}


void s_variableprototype::get_reference_list_implemented(references_list_t &rlist)
{
  rlist = {
    mytup1{"module", create_adms_list(_module)},
    mytup1{"lexval", create_adms_list(_lexval)},
    mytup1{"block", create_adms_list(_block)},
    mytup1{"maxsize", create_adms_list(_maxsize)},
    mytup1{"minsize", create_adms_list(_minsize)},
    mytup1{"default", create_adms_list(_default)},
    mytup1{"vlast", create_adms_list(_vlast)},
  };

  push_back_mytup1(rlist, "arraydefault", _arraydefault);
  push_back_mytup1(rlist, "alias", _alias);
  push_back_mytup1(rlist, "range", _range);
  push_back_mytup1(rlist, "instance", _instance);
}

void s_variableprototype::get_attribute_list_implemented(attributes_list_t &alist)
{
  push_back_attributes(alist, _attribute);
}

void s_variableprototype::get_string_list_implemented(strings_list_t &slist)
{
  slist = {
    mytup2{"sizetype", get_admse_string(_sizetype)},
    mytup2{"parametertype", get_admse_string(_parametertype)},
    mytup2{"type", get_admse_string(_type)},
    mytup2{"input", get_admse_string(_input)},
    mytup2{"output", get_admse_string(_output)},
    mytup2{"vcount", std::to_string(_vcount)},
  };
}


