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

#ifndef ADMS_NATURE_H
#define ADMS_NATURE_H
#include "adms.h"
#include "adms_discipline.h"

struct s_nature : public s_adms {
  s_nature(std::string myaccess) : s_adms(admse_nature), _access(myaccess), _abstol(nullptr), _base(nullptr), _ddt_nature(nullptr), _idt_nature(nullptr) {}
  std::string get_uid_implemented();

  void get_reference_list_implemented(references_list_t &rlist);
  void get_attribute_list_implemented(attributes_list_t &alist);
  void get_string_list_implemented(strings_list_t &slist);

  std::string _access;
  std::string _name;
  p_number _abstol;
  p_nature _base;
  std::string _ddt_name;
  p_nature _ddt_nature;
  std::string _idt_name;
  p_nature _idt_nature;
  std::string _units;
};
std::string adms_nature_uid (p_nature mynature);
p_nature adms_nature_new (std::string myaccess);
#endif

