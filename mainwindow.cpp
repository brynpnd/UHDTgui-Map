#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "container.h"

/* To Do:
   - Can't input anything else when inputting no-fly zone
   - Removing boundary/no-fly zone
   - Editting boundary/no-fly zone upon double-click
   - Cancel button when making no-fly zone
   - Mouse marker when clicked once
 */

#define WAYPOINT 0
#define BOUNDARY 1
#define NOFLYZONE 2

/* Given: The widget that the object belongs to
 * Returns: None
 * Purpose: Initializes the MainWindow object with the boundary input
 * and no-fly zone input components hidden, and the boundary and no-fly zone
 * coordinate containers empty. This function also opens the JavaScript file
 * and links its contents to a QWebView object
 */

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    boundIndex = 0;
    noFlyIndex = 0;

    QString content;
    QString fileName = ":/index.html";
    QFile file(fileName);

    if( file.open(QFile::ReadOnly) ){
        content = QString(file.readAll());
    }else{
        QMessageBox::information(this, tr("GPS Mapper"),
                                      tr("Cannot load %1.").arg(fileName));
        qFatal("Error opening index html file.");
    }
    file.close();

    QWebView *frame;

    frame = ui->webView;
    frame->page()->mainFrame()->setScrollBarPolicy(Qt::Vertical, Qt::ScrollBarAlwaysOff);
    frame->page()->mainFrame()->setScrollBarPolicy(Qt::Horizontal, Qt::ScrollBarAlwaysOff);
    frame->setHtml(content);

    myObject = new JavascriptObject(this);
    frame->page()->mainFrame()->addToJavaScriptWindowObject("myObject",myObject);

    // hide boundary input
    hideBoundaryInput();
    hideNoFlyInput();

}

// Deconstructor: Deletes the Ui pointer to prevent any memory leaks
MainWindow::~MainWindow()
{
    delete ui;
}

/* Slot function:
 * Signal: PutMarker QPushButton being pressed
 * Purpose: Add a marker onto the map by calling the markers JavaScript function
 */
void MainWindow::on_PutMarker_clicked()
{
    QString marker = QString("markers(%1,%2,%3,'You clicked me'); null")
            .arg(myObject->getLatLng().rx())
            .arg(myObject->getLatLng().ry())
            .arg(WAYPOINT);

    ui->webView->page()->mainFrame()->evaluateJavaScript(marker);
}

/* Slot function:
 * Signal: ClearMarker QPushButton being pressed
 * Purpose: Clears all markers on the map by calling the removemarkers JavaScript function
 */
void MainWindow::on_ClearMarker_clicked()
{
    QString marker = QString("removemarkers(%1); null")
            .arg(WAYPOINT);
    ui->webView->page()->mainFrame()->evaluateJavaScript(marker);
}

/* Given: None
 * Returns: None
 * Purpose: Display the latitude and longitude of the clicked location
 */
void MainWindow::javascriptClick(void){
    ui->lat->setText(QString::number(myObject->getLatLng().rx()));
    ui->lng->setText(QString::number(myObject->getLatLng().ry()));
}

/* Slot function:
 * Signal: DrawShape QPushButton being pressed
 * Purpose: Activates components used to implement the boundary
 */
void MainWindow::on_DrawShape_clicked()
{
    QString latline = ui->lat->text();
    QString longline = ui->lng->text();

    showBoundaryInput();



    // add current coordinates label
}

/* Given: None
 * Returns: None
 * Purpose: Hides the components used to implement the boundary
 */
void MainWindow::hideBoundaryInput() {
    ui->latBound->setHidden(true);
    ui->longBound->setHidden(true);
    ui->addBound->setHidden(true);
    ui->confirmBound->setHidden(true);
    ui->undoBound->setHidden(true);
    ui->copyBound->setHidden(true);
    ui->inputLatLabel->setHidden(true);
    ui->inputLongLabel->setHidden(true);
    ui->boundCoordinates->setHidden(true);
    ui->cancelBound->setHidden(true);

    ui->DrawShape->setHidden(false);

    ui->PutMarker->setEnabled(true);
    ui->ClearMarker->setEnabled(true);
    ui->DrawNoFly->setEnabled(true);
}

