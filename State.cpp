#include "State.h"

/**
 * Display the description of the room the player is in. */
void State::announceLoc() const { this->currentRoom->describe(); }

/**
 * Constructor.
 * @param startRoom Pointer to the room to start in.
 */
State::State(Room *startRoom) : currentRoom(startRoom) {};

/**
 * Move to a specified room and print its description.
 * @param target Pointer to the room to move to.
 */
void State::goTo(Room *target) {
    this->currentRoom = target;
    this->announceLoc();
}
void State::set_room(Room *target) { this->currentRoom = target; }

/**
 * Return a pointer to the current room.
 * @return Pointer to the current room.
 */
Room *State::getCurrentRoom() const { return this->currentRoom; }
void State::addObjectToInventory(GameObject *object) { inventory.push_back(object); }
const std::list<GameObject*>& State::getInventory() const { return inventory; }
void State::removeObjectFromInventory(GameObject* object) { inventory.remove(object); }

void State::Clear_Inventory(){ inventory.clear(); }
