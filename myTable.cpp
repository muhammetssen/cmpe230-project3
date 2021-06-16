#include "myTable.h"
#include <iostream>
#include <QHeaderView>
#include <QWidget>
#include <QSizePolicy>
MyTable::MyTable(vector<QString> wantedCurrencies) : QTableWidget()
{
    this->wantedCurrencies = wantedCurrencies;
    this->table = new QTableWidget(this);
    this->table->setRowCount(wantedCurrencies.size());
    this->table->setColumnCount(3);
    table->setMinimumSize(400,300);
    QStringList verticalLabels = QStringList::fromVector(QVector<QString>::fromStdVector(wantedCurrencies));
    this->topLabels.push_back(QString::fromStdString("USD"));
    this->topLabels.push_back(QString::fromStdString("EUR"));
    this->topLabels.push_back(QString::fromStdString("GBP"));
    
    QStringList horizontalLabels = QStringList::fromVector(QVector<QString>::fromStdVector(topLabels));
    this->table->setVerticalHeaderLabels(verticalLabels);
    this->table->setHorizontalHeaderLabels(horizontalLabels);
    // this->table->horizontalHeader()->sectionResizeMode()

    manager = new QNetworkAccessManager(this);
    string url ="https://api.coingecko.com/api/v3/simple/price?ids=";
    
    for(int i =0; i < wantedCurrencies.size()-1; i++){
        url += wantedCurrencies[i].toLower().toStdString() + ",";
    }
    url += wantedCurrencies[wantedCurrencies.size() -1].toStdString() + "&vs_currencies=usd,eur,gbp";

    connect(manager, SIGNAL(finished(QNetworkReply *)), this, SLOT(insertData(QNetworkReply *)));
    manager->get(QNetworkRequest(QUrl(QString::fromStdString(url))));
}

void MyTable::insertData(QNetworkReply *reply)
{
    QString data = reply->readAll();
    QJsonDocument jsonResponse = QJsonDocument::fromJson(data.toUtf8());
    QJsonObject jsonObject = jsonResponse.object();
    for (int i = 0; i < this->wantedCurrencies.size(); i++)
    {
        QString currency = this->wantedCurrencies[i];
        QJsonObject x = jsonObject[currency].toObject();
        for (int j = 0; j < this->topLabels.size(); j++)
        {
            QString label = this->topLabels[j];
            QJsonValueRef y = x[label.toLower()];
            QTableWidgetItem* item = new QTableWidgetItem();
            item->setData(Qt::DisplayRole, y.toDouble());
            this->table->setItem(i, j, item);
            // this->table->setItem(i, j, new QTableWidgetItem(tr("%1").arg(y.toDouble())));
        }

    }    
    this->table->resizeColumnsToContents();
    
    this->table->update();
}
