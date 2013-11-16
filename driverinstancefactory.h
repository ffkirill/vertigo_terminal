#ifndef DRIVERINSTANCEFACTORY_H
#define DRIVERINSTANCEFACTORY_H

#include <QObject>
#include <QVariant>

class DriverInstanceFactory : public QObject
{
    Q_OBJECT
public:
    explicit DriverInstanceFactory(QObject *parent = 0);

signals:

public slots:
    QVariant createInstance(QString name);

};

#endif // DRIVERINSTANCEFACTORY_H
