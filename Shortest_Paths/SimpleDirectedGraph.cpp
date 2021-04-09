#include "SimpleDirectedGraph.h"
#define PUBLIC
#define PRIVATE
#define VIRTUAL
#define PROTECTED

namespace ShortestPaths {
	/*Abstract Base Class Implementention */
	/*Vertex*/
	//SimpleDirectedGraph::Vertex::Vertex(const string i_IdStr)
	//{
	//	size_t pos;
	//	try
	//	{ // Converting string to int. In a case of invalid argumant exception will be throwen
	//		m_Id = stoi(i_IdStr,&pos);
	//		if (pos != i_IdStr.length()) // Unconverted non-numeric charecters remain
	//			throw invalid_argument("Invalid argument - Input contain non-numeric char.");
	//	}
	//	catch (invalid_argument)
	//	{
	//		throw;
	//	}
	//	if (m_Id < 0)
	//		throw SimDirGraphExceptions("Invalid argument - negative vertex ID");
	//}
	PUBLIC SimpleDirectedGraph::Vertex::Vertex()
	{
		static int currentId = 0;
		m_Id = currentId;
		currentId++;
	}
	PUBLIC void SimpleDirectedGraph::AddEdge(string i_uStr, string i_vStr, string i_WeightStr) 
	{
		int weight;
		try 
		{ // Converting string to int. In a case of invalid argumant exception will be throwen
			size_t pos;
			weight = stoi(i_WeightStr,&pos);
			if (pos != i_WeightStr.length())
				throw invalid_argument("Invalid argument - Input contain non-numeric charecters.");
		}
		catch (invalid_argument)
		{
			throw SimDirGraphExceptions("Invalid argument - Input contain non-numeric charecters.");
		}
		Vertex u = i_uStr; // implicit convert
		Vertex v = i_vStr; // implicit convert

		if (u >= m_NumOfVertex || v >= m_NumOfVertex)
			throw SimDirGraphExceptions("Vertex doesn't exist");
		if (u == v)
			throw SimDirGraphExceptions("Self-loop");
		if (weight < 0)
			throw SimDirGraphExceptions("Negative weight");

		AddEdge(u, v, weight); //Calling virtual function AddEdge of relevant data structure graph
	}


	/*Adjacency Matrix Graph Implementation*/
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
	PUBLIC VIRTUAL bool AdjacencyMatrix::IsAdjacent(const Vertex& i_u, const Vertex& i_v)
	{
		return (m_GraphMatrix[i_u][i_v] == nullptr ? false : true);
	}
	PUBLIC VIRTUAL list<const SimpleDirectedGraph::Vertex*>* AdjacencyMatrix::GetAdjList(const Vertex& i_u)
	{
		list<const Vertex*>* adjList = new list<const Vertex*>;
		for (int j = 0; j < m_NumOfVertex; j++) {
			Edge* currentEdge = m_GraphMatrix[i_u][j];
			if (currentEdge != nullptr) // If the edge exist
			{
				adjList->push_back(new Vertex(currentEdge->getTo())); // Adding vertex to list
			}
		}

		return adjList;
	}
	PRIVATE VIRTUAL void AdjacencyMatrix::AddEdge(const Vertex& i_u, const Vertex& i_v, const int i_Weight)
	{
		if (m_GraphMatrix[i_u][i_v] != nullptr)
			throw SimDirGraphExceptions("Parallel edge");

		m_GraphMatrix[i_u][i_v] = new Edge(i_u, i_v, i_Weight);
	}
	PUBLIC VIRTUAL void AdjacencyMatrix::RemoveEdge(const Vertex& i_u, const Vertex& i_v)
	{
		if (i_u >= m_NumOfVertex || i_v >= m_NumOfVertex)
			throw SimDirGraphExceptions("Vertex doesn't exist");
		if (m_GraphMatrix[i_u][i_v] == nullptr)
			throw SimDirGraphExceptions("Edge doesn't exist");

		delete m_GraphMatrix[i_u][i_v];
		m_GraphMatrix[i_u][i_v] = nullptr;
	}

}