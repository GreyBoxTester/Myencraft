#pragma once
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include <vector>

namespace cl
{
	template<typename T>
	class IndexedTriangleList
	{
	public:
		IndexedTriangleList() {}

		~IndexedTriangleList()
		{
			vertices.clear();
			indices.clear();
		}

		void calculateNormals()
		{
			for (size_t i = 0u; i < indices.size(); i += 3)
			{
				T& v0 = vertices[indices[i]];
				T& v1 = vertices[indices[i+1]];
				T& v2 = vertices[indices[i+2]];

				v0.normal = v1.normal = v2.normal = cl::normalize(cl::crossProduct(v1.pos - v0.pos, v2.pos - v0.pos));
			}
		}

	public:
		std::vector<T> vertices;
		std::vector<unsigned int> indices;
	};
}
