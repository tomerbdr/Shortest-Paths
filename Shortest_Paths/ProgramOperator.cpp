#define _CRT_SECURE_NO_WARNINGS
#include "ProgramOperator.h"
#include <chrono>
#include <iomanip>
#define PUBLIC
#define PRIVATE
#define STATIC
#define NUM_OF_ARGUMENTS 3

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

		ofstream timeFile(argv[2], ofstream::trunc); // Trunc time file for mesuare current function run time
		timeFile.close();

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
	PUBLIC STATIC void ProgramOperator::printShortPath(ShortPath* i_Algorithem, unsigned int i_From, unsigned int i_To, const char* i_InitialMessage, const char* i_TimeFile)
	{
		cout << i_InitialMessage;
		const float* shortPathPtr = runTimeMesuare(i_InitialMessage, i_Algorithem, i_From, i_To, i_TimeFile); // calculate shortest path and write the run time into file.
		if (shortPathPtr != nullptr)
			cout << *shortPathPtr << endl;
		else
			cout << "No path exist." << endl;

	}

	PRIVATE STATIC const float* ProgramOperator::runTimeMesuare(const char* i_String, ShortPath* i_Algorithem, unsigned int i_From, unsigned int i_To, const char* i_TimeFile)
	{
		const float* res;

		auto start = chrono::high_resolution_clock::now();

		// unsync the I/O of C and C++. 
		ios_base::sync_with_stdio(false);

		res = i_Algorithem->ShortestPath(i_From,i_To); // Here you put the name of the function you wish to measure

		auto end = chrono::high_resolution_clock::now();

		// Calculating total time taken by the program. 
		double time_taken =
			chrono::duration_cast<chrono::nanoseconds>(end - start).count();

		time_taken *= 1e-9;

		ofstream myfile(i_TimeFile,ios_base::app); // The name of the file which is in argv[2]

		myfile << i_String << fixed
			<< time_taken << setprecision(9);
		myfile << " sec" << endl;
		myfile.close();

		return res;
	}
}