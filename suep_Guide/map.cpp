#include "map.h"
#include "ui_map.h"
#include <QPainter>
#include <QPixmap>
#include<QMessageBox>
Map::Map(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Map)
{
    ui->setupUi(this);
    setFixedSize(width(), height());
    ui->textEdit->setReadOnly(true);
    ui->photoshow->setText("点击对应位置即可展示图片与介绍");

}

Map::~Map()
{
    delete ui;
}


void Map::mousePressEvent(QMouseEvent* event)
{
    int currentTabIndex=ui->tabWidget->currentIndex();
    qDebug()<<"现在的tab标签是"<<currentTabIndex;
    qDebug() << "Clicked at:" << event->pos();
    QPoint clickPos = event->pos();

    if(currentTabIndex==0)//防止在执行一个tab的内容时候，另一个tab也执行，减少不必要消耗
    {

        // 遍历顶点，判断是否点击到某个顶点
        for (const auto& vertex : adjMatrix.vertices)
        {
            if (QLineF(clickPos, vertex.position).length() <= 10)
            { // 判断点击范围
                // 更新右侧 TabWidget 的 Label
                QPixmap image(vertex.imagePath);
                if (!image.isNull())
                {
                    ui->photoshow->setPixmap(image.scaled(ui->photoshow->size(), Qt::KeepAspectRatio));
                    ui->textEdit->setText(vertex.description);
                } else
                {
                    ui->photoshow->setText("图片未找到");

                }
                return;
            }
        }
        // 若未点击到任何顶点，清空 Label
        ui->photoshow->setText("点击对应位置即可展示图片与介绍");

        ui->textEdit->clear();
    }
    else if(currentTabIndex==1)
    {

        for (const auto& vertex : adjMatrix.vertices)
        {
            if (QLineF(clickPos, vertex.position).length() <= 10)
            {
                if (!startSelected)
                {
                    // 选择起点
                    startPoint = vertex.position;
                    startSelected = true;
                    ui->label_3->setText("起点已选择: " + vertex.name);
                    ui->textEdit_2->setText("起点已选择");
                     ui->label_4->setText("终点选择: " );
                }
                else
                {
                    // 选择终点并查询路径
                    endPoint = vertex.position;

                    ui->label_4->setText("终点已选择: " + vertex.name);
                     ui->textEdit_2->setText("终点已选择");

                    QString startName, endName;

                    for (const auto& v : adjMatrix.vertices)
                    {
                        if (v.position == startPoint) startName = v.name;
                        if (v.position == endPoint) endName = v.name;
                    }


                    queryShortestPath(startName, endName);
                    startSelected = false;  // 重置起点选择
                    queryAllPaths(startName, endName);
                    update();
                }
                return;
            }

        }
        ui->textEdit_2->setText("未选择任何景点");


    }

}



void Map::paintEvent(QPaintEvent* ) {


    // 创建 QPainter 对象
    QPainter painter(this);

    // 加载地图图片
    QPixmap map(":/new/prefix1/suep_photo/map.jpg"); // 替换为你的图片路径

    // 绘制图片
    painter.drawPixmap(0, 0, 1180, 820, map);


    painter.setBrush(QColor(255, 0, 0)); // 设置填充颜色为明亮的红色
    painter.setPen(Qt::NoPen);           // 去除边框
    for (const auto& vertex : adjMatrix.vertices)
    {
        painter.drawEllipse(vertex.position, 15, 15);  // 绘制红色圆点
         painter.setPen(Qt::black);
        QFont font = painter.font();
        font.setBold(true);                 // 加粗字体
        painter.setFont(font);

        // 绘制景点名称
        painter.drawText(vertex.position + QPoint(-10, 5), vertex.name);

        // 恢复无边框设置
        painter.setPen(Qt::NoPen);


    }


    // 绘制连接线
    painter.setPen(QPen(Qt::blue, 2)); // 使用蓝色笔绘制连线，线宽为2
    for (int i = 0; i < adjMatrix.adjacencyMatrix.size(); ++i)
    {
        for (int j = i + 1; j < adjMatrix.adjacencyMatrix.size(); ++j) { // j 从 i + 1 开始，避免重复绘制
            if (adjMatrix.adjacencyMatrix[i][j] > 0) { // 存在连接
                // 获取顶点 i 和 j 的位置
                QPoint start = adjMatrix.vertices[i].position;
                QPoint end = adjMatrix.vertices[j].position;

                // 绘制连接线
                painter.drawLine(start, end);

                // 计算线条的中点位置用于显示权值
                QPoint midPoint((start.x() + end.x()) / 2, (start.y() + end.y()) / 2);

                // 获取该边的权值并绘制
                QString weight = QString::number(adjMatrix.adjacencyMatrix[i][j]);

                // 设置权值文本的颜色为紫色
                painter.setPen(QColor(Qt::black)); // 紫色

                // 设置字体为加粗且增大
                QFont weightFont = painter.font();
                weightFont.setPointSize(12); // 设置字体大小为12（您可以根据需要调整）
                weightFont.setBold(true);      // 加粗字体
                painter.setFont(weightFont);

                // 绘制权值文本
                painter.drawText(midPoint, weight);

                // 恢复绘制连接线的颜色和字体
                painter.setPen(QPen(Qt::blue, 2));

            }
        }
    }

    QString startName, endName;
    for (const auto& vertex : adjMatrix.vertices) {
        if (vertex.position == startPoint) startName = vertex.name;
        if (vertex.position == endPoint) endName = vertex.name;
    }

    // 绘制最短路径
    QVector<int> pathIndices = getShortestPathIndices(startName, endName);

    if (!pathIndices.isEmpty()) {
        painter.setPen(QPen(Qt::green, 3)); // 绿色线条
        for (int i = 0; i < pathIndices.size() - 1; ++i) {
            QPoint start = adjMatrix.vertices[pathIndices[i]].position;
            QPoint end = adjMatrix.vertices[pathIndices[i + 1]].position;
            painter.drawLine(start, end);
        }
    }




}

