#include "SimpleDirectedGraph.h"
#define PUBLIC
#define PRIVATE
#define VIRTUAL
#define PROTECTED

namespace ShortestPaths {
		
	/**** Adjacency Matrix Graph Implementation ****/
	PUBLIC AdjacencyMatrix::~AdjacencyMatrix()
	{
		if (m_VertexArr != nullptr)
			delete[] m_VertexArr;
		if (m_GraphMatrix == nullptr)
			return; // Graph doesn't exist
		else
		{
			for (int i = 0; i < m_NumOfVertex; i++)
			{
				for (int j = 0; j < m_NumOfVertex; j++)
				{
					delete m_GraphMatrix[i][j]; // Delete edge
				}

				delete[] m_GraphMatrix[i];
			}

			delete[] m_GraphMatrix;
		}
	}
	PUBLIC VIRTUAL void AdjacencyMatrix::MakeEmptyGraph(const int i_n)
	{
		if (i_n <= 0)
		{
			throw SimDirGraphExceptions("Invalid number of Vertex");
		}

		m_NumOfVertex = i_n;
		m_GraphMatrix = new Edge**[m_NumOfVertex];
		for (int i = 0; i < m_NumOfVertex; i++)
		{
			m_GraphMatrix[i] = new Edge*[m_NumOfVertex];
			for (int j = 0; j < m_NumOfVertex; j++)
			{
				m_GraphMatrix[i][j] = nullptr; // Initilaize all edges to nullptr 
			}
		}

		m_VertexArr = new Vertex[m_NumOfVertex]; // Creating array of vertex - each vertex will get ID according to position in the array.
	}
	PUBLIC VIRTUAL bool AdjacencyMatrix::IsAdjacent(const int i_u, const int i_v)
	{
		if (i_u < 0 || i_u >= m_NumOfVertex || i_v < 0 || i_v >= m_NumOfVertex)
		{
			throw SimDirGraphExceptions("Invalid argument - Vertex ID doesn't exist.");
		}

		return (m_GraphMatrix[i_u][i_v] == nullptr ? false : true);
	}
	PUBLIC VIRTUAL list<const SimpleDirectedGraph::Vertex*>* AdjacencyMatrix::GetAdjList(const int i_u)
	{
		if (i_u < 0 || i_u >= m_NumOfVertex)
		{
			throw SimDirGraphExceptions("Invalid argument - Vertex ID doesn't exist.");
		}

		list<const Vertex*>* adjList = new list<const Vertex*>;
		for (int j = 0; j < m_NumOfVertex; j++) {
			Edge* currentEdge = m_GraphMatrix[i_u][j];
			if (currentEdge != nullptr) // If the edge exist
			{
				adjList->push_back(&(currentEdge->getTo())); // Adding vertex to list
			}
		}

		return adjList;
	}
	PUBLIC VIRTUAL void AdjacencyMatrix::AddEdge(const int i_u, const int i_v, const int i_Weight)
	{
		if (i_u < 0 || i_u >= m_NumOfVertex || i_v < 0 || i_v >= m_NumOfVertex)
			throw SimDirGraphExceptions("Invalid argument - Vertex ID doesn't exist.");

		if (i_u == i_v)
			throw SimDirGraphExceptions("Self loop are not allowed.");

		if (m_GraphMatrix[i_u][i_v] != nullptr)
			throw SimDirGraphExceptions("Parallel edge doesn't allowed");

		if (!m_AllowNegativeWeight && i_Weight < 0)
			throw SimDirGraphExceptions("Negative weight doesn't allowed");

		m_GraphMatrix[i_u][i_v] = new Edge(m_VertexArr[i_u], m_VertexArr[i_v], i_Weight);
	}
	PUBLIC VIRTUAL void AdjacencyMatrix::RemoveEdge(const int i_u, const int i_v)
	{
		if (i_u < 0 || i_u >= m_NumOfVertex || i_v < 0 || i_v >= m_NumOfVertex)
			throw SimDirGraphExceptions("Invalid argument - Vertex ID doesn't exist.");
		if (m_GraphMatrix[i_u][i_v] == nullptr)
			throw SimDirGraphExceptions("Edge doesn't exist");

		delete m_GraphMatrix[i_u][i_v];
		m_GraphMatrix[i_u][i_v] = nullptr;
	}
	PRIVATE VIRTUAL int AdjacencyMatrix::getEdgeWeight(const unsigned int i_u, const unsigned int i_v)
	{
		if (i_u < 0 || i_u >= m_NumOfVertex || i_v < 0 || i_v >= m_NumOfVertex)
			throw SimDirGraphExceptions("Invalid argument - Vertex ID doesn't exist.");
		if (m_GraphMatrix[i_u][i_v] == nullptr)
			throw SimDirGraphExceptions("Edge doesn't exist");

		return m_GraphMatrix[i_u][i_v]->getWeight();
	}
	/**** Adjacency List Graph Implementation ****/

