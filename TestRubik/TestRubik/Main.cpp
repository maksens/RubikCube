#pragma once
#include "stdafx.h"
#include "resource.h"
#include "RubikApp.h"
#include "DirectInput.h"

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE prevInstance,
	_In_ LPWSTR lpCmdLine,
	_In_ int nCmdShow)
{
	srand(time(0));
	gD3DApp = new RubikApp(hInstance, nCmdShow);
	DirectInput di(DISCL_NONEXCLUSIVE | DISCL_FOREGROUND, DISCL_NONEXCLUSIVE | DISCL_FOREGROUND);
	gDInput = &di;
	return gD3DApp->Run();
}
