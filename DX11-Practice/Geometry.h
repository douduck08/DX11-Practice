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
	};

	static Geometry GenerateCube();
	static Geometry GenerateSphere(float radius = 0.5f, int widthSegment = 16, int heightSegment = 8);
	static Geometry LoadFromFile(const std::string fileName);

public:
	std::vector<Vertex> vertices;
	std::vector<unsigned short> indices;
};