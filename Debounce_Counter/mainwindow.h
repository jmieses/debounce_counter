#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFile>
#include "qcustomplot.h"


#include "dialog.h"
#include "ui_dialog.h"


namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

public slots:
    void configurePlot();
    void realtimePlot();
    void counterPlot();
    void setFailState(QVector<double>&);
    void setPassState(QVector<double>&);
    void setFailPassState(QVector<double>&);
    void showSecondWindow();

private slots:
    void on_pushButton_clicked();

    void on_actionOptions_triggered();

private:
    Ui::MainWindow *ui;
    QTimer *timer_plot;
    QSpinBox *periods;
    Dialog* dialog;
};

#endif // MAINWINDOW_H
