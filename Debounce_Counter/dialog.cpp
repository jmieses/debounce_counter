#include "dialog.h"
#include "ui_dialog.h"

Dialog::Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog)
{
    ui->setupUi(this);
}

bool Dialog::getFailPassedCheckBox()
{
    return ui->Dialog_fail_pass_checkBox->checkState();
}

bool Dialog::getSetFailCheckBox()
{
    return ui->Dialog_set_fail_checkBox->checkState();
}

double Dialog::getExecutionRateSpinBox()
{
    return ui->Dialog_executio_rate_spinBox->value();
}

double Dialog::getOperationCycleSpinBox()
{
    return ui->Dialog_operation_cycle_spinBox->value();
}

double Dialog::getTotalFailTimeSpingBox()
{
    return ui->Dialog_total_fail_time_spinBox->value();
}

Dialog::~Dialog()
{
    delete ui;
}
