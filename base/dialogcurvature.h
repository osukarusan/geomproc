#ifndef DIALOGCURVATURE_H
#define DIALOGCURVATURE_H

#include <QDialog>

namespace Ui {
class DialogCurvature;
}

class DialogCurvature : public QDialog
{
    Q_OBJECT

public:
    explicit DialogCurvature(QWidget *parent = 0);
    ~DialogCurvature();

    double getMinValue();
    double getMaxValue();
    int    getScalingType();

    void   setMinValue(double min, double current);
    void   setMaxValue(double max, double current);
    void   setScalingType(int type);

protected slots:
    void   updateRender();

signals:
    void   parametersChanged();

private:
    Ui::DialogCurvature *ui;
};


#endif // DIALOGCURVATURE_H
