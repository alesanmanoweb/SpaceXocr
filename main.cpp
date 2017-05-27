#include "mainwindow.h"
#include <QApplication>
#include <QCoreApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	QCoreApplication::setOrganizationName("Manoweb");
	QCoreApplication::setApplicationName("SpaceXocr");
	QCoreApplication::setApplicationVersion("0.0.2");
	MainWindow w;
	w.show();

	return a.exec();
}
