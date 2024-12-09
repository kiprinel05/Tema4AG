#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QPainter>
#include <QInputDialog>
#include <QFont>
#include <cmath>

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent), ui(new Ui::MainWindow)
{
    m_isFirstNodeSelected = false;
    ui->setupUi(this);

    connect(ui->buttonRunPrim, &QPushButton::clicked, this, &MainWindow::onRunPrimClicked);
    connect(ui->buttonRunKruskal, &QPushButton::clicked, this, &MainWindow::onRunKruskalClicked);

    m_mstColor = Qt::blue;
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::mouseReleaseEvent(QMouseEvent* m)
{
    const int clickRadius = 400;

    if (m->button() == Qt::RightButton)
    {
        if (m_graph.addNode(m->pos()))
        {
            m_mstEdges.clear();
            update();
        }
    }
    else if (m->button() == Qt::LeftButton)
    {
        std::vector<Node>& nodes = m_graph.getNodes();
        for (auto& n : nodes)
        {
            int dx = n.getPos().x() - m->pos().x();
            int dy = n.getPos().y() - m->pos().y();

            if (dx * dx + dy * dy < clickRadius)
            {
                if (m_isFirstNodeSelected)
                {
                    if (n != m_selectedNode)
                    {
                        int cost = QInputDialog::getInt(this, "Cost", "costul arcului:");
                        if (m_graph.addEdge(m_selectedNode, n, cost))
                        {
                            m_mstEdges.clear();
                            update();
                        }
                    }

                    m_isFirstNodeSelected = false;
                }
                else
                {
                    m_isFirstNodeSelected = true;
                    m_selectedNode = n;
                }
            }
        }
    }
}


void MainWindow::paintEvent(QPaintEvent* ev)
{
    QPainter p(this);

    std::vector<Node>& nodes = m_graph.getNodes();
    for (auto& n : nodes)
    {
        QRect r(n.getPos().x() - 20, n.getPos().y() - 20, 40, 40);

        QPen pen(Qt::white);
        p.setPen(pen);
        p.setBrush(Qt::white);
        p.drawEllipse(r);

        QFont font("Arial", 12, QFont::Bold);
        p.setFont(font);
        p.setPen(Qt::red);
        p.drawText(r, Qt::AlignCenter, QString::number(n.getValue()));
    }

    if (!m_mstEdges.empty())
    {
        QPen mstPen(m_mstColor);
        mstPen.setWidth(3);
        p.setPen(mstPen);

        for (const auto& e : m_mstEdges)
        {
            QPoint p1 = e.getFirst().getPos();
            QPoint p2 = e.getSecond().getPos();
            p.drawLine(p1, p2);
        }
    }
    else
    {
        std::vector<Edge> edges = m_graph.getEdges();
        QPen edgePen(Qt::white);
        edgePen.setWidth(3);
        p.setPen(edgePen);

        for (auto& e : edges)
        {
            QPoint p1 = e.getFirst().getPos();
            QPoint p2 = e.getSecond().getPos();

            double angle = std::atan2(p2.y() - p1.y(), p2.x() - p1.x());
            QPoint offset(int(20 * std::cos(angle)), int(20 * std::sin(angle)));

            p.drawLine(p1 + offset, p2 - offset);
        }
    }
}


void MainWindow::onRunPrimClicked()
{
    m_mstEdges = m_graph.prim();
    m_mstColor = Qt::blue;
    update();
}

void MainWindow::onRunKruskalClicked()
{
    m_mstEdges = m_graph.kruskal();
    m_mstColor = Qt::green;
    update();
}
