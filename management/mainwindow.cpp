#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    Init();
}
void MainWindow::Init()
{

    mDefaultDB=QCoreApplication::applicationDirPath() + "\\Customer.db";
    //初始化区域
    mAllType<<QStringLiteral("企业")<<QStringLiteral("高校")<<QStringLiteral("研究所")<<QStringLiteral("代理")<<QStringLiteral("外")<<QStringLiteral("政府机构")<<QStringLiteral("部队")<<QStringLiteral("第三方");
    mAllArea<<QStringLiteral("华东")<<QStringLiteral("华南")<<QStringLiteral("华中")<<QStringLiteral("华北")<<QStringLiteral("其他")<<QStringLiteral("外");
    mAllProvince<<QStringLiteral("山东")<<QStringLiteral("浙江")<<QStringLiteral("江苏")<<QStringLiteral("安徽")<<QStringLiteral("上海")<<QStringLiteral("福建")
                <<QStringLiteral("广东")<<QStringLiteral("广西")<<QStringLiteral("海南")
                <<QStringLiteral("湖北")<<QStringLiteral("湖南")<<QStringLiteral("河南")<<QStringLiteral("江西")
                <<QStringLiteral("北京")<<QStringLiteral("天津")<<QStringLiteral("河北")<<QStringLiteral("山西")<<QStringLiteral("内蒙")
                <<QStringLiteral("宁夏")<<QStringLiteral("新疆")<<QStringLiteral("青海")<<QStringLiteral("陕西")<<QStringLiteral("甘肃")<<QStringLiteral("四川")<<QStringLiteral("云南")<<QStringLiteral("贵州")<<QStringLiteral("西藏")<<QStringLiteral("重庆")<<QStringLiteral("辽宁")<<QStringLiteral("吉林")<<QStringLiteral("黑龙江");
    mFilterType="";
    mFilterArea="";
    mFilterProvince="";
    mFilterKey="";

    InitMenu();
    OpenDb(mDefaultDB);
    InitTable();
    ui->treeWidget->expandAll();//扩展所有可扩展项目。
    ui->treeWidget->setCurrentItem(ui->treeWidget->itemAt(0,0));//itemAt(0,0)返回指向坐标（x，y）处的项的指针并将这个最小项设置为当前项

    ui->comboBox->setView(new QListView());//组合框弹出窗口中使用的视图设置为给定的itemView

}
MainWindow::~MainWindow()
{
    delete ui;
}
void MainWindow::InitMenu()
{
    QMenu *file=menuBar()->addMenu("&file");
    file->addAction("open...",this,SLOT(onMenuOpen(bool)));
    file->addAction("save",this,SLOT(onMenuSave(bool)));
}
void MainWindow::InitModel()
{
    mModel->removeRows(0,mModel->rowCount());//清除指定的行列
    for(int i=0;i<mItems.size();i++)
    {
        Customer* p=mItems[i];
        QString sArea=Area2String(p->area);
        QString sProvince=Province2String(p->province);
        QString sType=Type2String(p->type);

        //区域过滤
        if(!mFilterArea.isEmpty())
        {
            if(mFilterArea!=sArea)
                continue;
        }
        //省份过滤
        if(!mFilterProvince.isEmpty())
        {
            if(mFilterProvince!=sProvince)
                continue;
        }
        //单位类型过滤
        if(!mFilterType.isEmpty())
        {
            if(mFilterType!=sType)
                continue;
        }
        //关键字过滤
        if(!mFilterKey.isEmpty())
        {
            QStringList strs;
            strs<<p->name<<sType<<sArea<<sProvince<<p->city<<p->site<<p->contact<<p->buy<<p->research<<p->remark;
            bool bFind=false;
            foreach (QString s, strs)
            {
                if(s.indexOf(mFilterKey)!=-1)//返回此字符串中第一次出现的字符串str的索引位置,找不到返回-1
                {
                    bFind=true;
                    break;
                }
            }
            if(!bFind)
                continue;
        }
        QList<QStandardItem *>items;
        QStandardItem *item1=new QStandardItem(p->name);
        QStandardItem *item2=new QStandardItem(Type2String(p->type));
        QStandardItem *item3=new QStandardItem(Area2String(p->area));
        QStandardItem *item4=new QStandardItem(Province2String(p->province));
        QStandardItem *item5=new QStandardItem(p->city);
        QStandardItem *item6=new QStandardItem(p->address);
        QStandardItem *item7=new QStandardItem(p->site);
        QStandardItem *item8=new QStandardItem(p->research);
        QStandardItem *item9=new QStandardItem(p->remark);
        items.append(item1);
        items.append(item2);
        items.append(item3);
        items.append(item4);
        items.append(item5);
        items.append(item6);
        items.append(item7);
        items.append(item8);
        items.append(item9);
        mModel->appendRow(items);
    }
}
void MainWindow::InitTable()
{
    QTableView *t=ui->tableView;
    t->setEditTriggers(QTreeView::NoEditTriggers);//tableview内容不可编辑(就是添加之后固定了不可改变)
    t->setSelectionBehavior(QTreeView::SelectRows);//一次选中整行(还可以选中一个项目或一整列)
    t->setAlternatingRowColors(true);//保持是否使用交替颜色绘制背景。

    QString strTreeStyle = "QTableView::item {height: 25px}";
    t->setStyleSheet(strTreeStyle);//设置表格风格
    QStringList headers;
    headers<< QStringLiteral("单位名称")<<QStringLiteral("单位类型")
            << QStringLiteral("区域")<<QStringLiteral("省份")
            << QStringLiteral("城市")<<QStringLiteral("地址")
            << QStringLiteral("网址")<<QStringLiteral("研究方向")
            << QStringLiteral("备注");
    mModel=new QStandardItemModel(t);
    mModel->setHorizontalHeaderLabels(headers);//使用标签设置水平标题标签。如有必要，列数会增加到标签的大小。

    InitModel();//对Model进行检查过滤
    t->setModel(mModel);//设置要显示的视图的模型。
    for(int i=0;i<headers.size();i++)
    {
        if(i==0)
        {
            t->horizontalHeader()->resizeSection(i,200);//返回表格表头(行表头)并设置每一个不同元素的列宽
        }
        else if(i==1)
        {
            t->horizontalHeader()->resizeSection(i,80);
        }
        else if(i>=2 && i<=4)
        {
            t->horizontalHeader()->resizeSection(i,60);
        }
        else
        {
            t->horizontalHeader()->resizeSection(i,150);
        }
    }
    QModelIndex rootIndex=t->rootIndex();//返回模型根项的模型索引。根项是视图顶层项的父项
    QModelIndex selIndex=mModel->index(0,0,rootIndex);//返回给定行，列和父索引指定的模型中项的索引。
    t->setCurrentIndex(selIndex);//将当前项设置为索引处的项。
}
void MainWindow::addItem(Customer *item)
{
    mItems.push_back(item);
}
void MainWindow::ClearData()
{
    for(int i=0;i<mItems.size();i++)
    {
        Customer *p=mItems[i];
        if(p)
        {
            delete p;//删除该内存
            p=NULL;
        }
    }
    mItems.clear();//除所有元素并释放向量使用的内存

}
void MainWindow::DeleteItem(QString name)
{
    for(int i=0;i<mItems.size();i++)
    {
        if(mItems[i]->name==name)
        {
            mItems.removeAt(i);
            break;
        }
    }
}
void MainWindow::onMenuOpen(bool checked)
{
    QString filename=QFileDialog::getOpenFileName(this,QStringLiteral("打开文件"),"","Database Files(*.db)");
    if(filename.isNull())
        return;
    if(filename.indexOf(".db")!=-1)
    {
        ClearData();
        OpenDb(filename);
        InitTable();//重新设置Table表格
    }
}
void MainWindow::onMenuSave(bool checked)
{
    QString fileName=QFileDialog::getSaveFileName(this,QStringLiteral("保存到文件"),"","Database Files(*.db)");
    if(fileName.isNull())
        return;
    if(QFile::exists(fileName))//如果fileName指定的文件存在，则返回true;
    {
        QFile::remove(fileName);//删除给定的fileName指定的文件
    }
    SaveToDb(fileName);
}
void MainWindow::OpenDb(QString filename)
{
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(filename);//将连接的数据库名称设置为name
    bool ok = db.open();
    if(ok)
    {
        qDebug()<<"Database Opened";
        mDefaultDB=filename;
        QSqlQuery sql_query;
        QString select_all_sql="select * from Customer";

        //查询所有记录(准备SQL查询在下面exec()执行)
        sql_query.prepare(select_all_sql);
        if(!sql_query.exec())//执行以前准备的SQL查询
        {
            //qDebug()<<sql_query.lastError();//返回查询错误
        }
        else
        {
            while(sql_query.next())//检索结果中的下一条记录（如果可用），并将查询定位在检索到的记录上
            {
                Customer* p = new Customer;
                p->name = sql_query.value(0).toString();
                p->type = sql_query.value(1).toInt();
                p->area = sql_query.value(2).toInt();
                p->province = sql_query.value(3).toInt();
                p->city = sql_query.value(4).toString();
                p->address = sql_query.value(5).toString();
                p->site = sql_query.value(6).toString();
                p->contact = sql_query.value(7).toString();
                p->buy = sql_query.value(8).toString();
                p->research = sql_query.value(9).toString();
                p->remark = sql_query.value(10).toString();
                mItems.push_back(p);
            }
        }
    }
    db.close();
}
void MainWindow::SaveToDb(QString filename)
{
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(filename);
    bool ok = db.open();
    if(ok)
    {
        qDebug()<<"Database Opened";

        QSqlQuery sql_query;//sql_query用于执行sql的语句

        //if not exists Customer如果不存在Customer这个表。
        //含有中文字符，用nchar/nvarchar，如果纯英文和数字，用char/varchar
        //这里\是为了不含多的"(如果没\回车会出现多余的")
        QString create_sql = "create table if not exists Customer (name varchar(100) primary key, \
                                                    type int ,\
                                                    area int,\
                                                    province int,\
                                                    city varchar(20),\
                                                    address varchar(100),\
                                                    site varchar(200),\
                                                    contact varchar(500),\
                                                    buy varchar(200),\
                                                    research varchar(100),\
                                                    remark varchar(200))"; //创建数据表
        QString insert_sql = "insert into Customer values(\"%1\",%2,%3,%4,\"%5\",\"%6\",\"%7\",\"%8\",\"%9\",\"%10\",\"%11\")";    //插入数据
        QString select_all_sql="select * from Customer";//查看表

        sql_query.prepare(create_sql);//创建表
        if(!sql_query.exec())//执行上述创建语句，检查是否创建成功。
        {
            qDebug()<<QObject::tr("Table Create failed");//表创建失败
            qDebug()<<sql_query.lastError();
        }
        else
        {
            qDebug()<<"Table create success";

            //创建成功后,开始插入数据
            for(int i=0;i<mItems.size();i++)
            {
                Customer* p=mItems[i];
                QString sql=insert_sql;
                sql.replace("%10",p->research);//字符串替换 把%10的部分替换成对应的数据
                sql.replace("%11",p->remark);//替换成备注
                sql=sql.arg(p->name).arg(p->type).arg(p->area).arg(p->province).arg(p->city).arg(p->address).arg(p->site).arg(p->contact).arg(p->buy);
                sql_query.prepare(sql);//准备插入数据(执行上面的insert语句)
                if(!sql_query.exec())//开始执行语句
                {
                    qDebug()<<sql_query.lastError();
                }
                else
                {
                    qDebug()<<"插入记录成功";
                }
            }
        }
    }
    db.close();
}
void MainWindow::InsertToDb(Customer *p)
{
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(mDefaultDB);
    if(db.open())
    {
        qDebug()<<"Database Opened";

        QSqlQuery sql_query;
        QString create_sql = "create table if not exists Customer (name varchar(100) primary key, \
                                                    type int ,\
                                                    area int,\
                                                    province int,\
                                                    city varchar(20),\
                                                    address varchar(100),\
                                                    site varchar(200),\
                                                    contact varchar(500),\
                                                    buy varchar(200),\
                                                    research varchar(100),\
                                                    remark varchar(200))"; //创建数据表
        sql_query.prepare(create_sql);
        sql_query.exec();                   //创建表

        QString sql = "insert into Customer values(\"%1\",%2,%3,%4,\"%5\",\"%6\",\"%7\",\"%8\",\"%9\",\"%10\",\"%11\")";    //插入数据
        sql.replace("%10",p->research);
        sql.replace("%11",p->remark);
        sql = sql.arg(p->name).arg(p->type).arg(p->area).arg(p->province).arg(p->city).arg(p->address).arg(p->site).arg(p->contact).arg(p->buy);
        sql_query.prepare(sql);
        if(!sql_query.exec())
        {
            qDebug()<<sql_query.lastError();
        }
        else
        {
            qDebug()<<"插入记录成功";
        }
    }
    db.close();

}
void MainWindow::DeleteFromDb(QString name)
{
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(mDefaultDB);
    bool ok = db.open();
    if(ok)
    {
        qDebug()<<"Database Opened";//数据库打开

        QSqlQuery sql_query;//sql_query用于执行sql的语句
        QString sql=QString("delete from Customer where name='%1'").arg(name);//待执行的sqlite语句
        sql_query.prepare(sql);//准备开始执行指定的sql语句
        if(!sql_query.exec())//开始执行
        {
            qDebug()<<sql_query.lastError();
        }
        else
        {
            qDebug()<<"删除记录成功";
        }

    }
    db.close();

}
//增加按钮
void MainWindow::on_btn_add_clicked()
{
    dlgitem d(NULL);
    //connect(&d,SIGNAL(sigaddItem(Customer*)),this,SLOT(doProcessSigaddItem(Customer *p)));
    connect(&d,&dlgitem::sigaddItem,this,&MainWindow::doProcessSigaddItem);
    d.exec();
}
//删除按钮
void MainWindow::on_btn_delete_clicked()
{
    int row=ui->tableView->currentIndex().row();
    if(row==-1)
        return;
    QString name=mModel->item(row,0)->text();
    DeleteItem(name);//1.UItable表中删除该数据

    mModel->removeRow(row);//2.还需要在模型中删除(上面只是单纯的删除的是显示界面)
    DeleteFromDb(name);//3.还需要把数据库的该项数据删除
}
void MainWindow::doProcessSigaddItem(Customer *p)
{

    mItems.push_back(p);
    qDebug()<<"good";
    QList<QStandardItem *> items;
    QStandardItem *item1=new QStandardItem(p->name);
    QStandardItem *item2=new QStandardItem(Type2String(p->type));
    QStandardItem *item3=new QStandardItem(Area2String(p->area));
    QStandardItem *item4=new QStandardItem(Province2String(p->province));
    QStandardItem *item5=new QStandardItem(p->city);
    QStandardItem *item6=new QStandardItem(p->address);
    QStandardItem *item7=new QStandardItem(p->site);
    QStandardItem *item8=new QStandardItem(p->research);
    QStandardItem *item9=new QStandardItem(p->remark);
    items.append(item1);
    items.append(item2);
    items.append(item3);
    items.append(item4);
    items.append(item5);
    items.append(item6);
    items.append(item7);
    items.append(item8);
    items.append(item9);
    mModel->appendRow(items);

    InsertToDb(p);


}
QString MainWindow::Type2String(int n)
{
    return mAllType[n];
}
QString MainWindow::Area2String(int n)
{
    return mAllArea[n];
}
QString MainWindow::Province2String(int n)
{
    if(n>=0 && n<mAllProvince.size() )
    {
        return mAllProvince[n];
    }
    return "";
}
Customer* MainWindow::FindItem(QString name)
{
    for(int i=0;i<mItems.size();i++)
    {
        if(mItems[i]->name==name)
        {
            return mItems[i];
        }
    }
    return NULL;
}
//tableview双击事件
void MainWindow::on_tableView_doubleClicked(const QModelIndex &index)
{
    QString name=mModel->item(index.row(),0)->text();
    Customer *p=FindItem(name);
    if(p)//如果存在
    {
        dlgitem d(p);//实例对话框对象(传入P会修改P)
        if(d.exec()==1)//对象执行成功（更新对话框的值(最新)）
        {
            mModel->item(index.row(),0)->setText(p->name);
            mModel->item(index.row(),1)->setText(Type2String(p->type));
            mModel->item(index.row(),2)->setText(Area2String(p->area));
            mModel->item(index.row(),3)->setText(Province2String(p->province));
            mModel->item(index.row(),4)->setText(p->city);
            mModel->item(index.row(),5)->setText(p->address);
            mModel->item(index.row(),6)->setText(p->site);
            mModel->item(index.row(),7)->setText(p->research);
            mModel->item(index.row(),8)->setText(p->remark);

            DeleteFromDb(name);//在数据库中删除原来的数据
            InsertToDb(p);//在数据库中插入更新后的数据
        }
    }
}
//搜索按钮事件
void MainWindow::on_btn_select_clicked()
{
    QString s=ui->lineEdit->text();
    mFilterKey=s;
    InitModel();
}
//当当前的treeWidget发生变化时候，会发出信号
void MainWindow::on_treeWidget_currentItemChanged(QTreeWidgetItem *current, QTreeWidgetItem *previous)
{
    QString s=current->text(0);//返回指定列中的文本。
    if(s==QStringLiteral("全部"))
    {
        mFilterArea="";
        mFilterProvince="";
    }
    else if(mAllArea.contains(s))//如果此字符串包含字符串str的出现，则返回true
    {
        //区域
        mFilterArea=s;
        mFilterProvince="";
    }
    else if(mAllProvince.contains(s))
    {
        //省份
        mFilterArea="";
        mFilterProvince=s;
    }
    InitModel();
}
//复选框文本内容被改变
void MainWindow::on_comboBox_currentTextChanged(const QString &arg1)
{
    if(arg1==QStringLiteral("所有类型"))
    {
        mFilterType="";
    }
    else
    {
        mFilterType=arg1;
    }
    InitModel();
}
