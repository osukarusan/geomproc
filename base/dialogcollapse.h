#ifndef DIALOGCOLLAPSE_H
#define DIALOGCOLLAPSE_H

#include <QDialog>

namespace Ui {
class DialogCollapse;
}

class DialogCollapse : public QDialog
{
    Q_OBJECT

public:
    explicit DialogCollapse(QWidget *parent = 0);
    ~DialogCollapse();

    int getNumIterations();
    int getNumCollapses();
    double getThreshold();

private:
    Ui::DialogCollapse *ui;
};

#endif // DIALOGCOLLAPSE_H
