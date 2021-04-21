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
	PROTECTED bool ShortPath::relax(const int i_u, const int i_v)
	{
		bool isRelaxed = false;
		float edgeWeight = m_Graph->getEdgeWeight(i_u, i_v);

		if (m_DistanceArr[i_u] == nullptr) // m_DistaceArr[i_u] = infinity - means that	path to i_u from starting vertex didnt revealed
			isRelaxed = false;
		else if ((m_DistanceArr[i_v] == nullptr) || (*m_DistanceArr[i_v] > *m_DistanceArr[i_u] + edgeWeight))
		{
			if (m_DistanceArr[i_v] == nullptr)
				m_DistanceArr[i_v] = new float;

			*m_DistanceArr[i_v] = *m_DistanceArr[i_u] + edgeWeight;
			m_ParentsArr[i_v] = const_cast<SimpleDirectedGraph::Vertex*>(&(m_Graph->GetVertex(i_u)));
			isRelaxed = true;
		}

		return isRelaxed;
	}
	PROTECTED void ShortPath::init(const int i_u)
	{
		m_DistanceArr = new float* [m_Graph->GetNumOfVertex()];
		m_ParentsArr = new SimpleDirectedGraph::Vertex * [m_Graph->GetNumOfVertex()];

		for (int i = 0; i < m_Graph->GetNumOfVertex(); i++) // INIT
		{
			m_DistanceArr[i] = nullptr;
			m_ParentsArr[i] = nullptr;
		}

		m_DistanceArr[i_u] = new float(0);
	}

/**** BelmanFord ****/
	VIRTUAL PUBLIC const float BelmanFord::ShortestPath(const int i_u, const int i_v)
	{
		if (i_u < 0 || i_u >= m_Graph->GetNumOfVertex() || i_v < 0 || i_v >= m_Graph->GetNumOfVertex())
		{
			throw SimDirGraphExceptions("Invalid argument - Vertex ID doesn't exist.");
		}
		
		this->init(i_u); //INIT

		for (int i = 0; i < m_Graph->GetNumOfVertex(); i++)
		{
			for (int j = 0; j < m_Graph->GetNumOfVertex(); j++)
			{
				const SimpleDirectedGraph::Vertex& currentFromVertex = m_Graph->GetVertex(j);
				list<const SimpleDirectedGraph::Vertex*>* adjVertexList = m_Graph->GetAdjList(currentFromVertex);

				for (auto itr = adjVertexList->begin(); itr != adjVertexList->end(); itr++)
				{
					SimpleDirectedGraph::Vertex* currentToVertex = const_cast<SimpleDirectedGraph::Vertex*>(*itr);
					relax(currentFromVertex, *currentToVertex);
				}

				delete adjVertexList;
			}
		}

		return *m_DistanceArr[i_v];
	}

/**** Dijkstra ****/

	VIRTUAL PUBLIC const float Dijkstra::ShortestPath(const int i_u, const int i_v)
	{
		if (i_u < 0 || i_u >= m_Graph->GetNumOfVertex() || i_v < 0 || i_v >= m_Graph->GetNumOfVertex())
		{
			throw SimDirGraphExceptions("Invalid argument - Vertex ID doesn't exist.");
		}

		this->init(i_u); //INIT
		Pair<SimpleDirectedGraph::Vertex*>* pairArr = new Pair<SimpleDirectedGraph::Vertex*>[m_Graph->GetNumOfVertex()];
		for (int i = 0; i < m_Graph->GetNumOfVertex(); i++)
		{
			pairArr[i].m_Data = const_cast<SimpleDirectedGraph::Vertex*>(&(m_Graph->GetVertex(i)));
			if (m_DistanceArr[i] != nullptr) // All the keys are initialized to Infinity
				pairArr[i].m_Key = *m_DistanceArr[i]; 
		}

		m_Q->Build(pairArr, m_Graph->GetNumOfVertex()); // Build the priority queue

		while (!m_Q->IsEmpty())
		{
			SimpleDirectedGraph::Vertex& u = *(m_Q->DeleteMin().m_Data);
			list<const SimpleDirectedGraph::Vertex*>* adjVertexList = m_Graph->GetAdjList(u);

			for (auto itr = adjVertexList->begin(); itr != adjVertexList->end(); itr++)
			{
				SimpleDirectedGraph::Vertex* currentToVertex = const_cast<SimpleDirectedGraph::Vertex*>(*itr);
				bool isRelaxed = relax(u, *currentToVertex); // relax will return TRUE if relaxed
				if (isRelaxed)
					m_Q->DecreaseKey(*currentToVertex, *m_DistanceArr[*currentToVertex]);
			}

			delete adjVertexList;	
		}

		delete[] pairArr;
		return *m_DistanceArr[i_v];
	}
}

