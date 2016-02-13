#!/usr/bin/env python

# Copyright 2016 Luther T. Cat
# 
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
# 
#     http://www.apache.org/licenses/LICENSE-2.0
# 
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

import sys
import json

class MyObject:
  def __init__(self, collection, obj):
    self.idx  = obj['id']
    self.obj  = obj
    self.datatypename = obj['datatypename']
    self.attributes = obj['attributes']
    self.parameters = obj['parameters']
    self.references = obj['references']
    #self.parents    = set([])
    self.name = None
    if 'name' in self.parameters:
      self.name = self.parameters['name']
    elif self.datatypename == 'variableprototype':
      self.name = self.obj['uid']
    self.collection = collection

  def set_parameter(self, name, value):
    self.parameters[name] = value

  def set_attribute(self, name, value):
    self.attributes[name] = value

  def get_attributes(self):
    return self.attributes

  def get_attribute(self, name):
    return self.attributes[name]

  def has_attribute(self, name):
    return name in self.attributes

  def get_parameters(self):
    return self.parameters

  def get_parameter(self, name):
    return self.parameters[name]

  def has_parameter(self, name):
    return name in self.parameters

  def get_children(self, name):
    return self.collection.get_entries(self.references[name])

  def get_child(self, name):
    return self.get_children(name)[0]

  def has_child(self, name):
    return name in self.references

  def add_child(self, name, obj, allow_duplicate = False):
    if name not in self.references:
      self.references[name] = []
    if allow_duplicate:
      self.references[name].append(obj.idx)
    else:
      if obj.idx not in self.references[name]:
        self.references[name].append(obj.idx)

  #def get_parents(self, name):
  #  return self.parents

  #def add_parent(self, obj):
  #  self.parents.update(obj.idx)






class MyCollection:
  def __init__(self):
    self.entries = []
    self.loaded = False
    self.max_index = None
    self.admsmain = None
    self.module = None

  def find_all_of_datatypename(self, datatypename):
    return [x for x in self.entries if x.datatypename == datatypename]

  def get_entries(self, entries):
    return [self.entries[x] for x in entries]

  def update_refs(self):
    ''' Set some common variables'''
    main = [x for x in self.entries if x.datatypename == 'admsmain']
    if len(main) != 1:
      raise RuntimeError("expected only one admsmain")
    self.admsmain = main[0]
    self.module   = self.admsmain.get_children('module')
        

  def load_json(self, filename):
    if self.loaded:
      raise RuntimeError("Cannot load more than one json file")
    with open(filename, 'r') as f:
      # an OrderedDict is much more expensive for a large number of objects
      #data = json.load(f, object_pairs_hook=OrderedDict)
      data = json.load(f)
    self.max_index = max([l['id'] for l in data])
    #print self.max_index
    self.entries = [None] * (self.max_index + 1)
    for l in data:
      self.entries[l['id']] = MyObject(self, l)
    #update_parents(self)
    self.update_refs()
    

if __name__ == "__main__":
  coll = MyCollection()
  coll.load_json(sys.argv[1])



