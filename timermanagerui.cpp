#include "timermanagerui.h"


#define LABEL_HEIGHT 20
#define ROW1_OFFSET 25
#define ROW2_OFFSET 50
#define ROW3_OFFSET 80
#define ROW4_OFFSET 120
#define ROW5_OFFSET 160
#define ROW6_OFFSET 200

TimerManagerUI::TimerManagerUI(QWidget *parent = 0) : QWidget(parent)
{
    // SetCentralWidget seems to force the child widget to encompass the entire geometry
    // of the parent widget.  So don't do it (or find an appropriate child widget
    // like a grid).

    // Initialize the TimerManagerUI itself
    this->setMinimumWidth(450);
    this->setMinimumHeight(500);
    setWindowTitle("Timers");

    // Label
    timerLabel = new QLabel("Create a new timer:", this);
    timerLabel->setFixedWidth(150);
    timerLabel->setFixedHeight(20);
    timerLabel->move(2, 0);

    /* =========================================================*/

    hoursLabel = new QLabel("Hours", this);
    hoursLabel->setFixedWidth(50);
    hoursLabel->setFixedHeight(20);
    hoursLabel->move(2, ROW1_OFFSET);

    minutesLabel = new QLabel("Minutes", this);
    minutesLabel->setFixedWidth(60);
    minutesLabel->setFixedHeight(20);
    minutesLabel->move(55, ROW1_OFFSET);

    messageLabel = new QLabel("Message", this);
    messageLabel->setFixedWidth(60);
    messageLabel->setFixedHeight(20);
    messageLabel->move(120, ROW1_OFFSET);

    /* =========================================================*/

    // Hour selection
    hourSpin = new QSpinBox(this);
    hourSpin->setFixedWidth(50);
    hourSpin->setFixedHeight(30);
    hourSpin->setMaximum(23);
    hourSpin->setMinimum(0);
    hourSpin->setValue(0);
    hourSpin->move(2, ROW2_OFFSET);

    // Minute selection
    minuteSpin = new QSpinBox(this);
    minuteSpin->setFixedWidth(50);
    minuteSpin->setFixedHeight(30);
    minuteSpin->setMaximum(59);
    minuteSpin->setMinimum(0);
    minuteSpin->setValue(0);
    minuteSpin->move(55, ROW2_OFFSET);


    // Add a text input line to provide a message that will be displayed when the timer expires
    alarmMessage = new QTextEdit(this);
    alarmMessage->setFixedWidth(320);
    alarmMessage->setFixedHeight(30);
    alarmMessage->move(120, ROW2_OFFSET);

    /* =========================================================*/
    soundLabel = new QLabel("Sound: ", this);
    soundLabel->setFixedWidth(50);
    soundLabel->setFixedHeight(30);
    soundLabel->move(2, ROW3_OFFSET);

    soundDropdown = new QComboBox(this);
    soundDropdown->addItem("ChaChing"); // From http://soundbible.com/1997-Cha-Ching-Register.html
    soundDropdown->addItem("Clink"); // From http://soundbible.com/1967-Clinking-Teaspoon.html
    soundDropdown->setFixedWidth(200);
    soundDropdown->setFixedHeight(30);
    soundDropdown->move(60, ROW3_OFFSET);


    // Schedule the timer to run
    addTimer = new QPushButton(this);
    addTimer->setFixedWidth(50);
    addTimer->setFixedHeight(30);
    addTimer->move(275, ROW3_OFFSET);
    addTimer->setText("Add");

    // Note: In order to allow a class to define a signal or slot, the Q_OBJECT macro must be
    // present at the start of the class definition.  If adding the macro seems to break the build,
    // clean all and then call QMake.
    connect(addTimer, SIGNAL(pressed()), this, SLOT(on_addtimer_pressed()));


    /* =========================================================*/
    listLabel = new QLabel("Scheduled timers:", this);
    listLabel->setFixedWidth(150);
    listLabel->setFixedHeight(30);
    listLabel->move(2, ROW5_OFFSET);

    /* =========================================================*/
    listModel = new QStringListModel(this);
    //QStringList list;
    //list << "A" << "B";
    //listModel->setStringList(list);
    listView = new QListView(this);
    listView->setModel(listModel);
    listView->move(2, ROW6_OFFSET);
    listView->setFixedWidth(350);
    listView->setFixedHeight(100);
    listView->show();
    selectionModel = listView->selectionModel();


    deleteTimer = new QPushButton(this);
    deleteTimer->move(355, ROW6_OFFSET);
    deleteTimer->setText("Delete");

    // Connect the Delete button
    connect(deleteTimer, SIGNAL(pressed()), this, SLOT(on_deletetimer_pressed()));
}


