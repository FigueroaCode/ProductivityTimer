#include "countdownpage.h"
#include "ui_countdownpage.h"
#include <QTime>
#include <QDebug>

CountDownPage::CountDownPage(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::CountDownPage)
{
    ui->setupUi(this);
    //Set the full screen on entering, optional delay
    //QTimer::singleShot(0, this, SLOT(showFullScreen()));
    //Reference widgets
    //mainWindow = parent;
    titleLabel = ui->titleLabel;
    repsLabel = ui->repsLabel;
    countDownLabel = ui->countDownLabel;
    totalTimeLeftLabel = ui->totalTimeLeftLabel;
    totalTimeLeftButton = ui->totalTimeDisplayButton;
    pauseResumeBtn = ui->pauseResumeButton;
    totalTimeTitleLabel = ui->totalTimeLeftTitleLabel;
    totalTimeDisplayed = true;
    setUp();
}

CountDownPage::~CountDownPage()
{
    delete ui;
}

void CountDownPage::update()
{
    //take off a second from current time and update it on label
    QStringList timeList = currentTime.split(":");
    int hours = getHours(timeList);
    int minutes = getMinutes(timeList);
    int seconds = getSeconds(timeList);
    if(hours == 0 && minutes == 0 && seconds == 0)
    {
        //Set new time
        if(!prepDone)
        {
            prepDone = true;
            repsLabel->show();
        }
        //Check this only if currentRestRep is less than totalReps
        if(currentRestRep < repsCount && inWork)
        {
         inWork = false;
         //Change title
         titleLabel->setText("REST REPS");
         currentTime = restTime;
         currentRestRep++;
         updateRepLabel(currentRestRep);
        }else if(currentRestRep < repsCount && !inWork)
        {
          inWork = true;
          //Change title
          titleLabel->setText("WORK REPS");
          currentTime = workTime;
          currentWorkRep++;
          updateRepLabel(currentWorkRep);
        }else
        {
            //Change title to finished
            titleLabel->setText("All Done, Great Job!");
            repsLabel->hide();
            //handle finished state
            timer->stop();
        }
    }else
    {
        seconds--;
        if(seconds < 0)
        {
            seconds = 59;
            minutes--;
            if(minutes < 0 && hours > 0)
            {
                minutes = 59;
                hours--;
            }else if(minutes < 0)
            {
                minutes = 59;
            }
        }
        currentTime = QTime(hours,minutes,seconds).toString(Qt::TextDate);
    }
    //Set new time
    setCountDownText(currentTime);
    //update total time
    updateTotalTime();
}

void CountDownPage::updateTotalTime()
{
    //take off a second from total time and update it on label
    QStringList timeList = currentTotalTime.split(":");
    int hours = getHours(timeList);
    int minutes = getMinutes(timeList);
    int seconds = getSeconds(timeList);
    seconds--;
    if(seconds < 0)
    {
        seconds = 59;
        minutes--;
        if(minutes < 0 && hours > 0)
        {
            minutes = 59;
            hours--;
        }else if(minutes < 0)
        {
            minutes = 59;
        }
    }
    currentTotalTime = QTime(hours,minutes,seconds).toString(Qt::TextDate);
    totalTimeLeftLabel->setText(currentTotalTime);
}

//------------ Helper Methods ----------------//
void CountDownPage::updateRepLabel(int rep)
{
    repsLabel->setText(QString::number(rep)+"/"+QString::number(repsCount));
}

int CountDownPage::getHours(QStringList timeList)
{
    try
    {
        if(timeList.size() == 3)
        {
           return timeList.at(0).toInt();
        }
        //otherwise return 0 hours
    }catch(std::exception &e)
    {
        //non number input
    }

    return 0;
}

int CountDownPage::getMinutes(QStringList timeList)
{
    try
    {
        if(timeList.size() == 3)
        {
           return timeList.at(1).toInt();
        }else if(timeList.size() == 2)
        {
            return timeList.at(0).toInt();
        }
        //otherwise return 0 minutes
    }catch(std::exception &e)
    {
        //non number input
    }
    return 0;
}

int CountDownPage::getSeconds(QStringList timeList)
{
    try
    {
        if(timeList.size() == 3)
        {
           return timeList.at(2).toInt();
        }else if(timeList.size() == 2)
        {
            return timeList.at(1).toInt();
        }else if(timeList.size() == 1)
        {
            return timeList.at(0).toInt();
        }
        //otherwise return 0 seconds
    }catch(std::exception &e)
    {
        //non number input
    }
    return 0;
}


//---------------- Setters -------------------//
void CountDownPage::setTitleText(QString text)
{
    titleLabel->setText(text);
}

void CountDownPage::setCountDownText(QString text)
{
    countDownLabel->setText(text);
}

void CountDownPage::setButtonState(QString text)
{
    pauseResumeBtn->setText(text);
}

void CountDownPage::setRepsCount(int count)
{
    repsCount = count;
}

void CountDownPage::setPrepTime(QString time)
{
    prepTime = time;
}

void CountDownPage::setWorkTime(QString time)
{
    workTime = time;
}

void CountDownPage::setRestTime(QString time)
{
    restTime = time;
}

void CountDownPage::setTotalTime(QString time)
{
    totalTime = time;
}

void CountDownPage::setCurrentTime(QString time)
{
    currentTime = time;
}

void CountDownPage::setCurrentTotalTime(QString time)
{
    currentTotalTime = time;
}
//--------------------- Buttons ---------------------//
void CountDownPage::on_pauseResumeButton_clicked()
{//Pause or Resume Timer
    if(!paused)
    {
        //pause timer
        timer->stop();
        //in pause state now
        paused = true;
        //change button text to resume
        pauseResumeBtn->setText("RESUME");
    }else
    {
        //resume timer
        timer->start(1000);//with timeout of 1 second
        paused = false;
        //change button text to pause
        pauseResumeBtn->setText("PAUSE");
    }

}
//----------------------TODO--------------------
void CountDownPage::setUp()
{
    //set current rep to 0
    currentRestRep = 0;
    currentWorkRep = 0;
    //Set title to Get Ready!
    titleLabel->setText("Get Ready!");
    //set current time to prep time
    currentTime = prepTime;
    currentTotalTime = totalTime;
    //set paused to false
    paused = false;
    //set inwork to false
    inWork = false;
    //set prep done to false
    prepDone = false;
    //set total time
    totalTimeLeftLabel->setText(totalTime);
    //Hide reps label
    repsLabel->setText(QString::number(currentWorkRep)+"/"+QString::number(repsCount));
    repsLabel->hide();
    //Set up timer to alert every second
    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(update()));
    timer->start(1000);

}

void CountDownPage::on_restartButton_clicked()
{
    //RESTART from prep stage
    setUp();
}

void CountDownPage::on_closeButton_clicked()
{
    //close window
    this->close();
}

void CountDownPage::on_totalTimeDisplayButton_clicked()
{
    if(totalTimeDisplayed)
    {
        totalTimeDisplayed = false;
        //currently shown, so hide it
        totalTimeTitleLabel->hide();
        totalTimeLeftLabel->hide();
        //change icon on button to point to the right
        totalTimeLeftButton->setArrowType(Qt::RightArrow);
    }else
    {
        totalTimeDisplayed = true;
        //currently hidden, so show it
        totalTimeTitleLabel->show();
        totalTimeLeftLabel->show();
        //change icon on button to point to the left
        totalTimeLeftButton->setArrowType(Qt::LeftArrow);
    }
}
