#ifndef MY_ITEMCOLLECTIONWIDGET_H
#define MY_ITEMCOLLECTIONWIDGET_H

#include <QtGui/QWidget>
#include <memory>
#include <memorycounter.h>


namespace Ui
{
	class ItemCollectionWidget;
}
class QString;

class ItemCollectionWidget : public QWidget
{
friend class MainWidget;
public:
	ItemCollectionWidget(QWidget *parent = 0);
	void Init(const QString &inputString);

private:
    std::shared_ptr<Ui::ItemCollectionWidget> ui;
	CMemoryCounter memoryCounter;
};

#endif
