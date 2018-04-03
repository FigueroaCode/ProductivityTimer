#ifndef COUNTDOWNPAGE_H
#define COUNTDOWNPAGE_H

#include <QMainWindow>

namespace Ui {
class CountDownPage;
}

class CountDownPage : public QMainWindow
{
    Q_OBJECT

public:
    explicit CountDownPage(QWidget *parent = 0);
    ~CountDownPage();

private:
    Ui::CountDownPage *ui;
};

#endif // COUNTDOWNPAGE_H
