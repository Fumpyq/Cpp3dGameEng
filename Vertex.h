#pragma once
#include <glm/glm.hpp>
struct v3
{
public:
	float x, y, z;
	constexpr v3() { x = 0; y = 0; z = 0; }
	constexpr v3(float X, float Y, float Z) { x = X; y = Y; z = Z; }
};
struct v2
{
public:
	float x, y;
	constexpr v2() { x = 0; y = 0; }
	constexpr v2(float X, float Y) { x = X; y = Y; }
};
struct fakeVertex
{
public:
	v3 v;
	v3 n;
	v2 uv;
	constexpr fakeVertex(float x, float y, float z, float nx, float ny, float nz, float ux, float uy)
	{
		v = v3(x, y, z);
		n = v3(nx, ny, nz);
		uv = v2(ux, uy);
	}
};
struct Vertex {
	glm::vec3 Position;
	glm::vec3 Normal;
	glm::vec2 UV;
	constexpr Vertex(fakeVertex v)
	{
		Position = glm::vec3(v.v.x, v.v.y, v.v.z);
		Normal = glm::vec3(v.n.x, v.n.y, v.n.z);
		UV = glm::vec2(v.uv.x, v.uv.y);
	}
	constexpr Vertex(glm::vec3 pos, glm::vec3 normal, glm::vec2 uv)
	{
		Position = pos;
		Normal = normal;
		UV = uv;
	}
};