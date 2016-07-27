#include "timer.h"
#include "ui_timer.h"
#include "timermanagerui.h"
#include <QMessageBox>
#include <QTime>

Timer::Timer(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Timer)
{
    ui->setupUi(this);
    timerManagerUI = new TimerManagerUI(ui->centralWidget);
}

Timer::~Timer()
{
    delete ui;
}
