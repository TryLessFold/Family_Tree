#include "ftinput_node.h"
#include "ui_ftinput_node.h"
#include <QGridLayout>
#include <QMessageBox>
#include <QDialogButtonBox>
#include <QDebug>

FTInput_node::FTInput_node(QString *first_name, QString *second_name, QString *patr,
                           bool *sx, QString *abt, QVector <FTNode::connect_s> *conn, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::FTInput_node)
{
    QString str;

    ui->setupUi(this);
    table = new QTableWidget();
    table -> setColumnCount(2);
    table->setHorizontalHeaderLabels(QStringList(QList <QString> {"Name", "Role"}));
    table->setRowCount(conn->size());
    for(int i = 0; i < conn->size(); i++)
    {
        str = conn->at(i).ptr->last_n + " "+ conn->at(i).ptr->first_n +" "+ conn->at(i).ptr->patronymic;
        table->setItem(i,0, new QTableWidgetItem(str));
        qDebug() << *conn->at(i).role;
        switch(*conn->at(i).role)
        {
        case FTNode::HUSBUND:
            table->setItem(i,1, new QTableWidgetItem(QString("Husband")));
            break;
        case FTNode::WIFE:
            table->setItem(i,1, new QTableWidgetItem(QString("Wife")));
            break;
        case FTNode::FATHER:
            table->setItem(i,1, new QTableWidgetItem(QString("Father")));
            break;
        case FTNode::MOTHER:
            table->setItem(i,1, new QTableWidgetItem(QString("Mother")));
            break;
        case FTNode::SON:
            table->setItem(i,1, new QTableWidgetItem(QString("Son")));
            break;
        case FTNode::DAUGHTER:
            table->setItem(i,1, new QTableWidgetItem(QString("Daughter")));
            break;
        default:
            break;

        }
    }

    first_n = first_name;
    last_n = second_name;
    patronymic = patr;
    sex = sx;
    about = abt;

    ui->lineEdit->setText(*first_n);
    ui->lineEdit_3->setText(*last_n);
    ui->lineEdit_2->setText(*patronymic);
    ui->comboBox->setCurrentIndex(*sex);
    ui->textEdit->setText(*about);

    ui->gridLayout_4->addWidget(table);

    setModal(true);
    show();
}

FTInput_node::~FTInput_node()
{
    delete ui;
}

void FTInput_node::on_buttonBox_clicked(QAbstractButton *button)
{
    QMessageBox::StandardButton reply;
    if ((ui->lineEdit->text()!=*first_n)||
       (ui->lineEdit_3->text()!=*last_n)||
       (ui->lineEdit_2->text()!=*patronymic)||
       (ui->comboBox->currentIndex()!=*sex)||
       (ui->textEdit->toPlainText()!=*about))
    {
        reply = QMessageBox::question(this, "Save", "Do you want save changes?",
                                      QMessageBox::Yes|QMessageBox::No);
        if(reply == QMessageBox::No)
        {
            return;
        }
        else
        {
            *first_n = ui->lineEdit->text();
            *last_n = ui->lineEdit_3->text();
            *patronymic = ui->lineEdit_2->text();
            *sex = ui->comboBox->currentIndex();
            *about = ui->textEdit->toPlainText();
        }
    }
}
