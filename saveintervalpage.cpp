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
        QString dirName = "Productivity_Intervals";

        //Check if a save directory has already been established.
        if(!dir.exists("StateInfo"))
        {
            QString filepath = QFileDialog::getExistingDirectory(
                        this,
                        "Select Directory",
                        QDir::homePath());

            //make folder to save Intervals in directory chosen
            QDir newDir(filepath);
            if(!newDir.exists())
            {
                newDir.mkpath(".");
            }else{
                //Make Folder
                if(!newDir.exists(dirName)){
                    newDir.mkdir(dirName);
                }
            }

            //New dir for creating files will be savePath
            savePath = filepath + "/" + dirName;
            //make state info dir to store file
            dir.mkdir("StateInfo");

            //Save this directory for future use.
            QFile file(QDir::homePath()+"/StateInfo/Directory_Path.txt");//will make the file if it doesn't exist
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
        //In case user deletes project folder, make a new one
        QDir projectPath(savePath);
        if(!projectPath.exists())
        {
            projectPath.mkdir(savePath);
        }
        //Write a new file that stores the interval as text
        QFile file(savePath + "/" + intervalName->text() + ".txt");
        if( file.open(QIODevice::WriteOnly | QIODevice::Text)){
            QTextStream out(&file);
            out << saveString;
        }

        file.close();

        this->close();
    }else{
        QMessageBox::information(this,tr("Error"),tr("Please enter a name for this interval"));
    }

}

void saveIntervalPage::on_cancelButton_clicked()
{
    this->close();
}

void saveIntervalPage::setSaveString(QString saveString)
{
    this->saveString = saveString;
}
