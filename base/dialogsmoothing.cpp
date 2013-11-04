#include "dialogsmoothing.h"
#include "ui_dialogsmoothing.h"

DialogSmoothing::DialogSmoothing(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogSmoothing)
{
    ui->setupUi(this);
}

DialogSmoothing::~DialogSmoothing()
{
    delete ui;
}

int DialogSmoothing::getNumIterations()
{
    return ui->sb_iters->value();
}

double DialogSmoothing::getLambda()
{
    return ui->sb_lambda->value();
}
