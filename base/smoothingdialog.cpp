#include "smoothingdialog.h"
#include "ui_smoothingdialog.h"

SmoothingDialog::SmoothingDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SmoothingDialog)
{
    ui->setupUi(this);
}

SmoothingDialog::~SmoothingDialog()
{
    delete ui;
}

int SmoothingDialog::getNumIterations()
{
    return ui->sb_iters->value();
}

double SmoothingDialog::getLambda()
{
    return ui->sb_lambda->value();
}
