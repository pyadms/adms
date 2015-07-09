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

#include "adms_module.h"
#include "adms_message.h"
#include "adms_branch.h"
#include "adms_math.h"
#include "adms_node.h"
#include "adms_variable.h"
#include "adms_source.h"
#include "adms_range.h"
#include "adms_admsmain.h"
#include "adms_lexval.h"
#include "adms_analogfunction.h"
#include "adms_instance.h"

/* ------- module -------------- */
p_node adms_module_list_node_prepend_by_id_once_or_ignore (p_module mymymodule,p_module mymodule, std::string myname)
{
  node_list &list = mymymodule->_node;
  p_node refnode=adms_node_new(mymodule,myname);
  return myprepend_or_ignore<s_node>(refnode, mymymodule->_node,
    [&](p_node n)->bool {return !adms_node_cmp(n, refnode);});
}

p_node adms_module_list_node_prepend_by_id_once_or_abort (p_module mymymodule,p_module mymodule,std::string myname)
{
  return myprepend_or_fail<s_node>(mymymodule->_node,
    [&](p_node n)->bool {return ((n->_module==mymodule)&&
       !n->_name.compare(myname));},
    [&](void)->void {
    adms_message_fatal("module=[%s] node=[%s] already defined\n",
      mymymodule->get_uid().c_str(), myname.c_str());},
    [&](void)->p_node {return adms_node_new(mymodule, myname);}
  );
}

p_node adms_module_list_node_lookup_by_id (p_module mymymodule,p_module mymodule,std::string myname)
{
  return mylookup_by_id<s_node>(mymymodule->_node, [&](p_node m)->bool {return ((m->_module==mymodule)&& !m->_name.compare(myname));});
}

p_nodealias adms_module_list_nodealias_prepend_by_id_once_or_abort (p_module mymymodule,p_module mymodule, std::string myname)
{
  return myprepend_or_fail<s_nodealias>(mymymodule->_nodealias,
    [&](p_nodealias n)->bool {return ((n->_module==mymodule)&&
       !n->_name.compare(myname));},
    [&](void)->void {
    adms_message_fatal("module=[%s] nodealias=[%s] already defined\n",
      mymymodule->get_uid().c_str(), myname.c_str());},
    [&](void)->p_nodealias {return adms_nodealias_new(mymodule, myname);}
  );
}

p_nodealias adms_module_list_nodealias_lookup_by_id (p_module mymymodule,p_module mymodule, std::string myname)
{
  return mylookup_by_id<s_nodealias>(mymymodule->_nodealias, [&](p_nodealias m)->bool {return ((m->_module==mymodule)&&
       !m->_name.compare(myname));});
}

p_branch adms_module_list_branch_prepend_by_id_once_or_ignore (p_module mymymodule,p_module mymodule,p_node mypnode,p_node mynnode)
{
  p_branch refbranch=adms_branch_new(mymodule,mypnode,mynnode);
  return myprepend_or_ignore<s_branch>(refbranch, mymymodule->_branch, 
    [&](p_branch n)->bool {return !adms_branch_cmp(n, refbranch);});
}

p_branchalias adms_module_list_branchalias_prepend_by_id_once_or_abort (p_module mymymodule,p_module mymodule, std::string myname)
{
  return myprepend_or_fail<s_branchalias>(mymymodule->_branchalias,
    [&](p_branchalias n)->bool {return ((n->_module==mymodule)&&
       !n->_name.compare(myname));},
    [&](void)->void {
    adms_message_fatal("module=[%s] branchalias=[%s] already defined\n",
      mymymodule->get_uid().c_str(), myname.c_str());},
    [&](void)->p_branchalias {return adms_branchalias_new(mymodule, myname);}
  );
}

p_branchalias adms_module_list_branchalias_lookup_by_id (p_module mymymodule,p_module mymodule, std::string myname)
{
  return mylookup_by_id<s_branchalias>(mymymodule->_branchalias, [&](p_branchalias m)->bool {return ((m->_module==mymodule)&&
       !m->_name.compare(myname));});
}

void adms_module_list_analogfunction_prepend_once_or_abort (p_module mymymodule,p_analogfunction myanalogfunction)
{
  myprepend_or_fail<s_analogfunction>(mymymodule->_analogfunction,
    [&](p_analogfunction m)->bool {return !adms_analogfunction_cmp(m, myanalogfunction);},
    [&](void)->void {
      //delete myanalogfunction;
      adms_message_fatal("module=[%s] analogfunction=[%s] already defined\n",
        mymymodule->get_uid().c_str(),myanalogfunction->get_uid().c_str());},
    [&](void)->p_analogfunction {return myanalogfunction;}
    );
}

