#pragma once

#include "D3DApp.h"
#include "Vertex.h"

#define CUBE_SIZE 1.9
#define NB_VERTICES 24
#define NB_FACES 6
#define VERTICESXFACES 36

class Cube
{
public:
	Cube();
	Cube(int x, int y, int z);
	~Cube();

	void Draw();

	inline VertexPosCol* const GetCubeVertices() const { return mVertices; }
	inline void SetCubeVertices(VertexPosCol* v) { mVertices = v; }
	inline IDirect3DIndexBuffer9* const GetCubeIB() const { return mIB; }
	inline IDirect3DVertexBuffer9* const GetCubeVB() const { return mVB; }
	inline D3DXMATRIX const GetWorld() { return mRotX * mRotY * mRotZ * mTransl * mScale; }
	inline int const GetPosX() const { return mX; }
	inline int const GetPosY() const { return mY; }
	inline int const GetPosZ() const { return mZ; }

private:
	D3DXMATRIX mRotX;
	D3DXMATRIX mRotY;
	D3DXMATRIX mRotZ;
	D3DXMATRIX mTransl;
	D3DXMATRIX mScale;

	VertexPosCol* mVertices;
	IDirect3DVertexBuffer9* mVB;
	IDirect3DIndexBuffer9* mIB;

	int mX, mY, mZ;
};
