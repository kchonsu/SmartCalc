#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "credit.h"
#include "deposit.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    connect(ui->pushButton_0, SIGNAL(clicked()), this, SLOT(input_button()));
    connect(ui->pushButton_1, SIGNAL(clicked()), this, SLOT(input_button()));
    connect(ui->pushButton_2, SIGNAL(clicked()), this, SLOT(input_button()));
    connect(ui->pushButton_3, SIGNAL(clicked()), this, SLOT(input_button()));
    connect(ui->pushButton_4, SIGNAL(clicked()), this, SLOT(input_button()));
    connect(ui->pushButton_5, SIGNAL(clicked()), this, SLOT(input_button()));
    connect(ui->pushButton_6, SIGNAL(clicked()), this, SLOT(input_button()));
    connect(ui->pushButton_7, SIGNAL(clicked()), this, SLOT(input_button()));
    connect(ui->pushButton_8, SIGNAL(clicked()), this, SLOT(input_button()));
    connect(ui->pushButton_9, SIGNAL(clicked()), this, SLOT(input_button()));
    connect(ui->pushButton_dot, SIGNAL(clicked()), this, SLOT(input_button()));
    connect(ui->pushButton_x, SIGNAL(clicked()), this, SLOT(input_button()));
    connect(ui->plus, SIGNAL(clicked()), this, SLOT(input_button()));
    connect(ui->minus, SIGNAL(clicked()), this, SLOT(input_button()));
    connect(ui->mult, SIGNAL(clicked()), this, SLOT(input_button()));
    connect(ui->div, SIGNAL(clicked()), this, SLOT(input_button()));
    connect(ui->mod, SIGNAL(clicked()), this, SLOT(input_button()));
    connect(ui->degree, SIGNAL(clicked()), this, SLOT(input_button()));
    connect(ui->sin, SIGNAL(clicked()), this, SLOT(input_button()));
    connect(ui->cos, SIGNAL(clicked()), this, SLOT(input_button()));
    connect(ui->tan, SIGNAL(clicked()), this, SLOT(input_button()));
    connect(ui->asin, SIGNAL(clicked()), this, SLOT(input_button()));
    connect(ui->acos, SIGNAL(clicked()), this, SLOT(input_button()));
    connect(ui->atan, SIGNAL(clicked()), this, SLOT(input_button()));
    connect(ui->log, SIGNAL(clicked()), this, SLOT(input_button()));
    connect(ui->ln, SIGNAL(clicked()), this, SLOT(input_button()));
    connect(ui->sqrt, SIGNAL(clicked()), this, SLOT(input_button()));
    connect(ui->l_bracket, SIGNAL(clicked()), this, SLOT(input_button()));
    connect(ui->r_bracket, SIGNAL(clicked()), this, SLOT(input_button()));

}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::input_button() {
    QPushButton *button = (QPushButton *) sender();
    QString all_buttons;
    if (ui->result->text() == '0') {
        ui->result->setText("");
    }
    all_buttons = (ui->result->text() + button->text());
    ui->result->setText(all_buttons);

}

void MainWindow::on_equal_button_clicked()
{
    double x = 0;
        QPushButton *button = (QPushButton *)sender();
        button->setChecked(true);
        QString send = ui->result->text();
        QByteArray ba = send.toLocal8Bit();
        char* buf = ba.data();

        if (ui->x_value->text() != "") {
            x = (ui->x_value->text().toDouble());
        }
        QString toStr = QString::number(s21_parser(buf, x), 'g', 15);
        ui->result->setText(toStr);
}

void MainWindow::on_clear_button_clicked()
{
    ui->result->setText("0");
}

void MainWindow::on_credit_clicked()
{
    credit window;
    window.setModal(true);
    window.exec();
}

void MainWindow::on_deposit_clicked()
{
    deposit window;
    window.setModal(true);
    window.exec();
}

void MainWindow::on_pushButton_graph_clicked()
{
    QString send = ui->result->text();
        QByteArray ba = send.toLocal8Bit();
        char* buf = ba.data();
        ui->widget->clearGraphs();
        x.clear();
        y.clear();
        result_1 = 0;
        result_2 = 0;
        xBegin = 0;
        xEnd = 0;
        h = 0.1;

        double Y = ui->x_value->text().toDouble();
        double X = 0;
        if (Y == 0) Y = 1;

        xy_1 = ui->x1->text().toDouble();
        xy_2 = ui->x2->text().toDouble();
        result_1 = ui->y1->text().toInt();
        result_2 = ui->y2->text().toInt();

        xBegin = result_2;
        xEnd = result_1 + h;

        ui->widget->xAxis->setRange(xy_2, xy_1);
        ui->widget->yAxis->setRange(result_2, result_1);
        N = (xEnd - xBegin)/h + 2;

        for (X = xBegin; X <= xEnd; X += h) {
            x.push_back(X);
            y.push_back(s21_parser(buf, Y*X));
        }
        ui->widget->addGraph();
        ui->widget->graph(0)->addData(x, y);
        ui->widget->replot();
}

