#include "eulerplot.h"
#include "ui_eulerplot.h"
#include "globals.h"

EulerPlot::EulerPlot(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::EulerPlot)
{
    ui->setupUi(this);
    EulerPlot::MakePlot();
}

EulerPlot::~EulerPlot()
{
    delete ui;
}

//Initial Plot
void EulerPlot::MakePlot()
{
        QVector<double> t;
        QVector<double> phi,the,psi;
        QString file_name = "./outputs/" + QString::fromStdString(currentTime) + "angles.txt";
        QFile textFile(file_name);

        //Read Data From File
        if(textFile.open(QIODevice::ReadOnly))
        {
            int i = 0;
            QTextStream textStream(&textFile);
            while (!textStream.atEnd()) {
                QString line = textStream.readLine();
                if(textStream.status() == QTextStream::Ok){
                    QStringList lstLine = line.split(QRegExp("\\s+"));
                    phi.append(lstLine.at(0).toDouble());
                    the.append(lstLine.at(1).toDouble());
                    psi.append(lstLine.at(2).toDouble());
                    t.append(i);
                    i++;
                }
                else
                    break;
            }
        ui->customEulerPlot->addGraph();
        ui->customEulerPlot->graph(0)->setData(t,phi);
        ui->customEulerPlot->graph(0)->setPen(QPen(Qt::blue));
        ui->customEulerPlot->graph(0)->setName("pitch - " + QString::fromUtf8("\u03B8"));

        ui->customEulerPlot->addGraph();
        ui->customEulerPlot->graph(1)->setData(t,the);
        ui->customEulerPlot->graph(1)->setPen(QPen(Qt::red));
        ui->customEulerPlot->graph(1)->setName("roll - " + QString::fromUtf8("\u03C6"));

        ui->customEulerPlot->addGraph();
        ui->customEulerPlot->graph(2)->setData(t,psi);
        ui->customEulerPlot->graph(2)->setPen(QPen(Qt::green));
        ui->customEulerPlot->graph(2)->setName("yaw - " + QString::fromUtf8("\u03C8"));

        ui->customEulerPlot->xAxis->setLabel("Time (Sec)");
        ui->customEulerPlot->yAxis->setLabel("Angle (Degrees)");
        ui->customEulerPlot->rescaleAxes();
        ui->customEulerPlot->yAxis->setRange(-180, 180);

        ui->customEulerPlot->legend->setVisible(true);
        ui->customEulerPlot->legend->setFont(QFont("Helvetica", 9));

        ui->customEulerPlot->replot();
        }
}

//Save and Close
void EulerPlot::on_pushButton_clicked()
{
    QString file_name = "./outputs/" + QString::fromStdString(currentTime) + "angles.png";
    ui->customEulerPlot->savePng(file_name);
    this->close();
}
