#include "javascriptobject.h"
#include "mainwindow.h"

/* Given: The Widget that the Object belongs to
 * Returns: None
 * Purpose: Initializes a JavascriptObject with myMainWindow connected
 * to the parent widget
 */
JavascriptObject::JavascriptObject(QWidget *parent)
{
    myMainWindow = (MainWindow *)parent;
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
    myMainWindow->javascriptClick();
}
