#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    readXML();
    this->setFixedSize(xmlSize["back"]["width"].toInt(),xmlSize["back"]["height"].toInt());

    QTimer *timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(update()));
    timer->start(1000);
}

MainWindow::~MainWindow()
{
    delete ui;
}

// Считывает необходимые данные из .xml
// и записывает в xmlCubes, xmlSize, xmlBackgroundCube
void MainWindow::readXML()
{
    QFile file("config.xml");
    if(!file.exists())
    {
        QMessageBox::about(NULL, tr("prompt"), tr("Файл конфигурации не найден"));
        return;
    }
    if (!file.open(QIODevice::ReadOnly)) return;
    QDomDocument Qdoc;
    if (!Qdoc.setContent(&file))
    {
        file.close();
        return ;
    }
    file.close();

    QDomNodeList list = Qdoc.elementsByTagName ("group");
    for(int i = 0; i < list.count(); ++i)
    {
        QMap<QString, QString> item;
        item.insert("color", list.at(i).toElement().attribute("color"));
        item.insert("x", list.at(i).toElement().attribute("x"));
        item.insert("y", list.at(i).toElement().attribute("y"));
        item.insert("col", list.at(i).toElement().attribute("col"));
        xmlCubes.insert(list.at(i).toElement().attribute("number"),item);
    }

    list = Qdoc.elementsByTagName("background");
    xmlBackgroundCube=list.at(0).toElement().attribute("color");

    list = Qdoc.elementsByTagName("sizes");
    QDomNodeList child = list.at(0).childNodes();
    for(int i = 0; i < child.count(); ++i)
    {
        QMap<QString, QString> item;
        item.insert("width", child.at(i).toElement().attribute("width"));
        item.insert("height", child.at(i).toElement().attribute("height"));
        xmlSize.insert(child.at(i).toElement().tagName(),item);
    }
}

// Отрисовывает данные
void MainWindow::paintEvent(QPaintEvent *)
{
    srand(time(NULL));
    QDateTime time = QDateTime::currentDateTime();
    int Hour = time.toString("hh").toInt();
    int Minute = time.toString("mm").toInt();

    // Векторы десятков и единиц часов и минут,
    // соответствующих размерам груп.
    // Рандомно заполняются "1", сумма "1" равна числу десятков и единиц.
    // Используются для закрашивания соответствующих cubes.
    QVector<int> hourTens(3);
    QVector<int> hourUnits(9);
    QVector<int> minuteTens(6);
    QVector<int> minuteUnits(9);

    for (int i=0; i<Hour/10; ++i)
    {
        int count=rand()%hourTens.size();
        while(hourTens[count]==1)
        {
            count=rand()%hourTens.size();
        }
        hourTens[count]=1;
    }

    for (int i=0; i<Hour%10; ++i)
    {
        int count=rand()%hourUnits.size();
        while(hourUnits[count]==1)
        {
            count=rand()%hourUnits.size();
        }
        hourUnits[count]=1;
    }

    for (int i=0; i<Minute/10; ++i)
    {
        int count=rand()%minuteTens.size();
        while(minuteTens[count]==1)
        {
            count=rand()%minuteTens.size();
        }
        minuteTens[count]=1;
    }

    for (int i=0; i<Minute%10; ++i)
    {
        int count=rand()%minuteUnits.size();
        while(minuteUnits[count]==1)
        {
            count=rand()%minuteUnits.size();
        }
        minuteUnits[count]=1;
    }

    QPainter painter(this);
    painter.setPen(QPen(Qt::black, 1, Qt::SolidLine, Qt::FlatCap));

    QVector<int> *pointer;
    // рисует cubes.
    //
    for (int i=1; i<=xmlCubes.size(); ++i)
    {
        int count=0;
        if(i==1) pointer=&hourTens;
        else if(i==2) pointer=&hourUnits;
        else if(i==3) pointer=&minuteTens;
        else if(i==4) pointer=&minuteUnits;

        for (int countX=0; countX<xmlCubes[QString::number(i)]["col"].toInt()*xmlSize["cube"]["width"].toInt(); countX+=xmlSize["cube"]["width"].toInt())
        {
            for (int countY=0; countY<3*xmlSize["cube"]["height"].toInt(); countY+=xmlSize["cube"]["height"].toInt())
            {
                if((*pointer)[count]==1) painter.setBrush(QBrush(QColor(xmlCubes[QString::number(i)]["color"]), Qt::SolidPattern));
                else painter.setBrush(QBrush(QColor(xmlBackgroundCube), Qt::SolidPattern));

                QPolygonF square;
                square.append(QPointF(xmlCubes[QString::number(i)]["x"].toInt()+countX,xmlCubes[QString::number(i)]["y"].toInt()+countY));
                square.append(QPointF(xmlCubes[QString::number(i)]["x"].toInt()+countX+xmlSize["cube"]["width"].toInt(),xmlCubes[QString::number(i)]["y"].toInt()+countY));
                square.append(QPointF(xmlCubes[QString::number(i)]["x"].toInt()+countX+xmlSize["cube"]["width"].toInt(),xmlCubes[QString::number(i)]["y"].toInt()+countY+xmlSize["cube"]["height"].toInt()));
                square.append(QPointF(xmlCubes[QString::number(i)]["x"].toInt()+countX,xmlCubes[QString::number(i)]["y"].toInt()+xmlSize["cube"]["height"].toInt()+countY));
                painter.drawPolygon(square);
                count++;
            }
        }
    }
}
