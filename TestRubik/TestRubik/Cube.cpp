#include "stdafx.h"
#include "Cube.h"


Cube::Cube()
{
}


Cube::Cube(int x, int y, int z)
{

	HR(gD3DDevice->CreateVertexBuffer(
		NB_VERTICES * sizeof(VertexPosCol), D3DUSAGE_WRITEONLY, 0, D3DPOOL_MANAGED, &mVB, 0));

	HR(mVB->Lock(0, 0, (void**)&mVertices, 0));

	// Declare all 6 faces vertices with their corresponding color
	mVertices[0] = VertexPosCol(x, y + CUBE_SIZE, z, 0xFF000000);
	mVertices[1] = VertexPosCol(x, y, z, 0xFF000000);
	mVertices[2] = VertexPosCol(x + CUBE_SIZE, y + CUBE_SIZE, z, 0xFF000000);
	mVertices[3] = VertexPosCol(x + CUBE_SIZE, y, z, 0xFF000000);

	mVertices[4] = VertexPosCol(x + CUBE_SIZE, y + CUBE_SIZE, z, 0xFFFF0000);
	mVertices[5] = VertexPosCol(x + CUBE_SIZE, y, z, 0xFFFF0000);
	mVertices[6] = VertexPosCol(x + CUBE_SIZE, y + CUBE_SIZE, z - CUBE_SIZE, 0xFFFF0000);
	mVertices[7] = VertexPosCol(x + CUBE_SIZE, y, z - CUBE_SIZE, 0xFFFF0000);

	mVertices[8] = VertexPosCol(x, y + CUBE_SIZE, z - CUBE_SIZE, 0xFF00FF00);
	mVertices[9] = VertexPosCol(x, y, z - CUBE_SIZE, 0xFF00FF00);
	mVertices[10] = VertexPosCol(x + CUBE_SIZE, y + CUBE_SIZE, z - CUBE_SIZE, 0xFF00FF00);
	mVertices[11] = VertexPosCol(x + CUBE_SIZE, y, z - CUBE_SIZE, 0xFF00FF00);

	mVertices[12] = VertexPosCol(x, y + CUBE_SIZE, z, 0xFF0000FF);
	mVertices[13] = VertexPosCol(x, y, z, 0xFF0000FF);
	mVertices[14] = VertexPosCol(x, y + CUBE_SIZE, z - CUBE_SIZE, 0xFF0000FF);
	mVertices[15] = VertexPosCol(x, y, z - CUBE_SIZE, 0xFF0000FF);

	mVertices[16] = VertexPosCol(x, y + CUBE_SIZE, z - CUBE_SIZE, 0x00000000);
	mVertices[17] = VertexPosCol(x, y + CUBE_SIZE, z, 0x00000000);
	mVertices[18] = VertexPosCol(x + CUBE_SIZE, y + CUBE_SIZE, z - CUBE_SIZE, 0x00000000);
	mVertices[19] = VertexPosCol(x + CUBE_SIZE, y + CUBE_SIZE, z, 0x00000000);

	mVertices[20] = VertexPosCol(x, y, z - CUBE_SIZE, 0x000000FF);
	mVertices[21] = VertexPosCol(x, y, z, 0x000000FF);
	mVertices[22] = VertexPosCol(x + CUBE_SIZE, y, z - CUBE_SIZE, 0x000000FF);
	mVertices[23] = VertexPosCol(x + CUBE_SIZE, y, z, 0x000000FF);

	HR(mVB->Unlock());

	HR(gD3DDevice->CreateIndexBuffer(
		VERTICESXFACES * sizeof(WORD), D3DUSAGE_WRITEONLY, D3DFMT_INDEX16, D3DPOOL_MANAGED, &mIB, 0));

	WORD* k;

	HR(mIB->Lock(0, 0, (void**)&k, 0));

	//Declare all vertices in each face of the cube
	for (int i = 0; i < VERTICESXFACES; i += NB_FACES)
	{
		k[i] = i - i / 3; 
		k[i + 1] = (i + 1) - i / 3; 
		k[i + 2] = (i + 2) - i / 3;
		k[i + 3] = (i + 2) - i / 3; 
		k[i + 4] = (i + 1) - i / 3;
		k[i + 5] = 3 + 2 * i / 3;
	}

	HR(mIB->Unlock());


	D3DXMatrixIdentity(&mScale);
}

Cube::~Cube()
{
	ReleaseCOM(mVB);
	ReleaseCOM(mIB);
}

void Cube::Draw()
{
	HR(gD3DDevice->SetStreamSource(0, mVB, 0, sizeof(VertexPosCol)));
	HR(gD3DDevice->SetVertexDeclaration(VertexPosCol::decl));
	HR(gD3DDevice->SetIndices(mIB));

	HR(gD3DDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, NB_VERTICES, 0, NB_FACES * 2));
}
