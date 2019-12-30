#ifndef FTViewer_H
#define FTViewer_H

#include <QGraphicsView>
#include <QGraphicsScene>
#include <QGraphicsRectItem>
#include <QScrollBar>
#include <QVector>
#include "ftnode.h"


#define DEFAULT_SIZE 10000

class FTViewer: public QGraphicsView
{
    Q_OBJECT

public:
    FTViewer(int x = DEFAULT_SIZE , int y = DEFAULT_SIZE, QWidget *parent = Q_NULLPTR);
    ~FTViewer();

    QGraphicsScene *scene = NULL;
    QVector <FTNode*> objs;
    int x_scene;
    int y_scene;

    QGraphicsRectItem *getRect();

signals:
    void sendComplete(int c);

public slots:
    void createScene(int y, int x);
    void deleteScene();
    void setCurs(Qt::CursorShape curs);

private:
    enum btns_e {
        BASE = Qt::ArrowCursor,
        MOVE = Qt::OpenHandCursor,
        ADD = Qt::CrossCursor,
        CONNECT = Qt::UpArrowCursor,
        DELETE = Qt::ForbiddenCursor,
        NONE
    };

    Qt::CursorShape cursor;
    int save_curs = 0;
    QPen *pen;
    QGraphicsRectItem *rect = NULL;
    QScrollBar *hor_bar = NULL;
    QScrollBar *vert_bar = NULL;
    FTNode *con_item = NULL;

    void wheelEvent(QWheelEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);

    bool middle_m = 0;
    int _panStartX = 0;
    int _panStartY = 0;
};

#endif // FTViewer_H
