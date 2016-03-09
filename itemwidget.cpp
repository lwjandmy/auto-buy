#include "itemwidget.h"
#include "networkaccessmanager.h"
#include <QtGui/QMessageBox>
#include <QtCore/QtDebug>
#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkRequest>
#include <QtNetwork/QNetworkReply>
#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkReply>
#include <QtCore/QFile>
#include <QtCore/QRegExp>
#include <QtGui/QMessageBox>
#include <QtCore/QString>
#include <QtCore/QtDebug>
#include <QtCore/QTextCodec>
#include <ui_itemwidget.h>
#include <QtGui/QWidget>
#include <QtCore/QString>
#include <QtCore/QUrl>
#include <QtNetwork/QNetworkAccessManager>



ItemWidget::ItemWidget(QWidget *parent)
	: QWidget(parent)
	, ui(new Ui::ItemWidget)
//	, networkAccessManager(new QNetworkAccessManager(this))
	, memoryCounter("ItemWidget")
{
//	networkAccessManager->setObjectName("networkAccessManager");
	connect(getNetworkAccessManager(), SIGNAL(finished(QNetworkReply *)),
			this, SLOT(onnetworkAccessManager_finished(QNetworkReply *)));

	ui->setupUi(this);
}



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



int ItemWidget::Init(const QString &inputString, int iPosition)
{

// just for debug
	Item item;

// <a target="_blank" href="http://2.taobao.com/item.htm?id=43084163777"><img alt="戴尔商务笔记本电脑，秒出，价格低，性能好" src="http://img.taobaocdn.com/bao/uploaded/i2/TB1kMIvGVXXXXbxXFXXwsV7.XXX_120x120.jpg"></a>
// *** 图片
// 生成regexp顺序：原匹配regexp，\变\\，"变\"
// \<img.*src=\"(.*)\"\>
// \\<img.*src=\\"(.*)\\"\\>
// \\<img.*src=\\\"(.*)\\\"\\>
	QRegExp regexpImageUrl("\\<img.*src=\\\"(.*)\\\"\\>");
	regexpImageUrl.setMinimal(true);
	iPosition = inputString.indexOf(regexpImageUrl, iPosition);
	setImageUrl(QUrl::fromUserInput(regexpImageUrl.cap(1)));
	item.imageUrl = regexpImageUrl.cap(1);
//	qDebug() << item.imageUrl;


// <h4 class="item-title"><a target="_blank" href="http://2.taobao.com/item.htm?id=43084163777">戴尔商务笔记本电脑，秒出，价格低，性能好</a></h4>
// *** 标题（商品名） 商品链接
// \<a.*href=\"(.*)\"\>(.*)\<\/a\>
// \\<a.*href=\\"(.*)\\"\\>(.*)\\<\\/a\\>
// \\<a.*href=\\\"(.*)\\\"\\>(.*)\\<\\/a\\>
	QRegExp regexpItemTitleUrl("\\<a.*href=\\\"(.*)\\\"\\>(.*)\\<\\/a\\>");
	regexpItemTitleUrl.setMinimal(true);
	iPosition = inputString.indexOf(regexpItemTitleUrl, iPosition);
	setItemTitle(regexpItemTitleUrl.cap(2));
	setItemUrl(QUrl::fromUserInput(regexpItemTitleUrl.cap(1)));
	item.itemTitle = regexpItemTitleUrl.cap(2);
	item.itemUrl = regexpItemTitleUrl.cap(1);
//	qDebug() << item.itemUrl;
//	qDebug() << item.itemTitle;



//     					<div class="item-price price-block"><span class="price"><b>¥</b><em>1750.00</em></span></div>
// *** 价格
// find item-price
	iPosition = inputString.indexOf(QString("item-price"), iPosition);
// \<em\>(.*)\<\/em\>
// \\<em\\>(.*)\\<\\/em\\>
// \\<em\\>(.*)\\<\\/em\\>
	QRegExp regexpItemPrice("\\<em\\>(.*)\\<\\/em\\>");
	regexpItemPrice.setMinimal(true);
	iPosition = inputString.indexOf(regexpItemPrice, iPosition);
	setItemPrice(regexpItemPrice.cap(1));
	item.itemPrice = regexpItemPrice.cap(1);
//	qDebug() << item.itemPrice;



//     					<div class="item-description">&nbsp;1.使用时长：一年多&nbsp;2.新旧程度描述：95成新以上3.存在的问题：没有问题4.到手时间...</div>
// *** 简介
// \<div class\=\"item-description\"\>(.*)\<\/div\>
// \\<div class\\=\\"item-description\\"\\>(.*)\\<\\/div\\>
// \\<div class\\=\\\"item-description\\\"\\>(.*)\\<\\/div\\>
	QRegExp regexpItemDescription("\\<div class\\=\\\"item-description\\\"\\>(.*)\\<\\/div\\>");
	regexpItemDescription.setMinimal(true);
	iPosition = inputString.indexOf(regexpItemDescription, iPosition);
	setItemDescription(regexpItemDescription.cap(1));
	item.itemDescription = regexpItemDescription.cap(1);
//	qDebug() << item.itemDescription;


//     					<div class="item-pub-info">
//     						<span class="item-pub-time">16分钟前</span>
// *** 发布时间
// find item-pub-info
	iPosition = inputString.indexOf(QString("item-pub-info"), iPosition);
// \<span class\=\"item-pub-time\"\>(.*)\<\/span\>
// \\<span class\\=\\"item-pub-time\\"\\>(.*)\\<\\/span\\>
// \\<span class\\=\\\"item-pub-time\\\"\\>(.*)\\<\\/span\\>
	QRegExp regexpItemPubTime("\\<span class\\=\\\"item-pub-time\\\"\\>(.*)\\<\\/span\\>");
	regexpItemPubTime.setMinimal(true);
	iPosition = inputString.indexOf(regexpItemPubTime, iPosition);
	setItemPubTime(regexpItemPubTime.cap(1));
	item.itemPubTime = regexpItemPubTime.cap(1);
//	qDebug() << item.itemPubTime;
//............................................................
    				
//     				<div class="seller-info-wrapper">
//     					<div class="seller-info">
//     						<a target="_blank" href="http://s.2.taobao.com/list/list.htm?usernick=%BE%AE%D6%DE1991" class="seller-avatar">
//     							<img src="http://wwc.taobaocdn.com/avatar/getAvatar.do?userNick=井洲1991&amp;width=60&amp;height=60&amp;type=sns">
// *** 卖家图片
// find seller-info
	iPosition = inputString.indexOf(QString("seller-info"), iPosition);
// \<img src\=\"(.*)\"\>
// \\<img src\\=\\"(.*)\\"\\>
// \\<img src\\=\\\"(.*)\\\"\\>
///////////////////////// so far /////////////////////////
	QRegExp regexpSalerImageUrl("\\<img src\\=\\\"(.*)\\\"\\>");
	regexpSalerImageUrl.setMinimal(true);
	iPosition = inputString.indexOf(regexpSalerImageUrl, iPosition);
	setSallerImageUrl(QUrl::fromUserInput(regexpSalerImageUrl.cap(1)));
	item.salerImageUrl = regexpSalerImageUrl.cap(1);
//	qDebug() << item.salerImageUrl;



//     						</a>
//     						<div class="seller-nick">
//     							<a target="_blank" href="http://s.2.taobao.com/list/list.htm?usernick=%BE%AE%D6%DE1991" class="seller-nick-name">井洲19...</a>
// <a target="_blank" href="http://s.2.taobao.com/list/list.htm?usernick=fenzi2012" class="seller-nick-name">fenzi2...</a>
// *** 卖家名 卖家链接
// find seller-nick
	iPosition = inputString.indexOf(QString("seller-nick"), iPosition);
// \<a target\=\".*\" href\=\"(.*)\" class\=\".*\"\>(.*)\<\/a\>
// \\<a target\\=\\".*\\" href\\=\\"(.*)\\" class\\=\\".*\\"\\>(.*)\\<\\/a\\>
// \\<a target\\=\\\".*\\\" href\\=\\\"(.*)\\\" class\\=\\\".*\\\"\\>(.*)\\<\\/a\\>
	QRegExp regexpSalesNameUrl("\\<a target\\=\\\".*\\\" href\\=\\\"(.*)\\\" class\\=\\\".*\\\"\\>(.*)\\<\\/a\\>");
	regexpSalesNameUrl.setMinimal(true);
	iPosition = inputString.indexOf(regexpSalesNameUrl, iPosition);
	setSallerName(regexpSalesNameUrl.cap(2));
	setSallerUrl(QUrl::fromUserInput(regexpSalesNameUrl.cap(1)));
	item.salesName = regexpSalesNameUrl.cap(2);
	item.salesUrl = regexpSalesNameUrl.cap(1);
//	qDebug() << item.salesUrl;
//	qDebug() << item.salesName;




//     							<span class="J_WangWang" data-nick="井洲1991" data-icon="small"></span>
//     						</div>
//     						<div class="seller-location">山东聊城</div>
// *** 卖家地址
// \<div class\=\"seller\-location\"\>(.*)\<\/div\>
// \\<div class\\=\\"seller\\-location\\"\\>(.*)\\<\\/div\\>
// \\<div class\\=\\\"seller\\-location\\\"\\>(.*)\\<\\/div\\>
	QRegExp regexpSalesLivingLocation("\\<div class\\=\\\"seller\\-location\\\"\\>(.*)\\<\\/div\\>");
	regexpSalesLivingLocation.setMinimal(true);
	iPosition = inputString.indexOf(regexpSalesLivingLocation, iPosition);
	setSallerLivingLocation(regexpSalesLivingLocation.cap(1));
	item.salesLivingLocation = regexpSalesLivingLocation.cap(1);
//	qDebug() << item.salesLivingLocation;



//     						<div class="seller-icons">
//     							        							<span class="sh-user-vip sh-vip-icon3">vip3</span>
//         						        						    						</div>
// *** 卖家等级
// find seller-icons
	iPosition = inputString.indexOf(QString("seller-icons"), iPosition);
// \<span class\=\"sh-user-vip.*\>(.*)\<\/span\>
// \\<span class\\=\\"sh-user-vip.*\\>(.*)\\<\\/span\\>
// \\<span class\\=\\\"sh-user-vip.*\\>(.*)\\<\\/span\\>
	QRegExp regexpSalesGrade("\\<span class\\=\\\"sh-user-vip.*\\>(.*)\\<\\/span\\>");
	regexpSalesGrade.setMinimal(true);
	iPosition = inputString.indexOf(regexpSalesGrade, iPosition);
	setSallerGrade(regexpSalesGrade.cap(1));
	item.salesGrade = regexpSalesGrade.cap(1);
//	qDebug() << item.salesGrade;


	return iPosition;


}
void ItemWidget::onnetworkAccessManager_finished(QNetworkReply *reply)
{
	if (mapNetworkReplyLabel[reply] == 0)
		return ;

	reply->deleteLater();

	QVariant redirectionTarget = reply->attribute(QNetworkRequest::RedirectionTargetAttribute);
	if (redirectionTarget.isNull() == false)
	{
		QUrl url = redirectionTarget.toUrl();
		QNetworkReply *reply_ = getNetworkAccessManager()->get(QNetworkRequest(url));
		if (reply_ != reply)
		{
			mapNetworkReplyLabel[reply_] = mapNetworkReplyLabel[reply];
			mapNetworkReplyLabel.erase(reply);
		}
		return ;
	}

	mapNetworkReplyLabel[reply]->setPixmap(QPixmap::fromImage(QImage::fromData(reply->readAll())));

	mapNetworkReplyLabel.erase(reply);

}

