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
        QDir dir(QDir::homePath());
        QString savePath = "";
        QString dirName = "Productivity Intervals";

        //Check if a save directory has already been established.
        if(!dir.exists("StateInfo"))
        {
            QString filepath = QFileDialog::getExistingDirectory(
                        this,
                        "Select Directory",
                        QDir::homePath());

            //make folder to save Intervals in directory chosen
            QDir dir(filepath);
            if(!dir.exists())
            {
                dir.mkpath(".");
            }else{
                //Make Folder
                if(!dir.exists(dirName)){
                    dir.mkdir(dirName);
                }
            }

            //New dir for creating files will be savePath
            savePath = filepath + "/" + dirName;
            dir.cd(savePath);

            //Save this directory for future use.
            QFile file(QDir::homePath()+"/StateInfo/Directory_Path.txt");
            if (file.open(QIODevice::WriteOnly | QIODevice::Text)){
                QTextStream out(&file);
                out << savePath;
            }
            file.close();
        }else{
            //Read the determined directory from a saved file, and then cd into it.
            QFile file(QDir::homePath()+"/StateInfo/Directory_Path.txt");
            file.open(QIODevice::ReadOnly | QIODevice::Text);
            QTextStream in(&file);
            savePath = in.readLine();
            file.close();

            //cd into savePath directory
            dir.cd(savePath);
        }

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
