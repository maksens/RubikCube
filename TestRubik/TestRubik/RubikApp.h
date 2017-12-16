#pragma once
#pragma once

#include "math.h"
#include "Cube.h"

#pragma region CubeConstants

#define NB_FACES 4
#define VERTICES_PER_FACE 6
#define NB_CUBES 3
#define CUBE_SIZE 1.9

#pragma endregion
#pragma region LayerDeclaration

#define MAX_Y 1
#define MIDDLE_Y -1
#define MIN_Y -3

#define MAX_Z 3
#define MIDDLE_Z 1
#define MIN_Z -1

#define MAX_X 1
#define MIDDLE_X -1
#define MIN_X -3

#pragma endregion

class RubikApp
	: public D3DApp
{

public:
	RubikApp();
	RubikApp(HINSTANCE hInstance, int nCmdShow);
	~RubikApp();

	void Update();
	void Draw();

private:
	float mCurrRot;

	ID3DXEffect* mFx;
	ID3DXBuffer* mErrors;

	D3DXHANDLE mhTech;
	D3DXHANDLE mhWVP;

	// Camera
	D3DXMATRIX mView;
	D3DXMATRIX mProj;
	D3DXVECTOR3 mEyePos;
	D3DXVECTOR3 mTarget;
	D3DXVECTOR3 mUP;

	// Matrix
	D3DXMATRIX mWVP;
	D3DXMATRIX mRotX;
	D3DXMATRIX mRotY;
	D3DXMATRIX mRotZ;
	D3DXVECTOR4 mTransformVec;

	Cube* mCubes[26];
};
