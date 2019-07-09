#include "VMF.h"
#include <fstream>
#include <iostream>
#include <algorithm>
#include "Plane.h"


const string VMF::writeChild(KeyVals * kv, unsigned depth)
{
#define ADD_TABS for (unsigned i = 0; i < depth; ++i) { result += "\t"; }

	string result;
	ADD_TABS;

	result += "\"" + kv->getKey() + "\"";

	// Check to see if there's children.
	if(!kv->getFirst())
	{
		// Write val if not.
		result += "\t\"" + kv->getValString() + "\"\n";
	}
	else {
		result += "\n";
		ADD_TABS;
		result += "{\n";

		for (auto child = kv->getFirst(); child; child = kv->getNext()) {
			result += writeChild(child, depth + 1);
		}
		
		ADD_TABS;
		result += "}\n";
	}

	return result;
#undef ADD_TABS
}

bool VMF::getNextToken()
{
	while (sourcePos != source.end() && (iswspace(*sourcePos))) {
		++sourcePos;
	}
	if (sourcePos == source.end())
		return false;

	switch (*sourcePos) {
	case '{':
	case '}':
		curToken = *sourcePos++;
		break;
	case '"':
		makeQuoteToken();
		break;
	default:
		makeToken();
		break;
	}
	return true;
}

void VMF::makeQuoteToken()
{
	curToken = "";
	while (++sourcePos != source.end() && (*sourcePos != '"')) {
		curToken += *sourcePos;
	}
	++sourcePos; // increment to after the trailing quote
}

inline bool newToken(char c) {
	switch (c) {
	case '{':
	case '}':
	case '"':
		return true;
	default:
		return false;
	}
}

void VMF::makeToken()
{
	curToken = "";
	while (sourcePos != source.end() && (!iswspace(*sourcePos)) && !newToken(*sourcePos)) {
		curToken += *sourcePos++;
	}
}

KeyVals * VMF::reflectSolid(KeyVals * kv)
{
	KeyVals* mirror = new KeyVals();
	mirror->setKey("solid");

	// Duplicate the solid, mirror, and adjust visgroups.
	for (auto child = kv->getFirst(); child; child = kv->getNext()) {
		if (child->getKey() == "editor") {
			// Change the visgroups.
			auto ed = mirror->addChild();
			ed->setKey("editor");

			// If a visgroup exists, reset it to ours.
			bool changedVis = false;
			for (auto c = child->getFirst(); c; c = child->getNext()) {
				auto last = ed->addChildTerminal(c->getKey(), c->getValString());
				if (last->getKey() == "visgroupid") {
					
					if (std::stoi(last->getValString()) == nomirror_visID) {
						// Skip everything in the no_mirror visgroup
						delete mirror;
						numSkipped++;
						return nullptr;
					}

					last->setValString("69");
					changedVis = true;
					break;
				}
			}
			// Otherwise, add a new visgroup entry.
			ed->addChildTerminal("visgroupid", "69");
		}
		else if (child->getKey() == "side") {
			// Reflect.
			auto side = mirror->addChild();
			side->setKey("side");

			for (auto c = child->getFirst(); c; c = child->getNext()) {
				if (c->getKey() == "dispinfo") {
					auto disp = side->addChild();
					disp->setKey("dispinfo");

					for (auto inf = c->getFirst(); inf; inf = c->getNext()) {
						if (inf->getKey() == "startposition") {
							string startpos = inf->getValString().substr(1, inf->getValString().size() - 2);
							Vector3 pos(startpos);
							pos.x = -pos.x;
							disp->addChildTerminal("startposition", "[" + pos.toString() + "]");
						}
						else disp->addChildExisting(copyRecursive(inf));
					}
				}
				else {
					auto inner = side->addChildTerminal(c->getKey(), c->getValString());

					if (inner->getKey() == "plane") {
						// flip
						Plane p(inner->getValString());
						p.mirror();
						inner->setValString(p.toString());
					}
					else if (inner->getKey() == "id") {
						// increment ID.
						inner->setValString(std::to_string(++sideID));
					}
				}
			}
		}
		else if (child->getKey() == "id") {
			// Generate a unique brush ID.
			mirror->addChildTerminal("id", std::to_string(++brushID));
		}
		else {
			// Just copy it over. (Should just be id -- not robust)
			mirror->addChildExisting(copyRecursive(child));
		}
	}

	return mirror;
}