	PUBLIC AdjacencyList::~AdjacencyList()
	{
		if (m_VertexArr != nullptr)
			delete[] m_VertexArr;
		if (m_GraphListsArr == nullptr)
			return; // Graph doesn't exist
		else
		{
			for (int i = 0; i < m_NumOfVertex; i++)
			{
				for (auto j = m_GraphListsArr[i].begin(); j != m_GraphListsArr[i].end(); j++)
				{
					delete* j; // Dealloc all pairs in the list
				}

				m_GraphListsArr[i].clear(); // Dealloc all list nodes
			}

			delete[] m_GraphListsArr;
		}
	}
	PUBLIC VIRTUAL void AdjacencyList::MakeEmptyGraph(const int i_n)
	{
		if (i_n <= 0)
		{
			throw SimDirGraphExceptions("Invalid number of Vertex");
		}

		m_NumOfVertex = i_n;
		m_GraphListsArr = new list<Pair*>[m_NumOfVertex];
		m_VertexArr = new Vertex[m_NumOfVertex]; // Creating array of vertex - each vertex will get ID according to position in the array.
	}
	PUBLIC VIRTUAL bool AdjacencyList::IsAdjacent(const int i_u, const int i_v)
	{
		if (i_u < 0 || i_u >= m_NumOfVertex || i_v < 0 || i_v >= m_NumOfVertex)
		{
			throw SimDirGraphExceptions("Invalid argument - Vertex ID doesn't exist.");
		}

		bool isAdjacent = false;
		Pair* desiredEdge = searchPairInListByVertex(m_VertexArr[i_u], m_VertexArr[i_v]);

		if (desiredEdge != nullptr)
			isAdjacent = true;

		return isAdjacent;
	}
	PUBLIC VIRTUAL list<const SimpleDirectedGraph::Vertex*>* AdjacencyList::GetAdjList(const int i_u)
	{
		if (i_u < 0 || i_u >= m_NumOfVertex)
		{
			throw SimDirGraphExceptions("Invalid argument - Vertex ID doesn't exist.");
		}

		list<const Vertex*>* adjList = new list<const Vertex*>;

		for (auto i = m_GraphListsArr[i_u].begin(); i != m_GraphListsArr[i_u].end(); i++)
		{
			Pair* currentPairToCompare = dynamic_cast<Pair*>(*i);
			adjList->push_back(&(currentPairToCompare->m_v)); // Adding vertex to list
		}

		return adjList;
	}
	PUBLIC VIRTUAL void AdjacencyList::AddEdge(const int i_u, const int i_v, const int i_Weight)
	{
		if (i_u < 0 || i_u >= m_NumOfVertex || i_v < 0 || i_v >= m_NumOfVertex)
			throw SimDirGraphExceptions("Invalid argument - Vertex ID doesn't exist.");

		if (i_u == i_v)
			throw SimDirGraphExceptions("Self loop are not allowed.");

		if (!m_AllowNegativeWeight && i_Weight < 0)
			throw SimDirGraphExceptions("Negative weight doesn't allowed");

		if (this->IsAdjacent(i_u,i_v) == true)
			throw SimDirGraphExceptions("Parallel edge doesn't allowed");

		Edge* newEdge = new Edge(m_VertexArr[i_u], m_VertexArr[i_v], i_Weight); // ~Pair() makes dealloc.
		Pair* pairToAdd = new Pair(m_VertexArr[i_v], *newEdge); // ~AdjacencyList() makes dealloc.
		m_GraphListsArr[i_u].push_back(pairToAdd);
	}
	PUBLIC VIRTUAL void AdjacencyList::RemoveEdge(const int i_u, const int i_v)
	{
		if (i_u < 0 || i_u >= m_NumOfVertex || i_v < 0 || i_v >= m_NumOfVertex)
			throw SimDirGraphExceptions("Invalid argument - Vertex ID doesn't exist.");
		
		for (auto i = m_GraphListsArr[i_u].begin(); i != m_GraphListsArr[i_u].end(); i++)
		{
			Pair* currentPairToCompare = dynamic_cast<Pair*>(*i);
			if (currentPairToCompare->m_v == m_VertexArr[i_v]) {
				m_GraphListsArr[i_u].erase(i); // Removing edge
				return;
			}
		}
		/*Passing loop means edge doesnt exist*/
		throw SimDirGraphExceptions("Edge doesn't exist");
	}
	PRIVATE AdjacencyList::Pair* AdjacencyList::searchPairInListByVertex(const Vertex& i_FromVertex,const Vertex& i_ToVertex)
	{
		for (auto i = m_GraphListsArr[i_FromVertex].begin(); i != m_GraphListsArr[i_FromVertex].end(); i++)
		{
			Pair* currentPairToCompare = dynamic_cast<Pair*>(*i);
			if (currentPairToCompare->m_v == m_VertexArr[i_ToVertex]) {
				return currentPairToCompare;
			}
		}

		return nullptr; // Pair didnt found
	}
	PRIVATE VIRTUAL int AdjacencyList::getEdgeWeight(const unsigned int i_u, const unsigned int i_v)
	{
		if (i_u < 0 || i_u >= m_NumOfVertex || i_v < 0 || i_v >= m_NumOfVertex)
			throw SimDirGraphExceptions("Invalid argument - Vertex ID doesn't exist.");

		Pair* desiredEdgePair = searchPairInListByVertex(m_VertexArr[i_u], m_VertexArr[i_v]);
		if (desiredEdgePair == nullptr)
			throw SimDirGraphExceptions("Edge doesn't exist");

		return desiredEdgePair->m_Edge.getWeight();
	}
	
}