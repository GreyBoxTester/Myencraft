#pragma once
#include <vector>
#include "IndexedTriangleList.h"

#define PI 3.14159f
//#define DEG2RAD 3.14159f / 180.0f

namespace cl
{
	namespace primitive
	{
		template<typename V>
		cl::IndexedTriangleList<V>* cube()
		{
			cl::IndexedTriangleList<V>* ptr = new cl::IndexedTriangleList<V>();

			ptr->vertices.resize(8);
			ptr->vertices[0].pos = {-0.5f, -0.5f, -0.5f };
			ptr->vertices[1].pos = { 0.5f, -0.5f, -0.5f };
			ptr->vertices[2].pos = {-0.5f,  0.5f, -0.5f };
			ptr->vertices[3].pos = { 0.5f,  0.5f, -0.5f };
			ptr->vertices[4].pos = {-0.5f, -0.5f,  0.5f };
			ptr->vertices[5].pos = { 0.5f, -0.5f,  0.5f };
			ptr->vertices[6].pos = {-0.5f,  0.5f,  0.5f };
			ptr->vertices[7].pos = { 0.5f,  0.5f,  0.5f };

			ptr->indices = {
				0,2,1, 2,3,1,
				1,3,5, 3,7,5,
				2,6,3, 3,6,7,
				4,5,7, 4,7,6,
				0,4,2, 2,4,6,
				0,1,4, 1,5,4
			};

			return ptr;
		}

		template<typename V>
		cl::IndexedTriangleList<V>* cubeIndependent()
		{
			cl::IndexedTriangleList<V>* ptr = new cl::IndexedTriangleList<V>();

			ptr->vertices.resize(24);
			ptr->vertices[0].pos = { -0.5f, -0.5f, -0.5f };
			ptr->vertices[1].pos = { 0.5f, -0.5f, -0.5f };
			ptr->vertices[2].pos = { -0.5f,  0.5f, -0.5f };
			ptr->vertices[3].pos = { 0.5f,  0.5f, -0.5f };

			ptr->vertices[4].pos = { -0.5f, -0.5f, 0.5f };
			ptr->vertices[5].pos = { 0.5f, -0.5f, 0.5f };
			ptr->vertices[6].pos = { -0.5f,  0.5f, 0.5f };
			ptr->vertices[7].pos = { 0.5f,  0.5f, 0.5f };

			ptr->vertices[8].pos = { -0.5f, -0.5f, -0.5f };
			ptr->vertices[9].pos = { -0.5f, 0.5f, -0.5f };
			ptr->vertices[10].pos = { -0.5f, -0.5f, 0.5f };
			ptr->vertices[11].pos = { -0.5f, 0.5f, 0.5f };

			ptr->vertices[12].pos = { 0.5f, -0.5f, -0.5f };
			ptr->vertices[13].pos = { 0.5f, 0.5f, -0.5f };
			ptr->vertices[14].pos = { 0.5f, -0.5f, 0.5f };
			ptr->vertices[15].pos = { 0.5f, 0.5f, 0.5f };

			ptr->vertices[16].pos = { -0.5f, -0.5f, -0.5f };
			ptr->vertices[17].pos = { 0.5f, -0.5f, -0.5f };
			ptr->vertices[18].pos = { -0.5f, -0.5f, 0.5f };
			ptr->vertices[19].pos = { 0.5f, -0.5f, 0.5f };

			ptr->vertices[20].pos = { -0.5f, 0.5f, -0.5f };
			ptr->vertices[21].pos = { 0.5f, 0.5f, -0.5f };
			ptr->vertices[22].pos = { -0.5f, 0.5f, 0.5f };
			ptr->vertices[23].pos = { 0.5f, 0.5f, 0.5f };

			ptr->indices = {
				0,2,1, 2,3,1,
				4,5,7, 4,7,6,
				8,10,9, 10,11,9,
				12,13,15, 12,15,14,
				16,17,18, 18,17,19,
				20,23,21, 20,22,23
			};

			return ptr;
		}

		template<typename V>
		cl::IndexedTriangleList<V>* sphere(unsigned int detalisationLevel)
		{
			//detalisationLevel = floorf(detalisationLevel);
			cl::IndexedTriangleList<V>* ptr = new cl::IndexedTriangleList<V>();

			ptr->vertices.reserve((detalisationLevel * 2) * (detalisationLevel - 1) + 2);
			ptr->indices.reserve((detalisationLevel * 2) * (detalisationLevel - 1) * 6);

			V vert;
			vert.pos = { 0.0f, 1.0f, 0.0f };
			ptr->vertices.push_back(vert);


			for (unsigned int i = 0u; i < detalisationLevel * 2u - 1u; i++)
			{
				ptr->indices.push_back(0);
				ptr->indices.push_back(i + 2);
				ptr->indices.push_back(i + 1);
			}
			ptr->indices.push_back(0);
			ptr->indices.push_back(1);
			ptr->indices.push_back(detalisationLevel * 2u);



			for (unsigned int i = 1u; i < detalisationLevel; i++)
			{
				float y = cosf((float)i * PI / (float)detalisationLevel);
				float mult = sinf((float)i * PI / (float)detalisationLevel);

				unsigned int count = detalisationLevel * 2u;

				for (unsigned int j = 0u; j < count; j++)
				{
					vert.pos = { cosf((float)j * PI / (float)detalisationLevel) * mult, y, sinf((float)j * PI / (float)detalisationLevel) * mult };
					ptr->vertices.push_back(vert);

					if (i < detalisationLevel - 1u)
					{
						unsigned int ind = 1u + (i - 1) * detalisationLevel * 2u + j;

						if (j == count - 1u)
						{
							ptr->indices.push_back(ind);
							ptr->indices.push_back(ind + 1u);
							ptr->indices.push_back(ind + count);
							ptr->indices.push_back(ind);
							ptr->indices.push_back(ind - count + 1u);
							ptr->indices.push_back(ind + 1u);
						}
						else
						{
							ptr->indices.push_back(ind);
							ptr->indices.push_back(ind + count + 1u);
							ptr->indices.push_back(ind + count);
							ptr->indices.push_back(ind);
							ptr->indices.push_back(ind + 1u);
							ptr->indices.push_back(ind + count + 1u);
						}
					}
				}
			}


			vert.pos = { 0.0f, -1.0f, 0.0f };
			ptr->vertices.push_back(vert);

			unsigned int ind = ptr->vertices.size() - 1;
			for (unsigned int i = 0; i < detalisationLevel * 2u - 1; i++)
			{
				ptr->indices.push_back(ind);
				ptr->indices.push_back(ind - i - 2u);
				ptr->indices.push_back(ind - i - 1u);
			}
			ptr->indices.push_back(ind);
			ptr->indices.push_back(ind - 1u);
			ptr->indices.push_back(ind - detalisationLevel * 2u);

			return ptr;
		}

