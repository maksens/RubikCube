#pragma once

#include "Rubik.h"
#include <math.h>
#include <time.h>

class RubikApp
	: public D3DApp
{

public:
	RubikApp();
	RubikApp(HINSTANCE hInstance, int nCmdShow);
	~RubikApp();

	void Update();
	void Draw();
	//void ChangeXZ(Cube* const c, const Orientation& const o, const Layer& const layer);
	//void ChangeXY(Cube* const c, const Orientation& const o, const Layer& const layer);
	//void ChangeYZ(Cube* const c, const Orientation& const o, const Layer& const layer);
	//bool Rotate(const Orientation& const o, const Layer& const layer, float currentAngle);
	//void ShuffleRubik();

private:
	/*bool mShufflingHasEnded = false;
	float mTimer;*/

	ID3DXEffect* mFx;
	ID3DXBuffer* mErrors;

	D3DXHANDLE mhTech;
	D3DXHANDLE mhWVP;

	// Camera
	D3DXMATRIX mView;
	D3DXMATRIX mProj;
	D3DXVECTOR3 mEyePos;
	D3DXVECTOR3 mTarget;
	D3DXVECTOR3 mUp;

	// Matrix
	D3DXMATRIX mWVP;
	//D3DXMATRIX mRotX;
	//D3DXMATRIX mRotY;
	//D3DXMATRIX mRotZ;
	//D3DXMATRIX mRotXInv;
	//D3DXMATRIX mRotYInv;
	//D3DXMATRIX mRotZInv;
	//D3DXVECTOR4 mTransformVec;

	/*Cube* mCubes[26];

	ShuffleMove mShuffleMoves[25];
	int currentMove;*/

	Rubik* mRubik;
};
