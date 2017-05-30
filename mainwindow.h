#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#define ORG_NAME "manoweb"
#define APP_NAME "spacexocr"

#include <QMainWindow>
#include <chartdialog.h>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	explicit MainWindow(QWidget *parent = 0);
	~MainWindow();

protected:
	void closeEvent(QCloseEvent *event);

private slots:
	void on_actionQuit_triggered();
	void on_actionOpen_triggered();
	void on_plainTextEdit_cursorPositionChanged();
	void on_actionSave_triggered();
	void on_textLineN_returnPressed();
	void on_plainTextEdit_blockCountChanged(int newBlockCount);
	void on_actionPrev_triggered();
	void on_actionNext_triggered();
	void on_actionDel_Next_triggered();
	void on_actionDel_Line_triggered();
	void on_actionNext_NN_triggered();
	void on_actionAutofix_triggered();
	void on_plainTextEdit_modificationChanged(bool changed);
	void on_actionExport_triggered();
	void on_actionShow_triggered();
	void on_actionRefresh_triggered();
	void on_plainTextEdit_textChanged();
	void clickedPoint(QPointF point);

private:
	bool checkAndSave();
	bool save();
	void readSettings();
	void writeSettings();
	void populateSeries();
	Ui::MainWindow *ui;
	QString directory;
	QString fileName;
	int currentLine = -1;
	ChartDialog* cd;
	QLineSeries seriesA;
	QLineSeries seriesB;
	bool loading;
};

#endif // MAINWINDOW_H
