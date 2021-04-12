#pragma once
#include "ShortPath.h"
#define PUBLIC
#define PRIVATE
#define VIRTUAL
#define PROTECTED

namespace ShortestPaths
{
/**** ShortPath - BASE class *****/
	PUBLIC ShortPath::~ShortPath()
	{
		if (m_ParentsArr != nullptr)
			delete[] m_ParentsArr;

		if (m_DistanceArr != nullptr)
		{
			for (int i = 0; i < m_Graph->GetNumOfVertex(); i++)
			{
				if (m_DistanceArr[i] != nullptr)
					delete m_DistanceArr[i];
			}
			
			delete[] m_DistanceArr;
		}
	}

	PUBLIC void ShortPath::relax(const int i_u, const int i_v)
	{
		int edgeWeight = m_Graph->getEdgeWeight(i_u, i_v);

		if (m_DistanceArr[i_u] == nullptr) // m_DistaceArr[i_u] = infinity - means that	path to i_u from starting vertex didnt revealed
			return;

		else if ((m_DistanceArr[i_v] == nullptr) || (*m_DistanceArr[i_v] > *m_DistanceArr[i_u] + edgeWeight))
		{
			if (m_DistanceArr[i_v] == nullptr)
				m_DistanceArr[i_v] = new int;

			*m_DistanceArr[i_v] = *m_DistanceArr[i_u] + edgeWeight;
			m_ParentsArr[i_v] = const_cast<SimpleDirectedGraph::Vertex*>(&(m_Graph->GetVertex(i_u)));
		}
	}
/**** BelmanFord ****/
	VIRTUAL PUBLIC const int BelmanFord::ShortestPath(const int i_u, const int i_v)
	{
		if (i_u < 0 || i_u >= m_Graph->GetNumOfVertex() || i_v < 0 || i_v >= m_Graph->GetNumOfVertex())
		{
			throw SimDirGraphExceptions("Invalid argument - Vertex ID doesn't exist.");
		}

		m_DistanceArr = new int* [m_Graph->GetNumOfVertex()];
		m_ParentsArr = new SimpleDirectedGraph::Vertex*[m_Graph->GetNumOfVertex()];

		for (int i = 0; i < m_Graph->GetNumOfVertex(); i++) // INIT
		{
			m_DistanceArr[i] = nullptr;
			m_ParentsArr[i] = nullptr;
		}

		m_DistanceArr[i_u] = new int(0);

		for (int i = 0; i < m_Graph->GetNumOfVertex(); i++)
		{
			for (int j = 0; j < m_Graph->GetNumOfVertex(); j++)
			{
				const SimpleDirectedGraph::Vertex& currentFromVertex = m_Graph->GetVertex(j);
				list<const SimpleDirectedGraph::Vertex*>* vertexList = m_Graph->GetAdjList(currentFromVertex);

				for (auto itr = vertexList->begin(); itr != vertexList->end(); itr++)
				{
					SimpleDirectedGraph::Vertex* currentToVertex = const_cast<SimpleDirectedGraph::Vertex*>(*itr);
					relax(currentFromVertex, *currentToVertex);
				}

				delete vertexList;
			}
		}

		return *m_DistanceArr[i_v];
	}
}

