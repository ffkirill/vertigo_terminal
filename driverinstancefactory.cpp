#include "driverinstancefactory.h"
#include "timeinforeaderdriver.h"

DriverInstanceFactory::DriverInstanceFactory(QObject *parent) :
    QObject(parent)
{
}

QVariant DriverInstanceFactory::createInstance(QString name)
{
    if (name.toLower() == "timeinfo") {
        return QVariant::fromValue(new TimeInfoReaderDriver(this));
    }
    return QVariant();
}
