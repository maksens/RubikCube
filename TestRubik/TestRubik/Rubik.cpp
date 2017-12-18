#include "stdafx.h"
#include "Rubik.h"


Rubik::Rubik()
	: mTransformVec(1, 1, 1, 0)
	, mCurrentMove(25)
	, mTimer(0.f)
	, mShufflingHasEnded(true)
	, mRotationHasStarted(false)
	, mRotationHasEnded(false)
{ 
	int currentCube = 0;

	// Instantiate the cubes and set their starting position
	for (int x = -NB_CUBES; x < NB_CUBES - 1; x += CUBE_OFFSET)
	{
		for (int y = -NB_CUBES; y < NB_CUBES - 1; y += CUBE_OFFSET)
		{
			for (int z = -NB_CUBES + CUBE_OFFSET; z < NB_CUBES + 1; z += CUBE_OFFSET)
			{
				bool isTheMiddleCube = (x == MIDDLE_X && y == MIDDLE_Y && z == MIDDLE_Z);

				if (!isTheMiddleCube)
				{
					mCubes[currentCube] = new Cube(x, y, z, currentCube);
					currentCube++;
				}
			}
		}
	}

	// Initalize all the matrix that are going to be used by the cube's rotation
	D3DXMatrixRotationAxis(&mRotX, &D3DXVECTOR3(-1, 0, 0), ROTATION_SPEED);
	D3DXMatrixRotationAxis(&mRotY, &D3DXVECTOR3(0, 1, 0), ROTATION_SPEED);
	D3DXMatrixRotationAxis(&mRotZ, &D3DXVECTOR3(0, 0, 1), ROTATION_SPEED);
	D3DXMatrixRotationAxis(&mRotXInv, &D3DXVECTOR3(1, 0, 0), ROTATION_SPEED);
	D3DXMatrixRotationAxis(&mRotYInv, &D3DXVECTOR3(0, -1, 0), ROTATION_SPEED);
	D3DXMatrixRotationAxis(&mRotZInv, &D3DXVECTOR3(0, 0, -1), ROTATION_SPEED);
}


Rubik::~Rubik()
{
	for each(Cube* c in mCubes)
	{
		delete c;
		c = nullptr;
	}
}

void Rubik::Update()
{
	mShufflingHasEnded = mCurrentMove >= SHUFFLING_COUNT;

	// Start by shuffling the cube with a random set of moves
	// Every rotation increase the number of moves done and at its max, stops the shuffling
	// Once the set has been emptied the game can start and the keyboard input get unlocked
	if (!mShufflingHasEnded && Rotate(mShuffleMoves[mCurrentMove].orientation, mShuffleMoves[mCurrentMove].layer, mTimer))
	{
		ChangeRowColumLayer(mShuffleMoves[mCurrentMove].orientation, mShuffleMoves[mCurrentMove].layer);
		mCurrentMove++;
		mTimer = 0;
	}
	
	if (mShufflingHasEnded)
	{
		gDInput->poll();

		if (!mRotationHasStarted)
			CheckInputs();
		else if (mRotationHasStarted)
			MoveRowColumnLayer();
	}
}

// Once the X Axis rotation has ended swap all the cubes position that were involved in it
void Rubik::ChangeYZ(Cube * const c, const Orientation & o, const Layer & layer)
{
	int posX = c->GetPosY();
	int posY = c->GetPosX();
	int posZ = c->GetPosZ();

	int Boundaries = 0;

	// Select the right cubes that fit the layer that just moved

	if (layer == X1)
		Boundaries = MIN_X;
	else if (layer == X2)
		Boundaries = MIDDLE_X;
	else if (layer == X3)
		Boundaries = MAX_X;

	// After a rotation around the X axis is done, set the new position of each cubes in the rubik

	if (posY == Boundaries)
	{
		if (posZ != MIDDLE_Z || posX != MIDDLE_X)
		{
			if (o == Up)
			{
				c->SetPosY(posZ - CUBE_OFFSET);
				c->SetPosZ(-posX);
			}
			else if (o == Down)
			{
				c->SetPosY(-posZ);
				c->SetPosZ(posX + CUBE_OFFSET);
			}
		}
	}
}

