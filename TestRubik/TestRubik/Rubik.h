#pragma once

#include "Cube.h"
#include "DirectInput.h"

#pragma region CubeConstants

#define NB_FACES 4
#define VERTICES_PER_FACE 6
#define NB_CUBES 3
#define CUBE_SIZE 1.9f
#define CUBE_OFFSET 2
#define ROTATION_ANGLE (D3DX_PI / 2)
#define SHUFFLE_SPEED 0.001f
#define ROTATION_SPEED 0.001f

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

enum Orientation
{
	Up, Down, Left, Right, FrontLeft, FrontRight
};

enum Layer
{
	X1, X2, X3, Y1, Y2, Y3, Z1, Z2, Z3
};

struct ShuffleMove
{
	ShuffleMove() {}
	ShuffleMove(Layer l, Orientation o) : layer(l), orientation(o) {}

	Layer layer;
	Orientation orientation;
};

class Rubik
{
public:
	Rubik();
	~Rubik();

	void Update();
	void ChangeXZ(Cube* const c, const Orientation& o, const Layer& layer);
	void ChangeXY(Cube* const c, const Orientation& o, const Layer& layer);
	void ChangeYZ(Cube* const c, const Orientation& o, const Layer& layer);
	bool Rotate(const Orientation& o, const Layer& layer, float currentAngle);
	void ShuffleRubik();
	void CheckInputs();
	void MoveRowColumnLayer();
	void ChangeRowColumLayer(const Orientation& o, const Layer l);

	Cube* mCubes[26];

private:
	D3DXMATRIX mRotX;
	D3DXMATRIX mRotY;
	D3DXMATRIX mRotZ;
	D3DXMATRIX mRotXInv;
	D3DXMATRIX mRotYInv;
	D3DXMATRIX mRotZInv;
	D3DXVECTOR4 mTransformVec;

	ShuffleMove mShuffleMoves[25];

	bool mRotationHasStarted;
	bool mRotationHasEnded;
	bool mShufflingHasEnded;
	float mTimer;
	int mCurrentMove;

	Orientation mOrientation;
	Layer mLayer;
};

