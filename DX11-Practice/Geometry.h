#pragma once
#include <vector>
#include <string>

class Geometry
{
public:
	Geometry() = default;
	~Geometry() = default;

	struct Vertex
	{
		struct
		{
			float x;
			float y;
			float z;
		} pos;
		struct
		{
			float x;
			float y;
			float z;
		} normal;
		struct
		{
			float u;
			float v;
		} uv;
		struct
		{
			float x;
			float y;
			float z;
		} tangent;
		struct
		{
			float x;
			float y;
			float z;
		} bitangent;
	};

	static Geometry GenerateCube();
	static Geometry GenerateSphere(float radius = 0.5f, int widthSegment = 16, int heightSegment = 8);

public:
	std::vector<Vertex> vertices;
	std::vector<unsigned short> indices;
};