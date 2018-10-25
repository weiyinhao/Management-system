#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include "dlgitem.h"
#include <QMainWindow>
#include <QFileDialog>
#include <QFile>
#include <QVector>
#include <QStandardItem>//QStandardItem类提供了与QStandardItemModel类一起使用的项
#include <QStandardItemModel>//提供用于存储自定义数据的通用模型
#include <QSqlDatabase>//与数据库进行连接
#include <QSqlQuery> //提供了一种执行和操作SQL语句的方法。
#include <QListView>//为模型提供列表或图标视图
#include <QSqlError>
#include <QTreeWidgetItem>
#include <QCoreApplication>
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
private slots:
    void onMenuOpen(bool checked);//打开文件
    void onMenuSave(bool checked);//保存文件

    void on_btn_add_clicked();//增加按钮
    void doProcessSigaddItem(Customer *p);//DigItem里面添加记录后需要不关闭对话框，因此发送个信号
    void on_btn_delete_clicked();

    void on_tableView_doubleClicked(const QModelIndex &index);//tableview双击事件

    void on_btn_select_clicked();//搜索按钮事件

    void on_treeWidget_currentItemChanged(QTreeWidgetItem *current, QTreeWidgetItem *previous);//当当前的treeWidget发生变化时候，会发出信号

    void on_comboBox_currentTextChanged(const QString &arg1);

private:
    QString Type2String(int type);//处理单位类型
    QString Area2String(int Area);//处理区域
    QString Province2String(int province);//处理省份
    void DeleteItem(QString name);//删除对应的列表
    Customer* FindItem(QString name);//查找对应客户的数据(返回值也应该是客户信息)

private:
    Ui::MainWindow *ui;

    void Init();
    void InitMenu();
    void InitModel();
    void addItem(Customer *item);
    void InitTable();
    void ClearData();

    void OpenDb(QString filename);//打开数据库文件
    void SaveToDb(QString filename);//保存数据库文件
    void DeleteFromDb(QString name);//删除数据库中的指定数据
    void InsertToDb(Customer *p);//对数据库插入指定的数据
private:
    QStandardItemModel *mModel;//tableview的model

    QString mDefaultDB;

    QStringList mAllType;//所有单位类型
    QStringList mAllArea;//所有区域
    QStringList mAllProvince;   //所有省份

    QVector<Customer *> mItems;//所有的客户数据
    QStringList myarea;//所有区域
    QStringList myprovince;//所有省份

    QString mFilterType;//单位类型过滤
    QString mFilterArea;//所有区域过滤
    QString mFilterProvince;//省份过滤
    QString mFilterKey;//关键字过滤

};

#endif // MAINWINDOW_H
