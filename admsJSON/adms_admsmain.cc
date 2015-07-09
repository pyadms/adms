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

#include "adms_admsmain.h"
#include "adms_module.h"
#include "adms_nature.h"
#include "adms_message.h"

p_admsmain globaladmsmain;
p_admsmain root (void) {return globaladmsmain;}
void rootnew (p_admsmain myglobaladmsmain) {globaladmsmain=myglobaladmsmain;}

p_module adms_admsmain_list_module_prepend_by_id_once_or_abort (p_admsmain mymyadmsmain, std::string myname)
{
  return myprepend_or_fail<s_module>(mymyadmsmain->_module,
    [&](p_module m)->bool {return m->_name == myname;},
    [&](void)->void {
      adms_message_fatal("admsmain=[%s] module=[%s] already defined\n",
        mymyadmsmain->get_uid().c_str(),myname.c_str());},
    [&](void)->p_module {return adms_module_new(myname);}
  );
}

p_module adms_admsmain_list_module_lookup_by_id (p_admsmain mymyadmsmain, std::string myname)
{
  return mylookup_by_id<s_module>(mymyadmsmain->_module, [&](p_module m)->bool {return m->_name == myname;});
}

void adms_admsmain_list_discipline_prepend_once_or_abort (p_admsmain mymyadmsmain, p_discipline mydiscipline)
{
  myprepend_or_fail<s_discipline>(mymyadmsmain->_discipline,
    [&](p_discipline m)->bool {return !adms_discipline_cmp(m, mydiscipline);},
    [&](void)->void {
      adms_message_fatal("admsmain=[%s] discipline=[%s] already defined\n",
        mymyadmsmain->get_uid().c_str(),mydiscipline->get_uid().c_str());
      },
    [&](void)->p_discipline {return mydiscipline;}
    );
}

p_discipline adms_admsmain_list_discipline_lookup_by_id (p_admsmain mymyadmsmain, std::string myname)
{
  return mylookup_by_id<s_discipline>(mymyadmsmain->_discipline, [&](p_discipline m)->bool {return m->_name == myname;});
}


p_nature adms_admsmain_list_nature_prepend_by_id_once_or_abort (p_admsmain mymyadmsmain, std::string myaccess)
{
  return myprepend_or_fail<s_nature>(mymyadmsmain->_nature,
    [&](p_nature m)->bool {return m->_access == myaccess;},
    [&](void)->void {
      adms_message_fatal("admsmain=[%s] nature=[%s] already defined\n",
        mymyadmsmain->get_uid().c_str(),myaccess.c_str());},
    [&](void)->p_nature {return adms_nature_new(myaccess);}
  );
}

p_nature adms_admsmain_list_nature_lookup_by_id (p_admsmain mymyadmsmain,std::string myaccess)
{
  return mylookup_by_id<s_nature>(mymyadmsmain->_nature, [&](p_nature m)->bool {return m->_access == myaccess;});
}


p_admsmain adms_admsmain_new (std::string myname)
{
  p_admsmain mynewadmsmain = new s_admsmain(myname);
  return mynewadmsmain;
}

std::string s_admsmain::get_uid_implemented()
{
  return _name;
}

void s_admsmain::get_reference_list_implemented(references_list_t &rlist)
{
  rlist = {
    mytup1{"simulator", create_adms_list(_simulator)},
  };
  push_back_mytup1(rlist, "module", _module);
  push_back_mytup1(rlist, "discipline", _discipline);
  push_back_mytup1(rlist, "nature", _nature);
}

void s_admsmain::get_attribute_list_implemented(attributes_list_t &alist)
{
}

void s_admsmain::get_string_list_implemented(strings_list_t &slist)
{
  slist = {
    mytup2{"name", _name},
    mytup2{"fullfilename", _fullfilename},
    mytup2{"filename", _filename},
    mytup2{"curfilename", _curfilename},
    mytup2{"curline", std::to_string(_curline)},
    mytup2{"fpos", std::to_string(_fpos)},
    mytup2{"info", get_admse_string(_info)},
    mytup2{"usage", get_admse_string(_usage)},
    mytup2{"verbose", get_admse_string(_verbose)},
    mytup2{"hint", get_admse_string(_hint)},
    mytup2{"warning", get_admse_string(_warning)},
    mytup2{"obsolete", get_admse_string(_obsolete)},
    mytup2{"debug", get_admse_string(_debug)},
    mytup2{"dbg_vla", get_admse_string(_dbg_vla)},
    mytup2{"dbg_xml", get_admse_string(_dbg_xml)},
    mytup2{"error", get_admse_string(_error)},
    mytup2{"fatal", get_admse_string(_fatal)},
    mytup2{"dbg_mem", get_admse_string(_dbg_mem)},
  };
}







