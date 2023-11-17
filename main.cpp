#include <iostream>
#include <iomanip>
#include <memory>
#include <forward_list>
#include "Room.h"
#include "wordwrap.h"
#include "State.h"
#include "strings.h"

using std::string;
using std::unique_ptr;

string commandBuffer;
State *currentState;

/**
 * Print out the command prompt then read a command into the provided string buffer.
 * @param buffer Pointer to the string buffer to use.
 */
void inputCommand(string *buffer) {
    buffer->clear();
    std::cout << "> ";
    std::getline(std::cin, *buffer);
}

void initRooms() {
    auto *r2 = new Room(&r2name, &r2desc);
    auto *r1 = new Room(&r1name, &r1desc);
    auto *r3 = new Room(&r3name, &r3desc);
    auto *r4 = new Room(&r4name, &r4desc);
    auto *r5 = new Room(&r5name, &r5desc);
    auto *r6 = new Room(&r6name, &r6desc);

    // Add rooms to the global list
    Room::addRoom(r1);
    Room::addRoom(r2);
    Room::addRoom(r3);
    Room::addRoom(r4);
    Room::addRoom(r5);
    Room::addRoom(r6);

    r1->setNorth(r3);
    r1->setWest(r4);        r1->addObject(new GameObject("Pencil","A simple pencil","pencil"));
    r1->setSouth(r5);
    r1->setEast(r2);

    r2->setWest(r1);

    r3->setSouth(r1);       r3->addObject(new GameObject("Key","A Mysterious Key","key"));

    r4->setEast(r1);

    r5->setSouth(r6);
    r5->setNorth(r1);

    r6->setNorth(r5);
}

//Sets up the game state.
void initState() { currentState = new State(Room::rooms.front()); }

void getObject(const std::string& keyword) {
    Room* currentRoom = currentState->getCurrentRoom();
    bool found = false;

    for (auto it = currentRoom->getObjects().begin(); it != currentRoom->getObjects().end(); ++it) {
        if ((*it)->getKeyword() == keyword) {
            currentState->addObjectToInventory(*it);
            currentRoom->getObjectsMutable().erase(it);
            wrapOut(new string("You picked up the " + keyword + "."));
            wrapEndPara();
            found = true;
            break;
        }
    }

    if (!found) {
        wrapOut(new string("There's no " + keyword + " to pick up here."));
        wrapEndPara();
    }
}

// Function to drop an object from the player's inventory to the current room
void dropObject(const std::string& keyword) {
    Room* currentRoom = currentState->getCurrentRoom();
    bool found = false;

    for (auto it = currentState->getInventory().begin(); it != currentState->getInventory().end(); ++it) {
        if ((*it)->getKeyword() == keyword) {
            currentRoom->addObject(*it);
            currentState->removeObjectFromInventory(*it);
            wrapOut(new string("You dropped the " + keyword + "."));
            wrapEndPara();
            found = true;
            break;
        }
    }

    if (!found) {
        wrapOut(new string("You don't have a " + keyword + " to drop."));
        wrapEndPara();
    }
}

// Function to examine an object in the current room or inventory
void examineObject(const std::string& keyword) {
    Room* currentRoom = currentState->getCurrentRoom();
    bool found = false;

    for (const auto& obj : currentRoom->getObjects()) {
        if (obj->getKeyword() == keyword) {
            wrapOut(&obj->getLongDescription());
            wrapEndPara();
            found = true;
            break;
        }
    }

    if (!found) {
        for (const auto& obj : currentState->getInventory()) {
            if (obj->getKeyword() == keyword) {
                wrapOut(&obj->getLongDescription());
                wrapEndPara();
                found = true;
                break;
            }
        }
    }

    if (!found) {
        wrapOut(new string("There's no " + keyword + " to examine here."));
        wrapEndPara();
    }
}

void showInventory() {
    const auto& inventory = currentState->getInventory();

    if (inventory.empty()) {
        wrapOut(new string("Your inventory is empty."));
        wrapEndPara();
    } else {
        wrapOut(new string("Your inventory:"));
        for (const auto& obj : inventory) {
            wrapOut(&obj->getShortName());
        }
        wrapEndPara();
    }
}
// The main game loop.
void gameLoop() {
    bool commandOk = false;
    while (true) {
        /* Ask for a command. */
        inputCommand(&commandBuffer);
        for (char & i : commandBuffer) {
            if(isupper(i))
                i = tolower(i);
        }
        /* The first word of a command would normally be the verb. The first word is the text before the first
         * space, or if there is no space, the whole string. */
        auto endOfVerb = static_cast<uint8_t>(commandBuffer.find(' '));
        std::string command = commandBuffer.substr(0, endOfVerb);
        std::string secondWord;

        if (commandBuffer.find(' ') != std::string::npos) {
            secondWord = commandBuffer.substr(endOfVerb + 1);  // Add 1 to skip the space
        } else {
            secondWord = "";  // No space, set secondWord to an empty string or handle it as needed
        }
        Room *room = nullptr;
        commandOk = false;
        if (command == "north" || command == "n") {
            room = currentState->getCurrentRoom()->getNorth();
            commandOk = true;
        } else if (command == "east" || command == "e") {
            room = currentState->getCurrentRoom()->getEast();
            commandOk = true;
        } else if (command == "south" || command == "s") {
            room = currentState->getCurrentRoom()->getSouth();
            commandOk = true;
        } else if (command == "west" || command == "w") {
            room = currentState->getCurrentRoom()->getWest();
            commandOk = true;
        } else if (command == "quit")
            exit(0); //terminate program
        else if (command == "get") {
            getObject(secondWord);      commandOk = true;
        } else if (command == "drop") {
            dropObject(secondWord);     commandOk = true;
        } else if (command == "inventory") {
            showInventory();                    commandOk = true;
        } else if (command == "examine") {
            examineObject(secondWord);  commandOk = true;
        }
        if(secondWord != ""){}
        else if (commandOk) {
            if(command == "examine" ||command == "inventory" ||command == "get" ||command == "drop"){}
            else if (room)
                currentState->goTo(room);
            else {
                wrapOut(&badExit); //correct direction was given but cant go there.
                wrapEndPara();
            }
        } else {
            wrapOut(&badCommand);   //Incorrect direction was given.
            wrapEndPara();
        }
    }
}

int main(){
    initWordWrap();
    initRooms();
    initState();
    currentState->announceLoc();
    gameLoop();
    return 0;
}