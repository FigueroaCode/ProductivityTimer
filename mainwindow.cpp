#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "countdownpage.h"
#include "saveintervalpage.h"

#include <QDebug>
#include <QTime>
#include <QTimer>
#include <QFileDialog>
#include <QDir>
#include <QFile>

CountDownPage *countDownPage = NULL;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    repsEdit = ui->repsEdit;
    workTimeEdit = ui->workTimeEdit;
    restTimeEdit = ui->restTimeEdit;
    prepTimeEdit = ui->prepTimeEdit;
    totalTimeLabel = ui->totalTimeLabel;
    loadIntervalMenu = ui->menuLoad_Interval;

    totalTimeInSec = 0;

    //Read the determined directory from a saved file, and then cd into it.
    savePath = readFromFile(QDir::homePath()+"/StateInfo/Directory_Path.txt");
    //cd into savePath directory
    QDir intervalDir(savePath);
    //Load the interval files into the menu
    if(intervalDir.exists())
    {
        QStringList dirs = intervalDir.entryList(QDir::Files | QDir::NoDotAndDotDot);
        for(int i = 0; i < dirs.size(); i++)
        {
            addIntervalAction(dirs.at(i));
        }
        //check every 2 seconds if a new file should be added
        QTimer *timer = new QTimer(this);
        connect(timer, SIGNAL(timeout()),this,SLOT(addNewAction()));
        timer->start(2000);
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::addNewAction()
{
    QDir intervalDir(savePath);
    //Load the interval files into the menu
    if(intervalDir.exists())
    {
        QStringList dirs = intervalDir.entryList(QDir::Files | QDir::NoDotAndDotDot);
        //get the last file, and check if its the same as the current last action interval
        if(dirs.size() > 0 && lastIntervalAction != dirs.at(dirs.size()-1))
        {
            addIntervalAction(dirs.at(dirs.size()-1));
        }
    }
}
//------ Rep spinner ---------//

void MainWindow::on_repsUpBtn_clicked()
{
    //Increment rep by 1 on label
    try
    {
     //get current value
     int repAmount = repsEdit->text().toInt();
     //increment it
     repAmount++;
     //convert to string and display it
     repsEdit->setText(QString::number(repAmount));
     updateTotalTime(&totalTimeInSec);
    }catch(std::exception &e)
    {
        //value in label is not a number
    }
}

void MainWindow::on_repsDownBtn_clicked()
{
    //Decrement rep by 1 on label
    try
    {
     //get current value
     int repAmount = repsEdit->text().toInt();
     //decrement it only if it is greater than 0
     if(repAmount > 0)
     {
         repAmount--;
         //convert to string and display it
         repsEdit->setText(QString::number(repAmount));
         updateTotalTime(&totalTimeInSec);
     }

    }catch(std::exception &e)
    {
        //value in label is not a number
    }
}

void MainWindow::on_repsEdit_editingFinished()
{
    if(repsEdit->text().toInt() > 0)
    {
        updateTotalTime(&totalTimeInSec);
    }else
    {
        repsEdit->setText("0");
    }
}

void MainWindow::on_repsEdit_returnPressed()
{
    if(repsEdit->text().toInt() > 0)
    {
        updateTotalTime(&totalTimeInSec);
    }else
    {
        repsEdit->setText("0");
    }
}
//------------- Helper Methods ----------------//
QString MainWindow::readFromFile(QString path)
{
    QFile file(path);
    file.open(QIODevice::ReadOnly | QIODevice::Text);
    QTextStream in(&file);
    QString text = in.readLine();
    file.close();

    return text;
}

void MainWindow::addIntervalAction(QString fileName)
{
    QAction *intervalAction = new QAction(this);
     //get a substring of the file name without the .txt portion
    QString name = fileName.left(fileName.lastIndexOf("."));
    intervalAction->setText(name);
    //Convert data into variant in order to access it in the update interval function
    QVariant v = qVariantFromValue(name);
    intervalAction->setData(v);
    //set trigger function for this action
    QObject::connect(intervalAction,SIGNAL(triggered(bool)), this, SLOT(updateInterval()));
    loadIntervalMenu->addAction(intervalAction);
    lastIntervalAction = fileName;
}

void MainWindow::updateInterval()
{
    QAction *intervalAction = qobject_cast<QAction *>(sender());
    QVariant v = intervalAction->data();
    QString fileName = (QString) v.value<QString>();
    //read from file the interval
    QString interval = readFromFile(savePath+"/"+fileName+".txt");
    if(interval.size() > 0)
    {
        QStringList intervalTimes = interval.split(";");
        //set the current time for all the widgets
        QList<QLineEdit*> timeEdits;
        timeEdits.append(repsEdit);
        timeEdits.append(workTimeEdit);
        timeEdits.append(restTimeEdit);
        timeEdits.append(prepTimeEdit);
        for(int i = 0; i < intervalTimes.size(); i++)
        {
            timeEdits.at(i)->setText(intervalTimes.at(i));
        }
        //update total time
        updateTotalTime(&totalTimeInSec);//this is just passed in to keep it updated globally
    }
}

void MainWindow::updateTimeLabel(int totalTime)
{
    //multiply total time by number of reps
    totalTime *= repsEdit->text().toInt();
    int hours = totalTime / 3600;
    int currentTimeInSec = (totalTime) - (hours * 3600);
    int minutes = currentTimeInSec / 60;
    int seconds = currentTimeInSec - (minutes * 60);
    totalTimeLabel->setText(QTime(hours,minutes,seconds).toString(Qt::TextDate));
}

int getHours(QStringList timeList)
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

int getMinutes(QStringList timeList)
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

int getSeconds(QStringList timeList)
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

bool incrementTime(QLineEdit *timeEdit)
{
    //Increment time by 1 minute
    try
    {
        QString workTimeText = timeEdit->text();
        QStringList timeList = workTimeText.split(":");
        int hours = getHours(timeList);
        int minutes = getMinutes(timeList);
        int seconds = getSeconds(timeList);
        //increment time
        minutes++;
        //handle new hour reached
        if(minutes > 59)
        {
            hours++;
            minutes = 0;
        }
        //update time
        timeEdit->setText(QTime(hours,minutes,seconds).toString(Qt::TextDate));
        return true;
    }catch(std::exception &e)
    {
        //non number input
    }

    return false;
}

bool decrementTime(QLineEdit *timeEdit)
{
    //Decrement time by 1 minute
    try
    {
        QString workTimeText = timeEdit->text();
        QStringList timeList = workTimeText.split(":");
        int hours = getHours(timeList);
        int minutes = getMinutes(timeList);
        int seconds = getSeconds(timeList);
        //decrement time
        //handle new hour reached
        if(minutes > 0)
        {
            minutes--;
        }else if(minutes == 0 && hours > 0)
        {
            hours--;
            minutes = 59;
        }else
        {
            return false;
        }
        //update time
       timeEdit->setText(QTime(hours,minutes,seconds).toString(Qt::TextDate));
       return true;
    }catch(std::exception &e)
    {
        //non number input
    }

    return false;
}

void MainWindow::updateTotalTime(int *timeInSec)
{
    //Get work time text
    QString workTimeText = workTimeEdit->text();
    QStringList timeList = workTimeText.split(":");
    //transform text into time for each line edit
    //WORK TIME
    int workHour = getHours(timeList);
    int workMinutes = getMinutes(timeList);
    int workSec = getSeconds(timeList);
    //REST TIME
    QString restTimeText = restTimeEdit->text();
    timeList = restTimeText.split(":");
    int restHour = getHours(timeList);
    int restMinutes = getMinutes(timeList);
    int restSec = getSeconds(timeList);
    //PREP TIME
    QString prepTimeText = prepTimeEdit->text();
    timeList = prepTimeText.split(":");
    int prepHour = getHours(timeList);
    int prepMinutes = getMinutes(timeList);
    int prepSec = getSeconds(timeList);
    //turn all the time into seconds
    int hoursInSec = (workHour * 3600) + (restHour * 3600) + (prepHour * 3600);
    int minutesInSec = (workMinutes * 60) + (restMinutes * 60) + (prepMinutes * 60);
    //need to keep track of it for when the buttons try to increment or decrement the time
    int total = workSec + restSec + prepSec + hoursInSec + minutesInSec;
    *timeInSec = total;
    //update time label
    updateTimeLabel(total);
}

//---------------- Spin Box Events -------------------//
void MainWindow::on_workUpBtn_clicked()
{
    if(incrementTime(workTimeEdit))
    {
        totalTimeInSec += 60;
        updateTimeLabel(totalTimeInSec);
    }
}

void MainWindow::on_workDownBtn_clicked()
{
   if(decrementTime(workTimeEdit))
   {
       if(totalTimeInSec >= 60)
       {
           totalTimeInSec -= 60;
           updateTimeLabel(totalTimeInSec);
       }
   }
}

void MainWindow::on_restUpBtn_clicked()
{
    if(incrementTime(restTimeEdit))
    {
        totalTimeInSec += 60;
        updateTimeLabel(totalTimeInSec);
    }
}

void MainWindow::on_restDownBtn_clicked()
{
    if(decrementTime(restTimeEdit))
    {
        if(totalTimeInSec >= 60)
        {
            totalTimeInSec -= 60;
            updateTimeLabel(totalTimeInSec);
        }
    }
}

void MainWindow::on_prepUpBtn_clicked()
{
    if(incrementTime(prepTimeEdit))
    {
        totalTimeInSec += 60;
        updateTimeLabel(totalTimeInSec);
    }
}

void MainWindow::on_prepDownBtn_clicked()
{
    if(decrementTime(prepTimeEdit))
    {
        if(totalTimeInSec >= 60)
        {
            totalTimeInSec -= 60;
            updateTimeLabel(totalTimeInSec);
        }
    }
}

//----------------- Line Edit input changes --------------------//
void MainWindow::on_workTimeEdit_editingFinished()
{
    updateTotalTime(&totalTimeInSec);
}

void MainWindow::on_workTimeEdit_returnPressed()
{
    updateTotalTime(&totalTimeInSec);
}

void MainWindow::on_restTimeEdit_editingFinished()
{
    updateTotalTime(&totalTimeInSec);
}

void MainWindow::on_restTimeEdit_returnPressed()
{
    updateTotalTime(&totalTimeInSec);
}

void MainWindow::on_prepTimeEdit_editingFinished()
{
    updateTotalTime(&totalTimeInSec);
}

void MainWindow::on_prepTimeEdit_returnPressed()
{
    updateTotalTime(&totalTimeInSec);
}
//----------- Start Button -------------------//
void MainWindow::on_startButton_clicked()
{
    if(totalTimeInSec > 0 && repsEdit->text().toInt() > 0)
    {
        countDownPage = new CountDownPage();
        //pass data
        countDownPage->setTitleText("Get Ready!");
        countDownPage->setRepsCount(repsEdit->text().toInt());
        countDownPage->setCountDownText(prepTimeEdit->text());
        countDownPage->setButtonState("PAUSE");
        countDownPage->setPrepTime(prepTimeEdit->text());
        countDownPage->setWorkTime(workTimeEdit->text());
        countDownPage->setRestTime(restTimeEdit->text());
        countDownPage->setTotalTime(totalTimeLabel->text());
        countDownPage->setCurrentTime(prepTimeEdit->text());
        countDownPage->setCurrentTotalTime(totalTimeLabel->text());
        //Open Window
        countDownPage->show();
    }
}

void MainWindow::on_actionSave_Interval_triggered()
{
    saveIntervalPage *nw = new saveIntervalPage();
    QString repAmount = repsEdit->text();
    QString workTime = workTimeEdit->text();
    QString restTime = restTimeEdit->text();
    QString prepTime = prepTimeEdit->text();
    QString timeToString = repAmount + ";" + workTime
                + ";" + restTime + ";" + prepTime;
    nw->setSaveString(timeToString);
    nw->show();
}


