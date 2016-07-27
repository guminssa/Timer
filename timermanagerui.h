#ifndef TIMERWINDOW_H
#define TIMERWINDOW_H

#include <QWidget>
#include <QDateTimeEdit>
#include <QMdiSubWindow>
#include <QMainWindow>
#include <QDateTime>
#include <QDate>
#include <QTime>
#include <QTextEdit>
#include <QPushButton>
#include <QLabel>
#include <QMessageBox>
#include <QSpinBox>
#include <QVector>
#include <QMediaPlayer>
#include <QComboBox>
#include <QListView>
#include <QStringListModel>
#include <QItemSelectionModel>
#include <QModelIndexList>

#include "internaltimer.h"

class TimerManagerUI : public QWidget
{
    Q_OBJECT

public:
    TimerManagerUI(QWidget *parent);

public slots:
    void on_addtimer_pressed();
    void on_deletetimer_pressed();
    void TimerExpired();


private:
    //QDateTimeEdit *dateTime;
    QTextEdit *alarmMessage;
    QPushButton *addTimer;
    QLabel *timerLabel, *hoursLabel, *minutesLabel, *messageLabel,
        *soundLabel, *listLabel;
    QSpinBox *hourSpin, *minuteSpin;
    QComboBox *soundDropdown;
    QVector<InternalTimer *> timerVector;
    QStringListModel *listModel;
    QListView *listView;
    QItemSelectionModel *selectionModel;
    QPushButton *deleteTimer;
};

#endif // TIMERWINDOW_H

