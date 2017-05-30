#ifndef CHARTDIALOG_H
#define CHARTDIALOG_H

#include <QDialog>
#include <QtCharts>
using namespace QtCharts;

namespace Ui {
class ChartDialog;
}

class ChartDialog : public QDialog
{
	Q_OBJECT

public:
	explicit ChartDialog(QWidget *parent = 0);
	~ChartDialog();
	void showChart(QLineSeries *seriesA, QLineSeries *seriesB);

private:
	Ui::ChartDialog *ui;
	QChartView* chartView;
	QChart* chart;
};

#endif // CHARTDIALOG_H
