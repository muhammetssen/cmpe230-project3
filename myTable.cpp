#include "myTable.h"
#include <iostream>
#include <QHeaderView>
#include <QWidget>
#include <QSizePolicy>
MyTable::MyTable(vector<QString> wantedCurrencies) : QTableWidget()
{
    this->wantedCurrencies = wantedCurrencies; 
    this->table = new QTableWidget(this); // Create a table widget
    this->table->setRowCount(wantedCurrencies.size()); // Initiate the number of rows
    this->table->setColumnCount(3); // 3 Columns: USD, EUR, GBP
    table->setMinimumSize(600, 420); // Size of the table
    QStringList verticalLabels = QStringList::fromVector(QVector<QString>::fromStdVector(wantedCurrencies)); //from QString vector to QStringList
    this->table->setVerticalHeaderLabels(verticalLabels);

    this->topLabels.push_back(QString::fromStdString("USD"));
    this->topLabels.push_back(QString::fromStdString("EUR"));
    this->topLabels.push_back(QString::fromStdString("GBP"));
    QStringList horizontalLabels = QStringList::fromVector(QVector<QString>::fromStdVector(topLabels));
    this->table->setHorizontalHeaderLabels(horizontalLabels);

    manager = new QNetworkAccessManager(this); // Initialize the network manager to make requests
    connect(manager, SIGNAL(finished(QNetworkReply *)), this, SLOT(nameToId(QNetworkReply *))); // Run nameToId function after the response came.
    manager->get(QNetworkRequest(QUrl(QString::fromStdString("https://api.coingecko.com/api/v3/coins/list")))); // Make the request to list endpoint
}

void MyTable::nameToId(QNetworkReply *coinList)
{
    disconnect(manager, SIGNAL(finished(QNetworkReply *)), this, SLOT(nameToId(QNetworkReply *)));  // We connected nameToId function to manager but we need to
                                                                                                    // run another function so disconnect and connect another one.
    QString data = coinList->readAll(); // Read the reply
    QJsonDocument jsonResponse = QJsonDocument::fromJson(data.toUtf8()); // reply to json
    QJsonArray jsonArray = jsonResponse.array(); // Endpoint returns us a list so we need to use QJsonArray 

    for (const QString currency : this->wantedCurrencies)
    {
        for (const auto obj : jsonArray)
        {   // If the name or symbol of a coın is equal to wanted coin, add to vector
            if (obj.toObject()["name"].toString().toLower() == currency.toLower() || obj.toObject()["symbol"].toString().toLower() == currency.toLower())
            {
                coinIDs.push_back(obj.toObject().value("id").toString());
                break;
            }
        }
    }
    // qDebug() << coinIDs;

    string url = "https://api.coingecko.com/api/v3/simple/price?ids=";
    for (int i = 0; i < int(coinIDs.size() - 1); i++)
    {
        url += coinIDs[i].toLower().toStdString() + ","; // Fence Problem, add '{name} ,' until the last one
    }
    url += coinIDs[coinIDs.size() - 1].toStdString() + "&vs_currencies=usd,eur,gbp"; // Just add '{name}', without comma
    connect(manager, SIGNAL(finished(QNetworkReply *)), this, SLOT(insertData(QNetworkReply *))); // 
    manager->get(QNetworkRequest(QUrl(QString::fromStdString(url)))); // Make request to get the prices
}
void MyTable::insertData(QNetworkReply *reply)
{
    QString data = reply->readAll(); // Read the reply
    QJsonDocument jsonResponse = QJsonDocument::fromJson(data.toUtf8()); // Encode the data
    QJsonObject jsonObject = jsonResponse.object();
    for (int i = 0; i < int(this->coinIDs.size()); i++) // Iterate over the coins
    {
        
        QString currency = this->coinIDs[i]; //Get coin id
        QJsonObject x = jsonObject[currency].toObject(); //Get the JsonObject by giving id
        // qDebug() << currency << x;
        for (int j = 0; j < int(this->topLabels.size()); j++) // Toplabels are usd, eur, and gbp
        {
            QString label = this->topLabels[j]; 
            QJsonValueRef y = x[label.toLower()];
            QTableWidgetItem *item = new QTableWidgetItem();// Create new table item
            item->setData(Qt::DisplayRole, y.toDouble()); // Insert the price to the item
            this->table->setItem(i, j, item); // Insert the item to table
        }
    }
    this->table->resizeColumnsToContents(); // Resize the column widths to make the prices visible
}
