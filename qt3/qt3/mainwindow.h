#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMouseEvent>
#include <QPainter>
#include "graph.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

private:
    Ui::MainWindow* ui;
    Graph m_graph;
    Node m_selectedNode;
    bool m_isFirstNodeSelected;
    bool m_isOriented;
    QColor m_mstColor;
    std::vector<Edge> m_mstEdges;

public:
    MainWindow(QWidget* parent = nullptr);
    ~MainWindow();

    void mouseReleaseEvent(QMouseEvent* m) override;
    void paintEvent(QPaintEvent* ev) override;
    void drawArrow(QPoint start, QPoint end, QPainter& p);

public slots:
    void onRunPrimClicked();
    void onRunKruskalClicked();
};

#endif // MAINWINDOW_H