void TimerManagerUI::on_deletetimer_pressed()
{
    QModelIndexList selectedList =  selectionModel->selectedIndexes(); // Actually a QList<QModelIndex>
    for ( QModelIndexList::const_iterator it = selectedList.constBegin(); it != selectedList.constEnd(); it++ )
    {
        int row;
        QModelIndex index = *it;
        InternalTimer *timer;
        row = index.row();
        timer = timerVector[row];

        // Cancel the active timer if it is the one getting deleted
        if ( row == 0 )
        {
            timer->stop();
            QMessageBox msg(QMessageBox::NoIcon, "Timer modified", "Canceled active timer");
            if ( timerVector.size() > 1 )
            {
                // Start the next timer in its place
                timerVector[1]->start(timerVector[1]->GetNextInterval());
            }
        }

        // Remove the timer from the vector
        timerVector.remove(row);

        // Remove the timer from the model and the view
        listModel->removeRow(index.row());
    }
}

void TimerManagerUI::on_addtimer_pressed()
{
    int hours, minutes;

    hours = hourSpin->value();
    minutes = minuteSpin->value();

    InternalTimer *newTimer = new InternalTimer(hours, minutes, false, alarmMessage->toPlainText(), soundDropdown->itemText(soundDropdown->currentIndex()));
    timerVector.push_back(newTimer);
    connect(newTimer, SIGNAL(timeout()), this, SLOT(TimerExpired()));

    // There were no previous timers scheduled, so start this new one immediately
    if ( timerVector.size() == 1)
    {
        newTimer->start(newTimer->GetNextInterval());
    }

    // Update the timer list
    listModel->insertRow(listModel->rowCount());
    QModelIndex index = listModel->index(listModel->rowCount()-1, 0); // Index of the last item in the list
    listModel->setData(index, newTimer->toString(), Qt::EditRole);

}


// Slot that is called when the current timer has expired.
void TimerManagerUI::TimerExpired()
{
    // Find the timer that just expired
    InternalTimer *currentTimer = timerVector.front();

    if ( currentTimer == Q_NULLPTR )
        return; // Nothing to do; should never happen

    // Decrement the remaining time by the quantum that just completed.
    if ( !currentTimer->DecrementInterval() )
    {
        // The timer has now expired

        // Play an alert sound
        QMediaPlayer player(this);
        QString sound = "qrc:/sounds/" + currentTimer->GetSound() +".mp3";
        player.setMedia(QUrl(sound));
        player.play();
        //qDebug() << player.errorString();


        // Display an alert message
        QMessageBox msg(QMessageBox::NoIcon, "Timer expired", currentTimer->GetMessage());
        msg.exec();


        // Remove the expired timer
        timerVector.pop_front();
        delete currentTimer;

        // Remove the timer from the list
        listModel->removeRow(0);


        // If there are more timers scheduled, start the next one
        if ( timerVector.size() > 0 )
        {
            currentTimer = timerVector.front();
            currentTimer->start(currentTimer->GetNextInterval());
        }
    }
    else
    {
        // There is still time left on the timer
        currentTimer->start(currentTimer->GetNextInterval());
        QModelIndex index = listModel->index(0, 0); // Index of the first item in the list
        listModel->setData(index, currentTimer->toString(), Qt::EditRole);
    }
}

