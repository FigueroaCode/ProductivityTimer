#ifndef COUNTDOWNPAGE_H
#define COUNTDOWNPAGE_H

#include <QMainWindow>
#include <QLabel>
#include <QPushButton>
#include <QTimer>
#include "mainwindow.h"

namespace Ui {
class CountDownPage;
}

class CountDownPage : public QMainWindow
{
    Q_OBJECT
public slots:
    void update();
public:
    explicit CountDownPage(QWidget *parent = 0);
    void setTitleText(QString text);
    void setCountDownText(QString text);
    void setButtonState(QString text);
    void setRepsCount(int count);
    void setPrepTime(QString time);
    void setWorkTime(QString time);
    void setRestTime(QString time);
    void setTotalTime(QString time);
    void setCurrentTime(QString time);
    ~CountDownPage();

private slots:
    void on_pauseResumeButton_clicked();

    void on_restartButton_clicked();

    void on_closeButton_clicked();

private:
    Ui::CountDownPage *ui;
    QLabel *titleLabel;
    QLabel *repsLabel;
    QLabel *countDownLabel;
    QPushButton *pauseResumeBtn;
    QTimer *timer;

    bool prepDone;
    bool paused;
    bool inWork;
    int currentWorkRep;
    int currentRestRep;
    QString currentTime;

    int repsCount;
    QString prepTime;
    QString workTime;
    QString restTime;
    QString totalTime;

    int getHours(QStringList timeList);
    int getMinutes(QStringList timeList);
    int getSeconds(QStringList timeList);
    void updateRepLabel(int rep);
};

#endif // COUNTDOWNPAGE_H
