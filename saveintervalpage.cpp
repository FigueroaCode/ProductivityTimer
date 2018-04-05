#include "saveintervalpage.h"
#include "ui_saveintervalpage.h"

saveIntervalPage::saveIntervalPage(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::saveIntervalPage)
{
    ui->setupUi(this);
}

saveIntervalPage::~saveIntervalPage()
{
    delete ui;
}
