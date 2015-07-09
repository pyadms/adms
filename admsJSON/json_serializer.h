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

#ifndef JSON_SERIALIZER_H
#define JSON_SERIALIZER_H
#include "adms.h"
#include <list>
#include <unordered_map>
#include <utility>
#include <vector>
#include <iostream>

struct json_object {
  json_object(size_t sn, p_adms pt) : serial_number(sn), pointer(pt) {}

  size_t serial_number;
  p_adms pointer;
  std::string uid;
  strings_list_t strings;
  references_list_t   references;
  attributes_list_t   attributes;
//// also do math info ////
};


typedef std::unordered_map<p_adms, json_object *> json_map_t;
typedef std::vector<json_object *> json_list_t;

void json_serialize(p_adms top, json_map_t &/*json_index*/, json_list_t &/*json_rindex*/);
void json_write(std::ostream &out, json_map_t &/*json_index*/, json_list_t &/*json_rindex*/);

#endif

