#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	explicit MainWindow(QWidget *parent = 0);
	~MainWindow();

private slots:
	void on_actionQuit_triggered();

	void on_actionOpen_triggered();

	void on_plainTextEdit_cursorPositionChanged();

	void on_actionSave_triggered();

	void on_btNext_clicked();

	void on_btPrev_clicked();

	void on_btDelNext_clicked();

	void on_btDelLine_clicked();

	void on_textLineN_returnPressed();

	void on_btNextNN_clicked();

	void on_btAutoFix_clicked();

	void on_plainTextEdit_blockCountChanged(int newBlockCount);

private:
	Ui::MainWindow *ui;
	QString directory;
	QString fileName;
	int currentLine = -1;
	int totLines = -1;
};

#endif // MAINWINDOW_H
