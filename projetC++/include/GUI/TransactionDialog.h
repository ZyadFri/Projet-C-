#pragma once

#include <QDialog>
#include <QLineEdit>
#include <QDialogButtonBox>
#include "../Transaction.h"

/*!
 * \brief A simple dialog for creating a new transaction.
 */
class TransactionDialog : public QDialog
{
    Q_OBJECT
public:
    explicit TransactionDialog(QWidget *parent = nullptr);
    Transaction getTransaction() const;

private:
    QLineEdit *fromEdit;
    QLineEdit *toEdit;
    QLineEdit *amountEdit;
    QDialogButtonBox *buttonBox;
};
