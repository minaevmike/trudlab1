#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QPaintEvent>
#include <QPainter>
#include <QTransform>
#include <QPen>
#include <QVector>
#include <algorithm>
#include <cmath>
#include <iostream>
#include <QPushButton>
#include <QThread>
#include <QDebug>
#include <iomanip>
#include <QLabel>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <thread>
#include <QTimer>
#include <mutex>
#include <vector>
#include <QPalette>

enum pointCondition{
    INSIDE,
    OUTSIDE,
    LEFTBORDER,
    RIGHTBORDER,
    TOPBORDER,
    BOTBORDER,
    OBLIQUEBORDER
};
class Widget : public QWidget
{
    Q_OBJECT

public:

    Widget(QWidget *parent = 0);
    ~Widget();
    void paintEvent(QPaintEvent *);
    void mousePressEvent(QMouseEvent *event);

    void drawFigure(QPainter *p);
    void drawPoints(QPainter *p);
    void drawTempMap(QPainter *p);
    void drawBorder(QPainter *p);
    QColor tempToColor(double temp, double min, double max);
    pointCondition condition(double x, double y);
    void printTMatrix();
    QThread calcThread;
    std::mutex tempMutex;
private:
    std::vector<std::vector<double> > T;
    //QVector<QVector<double> > T;
    int scale;
    int botTemp, leftTemp;
    int leftSide, botSide, rightSide, topSide;
    double dx, dy, dt, a1, a2;
    int n;
    int yOffset;
    bool module;
    QLabel *tempLabel;
    QPushButton *calcButton;
    double modulateTime;
private slots:
    void calcBackgound();
    void calc();
};

#endif // WIDGET_H
