#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QPushButton>  // QPushButton 类
#include <QPixmap>      // QPixmap 类，用于加载图片
#include <QRegion>      // QRegion 类，用于定义遮罩区域
#include<QPainter>

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QPushButton>
#include <QPainter>
#include <QPolygon>
#include<QDir>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setFixedSize(width(), height());
    qDebug() << "当前工作目录:" << QDir::currentPath();
}

MainWindow::~MainWindow()
{


    delete ui;
}


void MainWindow::on_pushButton_clicked()
{
    map = new Map;
    map->show();
    this->close();
}

