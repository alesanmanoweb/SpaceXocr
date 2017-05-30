#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QtWidgets>
#include <QRegularExpression>

MainWindow::MainWindow(QWidget *parent) :
	QMainWindow(parent),
	ui(new Ui::MainWindow)
{
	ui->setupUi(this);
	setWindowTitle(QString("New file[*] - SpaceX OCR"));
	readSettings();
	ui->statusBar->showMessage("Ready");
	loading = false;
	connect(&seriesA, SIGNAL(clicked(QPointF)), this, SLOT(clickedPoint(QPointF)));
	connect(&seriesB, SIGNAL(clicked(QPointF)), this, SLOT(clickedPoint(QPointF)));
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
		writeSettings();
		event->accept();
		QApplication::quit();
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

void MainWindow::readSettings()
{
	QSettings settings(QCoreApplication::organizationName(), QCoreApplication::applicationName());
	directory = settings.value("directory").toString();
}

void MainWindow::writeSettings()
{
	QSettings settings(QCoreApplication::organizationName(), QCoreApplication::applicationName());
	if(!directory.isNull())
		settings.setValue("directory", directory);
}

void MainWindow::on_actionOpen_triggered()
{
	if(checkAndSave())
	{
		QString openFileName = QFileDialog::getOpenFileName(this, tr("Open File"), directory);
		if(openFileName.isNull())
			return;
		QFileInfo fileInfo(openFileName);
		fileName = fileInfo.absoluteFilePath();
		directory = fileInfo.absolutePath();
		QFile file(openFileName);
		file.open(QFile::ReadOnly | QFile::Text);

		QTextStream ReadFile(&file);
		loading = true;
		ui->plainTextEdit->setPlainText(ReadFile.readAll());
		loading = false;
		file.close();
		setWindowTitle(QString("%1[*] - SpaceX OCR").arg(fileName));
		ui->statusBar->showMessage("File opened");
	}
}

void MainWindow::on_plainTextEdit_modificationChanged(bool changed)
{
	setWindowModified(changed);
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
	if(save())
		ui->statusBar->showMessage("File saved");
	else
		ui->statusBar->showMessage("File not saved");
}

void MainWindow::on_textLineN_returnPressed()
{
	int line = ui->textLineN->text().toInt();
	if(line > 0 && line < ui->plainTextEdit->blockCount())
	{
		QTextCursor cursor(ui->plainTextEdit->document()->findBlockByLineNumber(line - 1));
		ui->plainTextEdit->setTextCursor(cursor);
		ui->plainTextEdit->setFocus();
		ui->statusBar->showMessage("Ready");
	}
	else
	{
		ui->textLineN->setText(QString::number(currentLine));
		ui->statusBar->showMessage("Requested line does not exist");
	}
}

void MainWindow::on_plainTextEdit_blockCountChanged(int newBlockCount)
{
	ui->labelTotFrames->setText(QString("/ %1").arg(newBlockCount));
	//ui->statusBar->showMessage("Ready");
}

void MainWindow::on_actionPrev_triggered()
{
	bool res = ui->plainTextEdit->find("_", QTextDocument::FindBackward);
	ui->plainTextEdit->setFocus();
	if(!res)
		ui->statusBar->showMessage("Previous _ instance not found");
	else
		ui->statusBar->showMessage("Ready");
}

void MainWindow::on_actionNext_triggered()
{
	bool res = ui->plainTextEdit->find("_");
	ui->plainTextEdit->setFocus();
	if(!res)
		ui->statusBar->showMessage("Next _ instance not found");
	else
		ui->statusBar->showMessage("Ready");
}

void MainWindow::on_actionDel_Next_triggered()
{
	ui->plainTextEdit->textCursor().removeSelectedText();
	on_actionNext_triggered();
}

void MainWindow::on_actionDel_Line_triggered()
{
	loading = true;
	QTextCursor cursor = ui->plainTextEdit->textCursor();
	cursor.select(QTextCursor::BlockUnderCursor);
	cursor.removeSelectedText();
	ui->plainTextEdit->moveCursor(QTextCursor::Down);
	ui->plainTextEdit->setFocus();
	qDebug() << "startMod";
	populateSeries();
	qDebug() << "stopMod";
	ui->statusBar->showMessage("Ready");
	loading = false;
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
	int count = 0;
	QRegularExpression re("^(\?! *\\d+ +\\d+(\?:\\.\\d+)?$).*");
	for(int i = 0; i < ui->plainTextEdit->blockCount(); i++)
	{
		QString line = ui->plainTextEdit->document()->findBlockByLineNumber(i).text();
		QRegularExpressionMatch match = re.match(line);
		if(match.hasMatch())
		{
			QStringList list = line.split(QChar(' '), QString::SkipEmptyParts);
			if(list.count() == 3)
			{
				count++;
				QString newLine = QString("%1    %2.%3").arg(list.at(0)).arg(list.at(1)).arg(list.at(2));
				QTextCursor cursor(ui->plainTextEdit->document()->findBlockByLineNumber(i));
				cursor.select(QTextCursor::LineUnderCursor);
				cursor.insertText(newLine);
				ui->plainTextEdit->setTextCursor(cursor);
				ui->plainTextEdit->setFocus();
				ui->statusBar->showMessage("Ready");
			}
		}
	}
	if(count > 0)
		ui->statusBar->showMessage(QString("%1 matches auto-fixed").arg(count));
	else
		ui->statusBar->showMessage("No match found");
}

void MainWindow::on_actionExport_triggered()
{
	QString exportFileName = QFileDialog::getSaveFileName(this, tr("Export to..."));
	if(exportFileName.isNull())
		return;
	QFile file(exportFileName);
	if(file.open(QIODevice::WriteOnly | QFile::Text))
	{
		QTextStream stream(&file);
		double a, b;
		double time = 0;
		const double timegap = 1 / 29.97;
		for(int i = 0; i < ui->plainTextEdit->blockCount(); i++)
		{
			QString line = ui->plainTextEdit->document()->findBlockByLineNumber(i).text();
			QStringList list = line.split(QChar(' '), QString::SkipEmptyParts);
			if(list.count() != 2)
			{
				// Message window
				break;
				QTextCursor cursor(ui->plainTextEdit->document()->findBlockByLineNumber(i));
				ui->plainTextEdit->setTextCursor(cursor);
				ui->plainTextEdit->setFocus();
			}
			a = list.at(0).toDouble();
			b = list.at(1).toDouble();
			stream << time << ',' << a << ',' << b << '\n';
			time += timegap;
		}
		file.flush();
		file.close();
		return;
	}
	QMessageBox::critical(this, tr("Error"), tr("Error during export"));
}

void MainWindow::populateSeries()
{
	seriesA.clear();
	seriesB.clear();
	double a, b;
	double time = 0;
	const double timegap = 1 / 29.97;
	for(int i = 0; i < ui->plainTextEdit->blockCount(); i++)
	{
		QString line = ui->plainTextEdit->document()->findBlockByLineNumber(i).text();
		QStringList list = line.split(QChar(' '), QString::SkipEmptyParts);
		if(list.count() != 2) // non-nominal line; we set value to 0 so it's easy to spot it on the chart
		{
			a = 0;
			b = 0;
		}
		else
		{
			a = list.at(0).toDouble();
			b = list.at(1).toDouble();
		}
		seriesA.append(time, a);
		seriesB.append(time, b);
		time += timegap;
	}
}

void MainWindow::on_actionShow_triggered()
{
	populateSeries();
	cd = new ChartDialog();
	cd->show();
	cd->showChart(&seriesA, &seriesB);
}

void MainWindow::on_actionRefresh_triggered()
{
	qDebug() << "startMod";
	populateSeries();
	qDebug() << "stopMod";
}

void MainWindow::on_plainTextEdit_textChanged()
{
	int index = currentLine - 1;
	if(loading)
		return;
	double a, b;
	double time;
	QString line = ui->plainTextEdit->document()->findBlockByLineNumber(index).text();
	QStringList list = line.split(QChar(' '), QString::SkipEmptyParts);
	if(list.count() != 2) // non-nominal line; we set value to 0 so it's easy to spot it on the chart
	{
		a = 0;
		b = 0;
	}
	else
	{
		a = list.at(0).toDouble();
		b = list.at(1).toDouble();
	}
	time = seriesA.at(index).x();
	seriesA.replace(index, time, a);
	seriesB.replace(index, time, b);
}

void MainWindow::clickedPoint(QPointF point)
{
	int index = point.x() / (1 / 29.97);
	QTextCursor cursor(ui->plainTextEdit->document()->findBlockByLineNumber(index));
	ui->plainTextEdit->setTextCursor(cursor);
	ui->plainTextEdit->setFocus();
	ui->statusBar->showMessage("Ready");
}
