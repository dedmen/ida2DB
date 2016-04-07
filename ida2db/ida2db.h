#ifndef IDA2DB_H
#define IDA2DB_H
#include <QStringList>

class Ida2db
{

	public:
		Ida2db();
		QStringList getAllNamedFunctions();
};

#endif // IDA2DB_H
