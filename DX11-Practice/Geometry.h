#pragma once
#include <vector>

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
	};

	static Geometry GenerateCube();

public:
	std::vector<Vertex> vertices;
	std::vector<unsigned short> indices;
};