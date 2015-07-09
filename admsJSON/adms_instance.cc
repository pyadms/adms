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

#include "adms_instance.h"
#include "adms_message.h"
#include "adms_admsmain.h"
#include "adms_module.h"
#include "adms_node.h"
#include "adms_variable.h"

int adms_instance_cmp (p_instance myinstance,p_instance refinstance)
{
  return
  (
    (myinstance->_instantiator != refinstance->_instantiator)
    ||
    (myinstance->_module != refinstance->_module)
    ||
    myinstance->_name.compare(refinstance->_name)
  );
}
int adms_instanceparameter_cmp (p_instanceparameter myinstanceparameter,p_instanceparameter refinstanceparameter)
{
  return
  (
    (myinstanceparameter->_parameter != refinstanceparameter->_parameter)
  );
}
int adms_instancenode_cmp (p_instancenode myinstancenode,p_instancenode refinstancenode)
{
  return
  (
    (myinstancenode->_nodefrominstantiator != refinstancenode->_nodefrominstantiator)
    ||
    (myinstancenode->_nodefrommodule != refinstancenode->_nodefrommodule)
  );
}

/* ------- instance -------------- */
void adms_instance_list_terminal_prepend_once_or_abort (p_instance mymyinstance,p_instancenode myterminal)
{
  myprepend_or_fail<s_instancenode>(mymyinstance->_terminal,
    [&](p_instancenode m)->bool {
      return !adms_instancenode_cmp(m, myterminal);
    },
    [&](void)->void {
      //delete myterminal;
      adms_message_fatal("instance=[%s] terminal=[%s] already defined\n",
        mymyinstance->get_uid().c_str(),myterminal->get_uid().c_str());},
    [&](void)->p_instancenode {return myterminal;}
  );
}

void adms_instance_list_parameterset_prepend_once_or_abort (p_instance mymyinstance,p_instanceparameter myparameterset)
{
  myprepend_or_fail<s_instanceparameter>(mymyinstance->_parameterset,
    [&](p_instanceparameter m)->bool {
      return !adms_instanceparameter_cmp(m, myparameterset);
    },
    [&](void)->void {
      adms_message_fatal("instance=[%s] parameterset=[%s] already defined\n",
        mymyinstance->get_uid().c_str(),myparameterset->get_uid().c_str());
      //delete myparameterset;
    },
    [&](void)->p_instanceparameter {return myparameterset;}
  );
}

p_instance adms_instance_new (p_module myinstantiator,p_module mymodule, std::string myname)
{
  p_instance mynewinstance = new s_instance(myinstantiator, mymodule, myname);
  return mynewinstance;
}

std::string s_instance::get_uid_implemented()
{
  std::string myuid;
  myuid += _instantiator->get_uid();
  myuid += ".";
  myuid += _module->get_uid();
  myuid += "#";
  myuid += _name;
  return myuid;
}

void s_instance::get_reference_list_implemented(references_list_t &rlist)
{
  instancenode_list _terminal;
  instanceparameter_list _parameterset;
  rlist = {
    mytup1{"instantiator", create_adms_list(_instantiator)},
    mytup1{"module", create_adms_list(_module)},
  };
  push_back_mytup1(rlist, "terminal", _terminal);
  push_back_mytup1(rlist, "parameterset", _parameterset);
}

void s_instance::get_attribute_list_implemented(attributes_list_t &alist)
{
}

void s_instance::get_string_list_implemented(strings_list_t &slist)
{
  slist = {
    mytup2{"name", _name},
  };
}

/* ------- instanceparameter -------------- */
p_instanceparameter adms_instanceparameter_new (p_variableprototype myparameter)
{
  p_instanceparameter mynewinstanceparameter = new s_instanceparameter(myparameter);
  return mynewinstanceparameter;
}
std::string s_instanceparameter::get_uid_implemented()
{
  std::string myuid;
  myuid += _parameter->get_uid();
  myuid += "#";
  myuid += _value->get_uid();
  return myuid;
}

void s_instanceparameter::get_reference_list_implemented(references_list_t &rlist)
{
  rlist = {
    mytup1{"parameter", create_adms_list(_parameter)},
    mytup1{"value", create_adms_list(_value)},
  };
}

void s_instanceparameter::get_attribute_list_implemented(attributes_list_t &alist)
{
}

void s_instanceparameter::get_string_list_implemented(strings_list_t &slist)
{
}

/* ------- instancenode -------------- */
p_instancenode adms_instancenode_new (p_node mynodefrominstantiator,p_node mynodefrommodule)
{
  p_instancenode mynewinstancenode = new s_instancenode(mynodefrominstantiator,mynodefrommodule);
  return mynewinstancenode;
}

std::string s_instancenode::get_uid_implemented()
{
  std::string myuid;
  myuid += _nodefrominstantiator->get_uid();
  myuid += "#";
  myuid += _nodefrommodule->get_uid();
  return myuid;
}

void s_instancenode::get_reference_list_implemented(references_list_t &rlist)
{
  rlist = {
    mytup1{"nodefrominstantiator", create_adms_list(_nodefrominstantiator)},
    mytup1{"nodefrommodule", create_adms_list(_nodefrommodule)},
  };
}

void s_instancenode::get_attribute_list_implemented(attributes_list_t &alist)
{
}

void s_instancenode::get_string_list_implemented(strings_list_t &slist)
{
}