/* Given: None
 * Returns: None
 * Purpose: Shows the components used to implement the boundary
 */
void MainWindow::showBoundaryInput() {
    ui->latBound->setHidden(false);
    ui->longBound->setHidden(false);
    ui->addBound->setHidden(false);
    ui->confirmBound->setHidden(false);
    ui->undoBound->setHidden(false);
    ui->copyBound->setHidden(false);
    ui->inputLatLabel->setHidden(false);
    ui->inputLongLabel->setHidden(false);
    ui->boundCoordinates->setHidden(false);
    ui->cancelBound->setHidden(false);

    ui->DrawShape->setHidden(true);

    ui->PutMarker->setEnabled(false);
    ui->ClearMarker->setEnabled(false);
    ui->DrawNoFly->setEnabled(false);

    ui->boundCoordinates->setText("No boundary coordinates input.");



}

/* Given: None
 * Returns: None
 * Purpose: Hides the components used to implement the no-fly zone
 */
void MainWindow::hideNoFlyInput() {
    ui->latNoFly->setHidden(true);
    ui->lngNoFly->setHidden(true);
    ui->addNoFly->setHidden(true);
    ui->confirmNoFly->setHidden(true);
    ui->undoNoFly->setHidden(true);
    ui->copyNoFly->setHidden(true);
    ui->inputLatLabel_2->setHidden(true);
    ui->inputLongLabel_2->setHidden(true);
    ui->boundCoordinates->setHidden(true);
    ui->DrawNoFly->setEnabled(true);
}

/* Given: None
 * Returns: None
 * Purpose: Shows the components used to implement the no-fly zone
 */
void MainWindow::showNoFlyInput() {
    ui->latNoFly->setHidden(false);
    ui->lngNoFly->setHidden(false);
    ui->addNoFly->setHidden(false);
    ui->confirmNoFly->setHidden(false);
    ui->undoNoFly->setHidden(false);
    ui->copyNoFly->setHidden(false);
    ui->inputLatLabel_2->setHidden(false);
    ui->inputLongLabel_2->setHidden(false);
    ui->boundCoordinates->setHidden(false);
    ui->DrawNoFly->setEnabled(false);
    ui->boundCoordinates->setText("No no-fly zone coordinates input.");
}

/* Slot function:
 * Signal: addBound QPushButton being pressed
 * Purpose: Checks if the coordinates inputted are valid and adds them to their appropriate
 * containers. This function also adds a marker at the inputted coordinates and draws
 * a polyline when there is more than one point by calling javascript functions.
 */
void MainWindow::on_addBound_clicked()
{
    QString lat = ui->latBound->text();
    QString lng = ui->longBound->text();

    int errorlat = 0;
    int errorlong = 0;

    // check if coordinate is in correct format
    QRegExp rx = QRegExp("^[-+]?([0-9]|[1-9][0-9]|[1][0-7][0-9])(\\.[1-9]|.[0-9]{1,5}[1-9])?$");

    if(rx.exactMatch(lat))
        errorlat = 0;

    else if(lat.toInt() == 180)
        errorlat = 0;

    else if(lat.toInt() == -180)
        errorlat = 0;

    else
        errorlat = 1;

    if(rx.exactMatch(lng))
        errorlong = 0;

    else if(lng.toInt() == 180)
        errorlong = 0;

    else if(lng.toInt() == -180)
        errorlong = 0;

    else
        errorlong = 1;

    if(errorlat == 0 && errorlong == 0) {
        float latnum = lat.toFloat();
        float longnum = lng.toFloat();
        boundLats.insert(latnum, boundIndex);
        boundLongs.insert(longnum, boundIndex);

        QString boundmarker = QString("markers(%1,%2,%3,'You clicked me'); null")
                .arg(boundLats.get(boundIndex))
                .arg(boundLongs.get(boundIndex))
                .arg(BOUNDARY);

        //        .arg(boundIndex+1);


         ui->webView->page()->mainFrame()->evaluateJavaScript(boundmarker);

         // draw boundary line between last two points
         if(boundIndex > 0) {
             QString boundline = QString("drawPolyLine(%1,%2,%3,%4,%5,%6); null")
                 .arg(boundLats.get(boundIndex))
                 .arg(boundLongs.get(boundIndex))
                 .arg(boundLats.get(boundIndex-1))
                 .arg(boundLongs.get(boundIndex-1))
                 .arg(BOUNDARY)
                 .arg(boundIndex);

             ui->webView->page()->mainFrame()->evaluateJavaScript(boundline);
         }


        boundIndex++;
        listBoundCoordinates();
    }

    // append function?
    else if(errorlat == 1 && errorlong == 0)
        ui->boundCoordinates->append("Invalid latitude.");

    else if(errorlong == 1 && errorlat == 0)
        ui->boundCoordinates->append("Invalid longitude.");

    else
        ui->boundCoordinates->append("Invalid latitude and longitude.");



}

