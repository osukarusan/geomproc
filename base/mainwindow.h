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

	void on_actionClose_triggered();

	void on_actionRest_triggered();

    void on_actionRenderNormal_triggered();
    void on_actionRenderCorners_triggered();
    void on_actionRenderCurvature_triggered();
    void on_actionRenderOriginal_triggered();
    void on_actionRenderSmoothed_triggered();
    void on_actionRenderCollapsed_triggered();

    void on_actionSmooth_triggered();
    void on_actionEdgeCollapse_triggered();

private:
	Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
