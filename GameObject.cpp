#include "GameObject.h"

GameObject::GameObject(const std::string& _shortName, const std::string& _longDescription, const std::string& _keyword)
        : shortName(_shortName), longDescription(_longDescription), keyword(_keyword) {}

const std::string& GameObject::getShortName() const{ return shortName; }
const std::string& GameObject::getLongDescription() const{ return longDescription; }
const std::string& GameObject::getKeyword() const{ return keyword; }
