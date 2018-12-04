/***************************************************************************
 *  Project:    betax-router
 *  File:       list-bus-stop.cpp
 *  Language:   C++
 *
 *  Author:     Andrianambinina Marius Rabenarivo 
 *  E-Mail:     marius@betax.mg 
 *
 *  Description:    Parse OSM XML input file passed as argument to command line
 *                  and extract bus stops composing each bus line.
 *
 *  Dependencies:       libboost, libosmium
 *
 *  Compiler Options:   -std=c++11 -lboost_program_options 
 *
 *  Usage : ./parse-xml antananarivo.xml
 *
 ***************************************************************************/


#include <cstdlib>  // for std::exit
#include <iostream> // for std::cerr
#include <fstream>
// Allow any format of input files (XML, PBF, ...)
#include <osmium/io/any_input.hpp>
#include <osmium/relations/relations_manager.hpp>
#include <osmium/osm/object.hpp>
#include <osmium/osm/relation.hpp>
#include <osmium/osm/node.hpp>
#include <osmium/osm/way.hpp>

// For the NodeLocationForWays handler
#include <osmium/handler/node_locations_for_ways.hpp>

// For the location index. There are different types of indexes available.
// This will work for all input files keeping the index in memory.
#include <osmium/index/map/flex_mem.hpp>

// The type of index used. This must match the include file above
using index_type = osmium::index::map::FlexMem<osmium::unsigned_object_id_type, osmium::Location>;

// The location handler always depends on the index type
using location_handler_type = osmium::handler::NodeLocationsForWays<index_type>;

class YourManager : public osmium::relations::RelationsManager<YourManager, true, true, false> {
public:
  bool new_relation(const osmium::Relation& relation) noexcept;
  bool new_member(const osmium::Relation& /*relation*/, const osmium::RelationMember& /*member*/, std::size_t /*n*/) noexcept;
  void complete_relation(const osmium::Relation& relation) noexcept;

};

bool YourManager::new_relation(const osmium::Relation& relation) noexcept {
  return relation.tags().has_tag("route", "bus");
}
  
bool YourManager::new_member(const osmium::Relation& /*relation*/, const osmium::RelationMember& /*member*/, std::size_t /*n*/) noexcept {
  return true;
}

void YourManager::complete_relation(const osmium::Relation& relation) noexcept {
  for (const auto& tag : relation.tags()) {
    std::cout << tag.key() << '=' << tag.value() << '\n';
  }
  
  std::ofstream out_file;
  
  const char* name_cs = relation.tags().get_value_by_key("name");
  if (!name_cs)
    return;
  std::string line_name(name_cs);
  if (line_name.find(" ") != std::string::npos)
    out_file << line_name.replace(line_name.find(" "), 1, "_");
  
  out_file.open ("_" + line_name + ".txt", std::ios_base::out);
  
  // Iterate over all members
  for (const auto& member : relation.members()) {
    // member.ref() will be 0 for all members you are not interested
    // in. The objects for those members are not available.
    if (member.ref() != 0) {
      // Get the member object
      const osmium::OSMObject* obj = this->get_member_object(member);

      // If you know which type you have you can also use any of these:
      const osmium::Node* node         = this->get_member_node(member.ref());
      if (node != nullptr && (strcmp(node->tags().get_value_by_key("highway", ""), "bus_stop") == 0 ||
			      strcmp(node->tags().get_value_by_key("public_transport", ""), "stop_position") == 0)) {
	out_file << node->tags().get_value_by_key("name", "") << std::endl;
      }
      
    }
  }
  
  out_file << "=======================" << std::endl;
  out_file.close ();
}

int main(int argc, char* argv[]) {
  if (argc != 2) {
    std::cerr << "Usage: " << argv[0] << " OSMFILE\n";
    std::exit(1);
  }

  // You'll need some OSM input file
  osmium::io::File input_file{argv[1]};

  // Instantiate your manager class
  YourManager manager;

  // First pass through the file
  osmium::relations::read_relations(input_file, manager);

  // Second pass through the file
  osmium::io::Reader reader{input_file};
  
  // The index to hold node locations.
  index_type index;

  // The location handler will add the node locations to the index and then
  // to the ways
  location_handler_type location_handler{index};
  
  osmium::apply(reader, location_handler, manager.handler());

  // You do not have to close the Reader explicitly, but because the
  // destructor can't throw, you will not see any errors otherwise.
  reader.close();
}
