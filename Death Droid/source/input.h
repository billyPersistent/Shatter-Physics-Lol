#pragma once

#include <windows.h>
#include "glm.hpp"

//keyboard ID enum
namespace key
{
	enum Key { W = 87, S = 83, A = 65, D = 68, SPACE = 32, LSHIFT = 160, LCTRL = 162, ENTER = 13, UP = 38, DOWN = 40 };
}

bool keyDown(key::Key key);
bool leftMouseDown();

glm::vec2 getMousePos();
void setMousePos(glm::vec2 pos);