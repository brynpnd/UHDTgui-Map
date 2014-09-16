#include "uhdt.h"

UHDT::UHDT(QObject *parent) :
    QObject(parent)
{
    altitude = 0;
    airspeed = 0;
}

int UHDT::getAltitude() {
    return altitude;
}

int UHDT::getAirspeed() {
    return airspeed;
}

void UHDT::setAltitude(int a) {
    altitude = a;
}

void UHDT::setAirspeed(int a) {
    airspeed = a;
}
