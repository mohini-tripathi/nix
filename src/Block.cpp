// Copyright (c) 2013, German Neuroinformatics Node (G-Node)
//
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted under the terms of the BSD License. See
// LICENSE file in the root of the Project.

/**
 * @file src/Block.cpp
 * @brief Implementation of methods and functions related to the class Block.
 */

#include <pandora/Util.hpp>
#include <pandora/Group.hpp>
#include <pandora/File.hpp>
#include <pandora/Block.hpp>
#include <pandora/Source.hpp>
#include <pandora/SimpleTag.hpp>
#include <pandora/DataArray.hpp>

using namespace std;

namespace pandora {


Block::Block(const Block &block)
  : EntityWithMetadata(block.file, block.group, block.entity_id),
    source_group(block.source_group), data_array_group(block.data_array_group),
    simple_tag_group(block.simple_tag_group), data_tag_group(block.data_tag_group)
{
  // nothing to do
}


Block::Block(File file, Group group, const string &id)
  : EntityWithMetadata(file, group, id)
{
  source_group = group.openGroup("sources");
  data_array_group = group.openGroup("data_arrays");
  simple_tag_group = group.openGroup("simple_tags");
  data_tag_group = group.openGroup("data_tags");
}


Block::Block(File file, Group group, const string &id, time_t time)
  : EntityWithMetadata(file, group, id, time)
{
  source_group = group.openGroup("sources");
  data_array_group = group.openGroup("data_arrays");
  simple_tag_group = group.openGroup("simple_tags");
  data_tag_group = group.openGroup("data_tags");
}


// source methods

bool Block::hasSource(const string &id) const {
  return source_group.hasGroup(id);
}


Source Block::getSource(const string &id) const {
  return Source(file, source_group.openGroup(id, false), id);
}


Source Block::getSource(size_t index) const {
  string id = source_group.objectName(index);
  return Source(file, source_group.openGroup(id, false), id);
  // TODO handle exceptions!!
}




size_t Block::sourceCount() const {
  return source_group.objectCount();
}


std::vector<Source> Block::sources() const {
  vector<Source> source_obj;

  size_t source_count = sourceCount();
  for (size_t i = 0; i < source_count; i++) {
    Source s = getSource(i);
    source_obj.push_back(s);
  }

  return source_obj;
}

  
  
std::vector<Source> Block::findSources(std::function<bool(const Source &)> predicate) const
{
  vector<Source> result;
  
  size_t source_count = sourceCount();
  for (size_t i = 0; i < source_count; i++) {
    Source s = getSource(i);
    vector<Source> tmp = s.collectIf(predicate);
    result.insert(result.begin(), tmp.begin(), tmp.end());
  }
  
  return result;
}
  
  

Source Block::createSource(const string &name,const string &type) {
  string id = util::createId("source");

  while(source_group.hasObject(id)) {
    id = util::createId("source");
  }

  Source s(file, source_group.openGroup(id, true), id);
  s.name(name);
  s.type(type);

  return s;
}


bool Block::removeSource(const string &id) {
  bool removed = false;
  
  if (hasSource(id)) {
    source_group.removeGroup(id);
    removed = true;
  }
  
  return removed;
}


// SimpleTag methods

bool Block::hasSimpleTag(const string &id) const {
  return simple_tag_group.hasObject(id);
}


SimpleTag Block::getSimpleTag(const string &id) const {
  if (hasSimpleTag(id)) {
    SimpleTag st(file, *this, simple_tag_group.openGroup(id, true), id);
    return st;
  } else {
    throw runtime_error("Unable to find SimpleTag with id " + id + "!");
  }
}


SimpleTag Block::getSimpleTag(size_t index) const {
  if (index < simpleTagCount()) {
    string id = simple_tag_group.objectName(index);
    SimpleTag st(file, *this, simple_tag_group.openGroup(id, true), id);
    return st;
  } else {
    throw runtime_error("Unable to find SimpleTag with the given index!");
  }
}


size_t Block::simpleTagCount() const {
  return simple_tag_group.objectCount();
}


vector<SimpleTag> Block::simpleTags() const {
  vector<SimpleTag> tag_obj;

  size_t tag_count = simpleTagCount();
  for (size_t i = 0; i < tag_count; i++) {
    string id = simple_tag_group.objectName(i);
    SimpleTag st(file, *this, simple_tag_group.openGroup(id), id);
    tag_obj.push_back(st);
  }

  return tag_obj;
}


SimpleTag Block::createSimpleTag(const string &name, const string &type) {
  string id = util::createId("simple_tag");

  while(hasSimpleTag(id)) {
    id = util::createId("simple_tag");
  }

  SimpleTag st(file, *this, simple_tag_group.openGroup(id, true), id);
  st.name(name);
  st.type(type);

  return st;
}


bool Block::removeSimpleTag(const string &id) {
  bool removed = false;
  
  if (hasSimpleTag(id)) {
    simple_tag_group.removeGroup(id);
    removed = true;
  }
  
  return removed;
}


// Methods related to DataArray

bool Block::hasDataArray(const string &id) const {
  return data_array_group.hasObject(id);
}


DataArray Block::getDataArray(const string &id) const {
  if (hasDataArray(id)) {
    DataArray da(file, *this, data_array_group.openGroup(id, true), id);
    return da;   
  } else {
    throw runtime_error("Unable to find DataArray with id " + id + "!");
  }
}


DataArray Block::getDataArray(size_t index) const {
  if (index < dataArrayCount()) {
    string id = data_array_group.objectName(index);
    DataArray da(file, *this, data_array_group.openGroup(id, true), id);
    return da;
  } else {
    throw runtime_error("Unable to find DataArray with the given index!");
  } 
}


size_t Block::dataArrayCount() const {
  return data_array_group.objectCount();
}


vector<DataArray> Block::dataArrays() const {
  vector<DataArray> array_obj;
  
  size_t array_count = dataArrayCount();
  for (size_t i = 0; i < array_count; i++) {
    string id = data_array_group.objectName(i);
    DataArray da(file, *this, data_array_group.openGroup(id), id);
    array_obj.push_back(da);
  }
  
  return array_obj;
}


DataArray Block::createDataArray(const std::string &name, const std::string &type) {
  string id = util::createId("data_array");
  
  while (hasDataArray(id)) {
    id = util::createId("data_array");
  }
  
  DataArray da(this->file, *this, data_array_group.openGroup(id, true), id);
  da.name(name);
  da.type(type);
  
  return da;
}


bool Block::removeDataArray(const string &id) {
  bool removed = false;
  
  if (hasDataArray(id)) {
    data_array_group.removeGroup(id);
    removed = true;
  }
  
  return removed;
}


// Other methods and functions


Block& Block::operator=(const Block &other) {
  if (*this != other) {
      this->file = other.file;
      this->group = other.group;
      this->entity_id = other.entity_id;
      this->source_group = other.source_group;
      this->data_array_group = other.data_array_group;
      this->simple_tag_group = other.simple_tag_group;
      this->data_tag_group = other.data_tag_group;
    }
    return *this;
}

ostream& operator<<(ostream &out, const Block &ent) {
  out << "Block: {name = " << ent.name();
  out << ", type = " << ent.type();
  out << ", id = " << ent.id() << "}";
  return out;
}


Block::~Block() {}

} // end namespace pandora
