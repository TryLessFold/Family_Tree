#include "ftconnect.h"
#include "ui_ftconnect.h"
#include <QDebug>
FTConnect::FTConnect(FTNode::role_e *from, FTNode::role_e *to, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::FTConnect)
{
    role1 = from;
    role2 = to;

    ui->setupUi(this);

    setModal(true);
    show();
}

FTConnect::~FTConnect()
{
    delete ui;
    delete this;
}

void FTConnect::on_buttonBox_accepted()
{
    *role1 = (FTNode::role_e)ui->comboBox->currentIndex();
    *role2 = (FTNode::role_e)ui->comboBox_2->currentIndex();
    qDebug() << *role1 << " " << *role2;
}
