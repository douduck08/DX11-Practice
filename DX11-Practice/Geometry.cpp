# include "Geometry.h"

Geometry Geometry::GenerateCube()
{
	std::vector<Geometry::Vertex> vertices = {
		{ -0.5f,-0.5f,-0.5f },
		{  0.5f,-0.5f,-0.5f },
		{ -0.5f, 0.5f,-0.5f },
		{  0.5f, 0.5f,-0.5f },
		{ -0.5f,-0.5f, 0.5f },
		{  0.5f,-0.5f, 0.5f },
		{ -0.5f, 0.5f, 0.5f },
		{  0.5f, 0.5f, 0.5f },
	};

	std::vector<unsigned short> indices = {
		0,2,1, 2,3,1,
		1,3,5, 3,7,5,
		2,6,3, 3,6,7,
		4,5,7, 4,7,6,
		0,4,2, 2,4,6,
		0,1,4, 1,5,4
	};

	return { std::move(vertices), std::move(indices) };
}

Geometry Geometry::GenerateSphere(float radius)
{
    std::vector<Geometry::Vertex> vertices;
    std::vector<unsigned short> indices;

    float latitudeBands = 30;
    float longitudeBands = 30;

    for (float latNumber = 0; latNumber <= latitudeBands; latNumber++)
    {
        float theta = latNumber * 3.1415926f / latitudeBands;
        float sinTheta = sin(theta);
        float cosTheta = cos(theta);

        for (float longNumber = 0; longNumber <= longitudeBands; longNumber++)
        {
            float phi = longNumber * 2 * 3.1415926f / longitudeBands;
            float sinPhi = sin(phi);
            float cosPhi = cos(phi);

            float position[3], normal[3], texcoord[2];
            normal[0] = cosPhi * sinTheta;   // x
            normal[1] = cosTheta;            // y
            normal[2] = sinPhi * sinTheta;   // z
            texcoord[0] = 1 - (longNumber / longitudeBands); // u
            texcoord[1] = 1 - (latNumber / latitudeBands);   // v
            position[0] = radius * normal[0];
            position[1] = radius * normal[1];
            position[2] = radius * normal[2];

            vertices.push_back({ position[0], position[1], position[2] });
        }

        for (int latNumber = 0; latNumber < latitudeBands; latNumber++)
        {
            for (int longNumber = 0; longNumber < longitudeBands; longNumber++)
            {
                int first = (latNumber * (longitudeBands + 1)) + longNumber;
                int second = first + longitudeBands + 1;

                indices.push_back(first);
                indices.push_back(second);
                indices.push_back(first + 1);

                indices.push_back(second);
                indices.push_back(second + 1);
                indices.push_back(first + 1);
            }
        }
    }

	return { std::move(vertices), std::move(indices) };
}
