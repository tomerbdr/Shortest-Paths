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

	SimpleDirectedGraph* G = new AdjacencyList;
	int size;
	graphFile >> size;
	G->MakeEmptyGraph(size);
	int from, to;
	graphFile >> from >> to;
	from--;
	to--;
	while (!graphFile.eof())
	{
		int u, v;
		float weight;
		graphFile >> u >> v >> weight;
		G->AddEdge(u - 1, v - 1, weight);
	}
	graphFile.close();
	

	PriorityQueue<SimpleDirectedGraph::Vertex*>* Q1 = new ArrayPriorityQueue<SimpleDirectedGraph::Vertex*>;
	PriorityQueue<SimpleDirectedGraph::Vertex*>* Q2 = new HeapPriorityQueue<SimpleDirectedGraph::Vertex*>;

	ShortPath* S = new BelmanFord(G);
	float shortpath = S->ShortestPath(from, to);
	cout << "Short Path Ford: " << shortpath << endl;
	delete S;

	S = new Dijkstra(G, Q1);
	shortpath = S->ShortestPath(from, to);
	cout << "Short Path Dijkstra array: " << shortpath << endl;
	delete S;

	S = new Dijkstra(G, Q2);
	shortpath = S->ShortestPath(from, to);
	cout << "Short Path Dijkstra heap: " << shortpath << endl;

	delete S;
	delete Q1;
	delete Q2;
	delete G;
	}
	catch (exception& exp)
	{
		cout << "Exception: " << exp.what() << endl;
	}
}
