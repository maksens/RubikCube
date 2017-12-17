#pragma once

#include "Cube.h"
#pragma region CubeConstants

#define NB_FACES 4
#define VERTICES_PER_FACE 6
#define NB_CUBES 3
#define CUBE_SIZE 1.9f
#define ROTATION_ANGLE (D3DX_PI / 2)
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
	void ChangeXZ(Cube* const c, const Orientation& const o, const Layer& const layer);
	void ChangeXY(Cube* const c, const Orientation& const o, const Layer& const layer);
	void ChangeYZ(Cube* const c, const Orientation& const o, const Layer& const layer);
	bool Rotate(const Orientation& const o, const Layer& const layer, float currentAngle);
	void ShuffleRubik();

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

	bool mShufflingHasEnded = false;
	float mTimer;
	int currentMove;
};

