#include <iostream>
#include <fstream>
#include <vector>
#include <QtCore>
#include <QApplication>
#include <QMainWindow>
#include "myTable.h"
#include <QVBoxLayout>

using namespace std;

int main(int argc, char *argv[])
{
    ifstream inFile;
    inFile.open("givens.txt");
    vector<QString> givens;
    string line = "";
    while (getline(inFile, line))
    {
        givens.push_back(QString::fromStdString(line));
    }
    QApplication a(argc, argv);
    QMainWindow window;
    QWidget *centralWidget = new QWidget;
    window.setCentralWidget(centralWidget);
    QGridLayout *centralLayout = new QGridLayout(centralWidget);
    MyTable *m = new MyTable(givens);
    centralLayout->addWidget(m);
    window.resize(420, 300);
    window.show();
    return a.exec();
}