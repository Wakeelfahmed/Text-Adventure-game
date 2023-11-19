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
    const int numRooms = 6;
    Room* rooms[numRooms];
    for (int i = 0; i < numRooms; ++i) {
        rooms[i] = new Room(&rooms_name[i], &rooms_desc[i]); // Create a new room
        Room::addRoom(rooms[i]);
    }

    rooms[0]->setNorth(rooms[2]);
    rooms[0]->setWest(rooms[3]);
    rooms[0]->setSouth(rooms[4]);
    rooms[0]->setEast(rooms[1]);

    rooms[1]->setWest(rooms[0]);

    rooms[2]->setSouth(rooms[0]);

    rooms[3]->setEast(rooms[0]);

    rooms[4]->setSouth(rooms[5]);
    rooms[4]->setNorth(rooms[0]);

    rooms[5]->setNorth(rooms[4]);
    // Add objects to rooms
    rooms[0]->addObject(new GameObject("Lamp", "An old lamp with a flickering light.", "lamp"));
    rooms[0]->addObject(new GameObject("Table", "A wooden table with some papers scattered on it.", "table"));

    rooms[2]->addObject(new GameObject("Painting", "A mysterious painting hanging on the wall.", "painting"));

    rooms[2]->addObject(new GameObject("Knife", "A sharp kitchen knife on the countertop.", "knife"));
    rooms[2]->addObject(new GameObject("Book", "An ancient book with a worn leather cover.", "book"));

    rooms[3]->addObject(new GameObject("Bed", "A neatly made bed with a dark, velvet cover.", "bed"));
    rooms[3]->addObject(new GameObject("Mirror", "An ornate mirror reflecting your image.", "mirror"));

    rooms[4]->addObject(new GameObject("Bookshelf", "A tall bookshelf filled with ancient tomes.", "bookshelf"));
    rooms[4]->addObject(new GameObject("Crystal", "A sparkling crystal resting on a pedestal.", "crystal"));

    rooms[5]->addObject(new GameObject("Candlestick", "An ornate candlestick on a small pedestal.", "candlestick"));
    rooms[5]->addObject(new GameObject("Scroll", "A rolled-up scroll with a mysterious seal.", "scroll"));
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
            secondWord = "";  // No space, set secondWord to an empty string
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
        if(secondWord != "" && commandOk){}
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