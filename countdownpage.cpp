#include "countdownpage.h"
#include "ui_countdownpage.h"

#include <QTimer>

CountDownPage::CountDownPage(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::CountDownPage)
{
    ui->setupUi(this);
    QTimer::singleShot(0, this, SLOT(showFullScreen()));
}

CountDownPage::~CountDownPage()
{
    delete ui;
}
