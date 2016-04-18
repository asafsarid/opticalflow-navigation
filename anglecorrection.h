#ifndef ANGLECORRECTION_H
#define ANGLECORRECTION_H

#include <QDialog>
#include <QPen>

namespace Ui {
class AngleCorrection;
}

class AngleCorrection : public QDialog
{
    Q_OBJECT

public:
    explicit AngleCorrection(QWidget *parent = 0);
    ~AngleCorrection();

    void UpdatePlot(double deltaX, double deltaY, double predX, double predY);
private slots:
    void on_pushButton_clicked();

    void MakePlot();
private:
    Ui::AngleCorrection *ui;
    int time;
};

#endif // ANGLECORRECTION_H
