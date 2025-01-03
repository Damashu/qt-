#ifndef ADJMATRIX_H
#define ADJMATRIX_H

#include <QString>
#include <QPoint>
#include <QWidget>
#include<QVector>

#include <QFile>
#include <QTextStream>
#include <QVector>

#include<QDebug>
class Vertex {
public:

    QString name;         // 景点名称
    QString code;         // 景点代号
    QString description;  // 景点简介
    QPoint position;      // 在地图上的坐标点
     QString imagePath;

    Vertex() {}
    Vertex(QString name, QString code, QString description, QPoint position,QString imagePath)
        : name(name), code(code), description(description), position(position), imagePath(imagePath) {}

};



class adjMatrix
{
public:
    adjMatrix();
      friend class Map;

    QVector<QVector<double>> dist;  // 最短路径距离矩阵
    QVector<QVector<int>> pre;      // 前驱矩阵，用于存储路径信息

private:
    QVector<Vertex> vertices;//顶点集
    QVector<QVector<double>> adjacencyMatrix;//邻接矩阵

    QVector<QVector<double>> readAdjacencyMatrix(const QString& filePath);//读邻接矩阵给矩阵容器赋值
     QVector<Vertex> readVertices(const QString& filePath) ;//读顶点集，给顶点集赋值
    void initializeGraph();

     void printAdjacencyMatrix() const;//用于测试是否读取成功

     void calculateShortestPaths();  // 计算最短路径

};

#endif // ADJMATRIX_H
