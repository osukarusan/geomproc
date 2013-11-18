#include <QFileDialog>
#include <QMessageBox>
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "dialogsmoothing.h"
#include "dialogcollapse.h"

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

    QActionGroup* rendergroup = new QActionGroup(this);
    rendergroup->addAction(ui->actionRenderNormal);
    rendergroup->addAction(ui->actionRenderValence);
    rendergroup->addAction(ui->actionRenderCorners);
    rendergroup->addAction(ui->actionRenderCurvature);

    QActionGroup* meshgroup = new QActionGroup(this);
    meshgroup->addAction(ui->actionRenderOriginal);
    meshgroup->addAction(ui->actionRenderSmoothed);
    meshgroup->addAction(ui->actionRenderCollapsed);
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

void MainWindow::on_actionRenderNormal_triggered()
{
    ui->glwidget->gRendertype = RENDER_NORMAL;
    ui->glwidget->updateGL();
}

void MainWindow::on_actionRenderValence_triggered()
{
    ui->glwidget->gRendertype = RENDER_VALENCE;
    ui->glwidget->updateGL();
}

void MainWindow::on_actionRenderCorners_triggered()
{
    ui->glwidget->gRendertype = RENDER_CORNERS;
    ui->glwidget->updateGL();
}

void MainWindow::on_actionRenderCurvature_triggered()
{
    ui->glwidget->gRendertype = RENDER_CURVATURE;
    ui->glwidget->updateGL();
}

void MainWindow::on_actionRenderOriginal_triggered()
{
    ui->glwidget->gRendermesh = RENDER_ORIGINAL;
    ui->glwidget->updateGL();
}

void MainWindow::on_actionRenderSmoothed_triggered()
{
    ui->glwidget->gRendermesh = RENDER_SMOOTHED;
    ui->glwidget->updateGL();
}

void MainWindow::on_actionRenderCollapsed_triggered()
{
    ui->glwidget->gRendermesh = RENDER_COLLAPSED;
    ui->glwidget->updateGL();
}

void MainWindow::on_actionSmooth_triggered()
{
    DialogSmoothing dialog(this);
    if (dialog.exec() == QDialog::Accepted) {
        ui->glwidget->setSmoothParameters(dialog.getNumIterations(), dialog.getLambda());
        ui->actionRenderSmoothed->setChecked(true);
        this->on_actionRenderSmoothed_triggered();
    }
}

void MainWindow::on_actionEdgeCollapse_triggered()
{
    DialogCollapse dialog(this);
    if (dialog.exec() == QDialog::Accepted) {
        ui->glwidget->setCollapseParameters(dialog.getNumIterations(), dialog.getThreshold(), dialog.getNumCollapses());
        ui->actionRenderCollapsed->setChecked(true);
        this->on_actionRenderCollapsed_triggered();
    }
}
