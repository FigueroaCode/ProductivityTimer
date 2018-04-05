#ifndef SAVEINTERVALPAGE_H
#define SAVEINTERVALPAGE_H

#include <QDialog>

namespace Ui {
class saveIntervalPage;
}

class saveIntervalPage : public QDialog
{
    Q_OBJECT

public:
    explicit saveIntervalPage(QWidget *parent = 0);
    ~saveIntervalPage();

private:
    Ui::saveIntervalPage *ui;
};

#endif // SAVEINTERVALPAGE_H