/* Given: None
 * Returns: None
 * Purpose: Lists all the coordinates in their container
 */
void MainWindow::listBoundCoordinates() {
    QString coordinates;
    for(int i = 0; i < boundIndex; i++) {
        QString appendStr = QString("Point %1: %2, %3\n")
                .arg(i+1)
                .arg(boundLats.get(i))
                .arg(boundLongs.get(i));
        coordinates.append(appendStr);
    }

    ui->boundCoordinates->setText(coordinates);
}

/* Slot function:
 * Signal: copyBound QPushButton being pressed
 * Purpose: Copies the coordinates from the clicked location to their respective containers.
 * This function also adds a marker at the copied coordinates and draws
 * a polyline when there is more than one point by calling javascript functions
 */
void MainWindow::on_copyBound_clicked()
{
    QString lat = ui->lat->text();
    QString lng = ui->lng->text();

    // insert function?
    boundLats.insert(lat.toFloat(),boundIndex);
    boundLongs.insert(lng.toFloat(),boundIndex);

    QString boundmarker = QString("markers(%1,%2,%3,'You clicked me'); null")
            .arg(boundLats.get(boundIndex))
            .arg(boundLongs.get(boundIndex))
            .arg(BOUNDARY);

     ui->webView->page()->mainFrame()->evaluateJavaScript(boundmarker);

     // draw boundary line between last two points
     if(boundIndex > 0) {
         QString boundline = QString("drawPolyLine(%1,%2,%3,%4,%5); null")
             .arg(boundLats.get(boundIndex))
             .arg(boundLongs.get(boundIndex))
             .arg(boundLats.get(boundIndex-1))
             .arg(boundLongs.get(boundIndex-1))
             .arg(BOUNDARY);

         ui->webView->page()->mainFrame()->evaluateJavaScript(boundline);
     }


     boundIndex++;

     listBoundCoordinates();
}

/* Slot function:
 * Signal: undoBound QPushButton being pressed
 * Purpose: remove the last inputted coordinates from their container and the map
 */
void MainWindow::on_undoBound_clicked()
{
    // remove latest latitude and longitude
    if (boundIndex == 0)
        ui->boundCoordinates->append("No points left to remove.");

    // remove one marker function
    else {
        boundIndex--;
        boundLats.remove(boundIndex);
        boundLongs.remove(boundIndex);
        listBoundCoordinates();

        QString removeLast = QString("removeLastMarker(%1,%2); null")
                .arg(BOUNDARY)
                .arg(boundIndex);
        ui->webView->page()->mainFrame()->evaluateJavaScript(removeLast);
    }
}

/* Slot function:
 * Signal: confirmBound QPushButton being pressed
 * Purpose: Hides the components used to implement a boundary and connects the last inputted point
 * to the first point
 */
