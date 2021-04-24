#include "ProgramOperator.h"
#include "SimpleDirectedGraph.h"
#include "PriorityQueue.h"
#include "ShortPath.h"
#include <iostream>
#include <stdlib.h>

using namespace std;
using namespace ShortestPaths;
/***********Program Instructions*************/
//
// ** The program should be execute from the command line with the following syntax:
//                    <exeName>.exe "<GraphFileName>.txt" 
//
// ** The Graph-File format:
//
// <Num of vertices>
// <Source vertex>
// <Dest vertex>
// <FromVertex> <ToVertex> <EdgeWeight>
// .
// .         
// .
// <FromVertex> <ToVertex> <EdgeWeight>
//
//
// ** Create instances of the desired graph and ShortPath algorithem in the main function
// ** Use the abstract class ProgramOperator's static methods to build graph from file and calculate the shortest path with the desired graph and algorithems
/**********************************************/

void main(int argc, char** argv)
{
	try
	{
		SimpleDirectedGraph* G_List = new AdjacencyList;
		SimpleDirectedGraph* G_Matrix = new AdjacencyMatrix;
		int fromVertex, toVertex;
		int* shortPathPtr;
		ProgramOperator::MakeGraphsFromFile(argc, argv, G_List, G_Matrix,fromVertex,toVertex);

		/*Adjacency List*/
		PriorityQueue<SimpleDirectedGraph::Vertex*>* Q_Array = new ArrayPriorityQueue<SimpleDirectedGraph::Vertex*>;
		PriorityQueue<SimpleDirectedGraph::Vertex*>* Q_Heap = new HeapPriorityQueue<SimpleDirectedGraph::Vertex*>;
		Dijkstra Dijkstra_Heap_Adj_List_ShortPath(G_List, Q_Heap);
		Dijkstra Dijkstra_Array_Adj_List_ShortPath(G_List, Q_Array);
		BelmanFord Belman_Adj_List_ShortPath(G_List);

		ProgramOperator::printShortPath(&Dijkstra_Heap_Adj_List_ShortPath, fromVertex, toVertex, "Adjacency Dijkstra heap ");
		ProgramOperator::printShortPath(&Dijkstra_Array_Adj_List_ShortPath, fromVertex, toVertex, "Adjacency Dijkstra array ");
		ProgramOperator::printShortPath(&Belman_Adj_List_ShortPath, fromVertex, toVertex, "Adjacency Bellman Ford ");
		delete Q_Array;
		delete Q_Heap;

		/*Adjacency Matrix*/
		Q_Array = new ArrayPriorityQueue<SimpleDirectedGraph::Vertex*>;
		Q_Heap = new HeapPriorityQueue<SimpleDirectedGraph::Vertex*>;
		Dijkstra Dijkstra_Heap_Adj_Matrix_ShortPath(G_Matrix, Q_Heap);
		Dijkstra Dijkstra_Array_Adj_Matrix_ShortPath(G_Matrix, Q_Array);
		BelmanFord Belman_Adj_Matrix_ShortPath(G_Matrix);

		ProgramOperator::printShortPath(&Dijkstra_Heap_Adj_Matrix_ShortPath, fromVertex, toVertex, "Matrix Dijkstra heap ");
		ProgramOperator::printShortPath(&Dijkstra_Array_Adj_Matrix_ShortPath, fromVertex, toVertex, "Matrix Dijkstra array ");
		ProgramOperator::printShortPath(&Belman_Adj_Matrix_ShortPath, fromVertex, toVertex, "Matrix Bellman Ford ");

		delete Q_Array;
		delete Q_Heap;
		delete G_List;
		delete G_Matrix;
	}
	catch (exception& i_Exc)
	{
		cout << "Invalid input." << endl;
		/* cout << i_Ecx.what() << endl */ // Use this line to see what is the exaclty exception
		exit(1);
	}
}
