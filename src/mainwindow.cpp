#include "mainwindow.h"
#include"setting.h"
#include<QDebug>
#include<QtSql/QSqlError>
#include<Qtsql/QSqlQuery>
#include<windows.h>
#include<stdlib.h>
#include<QDir>
int MainWindow::getDate(){
    SYSTEMTIME time;
    GetLocalTime(&time);
    return 31*(12*time.wYear+time.wMonth)+time.wDay;
}
bool MainWindow::init(){
    QString path;
    wchar_t tmp[1024];

    QString strpath=QString::fromWCharArray(tmp);
    db= QSqlDatabase::addDatabase("QSQLITE");

    db.setDatabaseName(QDir::homePath()+"/"+"TimeLine.db");
    bool ok = db.open();
    if(!ok)return false;
    QSqlQuery query;

    ok=query.exec("CREATE TABLE IF NOT EXISTS EVENTS ("
                  "desc VARCHAR(1024),"
                  "time INT UNSIGNED,"
                  "forward BOOLEAN,"
                  "ID INTEGER PRIMARY KEY   AUTOINCREMENT);");
    if(!ok)return false;
    return true;
}

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{

    HANDLE hMutex=CreateMutexW(NULL,FALSE,L"QCalendar_MUTEX");
    DWORD dwRet=GetLastError();
    if (hMutex)
    {
        if (ERROR_ALREADY_EXISTS == dwRet)
        {
            this->close();
            return;
        }
    }


    if(!init()){
        MessageBoxW(0,L"数据库错误>_<",L"出错啦",MB_OK);
        return;
    }


    Image.load(":/image/board.png");
    Image=Image.scaled(160,280);

    this->resize(Image.size());
    this->setMask(Image.mask());
    lab1=new QLabel(this);
    lab2=new QLabel(this);
    lab3=new QLabel(this);

    lab1->setAlignment(Qt::AlignCenter);
    lab2->setAlignment(Qt::AlignCenter);
    lab3->setAlignment(Qt::AlignCenter);


    int id = QFontDatabase::addApplicationFont(":/font/test.ttf");
    QString family = QFontDatabase::applicationFontFamilies(id).at(0);
    font = new QFont(family);

    Clean();
    ShowLable();
    timer=new QTimer(this);
    timer->setSingleShot(false);
    timer->setInterval(60000);
    ttttt=getDate();
    this->connect(timer,SIGNAL(timeout()),this,SLOT(TIMER()));
    this->setWindowFlags(this->windowFlags()|Qt::Tool|Qt::WindowStaysOnTopHint);
    this->show();

}
void MainWindow::TIMER(){
    if(getDate()!=ttttt){
        Clean();
        ShowLable();
        ttttt=getDate();
    }
}
void MainWindow::ShowLable(){
    getToDoList();
    if(Today.size()==0){
        QSqlQuery query;
        int tod=getDate();
        query.prepare("SELECT * FROM Events WHERE (time = ( SELECT MIN(time) FROM Events WHERE (time>:val AND forward)) AND forward);");
        query.bindValue(":val",tod);
        query.exec();
        query.next();
        if(query.isValid()){

            font->setPointSize(10);
            lab1->setFont(*font);
            lab1->setAlignment(Qt::AlignCenter);
            lab1->setGeometry(35,150,100,20);



            QString strtmp=query.value(0).toString();
            if(strtmp.length()<=4)
                lab1->setText(QString::fromWCharArray(L"距离")+strtmp+QString::fromWCharArray(L"还有"));
            else if(strtmp.length()<=6)
                lab1->setText(QString::fromWCharArray(L"距离")+strtmp);
            else{
                lab1->setText(QString::fromWCharArray(L"距离")+strtmp.left(strtmp.length()/2-1));
                lab1->setGeometry(35,140,100,20);
                }
            lab1->show();


            font->setPointSize(15);
            lab2->setFont(*font);
            lab2->setGeometry(35,180,100,20);
            lab2->setAlignment(Qt::AlignCenter);
            if(strtmp.length()<=4){
                lab2->setText(QString::number(daysto(query.value(1).toInt()))+"天");
                lab3->hide();
            }
            else if(strtmp.length()<=6){
                lab2->setText("还有"+QString::number(daysto(query.value(1).toInt()))+"天");
                lab3->hide();
            }
            else if(strtmp.length()<=6){
                font->setPointSize(10);
                lab2->setFont(*font);
                lab2->setGeometry(35,155,100,20);
                lab2->setText(strtmp.left(strtmp.length()/2+1));

                font->setPointSize(15);
                lab3->setFont(*font);
                lab3->setGeometry(35,180,100,20);
                lab3->setText("还有"+QString::number(daysto(query.value(1).toInt()))+"天");
                lab3->show();
            }
            lab2->show();

        }else{
            font->setPointSize(10);
            lab1->setFont(*font);
            lab1->setAlignment(Qt::AlignCenter);
            lab1->setGeometry(35,150,100,20);
            lab1->setText("今天没有安排哦");
            lab1->show();
            font->setPointSize(15);
            lab2->setFont(*font);
            lab2->setGeometry(35,180,100,20);
            lab2->setAlignment(Qt::AlignCenter);
            lab2->setText("休息一下吧");
            lab2->show();
            lab3->hide();
        }
    }else{
        displaylabel();

    }
}
void MainWindow::displaylabel(){
    font->setPointSize(10);
    lab1->setFont(*font);
    lab1->setAlignment(Qt::AlignCenter);
    lab1->setGeometry(35,150,100,20);
    if(showing==0)
        lab1->setText("今天请务必记得");
    else
        lab1->setText("也不要忘了");
    lab1->show();
    QString msg=Today.at(showing);
    if(msg.length()<=4){

        font->setPointSize(15);
        lab2->setFont(*font);
        lab2->setGeometry(35,180,100,20);
        lab2->setText(msg);
        lab2->show();
        lab3->hide();
    }else if(msg.length()<=8){
        font->setPointSize(10);
        lab2->setFont(*font);
        lab2->setGeometry(35,180,100,20);
        lab2->setText(msg);
        lab2->show();
        lab3->hide();
    }else{
        lab1->setGeometry(35,140,100,20);
        font->setPointSize(10);
        lab2->setFont(*font);
        lab2->setGeometry(35,155,100,20);
        lab2->setText(msg.left(8));
        lab2->show();

        font->setPointSize(10);
        lab3->setFont(*font);
        lab3->setGeometry(35,170,100,20);
        lab3->setText(msg.right(msg.length()-8));
        lab3->show();

    }
}
void MainWindow::getToDoList(){
    Today.clear();
    int tod=getDate();
    QSqlQuery query;
    query.prepare("SELECT * from Events WHERE time=:val");
    query.bindValue(":val",tod);
    query.exec();
    query.next();
    while(query.isValid()){

        Today.push_back(query.value(0).toString());
        query.next();
    }
}
void MainWindow::mousePressEvent(QMouseEvent *e){
    if(e->button()==Qt::LeftButton){
        md=true;
        dragtmp=e->globalPos();
        if(Today.length()!=0){
            showing++;
            if(showing>=Today.length())showing=0;
            displaylabel();
        }
    }
    if(e->button()==Qt::RightButton){
        Setting setting(this);
        setting.exec();
        ShowLable();
    }
    e->accept();
}
void MainWindow::mouseReleaseEvent(){
    md=false;
}
void MainWindow::mouseMoveEvent(QMouseEvent *e){
    if(md){
        this->move(this->pos()-dragtmp+e->globalPos());
        dragtmp=e->globalPos();
    }
    e->accept();
}
void MainWindow::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    painter.drawPixmap(0, 0, Image);
}
int MainWindow::daysto(int n){
    QDate d1=QDate::currentDate();
    QDate d2;
    int y,m,d,t;

    d=(n-1)%31+1;
    t=(n-d)/31;
    m=(t-1)%12+1;
    y=(t-m)/12;

    d2.setDate(y,m,d);

    return (int)(d1.daysTo(d2));
}
void MainWindow::Clean(){
    QSqlQuery query;
    int date;
    date=getDate();
    query.prepare("DELETE from Events WHERE time<:val");
    query.bindValue(":val",date);
    query.exec();
}
void MainWindow::Insert(QString description,const unsigned int date,const bool forward){
    QSqlQuery query;

    std::string tmp=description.toStdString();
    const char *desc=tmp.c_str();
    query.prepare("INSERT INTO Events(desc,time,forward) values(:des,:dat,:frd)");
    query.bindValue(":des",desc);
    query.bindValue(":dat",date);
    query.bindValue(":frd",forward);
    query.exec();
}
void MainWindow::Delete(int id){
    if(id==-1)return;
    QSqlQuery query;
    query.prepare("Delete from Events where id=:key");
    query.bindValue(":key",id);
    query.exec();
}
void MainWindow::Config(QString description,const unsigned int date,const bool forward,const int id){
    if(id==-1)Insert(description,date,forward);
    QSqlQuery query;
    std::string tmp=description.toStdString();
    const char *desc=tmp.c_str();
    query.prepare("UPDATE Events SET desc=:des, time=:dat,forward=:frd  WHERE id=:key;");
    query.bindValue(":des",desc);
    query.bindValue(":dat",date);
    query.bindValue(":frd",forward);
    query.bindValue(":key",id);
    query.exec();
}

MainWindow::~MainWindow()
{

}
