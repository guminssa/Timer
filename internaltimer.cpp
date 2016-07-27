#include "internaltimer.h"

#define SECONDS_TO_MILLISECONDS 1000
#define QUANTUM_LENGTH (1 * SECONDS_TO_MILLISECONDS) // Trigger the callback every one second

InternalTimer::InternalTimer(int hours=0, int minutes=0,
                             bool repeat=false, const QString &message="Timer expired", const QString &sound="Firefly") :
                QTimer(), baseHours(hours), baseMinutes(minutes),
                repeatTimer(repeat), messageText(message), soundName(sound)
{

    if (baseMinutes > 59 )
    {
        baseHours += baseMinutes/60;
        baseMinutes %= 60;
    }

    // Calculate number of seconds in the requested interval
    baseMilliseconds = (baseHours * 3600 + baseMinutes * 60) * SECONDS_TO_MILLISECONDS;

    remainingMilliseconds = baseMilliseconds;

    if ( repeatTimer )
        setSingleShot(false);
    else
        setSingleShot(true);

}

// Returns the amount of time in milliseconds that the timer should wait before firing again.
//
int InternalTimer::GetNextInterval()
{
    if ( remainingMilliseconds > QUANTUM_LENGTH )
        return QUANTUM_LENGTH;
    return remainingMilliseconds;
}


// Decrements the time remaining by the quantum length that just expired.
// Returns true if there is still time remainig after the decrement.
bool InternalTimer::DecrementInterval()
{
    if ( remainingMilliseconds > QUANTUM_LENGTH )
    {
        remainingMilliseconds -= QUANTUM_LENGTH;
        return true;
    }
    else
    {
        remainingMilliseconds = 0;
        return false;
    }
}


QString InternalTimer::toString()
{
    int hours, minutes, seconds, convertedSeconds;


    convertedSeconds = (remainingMilliseconds/SECONDS_TO_MILLISECONDS); // Total remaining time in seconds
    hours = convertedSeconds/3600;
    convertedSeconds -= (hours*3600);
    minutes = convertedSeconds/60;
    seconds = convertedSeconds%60;
    return QString(QString("%1").arg(hours, 2, 10, QChar('0')) + ":" +
                   QString("%1").arg(minutes, 2, 10, QChar('0')) + ":" +
                   QString("%1").arg(seconds, 2, 10, QChar('0')) + " " + this->messageText);
}

