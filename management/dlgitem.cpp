#include "dlgitem.h"
#include "ui_dlgitem.h"
#include <QDebug>
dlgitem::dlgitem(Customer* item,QWidget *parent) :
    QDialog(parent),
    ui(new Ui::dlgitem)
{
    ui->setupUi(this);
    mItem=item;
    Init();

}
//初始化状态
void dlgitem::Init()
{
    QFont font( "Microsoft YaHei", 10, 75);
    ui->labelTip_3->setFont(font);
    ui->labelTip_3->setStyleSheet("QLabel{color:red;}");
    ui->tableWidget->setRowCount(20);
    for(int i=0;i<20;i++)
    {
        ui->tableWidget->setItem(i,0,new QTableWidgetItem(""));
        ui->tableWidget->setItem(i,1,new QTableWidgetItem(""));
        ui->tableWidget->setItem(i,2,new QTableWidgetItem(""));
        ui->tableWidget->setItem(i,3,new QTableWidgetItem(""));
        ui->tableWidget->setItem(i,4,new QTableWidgetItem(""));
        ui->tableWidget->setItem(i,5,new QTableWidgetItem(""));
    }
    on_cobox_area_currentIndexChanged(0);
    if(mItem)//客户不为空
    {
        //修改
        ui->btn_Add->setText(QStringLiteral("修改"));

        ui->edit_name->setText(mItem->name);
        ui->cobox_type->setCurrentIndex(mItem->type);
        ui->cobox_area->setCurrentIndex(mItem->area);
        on_cobox_area_currentIndexChanged(ui->cobox_area->currentIndex());
        ui->cobox_province->setCurrentText(Province2String(mItem->province));
        ui->edit_address->setText(mItem->address);
        ui->edit_city->setText(mItem->city);
        ui->edit_url->setText(mItem->site);
        ui->textEdit_buy->setText(mItem->buy);
        ui->textEdit_research->setText(mItem->research);
        ui->textEdit_remark->setText(mItem->remark);

        QStringList All=mItem->contact.split(";");
        for(int i=0;i<All.size();i++)
        {
            QStringList row=All[i].split(",");
            if(row.size()>=6)
            {
                ui->tableWidget->item(i,0)->setText(row[0]);
                ui->tableWidget->item(i,1)->setText(row[1]);
                ui->tableWidget->item(i,2)->setText(row[2]);
                ui->tableWidget->item(i,3)->setText(row[3]);
                ui->tableWidget->item(i,4)->setText(row[4]);
                ui->tableWidget->item(i,5)->setText(row[5]);
                if(!row[5].isEmpty())
                {
                    ui->tableWidget->item(i,0)->setBackground(Qt::red);
                    ui->tableWidget->item(i,1)->setBackground(Qt::red);
                    ui->tableWidget->item(i,2)->setBackground(Qt::red);
                    ui->tableWidget->item(i,3)->setBackground(Qt::red);
                    ui->tableWidget->item(i,4)->setBackground(Qt::red);
                    ui->tableWidget->item(i,5)->setBackground(Qt::red);
                }
            }
        }
    }
}
dlgitem::~dlgitem()
{
    delete ui;
}
//添加按钮
void dlgitem::on_btn_Add_clicked()
{
    if(ui->edit_name->text().isEmpty())
    {
        ui->labelTip_3->setText(QStringLiteral("单位名称不能为空"));
        return;
    }
    Customer* p=new Customer;
    p->name=ui->edit_name->text();
    p->type=ui->cobox_type->currentIndex();
    p->area=ui->cobox_area->currentIndex();
    p->province=String2province(ui->cobox_province->currentText());
    p->city=ui->edit_city->text();
    p->address=ui->edit_address->text();
    p->site=ui->edit_url->text();
    p->buy=ui->textEdit_buy->toPlainText();
    p->research=ui->textEdit_research->toPlainText();
    p->remark=ui->textEdit_remark->toPlainText();

    QString sAll="";
    QString s="";
    int columns=ui->tableWidget->columnCount();
    for(int i=0;i<ui->tableWidget->rowCount();i++)
    {
        if(ui->tableWidget->item(i,0)->text().isEmpty())
        {
            continue;
        }
        for(int j=0;j<columns;j++)
        {
            //,和;是用来分隔的标记，禁止用户输入这两个字符，输入了会被替换为空格
            s=ui->tableWidget->item(i,j)->text();
            s.replace(","," ");//替换成空格 防止用户输入错误
            s.replace(";"," ");
            sAll+=s;
            if(j==columns-1)//;进行结束标志
            {
                sAll+=";";
            }
            else//,进行各个信息分割
            {
                sAll+=",";
            }
        }
    }
    p->contact=sAll;
    if(mItem)//如果存在
    {
        //修改
        mItem->name = p->name;
        mItem->type = p->type;
        mItem->area = p->area;
        mItem->province = p->province;
        mItem->city = p->city;
        mItem->address = p->address;
        mItem->site = p->site;
        mItem->contact = p->contact;
        mItem->buy = p->buy;
        mItem->research = p->research;
        mItem->remark = p->remark;
        accept();//隐藏模态对话框并将结果代码设置为Accepted。
    }
    else//不存在
    {
        //新增(发射信号，已经增加了。清空显示)
        sigaddItem(p);
        qDebug()<<"??";
        //清空ui显示数据
        ui->edit_name->setText("");
        ui->edit_address->setText("");
        ui->edit_city->setText("");
        ui->edit_url->setText("");
        ui->textEdit_buy->setText("");
        ui->textEdit_remark->setText("");
        ui->textEdit_research->setText("");
        for(int i=0;i<20;i++)
        {
            ui->tableWidget->item(i,0)->setText("");
            ui->tableWidget->item(i,1)->setText("");
            ui->tableWidget->item(i,2)->setText("");
            ui->tableWidget->item(i,3)->setText("");
            ui->tableWidget->item(i,4)->setText("");
            ui->tableWidget->item(i,5)->setText("");
        }
        ui->edit_name->setFocus();//改变焦距(单位名称一栏);
        ui->labelTip_3->setText(QStringLiteral("添加成功！"));
    }

}
//取消按钮
void dlgitem::on_btn_Cancel_clicked()
{
    reject();//隐藏模态对话框并将结果代码设置为Accepted。
}

