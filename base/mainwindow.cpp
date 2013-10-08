#include <QFileDialog>
#include <QMessageBox>
#include "mainwindow.h"
#include "ui_mainwindow.h"

#ifdef MACOSX
#define MODELS_DIR "../../../../models"
#else
#define MODELS_DIR "../models"
#endif

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
	close();
}

void MainWindow::on_actionOpen_triggered()
{
	QString filename;

    filename = QFileDialog::getOpenFileName(this, tr("Load model"), MODELS_DIR, tr("PLY Files (*.ply)"));
	if(!filename.isNull())
	{
		if(!ui->glwidget->loadMesh(filename))
			QMessageBox::warning(this, tr("Error"), tr("The file could not be opened"));
	}
}

void MainWindow::on_actionClose_triggered()
{
	ui->glwidget->closeMesh();
}

void MainWindow::on_actionRest_triggered()
{
	ui->glwidget->resetCamera();
}
