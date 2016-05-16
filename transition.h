#ifndef TRANSITION_H
#define TRANSITION_H

#include <QGraphicsItem>
#include <QString>
#include <QPen>
#include <QSize>
#include <QPainter>
#include <QRectF>
#include <QPointF>
#include <QDebug>
#include <iostream>
#include <math.h>
#include "node.h"


class transition : public QGraphicsItem
{
public:

    transition(node * n1, node * n2,QString name,QString lastId);
    ~transition();

    QString getName();
    QString getId();
    QPointF getP1(); //retourne le point de départ de la transition
    QPointF getP2(); //retourn le point d'arrivé de la transition
    node * getN1();
    node * getN2();

    void setNodesAreCircles();
    void setNodesAreRectangles();

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    QRectF boundingRect() const;

private:

    //private functions
    QString generateId(QString lastId);

    //private var
    node * n1;
    node * n2;
    QPointF * p1;
    QPointF * p2;
    QString * name;
    QString * id;    //l'id est de la forme d'un simple int

    bool nodesAreCircles;

};

#endif // TRANSITION_H
