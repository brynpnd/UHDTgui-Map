#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QString>
#include <QTableWidgetItem>
#include <QtSerialPort/QSerialPort>

enum MyItemType {INT = 1, STRING = 2, DOUBLE = 3 };

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ui->textBrowser->setMaximumHeight(400);
    ui->textBrowser->setMaximumWidth(400);

    // set tab labels
    ui->tabWidget->setTabText(0, "Drone");
    ui->tabWidget->setTabText(1, "Target Data");
    ui->tabWidget->setMaximumHeight(400);
    ui->tabWidget->setMaximumWidth(400);

    // set numeric display fonts 
    QFont font = QFont("Trebuchet");
    font.setPointSize(14);

    QFont font2 = QFont("Trebuchet");
    font2.setPointSize(30);
    float testnum = 0.0;
    QString str = QString::number(testnum,'f',2);

    // set hard-coded data display
    ui->label->setFont(font);
    ui->label->setText("Altitude (feet/MSL)");
    ui->label->setAlignment(Qt::AlignCenter);
    ui->label_2->setFont(font2);
    ui->label_2->setText(str);
    ui->label_2->setAlignment(Qt::AlignCenter);

    ui->label_3->setFont(font);
    ui->label_3->setText("Airspeed (KIAS)");
    ui->label_3->setAlignment(Qt::AlignCenter);
    ui->label_4->setFont(font2);
    ui->label_4->setText(str);
    ui->label_4->setAlignment(Qt::AlignCenter);

    ui->label_5->setFont(font);
    ui->label_5->setText("Latitude (degrees)");
    ui->label_5->setAlignment(Qt::AlignCenter);
    ui->label_6->setFont(font2);
    ui->label_6->setText(str);
    ui->label_6->setAlignment(Qt::AlignCenter);

    ui->label_7->setFont(font);
    ui->label_7->setText("Longitude (degrees)");
    ui->label_7->setAlignment(Qt::AlignCenter);
    ui->label_8->setFont(font2);
    ui->label_8->setText(str);
    ui->label_8->setAlignment(Qt::AlignCenter);

    QString test = " test";
    QTableWidgetItem *item1 = new QTableWidgetItem(test, INT);
    item1->setFlags(Qt::ItemIsEditable);
    item1->setFlags(Qt::NoItemFlags);
    item1->setTextColor(Qt::black);

    ui->targetTable->horizontalHeader()->setStretchLastSection(true);
    ui->targetTable->horizontalHeader()->setHighlightSections(false);
    ui->targetTable->setCornerButtonEnabled(false);

  //  serial = new QSerialPort(this);
   // connect(serial, SIGNAL(readyRead()), this, SLOT(readData()));

    // set up terminal button
    terminal = new Terminal();

    ui->actionOpenTerminal->setEnabled(true);
    connect(ui->actionOpenTerminal, SIGNAL(triggered()), terminal, SLOT(show()));
}
/*
MainWindow::addNewTarget() {

}
*/

void MainWindow::sendData() {


}


MainWindow::~MainWindow()
{
    delete ui;
}
