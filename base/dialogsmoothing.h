#ifndef SMOOTHINGDIALOG_H
#define SMOOTHINGDIALOG_H

#include <QDialog>

namespace Ui {
class DialogSmoothing;
}

class DialogSmoothing : public QDialog
{
    Q_OBJECT

public:
    explicit DialogSmoothing(QWidget *parent = 0);
    ~DialogSmoothing();

    int getNumIterations();
    double getLambda();

private:
    Ui::DialogSmoothing *ui;
};


#endif // SMOOTHINGDIALOG_H
