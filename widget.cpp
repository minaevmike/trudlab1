#include "widget.h"

Widget::Widget(QWidget *parent)
    : QWidget(parent)
{
    dx = dy = 0.25;
    dt = dx * dx * dx / 2;
    scale = 60;
    leftSide = 6;
    topSide = 5;
    rightSide = 3;
    botSide = 8;
    a1 = a2 = 1;
    botTemp = 100;
    leftTemp = 200;
    yOffset = 500;
    modulateTime = 15;
    module = false;
    tempLabel = new QLabel(this);
    QHBoxLayout *layout = new QHBoxLayout;
    //tempLabel->move(0,50);
    calcButton = new QPushButton("Modulate", this);
    connect(&calcThread, SIGNAL(started()), this, SLOT(calcBackgound()));
    connect(calcButton,SIGNAL(clicked()), this, SLOT(calc()));
    //calcButton->move((int)this->size().width()/ 2, calcButton->pos().y());
    layout->addWidget(calcButton);
    layout->addWidget(tempLabel);
    QVBoxLayout *l = new QVBoxLayout;
    l->addStretch(0);
    l->addLayout(layout);
    this->setLayout(l);
    n  = (std::max(std::max(leftSide, topSide), std::max(rightSide, botSide)) + 1) / dx + 1;
    T.resize(n);
    for(int i = 0; i < n; i++){
        T[i].resize(n);
    }
    for(int i = 0;i < n; ++i){
        T[0][i] = leftTemp;
        T[i][0] = botTemp;
    }
    /*QColor color;
    color.setHsvF(0.00000001, 1, 1);
    qDebug() << color.name();
    color.setHsvF(0.5, 1, 1);
    qDebug() << color.name();*/
    update();
}

void Widget::mousePressEvent(QMouseEvent *event){
    int size = T.size();
    double x = event->pos().x();
    double y = event->pos().y();
    y = yOffset - y;
    int i = 0, j = 0;
    x /= scale;
    y /= scale;
    //std::cout << x << " " << y << std::endl;
    if( condition(x, y) != OUTSIDE){
        for(int m = 0; m < size; ++m){
            if( m * dx < x && (m+1) * dx >= x){
                i = m;
            }
            if( m * dy < y && (m+1) * dy >= y){
                j = m;
            }
        }
        //std::cout << i << " " << j << std::endl;
        //tempLabel->setWordWrap(true);
        //tempLabel->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Maximum);
        tempLabel->setText(QString("Temperature is ") + QString::number(T[i][j]));
    }
    else {
        tempLabel->setText("Outside of figure");
    }
    //std::cout << x << " " << y << std::endl;
}

void Widget::paintEvent(QPaintEvent *){
    QPainter p(this);
    p.translate(0, yOffset);
    p.scale(1, -1);
    //p.drawLine(0,0,100,100);
    drawFigure(&p);
    if(module){
        drawTempMap(&p);
        drawBorder(&p);
    }
    else{
        drawPoints(&p);
    }
}

void Widget::drawTempMap(QPainter *p){
    tempMutex.lock();
    int size = T.size();
    double min = T[1][1], max = T[1][1];
    for(int i = 0; i < size; ++i){
        for(int j = 0; j < size; j++){
            if(T[i][j] > max)
                max = T[i][j];
            if(T[i][j] < min)
                min = T[i][j];
        }
    }
    for(int i = 0; i < size; ++i){
        for(int j = 0; j < size; j++){
            double x = i * dx;
            double y = j * dy;
            if(condition(x, y) != OUTSIDE){
                QColor color = tempToColor(T[i][j], min, max);
                QPen pen;
                pen.setColor(color);
                p->setPen(pen);
                p->fillRect((x - dx/2) * scale - 1, (y - dy/2) * scale - 1, dx * scale + 1, dy * scale + 1, pen.brush());
            }
        }
    }
    tempMutex.unlock();
}

void Widget::printTMatrix(){
    int size = T.size();
    for(int i = 0; i < size; ++i){
            for(int j = 0; j < size; ++j){
                double x = i * dx;
                double y = j * dy;
                if(condition(x, y) != OUTSIDE){
                    std::cout << std::setw(4) << T[i][j] << " ";
                }
            }
            std::cout << std::endl;
        }
}

QColor Widget::tempToColor(double temp, double min, double max){
    QColor color;
    min--;
    max++;
    double k = 0.5/(min - max);
    double b = - k * max;
    double norm = k * temp + b;
    if(norm < 0 || norm > 1){
        std::cout << norm << " " << temp <<  std::endl;
    }
    color.setHsvF(norm, 1, 1, 0.5);
    //qDebug() << color.name();
    return color;
}

void Widget::drawFigure(QPainter *p){
    QPen pen;
    pen.setWidth(2);
    p->setPen(pen);
    p->drawLine(0, 0, 0, leftSide * scale);
    p->drawLine(0, leftSide * scale, topSide * scale, leftSide * scale);
    p->drawLine(topSide * scale, leftSide * scale, botSide * scale, rightSide * scale);
    p->drawLine(botSide * scale, rightSide * scale, botSide * scale, 0);
    p->drawLine(botSide * scale, 0, 0, 0);
}

