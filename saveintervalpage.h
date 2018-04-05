#ifndef SAVEINTERVALPAGE_H
#define SAVEINTERVALPAGE_H

#include <QDialog>
#include <QLineEdit>
#include <QFileDialog>
#include <QTime>
#include <QFile>
#include <QTextStream>

namespace Ui {
class saveIntervalPage;
}

class saveIntervalPage : public QDialog
{
    Q_OBJECT

public:
    explicit saveIntervalPage(QWidget *parent = 0);
    ~saveIntervalPage();
    void saveInterval();
    void setSaveString(QString saveString);

private slots:
    void on_saveButton_clicked();

    void on_cancelButton_clicked();

private:
    Ui::saveIntervalPage *ui;
    QLineEdit *intervalName;
    QString saveString;
};

#endif // SAVEINTERVALPAGE_H
