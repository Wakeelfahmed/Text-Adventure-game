#include <iostream>
#include <memory>
#include <forward_list>
#include "Room.h"
#include "wordwrap.h"
#include "State.h"
#include "strings.h"
#include <fstream>
#include <sstream>
#include <vector>

using std::string;
using namespace std;
using std::unique_ptr;

string commandBuffer;
State *currentState;

/**
 * Print out the command prompt then read a command into the provided string buffer.
 * @param buffer Pointer to the string buffer to use.
 */
void inputCommand(string *buffer) {
    buffer->clear();
    cout << "> ";
    getline(cin, *buffer);
}

const int numRooms = 6;
Room *rooms[numRooms];      //global Rooms

void initObjects(){
    // Add objects to rooms
    rooms[0]->addObject(&GameObjects[0]);
    rooms[0]->addObject(&GameObjects[1]);

    rooms[1]->addObject(&GameObjects[2]);

    rooms[2]->addObject(&GameObjects[3]);
    rooms[2]->addObject(&GameObjects[4]);

    rooms[3]->addObject(&GameObjects[5]);
    rooms[3]->addObject(&GameObjects[6]);

    rooms[4]->addObject(&GameObjects[7]);
    rooms[4]->addObject(&GameObjects[8]);

    rooms[5]->addObject(&GameObjects[9]);
    rooms[5]->addObject(&GameObjects[10]);
}

void initRooms() {
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

    initObjects();
}

//Sets up the game state.
void initState() { currentState = new State(Room::rooms.front()); }

