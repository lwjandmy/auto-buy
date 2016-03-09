#include "itemcollectionwidget.h"
#include "ui_itemcollectionwidget.h"
#include "itemwidget.h"
#include <QtCore/QString>
#include <QtCore/QRegExp>
#include <QtGui/QWidget>
#include <QtCore/QtDebug>

ItemCollectionWidget::ItemCollectionWidget(QWidget *parent)
	: QWidget(parent)
	, ui(new Ui::ItemCollectionWidget)
	, memoryCounter("ItemCollectionWidget")
{

	ui->setupUi(this);
}


void ItemCollectionWidget::Init(const QString &inputString)
{
	int iPosition = 0;

// 商品序号，用来更新layout中商品
//	int iItemIndex = 0;

	for (;;)
	{

// <li class="item-info-wrapper item-idle clearfix" data-spm="2007.1000337.18">
// **一个卖家
// find item-info-wrapper
		iPosition = inputString.indexOf(QString("item-info-wrapper"), iPosition);
		if (iPosition < 0)
			break;

// create new item widget, and read from html
		ItemWidget *itemWidget = new ItemWidget(this);
		iPosition = itemWidget->Init(inputString, iPosition);


		ui->verticalLayout->addWidget(itemWidget);


// show item widget
		itemWidget->show();

		//	++iItemIndex;

		qDebug() << "next item";
	}

	qDebug() << "ItemCollectionWidget::Init finish";

	updateGeometry();
}
