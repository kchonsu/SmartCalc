#include "credit.h"
#include "ui_credit.h"

credit::credit(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::credit)
{
    ui->setupUi(this);
    ui->lineEdit_monthly_payment->setReadOnly(true);
    ui->lineEdit_total_payment->setReadOnly(true);
    ui->lineEdit_overpayment->setReadOnly(true);
}

credit::~credit()
{
    delete ui;
}


void credit::on_pushButton_credit_clicked()
{
    double total_credit_amount = 0;
    double interes_rate = 0;
    double term = 0;
    double monthly_payment = 0;
    double first_month_payment = 0;
    double last_month_payment = 0;
    double overpayment = 0;
    double total_payment = 0;

    total_credit_amount = ui->lineEdit_total_credit_amount->text().toDouble();
    interes_rate = ui->lineEdit_interes_rate->text().toDouble();
    term = ui->lineEdit_term->text().toDouble();
    double monthly_interes_rate = interes_rate / (100 * 12);

    QString str_monthly_payment = "";
    QString str_overpayment = "";
    QString str_total_payment = "";
    QString str_first_month_payment = "";
    QString str_last_month_payment = "";

    if (ui->radioButton_Annuity->isChecked()) {
        if (ui->lineEdit_total_credit_amount->text() != "" && ui->lineEdit_interes_rate->text() != "" && ui->lineEdit_term->text() != "") {
            monthly_payment = total_credit_amount * (monthly_interes_rate / (1 - pow((1 + monthly_interes_rate), -term)));
            total_payment = monthly_payment * term;
            overpayment = total_payment - total_credit_amount;
            str_monthly_payment = QString::number(monthly_payment);
            str_total_payment = QString::number(total_payment);
            str_overpayment = QString::number(overpayment);
            ui->lineEdit_monthly_payment->setText(str_monthly_payment);
            ui->lineEdit_total_payment->setText(str_total_payment);
            ui->lineEdit_overpayment->setText(str_overpayment);
        }
    }
    if (ui->radioButton_Differentiated->isChecked()) {
        if (ui->lineEdit_total_credit_amount->text() != "" && ui->lineEdit_interes_rate->text() != "" && ui->lineEdit_term->text() != "") {
            double rest = total_credit_amount;
            double monthly = 0;
            for (int i = 0; i < term; i++) {
                if (i == 0) {
                    first_month_payment = rest * monthly_interes_rate + total_credit_amount / term;
                    total_payment += first_month_payment;
                    rest -= first_month_payment;
                } else if (i == (term - 1)) {
                    last_month_payment = rest * monthly_interes_rate + total_credit_amount / term;
                    total_payment += last_month_payment;
                    rest -= last_month_payment;
                } else {
                    monthly = rest * monthly_interes_rate + total_credit_amount / term;
                    total_payment += monthly;
                    rest -= monthly;
                }
            }
            overpayment = total_payment - total_credit_amount;
            str_first_month_payment = QString::number(first_month_payment);
            str_last_month_payment = QString::number(last_month_payment);
            str_total_payment = QString::number(total_payment);
            str_overpayment = QString::number(overpayment);
            ui->lineEdit_monthly_payment->setText(str_first_month_payment + "..." + str_last_month_payment);
            ui->lineEdit_total_payment->setText(str_total_payment);
            ui->lineEdit_overpayment->setText(str_overpayment);
        }
    }
}
