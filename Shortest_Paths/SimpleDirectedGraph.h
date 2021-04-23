#pragma once
#include "list.h"
#include <string>
using namespace std;

namespace ShortestPaths {
	/**** An abstract class which use as a BASE class to different data structores of Simple directed graphs****/
	class SimpleDirectedGraph 
	{
	public:
		class Vertex
		{
		private:
			unsigned int m_Id;
			// Possible to add data field.
		public:
			static unsigned int s_currentId; // Will initilaized to 0 when building a new graph
			Vertex() { m_Id = s_currentId++; } 
			const unsigned int getId() { return m_Id; }
		    operator const unsigned int() const { return m_Id; }
			bool operator ==(Vertex& i_other) { return (m_Id == i_other.getId()); }
		};
		class Edge
		{
		private:
			const Vertex& m_From;
			const Vertex& m_To;
			float m_Weight;
		public:
			Edge(const Vertex& i_From,const Vertex& i_To,const float i_VertexWeight) : m_From(i_From), m_To(i_To) , m_Weight(i_VertexWeight) {}
			const Vertex& getFrom() { return m_From; }
			const Vertex& getTo() { return m_To; }
			const float getWeight() { return m_Weight; }
		};
	/*Fields*/
	protected:
		unsigned int m_NumOfVertex;
		Vertex* m_VertexArr;
		bool m_AllowNegativeWeight = false;
	/*Methods*/
	public:
		virtual void MakeEmptyGraph(const int i_n) = 0;
		virtual bool IsAdjacent(const int i_u, const int i_v) = 0;
		virtual list<const Vertex*>* GetAdjList(const int i_u) = 0;
		virtual void AddEdge(const int i_u,const int i_v,const float i_Weight) = 0;
		virtual void RemoveEdge(const int i_u, const int i_v) = 0;
		const int GetNumOfVertex() { return m_NumOfVertex; }
		const Vertex& GetVertex(const int i_u);
		virtual float getEdgeWeight(const unsigned int i_u, const unsigned int i_v) = 0;
	};
	
	class AdjacencyMatrix : public SimpleDirectedGraph
	{
	/*Fields*/
	private:
		Edge*** m_GraphMatrix;

	/*Methods*/
	public:
		AdjacencyMatrix() : m_GraphMatrix(nullptr) { m_VertexArr = nullptr; m_NumOfVertex = 0; }
		~AdjacencyMatrix();
		virtual void MakeEmptyGraph(const int i_n) override;
		virtual bool IsAdjacent(const int i_u, const int i_v) override;
		virtual list<const Vertex*>* GetAdjList(const int i_u) override;
		virtual void AddEdge(const int i_u, const int i_v, const float i_Weight) override;
		virtual void RemoveEdge(const int i_u, const int i_v) override;
	private:
		virtual float getEdgeWeight(const unsigned int i_u, const unsigned int i_v);
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

	public:
		AdjacencyList() : m_GraphListsArr(nullptr) { m_VertexArr = nullptr; m_NumOfVertex = 0; }
		~AdjacencyList();
		virtual void MakeEmptyGraph(const int i_n) override;
		virtual bool IsAdjacent(const int i_u, const int i_v) override;
		virtual list<const Vertex*>* GetAdjList(const int i_u) override;
		virtual void AddEdge(const int i_u, const int i_v, const float i_Weight) override;
		virtual void RemoveEdge(const int i_u, const int i_v) override;
	private:
		Pair* searchPairInListByVertex(const Vertex& i_FromVertex,const Vertex& i_ToVertex);
		virtual float getEdgeWeight(const unsigned int i_u, const unsigned int i_v);
	};

	class SimDirGraphExceptions : public exception {
	protected:
		string m_Msg;
	public:
		explicit SimDirGraphExceptions(const string i_Msg) : m_Msg(i_Msg) {}
		virtual const char* what() const { return m_Msg.c_str(); }
	};
}