KeyVals * VMF::reflectEntity(KeyVals * kv)
{
	KeyVals* mirror = new KeyVals();
	mirror->setKey("entity");
	bool studiomdl = false;

	// Duplicate the entity, with exceptions:
	// Mirror origin about the axis, reflect the angles
	// Adjust visgroups, make a unique ID
	// invert TeamNum keyvals
	// Create a unique targetname
	for (auto child = kv->getFirst(); child; child = kv->getNext()) {
		if (child->getKey() == "editor") {
			// Change the visgroups.
			auto ed = mirror->addChild();
			ed->setKey("editor");

			// If a visgroup exists, reset it to ours.
			bool changedVis = false;
			for (auto c = child->getFirst(); c; c = child->getNext()) {
				auto last = ed->addChildTerminal(c->getKey(), c->getValString());
				if (last->getKey() == "visgroupid") {

					if (std::stoi(last->getValString()) == nomirror_visID) {
						// Skip everything in the no_mirror visgroup
						delete mirror;
						numSkipped++;
						return nullptr;
					}

					last->setValString("69");
					changedVis = true;
					break;
				}
			}
			if (!changedVis) {
				// Otherwise, add a new visgroup entry.
				ed->addChildTerminal("visgroupid", "69");
			}
		}
		else if (child->getKey() == "id") {
			// Generate a unique brush ID.
			mirror->addChildTerminal("id", std::to_string(++entID));
		}
		else if (child->getKey() == "targetname") {
			string newName = child->getValString() + "_mirror";
			mirror->addChildTerminal("targetname", newName);
			reflectedNames[child->getValString()] = newName;
		}
		else if (child->getKey() == "angles") {
			// Reflect yaw on the axis.
			Vector3 angles(child->getValString());
			float oldYaw = angles.y;
			angles.y = (180.f - angles.y);
			if (angles.y < 0)
				angles.y += 360.f;

			if (oldYaw != angles.y) {
				// Roll needs reflection too.
				angles.z = -angles.z;
			}

			mirror->addChildTerminal("angles", angles.toString());
		}
		else if (child->getKey() == "origin") {
			Vector3 origin(child->getValString());
			origin.x = -origin.x;
			mirror->addChildTerminal("origin", origin.toString());
		}
		else if (child->getKey() == "TeamNum") {
			int num = std::stoi(child->getValString());
			num = (num == 1 ? 2 : 1);
			mirror->addChildTerminal("TeamNum", std::to_string(num));
		}
		else if (child->getKey() == "model") {
			mirror->addChildTerminal("model", child->getValString());
			studiomdl = true;
		}
		else if (child->getKey() == "solid") {
			// Special case - reflect the solid inside the entity like a regular brush.
			// Make sure this isn't the "solidity" keyval that uses the same name.
			if(!studiomdl)
				mirror->addChildExisting(reflectSolid(child));
		}
		else {
			// Just copy it over. (Should just be id -- not robust)
			mirror->addChildExisting(copyRecursive(child));
		}
	}

	return mirror;
}

KeyVals * VMF::copyRecursive(KeyVals * kv)
{
	KeyVals* copy = new KeyVals();

	copy->setKey(kv->getKey());
	if (kv->getValString().empty()) {
		// copy children
		for (auto inner = kv->getFirst(); inner; inner = kv->getNext()) {
			copy->addChildExisting(copyRecursive(inner));
		}
	}
	else 
		copy->setValString(kv->getValString());
	return copy;
}

void VMF::Parse(string filepath)
{
	std::cout << "Opening map " << filepath << "...\n";
	std::ifstream file;
	file.open(filepath);
	if (!file.is_open()) {
		std::cout << "Couldn't open the file.\n";
		exit(1);
	}

	source.reserve(16384);
	std::cout << "Opened " << filepath << ", loading into memory...\n";

	string line;
	while (std::getline(file, line)) {
		source += line;
	}
	sourcePos = source.begin();
	file.close();

	std::cout << "Parsing file...\n";

	root.addChild();
	KeyVals* curNode = root.getLast();
	while (getNextToken()) {
		switch (state) {
		default:
		case Key:
			if (curToken == "}") {
				// Token is '}'
				// Remove the temp child we were supposed to fill.
				curNode = curNode->parent;
				curNode->removeLast();

				// We've finished the value for the parent node.
				// Move up a level if necessary.
				if (curNode->parent) {
					curNode = curNode->parent;
					curNode->addChild();
					curNode = curNode->getLast();
					state = Key;
				}
				else {
					// Top level value is a list, which we just filled.
					// We're done.
					return;
				}
			}
			else {
				curNode->setKey(curToken);
				state = Value;
			}
			break;

		case Value:
			if (curToken == "{") {
				// Value will be a list of children nodes.
				curNode->addChild();
				curNode = curNode->getLast();
			}
			else if (curToken != "}"){
				// Set the value, move back to parent node.
				curNode->setValString(curToken);

				// Keep track of SideIDs.
				if (curNode->getKey() == "id") {
					unsigned id = (unsigned)std::stoi(curNode->getValString());
					if (curNode->parent) {
						string parentKey = curNode->parent->getKey();
						if (parentKey == "entity")
							entID = std::max(id, entID);
						else if (parentKey == "side")
							sideID = std::max(id, sideID);
						else if (parentKey == "solid")
							brushID = std::max(id, brushID);
						else if (parentKey != "world") {
							std::cout << "Didn't recognize the owner of the ID keyfield: \n\t" << parentKey << " #" << id << "\n";
							std::cout << "ID won't be incremented if it's mirrored.\n\n";
						}
					}
				}


				// Start on a new child.
				if (curNode->parent) {
					curNode = (curNode->parent);
					curNode->addChild();
					curNode = curNode->getLast();
				}
				else {
					// Top level value is a string. We're done.
					return;
				}
			}
			else {
				std::cout << "Parse ERROR: Found '}', expected a Value.\n";
				return;
			}
			state = Key;
			break;
		}

		//std::cout << curToken << std::endl;
	}

	if (state == Value) {
		std::cout << "Parse ERROR: End of file before a value was matched.\n";
		std::cout << "Current key is: " << curNode->getValString() << "\n";
	}
	root.removeLast();
}

