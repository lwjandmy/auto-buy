#ifndef MY_MAINWIDGET_H
#define MY_MAINWIDGET_H


#include "itemwidget.h"
#include <QtGui/QWidget>


/* #include <QtNetwork/QNetworkAccessManager> */
/* #include <QtNetwork/QNetworkReply> */
/* #include <QtXml/QXmlAttributes> */
/* #include <QtCore/QString> */
/* #include <QtCore/QXmlStreamReader> */
#include <memory>

#include "memorycounter.h"

class QNetworkAccessManager;
class QString;
class QNetworkReply;
class QXmlStreamReader;
class QTimer;

namespace Ui
{
	class MainWidget;
}

class MainWidget : public QWidget
{
	Q_OBJECT;

public:
	MainWidget(QWidget *parent = 0);

	bool FindAttributes_NameAndValue_Contains_CaseInsensitive(QXmlStreamReader &xmlStreamReader, const QString &name, const QString &value);

	bool FindName_Contains_CaseInsensitive(QXmlStreamReader &xmlStreamReader, const QString &name);

public slots:
	void onnetworkAccessManager_finished(QNetworkReply *reply);
	void on_startPushButton_clicked(bool checked);
	void on_timer_timeout();

private:
	std::shared_ptr<Ui::MainWidget> ui;
//	QNetworkAccessManager *networkAccessManager;
	std::shared_ptr<QTimer> timer;
	std::map<QNetworkReply *, int> mapNetworkReply;

	CMemoryCounter memoryCounter;
};

#endif