void MainWindow::on_confirmBound_clicked()
{
    hideBoundaryInput();

/*
    // draw boundary from last point to first point
    QString boundline = QString("drawPolyLine(%1,%2,%3,%4,%5,%6); null")
            .arg(boundLats.get(0))
            .arg(boundLongs.get(0))
            .arg(boundLats.get(boundIndex-1))
            .arg(boundLongs.get(boundIndex-1))
            .arg(BOUNDARY)
            .arg(boundIndex);

    ui->webView->page()->mainFrame()->evaluateJavaScript(boundline);
*/


    // draw polygon
    QString drawPoly = QString("drawPolygon(%1); null")
            .arg(BOUNDARY);
    ui->webView->page()->mainFrame()->evaluateJavaScript(drawPoly);

    // clear lat and long boundary arrays
    QString clearbounds = QString("removemarkers(%1); null")
            .arg(BOUNDARY);

    ui->webView->page()->mainFrame()->evaluateJavaScript(clearbounds);

    clearBoundCoordinates();
}

/* Given: None
 * Returns: None
 * Purpose: Clears the containers used to hold the boundary coordinates
 */
void MainWindow::clearBoundCoordinates() {
    boundLats.clear();
    boundLongs.clear();
    boundIndex = 0;
}

/* Slot function:
 * Signal: DrawNoFly QPushButton being pressed
 * Purpose: Reveals the components used to implement a no-fly zone
 */
void MainWindow::on_DrawNoFly_clicked()
{
    showNoFlyInput();
}


void MainWindow::on_addNoFly_clicked()
{
    QString lat = ui->latNoFly->text();
    QString lng = ui->lngNoFly->text();

    int errorlat = 0;
    int errorlong = 0;

    // check if coordinate is in correct format
    QRegExp rx = QRegExp("^[-+]?([0-9]|[1-9][0-9]|[1][0-7][0-9])(\\.[1-9]|.[0-9]{1,5}[1-9])?$");

    if(rx.exactMatch(lat))
        errorlat = 0;

    else if(lat.toInt() == 180)
        errorlat = 0;

    else if(lat.toInt() == -180)
        errorlat = 0;

    else
        errorlat = 1;

    if(rx.exactMatch(lng))
        errorlong = 0;

    else if(lng.toInt() == 180)
        errorlong = 0;

    else if(lng.toInt() == -180)
        errorlong = 0;

    else
        errorlong = 1;

    if(errorlat == 0 && errorlong == 0) {
        float latnum = lat.toFloat();
        float longnum = lng.toFloat();
        noFlyLats.insert(latnum, noFlyIndex);
        noFlyLongs.insert(longnum, noFlyIndex);

        QString noflymarker = QString("markers(%1,%2,%3,'You clicked me'); null")
                .arg(noFlyLats.get(noFlyIndex))
                .arg(noFlyLongs.get(noFlyIndex))
                .arg(NOFLYZONE);

         ui->webView->page()->mainFrame()->evaluateJavaScript(noflymarker);

         // draw a no fly zone line between last two points
         if(noFlyIndex > 0) {
             QString noflyline = QString("drawPolyLine(%1,%2,%3,%4,%5); null")
                 .arg(noFlyLats.get(noFlyIndex))
                 .arg(noFlyLongs.get(noFlyIndex))
                 .arg(noFlyLats.get(noFlyIndex-1))
                 .arg(noFlyLongs.get(noFlyIndex-1))
                 .arg(NOFLYZONE);

             ui->webView->page()->mainFrame()->evaluateJavaScript(noflyline);
         }


        noFlyIndex++;
        listNoFlyCoordinates();
    }

    // append function?
    else if(errorlat == 1 && errorlong == 0)
        ui->boundCoordinates->append("Invalid latitude.");

    else if(errorlong == 1 && errorlat == 0)
        ui->boundCoordinates->append("Invalid longitude.");

    else
        ui->boundCoordinates->append("Invalid latitude and longitude.");
}