// Once the Y Axis rotation has ended swap all the cubes position that were involved in it
void Rubik::ChangeXZ(Cube * const c, const Orientation & o, const Layer & layer)
{
	int posX = c->GetPosX();
	int posY = c->GetPosY();
	int posZ = c->GetPosZ();

	int Boundaries = 0;

	// Select the right cubes that fit the layer that just moved

	if (layer == Y1)
		Boundaries = MIN_Y;
	else if (layer == Y2)
		Boundaries = MIDDLE_Y;
	else if (layer == Y3)
		Boundaries = MAX_Y;

	// After a rotation around the Y axis is done, set the new position of each cubes in the rubik

	if (posY == Boundaries)
	{
		if (posZ != MIDDLE_Z || posX != MIDDLE_X)
		{
			if (o == Left)
			{
				c->SetPosX(posZ - CUBE_OFFSET);
				c->SetPosZ(-posX);
			}
			else if (o == Right)
			{
				c->SetPosX(-posZ);
				c->SetPosZ(posX + CUBE_OFFSET);
			}
		}
	}
}

// Once the Z Axis rotation has ended swap all the cubes position that were involved in it
void Rubik::ChangeXY(Cube * const c, const Orientation & o, const Layer & layer)
{
	int posX = c->GetPosX();
	int posY = c->GetPosY();
	int posZ = c->GetPosZ();

	int Boundaries = 0;

	// Select the right cubes that fit the layer that just moved

	if (layer == Z1)
		Boundaries = MIN_Z;
	else if (layer == Z2)
		Boundaries = MIDDLE_Z;
	else if (layer == Z3)
		Boundaries = MAX_Z;

	// After a rotation around the Z axis is done, set the new position of each cubes in the rubik

	if (posZ == Boundaries)
	{
		if (posY != MIDDLE_Y || posX != MIDDLE_X)
		{
			if (o == FrontRight)
			{
				if (posY == MIN_Y)
					c->SetPosX(posY + 4);
				else if (posY == MAX_Y)
					c->SetPosX(posY - 4);
				else if (posY == MIDDLE_Y)
					c->SetPosX(posY);

				c->SetPosY(posX);
			}
			else if (o == FrontLeft)
			{
				if (posX == MIN_X)
					c->SetPosY(posX + 4);
				else if (posX == MAX_X)
					c->SetPosY(posX - 4);
				else if (posY == MIDDLE_Y || posX == MIDDLE_X)
					c->SetPosY(posX);

				c->SetPosX(posY);
			}
		}
	}
}

