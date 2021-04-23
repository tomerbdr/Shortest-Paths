#define _CRT_SECURE_NO_WARNINGS
#include "SimpleDirectedGraph.h"
#include "list.h"
#include "PriorityQueue.h"
#include "ShortPath.h"
#include <iostream>
#include <string>
#include <fstream>
#include <stdlib.h>
#define NUM_OF_ARGUMENTS 2

using namespace std;

using namespace ShortestPaths;

void MakeGraphsFromFile(int argc, char** argv, SimpleDirectedGraph* G1, SimpleDirectedGraph* G2, int& fromVertex, int& toVertex);

//void main() {
//	try
//	{
//	fstream graphFile("Graph.txt");
//	if (!graphFile)
//		throw "File doesnt exist";
//
//	SimpleDirectedGraph* G = new AdjacencyList;
//	int size;
//	graphFile >> size;
//	G->MakeEmptyGraph(size);
//	int from, to;
//	graphFile >> from >> to;
//	from--;
//	to--;
//	while (!graphFile.eof())
//	{
//		int u, v;
//		float weight;
//		graphFile >> u >> v >> weight;
//		G->AddEdge(u - 1, v - 1, weight);
//	}
//	graphFile.close();
//	
//
//	PriorityQueue<SimpleDirectedGraph::Vertex*>* Q1 = new ArrayPriorityQueue<SimpleDirectedGraph::Vertex*>;
//	PriorityQueue<SimpleDirectedGraph::Vertex*>* Q2 = new HeapPriorityQueue<SimpleDirectedGraph::Vertex*>;
//
//	ShortPath* S = new BelmanFord(G);
//	float shortpath = S->ShortestPath(from, to);
//	cout << "Short Path Ford: " << shortpath << endl;
//	delete S;
//
//	S = new Dijkstra(G, Q1);
//	shortpath = S->ShortestPath(from, to);
//	cout << "Short Path Dijkstra array: " << shortpath << endl;
//	delete S;
//
//	S = new Dijkstra(G, Q2);
//	shortpath = S->ShortestPath(from, to);
//	cout << "Short Path Dijkstra heap: " << shortpath << endl;
//
//	delete S;
//	delete Q1;
//	delete Q2;
//	delete G;
//	}
//	catch (exception& exp)
//	{
//		cout << "Exception: " << exp.what() << endl;
//	}
//}
void main(int argc, char** argv)
{
	try
	{
		SimpleDirectedGraph* G_List = new AdjacencyList;
		SimpleDirectedGraph* G_Matrix = new AdjacencyMatrix;
		int fromVertex, toVertex;
		MakeGraphsFromFile(argc, argv, G_List, G_Matrix,fromVertex,toVertex);

		PriorityQueue<SimpleDirectedGraph::Vertex*>* Q_Array = new ArrayPriorityQueue<SimpleDirectedGraph::Vertex*>;
		PriorityQueue<SimpleDirectedGraph::Vertex*>* Q_Heap = new HeapPriorityQueue<SimpleDirectedGraph::Vertex*>;
		Dijkstra Dijkstra_Heap_Adj_List_ShortPath(G_List, Q_Heap);
		Dijkstra Dijkstra_Array_Adj_List_ShortPath(G_List, Q_Array);
		BelmanFord Belman_Adj_List_ShortPath(G_List);

		cout << "Adjacency Dijkstra heap " << Dijkstra_Heap_Adj_List_ShortPath.ShortestPath(fromVertex, toVertex) << endl;
		cout << "Adjacency Dijkstra array " << Dijkstra_Array_Adj_List_ShortPath.ShortestPath(fromVertex, toVertex) << endl;
		cout << "Adjacency Bellman Ford " << Belman_Adj_List_ShortPath.ShortestPath(fromVertex, toVertex) << endl;
		delete Q_Array;
		delete Q_Heap;

		Q_Array = new ArrayPriorityQueue<SimpleDirectedGraph::Vertex*>;
		Q_Heap = new HeapPriorityQueue<SimpleDirectedGraph::Vertex*>;
		Dijkstra Dijkstra_Heap_Adj_Matrix_ShortPath(G_Matrix, Q_Heap);
		Dijkstra Dijkstra_Array_Adj_Matrix_ShortPath(G_Matrix, Q_Array);
		BelmanFord Belman_Adj_Matrix_ShortPath(G_Matrix);
		cout << "Matrix Dijkstra heap " << Dijkstra_Heap_Adj_Matrix_ShortPath.ShortestPath(fromVertex, toVertex) << endl;
		cout << "Matrix Dijkstra array " << Dijkstra_Array_Adj_Matrix_ShortPath.ShortestPath(fromVertex, toVertex) << endl;
		cout << "Matrix Bellman Ford " << Belman_Adj_Matrix_ShortPath.ShortestPath(fromVertex, toVertex) << endl;

		delete Q_Array;
		delete Q_Heap;
		delete G_List;
		delete G_Matrix;
	}
	catch (exception& i_Exc)
	{
		cout << "Invalid input" << endl;
		exit(1);
	}
}

void MakeGraphsFromFile(int argc, char** argv,SimpleDirectedGraph* G1,SimpleDirectedGraph* G2,int& fromVertex,int& toVertex)
{
		if (argc != NUM_OF_ARGUMENTS)
		{
			throw invalid_argument("Error - Invalid num of arguments.");
		}

		fstream inputFile(argv[1]);
		if (!inputFile)
			throw invalid_argument("Error - File doesnt exist");

		size_t idx;
		string lineReader;
		inputFile >> lineReader;
		int graphSize = stoi(lineReader,&idx);
		if (idx != lineReader.length())
			throw invalid_argument("Error - Invalid input");
		inputFile >> lineReader;
		fromVertex = stoi(lineReader,&idx);
		if (idx != lineReader.length())
			throw invalid_argument("Error - Invalid input");
		inputFile >> lineReader;
		toVertex = stoi(lineReader,&idx);
		if (idx != lineReader.length())
			throw invalid_argument("Error - Invalid input");
		fromVertex--;
		toVertex--;
		G1->MakeEmptyGraph(graphSize);
		G2->MakeEmptyGraph(graphSize);
		inputFile.get();

		while (!inputFile.eof())
		{
			int u, v;
			float weight;
			getline(inputFile,lineReader);
			char* split = strtok(const_cast<char*>(lineReader.c_str()), " ");
			u = stoi(split,&idx);
			if (idx != strlen(split))
				throw invalid_argument("Error - Invalid input");
			split = strtok(NULL, " ");
			v = stoi(split,&idx);
			if (idx != strlen(split))
				throw invalid_argument("Error - Invalid input");
			split = strtok(NULL, " ");
			weight = stof(split,&idx);
			if (idx != strlen(split))
				throw invalid_argument("Error - Invalid input");
			split = strtok(NULL, " ");

			if (split != nullptr)
				throw invalid_argument("Invalid input");

			G1->AddEdge(u - 1, v - 1, weight);
			G2->AddEdge(u - 1, v - 1, weight);
		}
		inputFile.close();
}
