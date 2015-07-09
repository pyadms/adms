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

#ifndef ADMS_NODE_H
#define ADMS_NODE_H
#include "adms.h"

/* ------- node -------------- */
struct s_node : public s_adms {
  s_node(p_module mymodule, std::string myname) : s_adms(admse_node), _module(mymodule), _name(myname),
  _direction(admse_inout),
  _discipline(nullptr),
  _location(admse_internal)
{}

  std::string get_uid_implemented();
  void get_reference_list_implemented(references_list_t &rlist);
  void get_attribute_list_implemented(attributes_list_t &alist);
  void get_string_list_implemented(strings_list_t &slist);

  p_module _module;
  std::string _name;
  admse _direction;
  p_discipline _discipline;
  admse _location;
  attribute_list _attribute;
};
p_node adms_node_new (p_module mymodule,std::string myname);


/* ------- nodealias -------------- */
struct s_nodealias : public s_adms{
  s_nodealias(p_module mymodule, std::string myname) : s_adms(admse_nodealias), _module(mymodule), _name(myname), _node(nullptr) {}

  std::string get_uid_implemented();
  void get_reference_list_implemented(references_list_t &rlist);
  void get_attribute_list_implemented(attributes_list_t &alist);
  void get_string_list_implemented(strings_list_t &slist);

  p_module _module;
  std::string _name;
  p_node _node;
};
std::string adms_nodealias_uid (p_nodealias mynodealias);
p_nodealias adms_nodealias_new (p_module mymodule, std::string myname);
int adms_node_cmp (p_node mynode,p_node refnode);
#endif
