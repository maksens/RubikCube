#include "stdafx.h"
#include "RubikApp.h"


RubikApp::RubikApp()
{
}


RubikApp::RubikApp(HINSTANCE hInstance, int nCmdShow)
	: D3DApp(hInstance, nCmdShow)
	/*, currentMove(0)
	, mTimer(0.f)*/
	, mEyePos(10.f, 10.f, 10.f)
	, mTarget(0.f, 0.f, 0.f)
	, mUp(0.f, 1.0f, 0.f)
	//, mTransformVec(1, 1, 1, 0)
{
	CreateAllVertexDeclaration();

	mRubik = new Rubik();
	/*int currentCube = 0;

	for (int x = -NB_CUBES; x < NB_CUBES - 1; x += 2)
	{
		for (int y = -NB_CUBES; y < NB_CUBES - 1; y += 2)
		{
			for (int z = -NB_CUBES + 2; z < NB_CUBES + 1; z += 2)
			{
				bool isTheMiddleCube = (x == -1 && y == -1 && z == 1);

				if (!isTheMiddleCube)
				{
					mCubes[currentCube] = new Cube(x, y, z, currentCube);
					currentCube++;
				}
			}
		}
	}*/

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

	//D3DXMatrixRotationAxis(&mRotX, &D3DXVECTOR3(-1, 0, 0), ROTATION_SPEED);
	//D3DXMatrixRotationAxis(&mRotY, &D3DXVECTOR3(0, 1, 0), ROTATION_SPEED);
	//D3DXMatrixRotationAxis(&mRotZ, &D3DXVECTOR3(0, 0, 1), ROTATION_SPEED);
	//D3DXMatrixRotationAxis(&mRotXInv, &D3DXVECTOR3(1, 0, 0), ROTATION_SPEED);
	//D3DXMatrixRotationAxis(&mRotYInv, &D3DXVECTOR3(0, -1, 0), ROTATION_SPEED);
	//D3DXMatrixRotationAxis(&mRotZInv, &D3DXVECTOR3(0, 0, -1), ROTATION_SPEED);

	mRubik->ShuffleRubik();
}

RubikApp::~RubikApp()
{
	//delete[] mCubes;
	delete mRubik;

	DestroyAllDeclaration();
}

