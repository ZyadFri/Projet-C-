#include "MainWindow.h"
#include "TransactionDialog.h"

#include <QMessageBox>
#include <QFileDialog>
#include <QInputDialog>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setWindowTitle("Blockchain Demo");

    centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);

    mainLayout = new QVBoxLayout(centralWidget);

    // Text box to display the blockchain
    blockchainView = new QTextEdit(this);
    blockchainView->setReadOnly(true);
    mainLayout->addWidget(blockchainView);

    // Buttons
    addBlockButton = new QPushButton("Add Block (Data)", this);
    addTransactionButton = new QPushButton("Add Transaction", this);
    validateButton = new QPushButton("Validate Chain", this);
    exportButton = new QPushButton("Export Chain", this);
    importButton = new QPushButton("Import Chain", this);

    // Add buttons to layout
    mainLayout->addWidget(addBlockButton);
    mainLayout->addWidget(addTransactionButton);
    mainLayout->addWidget(validateButton);
    mainLayout->addWidget(exportButton);
    mainLayout->addWidget(importButton);

    // Connect signals to slots
    connect(addBlockButton, &QPushButton::clicked, this, &MainWindow::addNewBlock);
    connect(addTransactionButton, &QPushButton::clicked, this, &MainWindow::addNewTransaction);
    connect(validateButton, &QPushButton::clicked, this, &MainWindow::validateChain);
    connect(exportButton, &QPushButton::clicked, this, &MainWindow::exportChain);
    connect(importButton, &QPushButton::clicked, this, &MainWindow::importChain);

    // Initial display
    updateBlockchainView();
}

void MainWindow::addNewBlock()
{
    // Optionally, user can input the block's data
    bool ok = false;
    QString data = QInputDialog::getText(this, "Add Block", "Enter block data:", QLineEdit::Normal, "", &ok);
    if (ok && !data.isEmpty())
    {
        // Add a block containing just textual data
        blockchain.addBlock(data.toStdString());

        // Alternatively, to mine all pending transactions:
        // blockchain.minePendingTransactions();

        updateBlockchainView();
        QMessageBox::information(this, "Block Added", "A new block has been mined and added to the chain.");
    }
}

void MainWindow::addNewTransaction()
{
    TransactionDialog dialog(this);
    if (dialog.exec() == QDialog::Accepted)
    {
        Transaction tx = dialog.getTransaction();
        blockchain.addTransaction(tx);

        QMessageBox::information(this, "Transaction Added",
                                 "Transaction has been added to pending transactions.\n"
                                 "To include it in the chain, please mine the pending transactions.");
    }
}

void MainWindow::validateChain()
{
    if (blockchain.isChainValid())
    {
        QMessageBox::information(this, "Validation", "Blockchain is valid!");
    }
    else
    {
        QMessageBox::warning(this, "Validation", "Blockchain is NOT valid!");
    }
}

void MainWindow::exportChain()
{
    QString fileName = QFileDialog::getSaveFileName(this, "Export Blockchain", "", "JSON Files (*.json);;All Files (*)");
    if (!fileName.isEmpty())
    {
        try
        {
            blockchain.exportToFile(fileName.toStdString());
            QMessageBox::information(this, "Export", "Blockchain exported successfully!");
        }
        catch (const std::exception &ex)
        {
            QMessageBox::critical(this, "Export Error", ex.what());
        }
    }
}

void MainWindow::importChain()
{
    QString fileName = QFileDialog::getOpenFileName(this, "Import Blockchain", "", "JSON Files (*.json);;All Files (*)");
    if (!fileName.isEmpty())
    {
        try
        {
            bool result = blockchain.importFromFile(fileName.toStdString());
            if (result)
            {
                QMessageBox::information(this, "Import", "Blockchain imported successfully!");
            }
            else
            {
                QMessageBox::warning(this, "Import Failed", "Blockchain import failed or the file was invalid.");
            }
            updateBlockchainView();
        }
        catch (const std::exception &ex)
        {
            QMessageBox::critical(this, "Import Error", ex.what());
        }
    }
}

void MainWindow::updateBlockchainView()
{
    QString chainText;

    const auto &chain = blockchain.getChain();
    for (const auto &block : chain)
    {
        chainText += "Block Index: " + QString::number(block.getIndex()) + "\n";
        chainText += "Timestamp: " + QString::number(block.getTimestamp()) + "\n";
        chainText += "Previous Hash: " + QString::fromStdString(block.getPreviousHash()) + "\n";
        chainText += "Hash: " + QString::fromStdString(block.getHash()) + "\n";
        chainText += "Nonce: " + QString::number(block.getNonce()) + "\n";

        if (!block.getData().empty())
        {
            chainText += "Data: " + QString::fromStdString(block.getData()) + "\n";
        }

        // Show transactions in this block
        if (!block.getTransactions().empty())
        {
            chainText += "Transactions:\n";
            for (const auto &tx : block.getTransactions())
            {
                chainText += "  From: " + QString::fromStdString(tx.getFrom()) +
                             "  To: " + QString::fromStdString(tx.getTo()) +
                             "  Amt: " + QString::number(tx.getAmount()) + "\n";
            }
        }

        chainText += "------------------------------------------\n";
    }

    // Optionally, show pending transactions
    const auto &pendingTxs = blockchain.getPendingTransactions();
    if (!pendingTxs.empty())
    {
        chainText += "\nPending Transactions:\n";
        for (const auto &ptx : pendingTxs)
        {
            chainText += "  From: " + QString::fromStdString(ptx.getFrom()) +
                         "  To: " + QString::fromStdString(ptx.getTo()) +
                         "  Amt: " + QString::number(ptx.getAmount()) + "\n";
        }
    }

    blockchainView->setPlainText(chainText);
}
