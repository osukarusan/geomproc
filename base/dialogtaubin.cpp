#include "dialogtaubin.h"
#include "ui_dialogtaubin.h"

DialogTaubin::DialogTaubin(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogTaubin)
{
    ui->setupUi(this);
}

DialogTaubin::~DialogTaubin()
{
    delete ui;
}

int DialogTaubin::getNumIterations()
{
    return ui->sb_iters->value();
}

double DialogTaubin::getLambda()
{
    return ui->sb_lambda->value();
}

double DialogTaubin::getMu()
{
    return ui->sb_mu->value();
}

double computeMu(double lam) {
    return lam/(0.1*lam - 1);
}

void DialogTaubin::on_cb_auto_toggled(bool a) {
    ui->sb_mu->setEnabled(!a);
    if (a)
        ui->sb_mu->setValue(computeMu(ui->sb_lambda->value()));
}

void DialogTaubin::on_sb_lambda_valueChanged(double lam) {
    if (ui->cb_auto->isChecked())
        ui->sb_mu->setValue(computeMu(lam));
}
