#pragma once
#include "list.h"
#include <string>
using namespace std;

namespace ShortestPaths {
	class SimpleDirectedGraph 
	{
	public:
		class Vertex
		{
		private:
			unsigned int m_Id;
		public:
			Vertex();
			Vertex(const string i_IdStr);
			Vertex(const Vertex& i_Other) : m_Id(i_Other.m_Id) {} // Copy Ctor
			Vertex(const unsigned int i_Id) : m_Id(i_Id) {}
			const unsigned int getId() { return m_Id; }
		    operator const unsigned int() const { return m_Id; }
			const Vertex& operator =(const Vertex& i_Other) { return i_Other; }
		};
		class Edge
		{
		private:
			Vertex m_From;
			Vertex m_To;
			int m_Weight;
		public:
			Edge(const Vertex& i_From,const Vertex& i_To,const int i_VertexWeight) : m_From(i_From), m_To(i_To) , m_Weight(i_VertexWeight) {}
			const Vertex& getFrom() { return m_From; }
			const Vertex& getTo() { return m_To; }
			const int getWeight() { return m_Weight; }
		};
		// TODO: Add a bool (negativeWeightsAllowed)

	/*Fields*/
	protected:
		unsigned int m_NumOfVertex;
	/*Methods*/
	public:
		virtual void MakeEmptyGraph(const int i_n) = 0;
		virtual bool IsAdjacent(const Vertex& i_u, const Vertex& i_v) = 0;
		virtual list<const Vertex*>* GetAdjList(const Vertex& i_u) = 0;
		void AddEdge(string i_u, string i_v, string i_Weight);
		virtual void RemoveEdge(const Vertex& i_u, const Vertex& i_v) = 0;
	protected:
		virtual void AddEdge(const Vertex& i_u,const Vertex& i_v,const int i_Weight) = 0;
	};
	
	class AdjacencyMatrix : public SimpleDirectedGraph
	{
	/*Fields*/
	private:
		Edge*** m_GraphMatrix;
		Vertex* m_VertexArr;

	/*Methods*/
	public:
		AdjacencyMatrix() : m_GraphMatrix(nullptr), m_VertexArr(nullptr) { m_NumOfVertex = 0; }
		~AdjacencyMatrix();
		virtual void MakeEmptyGraph(const int i_n) override;
		virtual bool IsAdjacent(const Vertex& i_u, const Vertex& i_v) override;
		virtual list<const Vertex*>* GetAdjList(const Vertex& i_u) override;
		virtual void RemoveEdge(const Vertex& i_u, const Vertex& i_v) override;
	private:
		virtual void AddEdge(const Vertex& i_u, const Vertex& i_v, const int i_Weight) override;
		
	};

	class AdjacencyList : public SimpleDirectedGraph
	{
	public:
		struct Pair {
			Vertex& m_v;
			Edge& m_Edge;
			Pair(Vertex& i_v, Edge& m_Edge) : m_v(i_v), m_Edge(m_Edge) {}
		};
	private:
		list<Pair>* m_GraphList;
	};

	class SimDirGraphExceptions : public exception {
	protected:
		string m_Msg;
	public:
		explicit SimDirGraphExceptions(const string i_Msg) : m_Msg(i_Msg) {}
		virtual const char* what() const { return m_Msg.c_str(); }
	};
}