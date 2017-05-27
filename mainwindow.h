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

private:
	bool checkAndSave();
	bool save();
	Ui::MainWindow *ui;
	QString directory;
	QString fileName;
	int currentLine = -1;
};

#endif // MAINWINDOW_H
