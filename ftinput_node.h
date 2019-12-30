#ifndef FTINPUT_NODE_H
#define FTINPUT_NODE_H

#include <QDialog>
#include <QTableWidget>
#include <QAbstractButton>
#include "ftnode.h"

namespace Ui {
class FTInput_node;
}

class FTInput_node : public QDialog
{
    Q_OBJECT

public:
    explicit FTInput_node(QString *first_name, QString *last_name, QString *patr,
                          bool *sx, QString *abt, QVector <FTNode::connect_s> *conn, QWidget *parent = 0);
    ~FTInput_node();

private slots:
    void on_buttonBox_clicked(QAbstractButton *button);

private:
    Ui::FTInput_node *ui;
    QTableWidget *table;

    QString *first_n;
    QString *last_n;
    QString *patronymic;
    bool *sex;
    QString *about;
};

#endif // FTINPUT_NODE_H
