#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QPushButton>
#include <QToolBar>
#include <QVBoxLayout>
#include <QMouseEvent>
#include <QGroupBox>
#include <QGridLayout>
#include <QFileDialog>
#include <QTextStream>
#include <QDebug>
#include <QMessageBox>
#include <helpbrowser.h>

#include "ftviewer.h"
#include "fteditor.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    this->setStyleSheet("QMainWindow::separator {width: 0px; border: none;}");

    view = new FTViewer(DEFAULT_SIZE,DEFAULT_SIZE,this);

    setCentralWidget(view);

    editor = new QDockWidget(" ");
    editor->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
    editor->setFixedSize(46, 500);

    editor->setWidget(AddButtons());

    addDockWidget(Qt::LeftDockWidgetArea, editor);

    connect(ui->move_act, SIGNAL(triggered(bool)), this, SLOT(setMove()));
    connect(ui->add_act, SIGNAL(triggered(bool)), this, SLOT(setAdd()));
    connect(ui->conn_act, SIGNAL(triggered(bool)), this, SLOT(setConnect()));
    connect(ui->delete_act, SIGNAL(triggered(bool)), this, SLOT(setDelete()));
    connect(ui->base_act, SIGNAL(triggered(bool)), this, SLOT(setBase()));
    connect(ui->create_act, SIGNAL(triggered(bool)), this, SLOT(createFile()));
    connect(ui->save_act, SIGNAL(triggered(bool)), this, SLOT(saveFile()));
    connect(ui->open_act, SIGNAL(triggered(bool)), this, SLOT(loadFile()));
    connect(ui->help_act, SIGNAL(triggered(bool)), this, SLOT(help()));
    connect(ui->QT_act, SIGNAL(triggered(bool)), this, SLOT(about_QT()));
    connect(ui->prog_act, SIGNAL(triggered(bool)), this, SLOT(about_prog()));
    QAction *lala = editor->toggleViewAction();
    lala->setText("Инструменты");
    ui->menu_3->addAction(lala);
}

MainWindow::~MainWindow()
{
    delete ui;
}

QGroupBox *MainWindow::AddButtons()
{
    QGroupBox *groupBox = new QGroupBox();
    QVBoxLayout *vbox = new QVBoxLayout();
    QPushButton *btn;

    for(int i = 0; i < NONE; i++)
    {
        btn = new QPushButton();
        btn->setCheckable(true);
        if (i == 0)
            btn->setChecked(true);
        btn->setAutoExclusive(true);
        btn->setIcon(QIcon(icons[i]));
        btn->setIconSize(QSize(16, 16));
        connect(btn, &QPushButton::clicked, [this, i](){ view->setCurs(cursors[i]); });
        btns.append(btn);
        vbox->addWidget(btn);
    }
    vbox->addStretch(1);


    groupBox->setLayout(vbox);
    groupBox->setFlat(true);
    return groupBox;
}

void MainWindow::procComplete(int c)
{
    btns.at(c)->animateClick();
}

void MainWindow::setMove()
{
    btns.at(MOVE)->animateClick();
}

void MainWindow::setAdd()
{
    btns.at(ADD)->animateClick();
}

void MainWindow::setConnect()
{
    btns.at(CONNECT)->animateClick();
}

void MainWindow::setDelete()
{
   btns.at(DELETE)->animateClick();
}

void MainWindow::setBase()
{
    btns.at(BASE)->animateClick();
}

void MainWindow::createFile()
{
    delete view;
    view = new FTViewer(DEFAULT_SIZE,DEFAULT_SIZE,this);
    setCentralWidget(view);
    setMove();
}

void MainWindow::saveFile()
{
    QFileDialog *dlg;
    QString file_name = QFileDialog::getSaveFileName(this, tr("Save file"), "");
    QFile f(file_name);
    f.open(QIODevice::WriteOnly);
    QTextStream fout(&f);
    fout << view->x_scene<<" "<<view->x_scene<<" ";
    fout << view->objs.size()<<" ";
    for(int i = 0; i < view->objs.size(); i++)
    {
       fout << view->objs.at(i)->x_rect + view->objs.at(i)->x() <<" "<< view->objs.at(i)->y_rect + view->objs.at(i)->y()<<" "\
            << view->objs.at(i)->x_size <<" "<< view->objs.at(i)->y_size<<" ";

       fout << ((view->objs.at(i)->first_n == "")? "~": view->objs.at(i)->first_n) <<" "\
            << ((view->objs.at(i)->last_n == "")? "~": view->objs.at(i)->last_n)<<" "\
            << ((view->objs.at(i)->patronymic == "")? "~": view->objs.at(i)->patronymic) <<" "\
            << view->objs.at(i)->sex<<" "\
            << ((view->objs.at(i)->about == "")? "~": view->objs.at(i)->about)<<" ";
    }
    fout <<view->objs.size()<<" ";
    for(int i = 0; i < view->objs.size(); i++)
    {
        fout << view->objs.at(i)->connections.size() <<" ";
        foreach (FTNode::connect_s var, view->objs.at(i)->connections) {
            fout << var.ptr->x_rect + var.ptr->x() << " " << var.ptr->y_rect + var.ptr->y() << " ";
            fout << var.ptr->x_size << " " << var.ptr->y_size << " ";
            fout << *var.role << " ";
            foreach (FTNode::connect_s val, var.ptr->connections) {
                if (val.ptr == view->objs.at(i))
                    fout << *val.role << " ";
            }
        }
    }
    f.close();
}

