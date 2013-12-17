#include "dialogcurvature.h"
#include "ui_dialogcurvature.h"

DialogCurvature::DialogCurvature(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogCurvature)
{
    ui->setupUi(this);
    connect(ui->cb_type, SIGNAL(currentIndexChanged(int)), this, SLOT(updateRender()));
    connect(ui->sb_min,  SIGNAL(valueChanged(double)),     this, SLOT(updateRender()));
    connect(ui->sb_max,  SIGNAL(valueChanged(double)),     this, SLOT(updateRender()));
}

DialogCurvature::~DialogCurvature()
{
    delete ui;
}

void DialogCurvature::updateRender() {
    ui->sb_max->setMinimum(ui->sb_min->value());
    ui->sb_min->setMaximum(ui->sb_max->value());
    emit parametersChanged();
}

double DialogCurvature::getMinValue() {
    return ui->sb_min->value();
}

double DialogCurvature::getMaxValue() {
    return ui->sb_max->value();
}

int DialogCurvature::getScalingType() {
    return ui->cb_type->currentIndex();
}

void DialogCurvature::setMinValue(double min, double current) {
    ui->sb_min->setMinimum(min);
    ui->sb_max->setMinimum(current);
    ui->sb_min->setValue(current);
}

void DialogCurvature::setMaxValue(double max, double current) {
    ui->sb_max->setMaximum(max);
    ui->sb_min->setMaximum(current);
    ui->sb_max->setValue(current);
}

void DialogCurvature::setScalingType(int type) {
    ui->cb_type->setCurrentIndex(type);
}
