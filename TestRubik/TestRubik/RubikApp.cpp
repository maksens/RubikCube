#include "stdafx.h"
#include "RubikApp.h"


RubikApp::RubikApp()
{
}


RubikApp::RubikApp(HINSTANCE hInstance, int nCmdShow)
	: D3DApp(hInstance, nCmdShow)
	, currRot(0.f)
	, eyePos(10.f, 10.f, 10.f)
	, target(0.f, 0.f, 0.f)
	, up(0.f, 1.0f, 0.f)
	, vT(1, 1, 1, 0)
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

	/*HR(gD3DDevice->CreateVertexBuffer(
		27 * NB_FACES * NB_FACES * sizeof(VertexPosCol), D3DUSAGE_DYNAMIC | D3DUSAGE_WRITEONLY, 0, D3DPOOL_DEFAULT, &mVB, 0));

	int currentCube = 0;

	for (int x = -3; x < NB_CUBES - 1; x+= 2)
	{
		for (int y = -3; y < NB_CUBES - 1; y+= 2)
		{
			for (int z = -1; z < NB_CUBES + 1; z+= 2)
			{
				bool isTheMiddleCube = (x == 0 && y == 0 && z == 0);
				if (!isTheMiddleCube)
				{
					int verticesCount = 0;

					square[currentCube * NB_FACES * NB_FACES + verticesCount++] = VertexPosCol(x, y + CUBE_SIZE, z, 0xFF000000);
					square[currentCube * NB_FACES * NB_FACES + verticesCount++] = VertexPosCol(x, y, z, 0xFF000000);
					square[currentCube * NB_FACES * NB_FACES + verticesCount++] = VertexPosCol(x + CUBE_SIZE, y + CUBE_SIZE, z, 0xFF000000);
					square[currentCube * NB_FACES * NB_FACES + verticesCount++] = VertexPosCol(x + CUBE_SIZE, y, z, 0xFF000000);

					square[currentCube * NB_FACES * NB_FACES + verticesCount++] = VertexPosCol(x + CUBE_SIZE, y + CUBE_SIZE, z, 0xFFFF0000);
					square[currentCube * NB_FACES * NB_FACES + verticesCount++] = VertexPosCol(x + CUBE_SIZE, y, z, 0xFFFF0000);
					square[currentCube * NB_FACES * NB_FACES + verticesCount++] = VertexPosCol(x + CUBE_SIZE, y + CUBE_SIZE, z - CUBE_SIZE, 0xFFFF0000);
					square[currentCube * NB_FACES * NB_FACES + verticesCount++] = VertexPosCol(x + CUBE_SIZE, y, z - CUBE_SIZE, 0xFFFF0000);

					square[currentCube * NB_FACES * NB_FACES + verticesCount++] = VertexPosCol(x, y + CUBE_SIZE, z - CUBE_SIZE, 0xFF00FF00);
					square[currentCube * NB_FACES * NB_FACES + verticesCount++] = VertexPosCol(x, y, z - CUBE_SIZE, 0xFF00FF00);
					square[currentCube * NB_FACES * NB_FACES + verticesCount++] = VertexPosCol(x + CUBE_SIZE, y + CUBE_SIZE, z - CUBE_SIZE, 0xFF00FF00);
					square[currentCube * NB_FACES * NB_FACES + verticesCount++] = VertexPosCol(x + CUBE_SIZE, y, z - CUBE_SIZE, 0xFF00FF00);

					square[currentCube * NB_FACES * NB_FACES + verticesCount++] = VertexPosCol(x, y + CUBE_SIZE, z, 0xFF0000FF);
					square[currentCube * NB_FACES * NB_FACES + verticesCount++] = VertexPosCol(x, y, z, 0xFF0000FF);
					square[currentCube * NB_FACES * NB_FACES + verticesCount++] = VertexPosCol(x, y + CUBE_SIZE, z - CUBE_SIZE, 0xFF0000FF);
					square[currentCube * NB_FACES * NB_FACES + verticesCount++] = VertexPosCol(x, y, z - CUBE_SIZE, 0xFF0000FF);
				}
				currentCube++;
			}
		}
	}

	HR(mVB->Lock(0, 0, (void**)&vertices, D3DLOCK_NOOVERWRITE));

	// Local de mes vertices

	for (int i = 0; i < 432; ++i)
	{
		vertices[i] = square[i];
	}

	HR(mVB->Unlock());

	// Build Index Buffer
	HR(gD3DDevice->CreateIndexBuffer(
		27 * 4 * 6 * sizeof(WORD), D3DUSAGE_WRITEONLY, D3DFMT_INDEX16, D3DPOOL_MANAGED, &mIB, 0));

	WORD* k;

	HR(mIB->Lock(0, 0, (void**)&k, 0));

	for (int i = 0; i < NB_CUBES * NB_CUBES * NB_CUBES * NB_FACES * VERTICES_PER_FACE; i+= VERTICES_PER_FACE)
	{
		k[i] = i - 2 * i/6; k[i + 1] = (i + 1) - 2 * i / 6; k[i + 2] = (i + 2) - 2 * i / 6;
		k[i + 3] = (i + 2) - 2 * i / 6; k[i + 4] = (i + 1) - 2 * i / 6; k[i + 5] = 3 + 4 * i / 6;
	}

	HR(mIB->Unlock());*/

	HR(D3DXCreateEffectFromFileW(
		gD3DDevice, L"Transform.fx", 0, 0, D3DXSHADER_DEBUG, 0, &mFx, &mErrors));

	if (mErrors)
	{
		MessageBoxW(0, (LPCWSTR)mErrors->GetBufferPointer(), 0, 0);
	}

	mhTech = mFx->GetTechniqueByName("TransformTech");
	mFx->SetTechnique(mhTech);

	mhWVP = mFx->GetParameterByName(0, "gWVP");
	/*mhEyePos = mFx->GetParameterByName(0, "gEyePos");
	mhFogColor = mFx->GetParameterByName(0, "gFogColor");
	mhFogStart = mFx->GetParameterByName(0, "gFogStart");
	mhFogRange = mFx->GetParameterByName(0, "gFogRange");*/

	/*HR(mFx->SetVector(mhFogColor, &D3DXVECTOR4(0.5f, 0.5f, 0.5f, 1.0f)));
	HR(mFx->SetFloat(mhFogStart, 5.0f));
	HR(mFx->SetFloat(mhFogRange, 25.0f));*/

	D3DXMatrixLookAtLH(&view, &eyePos, &target, &up);
	D3DXMatrixPerspectiveFovLH(&proj, D3DX_PI / 4,
		(float)d3dPP.BackBufferWidth / (float)d3dPP.BackBufferHeight, 0.01f, 1000.0f);
}

