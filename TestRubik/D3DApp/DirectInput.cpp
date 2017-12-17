#include "stdafx.h"
#include "DirectInput.h"

extern DirectInput* gDInput = nullptr;

DirectInput::DirectInput(DWORD keyboardCoopFlags, DWORD mouseCoopFlags)
{
	ZeroMemory(mKeyboardState, sizeof(mKeyboardState));
	ZeroMemory(&mMouseState, sizeof(mMouseState));

	HR(DirectInput8Create(gD3DApp->getAppInstance(), DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&mdInput, 0));

	HR(mdInput->CreateDevice(GUID_SysKeyboard, &mKeyBoard, 0));
	HR(mKeyBoard->SetDataFormat(&c_dfDIKeyboard));
	HR(mKeyBoard->SetCooperativeLevel(gD3DApp->getMainWnd(), keyboardCoopFlags));

	HR(mKeyBoard->Acquire());

	HR(mdInput->CreateDevice(GUID_SysMouse, &mMouse, 0));
	HR(mMouse->SetDataFormat(&c_dfDIMouse2));
	HR(mMouse->SetCooperativeLevel(gD3DApp->getMainWnd(), mouseCoopFlags));

	HR(mMouse->Acquire());
}


DirectInput::~DirectInput()
{
	ReleaseCOM(mdInput);
	mKeyBoard->Unacquire();
	mMouse->Unacquire();
	ReleaseCOM(mKeyBoard);
	ReleaseCOM(mMouse);
}

void DirectInput::poll()
{
	HRESULT hr = mKeyBoard->GetDeviceState(sizeof(mKeyboardState), (void**)&mKeyboardState);

	if (FAILED(hr))
	{
		ZeroMemory(mKeyboardState, sizeof(mKeyboardState));

		hr = mKeyBoard->Acquire();
	}

	hr = mMouse->GetDeviceState(sizeof(DIMOUSESTATE2), (void**)&mMouseState);

	if (FAILED(hr))
	{
		ZeroMemory(&mMouseState, sizeof(mMouseState));

		hr = mMouse->Acquire();
	}
}

bool DirectInput::keyDown(char key)
{
	return (mKeyboardState[key] & 0x80) != 0;
}

bool DirectInput::mouseButtonDown(int button)
{
	return (mMouseState.rgbButtons[button] & 0x80) != 0;
}

float DirectInput::mouseDX()
{
	return (float)mMouseState.lX;
}

float DirectInput::mouseDY()
{
	return (float)mMouseState.lY;
}

float DirectInput::mouseDZ()
{
	return (float)mMouseState.lZ;
}
