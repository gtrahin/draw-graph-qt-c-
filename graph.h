#ifndef GRAPH_H
#define GRAPH_H

#include <QGraphicsView>
#include <QGraphicsScene>
#include <QSLider>
#include <QString>
#include <QPointF>
#include <QMouseEvent>
#include <QEvent>
#include <QRect>
#include <iostream>
#include <math.h>
#include "node.h"
#include "transition.h"

class graph: public QGraphicsView
{
    Q_OBJECT
public:
    graph();
    ~graph();

    void addNode(int x,int  y,QString name);
    void addNode(QString name);
    std::vector<node *> getNodeByName(QString name); //récupération de tout les noeud avec un certain nom
    node * getNodeById(QString id);   //récupération du noeud avec un certain id
    bool eventFilter(QObject * object, QEvent * event);
    void keyPressEvent(QKeyEvent *event);
    void keyReleaseEvent(QKeyEvent *event);
    void wheelEvent( QWheelEvent * event );

    void addTransition(node * n1,node * n2,QString name);

    //types of nodes (rectangle | circle)
    void setCircledNodes();
    void setRectangleNodes();

    //Layouts
    void setCircledLayout();
    void setForceBaseLayout();

public slots:
    void zoomOrDezoom();


private:

    /***
     * Private functions
     **/
        void init();
        void createGraph();
        void setListenners();

    /***
    * Private var
    */
        QGraphicsScene * graphicsScene;
        QSlider * sliderZoom;

        //var for nodes
        QString currentNodeId;
        std::vector<node *> * nodes;

        //var for transitions
        QString currentTransitionId;
        std::vector<transition *> * transitions;

        //For drag & drop

            //for all the graph
            bool shortcutPressSingleItem; //indique si on a presser un touche pour déplacer suelemtn un item -> implique que le graph ne vas pas bouger
            bool rightButtonPressed;
            bool dragEnter;
            double  lastDragPositionX;
            double  lastDragPositionY;
            double lastSceneRectPositionX;
            double lastSceneRectPositionY;


};

#endif // GRAPH_H
