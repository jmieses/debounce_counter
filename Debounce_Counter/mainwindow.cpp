#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "qdebug.h"

#define MAX_FAILED 127
#define MAX_PASSED -128

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    dialog = new Dialog(this);
    configurePlot();
    qDebug() << "This is to test the home branch" << "\n";
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::configurePlot()
{
    ui->widget->addGraph()->setName("Threshold Up");
    ui->widget->addGraph()->setName("Threshold Down");
    ui->widget->addGraph()->setName("Fault Detection Counter");
}

/****************************************************************
 * Function Name : realtimePlot
 * Description   : Displays the real time graph on the GUI
 * Returns       : None
 * Params        : None
 ****************************************************************/

void MainWindow::realtimePlot()
{

        static QTime time(QTime::currentTime());
        double key = time.elapsed()/1000.0;
        static double lastPointKey = 0;
        static double step_size = 0;
        //qDebug() << step_size << endl;
        if(key - lastPointKey > 0.002)
        {
            ui->widget->graph(2)->addData(key, step_size);
            lastPointKey = key;
        }

        /* make key axis range scroll right with the data at a constant range of 8. */
        ui->widget->graph(2)->rescaleValueAxis();
        ui->widget->xAxis->setRange(key, 8, Qt::AlignRight);
        ui->widget->replot();
        if(step_size > 127)
            step_size--;
        else if(step_size < -128)
            step_size++;
        else
            step_size++;
}

/****************************************************************
 * Function Name : counterPlot()
 * Description   : Displays the counter graph on the GUI
 * Returns       : None
 * Params        : None
 ****************************************************************/

void MainWindow::counterPlot()
{
    int i;
    int n_points = (ui->total_fail_time->value()/ui->execution_rate->value() + 1)* ui->fault_periods->value();
    QVector<double> x(2*n_points), fault_detection_counter(n_points), threshold_up(2*n_points), threshold_down(2*n_points);
    QPen pen;
    double x_axis = 0;
    double step_size = 50;
    double t_up = ui->threshold_up->value();
    double t_down = ui->threshold_down->value();


    pen.setWidth(3);
    pen.setColor(QColor(244, 185, 66));
    
    for(i=0; i< 2*n_points;++i){
        threshold_up[i] = t_up;
        threshold_down[i] = t_down;

        x[i] += x_axis;
        x_axis += step_size;

    }

    if(ui->set_dtc->checkState()){
       // setFailState(fault_detection_counter);
        setFailPassState(fault_detection_counter);
    }else if(dialog->getFailPassedCheckBox()){
       setFailPassState(fault_detection_counter);
    }else{
        setPassState(fault_detection_counter);
    }

    /* General configuration of the graph*/
    ui->widget->setInteractions(ui->widget->interactions() | QCP::iRangeDrag);
    ui->widget->setInteractions(ui->widget->interactions() | QCP::iRangeZoom);
    ui->widget->legend->setVisible(true);

    /* add the actual points on the graph by creating
     the addGraph(), which will prepare the graph for drawing. */

    /* configure and set threshold up graph */
    //ui->widget->addGraph()->setName("Threshold Up");
    ui->widget->graph(0)->setPen(pen);
    ui->widget->graph(0)->setData(x,threshold_up);
    //ui->widget->graph(0)->setName("Threshold Up");


    // ui->widget->xAxis->scaleRange(20,ui->widget->xAxis->range().center());

    /* configure and set threshold down graph */
    //ui->widget->addGraph()->setName("Threshold Down");
    pen.setColor(QColor(234, 4, 66));
    ui->widget->graph(1)->setPen(pen);
    ui->widget->graph(1)->setData(x, threshold_down);
    //ui->widget->graph(1)->setName("Threshold Down");

    /* set debounce counter graph */
    //ui->widget->addGraph()->setName("FDC");
    ui->widget->graph(2)->setData(x,fault_detection_counter);
    //ui->widget->graph(2)->setName("Fault Detection Counter");

    /* type of lines that will be draw. This is a step size line between points. */
    ui->widget->graph(2)->setLineStyle((QCPGraph::LineStyle)QCPGraph::lsStepLeft);
    //ui->widget->yAxis->scaleRange(60,ui->widget->yAxis->range().center());

    ui->widget->graph(2)->rescaleAxes(true);
    ui->widget->xAxis->setLabel("Time (ms)");
    ui->widget->yAxis->setLabel("Fault Detection Counter");
    ui->widget->replot();

    // connect(&timer_plot, SIGNAL(timeout()),this,SLOT(realtimePlot()));
   // timer_plot.start(0);
}

/**************************************************************************
 * Function Name : setFailState()
 * Description   : Moves the counter by incrementing towards fail state
 * Returns       : None
 * Params        : QVector<double>&
 *
 **************************************************************************/

void MainWindow::setFailState(QVector<double>& fdc)
{
    int n = fdc.size();
    int i;
    double val = 0;
    double count_up = ui->count_up->value();
    if(ui->jump_up->checkState())
        val = ui->jump_up_value->value();

    if(n > 0){
        fdc[0] = 0;
        for(i = 1 ; i < n; ++i){
            fdc[i] = val;
            val += count_up;

            if(fdc[i] >= MAX_FAILED){
                fdc[i] = MAX_FAILED;
            }
        }
    }
}

/**************************************************************************
 * Function Name : setPassedState()
 * Description   : Moves the counter by decrementing towards passed state
 * Returns       : None
 * Params        : QVector<double>&
 **************************************************************************/

void MainWindow::setPassState(QVector<double>& fdc)
{
    /* In contrast to setFailState() use a separete variable val
     * to fill data of vector since -128 < fdc[i] < jump_down_value
    */
    int n = fdc.size() + 1;
    fdc.resize(n);
    int i;

    double val = ui->jump_down_value->value();
    double count_down = ui->count_down->value();
    if(n > 0){
        fdc[0] = 0;
        for(i = 1; i < n; ++i){
            fdc[i] = val;
            val += count_down;

            if(fdc[i] <= MAX_PASSED){
                fdc[i] = MAX_PASSED;
            }
        }
    }
}

/**************************************************************************
 * Function Name : setFailPassState()
 * Description   : Toggle the counter between Failed and Passed states.
 * Returns       : None
 * Params        : QVector<double>&
 **************************************************************************/

void MainWindow::setFailPassState(QVector<double>& fdc)
{
    int i;
    int n = fdc.size();
    double val = 0;
    double last_val = 0;
    bool toggle = true;
      fdc[0] = val;
    for(i=1; i< n;++i){
        if(ui->jump_up->checkState() && (MAX_FAILED - val) > (MAX_FAILED - ui->jump_up_value->value()) && last_val <= val)
            val = (double)(ui->jump_up_value->value()) - 1.;
        if(ui->jump_down->checkState() && (MAX_FAILED - val) < (MAX_FAILED - ui->jump_up_value->value()) && last_val >= val)
            val = (double)(ui->jump_down_value->value()) + 1.;
        last_val = val;
        fdc[i] = val;
        if(val >= MAX_FAILED ||  val <= MAX_PASSED)
            toggle = !toggle;
        if(toggle)
            val+= (double)(ui->count_up->value());
        else
            val+= (double)(ui->count_down->value());

    }
}

void MainWindow::showSecondWindow()
{

}

/**************************************************************************
 * Function Name : on_pushButton_clicked()
 * Description   : Connects submit button to action
 * Returns       : None
 * Params        : QVector<double>&
 **************************************************************************/
void MainWindow::on_pushButton_clicked()
{
    counterPlot();

}

void MainWindow::on_actionOptions_triggered()
{
    dialog->show();
}