// Picks all the cubes that need to be rotated with the corresponding layer and orientation and applies the corresponding
// rotation Matrix to its current position
bool Rubik::Rotate(const Orientation& o, const Layer& layer, float currentAngle)
{
	// Check if the rotation has reached its maximum and return the answer
	if (currentAngle > ROTATION_ANGLE)
	{
		mRotationHasStarted = false;
		return true;
	}
	else
	{
		// switch on all layer to apply the corresponding rotation to the right Row, Column or Layer
		switch (layer)
		{
		case Layer::X1:
			for each (Cube* c in mCubes)
			{
				if (c->GetPosX() == MIN_X)
				{
					auto vertices = c->GetCubeVertices();

					for (int v = 0; v < NB_VERTICES; ++v)
					{
						if (o == Up)
							D3DXVec3Transform(&mTransformVec, &vertices[v].pos, &(mRotX));
						else if (o == Down)
							D3DXVec3Transform(&mTransformVec, &vertices[v].pos, &(mRotXInv));

						vertices[v].pos.x = mTransformVec.x;
						vertices[v].pos.y = mTransformVec.y;
						vertices[v].pos.z = mTransformVec.z;
					}
				}
			}
			break;
		case Layer::X2:
			for each (Cube* c in mCubes)
			{
				if (c->GetPosX() == MIDDLE_X)
				{
					auto vertices = c->GetCubeVertices();

					for (int v = 0; v < NB_VERTICES; ++v)
					{
						if (o == Up)
							D3DXVec3Transform(&mTransformVec, &vertices[v].pos, &(mRotX));
						else if (o == Down)
							D3DXVec3Transform(&mTransformVec, &vertices[v].pos, &(mRotXInv));

						vertices[v].pos.x = mTransformVec.x;
						vertices[v].pos.y = mTransformVec.y;
						vertices[v].pos.z = mTransformVec.z;
					}
				}
			}
			break;
		case Layer::X3:
			for each (Cube* c in mCubes)
			{
				if (c->GetPosX() == MAX_X)
				{
					auto vertices = c->GetCubeVertices();

					for (int v = 0; v < NB_VERTICES; ++v)
					{
						if (o == Up)
							D3DXVec3Transform(&mTransformVec, &vertices[v].pos, &(mRotX));
						else if (o == Down)
							D3DXVec3Transform(&mTransformVec, &vertices[v].pos, &(mRotXInv));

						vertices[v].pos.x = mTransformVec.x;
						vertices[v].pos.y = mTransformVec.y;
						vertices[v].pos.z = mTransformVec.z;
					}
				}
			}
			break;
		case Layer::Y1:
			for each (Cube* c in mCubes)
			{
				if (c->GetPosY() == MIN_Y)
				{
					auto vertices = c->GetCubeVertices();

					for (int v = 0; v < NB_VERTICES; ++v)
					{
						if (o == Left)
							D3DXVec3Transform(&mTransformVec, &vertices[v].pos, &(mRotY));
						else if (o == Right)
							D3DXVec3Transform(&mTransformVec, &vertices[v].pos, &(mRotYInv));

						vertices[v].pos.x = mTransformVec.x;
						vertices[v].pos.y = mTransformVec.y;
						vertices[v].pos.z = mTransformVec.z;
					}
				}
			}
			break;
		case Layer::Y2:
			for each (Cube* c in mCubes)
			{
				if (c->GetPosY() == MIDDLE_Y)
				{
					auto vertices = c->GetCubeVertices();

					for (int v = 0; v < NB_VERTICES; ++v)
					{
						if (o == Left)
							D3DXVec3Transform(&mTransformVec, &vertices[v].pos, &(mRotY));
						else if (o == Right)
							D3DXVec3Transform(&mTransformVec, &vertices[v].pos, &(mRotYInv));

						vertices[v].pos.x = mTransformVec.x;
						vertices[v].pos.y = mTransformVec.y;
						vertices[v].pos.z = mTransformVec.z;
					}

				}
			}
			break;
			// Select the upper layer of the rubik and rotate its cubes
		case Layer::Y3:
			for each (Cube* c in mCubes)
			{
				if (c->GetPosY() == MAX_Y)
				{
					auto vertices = c->GetCubeVertices();

					for (int v = 0; v < NB_VERTICES; ++v)
					{
						// Depending of the orientation, applies different rotation 
						if (o == Left)
							D3DXVec3Transform(&mTransformVec, &vertices[v].pos, &(mRotY));
						else if (o == Right)
							D3DXVec3Transform(&mTransformVec, &vertices[v].pos, &(mRotYInv));

						vertices[v].pos.x = mTransformVec.x;
						vertices[v].pos.y = mTransformVec.y;
						vertices[v].pos.z = mTransformVec.z;
					}

				}
			}
			break;
		case Layer::Z1:
			for each (Cube* c in mCubes)
			{
				if (c->GetPosZ() == MIN_Z)
				{
					auto vertices = c->GetCubeVertices();

					for (int v = 0; v < NB_VERTICES; ++v)
					{
						if (o == FrontLeft)
							D3DXVec3Transform(&mTransformVec, &vertices[v].pos, &(mRotZInv));
						else if (o == FrontRight)
							D3DXVec3Transform(&mTransformVec, &vertices[v].pos, &(mRotZ));

						vertices[v].pos.x = mTransformVec.x;
						vertices[v].pos.y = mTransformVec.y;
						vertices[v].pos.z = mTransformVec.z;
					}

				}
			}
			break;
		case Layer::Z2:
			for each (Cube* c in mCubes)
			{
				if (c->GetPosZ() == MIDDLE_Z)
				{
					auto vertices = c->GetCubeVertices();

					for (int v = 0; v < NB_VERTICES; ++v)
					{
						if (o == FrontLeft)
							D3DXVec3Transform(&mTransformVec, &vertices[v].pos, &(mRotZInv));
						else if (o == FrontRight)
							D3DXVec3Transform(&mTransformVec, &vertices[v].pos, &(mRotZ));

						vertices[v].pos.x = mTransformVec.x;
						vertices[v].pos.y = mTransformVec.y;
						vertices[v].pos.z = mTransformVec.z;
					}

				}
			}
			break;
		case Layer::Z3:
			for each (Cube* c in mCubes)
			{
				if (c->GetPosZ() == MAX_Z)
				{
					auto vertices = c->GetCubeVertices();

					for (int v = 0; v < NB_VERTICES; ++v)
					{
						if (o == FrontLeft)
							D3DXVec3Transform(&mTransformVec, &vertices[v].pos, &(mRotZInv));
						else if (o == FrontRight)
							D3DXVec3Transform(&mTransformVec, &vertices[v].pos, &(mRotZ));

						vertices[v].pos.x = mTransformVec.x;
						vertices[v].pos.y = mTransformVec.y;
						vertices[v].pos.z = mTransformVec.z;
					}

				}
			}
			break;
		}
	}
	mTimer += ROTATION_SPEED;
	return false;
}

