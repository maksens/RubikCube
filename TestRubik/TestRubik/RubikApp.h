#pragma once
#pragma once

#include "math.h"
#include "Cube.h"

#define NB_FACES 4
#define VERTICES_PER_FACE 6
#define NB_CUBES 3
#define CUBE_SIZE 1.9

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
	float currRot;

	ID3DXEffect* mFx;
	ID3DXBuffer* mErrors;

	D3DXHANDLE mhTech;
	D3DXHANDLE mhWVP;
	/*D3DXHANDLE mhEyePos;
	D3DXHANDLE mhFogColor;
	D3DXHANDLE mhFogStart;
	D3DXHANDLE mhFogRange;*/

	IDirect3DVertexBuffer9* mVB;
	IDirect3DIndexBuffer9* mIB;

	// Camera
	D3DXMATRIX view;
	D3DXMATRIX proj;

	D3DXVECTOR3 eyePos;
	D3DXVECTOR3 target;
	D3DXVECTOR3 up;

	VertexPosCol square[432];
	VertexPosCol* vertices;

	D3DXMATRIX rotX;
	D3DXMATRIX rotY;
	D3DXMATRIX rotZ;
	D3DXVECTOR4 vT;

	D3DXMATRIX mT;
	D3DXMATRIX mTInverted;

	D3DXMATRIX WVP;
	D3DXMATRIX Ry;

	Cube* mCubes[26];
};