//单位区域输入框文本被改变
void dlgitem::on_edit_name_textChanged(const QString &arg1)
{
    ui->labelTip_3->setText("");
}
//区域选择框内容被改变

void dlgitem::on_cobox_area_currentIndexChanged(int index)
{
    ui->cobox_province->clear();//清空省份
    if(index==0)
    {
        ui->cobox_province->addItems(QStringList()<<QStringLiteral("山东")<<QStringLiteral("浙江")<<QStringLiteral("江苏")<<QStringLiteral("安徽")<<QStringLiteral("上海")<<QStringLiteral("福建"));
    }
    else if(index == 1)
    {
        ui->cobox_province->addItems(QStringList()<<QStringLiteral("广东")<<QStringLiteral("广西")<<QStringLiteral("海南"));
    }
    else if(index == 2)
    {
        ui->cobox_province->addItems(QStringList()<<QStringLiteral("湖北")<<QStringLiteral("湖南")<<QStringLiteral("河南")<<QStringLiteral("江西"));
    }
    else if(index == 3)
    {
        ui->cobox_province->addItems(QStringList()<<QStringLiteral("北京")<<QStringLiteral("天津")<<QStringLiteral("河北")<<QStringLiteral("山西")<<QStringLiteral("内蒙"));
    }
    else if(index == 4)
    {
        ui->cobox_province->addItems(QStringList()<<QStringLiteral("宁夏")<<QStringLiteral("新疆")<<QStringLiteral("青海")<<QStringLiteral("陕西")<<QStringLiteral("甘肃")<<QStringLiteral("四川")<<QStringLiteral("云南")<<QStringLiteral("贵州")<<QStringLiteral("西藏")<<QStringLiteral("重庆")<<QStringLiteral("辽宁")<<QStringLiteral("吉林")<<QStringLiteral("黑龙江"));
    }
}
/*void dlgitem::on_cobox_area_currentTextChanged(const QString &arg1)
{
    ui->cobox_province->clear();//清空省份
    if(arg1=="华东")
    {
        ui->cobox_province->addItems(QStringList()<<"山东"<<"浙江"<<"江苏"<<"安徽"<<"上海"<<"福建");
    }
    else if(arg1 == "华南")
    {
        ui->cobox_province->addItems(QStringList()<<"广东"<<"广西"<<"海南");
    }
    else if(arg1 == "华中")
    {
        ui->cobox_province->addItems(QStringList()<<"湖北"<<"湖南"<<"河南"<<"江西");
    }
    else if(arg1 == "华北")
    {
        ui->cobox_province->addItems(QStringList()<<"北京"<<"天津"<<"河北"<<"山西"<<"内蒙");
    }
    else if(arg1 == "其他")
    {
        ui->cobox_province->addItems(QStringList()<<"宁夏"<<"新疆"<<"青海"<<"陕西"<<"甘肃"<<"四川"<<"云南"<<"贵州"<<"西藏"<<"重庆"<<"辽宁"<<"吉林"<<"黑龙江");
    }


}*/
//把QString索引转换成int索引
int dlgitem::String2province(QString s)
{
    QStringList AllProvince;
    AllProvince<<QStringLiteral("山东")<<QStringLiteral("浙江")<<QStringLiteral("江苏")<<QStringLiteral("安徽")<<QStringLiteral("上海")<<QStringLiteral("福建")
              <<QStringLiteral("广东")<<QStringLiteral("广西")<<QStringLiteral("海南")
              <<QStringLiteral("湖北")<<QStringLiteral("湖南")<<QStringLiteral("河南")<<QStringLiteral("江西")
              <<QStringLiteral("北京")<<QStringLiteral("天津")<<QStringLiteral("河北")<<QStringLiteral("山西")<<QStringLiteral("内蒙")
              <<QStringLiteral("宁夏")<<QStringLiteral("新疆")<<QStringLiteral("青海")<<QStringLiteral("陕西")<<QStringLiteral("甘肃")<<QStringLiteral("四川")<<QStringLiteral("云南")<<QStringLiteral("贵州")<<QStringLiteral("西藏")<<QStringLiteral("重庆")<<QStringLiteral("辽宁")<<QStringLiteral("吉林")<<QStringLiteral("黑龙江");
    return AllProvince.indexOf(s);//返回列表中的第一个完全匹配的索引位置
}

//把int索引转换QString索引
QString dlgitem::Province2String(int n)
{
    QStringList AllProvince;
    AllProvince<<"山东"<<"浙江"<<"江苏"<<"安徽"<<"上海"<<"福建"
                <<"广东"<<"广西"<<"海南"
                <<"湖北"<<"湖南"<<"河南"<<"江西"
                <<"北京"<<"天津"<<"河北"<<"山西"<<"内蒙"
                <<"宁夏"<<"新疆"<<"青海"<<"陕西"<<"甘肃"<<"四川"<<"云南"<<"贵州"<<"西藏"<<"重庆"<<"辽宁"<<"吉林"<<"黑龙江";
    if(n>=0 && n<AllProvince.size())
        return AllProvince[n];
    return "";

}

