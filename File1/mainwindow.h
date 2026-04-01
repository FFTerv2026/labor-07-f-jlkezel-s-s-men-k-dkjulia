#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLineEdit>
#include <QPushButton>
#include <QTableWidget>

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void selectDirectory();
    void startSearch();

private:
    void searchFiles(QString dirPath, QString searchText);
    void addToTable(QString filePath);

    QLineEdit *searchEdit;
    QPushButton *dirButton;
    QPushButton *searchButton;
    QTableWidget *table;

    QString selectedDir;
};

#endif // MAINWINDOW_H
