#ifndef SETTING_H
#define SETTING_H
#include "mainwindow.h"
#include <QDialog>
#include<QMouseEvent>
#include<QVector>
#include<QLine>
#include<QSqlQuery>
#include<QLabel>
#include<QFont>
#include<QPoint>
#include<QString>
#include<QLineEdit>
#include<QDateEdit>
#include<QDate>
class Setting : public QDialog
{
    Q_OBJECT
public:
    explicit Setting(MainWindow *parent = nullptr);
    void mousePressEvent(QMouseEvent *event);
signals:
private:
    typedef struct thing{
        QString desc;
        int date;
        bool forward;
        int id;
    }thing;
    int page;
    QVector<thing> thingstodo;
//    void getToDoList();
    void refresh(MainWindow *parent);
    void refpage();
    typedef QLabel *ptrLabel;
    ptrLabel itemdes[12];
    ptrLabel itemdat[12];
    ptrLabel itemfwd[12];
    ptrLabel itemcfm[12];
    ptrLabel itemdel[12];

    ptrLabel PNum;
    int editing;
    int getDiv(QPoint pos);
    QFont* font;
    void gotoPage(int page);
    QString fromnum(int n);
    QString DateToStr(int num);
    QString tmpdesc;
    int tmpdate;
    bool tmpforward;
    QLineEdit *tmpLE;
    QDateEdit *tmpDE;

public slots:
};

#endif // SETTING_H
