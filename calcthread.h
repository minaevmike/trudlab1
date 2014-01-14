#ifndef CALCTHREAD_H
#define CALCTHREAD_H

#include <QThread>

class calcThread : public QThread
{
    Q_OBJECT
public:
    calcThread();
};

#endif // CALCTHREAD_H
