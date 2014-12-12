#include "javascriptobject.h"
#include "map.h"

/* Given: The Widget that the Object belongs to
 * Returns: None
 * Purpose: Initializes a JavascriptObject with myMap connected
 * to the parent widget
 */
JavascriptObject::JavascriptObject(QWidget *parent)
{
    myMap = (Map *)parent;
}

/* Given: None
 * Returns: QPointF Object containing coordinates
 * Purpose: Allows the user to obtain the latitude and longitude clicked on the JavascriptObject
 */
QPointF JavascriptObject::getLatLng() {
    return mVar;
}

/* Given: Latitude and Longitude
 * Returns: None
 * Purpose: Set the latitude and longitude of the map
 */
void JavascriptObject::set(float lat, float lng){
    mVar.setX(lat);
    mVar.setY(lng);
    myMap->javascriptClick();
}
