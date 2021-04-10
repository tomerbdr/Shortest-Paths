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
			// Possible to add data field.
		public:
			Vertex() { static int currentId = 0; m_Id = currentId++; } 
			const unsigned int getId() { return m_Id; }
		    operator const unsigned int() const { return m_Id; }
			bool operator ==(Vertex& i_other) { return (m_Id == i_other.getId()); }
		};
		class Edge
		{
		private:
			const Vertex& m_From;
			const Vertex& m_To;
			int m_Weight;
		public:
			Edge(const Vertex& i_From,const Vertex& i_To,const int i_VertexWeight) : m_From(i_From), m_To(i_To) , m_Weight(i_VertexWeight) {}
			const Vertex& getFrom() { return m_From; }
			const Vertex& getTo() { return m_To; }
			const int getWeight() { return m_Weight; }
		};


	/*Fields*/
	protected:
		unsigned int m_NumOfVertex;
		bool m_AllowNegativeWeight = false;
		// TODO: Add Vertex Array to here.
	/*Methods*/
	public:
		virtual void MakeEmptyGraph(const int i_n) = 0;
		virtual bool IsAdjacent(const int i_u, const int i_v) = 0;
		virtual list<const Vertex*>* GetAdjList(const int i_u) = 0;
		virtual void AddEdge(const int i_u,const int i_v,const int i_Weight) = 0;
		virtual void RemoveEdge(const int i_u, const int i_v) = 0;
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
		virtual bool IsAdjacent(const int i_u, const int i_v) override;
		virtual list<const Vertex*>* GetAdjList(const int i_u) override;
		virtual void AddEdge(const int i_u, const int i_v, const int i_Weight) override;		
		virtual void RemoveEdge(const int i_u, const int i_v) override;
	};

	class AdjacencyList : public SimpleDirectedGraph
	{
	private:
		struct Pair {
			Vertex& m_v;
			Edge& m_Edge;
			Pair(Vertex& i_v, Edge& m_Edge) : m_v(i_v), m_Edge(m_Edge) {}
			~Pair() { delete &m_Edge; }
		};
		list<Pair*>* m_GraphListsArr;
		Vertex* m_VertexArr;

	public:
		AdjacencyList() : m_GraphListsArr(nullptr), m_VertexArr(nullptr) {}
		~AdjacencyList();
		virtual void MakeEmptyGraph(const int i_n) override;
		virtual bool IsAdjacent(const int i_u, const int i_v) override;
		virtual list<const Vertex*>* GetAdjList(const int i_u) override;
		virtual void AddEdge(const int i_u, const int i_v, const int i_Weight) override;
		virtual void RemoveEdge(const int i_u, const int i_v) override;
	private:
		Pair* searchPairInListByVertex(const Vertex& i_FromVertex,const Vertex& i_ToVertex);
	};

	class SimDirGraphExceptions : public exception {
	protected:
		string m_Msg;
	public:
		explicit SimDirGraphExceptions(const string i_Msg) : m_Msg(i_Msg) {}
		virtual const char* what() const { return m_Msg.c_str(); }
	};
}