#ifndef EULERPLOT_H
#define EULERPLOT_H

#include <QDialog>

namespace Ui {
class EulerPlot;
}

class EulerPlot : public QDialog
{
    Q_OBJECT

public:
    explicit EulerPlot(QWidget *parent = 0);
    ~EulerPlot();

protected slots:
    void MakePlot();
private slots:
    void on_pushButton_clicked();

private:
    Ui::EulerPlot *ui;
};

#endif // EULERPLOT_H
