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

#ifndef ADMS_DISCIPLINE_H
#define ADMS_DISCIPLINE_H
#include "adms.h"

/* ------- discipline -------------- */
struct s_discipline : public s_adms {
  s_discipline(std::string myname) : s_adms(admse_discipline), _name(myname), _flow(nullptr), _potential(nullptr), _domain(admse_continuous) {}
  std::string get_uid_implemented();
  void get_reference_list_implemented(references_list_t &rlist);
  void get_attribute_list_implemented(attributes_list_t &alist);
  void get_string_list_implemented(strings_list_t &slist);

  std::string _name;
  p_nature _flow;
  p_nature _potential;
  admse _domain;
};
std::string adms_discipline_uid (p_discipline mydiscipline);
p_discipline adms_discipline_new (std::string myname);
int adms_discipline_cmp (p_discipline mydiscipline,p_discipline refdiscipline);

#endif

