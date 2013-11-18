#include "dialogcollapse.h"
#include "ui_dialogcollapse.h"

DialogCollapse::DialogCollapse(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogCollapse)
{
    ui->setupUi(this);
}

DialogCollapse::~DialogCollapse()
{
    delete ui;
}

int DialogCollapse::getNumIterations()
{
    return ui->sb_iters->value();
}

int DialogCollapse::getNumCollapses()
{
    return ui->sb_collapses->value();
}

double DialogCollapse::getThreshold()
{
    return 0.01*ui->sb_threshold->value();
}
