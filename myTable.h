#include <QtGui>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QTableWidget>
#include <vector>
using namespace std;
class MyTable : public QTableWidget
{
    Q_OBJECT
public:
    vector<QString> wantedCurrencies;
        vector<QString> topLabels;
        vector<QString> coinIDs;

    MyTable(vector<QString> wantedCurrencies);
public slots:
    void nameToId(QNetworkReply *coinList);
    void insertData(QNetworkReply *reply);

private:
    QTableWidget *table;
    QNetworkAccessManager *manager;


};
