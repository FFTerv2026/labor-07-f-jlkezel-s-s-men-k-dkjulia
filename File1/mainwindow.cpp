#include "mainwindow.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFileDialog>
#include <QDir>
#include <QDirIterator>
#include <QFile>
#include <QTextStream>
#include <QProgressDialog>
#include <QFileInfo>
#include <QHeaderView>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    QWidget *central = new QWidget(this);
    setCentralWidget(central);

    QVBoxLayout *mainLayout = new QVBoxLayout(central);
    QHBoxLayout *topLayout = new QHBoxLayout();

    searchEdit = new QLineEdit();
    searchEdit->setPlaceholderText("Keresési szöveg...");

    dirButton = new QPushButton("Könyvtár kiválasztása");
    searchButton = new QPushButton("Keresés");

    topLayout->addWidget(searchEdit);
    topLayout->addWidget(dirButton);
    topLayout->addWidget(searchButton);

    table = new QTableWidget();
    table->setColumnCount(2);
    table->setHorizontalHeaderLabels({"Fájlnév", "Méret (byte)"});
    table->horizontalHeader()->setStretchLastSection(true);

    mainLayout->addLayout(topLayout);
    mainLayout->addWidget(table);

    connect(dirButton, &QPushButton::clicked, this, &MainWindow::selectDirectory);
    connect(searchButton, &QPushButton::clicked, this, &MainWindow::startSearch);

    selectedDir = QDir::currentPath();
}

MainWindow::~MainWindow()
{
}

void MainWindow::selectDirectory()
{
    selectedDir = QFileDialog::getExistingDirectory(
        this,
        "Könyvtár kiválasztása",
        QDir::currentPath()
        );
}

void MainWindow::startSearch()
{
    if (selectedDir.isEmpty())
        return;

    QString searchText = searchEdit->text();
    searchFiles(selectedDir, searchText);
}

void MainWindow::searchFiles(QString dirPath, QString searchText)
{
    table->setRowCount(0);

    QDirIterator it(dirPath, QDir::Files, QDirIterator::Subdirectories);

    QList<QString> files;

    while (it.hasNext()) {
        files.append(it.next());
    }

    QProgressDialog progress("Keresés folyamatban...", "Mégse", 0, files.size(), this);
    progress.setWindowModality(Qt::WindowModal);

    int index = 0;

    foreach(QString filePath, files) {

        progress.setValue(index++);
        if (progress.wasCanceled())
            break;

        QFile file(filePath);

        if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
            continue;

        bool match = false;

        if (searchText.isEmpty()) {
            match = true;
        } else {
            QTextStream in(&file);
            QString content = in.readAll();

            if (content.contains(searchText, Qt::CaseInsensitive)) {
                match = true;
            }
        }

        file.close();

        if (match) {
            addToTable(filePath);
        }
    }

    progress.setValue(files.size());
}

void MainWindow::addToTable(QString filePath)
{
    QFileInfo info(filePath);

    int row = table->rowCount();
    table->insertRow(row);

    table->setItem(row, 0, new QTableWidgetItem(info.fileName()));
    table->setItem(row, 1, new QTableWidgetItem(QString::number(info.size())));
}
