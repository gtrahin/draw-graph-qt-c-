#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    graph * g  = new graph();
    QHBoxLayout * layout =  new QHBoxLayout();
    QWidget * mainFrame = new QWidget();

    layout->addWidget(g);
    mainFrame->setLayout(layout);


    this->setCentralWidget(mainFrame);
    this->setMinimumSize(700,600);
    this->setWindowTitle("graph");
}

MainWindow::~MainWindow()
{
    delete ui;
}
