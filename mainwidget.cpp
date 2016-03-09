#include "mainwidget.h"
#include "itemcollectionwidget.h"
#include "ui_itemcollectionwidget.h"
#include "ui_itemwidget.h"
#include "networkaccessmanager.h"
#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkReply>
#include <QtCore/QFile>
#include <QtCore/QRegExp>
#include <QtGui/QMessageBox>
#include <QtCore/QString>
#include <QtCore/QtDebug>
#include <QtCore/QTextCodec>
#include <QtGui/QScrollArea>
#include "ui_mainwidget.h"
#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkReply>
#include <QtXml/QXmlAttributes>
#include <QtCore/QString>
#include <QtCore/QXmlStreamReader>
#include <QtCore/QTimer>



class Item
{
public:
	QString imageUrl;
	QString itemUrl; 
	QString itemTitle;
	QString itemPrice;
	QString itemDescription;
	QString itemPubTime;
	QString salerImageUrl;
	QString salesUrl;
	QString salesName;
	QString salesLivingLocation;
	QString salesGrade;
};


MainWidget::MainWidget(QWidget *parent)
	: QWidget(parent)
	, ui(new Ui::MainWidget)
//	, networkAccessManager(new QNetworkAccessManager(this))
	, timer(new QTimer(this))
	, memoryCounter("MainWidget")


{
//	networkAccessManager->setObjectName("networkAccessManager");
	connect(getNetworkAccessManager(), SIGNAL(finished(QNetworkReply *)),
			this, SLOT(onnetworkAccessManager_finished(QNetworkReply *)));
	timer->setObjectName("timer");
	ui->setupUi(this);

   
//	return ;

	ItemCollectionWidget *itemCollectionWidget = new ItemCollectionWidget(this);

	QFile reply1("download.html");
	QFile *reply = &reply1;
	reply1.open(QIODevice::ReadOnly);

// read reply html
    QByteArray inputByteArray(reply->readAll());
	QString inputString(QTextCodec::codecForHtml(inputByteArray)->toUnicode(inputByteArray));

	itemCollectionWidget->Init(inputString);

	ui->verticalLayout->addWidget(itemCollectionWidget);

}



void MainWidget::onnetworkAccessManager_finished(QNetworkReply *reply)
{
	if (mapNetworkReply[reply] == 0)
		return ;


	reply->deleteLater();

	ItemCollectionWidget *itemCollectionWidget = new ItemCollectionWidget(this);

// read reply html
    QByteArray inputByteArray(reply->readAll());
	QString inputString(QTextCodec::codecForHtml(inputByteArray)->toUnicode(inputByteArray));

	itemCollectionWidget->Init(inputString);

	if (ui->verticalLayout->count() == 1) // collectionWidget not exist
	{		itemCollectionWidget->show();
		ui->verticalLayout->addWidget(itemCollectionWidget);
		qDebug() << "on_finished collectionWidget not exist";
	}
	else
	{

/////  bug appear
		qDebug() << "MainWidget::debug 1";
		QLayoutItem *originLayoutItem = ui->verticalLayout->takeAt(1);
		ItemCollectionWidget *originItemCollectionWidget = (ItemCollectionWidget *)originLayoutItem->widget();
		qDebug() << "MainWidget::debug 2";


// debug
		QLayoutItem *layoutItem1 = itemCollectionWidget->ui->verticalLayout->itemAt(0);
		ItemWidget *itemWidget1 = (ItemWidget *)layoutItem1->widget();
		QString str1 = itemWidget1->ui->itemTitleTextBrowser->toPlainText();

		QLayoutItem *layoutItem2 = originItemCollectionWidget->ui->verticalLayout->itemAt(0);
		ItemWidget *itemWidget2 = (ItemWidget *)layoutItem2->widget();
		QString str2 = itemWidget2->ui->itemTitleTextBrowser->toPlainText();

		((ItemWidget *)originItemCollectionWidget->ui->verticalLayout->itemAt(0)->widget())->ui->itemTitleTextBrowser->toPlainText();


		if (((ItemWidget *)itemCollectionWidget->ui->verticalLayout->itemAt(0)->widget())->ui->itemTitleTextBrowser->toPlainText() !=
			((ItemWidget *)originItemCollectionWidget->ui->verticalLayout->itemAt(0)->widget())->ui->itemTitleTextBrowser->toPlainText())
		{

			QMessageBox *messageBox = new QMessageBox(QMessageBox::Information, QString::fromUtf8("提示"), QString::fromUtf8("商品列表变化!"));
			messageBox->setWindowFlags(messageBox->windowFlags() | Qt::WindowStaysOnTopHint);
			messageBox->setAttribute(Qt::WA_DeleteOnClose);
			messageBox->show();
		}

		ui->verticalLayout->addWidget(itemCollectionWidget);
		itemCollectionWidget->show();

		qDebug() << "on_finished collectionWidget exist";

		originItemCollectionWidget->deleteLater();
		delete originLayoutItem;
	}

	qDebug() << "on_networkAccessManager_finished updateGeometry()";
	updateGeometry();
	mapNetworkReply.erase(reply);
	qDebug() << "on_networkAccessManager_finished finish";

}

void MainWidget::on_startPushButton_clicked(bool checked)
{
	if (checked)
	{
// Init timer clock: if collectionWidget exist, hide it
		if (ui->verticalLayout->count() == 2) // collectionWidget
		{
			QLayoutItem *originLayoutItem = ui->verticalLayout->takeAt(1);
			originLayoutItem->widget()->deleteLater();
			delete originLayoutItem;
		}

// Init timer clock: create first request
		QNetworkRequest request(QUrl::fromUserInput(
									ui->taobaoItemLinkLineEdit->text()));		
		QNetworkReply *networkReply = getNetworkAccessManager()->get(request);
		mapNetworkReply[networkReply] = 1;


// get timer interval
		int iRefreshInterval = ui->refreshIntervalLineEdit->text().toInt();

// set remain time
		ui->timerRemainTimeLineEdit->setText(QString::number(iRefreshInterval));
		
		timer->start(1000); // 1000ms = 1s
	}
	else
	{
		ui->timerRemainTimeLineEdit->setText("");
		timer->stop();
	}
}

void MainWidget::on_timer_timeout()
{
// get remain time
	int iRemainTime = ui->timerRemainTimeLineEdit->text().toInt();
// remain time subtract 1
	--iRemainTime;
// set remain time
	ui->timerRemainTimeLineEdit->setText(QString::number(iRemainTime));

	if (iRemainTime <= 0)
	{
// get timer interval
		int iRefreshInterval = ui->refreshIntervalLineEdit->text().toInt();

// set remain time
		ui->timerRemainTimeLineEdit->setText(QString::number(iRefreshInterval));

// generate network request
		QNetworkRequest request(QUrl::fromUserInput(
									ui->taobaoItemLinkLineEdit->text()));					
		QNetworkReply *networkReply = getNetworkAccessManager()->get(request);
		mapNetworkReply[networkReply] = 1;
	}


}

const char *d_QString(const QString &str)
{
	static QByteArray ba;
	ba = str.toLatin1();
	return ba.constData();
}

const char *d_QStringRef(const QStringRef &str)
{
	return d_QString(*str.string());
}