void getObject(const string &keyword) {
    Room *currentRoom = currentState->getCurrentRoom();
    bool found = false;

    for (auto it = currentRoom->getObjects().begin(); it != currentRoom->getObjects().end(); ++it) {
        if ((*it)->getKeyword() == keyword) {
            currentState->addObjectToInventory(*it);
            currentRoom->getObjects().erase(it);
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
void dropObject(const string &keyword) {
    Room *currentRoom = currentState->getCurrentRoom();
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
void examineObject(const string &keyword) {
    Room *currentRoom = currentState->getCurrentRoom();
    bool found = false;

    for (const auto &obj: currentRoom->getObjects()) {
        if (obj->getKeyword() == keyword) {
            wrapOut(&obj->getLongDescription());
            wrapEndPara();
            found = true;
            break;
        }
    }

    if (!found) {
        for (const auto &obj: currentState->getInventory()) {
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
    const auto &inventory = currentState->getInventory();

    if (inventory.empty()) {
        wrapOut(new string("Your inventory is empty."));
        wrapEndPara();
    } else {
        wrapOut(new string("Your inventory : "));
        auto it = inventory.begin();
        while (it != inventory.end()) {
            wrapOut(&(*it)->getShortName());

            // Check if it's not the last item before printing the comma
            if (++it != inventory.end()) {
                wrapOut(new string(","));
            }
        }
        wrapEndPara();
    }
}

void saveGameState(const string &filename) {
    ofstream saveFile(filename);
    if (!saveFile.is_open()) {
        wrapOut(new string("Error: Unable to open the save file."));
        wrapEndPara();
        return;
    }

    // Save the inventory
    const auto &inventory = currentState->getInventory();
    for (const auto &obj: inventory) {
        for (int i = 0; i < Number_of_Objects; ++i) {
            if (obj == &GameObjects[i])
                saveFile << i << ",";
        }
    }
    saveFile << "\n";

    // Save the objects in rooms
    int i = 0;
    for (const auto &room: Room::rooms) {
        const auto &droppedObjects = room->getObjects();
        if (*(room->getname()) == *(currentState->getCurrentRoom()->getname()))
            saveFile << "*" << i << ":";   //indicate current room by 1st character asterisk
        else
            saveFile << " " << i << ":";    //if not current room, then leave 1st character as space
        for (const auto &obj: droppedObjects) {
            for (int i = 0; i < Number_of_Objects; ++i) {
                if (obj == &GameObjects[i])
                    saveFile << i << ",";
            }
        }
        saveFile << "\n";
        i++;
    }
    wrapOut(new string("Game state saved to " + filename));
    wrapEndPara();
}

bool isInteger(const string &s) {
    if (s.empty())
        return false;  // Empty string is not a valid integer

    for (char c: s) {
        if (!isdigit(c))
            return false;  // Non-digit character found, not a valid integer
    }

    return true;  // All characters are digits, valid integer
}

void Reset_Game_if_corruptfile(const string &filename){
    currentState->Clear_Inventory();    //Clear Inventory and update with new inventory from file
    for (Room *room: rooms)
        room->Clear_room_objects(); //Clear all room objects
    initObjects();
    initState();
    wrapOut(new string("Corrupt save file attempted to load: " + filename));
    wrapEndPara();
    wrapOut(new string("Game and the file(" + filename + ") reset"));
    wrapEndPara();
    saveGameState(filename);
    currentState->announceLoc();
}
// Function to load the game state from a file
void loadGameState(const string &filename) {
    ifstream loadFile(filename);
    if (!loadFile.is_open()) {
        wrapOut(new string("Error: Unable to open the save file."));
        wrapEndPara();
        return;
    }

    string line;
    currentState->Clear_Inventory();    //Clear Inventory and update with new inventory from file
    getline(loadFile, line);      //get inventory line
    istringstream iss(line);
    string words;
    // Iterate through the Inventory CSV line and extract each number
    while (getline(iss, words, ',')) {
        if (isInteger(words)) {
            int number = stoi(words);    // Now 'number' contains the integer value
            currentState->addObjectToInventory(&GameObjects[number]); //those words found, put in inventory
        }
        else {
            loadFile.close(); //close the current file
            Reset_Game_if_corruptfile(filename);
            return;
        }
    }
    for (Room *room: rooms)
        room->Clear_room_objects(); //Clear all room objects

    string Room_name, Objects;
    int room_number;
    while (getline(loadFile, line)) {
        Room_name = line.substr(1, line.find(':') - 1);  //Room name is till ':'
        if (isInteger(Room_name)) {
            room_number = stoi(Room_name);  // Now 'number' contains the integer value
        }
        else {
            loadFile.close(); //close the current file
            Reset_Game_if_corruptfile(filename);
            return;
        }

        if (line[0] == '*')
            currentState->set_room(rooms[room_number]);

        Objects = line.substr(line.find(':') + 1); //onwards ':', its CSV room objects
        istringstream iss(Objects);
        while (getline(iss, words, ',')) {
            if (isInteger(words)) {
                int number = stoi(words);
                rooms[room_number]->addObject(&GameObjects[number]); //those objects found, put in room
            }
            else {
                loadFile.close(); //close the current file
                Reset_Game_if_corruptfile(filename);
                return;
            }

        }
    }
    wrapOut(new string("Succesfully saved game read from " + filename));
    wrapEndPara();

    currentState->announceLoc();
    loadFile.close(); // Close the file after reading
}

// The main game loop.
void gameLoop() {
    bool commandOk = false;
    while (true) {
        /* Ask for a command. */
        inputCommand(&commandBuffer);
        for (char &i: commandBuffer) {
            if (isupper(i))
                i = tolower(i);
        }
        /* The first word of a command would normally be the verb. The first word is the text before the first
         * space, or if there is no space, the whole string. */
        auto endOfVerb = static_cast<uint8_t>(commandBuffer.find(' '));
        string command = commandBuffer.substr(0, endOfVerb);
        string secondWord;

        if (commandBuffer.find(' ') != string::npos)
            secondWord = commandBuffer.substr(endOfVerb + 1);  // Add 1 to skip the space
        else
            secondWord = "";  // No space, set secondWord to an empty string

        Room *room = nullptr;
        commandOk = false;
        if (command == "save") {
            saveGameState(secondWord);
            commandOk = true;
        } else if (command == "load") {
            loadGameState(secondWord);
            commandOk = true;
        } else if (command == "north" || command == "n") {
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
            getObject(secondWord);
            commandOk = true;
        } else if (command == "drop") {
            dropObject(secondWord);
            commandOk = true;
        } else if (command == "inventory") {
            showInventory();
            commandOk = true;
        } else if (command == "examine") {
            examineObject(secondWord);
            commandOk = true;
        }
        if (commandOk) {
            if (command == "examine" || command == "inventory" || command == "get" || command == "drop" ||
                command == "save" || command == "load") {
                //do nothing
            } else if (room)
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

int main() {
    initWordWrap();
    initRooms();
    initState();
    currentState->announceLoc();
    gameLoop();
    return 0;
}