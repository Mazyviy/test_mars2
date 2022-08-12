#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFile>
#include <QDomDocument>
#include <QDateTime>
#include <QTimer>
#include <QPainter>
#include <QMessageBox>
#include <QMap>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void readXML();
    void paintEvent(QPaintEvent *);

private:
    QMap<QString, QMap<QString, QString>> xmlCubes, xmlSize;
    QString xmlBackgroundCube;
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
