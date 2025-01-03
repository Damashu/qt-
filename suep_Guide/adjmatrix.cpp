#include "adjmatrix.h"

adjMatrix::adjMatrix() {
    initializeGraph();
    printAdjacencyMatrix();
}

void adjMatrix::initializeGraph() {
    QString vertexFile = "vertices.txt";
    QString matrixFile = "adjacency_matrix.txt";

    vertices = readVertices(vertexFile);
    adjacencyMatrix = readAdjacencyMatrix(matrixFile);

    // 检查数据正确性
    if (vertices.isEmpty() || adjacencyMatrix.isEmpty()) {
        qWarning("初始化失败，请检查文件格式");
    } else {
         calculateShortestPaths();  // 计算最短路径
        qDebug("图初始化成功");
    }
}


QVector<Vertex>  adjMatrix::readVertices(const QString& filePath) {
    QVector<Vertex> vertices;
    QFile file(filePath);

    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qWarning("无法打开顶点文件");
        return vertices;
    }

    QTextStream in(&file);
    while (!in.atEnd()) {
        QString line = in.readLine();
        QStringList parts = line.split(',');
        if (parts.size() != 7) continue;

        int id = parts[0].toInt();
        QString name = parts[1];
        QString code = parts[2];
        QString description = parts[3];
        int x = parts[4].toInt();
        int y = parts[5].toInt();
        QString imagePath = parts[6];

       vertices.append(Vertex(name, code, description, QPoint(x, y), imagePath));
    }

    file.close();
    return vertices;
}

QVector<QVector<double>> adjMatrix::readAdjacencyMatrix(const QString& filePath) {
    QVector<QVector<double>> matrix;
    QFile file(filePath);

    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qWarning("无法打开邻接矩阵文件");
        return matrix;
    }

    QTextStream in(&file);
    while (!in.atEnd()) {
        QString line = in.readLine();
        QStringList parts = line.split(' ');  // 假设矩阵的权重用空格分隔
        QVector<double> row;
        for (const QString& part : parts) {
            row.append(part.toDouble());
        }
        matrix.append(row);
    }

    file.close();
    return matrix;
}

void adjMatrix::printAdjacencyMatrix() const {
    for (const QVector<double>& row : adjacencyMatrix) {
        QString rowOutput;  // 创建一个 QString 用于存储一行的输出
        for (double value : row) {
            // 输出矩阵元素，未连接的用"-1"替换
            QString outputValue = (value == -1) ? "-1" : QString::number(value);
            rowOutput.append(outputValue + " "); // 拼接每个元素
        }
        qDebug() << rowOutput; // 输出整行
    }
}
void adjMatrix::calculateShortestPaths()
{
    int n = vertices.size();
    dist = adjacencyMatrix;  // 初始化距离矩阵
    pre.resize(n, QVector<int>(n, -1));//调用 resize(n)，表示调整 pre 的大小为 n，即分配 n 行。每行被初始化为一个 QVector<int>(n)，即列大小也为 n。
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            if (adjacencyMatrix[i][j] == -1) {
                pre[i][j] = -1;  // 无前驱
            } else {
                pre[i][j] = i;  // 自己是前驱
            }
        }
    }

    // 初始化 pre 数组
    for (int i = 0; i < n; ++i)
    {
        for (int j = 0; j < n; ++j)
        {
            if (dist[i][j] != -1 && i != j)
            {
                pre[i][j] = i;
            }
        }
    }

    // Floyd-Warshall 算法
    //不断地添加k点，添加一个更改矩阵为当前最短路径，知道所有的点全部遍历完
    for (int k = 0; k < n; ++k)
    {
        for (int i = 0; i < n; ++i)
        {
            for (int j = 0; j < n; ++j)
            {
                if (dist[i][k] != -1 && dist[k][j] != -1)
                {
                    double newDist = dist[i][k] + dist[k][j];
                    if (dist[i][j] == -1 || newDist < dist[i][j])
                    {
                        dist[i][j] = newDist;
                        pre[i][j] = pre[k][j];
                    }
                }
            }
        }
    }
}
