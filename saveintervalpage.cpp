#include "saveintervalpage.h"
#include "ui_saveintervalpage.h"
#include "mainwindow.h"

#include "QMessageBox"

saveIntervalPage::saveIntervalPage(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::saveIntervalPage)
{
    ui->setupUi(this);

    intervalName = ui->intervalNameEdit;
}

saveIntervalPage::~saveIntervalPage()
{
    delete ui;
}

void saveIntervalPage::on_saveButton_clicked()
{
    if(this->intervalName->text() != ""){
        QString dirName = "Productivity Intervals";
        QString filepath = QFileDialog::getExistingDirectory(
                    this,
                    "Select Directory",
                    QDir::homePath());

        //Find the time values currently in place and store them

        //make folder to save Intervals in directory chosen
        QDir dir(filepath);
        if(!dir.exists()){
            dir.mkpath(".");
        }else{
            //Make Folder
            if(!dir.exists(dirName)){
                dir.mkdir(dirName);
            }
        }

        //New dir for creating files will be filepath
        QString savePath = filepath + "/" + dirName;
        dir.cd(savePath);

        //TODO: Save this directory for future use.

        //Write a new file that stores the interval as text
        QFile file(savePath + "/" + intervalName->text() + ".txt");
        if( file.open(QIODevice::WriteOnly | QIODevice::Text)){
            QTextStream out(&file);
            out << saveString;
        }

        file.close();
        this->hide();
    }else{
        QMessageBox::information(this,tr("Error"),tr("Please enter a name for this interval"));
    }

}

void saveIntervalPage::on_cancelButton_clicked()
{
    this->hide();
}

void saveIntervalPage::setSaveString(QString saveString)
{
    this->saveString = saveString;
}
