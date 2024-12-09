#ifndef EDGE_H
#define EDGE_H

#include "node.h"

class Edge
{
public:
    Edge();
    Edge(Node f, Node s) : m_first(f), m_second(s) {}

    Node getFirst() const { return m_first; }
    Node getSecond() const { return m_second; }

private:
    Node m_first, m_second;
};

#endif // EDGE_H
