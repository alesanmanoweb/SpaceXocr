#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QtWidgets>

MainWindow::MainWindow(QWidget *parent) :
	QMainWindow(parent),
	ui(new Ui::MainWindow)
{
	ui->setupUi(this);
	setWindowTitle(QString("New file[*] - SpaceX OCR"));
}

MainWindow::~MainWindow()
{
	delete ui;
}

void MainWindow::on_actionQuit_triggered()
{
	close();
}

void MainWindow::closeEvent(QCloseEvent *event)
{
	if(checkAndSave())
	{
		// write settings
		event->accept();
	}
	else
		event->ignore();
}

bool MainWindow::checkAndSave()
{
	if(!ui->plainTextEdit->document()->isModified())
		return true;
	const QMessageBox::StandardButton ret = QMessageBox::warning(this, tr("Application"), tr("The document has been modified.\nDo you want to save your changes?"), QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);
	switch(ret)
	{
		case QMessageBox::Save:
			return save();
		case QMessageBox::Cancel:
			return false;
		default:
			break;
	}
	return true;
}

bool MainWindow::save()
{
	if(fileName.isNull())
	{
		fileName = QFileDialog::getSaveFileName(this, tr("Save File"));
		if(fileName.isNull())
			return false;
	}
	QFile file(fileName);
	if(file.open(QIODevice::WriteOnly | QFile::Text))
	{
		QTextStream stream(&file);
		stream << ui->plainTextEdit->toPlainText();
		file.flush();
		file.close();
		setWindowModified(false);
		ui->plainTextEdit->document()->setModified(false);
		return true;
	}
	QMessageBox::critical(this, tr("Error"), tr("Error during save"));
	return false;
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
	setWindowTitle(QString("%1[*] - SpaceX OCR").arg(fileName));
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
	save();
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

void MainWindow::on_plainTextEdit_blockCountChanged(int newBlockCount)
{
	ui->labelTotFrames->setText(QString("/ %1").arg(newBlockCount));
}

void MainWindow::on_actionPrev_triggered()
{
	ui->plainTextEdit->find("_", QTextDocument::FindBackward);
	ui->plainTextEdit->setFocus();

}

void MainWindow::on_actionNext_triggered()
{
	ui->plainTextEdit->find("_");
	ui->plainTextEdit->setFocus();
}

void MainWindow::on_actionDel_Next_triggered()
{
	ui->plainTextEdit->textCursor().removeSelectedText();
	on_actionNext_triggered();
}

void MainWindow::on_actionDel_Line_triggered()
{
	QTextCursor cursor = ui->plainTextEdit->textCursor();
	cursor.select(QTextCursor::BlockUnderCursor);
	cursor.removeSelectedText();
	ui->plainTextEdit->moveCursor(QTextCursor::Down);
	ui->plainTextEdit->setFocus();
}

void MainWindow::on_actionNext_NN_triggered()
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

void MainWindow::on_actionAutofix_triggered()
{
	for(int i = 0; i < ui->plainTextEdit->textCursor().blockNumber(); i++)
	{
		QString line = ui->plainTextEdit->document()->findBlockByLineNumber(i).text();
		QStringList list = line.split(" ", QString::SkipEmptyParts);
	}
}

void MainWindow::on_plainTextEdit_modificationChanged(bool changed)
{
	setWindowModified(changed);
}
