#pragma once
#include <dinput.h>
#include "D3DApp.h"

#define DIRECTINPUT_VERSION 0x0800

class DirectInput
{
public:
	DirectInput(DWORD keyboardCoopFlags, DWORD mouseCoopFlags);
	~DirectInput();

	void poll();
	bool keyDown(char key);
	bool mouseButtonDown(int button);
	float mouseDX();
	float mouseDY();
	float mouseDZ();

private:
	DirectInput(const DirectInput& rhs);
	DirectInput& operator=(const DirectInput& rhs);

private:

	IDirectInput8* mdInput;

	IDirectInputDevice8* mKeyBoard;
	char mKeyboardState[256];

	IDirectInputDevice8* mMouse;
	DIMOUSESTATE2 mMouseState;

};

extern DirectInput* gDInput;