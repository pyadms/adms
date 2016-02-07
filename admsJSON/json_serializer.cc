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

#include "json_serializer.h"
#include <sstream>

////datatypenames (map initialization), attributes, etc
/////datatypename and uid
void json_serialize(p_adms ref, json_map_t &json_index, json_list_t &json_rindex)
{
    // check if already processed
    if ((!ref) || (json_index.count(ref)))
    {
      return;
    }
    else
    {
      json_object *jobj = new json_object(json_rindex.size(), ref);
      json_rindex.push_back(jobj);
      json_index[ref] = jobj;
      ref->get_string_list(jobj->strings);
      ref->get_attribute_list(jobj->attributes);
      ref->get_reference_list(jobj->references);
      for (auto i : jobj->references)
      {
        for (auto j : std::get<1>(i))
        {
          json_serialize(j, json_index, json_rindex);
        }
      }
      jobj->uid = ref->get_uid();
    }
}
namespace {
void print_key_value(std::ostream &out, const std::string &key, size_t value)
{
  out << "\"" << key << "\" : " << value;
}
void print_key_value(std::ostream &out, const std::string &key, const std::string &value)
{
  out << "\"" <<  key << "\" : \"";
  //TODO: use a regex replace
  for (const char &c : value)
  {
    if (c == '\n')
    {
      out << "\\n";
    }
    else if (c == '"')
    {
      out << "\\\"";
    }
    else
    {
      out << c;
    }
  }
  out << "\"";
}
void print_key_list(std::ostream &out, json_map_t &json_index, const references_list_t &refs)
{
  bool first_i = true;
  for (auto i : refs)
  {
    if (first_i)
    {
      first_i = false;
    }
    else
    {
      out << ",\n";
    }

    const std::string &name = std::get<0>(i);
    out << "\"" << name << "\" : [";
    bool first_j = true;
    for (auto j : std::get<1>(i))
    {
      if (j)
      {
        if (first_j)
        {
          first_j = false;
        }
        else
        {
          out << ", ";
        }
        out << json_index[j]->serial_number;
      }
    }
    out << "]";
  }
}
}

void json_write(std::ostream &out, json_map_t &json_index, json_list_t &json_rindex)
{
  bool first_k;
  bool empty_k;
  bool first_l = true;
  out << "[\n";
  for (auto l : json_rindex)
  {
    empty_k = false;
    if (first_l)
    {
      first_l = false;
    }
    else
    {
      out << ",\n";
    }
    out << "{\n";
    print_key_value(out, "id", l->serial_number);
    out << ",\n";
    print_key_value(out, "uid", l->uid);
    out << ",\n";
    print_key_value(out, "datatypename", get_admse_string(l->pointer->_datatypename));
    out << ",\n";
    out << "\"attributes\" : {";
    first_k = true;
    empty_k = true;
    for (auto k : l->attributes) {
      empty_k = false;
      if (first_k)
      {
        out << "\n";
        first_k = false;
      }  
      else
      {
        out << ",\n";
      }
      print_key_value(out, std::get<0>(k), std::get<1>(k));
    }
    if (!empty_k)
    {
      out << "\n";
    }
    out << "},\n";

    out << "\"parameters\" : {";
    first_k = true;
    for (auto k : l->strings) {
      empty_k = false;
      if (first_k)
      {
        out << "\n";
        first_k = false;
      }  
      else
      {
        out << ",\n";
      }
      print_key_value(out, std::get<0>(k), std::get<1>(k));
    }

    if (!empty_k)
    {
      out << "\n";
    }
    out << "},\n";


    out << "\"references\" : {";
    std::ostringstream os1;
    print_key_list(os1, json_index, l->references);
    if (!os1.str().empty())
    {
      out << "\n" << os1.str() << "\n";
    }
    out << "}\n";
    // end of unit
    out << "}";
    out.flush();
  }
  out << "\n]\n";
}



