#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QFile>
#include <QMessageBox>
#include <QDomDocument>
#include <QDebug>
#include <QTime>
#include <QTimer>
#include <QPainter>

QMap<QString, QMap<QString, QString>> xmlElements;
QMap<QString, QString> xmlSize;
QString xmlBackground;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    readXML();

    //QTimer *timer = new QTimer(this);
    //connect(timer, SIGNAL(timeout()), this, SLOT(update()));
   // timer->start(1000);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::readXML()
{
    QFile file("config.xml");
    QDomDocument Qdoc;
    if (!Qdoc.setContent(&file))
    {
        file.close();
        return ;
    }
    file.close();

    QDomNodeList list = Qdoc.elementsByTagName ("root");
    QDomNodeList child = list.at(0).childNodes();
    for(int i = 0; i < child.count(); ++i)
    {
        if(child.at(i).toElement().tagName()=="group")
        {
            QMap<QString, QString> item;
            item.insert("color", child.at(i).toElement().attribute("color"));
            item.insert("x", child.at(i).toElement().attribute("x"));
            item.insert("y", child.at(i).toElement().attribute("y"));
            xmlElements.insert(child.at(i).toElement().tagName()+child.at(i).toElement().attribute("number"),item);
        }
    }

    list = Qdoc.elementsByTagName("background");
    xmlBackground=list.at(0).toElement().attribute("color");

    list = Qdoc.elementsByTagName("cube");
    xmlSize.insert("width", list.at(0).toElement().attribute("width"));
    xmlSize.insert("height", list.at(0).toElement().attribute("height"));

    qDebug()<<xmlElements;
}

void MainWindow::paintEvent(QPaintEvent *)
{
    QDateTime time = QDateTime::currentDateTime();
    QString text = time.toString("hh")+" ";
   text += time.toString("mm")+" ";
    text += time.toString("ss");


    ui->label->setText(text);



    QPainter painter(this);
    painter.setPen(QPen(Qt::black, 1, Qt::SolidLine, Qt::FlatCap));


    for (int countY=0; countY<3*xmlSize["height"].toInt(); countY+=xmlSize["height"].toInt())
    {
        QPolygonF Rectangle;
        Rectangle.append(QPointF(xmlElements["group1"]["x"].toInt(),xmlElements["group1"]["y"].toInt()+countY));
        Rectangle.append(QPointF(xmlElements["group1"]["x"].toInt()+xmlSize["width"].toInt(),xmlElements["group1"]["y"].toInt()+countY));
        Rectangle.append(QPointF(xmlElements["group1"]["x"].toInt()+xmlSize["width"].toInt(),xmlElements["group1"]["y"].toInt()+countY+xmlSize["height"].toInt()));
        Rectangle.append(QPointF(xmlElements["group1"]["x"].toInt(),xmlElements["group1"]["y"].toInt()+xmlSize["height"].toInt()+countY));
        painter.drawPolygon(Rectangle);
    }

    for (int countX=0; countX<xmlSize["width"].toInt()*3; countX+=xmlSize["width"].toInt())
    {
        for (int countY=0; countY<3*xmlSize["height"].toInt(); countY+=xmlSize["height"].toInt())
        {
            QPolygonF Rectangle;
            Rectangle.append(QPointF(xmlElements["group2"]["x"].toInt()+countX,xmlElements["group2"]["y"].toInt()+countY));
            Rectangle.append(QPointF(xmlElements["group2"]["x"].toInt()+countX+xmlSize["width"].toInt(),xmlElements["group2"]["y"].toInt()+countY));
            Rectangle.append(QPointF(xmlElements["group2"]["x"].toInt()+countX+xmlSize["width"].toInt(),xmlElements["group2"]["y"].toInt()+countY+xmlSize["height"].toInt()));
            Rectangle.append(QPointF(xmlElements["group2"]["x"].toInt()+countX,xmlElements["group2"]["y"].toInt()+xmlSize["height"].toInt()+countY));
            painter.drawPolygon(Rectangle);
        }
    }


    for (int countX=0; countX<2*xmlSize["width"].toInt(); countX+=xmlSize["width"].toInt())
    {
        for (int countY=0; countY<3*xmlSize["height"].toInt(); countY+=xmlSize["height"].toInt())
        {
            QPolygonF Rectangle;
            Rectangle.append(QPointF(xmlElements["group3"]["x"].toInt()+countX,xmlElements["group3"]["y"].toInt()+countY));
            Rectangle.append(QPointF(xmlElements["group3"]["x"].toInt()+countX+xmlSize["width"].toInt(),xmlElements["group3"]["y"].toInt()+countY));
            Rectangle.append(QPointF(xmlElements["group3"]["x"].toInt()+countX+xmlSize["width"].toInt(),xmlElements["group3"]["y"].toInt()+countY+xmlSize["height"].toInt()));
            Rectangle.append(QPointF(xmlElements["group3"]["x"].toInt()+countX,xmlElements["group3"]["y"].toInt()+xmlSize["height"].toInt()+countY));
            painter.drawPolygon(Rectangle);
        }
    }



    for (int countX=0; countX<3*xmlSize["width"].toInt(); countX+=xmlSize["width"].toInt())
    {
        for (int countY=0; countY<3*xmlSize["height"].toInt(); countY+=xmlSize["height"].toInt())
        {
            if(countY==2*xmlSize["height"].toInt() && countX==1*xmlSize["width"].toInt()) painter.setBrush(QBrush(Qt::green, Qt::SolidPattern));
            else painter.setBrush(QBrush());

            QPolygonF Rectangle;
            Rectangle.append(QPointF(xmlElements["group4"]["x"].toInt()+countX,xmlElements["group4"]["y"].toInt()+countY));
            Rectangle.append(QPointF(xmlElements["group4"]["x"].toInt()+countX+xmlSize["width"].toInt(),xmlElements["group4"]["y"].toInt()+countY));
            Rectangle.append(QPointF(xmlElements["group4"]["x"].toInt()+countX+xmlSize["width"].toInt(),xmlElements["group4"]["y"].toInt()+countY+xmlSize["height"].toInt()));
            Rectangle.append(QPointF(xmlElements["group4"]["x"].toInt()+countX,xmlElements["group4"]["y"].toInt()+xmlSize["height"].toInt()+countY));
            painter.drawPolygon(Rectangle);
        }
    }

 // painter.drawPoint(xmlElements["group1"]["x"].toInt(), xmlElements["group1"]["y"].toInt());

}
