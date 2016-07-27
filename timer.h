#ifndef ACTIVITYMANAGER_H
#define ACTIVITYMANAGER_H

#include <QMainWindow>
#include <QMdiSubWindow>
#include "timermanagerui.h"

namespace Ui {
class Timer;
}

class Timer : public QMainWindow
{
    Q_OBJECT

public:
    explicit Timer(QWidget *parent = 0);
    ~Timer();

private slots:


private:
    Ui::Timer *ui;
    //QMdiSubWindow *timerWindow;
    TimerManagerUI *timerManagerUI;
};

#endif // ACTIVITYMANAGER_H
