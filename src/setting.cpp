#include "setting.h"
#include<QDebug>
#include<windows.h>
Setting::Setting(MainWindow *parent) : QDialog(parent)
{
    editing=-1;
    QPixmap bkgnd(":/image/book.png");
    this->resize(bkgnd.size());
    //bkgnd = bkgnd.scaled(this->size(), Qt::IgnoreAspectRatio);
    QPalette palette;
    palette.setBrush(QPalette::Background, bkgnd);
    this->setPalette(palette);
    qDebug()<<"show";
    this->show();
    refresh(parent);
    page=0;
    int id = QFontDatabase::addApplicationFont(":/font/test.ttf");
    QString family = QFontDatabase::applicationFontFamilies(id).at(0);
    font = new QFont(family);

    this->setFont(*font);
    for(int i=0;i<12;i++){
        itemdat[i]=new QLabel(this);
        itemdes[i]=new QLabel(this);
        itemfwd[i]=new QLabel(this);
        itemcfm[i]=new QLabel(this);
        itemdel[i]=new QLabel(this);


        itemdes[i]->setGeometry(75,32*i+75,200,20);
        itemdat[i]->setGeometry(300,32*i+75,100,20);
        itemfwd[i]->setGeometry(425,32*i+75,50,20);
        itemcfm[i]->setGeometry(500,32*i+75,30,20);
        itemdel[i]->setGeometry(550,32*i+75,30,20);


        itemdes[i]->show();
        itemdat[i]->show();
        itemfwd[i]->show();

        itemcfm[i]->setText("保存");
        itemdel[i]->setText("删除");
        itemcfm[i]->hide();
        itemdel[i]->hide();
    }
    itemfwd[1]->setText("提前提醒");
    itemfwd[1]->update();
    PNum=new QLabel(this);
    QFont tmp=PNum->font();
    tmp.setPointSize(20);
    PNum->setFont(tmp);
    PNum->setGeometry(75,32*15+75,100,30);
    PNum->show();

    tmpLE=new QLineEdit(this);
    tmpDE=new QDateEdit(this);
    tmpLE->hide();
    tmpDE->hide();
    gotoPage(0);
}
void Setting::refresh(MainWindow *parent){
    thingstodo.clear();
    parent->Clean();
    int tod=parent->getDate();
    QSqlQuery query;
    query.prepare("SELECT * from Events WHERE time>=:val ORDER BY time");
    query.bindValue(":val",tod);
    query.exec();
    query.next();
    while(query.isValid()){
        thing tmp;
        tmp.desc=query.value(0).toString();
        tmp.date=query.value(1).toInt();
        tmp.forward=query.value(2).toBool();
        tmp.id=query.value(3).toInt();

        thingstodo.push_back(tmp);
        query.next();
    }
    thing tmp;
    tmp.desc="";
    tmp.date=0;
    tmp.forward=false;
    tmp.id=-1;
    thingstodo.push_back(tmp);

    for(int i=0;i<thingstodo.size();i++){
        qDebug()<<">>"<<thingstodo.at(i).desc<<" "<<thingstodo.at(i).date<<" "<<thingstodo.at(i).forward;
    }
    return;
}
void Setting::gotoPage(int Page){
    qDebug()<<"Page"<<Page;
    if(Page<0)return;
    if(12*Page>=thingstodo.size())return;
    page=Page;
    for(int i=0;i<12;i++){
        if(page*12+i>=thingstodo.size()){
            qDebug()<<"hide"<<i;
            itemdes[i]->setText("");
            itemdat[i]->setText("");
            itemfwd[i]->setText("");
            itemdes[i]->hide();
            itemdat[i]->hide();
            itemfwd[i]->hide();

        }else
        {
            qDebug()<<"show"<<i;

            itemdes[i]->setText(thingstodo.at(12*page+i).desc);
            itemdes[i]->show();
            itemdat[i]->setText(DateToStr(thingstodo.at(12*page+i).date));
            itemdat[i]->show();
            if(thingstodo.at(12*page+i).forward){
                qDebug()<<"+"<<QString::number(i);
                itemfwd[i]->setText(QString("提前提醒"));
                itemfwd[i]->show();
                itemfwd[i]->update();
                qDebug()<<itemfwd[i]->text();
            }
            else{
                qDebug()<<"-"<<QString::number(i);
                itemfwd[i]->setText("");
                itemfwd[i]->hide();
            }
            qDebug()<<itemfwd[i]->text();
        }
        qDebug()<<itemfwd[i]->text();
    }
    qDebug()<<"<<<>>>";
    for(int i=0;i<12;i++)
        qDebug()<<itemfwd[i]->text();
    qDebug()<<"PAGE"<<page+1;
    PNum->setText("第"+fromnum(page+1)+"页");

}
int Setting::getDiv(QPoint pos){
    if((pos.x()+pos.y())>1100)return -2;
    if((pos.x()>350)&&(pos.y()>500)&&(pos.x()+pos.y()<1050))return -3;
    if(pos.x()<70)
        return -1;
    if(pos.x()>500)
        return -1;
    for(int i=0;i<12;i++){
        if((pos.y()>=32*i+75)&&(pos.y()<=32*i+95))
            return i;
    }

    return -1;
}
QString Setting::fromnum(int n){
    if(n==10)return QString("十");
    const QString table="零一二三四五六七八九";
    QString tmp1,tmp2;
    if(n>=100000000)return fromnum(n/100000000)+QString("亿")+fromnum(n%100000000);
    if(n>=10000)
    {
        if(n%10000>=1000)return fromnum(n/10000)+QString("万")+fromnum(n%10000);
        else return fromnum(n/10000)+QString("万零")+fromnum(n%10000);
    }
    QString tmp="";
    if(n>=1000){
        tmp=tmp+table.at(n/1000)+QString("千");
        n=n%1000;
        if((n<100)&&(n!=0))tmp+=QString("零");
    }
    if(n>=100){
        tmp=tmp+table.at(n/100)+QString("百");
        n=n%100;
        if((n<10)&&(n!=0))tmp+=QString("零");
    }
    if(n>=10){
        tmp=tmp+table.at(n/10)+QString("十");
        n=n%10;
    }
    if(n>=1)
        tmp=tmp+table.at(n);
    return tmp;
}


