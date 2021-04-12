#include "SimpleDirectedGraph.h"
#include "list.h"
#include "PriorityQueue.h"
#include "ShortPath.h"
#include <iostream>
#include <string>
#include <fstream>

using namespace std;

using namespace ShortestPaths;

void main() {
	try
	{
	fstream graphFile("Graph.txt");
	if (!graphFile)
		throw "File doesnt exist";

	SimpleDirectedGraph* G = new AdjacencyMatrix;
	int size;
	graphFile >> size;
	G->MakeEmptyGraph(size);
	int from, to;
	graphFile >> from >> to;
	from--;
	to--;
	while (!graphFile.eof())
	{
		int u, v, weight;
		graphFile >> u >> v >> weight;
		G->AddEdge(u - 1, v - 1, weight);
	}
	graphFile.close();
	
	/*
	G->MakeEmptyGraph(6);
	G->AddEdge(0, 1, 16);
	G->AddEdge(0, 2, 13);
	G->AddEdge(1, 2, 10);
	G->AddEdge(2, 1, 4);
	G->AddEdge(1, 3, 12);
	G->AddEdge(3, 2, 9);
	G->AddEdge(2, 4, 14);
	G->AddEdge(4, 3, 7);
	G->AddEdge(4, 5, 4);
	G->AddEdge(3, 5, 20);
	*/

	ShortPath* S = new BelmanFord(G);
	int shortpath = S->ShortestPath(from, to);
	cout << "Short Path: " << shortpath << endl;

	delete S;
	delete G;
	}
	catch (exception& exp)
	{
		cout << "Exception: " << exp.what() << endl;
	}
}
