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
#define ROTATION_SPEED 0.001f
#define CUBE_TOTAL 26
#define SHUFFLING_COUNT 25
#define LAYER_SIZE 9

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

// Enum used to determine which side the rotation is going to be applied to
enum Orientation
{
	Up, Down, Left, Right, FrontLeft, FrontRight
};

// Enum used to determine which Row, Column or layer is going to be rotate
enum Layer
{
	X1, X2, X3, Y1, Y2, Y3, Z1, Z2, Z3
};

// Custom Object used to store the information for the cube shuffling
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
	bool Rotate(const Orientation& o, const Layer& layer, float currentAngle);
	void ChangeXZ(Cube* const c, const Orientation& o, const Layer& layer);
	void ChangeXY(Cube* const c, const Orientation& o, const Layer& layer);
	void ChangeYZ(Cube* const c, const Orientation& o, const Layer& layer);
	void ChangeRowColumLayer(const Orientation& o, const Layer l);
	void MoveRowColumnLayer();
	void ShuffleRubik();
	void CheckInputs();

	Cube* mCubes[CUBE_TOTAL];

private:
	D3DXMATRIX mRotX;
	D3DXMATRIX mRotY;
	D3DXMATRIX mRotZ;
	D3DXMATRIX mRotXInv;
	D3DXMATRIX mRotYInv;
	D3DXMATRIX mRotZInv;
	D3DXVECTOR4 mTransformVec;

	ShuffleMove mShuffleMoves[SHUFFLING_COUNT];

	bool mRotationHasStarted;
	bool mRotationHasEnded;
	bool mShufflingHasEnded;
	float mTimer;
	int mCurrentMove;

	Orientation mOrientation;
	Layer mLayer;
};

