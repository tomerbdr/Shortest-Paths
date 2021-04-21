#pragma once
#include "SimpleDirectedGraph.h"
#include "PriorityQueue.h"
#include "list.h"

namespace ShortestPaths {

	class ShortPath
	{
	protected:
		int** m_DistanceArr;
		SimpleDirectedGraph::Vertex** m_ParentsArr;
		SimpleDirectedGraph* m_Graph;

		bool relax(const int i_u, const int i_v);
		void init(const int i_u);
	public:
		~ShortPath();
		virtual const int ShortestPath(const int i_u, const int i_v) = 0;
	};

	class BelmanFord : public ShortPath
	{
	public:
		BelmanFord(SimpleDirectedGraph* i_Graph) { m_DistanceArr = nullptr; m_ParentsArr = nullptr; m_Graph = i_Graph; }
		virtual const int ShortestPath(const int i_u, const int i_v) override;
	};
	class Dijkstra : public ShortPath
	{
	private:
		PriorityQueue<SimpleDirectedGraph::Vertex*>* m_Q;
	public:
		Dijkstra(SimpleDirectedGraph* i_Graph,PriorityQueue<SimpleDirectedGraph::Vertex*>* i_Q) : m_Q(i_Q) { m_DistanceArr = nullptr; m_ParentsArr = nullptr; m_Graph = i_Graph; }
		virtual const int ShortestPath(const int i_u, const int i_v) override;
	};
}