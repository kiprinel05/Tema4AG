#include "graph.h"
#include <set>
#include <vector>
#include <algorithm>

bool Graph::addNode(QPoint pos) {
    const int minDistanceSquared = 400;

    for (const auto& node : m_nodes) {
        int dx = node.getPos().x() - pos.x();
        int dy = node.getPos().y() - pos.y();
        if (dx * dx + dy * dy < minDistanceSquared) {
            return false;
        }
    }

    Node newNode;
    newNode.setPos(pos);
    newNode.setValue(m_nodes.size() + 1);
    m_nodes.push_back(newNode);

    for (auto& row : m_costMatrix) {
        row.push_back(0);
    }
    m_costMatrix.push_back(std::vector<int>(m_nodes.size(), 0));

    return true;
}

bool Graph::addEdge(Node first, Node second, int cost) {
    for (const auto& edge : m_edges) {
        if ((edge.getFirst().getPos() == first.getPos() && edge.getSecond().getPos() == second.getPos()) ||
            (edge.getFirst().getPos() == second.getPos() && edge.getSecond().getPos() == first.getPos())) {
            return false;
        }
    }

    if (m_costMatrix.size() < m_nodes.size()) {
        m_costMatrix.resize(m_nodes.size(), std::vector<int>(m_nodes.size(), 0));
    }

    Edge newEdge(first, second);
    m_edges.push_back(newEdge);
    m_costMatrix[first.getValue() - 1][second.getValue() - 1] = cost;

    if (!m_isOriented) {
        m_costMatrix[second.getValue() - 1][first.getValue() - 1] = cost;
    }

    return true;
}

std::vector<Edge> Graph::prim() {
    const int INF = 1e9;
    int nodecost = m_nodes.size();
    if (nodecost == 0) return {};

    std::vector<int> min(nodecost, INF);
    std::vector<bool> isInMST(nodecost, false);
    std::vector<int> parent(nodecost, -1);

    min[0] = 0;

    for (int count = 0; count < nodecost - 1; ++count) {
        int minNode = -1;
        for (int i = 0; i < nodecost; ++i) {
            if (!isInMST[i] && (minNode == -1 || min[i] < min[minNode])) {
                minNode = i;
            }
        }

        isInMST[minNode] = true;

        for (int neighbor = 0; neighbor < nodecost; ++neighbor) {
            int edgeCost = m_costMatrix[minNode][neighbor];
            if (edgeCost && !isInMST[neighbor] && edgeCost < min[neighbor]) {
                min[neighbor] = edgeCost;
                parent[neighbor] = minNode;
            }
        }
    }

    std::vector<Edge> edges;
    for (int i = 1; i < nodecost; ++i) {
        if (parent[i] != -1) {
            Node first = m_nodes[parent[i]];
            Node second = m_nodes[i];
            edges.emplace_back(first, second);
        }
    }

    return edges;
}

int find(std::vector<std::pair<int, int>>& subsets, int i) {
    if (subsets[i].first != i) {
        subsets[i].first = find(subsets, subsets[i].first);
    }
    return subsets[i].first;
}

void unionSubs(std::vector<std::pair<int, int>>& subsets, int x, int y) {
    int rootX = find(subsets, x);
    int rootY = find(subsets, y);

    if (subsets[rootX].second < subsets[rootY].second) {
        subsets[rootX].first = rootY;
    }
    else if (subsets[rootX].second > subsets[rootY].second) {
        subsets[rootY].first = rootX;
    }
    else {
        subsets[rootY].first = rootX;
        subsets[rootX].second++;
    }
}

std::vector<Edge> Graph::kruskal() {
    std::vector<Edge> edges;
    std::vector<std::pair<int, Edge>> edgesWithC;

    for (const auto& edge : m_edges) {
        int cost = m_costMatrix[edge.getFirst().getValue() - 1][edge.getSecond().getValue() - 1];
        edgesWithC.emplace_back(cost, edge);
    }

    std::sort(edgesWithC.begin(), edgesWithC.end(),
        [](const std::pair<int, Edge>& a, const std::pair<int, Edge>& b) {
            return a.first < b.first;
        });

    int nodeCount = m_nodes.size();
    std::vector<std::pair<int, int>> subsets(nodeCount);

    for (int i = 0; i < nodeCount; ++i) {
        subsets[i] = { i, 0 };
    }

    for (const auto& [cost, edge] : edgesWithC) {
        int u = edge.getFirst().getValue() - 1;
        int v = edge.getSecond().getValue() - 1;

        int setU = find(subsets, u);
        int setV = find(subsets, v);

        if (setU != setV) {
            edges.push_back(edge);
            unionSubs(subsets, setU, setV);
        }
    }

    return edges;
}
