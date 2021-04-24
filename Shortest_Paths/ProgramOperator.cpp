#define _CRT_SECURE_NO_WARNINGS
#include "ProgramOperator.h"
#define PUBLIC
#define PRIVATE
#define STATIC
#define NUM_OF_ARGUMENTS 2

namespace ShortestPaths
{
	// Parse string to int - exception will be throwen if the string is not exaclly a integer
	PRIVATE STATIC int ProgramOperator::StringToInt(const char* i_String)
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
	PRIVATE STATIC float ProgramOperator::StringToFloat(const char* i_String)
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
	PUBLIC STATIC void ProgramOperator::MakeGraphsFromFile(int argc, char** argv, SimpleDirectedGraph* G1, SimpleDirectedGraph* G2, int& fromVertex, int& toVertex)
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
			getline(inputFile, lineReader);
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
	PUBLIC STATIC void ProgramOperator::printShortPath(ShortPath* i_Algorithem, unsigned int i_From, unsigned int i_To, const char* i_InitialMessage)
	{
		cout << i_InitialMessage;
		const float* shortPathPtr = i_Algorithem->ShortestPath(i_From, i_To);
		if (shortPathPtr != nullptr)
			cout << *shortPathPtr << endl;
		else
			cout << "No path exist." << endl;

	}
}