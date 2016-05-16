#ifndef NODE_H
#define NODE_H

#include <QGraphicsItem>
#include <QString>
#include <QPen>
#include <QSize>
#include <QPainter>
#include <QRectF>
#include <QPointF>
#include <QEvent>
#include <QGraphicsSceneMouseEvent>
#include <QMouseEvent>
#include <QFocusEvent>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QVariant>
#include <iostream>
#include <QDebug>


class node : public QGraphicsItem
{
public:
    node(int x, int y,QString name,QString lastId);
    ~node();

    QString getName();
    QString getId();
    int getX();
    int getY();
    void setX(int x);
    void setY(int y);
    int getHeight();
    int getWidth();
    void setRectangleNode();
    void setCircledNode();

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    QRectF boundingRect() const;
    bool sceneEventFilter(QGraphicsItem *watched, QEvent *event);

    //var for layout
         bool circledLayouted; //definit si le layout est déja passé dessus ou pas

private:


    /***
     * Private functions
     **/
    QString generateId(QString lastId);



    /***
    * Private var
    */
        int x;
        int y;
        QString * name;
        QString * id;  //l'id est de la forme d'un simple int
        bool circledNode; //definit si les neuds sont circulaires ou pas

    //For drag & drop

        //for all the graph
        bool leftButtonPressed;
        bool dragEnter;
        double  lastDragPositionX;
        double  lastDragPositionY;
        double lastSceneRectPositionX;
        double lastSceneRectPositionY;

};

#endif // NODE_H
