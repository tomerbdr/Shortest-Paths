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

void printShortPath(ShortPath* i_Algorithem, unsigned int i_From, unsigned int i_To, const char* i_InitialMessage);
void MakeGraphsFromFile(int argc, char** argv, SimpleDirectedGraph* G1, SimpleDirectedGraph* G2, int& fromVertex, int& toVertex);
int StringToFloat(const char* i_String);
int StringToFloat(const char* i_String);

void main(int argc, char** argv)
{
	try
	{
		SimpleDirectedGraph* G_List = new AdjacencyList;
		SimpleDirectedGraph* G_Matrix = new AdjacencyMatrix;
		int fromVertex, toVertex;
		int* shortPathPtr;
		MakeGraphsFromFile(argc, argv, G_List, G_Matrix,fromVertex,toVertex);

		/*Adjacency List*/
		PriorityQueue<SimpleDirectedGraph::Vertex*>* Q_Array = new ArrayPriorityQueue<SimpleDirectedGraph::Vertex*>;
		PriorityQueue<SimpleDirectedGraph::Vertex*>* Q_Heap = new HeapPriorityQueue<SimpleDirectedGraph::Vertex*>;
		Dijkstra Dijkstra_Heap_Adj_List_ShortPath(G_List, Q_Heap);
		Dijkstra Dijkstra_Array_Adj_List_ShortPath(G_List, Q_Array);
		BelmanFord Belman_Adj_List_ShortPath(G_List);

		printShortPath(&Dijkstra_Heap_Adj_List_ShortPath, fromVertex, toVertex, "Adjacency Dijkstra heap ");
		printShortPath(&Dijkstra_Array_Adj_List_ShortPath, fromVertex, toVertex, "Adjacency Dijkstra array ");
		printShortPath(&Belman_Adj_List_ShortPath, fromVertex, toVertex, "Adjacency Bellman Ford ");
		delete Q_Array;
		delete Q_Heap;

		/*Adjacency Matrix*/
		Q_Array = new ArrayPriorityQueue<SimpleDirectedGraph::Vertex*>;
		Q_Heap = new HeapPriorityQueue<SimpleDirectedGraph::Vertex*>;
		Dijkstra Dijkstra_Heap_Adj_Matrix_ShortPath(G_Matrix, Q_Heap);
		Dijkstra Dijkstra_Array_Adj_Matrix_ShortPath(G_Matrix, Q_Array);
		BelmanFord Belman_Adj_Matrix_ShortPath(G_Matrix);

		printShortPath(&Dijkstra_Heap_Adj_Matrix_ShortPath, fromVertex, toVertex, "Matrix Dijkstra heap ");
		printShortPath(&Dijkstra_Array_Adj_Matrix_ShortPath, fromVertex, toVertex, "Matrix Dijkstra array ");
		printShortPath(&Belman_Adj_Matrix_ShortPath, fromVertex, toVertex, "Matrix Bellman Ford ");

		delete Q_Array;
		delete Q_Heap;
		delete G_List;
		delete G_Matrix;
	}
	catch (exception& i_Exc)
	{
		cout << "Invalid input" << endl << i_Exc.what() << endl;
		exit(1);
	}
}
// Parse string to int - exception will be throwen if the string is not exaclly a integer
int StringToInt(const char* i_String)
{
	if (i_String == nullptr)
		throw invalid_argument("Error - empty string.");

	size_t idx;
	int res = stoi(i_String, &idx);
	if (idx != strlen(i_String))
		throw invalid_argument("Error - String is not an integer.");
	return res;
}

// Parse string to float - exception will be throwen if the string is not exaclly a float
int StringToFloat(const char* i_String)
{
	if (i_String == nullptr)
		throw invalid_argument("Error - empty string.");
	size_t idx;
	float res = stof(i_String, &idx);
	if (idx != strlen(i_String))
		throw invalid_argument("Error - String is not an float.");
	return res;
}

// Get command line arguments (Conatin file name), two empty graphs, start and dest verteices. Build the graphs and set the verteices.
// In case of bad input invalid arguments exception will throwen.
void MakeGraphsFromFile(int argc, char** argv,SimpleDirectedGraph* G1,SimpleDirectedGraph* G2,int& fromVertex,int& toVertex)
{
		if (argc != NUM_OF_ARGUMENTS)
		{
			throw invalid_argument("Error - Invalid num of arguments.");
		}

		fstream inputFile(argv[1]);
		if (!inputFile)
			throw invalid_argument("Error - File doesnt exist");

		string lineReader;
		inputFile >> lineReader;
		int graphSize = StringToInt(lineReader.c_str()); // Num of vertex
		inputFile >> lineReader;
		fromVertex = StringToInt(lineReader.c_str()); // Source vertex
		inputFile >> lineReader;
		toVertex = StringToInt(lineReader.c_str()); // Destination vertex
		fromVertex--; // The user vertex id begin from 1 to n
		toVertex--;  //  The program decrease all ids by 1 - from 0 to n-1
		G1->MakeEmptyGraph(graphSize);
		G2->MakeEmptyGraph(graphSize);
		inputFile.get();

		while (!inputFile.eof()) // Reading edges input
		{
			int u, v;
			float weight;
			getline(inputFile,lineReader);
			char* split = strtok(const_cast<char*>(lineReader.c_str()), " ");
			u = StringToInt(split);
			split = strtok(NULL, " ");
			v = StringToInt(split);
			split = strtok(NULL, " ");
			weight = StringToFloat(split);
			split = strtok(NULL, " ");

			if (split != nullptr) // Remain input in the current line
				throw invalid_argument("Error - To many arguments in line.");

			G1->AddEdge(u - 1, v - 1, weight);  // The user vertex id begin from 1 to n
			G2->AddEdge(u - 1, v - 1, weight); //  The program decrease all ids by 1 - from 0 to n-1
		}
		inputFile.close();
}

/* Get ptr to initiliazed shortpath object (Bellman/Dijkstra) and print the shortest path between the input vertices */
void printShortPath(ShortPath* i_Algorithem, unsigned int i_From, unsigned int i_To, const char* i_InitialMessage)
{
	cout << i_InitialMessage;
	const float* shortPathPtr = i_Algorithem->ShortestPath(i_From, i_To);
	if (shortPathPtr != nullptr)
		cout << *shortPathPtr << endl;
	else
		cout << "No path exist." << endl;
	
}