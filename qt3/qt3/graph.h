#ifndef GRAPH_H
#define GRAPH_H

#include "edge.h"
#include <vector>
#include <queue>
#include <algorithm>

class Graph
{
public:
    Graph() : m_isOriented(false) {}

    bool addNode(QPoint pos);
    bool addEdge(Node f, Node s, int cost);
    std::vector<Edge> prim();
    std::vector<Edge> kruskal();

    std::vector<Node>& getNodes() { return m_nodes; }
    std::vector<Edge>& getEdges() { return m_edges; }

private:
    std::vector<Node> m_nodes;
    std::vector<Edge> m_edges;
    std::vector<std::vector<int>> m_costMatrix;
    bool m_isOriented;
};

#endif // GRAPH_H
