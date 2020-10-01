#include "input.h"

//test if a key is down
bool keyDown(key::Key key)
{
	return GetAsyncKeyState(key);
}

bool leftMouseDown()
{
	return (GetKeyState(VK_LBUTTON) & 0x100) != 0;
}

//returns current mouse position relative to top left corner of screen
glm::vec2 getMousePos()
{
	POINT pointPos;
	GetCursorPos(&pointPos);

	glm::vec2 pos;
	pos.x = pointPos.x;
	pos.y = pointPos.y;

	return pos;
}

//does the same thing as getMousPos() but set instead
void setMousePos(glm::vec2 pos)
{
	SetCursorPos(pos.x, pos.y);
}