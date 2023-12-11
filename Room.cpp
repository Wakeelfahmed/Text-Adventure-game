#include "Room.h"
#include "wordwrap.h"

std::list<Room*> Room::rooms; // Stores a static list of all rooms.

/**
 * Room default constructor.
 * @param _name Room's name.
 * @param _desc Room's description.
 */
Room::Room() : north(nullptr), south(nullptr), east(nullptr), west(nullptr) {}
Room::Room(const string* _name, const string* _desc) :	name(_name), description(_desc), north(nullptr), east(nullptr), south(nullptr), west(nullptr) {}

Room::~Room() {	Room::rooms.remove(this); }     //Remove destroyed rooms from the static list.

void Room::describe() const {       //Prints the description of a room (the name and long description)
	wrapOut(this->name);
	wrapEndPara();
	wrapOut(this->description);
	wrapEndPara();

    if (!objects.empty()) {
        size_t objectCount = objects.size();
        wrapOut(new string("Objects in room: "));

        // Loop through objects and print their names with commas
        for (auto objectIt = objects.begin(); objectIt != objects.end(); ++objectIt) {
            wrapOut(&(*objectIt)->getShortName());

            if (objectCount > 1 && std::next(objectIt) != objects.end())
                wrapOut(new string(","));

        }
        wrapEndPara();
    }
}

/**
 * Statically creates a room and then adds it to the global list.
 * @param _name Name for the new room.
 * @param _desc Description for the new room.
 * @return A pointer to the newly created room.
 */
Room* Room::addRoom(const string* _name, const string* _desc) {
	auto* newRoom = new Room(_name, _desc);
	Room::rooms.push_back(newRoom);
	return newRoom;
}

/**
 * Adds an existing room to the static list.
 * @param room Pointer to the room to add.
 * @return
 */
void Room::addRoom(Room* room) { Room::rooms.push_back(room); }


/**
 * Gets the north exit of a room.
 * @return The room that is north of this one, or NULL if there is no exit.
 */
Room* Room::getNorth() const { return this->north; }
void Room::setNorth(Room* _north) { this->north = _north; }
Room* Room::getEast() const { return this->east; } // Accessor method for east exit
Room* Room::getSouth() const { return this->south; } // Accessor method for south exit
Room* Room::getWest() const { return this->west; }  // Accessor method for west exit
void Room::setEast(Room* _east) { this->east = _east; }
void Room::setSouth(Room* _south) { this->south = _south; }
void Room::setWest(Room* _west) { this->west = _west; }

void Room::addObject(GameObject* object) { objects.push_back(object); }
std::list<GameObject*>& Room::getObjects() { return objects; }

void Room::Clear_room_objects() { objects.clear(); }
