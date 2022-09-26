#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QVector>
#include <QTimer>
#include <QtMath>
#include "qcustomplot.h"

#include "ui_mainwindow.h"


extern "C" {
#include "../s21_SmartCalc.h"
}

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void input_button();

    void on_equal_button_clicked();
    void on_clear_button_clicked();

    void on_deposit_clicked();
    void on_credit_clicked();

    void on_pushButton_graph_clicked();

private:
    Ui::MainWindow *ui;

    double xBegin, xEnd, h, X, xy_1, xy_2, result_1, result_2;
    int N;

    QVector<double> x, y;
};
#endif // MAINWINDOW_H
