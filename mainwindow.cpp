#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QtWidgets>

MainWindow::MainWindow(QWidget *parent) :
	QMainWindow(parent),
	ui(new Ui::MainWindow)
{
	ui->setupUi(this);
}

MainWindow::~MainWindow()
{
	delete ui;
}

void MainWindow::on_actionQuit_triggered()
{
	QApplication::quit();
}

void MainWindow::on_actionOpen_triggered()
{
	QString openFileName = QFileDialog::getOpenFileName(this, tr("Open File"), "/root/spacex/SES10/img/proc");
	if(openFileName.isNull())
		return;
	QFileInfo fileInfo(openFileName);
	fileName = fileInfo.absoluteFilePath();
	directory = fileInfo.absolutePath();
	QFile file(openFileName);
	file.open(QFile::ReadOnly | QFile::Text);

	QTextStream ReadFile(&file);
	ui->plainTextEdit->setPlainText(ReadFile.readAll());
	file.close();
}

void MainWindow::on_plainTextEdit_cursorPositionChanged()
{
	int line = ui->plainTextEdit->textCursor().blockNumber() + 1;
	if(line == currentLine)
		return;
	currentLine = line;
	ui->textLineN->setText(QString::number(currentLine));

	QString frameName = QString("%1/frame%2.png").arg(directory).arg(currentLine, 5, 10, QChar('0'));
	QImageReader reader(frameName);
	const QImage image = reader.read();
	if(image.isNull())
	{
		ui->label->setPixmap(QPixmap());
		return;
	}
	ui->label->setPixmap(QPixmap::fromImage(image));
}

void MainWindow::on_actionSave_triggered()
{
	QFile file(fileName);
	if(file.open(QIODevice::WriteOnly))
	{
		QTextStream stream(&file);
		stream << ui->plainTextEdit->toPlainText();
		file.flush();
		file.close();
	}
	else
		QMessageBox::critical(this, tr("Error"), tr("Error during save"));
}

void MainWindow::on_btNext_clicked()
{
	ui->plainTextEdit->find("_");
	ui->plainTextEdit->setFocus();
}

void MainWindow::on_btPrev_clicked()
{
	ui->plainTextEdit->find("_", QTextDocument::FindBackward);
	ui->plainTextEdit->setFocus();
}

void MainWindow::on_btDelNext_clicked()
{
	ui->plainTextEdit->textCursor().removeSelectedText();
	on_btNext_clicked();
}

void MainWindow::on_btDelLine_clicked()
{
	QTextCursor cursor = ui->plainTextEdit->textCursor();
	cursor.select(QTextCursor::BlockUnderCursor);
	cursor.removeSelectedText();
	ui->plainTextEdit->moveCursor(QTextCursor::Down);
	ui->plainTextEdit->setFocus();
}

void MainWindow::on_textLineN_returnPressed()
{
	int line = ui->textLineN->text().toInt();
	if(line > 0 && line < ui->plainTextEdit->blockCount())
	{
		QTextCursor cursor(ui->plainTextEdit->document()->findBlockByLineNumber(line-1));
		ui->plainTextEdit->setTextCursor(cursor);
		ui->plainTextEdit->setFocus();
	}
	else
		ui->textLineN->setText(QString::number(currentLine));
}

void MainWindow::on_btNextNN_clicked()
{
	double oldA = -1;
	double oldB = -1;
	double a, b;
	for(int i = ui->plainTextEdit->textCursor().blockNumber(); i < ui->plainTextEdit->blockCount(); i++)
	{
		QString line = ui->plainTextEdit->document()->findBlockByLineNumber(i).text();
		QStringList list = line.split(QChar(' '), QString::SkipEmptyParts);
		if(list.count() != 2)
		{
			QTextCursor cursor(ui->plainTextEdit->document()->findBlockByLineNumber(i));
			ui->plainTextEdit->setTextCursor(cursor);
			ui->plainTextEdit->setFocus();
			break;
		}
		a = list.at(0).toDouble();
		b = list.at(1).toDouble();
		qDebug() << a << b;
		if(a < oldA || b < oldB)
		{
			QTextCursor cursor(ui->plainTextEdit->document()->findBlockByLineNumber(i));
			ui->plainTextEdit->setTextCursor(cursor);
			ui->plainTextEdit->setFocus();
			break;
		}
		oldA = a;
		oldB = b;
	}
}

void MainWindow::on_btAutoFix_clicked()
{
	for(int i = 0; i < ui->plainTextEdit->textCursor().blockNumber(); i++)
	{
		QString line = ui->plainTextEdit->document()->findBlockByLineNumber(i).text();
		QStringList list = line.split(" ", QString::SkipEmptyParts);
	}
}

void MainWindow::on_plainTextEdit_blockCountChanged(int newBlockCount)
{
	ui->labelTotFrames->setText(QString("/ %1").arg(newBlockCount));
}
