#include "chartdialog.h"
#include "ui_chartdialog.h"

ChartDialog::ChartDialog(QWidget *parent) :
	QDialog(parent),
	ui(new Ui::ChartDialog)
{
	ui->setupUi(this);
}

ChartDialog::~ChartDialog()
{
	delete ui;
}

void ChartDialog::showChart(QLineSeries* seriesA, QLineSeries *seriesB)
{
	chart = new QChart();
	chart->legend()->hide();

	QValueAxis *axisX = new QValueAxis;
	axisX->setTickCount(10);
	chart->addAxis(axisX, Qt::AlignBottom);

	chart->addSeries(seriesA);
	QValueAxis *axisYA = new QValueAxis;
	axisYA->setLinePenColor(seriesA->pen().color());
	chart->addAxis(axisYA, Qt::AlignLeft);
	seriesA->attachAxis(axisX);
	seriesA->attachAxis(axisYA);

	chart->addSeries(seriesB);
	QValueAxis *axisYB = new QValueAxis;
	axisYB->setLinePenColor(seriesB->pen().color());
	chart->addAxis(axisYB, Qt::AlignRight);
	seriesB->attachAxis(axisX);
	seriesB->attachAxis(axisYB);

	chartView = new QChartView(chart);
	chartView->setRenderHint(QPainter::Antialiasing);
	layout()->addWidget(chartView);
}
