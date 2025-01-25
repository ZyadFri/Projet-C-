#pragma once

#include <QMainWindow>
#include <QVBoxLayout>
#include <QPushButton>
#include <QTextEdit>
#include "../Blockchain.h"

/*!
 * \brief MainWindow class provides a basic GUI to interact with the blockchain.
 */
class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = nullptr);

private slots:
    void addNewBlock();
    void addNewTransaction();
    void validateChain();
    void exportChain();
    void importChain();
    void updateBlockchainView();

private:
    Blockchain blockchain;
    QWidget *centralWidget;
    QVBoxLayout *mainLayout;
    QTextEdit *blockchainView;

    QPushButton *addBlockButton;
    QPushButton *addTransactionButton;
    QPushButton *validateButton;
    QPushButton *exportButton;
    QPushButton *importButton;
};
