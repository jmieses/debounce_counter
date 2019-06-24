#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>

namespace Ui {
class Dialog;
}

class Dialog : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog(QWidget *parent = nullptr);
    bool getFailPassedCheckBox();
    bool getSetFailCheckBox();
    double getExecutionRateSpinBox();
    double getOperationCycleSpinBox();
    double getTotalFailTimeSpingBox();
    ~Dialog();

private:
    Ui::Dialog *ui;
};

#endif // DIALOG_H
