#include <QMainWindow>
#include "qcustomplot.h"
#include "eulerplot.h"
#include "anglecorrection.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void SavePlot();
    void UpdatePlot(double x, double y);

    void AngleCorrectionUpdate(double deltaX, double deltaY, double predX, double predY);
private slots:
    void MakePlot();
    void on_close_button_clicked();

    void on_euler_button_clicked();

    void on_optical_button_clicked();

    void on_clear_button_clicked();

    void on_pushButton_clicked();

private:
    QCPCurve *newCurve;
    EulerPlot *ePlot;
    AngleCorrection *angleCorrectionPlot;
    Ui::MainWindow *ui;
};
