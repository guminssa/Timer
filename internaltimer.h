#ifndef INTERNALTIMER_H
#define INTERNALTIMER_H

#include <QObject>
#include <QString>
#include <QTimer>
#include <QMessageBox>



class InternalTimer: public QTimer
{
    Q_OBJECT

public:
    InternalTimer(int hours, int minutes, bool repeat, const QString &message, const QString &sound);
    const QString & GetMessage() { return messageText; }
    int GetNextInterval();
    bool DecrementInterval();
    const QString & GetSound() { return soundName; }
    QString toString();

private:
    int baseHours, baseMinutes, baseMilliseconds;
    int remainingMilliseconds;
    bool repeatTimer;
    QString messageText;
    QString soundName;
};


#endif // INTERNALTIMER_H

