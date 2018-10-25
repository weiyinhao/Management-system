#ifndef DLGITEM_H
#define DLGITEM_H

#include <QDialog>
#include <QTableWidgetItem>
namespace Ui {
class dlgitem;
}
typedef struct _Customer{
    QString name;//姓名
    int type;//类型
    int area;//区域
    int province;//省份
    QString city;//城市
    QString address;//地址
    QString site;//地点
    QString contact;//联系方式
    QString buy;//采购情况
    QString research;//研究方向
    QString remark;//备注
}Customer,*PCustomer;//customer顾客
class dlgitem : public QDialog
{
    Q_OBJECT

public:
    explicit dlgitem(Customer* item,QWidget *parent = 0);
    ~dlgitem();

private slots:
    void on_btn_Add_clicked();

    void on_btn_Cancel_clicked();

    void on_edit_name_textChanged(const QString &arg1);

    //void on_cobox_area_currentTextChanged(const QString &arg1);

    void on_cobox_area_currentIndexChanged(int index);

private:
    Ui::dlgitem *ui;
    Customer *mItem;
    void Init();//初始化状态
private:
    int String2province(QString s);
    QString Province2String(int province);
signals:
    void sigaddItem(Customer*);
};

#endif // DLGITEM_H
