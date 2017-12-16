#include "stdafx.h"
#include "RubikApp.h"


RubikApp::RubikApp()
{
}


RubikApp::RubikApp(HINSTANCE hInstance, int nCmdShow)
	: D3DApp(hInstance, nCmdShow)
	, mCurrRot(0.f)
	, mEyePos(10.f, 10.f, 10.f)
	, mTarget(0.f, 0.f, 0.f)
	, mUP(0.f, 1.0f, 0.f)
	, mTransformVec(1, 1, 1, 0)
{
	CreateAllVertexDeclaration();

	int currentCube = 0;

	for (int x = -NB_CUBES; x < NB_CUBES - 1; x += 2)
	{
		for (int y = -NB_CUBES; y < NB_CUBES - 1; y += 2)
		{
			for (int z = -NB_CUBES + 2; z < NB_CUBES + 1; z += 2)
			{
				bool isTheMiddleCube = (x == -1 && y == -1 && z == 1);

				if (!isTheMiddleCube)
					mCubes[currentCube++] = new Cube(x, y, z);
			}
		}
	}

	HR(D3DXCreateEffectFromFileW(
		gD3DDevice, L"Transform.fx", 0, 0, D3DXSHADER_DEBUG, 0, &mFx, &mErrors));

	if (mErrors)
	{
		MessageBoxW(0, (LPCWSTR)mErrors->GetBufferPointer(), 0, 0);
	}

	mhTech = mFx->GetTechniqueByName("TransformTech");
	mFx->SetTechnique(mhTech);

	mhWVP = mFx->GetParameterByName(0, "gWVP");

	D3DXMatrixLookAtLH(&mView, &mEyePos, &mTarget, &mUP);
	D3DXMatrixPerspectiveFovLH(&mProj, D3DX_PI / 4,
		(float)d3dPP.BackBufferWidth / (float)d3dPP.BackBufferHeight, 0.01f, 1000.0f);

	D3DXMatrixRotationAxis(&mRotX, &D3DXVECTOR3(1, 0, 0), 0.0001f);
	D3DXMatrixRotationAxis(&mRotY, &D3DXVECTOR3(0, 1, 0), 0.0001f);
	D3DXMatrixRotationAxis(&mRotZ, &D3DXVECTOR3(0, 0, 1), 0.0001f);

	for each (Cube* var in mCubes)
	{
		std::cout << var->GetPosY() << std::endl;
	}
}

RubikApp::~RubikApp()
{
	delete[] mCubes;
	
	DestroyAllDeclaration();
}

void RubikApp::Update()
{
	mCurrRot += 0.0001f;
}

void RubikApp::Draw()
{
	HR(gD3DDevice->Clear(0, 0,
		D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER,
		D3DCOLOR_XRGB(128, 128, 128), 1.0f, 0));

	// Compute WVP

	mWVP = mView * mProj;
	mFx->SetMatrix(mhWVP, &mWVP);

	//Draw 3D
	HR(gD3DDevice->BeginScene());
	UINT nbPass;
	HR(mFx->Begin(&nbPass, 0));
	for (int i = 0; i < nbPass; i++)
	{
		HR(mFx->BeginPass(i));

		for each (Cube* c in mCubes)
		{
			if (c->GetPosX() == -3)
			{
				auto vertices = c->GetCubeVertices();

				for (int v = 0; v < NB_VERTICES; ++v)
				{
					D3DXVec3Transform(&mTransformVec, &vertices[v].pos, &(mRotX));
					vertices[v].pos.x = mTransformVec.x;
					vertices[v].pos.y = mTransformVec.y;
					vertices[v].pos.z = mTransformVec.z;
				}
			}
			c->Draw();
		}

		HR(mFx->EndPass());
	}

	HR(mFx->End());

	HR(gD3DDevice->EndScene());

	HR(gD3DDevice->Present(0, 0, 0, 0));
}
