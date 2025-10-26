#include <functional>
#include <string>
#include "Player.h"

struct Upgrade
{
	std::string description;
	std::function<void(Player&)> apply;
};