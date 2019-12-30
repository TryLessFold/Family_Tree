#ifndef FTNode_H
#define FTNode_H

#include <QGraphicsItem>
#include <QPainter>
#include <QRectF>
#include <QVector>
#include <QLine>

class FTNode: public QObject, public QGraphicsItem
{
    Q_OBJECT

public:
    enum sex_e {
        MALE,
        FEMALE
    };

    enum role_e {
        HUSBUND,
        WIFE,
        FATHER,
        MOTHER,
        SON,
        DAUGHTER,
    };

    struct conn {
        FTNode *ptr;
        role_e *role;
        bool dominate;
        QLine *line;
    } typedef connect_s;

    QVector <connect_s> connections;

    QString first_n = {""};
    QString last_n = {""};
    QString patronymic = {""};
    bool sex = 0;
    QString about = {""};

    qreal x_size, y_size;
    qreal x_rect, y_rect;

    FTNode(qreal x_r = 0, qreal y_r = 0, qreal x_s = 100, qreal y_s = 50, QGraphicsItem *parent = NULL);
    virtual ~FTNode();
    bool getActive();
    bool isConnected(FTNode* item);
    FTNode *getMother();
    FTNode *getFather();

public slots:
    void setActive(bool c);

protected:
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
                   QWidget *widget) override;
    QRectF boundingRect() const  override;

private:
    bool is_active;
};

#endif // FTNode_H
