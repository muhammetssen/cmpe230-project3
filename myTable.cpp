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
    table->setMinimumSize(600, 420);
    QStringList verticalLabels = QStringList::fromVector(QVector<QString>::fromStdVector(wantedCurrencies));
    this->topLabels.push_back(QString::fromStdString("USD"));
    this->topLabels.push_back(QString::fromStdString("EUR"));
    this->topLabels.push_back(QString::fromStdString("GBP"));

    QStringList horizontalLabels = QStringList::fromVector(QVector<QString>::fromStdVector(topLabels));
    this->table->setVerticalHeaderLabels(verticalLabels);
    this->table->setHorizontalHeaderLabels(horizontalLabels);
    // this->table->horizontalHeader()->sectionResizeMode()

    manager = new QNetworkAccessManager(this);
    connect(manager, SIGNAL(finished(QNetworkReply *)), this, SLOT(nameToId(QNetworkReply *)));
    manager->get(QNetworkRequest(QUrl(QString::fromStdString("https://api.coingecko.com/api/v3/coins/list"))));
}

void MyTable::nameToId(QNetworkReply *coinList)
{
    disconnect(manager, SIGNAL(finished(QNetworkReply *)), this, SLOT(nameToId(QNetworkReply *)));

    QString data = coinList->readAll();
    QJsonDocument jsonResponse = QJsonDocument::fromJson(data.toUtf8());
    QJsonArray jsonArray = jsonResponse.array();

    for (const QString currency : this->wantedCurrencies)
    {
        for (const auto obj : jsonArray)
        {
            if (obj.toObject()["name"].toString().toLower() == currency.toLower() || obj.toObject()["symbol"].toString().toLower() == currency.toLower())
            {
                coinIDs.push_back(obj.toObject().value("id").toString());
                qDebug() << currency << obj.toObject() ;
                continue;
            }
        }
    }
    // qDebug() << coinIDs;

    string url = "https://api.coingecko.com/api/v3/simple/price?ids=";
    for (int i = 0; i < coinIDs.size() - 1; i++)
    {
        url += coinIDs[i].toLower().toStdString() + ",";
    }
    url += coinIDs[coinIDs.size() - 1].toStdString() + "&vs_currencies=usd,eur,gbp";
    qDebug() << "URL:" << QString::fromStdString(url);
    connect(manager, SIGNAL(finished(QNetworkReply *)), this, SLOT(insertData(QNetworkReply *)));
    manager->get(QNetworkRequest(QUrl(QString::fromStdString(url))));
}
void MyTable::insertData(QNetworkReply *reply)
{
    QString data = reply->readAll();
    QJsonDocument jsonResponse = QJsonDocument::fromJson(data.toUtf8());
    QJsonObject jsonObject = jsonResponse.object();
    for (int i = 0; i < this->coinIDs.size(); i++)
    {
        
        QString currency = this->coinIDs[i];
        QJsonObject x = jsonObject[currency].toObject();
        qDebug() << currency << x;
        for (int j = 0; j < this->topLabels.size(); j++)
        {
            QString label = this->topLabels[j];
            QJsonValueRef y = x[label.toLower()];
            QTableWidgetItem *item = new QTableWidgetItem();
            item->setData(Qt::DisplayRole, y.toDouble());
            this->table->setItem(i, j, item);
            // this->table->setItem(i, j, new QTableWidgetItem(tr("%1").arg(y.toDouble())));
        }
    }
    this->table->resizeColumnsToContents();

    this->table->update();
}
