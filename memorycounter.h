#ifndef MY_MEMORYCOUNTER_H
#define MY_MEMORYCOUNTER_H

#include <QtCore/QtDebug>
#include <cstring>

class CMemoryCounter
{
public:
	CMemoryCounter(const char *chName_)
	{
		std::strcpy(chName, chName_);

		qDebug() << "MemoryCounter: " << chName << " " << count << "+1=" << count + 1;
		++count;
	}

	~CMemoryCounter()
	{
		qDebug() << "MemoryCounter: " << chName << " " << count << "-1=" << count - 1;
		--count;
	}
private:
	char chName[100];
	static int count;
};



#endif
