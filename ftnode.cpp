#include "ftnode.h"
#include <QDebug>

FTNode::FTNode(qreal x_r, qreal y_r, qreal x_s, qreal y_s,
                 QGraphicsItem *parent): QGraphicsItem(parent)
{
    x_size = x_s;
    y_size = y_s;
    x_rect = x_r;
    y_rect = y_r;

    this->setFlag(QGraphicsItem::ItemIsMovable);
    this->setFlag(QGraphicsItem::ItemIsSelectable);
}

FTNode::~FTNode()
{
    foreach (connect_s var, connections) {
        for(int i = 0; i < var.ptr->connections.size(); i++)
        {
            if (var.ptr->connections.at(i).ptr == this)
            {
                var.ptr->connections.remove(i);
            }
        }
        if (var.line != NULL)
        {
            delete(var.line);
            var.line = NULL;
            delete(var.role);
            var.role = NULL;
        }
    }
}

void FTNode::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    QPen pen(Qt::darkGray, 5);
    QPen tmp_pen;
//    FTNode *fath, *moth;
//    qreal x_t, y_t;
//    int fl = 0;
    painter->setRenderHint(QPainter::Antialiasing);
    painter->setRenderHint(QPainter::HighQualityAntialiasing);
    tmp_pen = painter->pen();
    painter->setPen(pen);
    painter->setBrush(Qt::lightGray);

    foreach (connect_s var, connections) {
        if (var.dominate == 1)
        {
//            if ((*var.role == SON)||(*var.role == DAUGHTER))
//            {
//                fath = var.ptr->getFather();
//                moth = var.ptr->getMother();
//                if(fath && moth)
//                {
//                    qDebug()<<moth->first_n<< " " << fath->first_n;
//                    if (fath == this)
//                    {
//                        x_t = (x_rect + moth->x_rect + moth->x_size/2 + (moth->x() - x())) / 2;
//                        y_t = (y_rect + moth->x_rect + moth->x_size/2 + (moth->x() - x())) / 2;
//                    }
//                    if (moth == this)
//                    {
//                        x_t = (x_rect + fath->x_rect + fath->x_size/2 + (fath->x() - x())) / 2;
//                        y_t = (y_rect + fath->x_rect + fath->x_size/2 + (fath->x() - x())) / 2;
//                    }

//                    var.line->setLine(x_t, y_t,
//                                      (var.ptr->x_rect + var.ptr->x_size/2 + (var.ptr->x() - x())),
//                                      (var.ptr->y_rect + var.ptr->y_size/2 + (var.ptr->y() - y())));
//                }
//                else
//                {
                    var.line->setLine(x_rect + x_size/2, y_rect + y_size/2,
                                      (var.ptr->x_rect + var.ptr->x_size/2 + (var.ptr->x() - x())),
                                      (var.ptr->y_rect + var.ptr->y_size/2 + (var.ptr->y() - y())));
                    painter->drawLine(*var.line);
//                }
//            }
//            else
//            {
                var.line->setLine(x_rect + x_size/2, y_rect + y_size/2,
                                  (var.ptr->x_rect + var.ptr->x_size/2 + (var.ptr->x() - x())),
                                  (var.ptr->y_rect + var.ptr->y_size/2 + (var.ptr->y() - y())));
                painter->drawLine(*var.line);
//            }

        }
    }

    painter->drawRoundedRect(x_rect, y_rect, x_size, y_size, 5, 5);
    pen.setColor(Qt::black);
    painter->setPen(pen);
    painter->setFont(QFont("Times", 10));
    painter->drawText(QRectF(x_rect, y_rect-y_size/(3.5), x_size, y_size), Qt::AlignCenter, last_n);
    painter->drawText(QRectF(x_rect, y_rect, x_size, y_size), Qt::AlignCenter, first_n);
    painter->drawText(QRectF(x_rect, y_rect+y_size/(3.5), x_size, y_size), Qt::AlignCenter, patronymic);

    painter->setPen(tmp_pen);
}

QRectF FTNode::boundingRect() const
{
    qreal penWidth = 1;
    return QRectF(x_rect - penWidth / 2, y_rect - penWidth / 2,
                  x_size + penWidth, y_size + penWidth);
}

void FTNode::setActive(bool c)
{
    is_active = c;
}

bool FTNode::getActive()
{
    return is_active;
}

bool FTNode::isConnected(FTNode* item)
{
    foreach (connect_s val, connections) {
        if (val.ptr == item)
        {
            return true;
        }
    }
    return false;
}

FTNode *FTNode::getMother()
{
    foreach (connect_s var, connections) {
        if (*var.role == MOTHER)
            return var.ptr;
    }
    return NULL;
}

FTNode *FTNode::getFather()
{
    foreach (connect_s var, connections) {
        if (*var.role == FATHER)
            return var.ptr;
    }
    return NULL;
}
