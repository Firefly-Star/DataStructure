#pragma once

#include <iostream>
#include <vector>

enum class StoreMethod
{
	AdjacencyMatrix,
	AdjacencyList,
	OrthogonalList,
	AdjacencyMultipleList,
};

template<typename EWeightType>
struct Edge
{
	using size_type = size_t;
	size_type i;
	size_type j;
	EWeightType weight = 1;
};

template<typename VInfoType>
using vertex_set = std::vector<VInfoType>;

template<typename EWeighgType>
using edge_set = std::vector<Edge<EWeighgType>>;

template<typename VInfoType = int, typename EWeightType = int, StoreMethod method = StoreMethod::AdjacencyMatrix>
class Graph
{
public:
	Graph()
	{
		static_assert(false, "Unkown store method");
	}
};

template<typename VInfoType, typename EWeightType>
class Graph<VInfoType, EWeightType, StoreMethod::AdjacencyMatrix>
{
public:
	struct Matrix
	{
		Matrix(size_t n, EWeightType initval)
			:n(n), data(n* n, initval)
		{
		}
		EWeightType& operator()(size_t i, size_t j)
		{
			if (i < n && j < n)
			{
				return data[i * n + j];
			}
			throw "index out of range";
		}
		EWeightType const& operator()(size_t i, size_t j) const
		{
			if (i < n && j < n)
			{
				return data[i * n + j];
			}
			throw "index out of range";
		}
		friend std::ostream& operator<<(std::ostream& os, Matrix const& mat)
		{
			for (int i = 0; i < mat.n; ++i)
			{
				for (int j = 0; j < mat.n; ++j)
				{
					if (mat(i, j) == std::numeric_limits<EWeightType>::max())
					{
						os << "inf\t";
					}
					else
					{
						os << mat(i, j) << "\t";
					}
				}
				os << "\n";
			}
			return os;
		}
		std::vector<EWeightType> data;
		size_t n;
	};
public:
	Graph(vertex_set<VInfoType> const& vertices, edge_set<EWeightType> const& edges, bool directed = false)
		: vertices(vertices), adjacencyMatrix(vertices.size(), directed? std::numeric_limits<EWeightType>::max() : 0)
	{
		for (auto edge : edges)
		{
			adjacencyMatrix(edge.i, edge.j)= edge.weight;
			if (!directed)
			{
				adjacencyMatrix(edge.j, edge.i) = edge.weight;
			}
		}
	}
	Graph Kruskal() const
	{

	}
	Matrix& data() { return adjacencyMatrix; }
private:
	vertex_set<VInfoType> vertices;
	Matrix adjacencyMatrix;
};

template<typename VInfoType, typename EWeightType>
class Graph<VInfoType, EWeightType, StoreMethod::AdjacencyList>
{
public:
	Graph(vertex_set<VInfoType> const& vertices, edge_set<EWeightType> const& edges, bool directed = false)
	{

	}
private:
};

using namespace std;
bool checkPossibility(vector<int>& nums) {
	int counter = 0;
	for (int i = 0; i < nums.size() - 1; ++i)
	{
		if (nums[i] > nums[i + 1])
		{
			if ((i > 0 && i + 2 < nums.size() && nums[i + 1] < nums[i - 1] && nums[i + 2] < nums[i]) || counter >= 1)
			{
				return false;
			}
			else
			{
				++counter;
			}
		}
	}
	return true;
}