void VMF::writeFile(string filepath)
{
	std::cout << "Writing the map to " << filepath << "...\n";

	std::ofstream file(filepath);
	if (!file.is_open()) {
		std::cout << "Write ERROR: Couldn't open file " << filepath << " to write to.\n";
		return;
	}
	file.clear();

	for (auto child = root.getFirst(); child; child = root.getNext()) {
		file << writeChild(child, 0);
	}

	file.close();
}

void VMF::ReflectBrushes()
{
	KeyVals* kv;
	bool hasWorld = false;

	for (kv = root.getFirst(); kv; kv = root.getNext()) {
		if (kv->getKey() == "visgroups") {
			// get ID for no_mirror
			for (auto vg = kv->getFirst(); vg; vg = kv->getNext()) {
				if (vg->getFirst()->getValString() == "no_mirror") {
					nomirror_visID = std::stoi(vg->getNext()->getValString());
					std::cout << "Skipping everything in visgroup " << nomirror_visID << "...\n";
					break;
				}
			}

			// Add a new visgroup
			kv = kv->addChild();

			kv->setKey("visgroup");
			kv->addChildTerminal("name", "mirrored");
			kv->addChildTerminal("id", "69");
			kv->addChildTerminal("color", "255 255 200");
			break;
		}
	}

	// Access the "world" node.
	for (kv = root.getFirst(); kv; kv = root.getNext()) {
		if (kv->getKey() == "world") {
			hasWorld = true;
			break;
		}
	}
	if (!hasWorld) {
		std::cerr << "VMF formatting error: No world/brushes found.\n";
		return;
	}

	std::cout << "Reflecting world brushes...\n";

	// Create the reflected brushwork.
	vector<KeyVals*> newBrushes;
	for (auto solid = kv->getFirst(); solid; solid = kv->getNext()){
		if (solid->getKey() == "solid") {
			newBrushes.push_back(reflectSolid(solid));
		}
	}

	// Add the reflected brushes to the VMF tree.
	for (auto ptr : newBrushes) {
		kv->addChildExisting(ptr);
	}
	std::cout << "Skipping " << numSkipped << " brushes in the no_mirror visgroup...\n";
}

void VMF::ReflectEntities()
{
	numSkipped = 0;
	std::cout << "Reflecting entities...\n";

	vector<KeyVals*> newEnts;
	for (auto kv = root.getFirst(); kv; kv = root.getNext()) {
		if (kv->getKey() == "entity") {
			newEnts.push_back(reflectEntity(kv));
		}
	}

	std::cout << "Skipping " << numSkipped << " entities in the no_mirror visgroup...\n";
	std::cout << "Reflecting IO in reflected entities...\n";


	for (auto ent : newEnts) {
		if (!ent)
			continue;

		// Find the connections node if there is one.
		KeyVals* connections;
		bool hasConnections = false;
		for (connections = ent->getFirst(); connections; connections = ent->getNext()) {
			if (connections->getKey() == "connections") {
				hasConnections = true;
				break;
			}
		}

		if (!hasConnections)
			continue;

		for (auto io = connections->getFirst(); io; io = connections->getNext()) {
			auto commaPos = io->getValString().find(',');
			string connectionName = io->getValString().substr(0, commaPos);
			if (reflectedNames.find(connectionName) != reflectedNames.end()) {
				// If we duplicated the connection's target, use the duplicated version.
				string newName = reflectedNames[connectionName];
				io->setValString(newName + io->getValString().substr(commaPos));
				++numIOReflected;
			}
		}
	}

	std::cout << "Rerouted " << numIOReflected << " IO entries to the new duplicated versions.\n";

	// Add the reflected ents to the VMF tree.
	for (auto ptr : newEnts) {
		root.addChildExisting(ptr);
	}
}


VMF::~VMF()
{
}
