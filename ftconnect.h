#ifndef FTCONNECT_H
#define FTCONNECT_H

#include <QDialog>
#include "ftnode.h"

namespace Ui {
class FTConnect;
}

class FTConnect : public QDialog
{
    Q_OBJECT

public:
    explicit FTConnect(FTNode::role_e *from, FTNode::role_e *to, QWidget *parent = 0);
    ~FTConnect();

private slots:
    void on_buttonBox_accepted();

private:
    Ui::FTConnect *ui;

    FTNode::role_e *role1;
    FTNode::role_e *role2;
};

#endif // FTCONNECT_H
