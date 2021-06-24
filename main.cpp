#include <fstream>
#include <vector>
#include <QtCore>
#include <QApplication>
#include <QMainWindow>
#include <QVBoxLayout>
#include "myTable.h"

using namespace std;

int main(int argc, char *argv[])
{
    ifstream inFile;
    inFile.open(getenv("MYCRYPTOCONVERT")); // Get the filename from environment variables
    vector<QString> givens; // Names and/or symbols given
    string line = ""; // Current line to read
    while (getline(inFile, line))
        givens.push_back(QString::fromStdString(line)); // Read the lines 
    QApplication a(argc, argv); // Initialize Qt application
    QMainWindow window; // Create the main window to be able to resize the table
    QWidget *centralWidget = new QWidget; 
    window.setCentralWidget(centralWidget);
    QGridLayout *centralLayout = new QGridLayout(centralWidget);
    MyTable *m = new MyTable(givens);
    centralLayout->addWidget(m);
    window.resize(620, 400); // Initial size of the window
    window.show(); 
    return a.exec();
}