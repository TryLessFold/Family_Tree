#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsView>
#include <QGridLayout>
#include <QDockWidget>
#include <QGroupBox>
#include <QVector>
#include <QPushButton>
#include "ftviewer.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    enum btns_e {
        BASE,
        MOVE,
        ADD,
        CONNECT,
        DELETE,
        NONE
    };

    Qt::CursorShape cursors[5] = { Qt::ArrowCursor, Qt::OpenHandCursor, Qt::CrossCursor, Qt::UpArrowCursor, Qt::ForbiddenCursor };
    QString icons[5] = {"C:/Users/TryLessFold/Documents/Family_Tree/icons/base.png",
                        "C:/Users/TryLessFold/Documents/Family_Tree/icons/move.png",
                        "C:/Users/TryLessFold/Documents/Family_Tree/icons/add.png",
                        "C:/Users/TryLessFold/Documents/Family_Tree/icons/connect.png",
                        "C:/Users/TryLessFold/Documents/Family_Tree/icons/delete.png"};
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    FTViewer *view;
    QDockWidget *editor;
    QVector <QPushButton*> btns;

    QGroupBox *AddButtons();

public slots:
    void procComplete(int c);
    void setMove();
    void setAdd();
    void setDelete();
    void setConnect();
    void setBase();
    void createFile();
    void saveFile();
    void loadFile();
    void help();
    void about_prog();
    void about_QT();
};

#endif // MAINWINDOW_H