RubikApp::~RubikApp()
{
	/*ReleaseCOM(mVB);
	ReleaseCOM(mIB);*/

	delete[] mCubes;

	DestroyAllDeclaration();
}

void RubikApp::Update()
{
	currRot += 0.0001f;

	/*D3DXMatrixRotationAxis(&rotX, &D3DXVECTOR3(1, 0, 0), 0.0001);
	D3DXMatrixIdentity(&mT);
	D3DXMatrixIdentity(&mTInverted);

	for (int i = 0; i < 9 * 16; ++i)
	{
		D3DXMatrixTranslation(&mT, vertices[i].pos.x/2, vertices[i].pos.y/2, vertices[i].pos.z/2);
		D3DXMatrixTranslation(&mTInverted, -vertices[i].pos.x/2, -vertices[i].pos.y/2, -vertices[i].pos.z/2);
		D3DXVec3Transform(&vT, &vertices[i].pos, &(mT*rotX*mTInverted));
		vertices[i].pos.x = vT.x;
		vertices[i].pos.y = vT.y;
		vertices[i].pos.z = vT.z;
	}*/
}

void RubikApp::Draw()
{
	HR(gD3DDevice->Clear(0, 0,
		D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER,
		D3DCOLOR_XRGB(128, 128, 128), 1.0f, 0));

	/*HR(gD3DDevice->SetStreamSource(0, mVB, 0, sizeof(VertexPosCol)));
	HR(gD3DDevice->SetVertexDeclaration(VertexPosCol::decl));
	HR(gD3DDevice->SetIndices(mIB));*/

	//HR(mFx->SetValue(mhEyePos, &eyePos, sizeof(D3DXVECTOR3)));

	// Compute WVP

	D3DXMatrixRotationAxis(&Ry, &D3DXVECTOR3(0, 1, 0), 1.58f);
	WVP = Ry * view * proj;
	mFx->SetMatrix(mhWVP, &WVP);
	/*HR(gD3DDevice->SetVertexDeclaration(VertexPosCol::decl));*/

	//Draw 3D
	HR(gD3DDevice->BeginScene());
	UINT nbPass;
	HR(mFx->Begin(&nbPass, 0));
	for (int i = 0; i < nbPass; i++)
	{
		HR(mFx->BeginPass(i));

		for each (Cube* c in mCubes)
		{
			c->Draw();
		}
		/*HR(gD3DDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, 432, 0, 216));*/

		HR(mFx->EndPass());
	}

	HR(mFx->End());

	HR(gD3DDevice->EndScene());

	HR(gD3DDevice->Present(0, 0, 0, 0));
}
