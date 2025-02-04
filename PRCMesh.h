#pragma once
#include <array>
#include <vector>
#include "Vertex.h"
struct PreCompiledMesh
{
public:
	virtual constexpr std::string GetName() = 0;
	virtual constexpr void LoadTris(std::vector<Vertex>* vertices) = 0;
    virtual constexpr void LoadVert(std::vector<unsigned int>* triangles) = 0;
	constexpr void LoadMesh(std::vector<Vertex>* vertices, std::vector<unsigned int>* triangles)
	{
		LoadTris(vertices);
		LoadVert(triangles);
	};
};

struct PRC_Cube :PreCompiledMesh
{
	constexpr std::string GetName() override { return "Cube"; }
	static constexpr std::array < fakeVertex,24> vers =
	{
		{
		//vert					//normal		// uv
		{-0.5f, 0.5f, 0.5f,		1.f,1.f,1.f,	0.f,0.f},
		{-0.5f, 0.5f,-0.5f,		1.f,1.f,1.f,	1.f,0.f},
		{ 0.5f, 0.5f, 0.5f,		1.f,1.f,1.f,	0.f,1.f},
		{ 0.5f, 0.5f,-0.5f,		1.f,1.f,1.f,	1.f,1.f},//Top
		{-0.5f,-0.5f, 0.5f,		1.f,1.f,1.f,	0.f,0.f},
		{-0.5f,-0.5f,-0.5f,		1.f,1.f,1.f,	0.f,1.f},
		{ 0.5f,-0.5f, 0.5f,		1.f,1.f,1.f,	1.f,0.f},
		{ 0.5f,-0.5f,-0.5f,		1.f,1.f,1.f,	1.f,1.f},//Down
		{-0.5f, 0.5f, 0.5f,		1.f,1.f,1.f,	0.f,0.f},
		{-0.5f,-0.5f, 0.5f,		1.f,1.f,1.f,	0.f,1.f},
		{-0.5f , 0.5f,-0.5f,	1.f,1.f,1.f,	1.f,0.f},
		{-0.5f ,-0.5f,-0.5f,	1.f,1.f,1.f,	1.f,1.f},//Left
		{ 0.5f, 0.5f, 0.5f,		1.f,1.f,1.f,	0.f,0.f},
		{ 0.5f,-0.5f, 0.5f,		1.f,1.f,1.f,	0.f,1.f},
		{ 0.5f , 0.5f,-0.5f,	1.f,1.f,1.f,	1.f,0.f},
		{ 0.5f ,-0.5f,-0.5f,	1.f,1.f,1.f,	1.f,1.f},//right
		{-0.5f, 0.5f, 0.5f,		1.f,1.f,1.f,	0.f,0.f},
		{-0.5f,-0.5f, 0.5f,		1.f,1.f,1.f,	0.f,1.f},
		{ 0.5f, 0.5f, 0.5f,		1.f,1.f,1.f,	1.f,0.f},
		{ 0.5f,-0.5f, 0.5f,		1.f,1.f,1.f,	1.f,1.f},// Front
		{ -0.5f, 0.5f,-0.5f,	1.f,1.f,1.f,	0.f,0.f},
		{ -0.5f,-0.5f,-0.5f,	1.f,1.f,1.f,	0.f,1.f},
		{ 0.5f , 0.5f,-0.5f,	1.f,1.f,1.f,	1.f,0.f},
		{ 0.5f ,-0.5f,-0.5f,	1.f,1.f,1.f,	1.f,1.f}//back
		}
	};
	static constexpr std::array < uint32_t,36> tris =
	{
		{
		0,1,2,
		3,2,1,//Top
		6,5,4,
		5,6,7,//Bottom
		8,9,10,
		11,10,9,//Lelt
		14,13,12,
		13,14,15,//right
		18,17,16,
		17,18,19,//front
		20,21,22,
		23,22,21
		}
	};

	constexpr void LoadTris(std::vector<Vertex>* vertices) override
	{
		vertices->operator= (std::vector<Vertex>(vers.begin(), vers.end()));
	}
	constexpr void LoadVert(std::vector<unsigned int>* vertices) override
	{
		vertices->operator= (std::vector<unsigned int>(tris.begin(), tris.end()));
	}
};

