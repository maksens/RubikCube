#pragma once
#include "stdafx.h"
#include "resource.h"
#include "RubikApp.h"

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE prevInstance,
	_In_ LPWSTR lpCmdLine,
	_In_ int nCmdShow)
{
	gD3DApp = new RubikApp(hInstance, nCmdShow);
	srand(time(0));
	return gD3DApp->Run();
}
