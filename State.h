#ifndef TEXTADV_STATE_H
#define TEXTADV_STATE_H

#include "Room.h"
class State {
    Room *currentRoom;
    std::list<GameObject*> inventory;
public:
    explicit State(Room *startRoom);
    void goTo(Room *target);
    void announceLoc() const;
    Room* getCurrentRoom() const;

    void addObjectToInventory(GameObject* object);
    void removeObjectFromInventory(GameObject* object);
    const std::list<GameObject*>& getInventory() const;
};

#endif //TEXTADV_STATE_H