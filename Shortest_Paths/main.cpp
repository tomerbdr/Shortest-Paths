#include "SimpleDirectedGraph.h"
#include "list.h"
#include "PriorityQueue.h"
#include <iostream>
#include <string>

using namespace std;

using namespace ShortestPaths;

void main() {
	Pair<char> arr[5] = { {2,'a'},{3,'b'},{5,'c'},{1,'d'},{4,'e'} };

	HeapPriorityQueue<char> A;
	A.Build(arr, 5);
	
	A.DecreaseKey(4, 0);
	for (int i = 5; i > 0; i--)
		cout << A.DeleteMin().m_Key << endl;

}
