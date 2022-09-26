#include "deposit.h"
#include "ui_deposit.h"

deposit::deposit(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::deposit)
{
    ui->setupUi(this);
    ui->lineEdit_accured_interest->setReadOnly(true);
    ui->lineEdit_tax_ammount->setReadOnly(true);
    ui->lineEdit_deposit_end->setReadOnly(true);
}

deposit::~deposit()
{
    delete ui;
}

void deposit::on_pushButton_credit_clicked()
{
    double deposit_amount = 0;
    double deposit_term = 0;
    double interest_rate = 0;
    double tax_rate = 0;
    double periodicity_of_payments = 0;
    double replenishments_sum = 0;
    double replenishments_month = 0;
    double partial_withdrawals_sum = 0;
    double partial_withdrawals_month = 0;

    double accrued_interest = 0;
    double tax_amount = 0;
    double deposit_end = 0;

    deposit_amount = ui->lineEdit_deposit_amount->text().toDouble();
    deposit_term = ui->lineEdit_deposit_term->text().toDouble();
    interest_rate = ui->lineEdit_interes_rate->text().toDouble();
    tax_rate = ui->lineEdit_tax_rate->text().toDouble();
    periodicity_of_payments = ui->lineEdit_periodicity_of_payments->text().toDouble();
    partial_withdrawals_sum = ui->lineEdit_withdraw_sum->text().toDouble();
    partial_withdrawals_month = ui->lineEdit_partial_withdraw_month->text().toDouble();
    replenishments_sum = ui->lineEdit_replenishments_sum->text().toDouble();
    replenishments_month = ui->lineEdit_partial_replanishment_month->text().toDouble();


    QString str_accrued_interest = "";
    QString str_tax_amount = "";
    QString str_deposit_end = "";

    if (ui->checkBox->isChecked()) {
        double accrued_month_interest = 0;
        double month_tax_amount = 0;
        for (int i = 0; i < deposit_term; i++) {
            accrued_month_interest = (((deposit_amount/100)*interest_rate)/12);
            month_tax_amount = accrued_month_interest * (tax_rate / 100);
            accrued_month_interest = accrued_month_interest - month_tax_amount;
            deposit_amount += accrued_month_interest;
            accrued_interest += accrued_month_interest;
            tax_amount += month_tax_amount;
            if (i == replenishments_month && replenishments_sum > 0) {
                deposit_amount += replenishments_sum;
            }
            if (i == partial_withdrawals_month && partial_withdrawals_sum > 0) {
                deposit_amount -= partial_withdrawals_sum;
            }
        }
    } else if (partial_withdrawals_month != 0 || replenishments_month != 0) {
        double accrued_month_interest = 0;
        for (int i = 0; i < deposit_term; i++) {
            accrued_month_interest = (((deposit_amount/100)*interest_rate)/12);
            accrued_interest += accrued_month_interest;
            if (i == replenishments_month && replenishments_sum > 0) {
                deposit_amount += replenishments_sum;
            }
            if (i == partial_withdrawals_month && partial_withdrawals_sum > 0) {
                deposit_amount -= partial_withdrawals_sum;
            }
        }
        tax_amount = accrued_interest * (tax_rate / 100);
        accrued_interest = accrued_interest - tax_amount;
    } else {
        accrued_interest = (((deposit_amount/100)*interest_rate)/12)*deposit_term;
        tax_amount = accrued_interest * (tax_rate / 100);
        accrued_interest = accrued_interest - tax_amount;
    }

    deposit_end = accrued_interest + deposit_amount;

    str_accrued_interest = QString::number(accrued_interest);
    str_deposit_end = QString::number(deposit_end);
    str_tax_amount = QString::number(tax_amount);

    ui->lineEdit_accured_interest->setText(str_accrued_interest);
    ui->lineEdit_deposit_end->setText(str_deposit_end);
    ui->lineEdit_tax_ammount->setText(str_tax_amount);
}