QString Setting::DateToStr(int num){
    if(num==0)return "新建事件";
    int y,m,d,t;
    d=(num-1)%31+1;
    t=(num-d)/31;
    m=(t-1)%12+1;
    y=(t-m)/12;
    return QString::number(y)+"/"+QString::number(m)+"/"+QString::number(d);
}
void Setting::mousePressEvent(QMouseEvent *event){
    if(editing==-1){
        qDebug()<<"intoe";
        int m=getDiv(event->pos());
        if(m==-2)gotoPage(page+1);
        else if(m==-3)gotoPage(page-1);
        else if((m>=0)&&(12*page+m<thingstodo.size())){

            editing=m;
            itemdel[editing]->show();
            itemcfm[editing]->show();

            itemdes[editing]->hide();
            itemdat[editing]->hide();

            tmpLE->setGeometry(75,32*editing+75,200,20);
            tmpDE->setGeometry(300,32*editing+75,100,20);
            tmpLE->show();
            tmpDE->show();
            tmpLE->setText(thingstodo[12*page+editing].desc);
            int ttt=thingstodo[12*page+editing].date;
            QDate tmpda;
            if(ttt)
            {
                int y,m,d,t;

                d=(ttt-1)%31+1;
                t=(ttt-d)/31;
                m=(t-1)%12+1;
                y=(t-m)/12;

                tmpda.setDate(y,m,d);
            }
            else{
                tmpda=QDate::currentDate();
            }

            tmpDE->setDate(tmpda);
            tmpdesc=thingstodo[12*page+editing].desc;
            tmpdate=thingstodo[12*page+editing].date;
            tmpforward=thingstodo[12*page+editing].forward;
        }
        event->accept();
    }else{
        if((event->pos().y()<32*editing+75)||(event->pos().y()>32*editing+95)){
            itemdel[editing]->hide();
            itemcfm[editing]->hide();
            itemdes[editing]->show();
            itemdat[editing]->show();
            tmpDE->hide();
            tmpLE->hide();
            itemdes[editing]->setText(thingstodo.at(12*page+editing).desc);
            itemdat[editing]->setText(DateToStr(thingstodo.at(12*page+editing).date));
            if(thingstodo.at(12*page+editing).forward){
                itemfwd[editing]->setText("提前提醒");
                itemfwd[editing]->show();
            }else{
                itemfwd[editing]->setText("");
                itemfwd[editing]->hide();
            }

            editing=-1;
        }else{
            if((event->pos().x()>=500)&&(event->pos().x()<=530))
            {
                qDebug()<<"save";
                itemdat[editing]->show();
                itemdes[editing]->show();
                tmpDE->hide();
                tmpLE->hide();

                itemdel[editing]->hide();
                itemcfm[editing]->hide();

                MainWindow *parent=(MainWindow *)this->parent();
                parent->Config(tmpLE->text(),(unsigned int)31*(12*tmpDE->date().year()+tmpDE->date().month())+tmpDE->date().day(),tmpforward,thingstodo.at(12*page+editing).id);
                qDebug()<<tmpdesc;
                refresh(parent);
                gotoPage(page);
                editing=-1;
            }
            else if((event->pos().x()>=550)&&(event->pos().x()<=580))
            {
                qDebug()<<"del";

                MainWindow *parent=(MainWindow *)this->parent();
                parent->Delete(thingstodo[12*page+editing].id);
                itemdel[editing]->hide();
                itemcfm[editing]->hide();
                tmpLE->hide();
                tmpDE->hide();
                editing=-1;

                refresh(parent);
                gotoPage(page);
            }

            else if((event->pos().x()>=425)&&(event->pos().x()<=470)){
                tmpforward=!tmpforward;
                if(tmpforward){
                    itemfwd[editing]->setText("提前提醒");
                    itemfwd[editing]->show();
                }else{
                    itemfwd[editing]->setText("");
                    itemfwd[editing]->hide();
                }
            }
        }

    }
}
