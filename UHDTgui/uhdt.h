#ifndef UHDT_H
#define UHDT_H

#include <QObject>

class UHDT : public QObject
{
    Q_OBJECT
public:
    explicit UHDT(QObject *parent = 0);
    int getAirspeed();
    int getAltitude();
    void setAirspeed(int a);
    void setAltitude(int a);

private:
    int airspeed;
    int altitude;

signals:

public slots:

};

#endif // UHDT_H