		template<typename V>
		cl::IndexedTriangleList<V>* cone(unsigned int sides)
		{
			cl::IndexedTriangleList<V>* ptr = new cl::IndexedTriangleList<V>();

			ptr->vertices.reserve(sides + 1u);
			ptr->indices.reserve(sides * 2u - 2u);

			V vert;
			vert.pos = { 0.0f, 1.0f, 0.0f };
			ptr->vertices.push_back(vert);

			for (unsigned int i = 0u; i < sides; i++)
			{
				vert.pos = { cosf((float)i * PI * 2.0f / (float)sides), -1.0f, sinf((float)i * PI * 2.0f / (float)sides) };
				ptr->vertices.push_back(vert);
			}

			for (unsigned int i = 1u; i < sides; i++)
			{
				ptr->indices.push_back(0);
				ptr->indices.push_back(i + 1u);
				ptr->indices.push_back(i);
			}
			ptr->indices.push_back(0);
			ptr->indices.push_back(1);
			ptr->indices.push_back(sides);

			for (unsigned int i = 2u; i < sides; i++)
			{
				ptr->indices.push_back(1);
				ptr->indices.push_back(i);
				ptr->indices.push_back(i + 1u);
			}

			return ptr;
		}

		template<typename V>
		cl::IndexedTriangleList<V>* plane(unsigned int divisions)
		{
			cl::IndexedTriangleList<V>* ptr = new cl::IndexedTriangleList<V>();

			ptr->vertices.reserve((divisions + 1u) * (divisions + 1u));
			ptr->indices.reserve(divisions * divisions * 6u);

			V vert;
			for (unsigned int i = 0; i < divisions + 1; i++)
			{
				for (unsigned int j = 0; j < divisions + 1; j++)
				{
					vert.pos = { 0.5f - ((float)i / (float)divisions), 0.0f, 0.5f - ((float)j / (float)divisions) };
					ptr->vertices.push_back(vert);
				}
			}

			for (unsigned int i = 0; i < divisions; i++)
			{
				for (unsigned int j = 0; j < divisions; j++)
				{
					unsigned int ind = i * (divisions + 1u) + j;
					ptr->indices.push_back(ind);
					ptr->indices.push_back(ind + divisions + 2);
					ptr->indices.push_back(ind + divisions + 1);
					ptr->indices.push_back(ind);
					ptr->indices.push_back(ind + 1);
					ptr->indices.push_back(ind + divisions + 2);
				}
			}

			return ptr;
		}

		template<typename V>
		cl::IndexedTriangleList<V>* planeIndependent(unsigned int divisions)
		{
			cl::IndexedTriangleList<V>* ptr = new cl::IndexedTriangleList<V>();

			ptr->vertices.reserve(divisions * divisions * 4u);
			ptr->indices.reserve(divisions * divisions * 6u);

			V vert;
			for (unsigned int i = 0; i < divisions; i++)
			{
				for (unsigned int j = 0; j < divisions; j++)
				{
					vert.pos = { 0.5f - ((float)i / (float)divisions), 0.0f, 0.5f - ((float)j / (float)divisions) };
					ptr->vertices.push_back(vert);
					vert.pos = { 0.5f - ((float)(i + 1) / (float)divisions), 0.0f, 0.5f - ((float)j / (float)divisions) };
					ptr->vertices.push_back(vert);
					vert.pos = { 0.5f - ((float)i / (float)divisions), 0.0f, 0.5f - ((float)(j + 1) / (float)divisions) };
					ptr->vertices.push_back(vert);
					vert.pos = { 0.5f - ((float)(i + 1) / (float)divisions), 0.0f, 0.5f - ((float)(j + 1) / (float)divisions) };
					ptr->vertices.push_back(vert);

					unsigned int ind = (j * divisions + i) * 4;
					ptr->indices.push_back(ind);
					ptr->indices.push_back(ind + 2);
					ptr->indices.push_back(ind + 3);
					ptr->indices.push_back(ind);
					ptr->indices.push_back(ind + 3);
					ptr->indices.push_back(ind + 1);
				}
			}

			return ptr;
		}
	}
}