// Randomize several moves to shuffle the rubbik
void Rubik::ShuffleRubik()
{
	for (int i = 0; i < SHUFFLING_COUNT; ++i)
	{
		// Randomize a number that fits into Layer range
		int layerRand = rand() % LAYER_SIZE;
		Layer l = (Layer)layerRand;

		// Randomize a number that fits into Orientation range and that fits with the layer choice too
		int orientRand = rand() % 2;

		if (layerRand > 5)
			orientRand += 4;
		else if (layerRand > 2)
			orientRand += 2;

		Orientation o = (Orientation)orientRand;

		// Filling in the array with the new move
		mShuffleMoves[i] = ShuffleMove(l, o);
	}
}

// Check if the user is pressing the right key to do moves
// 1 2 3 are for layers X1, X2, X3
// 4 5 6 are for layers Y1, Y2, Y3
// 7 8 9 are for layers Z1, Z2, Z3
// Q and E are use to determine wich side the layer is going to rotate to
// Had to do it this way because of lack of mouse position usage
// Press SpaceBar and Left Control to shuffle the cube
void Rubik::CheckInputs()
{
	if (gDInput->keyDown(DIKEYBOARD_NUMPAD1) && gDInput->keyDown(DIKEYBOARD_Q))
	{
		mOrientation = Up;
		mLayer = X1;
		mRotationHasStarted = true;
	}
	else if (gDInput->keyDown(DIKEYBOARD_NUMPAD1) && gDInput->keyDown(DIKEYBOARD_E))
	{
		mOrientation = Down;
		mLayer = X1;
		mRotationHasStarted = true;
	}
	else if (gDInput->keyDown(DIKEYBOARD_NUMPAD2) && gDInput->keyDown(DIKEYBOARD_Q))
	{
		mOrientation = Up;
		mLayer = X2;
		mRotationHasStarted = true;
	}
	else if (gDInput->keyDown(DIKEYBOARD_NUMPAD2) && gDInput->keyDown(DIKEYBOARD_E))
	{
		mOrientation = Down;
		mLayer = X2;
		mRotationHasStarted = true;
	}
	else if (gDInput->keyDown(DIKEYBOARD_NUMPAD3) && gDInput->keyDown(DIKEYBOARD_Q))
	{
		mOrientation = Up;
		mLayer = X3;
		mRotationHasStarted = true;
	}
	else if (gDInput->keyDown(DIKEYBOARD_NUMPAD3) && gDInput->keyDown(DIKEYBOARD_E))
	{
		mOrientation = Down;
		mLayer = X3;
		mRotationHasStarted = true;
	}
	else if (gDInput->keyDown(DIKEYBOARD_NUMPAD4) && gDInput->keyDown(DIKEYBOARD_Q))
	{
		mOrientation = Left;
		mLayer = Y1;
		mRotationHasStarted = true;
	}
	else if (gDInput->keyDown(DIKEYBOARD_NUMPAD4) && gDInput->keyDown(DIKEYBOARD_E))
	{
		mOrientation = Right;
		mLayer = Y1;
		mRotationHasStarted = true;
	}
	else if (gDInput->keyDown(DIKEYBOARD_NUMPAD5) && gDInput->keyDown(DIKEYBOARD_Q))
	{
		mOrientation = Left;
		mLayer = Y2;
		mRotationHasStarted = true;
	}
	else if (gDInput->keyDown(DIKEYBOARD_NUMPAD5) && gDInput->keyDown(DIKEYBOARD_E))
	{
		mOrientation = Right;
		mLayer = Y2;
		mRotationHasStarted = true;
	}
	else if (gDInput->keyDown(DIKEYBOARD_NUMPAD6) && gDInput->keyDown(DIKEYBOARD_Q))
	{
		mOrientation = Left;
		mLayer = Y3;
		mRotationHasStarted = true;
	}
	else if (gDInput->keyDown(DIKEYBOARD_NUMPAD6) && gDInput->keyDown(DIKEYBOARD_E))
	{
		mOrientation = Right;
		mLayer = Y3;
		mRotationHasStarted = true;
	}
	else if (gDInput->keyDown(DIKEYBOARD_NUMPAD7) && gDInput->keyDown(DIKEYBOARD_Q))
	{
		mOrientation = FrontLeft;
		mLayer = Z1;
		mRotationHasStarted = true;
	}
	else if (gDInput->keyDown(DIKEYBOARD_NUMPAD7) && gDInput->keyDown(DIKEYBOARD_E))
	{
		mOrientation = FrontRight;
		mLayer = Z1;
		mRotationHasStarted = true;
	}
	else if (gDInput->keyDown(DIKEYBOARD_NUMPAD8) && gDInput->keyDown(DIKEYBOARD_Q))
	{
		mOrientation = FrontLeft;
		mLayer = Z2;
		mRotationHasStarted = true;
	}
	else if (gDInput->keyDown(DIKEYBOARD_NUMPAD8) && gDInput->keyDown(DIKEYBOARD_E))
	{
		mOrientation = FrontRight;
		mLayer = Z2;
		mRotationHasStarted = true;
	}
	else if (gDInput->keyDown(DIKEYBOARD_NUMPAD9) && gDInput->keyDown(DIKEYBOARD_Q))
	{
		mOrientation = FrontLeft;
		mLayer = Z3;
		mRotationHasStarted = true;
	}
	else if (gDInput->keyDown(DIKEYBOARD_NUMPAD9) && gDInput->keyDown(DIKEYBOARD_E))
	{
		mOrientation = FrontRight;
		mLayer = Z3;
		mRotationHasStarted = true;
	}
	else if (gDInput->keyDown(DIKEYBOARD_SPACE) && gDInput->keyDown(DIKEYBOARD_LCONTROL))
	{
		mCurrentMove = 0;
		mShufflingHasEnded = false;
	}
}

