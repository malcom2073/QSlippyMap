#include "datasource.h"

DataSource::DataSource()
{
	qRegisterMetaType<DataClass>("DataClass");
}
