// GameObject.h
#ifndef TEXTADV_GAMEOBJECT_H
#define TEXTADV_GAMEOBJECT_H

#include <string>

class GameObject {
    std::string shortName;
    std::string longDescription;
    std::string keyword;

public:
    GameObject(const std::string& _shortName, const std::string& _longDescription, const std::string& _keyword)
            : shortName(_shortName), longDescription(_longDescription), keyword(_keyword) {}

    const std::string& getShortName() const{ return shortName; }
    const std::string& getLongDescription() const{ return longDescription; }
    const std::string& getKeyword() const{ return keyword; }
};
#endif // TEXTADV_GAMEOBJECT_H