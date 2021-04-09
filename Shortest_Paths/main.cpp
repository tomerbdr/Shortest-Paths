#include "SimpleDirectedGraph.h"
#include "list.h"
#include <iostream>
#include <string>

using namespace std;

using namespace ShortestPaths;

void main() {
	SimpleDirectedGraph* G = new AdjacencyMatrix;
	G->MakeEmptyGraph(3);
	string u = "0";
	string v = "1";
	string w = "2";
	G->AddEdge(u, v, "20");
	G->AddEdge(v, u, "10");
	G->AddEdge(w, v, "30");
	list<const SimpleDirectedGraph::Vertex*>* AdjLst = G->GetAdjList(u);
	const SimpleDirectedGraph::Vertex* temp = AdjLst->front(); /* Undirect Data FIXXX*/
	delete AdjLst;
	cout << (G->IsAdjacent(u, v) ? "yes" : "no") << endl;
	cout << (G->IsAdjacent(v, u) ? "yes" : "no") << endl;
	cout << (G->IsAdjacent(w, v) ? "yes" : "no") << endl;
	cout << (G->IsAdjacent(u, u) ? "yes" : "no") << endl;
	cout << (G->IsAdjacent(v, w) ? "yes" : "no") << endl;
	cout << (G->IsAdjacent(u, w) ? "yes" : "no") << endl;
	G->RemoveEdge(u, v);
	cout << (G->IsAdjacent(u, v) ? "yes" : "no") << endl;






		
}