p_instance adms_module_list_instance_prepend_by_id_once_or_abort (p_module mymymodule,p_module myinstantiator,p_module mymodule, std::string myname)
{
  p_instance refinstance=adms_instance_new(myinstantiator,mymodule,myname);
  return myprepend_or_fail<s_instance>(mymymodule->_instance,
    [&](p_instance m)->bool {return !adms_instance_cmp(m, refinstance);},
    [&](void)->void {
      //delete refinstance;
      adms_message_fatal("module=[%s] instance=[%s] already defined\n",
        mymymodule->get_uid().c_str(),refinstance->get_uid().c_str());},
    [&](void)->p_instance {return refinstance;}
    );
}

void adms_module_list_variable_prepend_once_or_abort (p_module mymymodule,p_variableprototype myvariable)
{
  myprepend_or_fail<s_variableprototype>(mymymodule->_variable,
    [&](p_variableprototype m)->bool {return !adms_variableprototype_cmp(m, myvariable);},
    [&](void)->void {
      //delete myvariable;
      adms_message_fatal("module=[%s] variable=[%s] already defined\n",
        mymymodule->get_uid().c_str(),myvariable->get_uid().c_str());},
    [&](void)->p_variableprototype {return myvariable;}
    );
}

p_variableprototype adms_module_list_variable_lookup_by_id (p_module mymymodule,p_module mymodule,p_lexval mylexval,p_adms myblock)
{
  p_variableprototype ret = nullptr;
  if (mymymodule)
  {
    ret = mylookup_by_id<s_variableprototype>(mymymodule->_variable,
      [&](p_variableprototype m)->bool {
        return (m->_module==mymodule)&&
           !m->_lexval->_string.compare(mylexval->_string)&&
           (m->_block==myblock);
      }
    );
  }
  return ret;
}


p_probe adms_module_list_probe_prepend_by_id_once_or_ignore (p_module mymymodule,p_module mymodule,p_branch mybranch,p_nature mynature)
{
  p_probe refprobe=adms_probe_new(mymodule,mybranch,mynature);

  return myprepend_or_ignore<s_probe>(
    refprobe,
  mymymodule->_probe,
    [&](p_probe m)->bool {
      return (!adms_probe_cmp(m,refprobe));
    }
  );
}

p_source adms_module_list_source_prepend_by_id_once_or_ignore (p_module mymymodule,p_module mymodule,p_branch mybranch,p_nature mynature)
{
  p_source refsource=adms_source_new(mymodule,mybranch,mynature);

  return myprepend_or_ignore<s_source>(
    refsource,
    mymymodule->_source,
    [&](p_source m)->bool {
      return (!adms_source_cmp(m,refsource));
    }
  );

}

p_range adms_module_list_range_prepend_by_id_once_or_abort (p_module mymymodule,p_module mymodule,p_expression myinfexpr,p_expression mysupexpr)
{
//// TODO: fix all abort functions to delete new ref
  p_range refrange=adms_range_new(mymodule,myinfexpr,mysupexpr);
  return myprepend_or_fail<s_range>(mymymodule->_range,
    [&](p_range m)->bool {return !adms_range_cmp(m, refrange);},
    [&](void)->void {
        //delete refrange;
      adms_message_fatal("module=[%s] range=[%s] already defined\n",
        mymymodule->get_uid().c_str(),refrange->get_uid().c_str());
      },
    [&](void)->p_range {return refrange;}
    );
}

p_module adms_module_new (std::string myname)
{
  p_module mynewmodule = new s_module(myname);
  return mynewmodule;
}
std::string s_module::get_uid_implemented()
{
  return _name;
}

void s_module::get_reference_list_implemented(references_list_t &rlist)
{
  rlist = {
    mytup1{"analog", create_adms_list(_analog)},
  };
  push_back_mytup1(rlist, "node", _node);
  push_back_mytup1(rlist, "nodealias", _nodealias);
  push_back_mytup1(rlist, "branch", _branch);
  push_back_mytup1(rlist, "branchalias", _branchalias);
  push_back_mytup1(rlist, "analogfunction", _analogfunction);
  push_back_mytup1(rlist, "instance", _instance);
  push_back_mytup1(rlist, "variable", _variable);
  push_back_mytup1(rlist, "block", _block);
  push_back_mytup1(rlist, "blockvariable", _blockvariable);
  push_back_mytup1(rlist, "assignment", _assignment);
  push_back_mytup1(rlist, "callfunction", _callfunction);
  push_back_mytup1(rlist, "contribution", _contribution);
  push_back_mytup1(rlist, "conditional", _conditional);
  push_back_mytup1(rlist, "case", _case);
  push_back_mytup1(rlist, "forloop", _forloop);
  push_back_mytup1(rlist, "whileloop", _whileloop);
  push_back_mytup1(rlist, "expression", _expression);
  push_back_mytup1(rlist, "probe", _probe);
  push_back_mytup1(rlist, "source", _source);
  push_back_mytup1(rlist, "range", _range);
  push_back_mytup1(rlist, "attribute", _attribute);
}

void s_module::get_attribute_list_implemented(attributes_list_t &alist)
{
}

void s_module::get_string_list_implemented(strings_list_t &slist)
{
  slist = {mytup2{"name", _name}};
}
