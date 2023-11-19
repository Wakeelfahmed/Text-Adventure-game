#ifndef TEXTADV_STRINGS_H
#define TEXTADV_STRINGS_H

#include <string>
string rooms_name[6] = {"Entry Hall",
                        "The Drawing Room",
                        "The Kitchen",
                        "The Bedroom",
                        "The Library",
                        "The Hidden Room"
};
string rooms_desc[6]= {"You find yourself in the Entry Hall of A massive mansion. It's a dimly lit room. There are passages to the North, West, South, and East. Beckoning you forward to find the hidden room in the mansion.",
                       "You are now in The Drawing Room. The walls are painted in deep, rich blue, giving it an enchanting ambiance. Going back West will help you get closer.",
                       "As you step forward, you enter The Kitchen. The air is filled with mystery, but there is no Hidden room here better go back South.",
                       "You venture into The Bedroom. The darkness adds an eerie touch to your surroundings, You better go back East.",
                       "You venture into The Bedroom. The darkness adds an eerie touch to your surroundings, You better go back East.",
                       "Congrats You've found The Hidden Room. The room is shrouded in darkness, and your footsteps echo mysteriously. There's a passages back north, inviting you to explore further."
};
const std::string badExit = "You can't go that way.";
const std::string badCommand = "I don't understand that.";

#endif // TEXTADV_STRINGS_H