void RubikApp::Update()
{
	mRubik->Update();
	//mShufflingHasEnded = currentMove >= 25;

	//if (Rotate(mShuffleMoves[currentMove].orientation, mShuffleMoves[currentMove].layer, mTimer) && !mShufflingHasEnded)
	//{
	//	Layer l = mShuffleMoves[currentMove].layer;
	//	Orientation o = mShuffleMoves[currentMove].orientation;

	//	bool xNotMoving = (l == X1 || l == X2 || l == X3);
	//	bool yNotMoving = (l == Y1 || l == Y2 || l == Y3);
	//	bool zNotMoving = (l == Z1 || l == Z2 || l == Z3);

	//	if (xNotMoving)
	//	{
	//		for each (Cube* c in mCubes)
	//		{
	//			ChangeYZ(c, o, l);
	//		}
	//	}
	//	else if (yNotMoving)
	//	{
	//		for each (Cube* c in mCubes)
	//		{
	//			ChangeXZ(c, o, l);
	//		}
	//	}
	//	else if (zNotMoving)
	//	{
	//		for each (Cube* c in mCubes)
	//		{
	//			ChangeXY(c, o, l);
	//		}
	//	}

	//	currentMove++;
	//	mTimer = 0;
	//}
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

//void RubikApp::ChangeXZ(Cube* const c, const Orientation& const o, const Layer& const layer)
//{
//	int posX = c->GetPosX();
//	int posY = c->GetPosY();
//	int posZ = c->GetPosZ();
//
//	int Boundaries = 0;
//
//	if (layer == Y1)
//		Boundaries = MIN_Y;
//	else if (layer == Y2)
//		Boundaries = MIDDLE_Y;
//	else if (layer == Y3)
//		Boundaries = MAX_Y;
//
//	if (posY == Boundaries)
//	{
//		if (posZ != MIDDLE_Z || posX != MIDDLE_X)
//		{
//			if (o == Left)
//			{
//				c->SetPosX(posZ - 2);
//				c->SetPosZ(-posX);
//			}
//			else if (o == Right)
//			{
//				c->SetPosX(-posZ);
//				c->SetPosZ(posX + 2);
//			}
//		}
//	}
//}
//
//void RubikApp::ChangeXY(Cube* const c, const Orientation& const o, const Layer& const layer)
//{
//	int posX = c->GetPosX();
//	int posY = c->GetPosY();
//	int posZ = c->GetPosZ();
//
//	int Boundaries = 0;
//
//	if (layer == Z1)
//		Boundaries = MIN_Z;
//	else if (layer == Z2)
//		Boundaries = MIDDLE_Z;
//	else if (layer == Z3)
//		Boundaries = MAX_Z;
//
//	if (posZ == Boundaries)
//	{
//		if (posY != MIDDLE_Y || posX != MIDDLE_X)
//		{
//			if (o == FrontRight)
//			{
//				if (posY == MIN_Y)
//					c->SetPosX(posY + 4);
//				else if (posY == MAX_Y)
//					c->SetPosX(posY - 4);
//				else if (posY == MIDDLE_Y)
//					c->SetPosX(posY);
//
//				c->SetPosY(posX);
//			}
//			else if (o == FrontLeft)
//			{
//				if (posX == MIN_X)
//					c->SetPosY(posX + 4);
//				else if (posX == MAX_X)
//					c->SetPosY(posX - 4);
//				else if (posY == MIDDLE_Y || posX == MIDDLE_X)
//					c->SetPosY(posX);
//
//				c->SetPosX(posY);
//			}
//		}
//	}
//}
//
//void RubikApp::ChangeYZ(Cube* const c, const Orientation& const o, const Layer& const layer)
//{
//	int posX = c->GetPosY();
//	int posY = c->GetPosX();
//	int posZ = c->GetPosZ();
//
//	int Boundaries = 0;
//
//	if (layer == X1)
//		Boundaries = MIN_X;
//	else if (layer == X2)
//		Boundaries = MIDDLE_X;
//	else if (layer == X3)
//		Boundaries = MAX_X;
//
//
//	if (posY == Boundaries)
//	{
//		if (posZ != MIDDLE_Z || posX != MIDDLE_X)
//		{
//			if (o == Up)
//			{
//				c->SetPosY(posZ - 2);
//				c->SetPosZ(-posX);
//			}
//			else if (o == Down)
//			{
//				c->SetPosY(-posZ);
//				c->SetPosZ(posX + 2);
//			}
//		}
//	}
//}

//bool RubikApp::Rotate(const Orientation& const o, const Layer& const layer, float currentAngle)
//{
//
//	if (currentAngle > ROTATION_ANGLE)
//	{
//		return true;
//	}
//	else
//	{
//		switch (layer)
//		{
//		case Layer::X1:
//			for each (Cube* c in mCubes)
//			{
//				if (c->GetPosX() == MIN_X)
//				{
//					auto vertices = c->GetCubeVertices();
//
//					for (int v = 0; v < NB_VERTICES; ++v)
//					{
//						if (o == Up)
//							D3DXVec3Transform(&mTransformVec, &vertices[v].pos, &(mRotX));
//						else if (o == Down)
//							D3DXVec3Transform(&mTransformVec, &vertices[v].pos, &(mRotXInv));
//
//						vertices[v].pos.x = mTransformVec.x;
//						vertices[v].pos.y = mTransformVec.y;
//						vertices[v].pos.z = mTransformVec.z;
//					}
//				}
//			}
//			break;
//		case Layer::X2:
//			for each (Cube* c in mCubes)
//			{
//				if (c->GetPosX() == MIDDLE_X)
//				{
//					auto vertices = c->GetCubeVertices();
//
//					for (int v = 0; v < NB_VERTICES; ++v)
//					{
//						if (o == Up)
//							D3DXVec3Transform(&mTransformVec, &vertices[v].pos, &(mRotX));
//						else if (o == Down)
//							D3DXVec3Transform(&mTransformVec, &vertices[v].pos, &(mRotXInv));
//
//						vertices[v].pos.x = mTransformVec.x;
//						vertices[v].pos.y = mTransformVec.y;
//						vertices[v].pos.z = mTransformVec.z;
//					}
//				}
//			}
//			break;
//		case Layer::X3:
//			for each (Cube* c in mCubes)
//			{
//				if (c->GetPosX() == MAX_X)
//				{
//					auto vertices = c->GetCubeVertices();
//
//					for (int v = 0; v < NB_VERTICES; ++v)
//					{
//						if (o == Up)
//							D3DXVec3Transform(&mTransformVec, &vertices[v].pos, &(mRotX));
//						else if (o == Down)
//							D3DXVec3Transform(&mTransformVec, &vertices[v].pos, &(mRotXInv));
//
//						vertices[v].pos.x = mTransformVec.x;
//						vertices[v].pos.y = mTransformVec.y;
//						vertices[v].pos.z = mTransformVec.z;
//					}
//				}
//			}
//			break;
//		case Layer::Y1:
//			for each (Cube* c in mCubes)
//			{
//				if (c->GetPosY() == MIN_Y)
//				{
//					auto vertices = c->GetCubeVertices();
//
//					for (int v = 0; v < NB_VERTICES; ++v)
//					{
//						if (o == Left)
//							D3DXVec3Transform(&mTransformVec, &vertices[v].pos, &(mRotY));
//						else if (o == Right)
//							D3DXVec3Transform(&mTransformVec, &vertices[v].pos, &(mRotYInv));
//
//						vertices[v].pos.x = mTransformVec.x;
//						vertices[v].pos.y = mTransformVec.y;
//						vertices[v].pos.z = mTransformVec.z;
//					}
//				}
//			}
//			break;
//		case Layer::Y2:
//			for each (Cube* c in mCubes)
//			{
//				if (c->GetPosY() == MIDDLE_Y)
//				{
//					auto vertices = c->GetCubeVertices();
//
//					for (int v = 0; v < NB_VERTICES; ++v)
//					{
//						if (o == Left)
//							D3DXVec3Transform(&mTransformVec, &vertices[v].pos, &(mRotY));
//						else if (o == Right)
//							D3DXVec3Transform(&mTransformVec, &vertices[v].pos, &(mRotYInv));
//
//						vertices[v].pos.x = mTransformVec.x;
//						vertices[v].pos.y = mTransformVec.y;
//						vertices[v].pos.z = mTransformVec.z;
//					}
//
//				}
//			}
//			break;
//		case Layer::Y3:
//			for each (Cube* c in mCubes)
//			{
//				if (c->GetPosY() == MAX_Y)
//				{
//					auto vertices = c->GetCubeVertices();
//
//					for (int v = 0; v < NB_VERTICES; ++v)
//					{
//						if (o == Left)
//							D3DXVec3Transform(&mTransformVec, &vertices[v].pos, &(mRotY));
//						else if (o == Right)
//							D3DXVec3Transform(&mTransformVec, &vertices[v].pos, &(mRotYInv));
//
//						vertices[v].pos.x = mTransformVec.x;
//						vertices[v].pos.y = mTransformVec.y;
//						vertices[v].pos.z = mTransformVec.z;
//					}
//
//				}
//			}
//			break;
//		case Layer::Z1:
//			for each (Cube* c in mCubes)
//			{
//				if (c->GetPosZ() == MIN_Z)
//				{
//					auto vertices = c->GetCubeVertices();
//
//					for (int v = 0; v < NB_VERTICES; ++v)
//					{
//						if (o == FrontLeft)
//							D3DXVec3Transform(&mTransformVec, &vertices[v].pos, &(mRotZInv));
//						else if (o == FrontRight)
//							D3DXVec3Transform(&mTransformVec, &vertices[v].pos, &(mRotZ));
//
//						vertices[v].pos.x = mTransformVec.x;
//						vertices[v].pos.y = mTransformVec.y;
//						vertices[v].pos.z = mTransformVec.z;
//					}
//
//				}
//			}
//			break;
//		case Layer::Z2:
//			for each (Cube* c in mCubes)
//			{
//				if (c->GetPosZ() == MIDDLE_Z)
//				{
//					auto vertices = c->GetCubeVertices();
//
//					for (int v = 0; v < NB_VERTICES; ++v)
//					{
//						if (o == FrontLeft)
//							D3DXVec3Transform(&mTransformVec, &vertices[v].pos, &(mRotZInv));
//						else if (o == FrontRight)
//							D3DXVec3Transform(&mTransformVec, &vertices[v].pos, &(mRotZ));
//
//						vertices[v].pos.x = mTransformVec.x;
//						vertices[v].pos.y = mTransformVec.y;
//						vertices[v].pos.z = mTransformVec.z;
//					}
//
//				}
//			}
//			break;
//		case Layer::Z3:
//			for each (Cube* c in mCubes)
//			{
//				if (c->GetPosZ() == MAX_Z)
//				{
//					auto vertices = c->GetCubeVertices();
//
//					for (int v = 0; v < NB_VERTICES; ++v)
//					{
//						if (o == FrontLeft)
//							D3DXVec3Transform(&mTransformVec, &vertices[v].pos, &(mRotZInv));
//						else if (o == FrontRight)
//							D3DXVec3Transform(&mTransformVec, &vertices[v].pos, &(mRotZ));
//
//						vertices[v].pos.x = mTransformVec.x;
//						vertices[v].pos.y = mTransformVec.y;
//						vertices[v].pos.z = mTransformVec.z;
//					}
//
//				}
//			}
//			break;
//		}
//	}
//	mTimer += ROTATION_SPEED;
//	return false;
//}

//void RubikApp::ShuffleRubik()
//{
//	// Randomize several moves to shuffle the rubbik
//	for (int i = 0; i < 25; ++i)
//	{
//		// Randomize a number that fits into Layer range
//		int layerRand = rand() % 9;
//		Layer l = (Layer)layerRand;
//
//		// Randomize a number that fits into Orientation range and that fits with the layer choice too
//		int orientRand = rand() % 2;
//
//		if (layerRand > 5)
//			orientRand += 4;
//		else if (layerRand > 2)
//			orientRand += 2;
//
//		Orientation o = (Orientation)orientRand;
//
//		// Filling in the array with the new move
//		mShuffleMoves[i] = ShuffleMove(l, o);
//	}
//}

