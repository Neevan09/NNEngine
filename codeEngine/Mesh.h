#pragma once

#include <d3d11.h>
#include "Vertex.h"
#include <iostream>
#include <fstream>
#include <vector>
class Mesh
{
private:
	ID3D11Buffer* vertexBuffer;
	ID3D11Buffer* indexBuffer;
	int numOfIndices;
public:
	Mesh();
	Mesh(Vertex* vertexArray, int numOfVertices, unsigned int* indexArray, int numOfIndices, ID3D11Device* device);
	Mesh(const char* fileName, ID3D11Device* device);
	~Mesh();
	ID3D11Buffer* GetVertexBuffer();
	ID3D11Buffer* GetIndexBuffer();
	int GetIndexCount();
	void CreateBuffers(Vertex* vertexArray, int numOfVertices, unsigned int* indexArray, int numOfIndices, ID3D11Device* device);
};

