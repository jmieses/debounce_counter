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

   // QCPTextElement *title = new QCPTextElement(ui->widget, "Power vs. Wind Speed", QFont("sans", 17, QFont::Bold));
   // ui->widget->plotLayout()->addElement(0,0,title);
   // counterPlot();
   //timer_plot = new QTimer(this);
   //connect(timer_plot, SIGNAL(timeout()), this, SLOT(counterPlot()));
   //auto g = ui->layoutWidget->setLayout(periods));
   //qDebug() << "g: " <<  g << endl;

}

MainWindow::~MainWindow()
{
    delete ui;
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
    qDebug() << "n_points: " << n_points << endl;
    QVector<double> x(n_points), fault_detection_counter(n_points), threshold_up(n_points), threshold_down(n_points);
    // QCPTextElement *title = new QCPTextElement(ui->widget, "Power vs. Wind Speed", QFont("sans", 17, QFont::Bold));
    QPen pen;
    bool toggle = true;
    double val = 0;
    double last_val = 0;
    double x_axis = 0;
    double step_size = 50;
    double t_up = ui->threshold_up->value();
    double t_down = ui->threshold_down->value();
    // set parameters
    pen.setWidth(3);
    pen.setColor(QColor(244, 185, 66));
    fault_detection_counter[0] = 0;
    for(i=0; i< n_points;++i){
        threshold_up[i] = t_up;
        threshold_down[i] = t_down;
        x[i] += x_axis;
        x_axis += step_size;
        fault_detection_counter[i] = val;

        if(ui->jump_up->checkState() && (MAX_FAILED - val) > (MAX_FAILED - ui->jump_up_value->value()) && last_val <= val)
            val = (double)ui->jump_up_value->value() - 1;
        if(ui->jump_down->checkState() && (MAX_FAILED - val) < (MAX_FAILED - ui->jump_up_value->value()) && last_val >= val)
            val = (-1)*((double)ui->jump_up_value->value() - 1);
        last_val = val;
        if(val >= MAX_FAILED ||  val <= MAX_PASSED)
            toggle = !toggle;
        if(toggle)
            val+= (double)ui->count_up->value();
        else
            val-= (double)ui->count_down->value();
    }

    if(ui->set_dtc->checkState()){
        setDTC(fault_detection_counter);
    }
    //ui->widget->plotLayout()->addElement(0,0,title);
    ui->widget->setInteractions(ui->widget->interactions() | QCP::iRangeDrag);
    ui->widget->setInteractions(ui->widget->interactions() | QCP::iRangeZoom);

    // add the actual points on the graph by creating
    // the addGraph(), which will prepare the graph for drawing.

    // set threshold up graph
    ui->widget->addGraph();
    ui->widget->graph(0)->setPen(pen);
    ui->widget->graph(0)->setData(x,threshold_up);


    // ui->widget->xAxis->scaleRange(20,ui->widget->xAxis->range().center());

    /* set threshold down graph */
    ui->widget->addGraph();
    ui->widget->graph(1)->setPen(pen);
    ui->widget->graph(1)->setData(x, threshold_down);
    ui->widget->replot();

    /* set debounce counter graph */
    ui->widget->addGraph();
    ui->widget->graph(2)->setData(x,fault_detection_counter);

    /* type of lines that will be draw. This is a step size line between points. */
    ui->widget->graph(2)->setLineStyle((QCPGraph::LineStyle)QCPGraph::lsStepLeft);
    //ui->widget->yAxis->scaleRange(60,ui->widget->yAxis->range().center());

    ui->widget->graph(2)->rescaleAxes(true);
    qDebug() << ui->widget->graph(1)->dataCount() << endl;
    ui->widget->xAxis->setLabel("Time (ms)");
    ui->widget->yAxis->setLabel("Fault Detection Counter");
    ui->widget->replot();
   qDebug() << threshold_down.last() << endl;
    // connect(&timer_plot, SIGNAL(timeout()),this,SLOT(realtimePlot()));
   // timer_plot.start(0);
}

void MainWindow::setDTC(QVector<double>& dtc)
{
    int n = dtc.size();
    int i;
    if(n > 0){
        for(i = 0; i < n; ++i){
            dtc[i] = i;
        }
    }
}

void MainWindow::on_pushButton_clicked()
{
    counterPlot();
}