void ItemWidget::setImageUrl(const QUrl &url)
{
//	qDebug() << "----------" << url;
	QNetworkReply *reply = getNetworkAccessManager()->get(QNetworkRequest(url));
	mapNetworkReplyLabel[reply] = ui->imageUrlLabel;
}

void ItemWidget::setItemUrl(const QUrl &url)
{
//	qDebug() << "----------" << url;
	QString str("<a href=\"");
	str += url.toString();
	str +=QString::fromUtf8("\">查看商品</a>");
	ui->itemTitleTextBrowser->insertHtml(str);
}

void ItemWidget::setItemTitle(const QString &str)
{
	ui->itemTitleTextBrowser->insertPlainText(str);
}

void ItemWidget::setItemPrice(const QString &str)
{
	ui->itemPriceTextBrowser->insertPlainText(str);
}

void ItemWidget::setItemDescription(const QString &str)
{
	ui->itemDescriptionTextBrowser->insertPlainText(str);
}

void ItemWidget::setItemPubTime(const QString &str)
{
	ui->itemPubTimeTextBrowser->insertPlainText(str);
}

void ItemWidget::setSallerImageUrl(const QUrl &url)
{
//	qDebug() << "----------" << url;
	QNetworkReply *reply = getNetworkAccessManager()->get(QNetworkRequest(url));
	mapNetworkReplyLabel[reply] = ui->sallerImageUrlLabel;

}

void ItemWidget::setSallerUrl(const QUrl &url)
{
//	qDebug() << "----------" << url;
	QString str("<a href=\"");
	str += url.toString();
	str += QString::fromUtf8("\">查看卖家</a>");
	ui->sallerNameTextBrowser->insertHtml(str);


}

void ItemWidget::setSallerName(const QString &str)
{
	ui->sallerNameTextBrowser->insertPlainText(str);
}

void ItemWidget::setSallerLivingLocation(const QString &str)
{
	ui->sallerLivingLocationTextBrowser->insertPlainText(str);
}

void ItemWidget::setSallerGrade(const QString &str)
{
	ui->sallerGradeTextBrowser->insertPlainText(str);
}