void Map::queryShortestPath(const QString& startName, const QString& endName) {
    QVector<int> pathIndices = getShortestPathIndices(startName, endName);

    if (pathIndices.isEmpty()) {
        ui->textEdit_2->setText("无效的起点或终点，或无路径可达");
        return;
    }

    // 构造路径名称
    QStringList pathNames;
    for (int index : pathIndices) {
        pathNames.append(adjMatrix.vertices[index].name);
    }
    QString pathString = pathNames.join(" -> ");

    // 获取路径长度
    int startIndex = pathIndices.first();
    int endIndex = pathIndices.last();
    double pathLength = adjMatrix.dist[startIndex][endIndex];

    // 显示路径信息
    ui->textEdit_2->setText(QString("路径: %1\n路径长度: %2").arg(pathString).arg(pathLength));
}


QVector<int> Map::getShortestPathIndices(const QString& startName, const QString& endName) {
    QVector<int> pathIndices;

    // 找到起点和终点的索引
    int startIndex = -1, endIndex = -1;
    for (int i = 0; i < adjMatrix.vertices.size(); ++i) {
        if (adjMatrix.vertices[i].name == startName) startIndex = i;
        if (adjMatrix.vertices[i].name == endName) endIndex = i;
    }

    if (startIndex == -1 || endIndex == -1 || adjMatrix.dist[startIndex][endIndex] == -1) {
        return pathIndices; // 无效的起点、终点或无路径可达
    }

    // 通过 pre 数组还原路径索引
    int current = endIndex;
    while (current != -1) {
        pathIndices.prepend(current);
        if (current == startIndex) break;
        current = adjMatrix.pre[startIndex][current];
    }

    return pathIndices;
}


void Map::getAllPaths(int startIndex, int endIndex, QVector<int>& visited, QVector<int>& currentPath, QVector<QVector<int>>& allPaths) {
    visited[startIndex] = 1; // 标记当前节点已访问
    currentPath.append(startIndex);

    // 如果到达终点，保存路径
    if (startIndex == endIndex) {
        allPaths.append(currentPath);
    } else {
        // 递归访问相邻未访问的节点
        for (int i = 0; i < adjMatrix.adjacencyMatrix.size(); ++i) {
            if (adjMatrix.adjacencyMatrix[startIndex][i] > 0 && !visited[i]) {
                getAllPaths(i, endIndex, visited, currentPath, allPaths);
            }
        }
    }

    // 回溯
    visited[startIndex] = 0;
    currentPath.pop_back();
}


void Map::queryAllPaths(const QString& startName, const QString& endName) {
    int startIndex = -1, endIndex = -1;

    // 找到起点和终点在图中的索引
    for (int i = 0; i < adjMatrix.vertices.size(); ++i) {
        if (adjMatrix.vertices[i].name == startName) startIndex = i;
        if (adjMatrix.vertices[i].name == endName) endIndex = i;
    }

    if (startIndex == -1 || endIndex == -1) {
        ui->textEdit_3->setText("无效的起点或终点");
        return;
    }

    QVector<int> visited(adjMatrix.vertices.size(), 0);
    QVector<int> currentPath;
    QVector<QVector<int>> allPaths;

    // 计算所有路径
    getAllPaths(startIndex, endIndex, visited, currentPath, allPaths);

    // 显示路径统计信息和详细路径到 textEdit_3
    QStringList pathStrings;
    pathStrings.append(QString("起点: %1, 终点: %2").arg(startName).arg(endName));
    pathStrings.append(QString("总路径数: %1").arg(allPaths.size()));

    for (const auto& path : allPaths) {
        QStringList pathNames;
        double pathLength = 0.0;

        // 构造路径名称和长度
        for (int i = 0; i < path.size(); ++i) {
            pathNames.append(adjMatrix.vertices[path[i]].name);
            if (i < path.size() - 1) {
                pathLength += adjMatrix.adjacencyMatrix[path[i]][path[i + 1]];
            }
        }

        pathStrings.append(QString("路径: %1\n路径长度: %2").arg(pathNames.join(" -> ")).arg(pathLength));
    }

    if (!pathStrings.isEmpty()) {
        ui->textEdit_3->setText(pathStrings.join("\n\n"));
    } else {
        ui->textEdit_3->setText("无路径可达");
    }
}

