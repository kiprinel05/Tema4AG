#ifndef NODE_H
#define NODE_H

#include <QPoint>

class Node
{
public:
    Node() : m_value(0) {}
    Node(int val, QPoint pos) : m_value(val), m_pos(pos) {}

    void setValue(int val) { m_value = val; }
    void setPos(QPoint p) { m_pos = p; }
    int getValue() const { return m_value; }
    QPoint getPos() const { return m_pos; }
    bool operator!=(const Node& other) const {
        return this->getValue() != other.getValue(); 
    }

private:
    QPoint m_pos;
    int m_value;
};

#endif // NODE_H
