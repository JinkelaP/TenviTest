#include"rapidxml/rapidxml.hpp"
#include"rapidxml/rapidxml_utils.hpp"
#include"TenviMap.h"
#include"TenviData.h"

TenviMap::TenviMap(DWORD mapid) {
	id = mapid;
	LoadXML();
}

rapidxml::xml_node<>* xml_find_dir(rapidxml::xml_node<>* parent, std::string name) {
	for (rapidxml::xml_node<>* child = parent->first_node(); child; child = child->next_sibling()) {
		if (name.compare(child->name()) == 0) {
			return child;
		}
	}
	return NULL;
}

bool TenviMap::LoadXML() {

	std::string mapid_str = (id < 10000) ? ("0" + std::to_string(id)) : std::to_string(id);
	std::string map_xml = tenvi_data.get_xml_path() + +"\\" + tenvi_data.get_region_str() + "\\map\\" + mapid_str + "_0.xml";
	OutputDebugStringA(("[Maple] xml = " + map_xml).c_str());
	rapidxml::xml_document<> doc;

	try {
		rapidxml::file<> xmlFile(map_xml.c_str());
		doc.parse<0>(xmlFile.data());
	}
	catch (...) {
		return false;
	}

	rapidxml::xml_node<>* root = doc.first_node();

	if (!root) {
		return false;
	}

	// spawn point
	rapidxml::xml_node<> *map_sp = xml_find_dir(root, "sp");
	if (map_sp) {
		for (rapidxml::xml_node<>* child = map_sp->first_node(); child; child = child->next_sibling()) {
			TenviSpawnPoint spawn_point = {};
			spawn_point.id = data_spawn_point.size(); // all map have only 1 spawn point?
			spawn_point.x = (float)atoi(child->first_attribute("x")->value());
			spawn_point.y = (float)atoi(child->first_attribute("y")->value());
			AddSpawnPoint(spawn_point);
		}
	}

	// portal
	rapidxml::xml_node<> *map_portal = xml_find_dir(root, "portal");
	if (map_portal) {
		for (rapidxml::xml_node<>* child = map_portal->first_node(); child; child = child->next_sibling()) {
			TenviPortal portal = {};
			portal.id = atoi(child->first_attribute("no")->value());
			portal.next_id = atoi(child->first_attribute("tno")->value());
			portal.next_mapid = atoi(child->first_attribute("tid")->value());
			portal.x = (float)atoi(child->first_attribute("x")->value());
			portal.y = (float)atoi(child->first_attribute("y")->value());
			AddPortal(portal);
		}
	}

	return true;
}

DWORD TenviMap::GetID() {
	return id;
}

void TenviMap::AddSpawnPoint(TenviSpawnPoint &spawn_point) {
	data_spawn_point.push_back(spawn_point);
}

void TenviMap::AddPortal(TenviPortal &portal) {
	data_portal.push_back(portal);
}

TenviSpawnPoint TenviMap::FindSpawnPoint(DWORD id) {
	for (auto &spawn_point : data_spawn_point) {
		if (spawn_point.id == id) {
			return spawn_point;
		}
	}

	TenviSpawnPoint fake_spawn_point = {};
	return fake_spawn_point;
}

TenviPortal TenviMap::FindPortal(DWORD id) {
	for (auto &portal : data_portal) {
		if (portal.id == id) {
			return portal;
		}
	}

	TenviPortal fake_portal = {};
	return fake_portal;
}
