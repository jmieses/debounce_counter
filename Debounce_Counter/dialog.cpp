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
    return ui->fail_pass_checkBox->checkState();
}

Dialog::~Dialog()
{
    delete ui;
}
