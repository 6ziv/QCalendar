#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtSql/QSql>
#include<QtSql/QSqlDatabase>
#include<QLabel>
#include<QBitmap>
#include<QPixmap>
#include<QPainter>
#include<QMouseEvent>
#include<QFont>
#include<QFontDatabase>
#include<QTimer>
class MainWindow : public QMainWindow
{
    Q_OBJECT
private:
    QTimer *timer;
    QSqlDatabase db;
    bool init();
    void getToDoList();
    QLabel *Lab;

    QStringList Today;
    QPixmap Image;
    bool md;
    QPoint dragtmp;
    int showing;
    void ShowLable();
    QLabel *lab1;
    QLabel *lab2;
    QLabel *lab3;
    void displaylabel();
    int daysto(int n);
    int ttttt;
private slots:
    void TIMER();
public:
    QFont *font;
    int getDate();
    MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void paintEvent(QPaintEvent*);
    void mousePressEvent(QMouseEvent *e);
    void mouseReleaseEvent();
    void mouseMoveEvent(QMouseEvent *e);
    void Clean();
    void Insert(QString,const unsigned int,const bool);
    void Config(QString,const unsigned int,const bool,const int);
    void Delete(int id);

};

#endif // MAINWINDOW_H
