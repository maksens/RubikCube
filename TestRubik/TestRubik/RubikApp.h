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

private:

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
	D3DXMATRIX mRotX;
	D3DXMATRIX mRotY;
	D3DXMATRIX mRotZ;

	float mCurrentRotX;
	float mCurrentRotY;
	float mCurrentRotZ;

	Rubik* mRubik;
};
