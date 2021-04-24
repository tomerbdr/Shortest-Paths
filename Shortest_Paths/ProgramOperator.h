#pragma once
#include "SimpleDirectedGraph.h"
#include "list.h"
#include "PriorityQueue.h"
#include "ShortPath.h"
#include <iostream>
#include <string>
#include <fstream>
#include <stdlib.h>

namespace ShortestPaths 
{
	/*An abstract class with static methods to opearte the program*/
	class ProgramOperator
	{
	public:
		static void printShortPath(ShortPath* i_Algorithem, unsigned int i_From, unsigned int i_To, const char* i_InitialMessage);
		static void MakeGraphsFromFile(int argc, char** argv, SimpleDirectedGraph* G1, SimpleDirectedGraph* G2, int& fromVertex, int& toVertex);
	private:
		ProgramOperator(); // Abstarct class
		static int StringToInt(const char* i_String);
		static float StringToFloat(const char* i_String);
	};

}