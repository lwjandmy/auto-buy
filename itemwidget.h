#ifndef MY_ITEMWIDGET_H
#define MY_ITEMWIDGET_H


#include <QtGui/QWidget>
#include <memory>
#include <map>

class QNetworkAccessManager;
class QNetworkReply;
class QUrl;
class QString;
class QLabel;

namespace Ui
{
	class ItemWidget;
}

#include "memorycounter.h"

class ItemWidget : public QWidget
{
	friend class MainWidget;
	Q_OBJECT;

public:
	ItemWidget(QWidget *parent);


	int Init(const QString &inputString, int iPosition);

	void setImageUrl(const QUrl &url);
	void setItemUrl(const QUrl &url);
	void setItemTitle(const QString &str);
	void setItemPrice(const QString &str);
	void setItemDescription(const QString &str);
	void setItemPubTime(const QString &str);
	void setSallerImageUrl(const QUrl &url);
	void setSallerUrl(const QUrl &url);
	void setSallerName(const QString &str);
	void setSallerLivingLocation(const QString &str);
	void setSallerGrade(const QString &str);

public slots:
	void onnetworkAccessManager_finished(QNetworkReply *reply);

private:
	std::shared_ptr<Ui::ItemWidget> ui;
//	std::shared_ptr<QNetworkAccessManager> networkAccessManager;
	/* Ui::ItemWidget ui; */
	/* QNetworkAccessManager networkAccessManager; */
	/* QMap<QNetworkReply *, QLabel *> mapNetworkReplyLabel; */
	std::map<QNetworkReply *, QLabel *> mapNetworkReplyLabel;

	CMemoryCounter memoryCounter;
};

#endif
