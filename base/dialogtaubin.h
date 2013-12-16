#ifndef DIALOGTAUBIN_H
#define DIALOGTAUBIN_H

#include <QDialog>

namespace Ui {
class DialogTaubin;
}

class DialogTaubin : public QDialog
{
    Q_OBJECT

public:
    explicit DialogTaubin(QWidget *parent = 0);
    ~DialogTaubin();

    int getNumIterations();
    double getLambda();
    double getMu();

protected slots:
    void on_cb_auto_toggled(bool);
    void on_sb_lambda_valueChanged(double);

private:
    Ui::DialogTaubin *ui;
};

#endif // DIALOGTAUBIN_H
