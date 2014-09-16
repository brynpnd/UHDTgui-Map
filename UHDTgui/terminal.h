#ifndef TERMINAL_H
#define TERMINAL_H

#include <QtCore/QtGlobal>

#include <QMainWindow>

#include <QtSerialPort/QSerialPort>

QT_USE_NAMESPACE

namespace Ui {
class Terminal;
}

class Console;
class SettingsDialog;

class Terminal : public QMainWindow
{
    Q_OBJECT

public:
    explicit Terminal(QWidget *parent = 0);
    ~Terminal();

private slots:
    void openSerialPort();
    void closeSerialPort();
    void about();
    void writeData(const QByteArray &data);
    void readData();

    void handleError(QSerialPort::SerialPortError error);

private:
    void initActionsConnections();

private:
    Ui::Terminal *ui;
    Console *console;
    SettingsDialog *settings;
    QSerialPort *serial;
};

#endif // TERMINAL_H
