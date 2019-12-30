#include <ftviewer.h>
#include <QFrame>
#include <QWheelEvent>
#include <QTransform>
#include <QMessageBox>
#include <QDebug>
#include "ftnode.h"
#include "mainwindow.h"
#include "ftinput_node.h"
#include "ftconnect.h"

FTViewer::FTViewer(int x, int y, QWidget *parent):
    QGraphicsView(parent)
{
    x_scene = x;
    y_scene = y;

    this->setTransformationAnchor(QGraphicsView::AnchorUnderMouse);
    this->setViewportUpdateMode(QGraphicsView::FullViewportUpdate);

    createScene(x, y);

    connect(this, SIGNAL(sendComplete(int)), parent, SLOT(procComplete(int)));
}

FTViewer::~FTViewer()
{
    scene = NULL;
    rect = NULL;
    hor_bar = NULL;
    vert_bar = NULL;
    con_item = NULL;
    objs.clear();
}

void FTViewer::createScene(int x, int y)
{
    scene = new QGraphicsScene(QRect(0, 0, x, y));

    rect = new QGraphicsRectItem(QRect(0, 0, x, y));
    rect->setBrush(Qt::white);

    scene->setBackgroundBrush(Qt::gray);
    scene->addItem(rect);

    pen = new QPen(Qt::black, 5);

    hor_bar = this->horizontalScrollBar();
    vert_bar = this->verticalScrollBar();

    this->setScene(scene);
}

void FTViewer::deleteScene()
{
    if (scene)
    {
        delete(scene);
        scene = NULL;
        rect = NULL;
        x_scene = 0;
        y_scene = 0;
    }
}

void FTViewer::wheelEvent(QWheelEvent *event)
{
    double scaleFactor = 1.15;

    if (event->delta() > 0)
    {
        setTransformationAnchor(QGraphicsView::AnchorUnderMouse);
        if ((hor_bar->maximum() >= INT_MAX-10) && (vert_bar->maximum() >= INT_MAX-10))
            return;
        scale(scaleFactor, scaleFactor);

    }
    else
    {
       setTransformationAnchor(QGraphicsView::AnchorUnderMouse);
        if ((hor_bar->maximum()==hor_bar->minimum())&&(vert_bar->maximum()==vert_bar->minimum()))
            return;
        scale(1/scaleFactor, 1/scaleFactor);

    }

    //this->update();
}

void FTViewer::mousePressEvent(QMouseEvent *event){
    QPointF pos = mapToScene(event->pos());
    QMessageBox::StandardButton reply;
    FTInput_node *dialog;
    QLine *line;
    FTConnect *conn;
    FTNode::role_e *r1, *r2;

    if (event->button() == Qt::MiddleButton)
    {
        middle_m = true;
        _panStartX = event->x();
        _panStartY = event->y();
        setCursor(Qt::ClosedHandCursor);
        event->accept();
        return;
    }
    else if ((event->button() == Qt::LeftButton)&&(getRect()->contains(QPoint(pos.x(), pos.y()))))
    {
        switch(cursor)
        {
        case BASE:
            for (int i = objs.size()-1; i >= 0; i--)
            {
                if (objs.at(i)->contains(QPoint(pos.x()-objs.at(i)->x(), pos.y()-objs.at(i)->y())))
                {
                     dialog = new FTInput_node(&(objs.at(i)->first_n), &(objs.at(i)->last_n),
                                               &(objs.at(i)->patronymic), &(objs.at(i)->sex),
                                               &(objs.at(i)->about), &(objs.at(i)->connections));
                     break;
                }
            }
            break;
        case MOVE:
            QGraphicsView::mousePressEvent(event);
            break;
        case ADD:
            objs.append(new FTNode(pos.x(), pos.y()));
            dialog = new FTInput_node(&(objs.last()->first_n), &(objs.last()->last_n),
                                      &(objs.last()->patronymic), &(objs.last()->sex),
                                      &(objs.last()->about), &(objs.last()->connections));
            scene->addItem(objs.last());
            emit(sendComplete(save_curs));
            setCursor(Qt::ArrowCursor);
            break;
        case CONNECT:
            for (int i = objs.size()-1; i >= 0; i--)
            {
                if (objs.at(i)->contains(QPoint(pos.x()-objs.at(i)->x(), pos.y()-objs.at(i)->y())))
                {
                    if (con_item == NULL)
                    {
                        con_item = objs.at(i);
                    }
                    else
                    {
                        if ((con_item!=objs.at(i))&&(!con_item->isConnected(objs.at(i))))
                        {
                            r1 = new FTNode::role_e;
                            r2 = new FTNode::role_e;
                            conn = new FTConnect(r1, r2);
                            line = new QLine();
                            if (i > objs.indexOf(con_item))
                            {
                                con_item->connections.append(FTNode::connect_s{objs.at(i), r2, 1, line});
                                objs.at(i)->connections.append(FTNode::connect_s{con_item, r1, 0, line});
                            }
                            else
                            {
                                con_item->connections.append(FTNode::connect_s{objs.at(i), r2, 0, line});
                                objs.at(i)->connections.append(FTNode::connect_s{con_item, r1, 1, line});
                            }
                            qDebug() << *r1 << " " << *r2;
                            objs.at(i)->update();
                            emit(sendComplete(save_curs));
                            setCursor(Qt::ArrowCursor);
                        }
                    }
                    break;
                }
            }
            break;
        case DELETE:
            for (int i = objs.size()-1; i >= 0; i--)
            {
                if (objs.at(i)->contains(QPoint(pos.x()-objs.at(i)->x(), pos.y()-objs.at(i)->y())))
                {
                    reply = QMessageBox::question(this, "Delete", "Delete node?",
                                                  QMessageBox::Yes|QMessageBox::No);
                    if(reply == QMessageBox::Yes)
                    {
                        delete(objs.at(i));
                        objs.remove(i);
                        emit(sendComplete(save_curs));
                        setCursor(Qt::ArrowCursor);
                    }
                    break;
                }
            }
            break;
        default:
            break;
        }
    }
    event->ignore();
}

void FTViewer::mouseReleaseEvent(QMouseEvent *event){
    if (event->button() == Qt::MiddleButton)
    {
        middle_m = false;
        setCursor(cursor);
        event->accept();
        return;
    }
    QGraphicsView::mouseReleaseEvent(event);
}

void FTViewer::mouseMoveEvent(QMouseEvent *event){
    QPointF pos = mapToScene(event->pos());
    if (middle_m)
    {
        horizontalScrollBar()->setValue(horizontalScrollBar()->value() - (event->x() - _panStartX));
        verticalScrollBar()->setValue(verticalScrollBar()->value() - (event->y() - _panStartY));
        _panStartX = event->x();
        _panStartY = event->y();
        event->accept();
        //scene->update();
        return;
    }
    if(this->underMouse())
    {
        setCursor(cursor);
    }
    QGraphicsView::mouseMoveEvent(event);
}

void FTViewer::setCurs(Qt::CursorShape curs)
{
    cursor = curs;
    if (curs == Qt::ArrowCursor)
        save_curs = 0;
    else if (curs == Qt::OpenHandCursor)
        save_curs = 1;
    con_item = NULL;
}

QGraphicsRectItem *FTViewer::getRect()
{
    return rect;
}