// Once the user has pressed an adequate combination of keys, the cube will rotate untill it reachs a complete rotation of 90 degres
// on the corresponding Row, Column or layer
void Rubik::MoveRowColumnLayer()
{
	if (Rotate(mOrientation, mLayer, mTimer))
	{
		ChangeRowColumLayer(mOrientation, mLayer);
		mTimer = 0;
	}
}

// Once the rotation has ended, the cube changes all cubes position that were moved during the Row, Column or Layer Rotation
void Rubik::ChangeRowColumLayer(const Orientation& o, const Layer l)
{
	Layer layer = l;
	Orientation orientation = o;

	//Check in which layer the rotation has occured
	bool xNotMoving = (layer == X1 || layer == X2 || layer == X3);
	bool yNotMoving = (layer == Y1 || layer == Y2 || layer == Y3);
	bool zNotMoving = (layer == Z1 || layer == Z2 || layer == Z3);

	// Swap all cubes position that were moved during the Row, Column or Layer Rotation
	if (xNotMoving)
	{
		for each (Cube* c in mCubes)
		{
			ChangeYZ(c, orientation, layer);
		}
		mRotationHasStarted = false;
	}
	else if (yNotMoving)
	{
		for each (Cube* c in mCubes)
		{
			ChangeXZ(c, orientation, layer);
		}
		mRotationHasStarted = false;
	}
	else if (zNotMoving)
	{
		for each (Cube* c in mCubes)
		{
			ChangeXY(c, orientation, layer);
		}
		mRotationHasStarted = false;
	}
}
