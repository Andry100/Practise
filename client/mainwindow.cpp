#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    connect(socket,SIGNAL(readyRead()),this,SLOT(sockReady()));
    connect(socket,SIGNAL(disconnected()),this,SLOT(sockDisc()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_clicked()
{
    socket->connectToHost("194.44.220.91",4423);
}

void MainWindow::sockDisc()
{
    socket -> deleteLater();
}

void MainWindow::sockReady()
{
    if(socket->waitForConnected(500))
    {
        socket->waitForReadyRead(500);
        Data = socket->readAll();
        qDebug() << Data;

        doc = QJsonDocument::fromJson(Data, &docError);

        if(docError.errorString().toInt()==QJsonParseError::NoError)
        {
            if((doc.object().value("type").toString() == "connect") && (doc.object().value("status").toString()== "yes"))
            {
                QMessageBox::information(this, "Information", " Connecting ");
            }
            else if(doc.object().value("type").toString()=="resultSelect")
            {
                QStandardItemModel* model = new QStandardItemModel(nullptr);
                model->setHorizontalHeaderLabels(QStringList()<<"name");

                QJsonArray docAr = doc.object().value("result").toArray();
                for(int i = 0; i < docAr.count(); i++)
                {
                    QStandardItem* col = new QStandardItem(docAr[i].toObject().value("name").toString());
                    model->appendRow(col);
                }
            }
            else
            {
                QMessageBox::information(this, "Information", " Don`t connect ");
            }
        }
        else
        {
            QMessageBox::information(this,"Information"," Mistake with data ",docError.errorString());
        }
    }
}
void MainWindow::on_pushButton_2_clicked()
{
    if(socket->isOpen())
    {
        socket->write("{\"type\":\"select\",\"params\":\"workers\"}");
        socket->waitForBytesWritten(500);
    }
    else
    {
        QMessageBox::iformation(this,"information","Don`t connect");
    }
}

