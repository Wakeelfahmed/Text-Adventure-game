#ifndef TEXTADV_ROOM_H
#define TEXTADV_ROOM_H
#include <string>
#include <forward_list>
#include <list>
#include "GameObject.h"
using std::string;

// Represents a room (accessible location in the game).
class Room {
    const string* name; //Short name used as a header.
    const string* description;  //Full description of the room.
    Room* north;    //Pointer to room that is north of this one.
    Room* east;   // New property for east exit
    Room* south;  // New property for south exit
    Room* west;   // New property for west exit
    std::list<GameObject*> objects;

public:
    /**
     * Constructs a new Room.
     * @param _name Name of the room.
     * @param _desc Description of the room.
     */
    Room(const string *_name, const string *_desc);

    ~Room(); //Removes a destroyed room from the global list if it's there.
    const std::string* getname() const {
        return name;
    }
    void describe() const;  //Outputs the name and description of the room in standard format.

    static std::list<Room*> rooms; //List storing all rooms that have been registered via addRoom().

    /**
     * Creates a new Room with the given parameters and register it with the static list.
     * @param _name Name of the room.
     * @param _desc Description of the room.
     */
    static Room* addRoom(const string* _name, const string* _desc);
    static void addRoom(Room* room);


    Room* getNorth() const;
    void setNorth(Room* _north);
    Room* getEast() const;  // Accessor method for east exit
    Room* getSouth() const; // Accessor method for south exit
    Room* getWest() const;  // Accessor method for west exit
    void setEast(Room* _east);
    void setSouth(Room* _south);
    void setWest(Room* _west);

    void addObject(GameObject* object);
    const std::list<GameObject*>& getObjects() const;
    std::list<GameObject*>& getObjectsMutable();

};
#endif //TEXTADV_ROOM_H