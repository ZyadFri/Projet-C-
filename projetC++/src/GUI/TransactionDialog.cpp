#include "TransactionDialog.h"
#include <QFormLayout>
#include <QPushButton>

TransactionDialog::TransactionDialog(QWidget *parent)
    : QDialog(parent),
      fromEdit(new QLineEdit(this)),
      toEdit(new QLineEdit(this)),
      amountEdit(new QLineEdit(this))
{
    setWindowTitle("New Transaction");

    auto layout = new QFormLayout(this);

    layout->addRow("From (Public Key):", fromEdit);
    layout->addRow("To (Public Key):", toEdit);
    layout->addRow("Amount:", amountEdit);

    buttonBox = new QDialogButtonBox(
        QDialogButtonBox::Ok | QDialogButtonBox::Cancel,
        this);
    layout->addRow(buttonBox);

    connect(buttonBox, &QDialogButtonBox::accepted,
            this, &QDialog::accept);
    connect(buttonBox, &QDialogButtonBox::rejected,
            this, &QDialog::reject);
}

Transaction TransactionDialog::getTransaction() const
{
    return Transaction(
        fromEdit->text().toStdString(),
        toEdit->text().toStdString(),
        amountEdit->text().toDouble());
}
