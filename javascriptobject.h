#ifndef JAVASCRIPTOBJECT_H
#define JAVASCRIPTOBJECT_H

#include <QObject>
#include <QPointF>

class Map;

class JavascriptObject : public QObject
{
    Q_OBJECT

public:
    JavascriptObject(QWidget *parent);
    QPointF getLatLng();


public slots:
   void set(float lat, float lng);

private:
   QPointF mVar;
   Map *myMap;

};

#endif // JAVASCRIPTOBJECT_H
