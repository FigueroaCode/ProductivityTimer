#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLineEdit>
#include <QLabel>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_repsUpBtn_clicked();

    void on_repsDownBtn_clicked();

    void on_workUpBtn_clicked();

    void on_workDownBtn_clicked();

    void on_restUpBtn_clicked();

    void on_restDownBtn_clicked();

    void on_prepUpBtn_clicked();

    void on_prepDownBtn_clicked();

    void on_workTimeEdit_editingFinished();

    void on_workTimeEdit_returnPressed();

    void on_restTimeEdit_editingFinished();

    void on_restTimeEdit_returnPressed();

    void on_prepTimeEdit_editingFinished();

    void on_prepTimeEdit_returnPressed();

    void on_startButton_clicked();

    void on_actionSave_Interval_triggered();

    void on_repsEdit_editingFinished();

    void on_repsEdit_returnPressed();

private:
    Ui::MainWindow *ui;

    //Widgets
    QLineEdit *repsEdit;
    QLineEdit *workTimeEdit;
    QLineEdit *restTimeEdit;
    QLineEdit *prepTimeEdit;
    QLabel *totalTimeLabel;

    //globals
    int totalTimeInSec;

    //functions
    void updateTimeLabel(int totalTime);
    void updateTotalTime(int *timeInSec);
};

#endif // MAINWINDOW_H