void MainWindow::on_undoNoFly_clicked()
{
    // remove latest latitude and longitude
    if (noFlyIndex == 0)
        ui->boundCoordinates->append("No points left to remove.");

    // remove one marker function
    else {
        noFlyIndex--;
        noFlyLats.remove(noFlyIndex);
        noFlyLongs.remove(noFlyIndex);

        listNoFlyCoordinates();

        QString removeLast = QString("removeLastMarker(%1,%2); null")
                .arg(NOFLYZONE)
                .arg(noFlyIndex);
        ui->webView->page()->mainFrame()->evaluateJavaScript(removeLast);
    }
}

void MainWindow::on_copyNoFly_clicked()
{
    QString lat = ui->lat->text();
    QString lng = ui->lng->text();

    // insert function?
    noFlyLats.insert(lat.toFloat(),noFlyIndex);
    noFlyLongs.insert(lng.toFloat(),noFlyIndex);

    QString noFlymarker = QString("markers(%1,%2,%3,'You clicked me'); null")
            .arg(noFlyLats.get(noFlyIndex))
            .arg(noFlyLongs.get(noFlyIndex))
            .arg(NOFLYZONE);

     ui->webView->page()->mainFrame()->evaluateJavaScript(noFlymarker);

     // draw no fly zone line between last two points
     if(noFlyIndex > 0) {
         QString noFlyline = QString("drawPolyLine(%1,%2,%3,%4,%5); null")
             .arg(noFlyLats.get(noFlyIndex))
             .arg(noFlyLongs.get(noFlyIndex))
             .arg(noFlyLats.get(noFlyIndex-1))
             .arg(noFlyLongs.get(noFlyIndex-1))
             .arg(NOFLYZONE);

         ui->webView->page()->mainFrame()->evaluateJavaScript(noFlyline);
     }


     noFlyIndex++;

     listNoFlyCoordinates();
}

void MainWindow::on_confirmNoFly_clicked()
{
    hideNoFlyInput();
    ui->DrawShape->setEnabled(true);

    // draw a no-fly zone line from last point to first point
    QString noFlyline = QString("drawPolyLine(%1,%2,%3,%4,%5); null")
            .arg(noFlyLats.get(noFlyIndex-1))
            .arg(noFlyLongs.get(noFlyIndex-1))
            .arg(noFlyLats.get(0))
            .arg(noFlyLongs.get(0))
            .arg(NOFLYZONE);

    ui->webView->page()->mainFrame()->evaluateJavaScript(noFlyline);

    // draw polygon
    QString drawPoly = QString("drawPolygon(%1); null")
            .arg(NOFLYZONE);
    ui->webView->page()->mainFrame()->evaluateJavaScript(drawPoly);

    // clear lat and long no fly zone arrays
    QString clearnofly = QString("removemarkers(%1); null")
            .arg(NOFLYZONE);

    ui->webView->page()->mainFrame()->evaluateJavaScript(clearnofly);

    clearNoFlyCoordinates();
}

void MainWindow::listNoFlyCoordinates() {
    QString coordinates;
    for(int i = 0; i < noFlyIndex; i++) {
        QString appendStr = QString("Point %1: %2, %3\n")
                .arg(i+1)
                .arg(noFlyLats.get(i))
                .arg(noFlyLongs.get(i));
        coordinates.append(appendStr);
    }

    ui->boundCoordinates->setText(coordinates);
}

void MainWindow::clearNoFlyCoordinates() {
    noFlyLats.clear();
    noFlyLongs.clear();
    noFlyIndex = 0;
}


void MainWindow::on_cancelBound_clicked()
{
    hideBoundaryInput();
    clearBoundCoordinates();

    QString removeLines = QString("removeLines();");
    ui->webView->page()->mainFrame()->evaluateJavaScript(removeLines);

    QString removeMarkers = QString("removemarkers(%1); null")
            .arg(BOUNDARY);
    ui->webView->page()->mainFrame()->evaluateJavaScript(removeMarkers);

    QString removePoly = QString("removePolyCoords();");
    ui->webView->page()->mainFrame()->evaluateJavaScript(removePoly);
}
