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

    QTimer *timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(update()));
    timer->start(1000);
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
    srand(time(NULL));

    QDateTime time = QDateTime::currentDateTime();
    int Hour = time.toString("hh").toInt();
    int Minute = time.toString("mm").toInt();

    QVector<int> hourTens(3);
    QVector<int> hourUnits(9);
    QVector<int> minuteTens(6);
    QVector<int> minuteUnits(9);

    for (int i=0; i<Hour/10; ++i)
    {
        int count=rand()%3;
        while(hourTens[count]==1)
        {
            count=rand()%3;
        }
        hourTens[count]=1;
    }

    for (int i=0; i<Hour%10; ++i)
    {
        int count=rand()%9;
        while(hourUnits[count]==1)
        {
            count=rand()%9;
        }
        hourUnits[count]=1;
    }

    for (int i=0; i<Minute/10; ++i)
    {
        int count=rand()%6;
        while(minuteTens[count]==1)
        {
            count=rand()%6;
        }
        minuteTens[count]=1;
    }

    for (int i=0; i<Minute%10; ++i)
    {
        int count=rand()%9;
        while(minuteUnits[count]==1)
        {
            count=rand()%9;
        }
        minuteUnits[count]=1;
    }

    qDebug()<<"hourTens";
    QString str;
    for (int i=0; i<hourTens.size(); ++i)
    {
        str+=QString::number(hourTens[i])+" ";
    }
    qDebug()<<str;
    str="";
    qDebug()<<"hourUnits";
    for (int i=0; i<hourUnits.size(); ++i)
    {
        str+=QString::number(hourUnits[i])+" ";
    }
    qDebug()<<str;
    str="";
    qDebug()<<"minuteTens";
    for (int i=0; i<minuteTens.size(); ++i)
    {
        str+=QString::number(minuteTens[i])+" ";
    }
    qDebug()<<str;
    str="";
    qDebug()<<"minuteUnits";
    for (int i=0; i<minuteUnits.size(); ++i)
    {
        str+=QString::number(minuteUnits[i])+" ";
    }
    qDebug()<<str;


    QPainter painter(this);
    painter.setPen(QPen(Qt::black, 1, Qt::SolidLine, Qt::FlatCap));

    int i=0;
    for (int countY=0; countY<3*xmlSize["height"].toInt(); countY+=xmlSize["height"].toInt())
    {
        if(hourTens[i]==1) painter.setBrush(QBrush(QColor(xmlElements["group1"]["color"]), Qt::SolidPattern));
        else painter.setBrush(QBrush());

        QPolygonF square;
        square.append(QPointF(xmlElements["group1"]["x"].toInt(),xmlElements["group1"]["y"].toInt()+countY));
        square.append(QPointF(xmlElements["group1"]["x"].toInt()+xmlSize["width"].toInt(),xmlElements["group1"]["y"].toInt()+countY));
        square.append(QPointF(xmlElements["group1"]["x"].toInt()+xmlSize["width"].toInt(),xmlElements["group1"]["y"].toInt()+countY+xmlSize["height"].toInt()));
        square.append(QPointF(xmlElements["group1"]["x"].toInt(),xmlElements["group1"]["y"].toInt()+xmlSize["height"].toInt()+countY));
        painter.drawPolygon(square);

        i++;
    }
    i=0;

    for (int countX=0; countX<xmlSize["width"].toInt()*3; countX+=xmlSize["width"].toInt())
    {
        for (int countY=0; countY<3*xmlSize["height"].toInt(); countY+=xmlSize["height"].toInt())
        {
            if(hourUnits[i]==1) painter.setBrush(QBrush(QColor(xmlElements["group2"]["color"]), Qt::SolidPattern));
            else painter.setBrush(QBrush());

            QPolygonF square;
            square.append(QPointF(xmlElements["group2"]["x"].toInt()+countX,xmlElements["group2"]["y"].toInt()+countY));
            square.append(QPointF(xmlElements["group2"]["x"].toInt()+countX+xmlSize["width"].toInt(),xmlElements["group2"]["y"].toInt()+countY));
            square.append(QPointF(xmlElements["group2"]["x"].toInt()+countX+xmlSize["width"].toInt(),xmlElements["group2"]["y"].toInt()+countY+xmlSize["height"].toInt()));
            square.append(QPointF(xmlElements["group2"]["x"].toInt()+countX,xmlElements["group2"]["y"].toInt()+xmlSize["height"].toInt()+countY));
            painter.drawPolygon(square);

            i++;
        }
    }
    i=0;


    for (int countX=0; countX<2*xmlSize["width"].toInt(); countX+=xmlSize["width"].toInt())
    {
        for (int countY=0; countY<3*xmlSize["height"].toInt(); countY+=xmlSize["height"].toInt())
        {
            if(minuteTens[i]==1) painter.setBrush(QBrush(QColor(xmlElements["group3"]["color"]), Qt::SolidPattern));
            else painter.setBrush(QBrush());

            QPolygonF square;
            square.append(QPointF(xmlElements["group3"]["x"].toInt()+countX,xmlElements["group3"]["y"].toInt()+countY));
            square.append(QPointF(xmlElements["group3"]["x"].toInt()+countX+xmlSize["width"].toInt(),xmlElements["group3"]["y"].toInt()+countY));
            square.append(QPointF(xmlElements["group3"]["x"].toInt()+countX+xmlSize["width"].toInt(),xmlElements["group3"]["y"].toInt()+countY+xmlSize["height"].toInt()));
            square.append(QPointF(xmlElements["group3"]["x"].toInt()+countX,xmlElements["group3"]["y"].toInt()+xmlSize["height"].toInt()+countY));
            painter.drawPolygon(square);
            i++;
        }
    }
    i=0;

    for (int countX=0; countX<3*xmlSize["width"].toInt(); countX+=xmlSize["width"].toInt())
    {
        for (int countY=0; countY<3*xmlSize["height"].toInt(); countY+=xmlSize["height"].toInt())
        {
            if(minuteUnits[i]==1) painter.setBrush(QBrush(QColor(xmlElements["group4"]["color"]), Qt::SolidPattern));
            else painter.setBrush(QBrush());

            QPolygonF square;
            square.append(QPointF(xmlElements["group4"]["x"].toInt()+countX,xmlElements["group4"]["y"].toInt()+countY));
            square.append(QPointF(xmlElements["group4"]["x"].toInt()+countX+xmlSize["width"].toInt(),xmlElements["group4"]["y"].toInt()+countY));
            square.append(QPointF(xmlElements["group4"]["x"].toInt()+countX+xmlSize["width"].toInt(),xmlElements["group4"]["y"].toInt()+countY+xmlSize["height"].toInt()));
            square.append(QPointF(xmlElements["group4"]["x"].toInt()+countX,xmlElements["group4"]["y"].toInt()+xmlSize["height"].toInt()+countY));
            painter.drawPolygon(square);

            i++;
        }
    }
}