void Widget::drawPoints(QPainter *p){
    int size = T.size();
    QColor color;
    for(int i = 0; i < size; ++i){
        for(int j = 0; j < size; j++){
            double x = i * dx;
            double y = j * dy;
            switch (condition(x, y)) {
            case INSIDE:{
                color = QColor(Qt::red);
                break;
            }
            case LEFTBORDER:{
                continue;
                color = QColor(Qt::green);
                break;
            }
            case BOTBORDER:{
                continue;
                color = QColor(Qt::blue);
                break;
            }
            case TOPBORDER:{
                continue;
                color = QColor(Qt::white);
                break;
            }
            case RIGHTBORDER:{
                continue;
                color = QColor(Qt::white);
                break;
            }
            case OBLIQUEBORDER:{
                continue;
                color = QColor(Qt::white);
                break;
            }
            case OUTSIDE:{
                continue;
                color = QColor(Qt::black);
                break;
            }
            default:
                break;
            }
            QPen pen;
            pen.setColor(color);
            pen.setWidth(2);
            p->setPen(pen);
            p->drawPoint(x * scale, y * scale);
        }
    }
}

void Widget::calc(){
   //moveToThread(&calcThread);
   //connect(&calcThread, SIGNAL(started()), this, SLOT(calcBackgound()));
   //calcThread.start();
   calcButton->setDisabled(true);
   calcButton->setText("...");
   repaint();
   /*calcThread.start();
   calcThread.quit();
   calcButton->setText("Modulate");
   calcButton->setDisabled(false);*/
    QTimer *timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(repaint()));
    timer->start(1);
    std::thread t(&Widget::calcBackgound, this);
//    t.join();

    t.detach();
    calcButton->setText("Modulate");
    calcButton->setDisabled(false);
}

void Widget::drawBorder(QPainter *p){
    QColor color = QWidget::palette().color(QWidget::backgroundRole());
    QPen pen;
    pen.setColor(color);
    int rectWidth = 20;
    //top rectangle
    p->fillRect(0, leftSide * scale + 1, (topSide + dx) * scale, rectWidth, pen.brush());
    //bot rectangle
    p->fillRect(0, -rectWidth,(botSide + dx) * scale,  rectWidth - 1, pen.brush());
    //right rectagle
    p->fillRect(botSide * scale + 1, 0, rectWidth, leftSide * scale, pen.brush());
    //oblique rectangle
    p->translate(topSide * scale, leftSide * scale);
    p->rotate(-45);
    p->fillRect(0, 0, sqrt(pow(leftSide - rightSide,2) + pow(topSide - botSide, 2)) * scale, rectWidth, pen.brush());
    //reseting transform
    p->resetTransform();
    //update transform to old
    p->translate(0, yOffset);
    p->scale(1, -1);

}

void Widget::calcBackgound(){
    module = true;
    tempMutex.lock();
    int size = T.size();
    std::vector<std::vector<double> > nextT = T;
   // QVector<QVector<double> > nextT = T;
    tempMutex.unlock();
    double time = 0;
    while(time < modulateTime){
        for(int i = 0;i < n; ++i){
            nextT[0][i] = leftTemp;
            nextT[i][0] = botTemp;
        }
        for(int i = 0; i < size - 1; ++i){
            for(int j = 0; j < size - 1; j++){
                double x = i * dx;
                double y = j * dy;
                if(condition(x, y) == INSIDE){
                    nextT[i][j] = T[i][j] + dt*(a1 * (T[i+1][j] - 2 * T[i][j] + T[i-1][j]) /(dx * dx) + a2 * (T[i][j+1] - 2 * T[i][j] + T[i][j-1]) /(dy * dy));
                    //std::cout << nextT[i][j] << " " << T[i][j]<< " " <<T[i+1][j]<< " " <<T[i-1][j]<< std::endl;
                }
            }
        }

        for(int i = 0; i < size; ++i){
            for(int j = 0; j < size; j++){
                double x = i * dx;
                double y = j * dy;
                switch (condition(x, y)) {
                case TOPBORDER:{
                    nextT[i][j] = nextT[i][j-1] / (1 + dy);
                    break;
                }
                case RIGHTBORDER:{
                    nextT[i][j] = nextT[i-1][j] / (1 + dx);
                    break;
                }
                case OBLIQUEBORDER:{
                    nextT[i][j] = nextT[i-1][j-1] / (1 + sqrt(2) * dx);
                }
                default:
                    break;
                }
            }
        }
        tempMutex.lock();
        T = nextT;
        tempMutex.unlock();
        time += dt;
   }
    //calcButton->setText("Modulate");
    //calcButton->setDisabled(false);
    //std::cout << "FINISH" << std::endl;

}

pointCondition Widget::condition(double x, double y){
    double eps = 1e-4;
    if (x == 0 && y >= 0 && y <= leftSide)
        return LEFTBORDER;
    if(y == 0 && x >= 0 && x <= botSide)
        return BOTBORDER;
    if(fabs(x - botSide) < eps && y >= 0 && y <= rightSide)
        return RIGHTBORDER;
    if(fabs(y - leftSide) < eps && x >=0 && x <= topSide)
        return TOPBORDER;
    if( fabs(x + y - 11) < eps && x <= botSide && y <= leftSide)
        return OBLIQUEBORDER;
    if(x > 0 && y > 0 &&  x < botSide && y < leftSide && x + y < 11)
        return INSIDE;
    return OUTSIDE;
}

Widget::~Widget()
{

}
