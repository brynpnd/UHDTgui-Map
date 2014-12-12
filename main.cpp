#include <QApplication>
#include "map.h"


/* Given: Any arguments passed through a command line
 * Returns: Whether or not the application quit via error or exit button
 * Purpose: main() is the first function that is opened when the GUI is launched.
 * It displays the GUI application maximized.
 */
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Map w;
    w.setWindowTitle("DroneTech");
    w.showMaximized();
    return a.exec();
}