void MainWindow::loadFile()
{
    QFileDialog *dlg;
    QString file_name = QFileDialog::getOpenFileName(this, tr("Open file"), "");
    QFile f(file_name);
    f.open(QIODevice::ReadOnly);
    QTextStream fin(&f);
    int x, y, n, m, k;
    qreal x_rect, y_rect, x_size, y_size;
    QString first_n, last_n, patronymic, about;
    int sex;
    int role1, role2;

    fin>>x; fin>>y;

    delete view;
    view = new FTViewer(x, y, this);

    this -> setCentralWidget(view);

    fin>>n;
    for(int i = 0; i < n; i++)
    {
        fin >> x_rect; fin >> y_rect; fin >> x_size; fin >> y_size;
        fin >> first_n; fin >> last_n; fin >> patronymic; fin >> sex; fin >> about;
        view->objs.append(new FTNode(x_rect, y_rect, x_size, y_size));
        view->objs.last()->first_n = first_n;
        view->objs.last()->last_n = last_n;
        view->objs.last()->patronymic = patronymic;
        view->objs.last()->sex = (FTNode::sex_e)sex;
        view->objs.last()->about = about;
        view->scene->addItem(view->objs.last());
    }

    fin>>n;
    for(int k = 0; k < n; k++)
    {
        fin>>m;
        for(int j = 0; j < m; j++)
        {
            fin >> x_rect; fin >> y_rect; fin >> x_size; fin >> y_size;
            fin >> role2; fin >> role1;
            for (int i = view->objs.size()-1; i >= 0; i--)
            {
                qDebug() << x_rect << y_rect;
                if (view->objs.at(i)->contains(QPoint(x_rect, y_rect)))
                {

                    if ((view->objs.at(k)!=view->objs.at(i))&&(!view->objs.at(k)->isConnected(view->objs.at(i))))
                    {
                        qDebug() << 2;
                        FTNode::role_e *r1 = new FTNode::role_e;
                        FTNode::role_e *r2 = new FTNode::role_e;

                        *r1 = (FTNode::role_e)role1;
                        *r2 = (FTNode::role_e)role2;

                        QLine *line = new QLine();
                        if (i > view->objs.indexOf(view->objs.at(k)))
                        {
                            view->objs.at(k)->connections.append(FTNode::connect_s{view->objs.at(i), r2, 1, line});
                            view->objs.at(i)->connections.append(FTNode::connect_s{view->objs.at(k), r1, 0, line});
                        }
                        else
                        {
                            view->objs.at(k)->connections.append(FTNode::connect_s{view->objs.at(i), r2, 0, line});
                            view->objs.at(i)->connections.append(FTNode::connect_s{view->objs.at(k), r1, 1, line});
                        }
                        view->objs.at(i)->update();
                    }
                    break;
                }
            }
        }
    }
}

void MainWindow::help()
{
    HelpBrowser *helpBrowser = new HelpBrowser("qrc:/", "qrc:/Scripts.htm");
    helpBrowser -> setWindowTitle("Помощь");
    helpBrowser -> resize(600, 350);
    helpBrowser -> show();
}
void MainWindow::about_prog()
{
    QMessageBox msgBox;
    msgBox.setText("Family tree\nIP711 Shcherba Artyom");
    msgBox.setStandardButtons(QMessageBox::Ok);
    msgBox.exec();
}
void MainWindow::about_QT()
{
    QMessageBox msgBox;
    msgBox.setText("Qt — кроссплатформенный фреймворк для разработки\n"
                   "программного обеспечения на языке программирования C++.");
    msgBox.setStandardButtons(QMessageBox::Ok);
    msgBox.exec();
}


