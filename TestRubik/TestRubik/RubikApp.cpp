#include "stdafx.h"
#include "RubikApp.h"


RubikApp::RubikApp()
{
}


RubikApp::RubikApp(HINSTANCE hInstance, int nCmdShow)
	: D3DApp(hInstance, nCmdShow)
	, mEyePos(10.f, 10.f, 10.f)
	, mTarget(0.f, 0.f, 0.f)
	, mUp(0.f, 1.0f, 0.f)
	, mCurrentRotX(0.f)
	, mCurrentRotY(0.f)
	, mCurrentRotZ(0.f)
{
	CreateAllVertexDeclaration();

	mRubik = new Rubik();

	HR(D3DXCreateEffectFromFileW(
		gD3DDevice, L"Transform.fx", 0, 0, D3DXSHADER_DEBUG, 0, &mFx, &mErrors));

	if (mErrors)
	{
		MessageBoxW(0, (LPCWSTR)mErrors->GetBufferPointer(), 0, 0);
	}

	mhTech = mFx->GetTechniqueByName("TransformTech");
	mFx->SetTechnique(mhTech);
	mhWVP = mFx->GetParameterByName(0, "gWVP");

	D3DXMatrixLookAtLH(&mView, &mEyePos, &mTarget, &mUp);
	D3DXMatrixPerspectiveFovLH(&mProj, D3DX_PI / 4,
		(float)d3dPP.BackBufferWidth / (float)d3dPP.BackBufferHeight, 0.01f, 1000.0f);

	// Initialize the Matrix that are going to be used to rotate around the cube
	D3DXMatrixIdentity(&mRotX);
	D3DXMatrixIdentity(&mRotY);
	D3DXMatrixIdentity(&mRotZ);

	// Once everything is set, starts the game by shuffling the cube
	mRubik->ShuffleRubik();
}

RubikApp::~RubikApp()
{
	delete mRubik;

	DestroyAllDeclaration();
}

void RubikApp::Update()
{
	gDInput->poll();
	mRubik->Update();

	// Rotates the camera on the Y and the Z Axis using WASD
	// to help visualize all sides of the cube
	gDInput->keyDown(DIKEYBOARD_W) ? mCurrentRotZ += ROTATION_SPEED / 2 : gDInput->keyDown(DIKEYBOARD_S) ? mCurrentRotZ -= ROTATION_SPEED / 2 : 0;
	gDInput->keyDown(DIKEYBOARD_D) ? mCurrentRotY -= ROTATION_SPEED / 2 : gDInput->keyDown(DIKEYBOARD_A) ? mCurrentRotY += ROTATION_SPEED / 2 : 0;
}

void RubikApp::Draw()
{
	HR(gD3DDevice->Clear(0, 0,
		D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER,
		D3DCOLOR_XRGB(128, 128, 128), 1.0f, 0));

	D3DXMatrixRotationY(&mRotY, mCurrentRotY);
	D3DXMatrixRotationZ(&mRotZ, mCurrentRotZ);

	mWVP = mRotY * mRotZ * mView * mProj;
	mFx->SetMatrix(mhWVP, &mWVP);


	HR(gD3DDevice->BeginScene());
	UINT nbPass;
	HR(mFx->Begin(&nbPass, 0));

	for (int i = 0; i < nbPass; i++)
	{
		HR(mFx->BeginPass(i));

		// Draws each cube in the scene
		for each (Cube* c in mRubik->mCubes)
		{
			c->Draw();
		}
		HR(mFx->EndPass());
	}

	HR(mFx->End());
	HR(gD3DDevice->EndScene());
	HR(gD3DDevice->Present(0, 0, 0, 0));
}