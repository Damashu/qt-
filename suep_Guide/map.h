#ifndef MAP_H
#define MAP_H
#include <QString>
#include <QPoint>
#include <QWidget>
#include<QVector>

#include"adjmatrix.h"
#include <QMouseEvent>

namespace Ui {
class Map;
}

class Map : public QWidget
{
    Q_OBJECT

public:

    explicit Map(QWidget *parent = nullptr);
    ~Map();
    void mousePressEvent(QMouseEvent* event);

    void queryShortestPath(const QString& startName, const QString& endName);

private:
    Ui::Map *ui;

    adjMatrix adjMatrix;

    void paintEvent(QPaintEvent* );

    QPoint startPoint, endPoint;//选择两个目的地
    bool startSelected = false;//判断是否选择到了目的地
    QVector<int> getShortestPathIndices(const QString& startName, const QString& endName);

    void getAllPaths(int startIndex, int endIndex, QVector<int>& visited, QVector<int>& currentPath, QVector<QVector<int>>& allPaths);

    void queryAllPaths(const QString& startName, const QString& endName);

};




#endif // MAP_H
