#include "graph.h"

#define PI 3.14159265


const int SHORTCUT_MOVE_SINGLE_ITEM = Qt::Key_Space;

graph::graph()
{
    init();
    createGraph();
    setListenners();

    addNode(130,250,"azertyuiop");

    addNode("a");
    addNode(250,200,"b");
    addNode(300,250,"c");
    addNode(250,350,"d");
    addNode(180,350,"e");
    addNode(100,350,"f");
    addNode(80,250,"g");
    addNode(100,180,"h");

    addNode(130,160,"h1");
    addNode(130,210,"h2");




    addTransition(getNodeByName("azertyuiop").at(0),getNodeByName("a").at(0),"");
    addTransition(getNodeByName("azertyuiop").at(0),getNodeByName("b").at(0),"");
    addTransition(getNodeByName("azertyuiop").at(0),getNodeByName("c").at(0),"");
    addTransition(getNodeByName("azertyuiop").at(0),getNodeByName("d").at(0),"");
    addTransition(getNodeByName("azertyuiop").at(0),getNodeByName("e").at(0),"");
    addTransition(getNodeByName("azertyuiop").at(0),getNodeByName("f").at(0),"");
    addTransition(getNodeByName("azertyuiop").at(0),getNodeByName("g").at(0),"");
    addTransition(getNodeByName("azertyuiop").at(0),getNodeByName("h").at(0),"");

    addTransition(getNodeByName("h").at(0),getNodeByName("h1").at(0),"");
    addTransition(getNodeByName("h").at(0),getNodeByName("h2").at(0),"");

    addTransition(getNodeByName("h").at(0),getNodeByName("g").at(0),"");

    this->setCircledLayout();
    this->setCircledNodes();


}

graph::~graph(){

    for(unsigned long i=0;i<transitions->size();i++){
        delete transitions->at(i);
    }
    delete transitions;

    for(unsigned long i=0;i<nodes->size();i++){
        delete nodes->at(i);
    }
    delete nodes;


    delete sliderZoom;
    delete graphicsScene;

};


void graph::init(){

    graphicsScene = new QGraphicsScene();
    sliderZoom = new QSlider(this);
    currentNodeId = "0";
    currentTransitionId = "0";

    nodes = new std::vector<node *>();
    transitions = new std::vector<transition *>();

    //variables for drag & drop
        //du graph entier
        rightButtonPressed = false;
        dragEnter = false;
        shortcutPressSingleItem = false;
        lastDragPositionX = 0;
        lastDragPositionY = 0;
        lastSceneRectPositionX = 0;
        lastSceneRectPositionY = 0;

}

void graph::createGraph(){

    graphicsScene->setBackgroundBrush(QBrush("#4c5053"));
    graphicsScene->addWidget(sliderZoom);


    sliderZoom->setRange(1000,30000);
    sliderZoom->setValue(10000);
    sliderZoom->blockSignals(false);

    this->setHorizontalScrollBarPolicy ( Qt::ScrollBarAlwaysOff );
    //this->setLayout(NULL);
    this->setVerticalScrollBarPolicy ( Qt::ScrollBarAlwaysOff );
    //this->setEnabled(false);
    this->setStyleSheet("border:none;");
    this->setTransform(QTransform::fromScale(1, 1));
    this->setScene(graphicsScene);
    this->setSceneRect(QRect(0,0,700,600));
    //this->fitInView(graphicsScene->sceneRect());
    this->viewport()->installEventFilter(this);
    //this->setDragMode(QGraphicsView::NoDrag);

}

void graph::setListenners(){

    QObject::connect(sliderZoom, SIGNAL(valueChanged(int)), this, SLOT(zoomOrDezoom()));

}

/*----------------------
 *  GESTION DES NOEUDS
 * ------------------*/

void graph::addNode(QString name){

    addNode(0,0,name);
}


void graph::addNode(int x,int y, QString name){

    QPointF p = this->mapToScene(x,y);
    node * n = new node(p.x(),p.y(),name,currentNodeId);
    //n->setFlag(QGraphicsItem::ItemIsMovable); //permet de bouger les éléments
    n->setFlag(QGraphicsItem::ItemIsSelectable);
    //n->setFlag(QGraphicsItem::ItemSendsGeometryChanges, true);
    //n->setFlag(QGraphicsItem::ItemIgnoresTransformations); //obligatoire si non ignore les drag apres le premier drag
    //n->setFlag(QGraphicsItem::ItemSendsGeometryChanges);
    currentNodeId = n->getId(); //met a jour le dernier ID
    this->graphicsScene->addItem(n);
    n->installSceneEventFilter(n); //l'event ne peut etre installé seulement apres etre ajouté a la scene correspondante
    nodes->push_back(n);

}

std::vector<node *> graph::getNodeByName(QString name){

    std::vector<node *> retNodes;
    for(unsigned long i=0;i<nodes->size();i++){

        if(nodes->at(i)->getName() == name){
            retNodes.push_back(nodes->at(i));
        }
    }

    return retNodes;
}


node * graph::getNodeById(QString id){

   node * retNode = NULL;
   for(unsigned long i =0;i<nodes->size();i++){

       if(nodes->at(i)->getId() == id){

           retNode = nodes->at(i);
       }
   }

   return retNode;
}

/*--------------------------
 *  GESTION DES TRANSITIONS
 * ------------------------*/

void graph::addTransition(node * n1,node * n2,QString name){

    transition * t = new transition(n1,n2,name,currentTransitionId);
    currentTransitionId = t->getId();
    this->graphicsScene->addItem(t);
    transitions->push_back(t);
}

/*----------------------------------------------------
 *  GESTION DES TYPES D'ELEMENTS (circle | rectangle)
 * --------------------------------------------------*/

void graph::setCircledNodes(){

    for(unsigned i =0;i<nodes->size();i++){

        nodes->at(i)->setCircledNode();
    }

    for(unsigned i = 0;i<transitions->size();i++){
        transitions->at(i)->setNodesAreCircles();
    }

    this->update();
}

void graph::setRectangleNodes(){

    for(unsigned i =0;i<nodes->size();i++){

        nodes->at(i)->setRectangleNode();
    }

    for(unsigned i = 0;i<transitions->size();i++){
        transitions->at(i)->setNodesAreRectangles();

    }

    this->update();
}

/*--------------------------
 *  LAYOUTS
 * ------------------------*/

void graph::setCircledLayout(){

    std::vector<std::vector<QString> > vectorChildNodesToPlace;
    std::vector<QString> idParentNode;

    for(unsigned int i=0;i<nodes->size();i++){

        idParentNode.push_back(nodes->at(i)->getId());
        std::vector<QString> tmp_vector;

        for(unsigned int j=0;j<transitions->size();j++){

            //si une transition a pour n1 le noeud actuelle, on vas crée un vector
            //de ces noeuds fils (les n2 de la transition actuelle)
            if( transitions->at(j)->getN1()->getId() == nodes->at(i)->getId() ){

                tmp_vector.push_back(transitions->at(j)->getN2()->getId());
            }

        }

        vectorChildNodesToPlace.push_back(tmp_vector);
        tmp_vector.clear();

    }

    for(unsigned i=0;i<vectorChildNodesToPlace.size();i++){


        for(unsigned j =0;j<vectorChildNodesToPlace.at(i).size();j++){

           //si le layout n'est pas encore passé sur ce noeud
           if(! getNodeById(vectorChildNodesToPlace.at(i).at(j))->circledLayouted){

               int nbNode = vectorChildNodesToPlace.size(); //nombre de noeuds fils pour le parent
               int xParent = getNodeById(idParentNode.at(i))->getX()+(getNodeById(idParentNode.at(i))->getWidth()/2); //corrdonée du centre  parent X
               int yParent = getNodeById(idParentNode.at(i))->getY()+(getNodeById(idParentNode.at(i))->getHeight()/2); //corrdonée du centre parent Y
               int coef  = nbNode * getNodeById(idParentNode.at(i))->getWidth()/(nbNode/2);//décalage a partir du noeud central = nombre de neoud * la taille du noeud central (parent)
               int newX = cos( ( (2*PI)/nbNode ) * j ) * coef + xParent;
               int newY = -sin( ( (2*PI)/nbNode ) * j ) * coef + yParent;
               getNodeById(vectorChildNodesToPlace.at(i).at(j))->setX( newX );
               getNodeById(vectorChildNodesToPlace.at(i).at(j))->setY( newY );
               getNodeById(vectorChildNodesToPlace.at(i).at(j))->circledLayouted = true;
           }

        }

    }

    this->update();

    //affiche le vectorChildNodeToPlace
    for(unsigned i=0;i<vectorChildNodesToPlace.size();i++){
        std::cout<<"[ "<<idParentNode.at(i).toStdString()<<" ] -> ";
        for(unsigned j =0;j<vectorChildNodesToPlace.at(i).size();j++){
            std::cout<<vectorChildNodesToPlace.at(i).at(j).toStdString()<<" , ";
        }
        std::cout<<"\n";
    }
}
void graph::setForceBaseLayout(){

}


/*----------
 *  Event filter
 * ---------*/

bool graph::eventFilter(QObject * object, QEvent * event){
    object = NULL; //juste pour éviter un warning


    /*---------------------------
     *  Drag graphicsScene part
     * -----------------------*/
    if(!shortcutPressSingleItem){

           if(event->type() == QMouseEvent::MouseButtonPress){

               if(QMouseEvent * mouseEvent = static_cast<QMouseEvent*>(event)){

                   if(mouseEvent->button() == Qt::LeftButton){

                        rightButtonPressed = true;
                   }
               }

               return true;


           }
           if(event->type() == QEvent::MouseButtonRelease){

                 rightButtonPressed =false;
                 dragEnter = false;
                 //this->setDragMode(QGraphicsView::NoDrag);
                 //this->setCursor(Qt::ArrowCursor);

                 return true;


           }
           if(event->type() == QEvent::MouseMove &&  rightButtonPressed){

                if(QMouseEvent * mouseEvent = static_cast<QMouseEvent*>(event)){


                        //si il n'y a pas encore de position initial on la cree a la position actuelle du curseur
                        //si non on peut bouger la scene
                        if(!dragEnter){
                            lastDragPositionX = mouseEvent->globalX();
                            lastDragPositionY = mouseEvent->globalY();
                            dragEnter = true;
                           //this->setDragMode(QGraphicsView::ScrollHandDrag);

                        }
                        else{

                            this->setSceneRect(QRect(lastSceneRectPositionX-(mouseEvent->globalX()-lastDragPositionX),lastSceneRectPositionY+(mouseEvent->globalY()-lastDragPositionY),700,600));
                            lastSceneRectPositionX =lastSceneRectPositionX-(mouseEvent->globalX()-lastDragPositionX) ;
                            lastSceneRectPositionY =lastSceneRectPositionY-(mouseEvent->globalY()-lastDragPositionY) ;
                            lastDragPositionX = mouseEvent->globalX();
                            lastDragPositionY = mouseEvent->globalY();

                        }
                }

                return true;
         }
    }

        return false;

}

/*------------------------------------------------
 *  Récuperation du bouton qui indique le drag
 *  d'un élement du graph ou non
 * ------------------------------------------*/

void graph::keyPressEvent(QKeyEvent *event){

    if(event->key() == SHORTCUT_MOVE_SINGLE_ITEM){

        shortcutPressSingleItem = true;
    }

}

void graph::keyReleaseEvent(QKeyEvent *event){

    if(event->key() == SHORTCUT_MOVE_SINGLE_ITEM){

        shortcutPressSingleItem = false;

        for(unsigned i =0;i<nodes->size();i++){
            nodes->at(i)->setFlag(QGraphicsItem::ItemIsSelectable);
            nodes->at(i)->installSceneEventFilter(nodes->at(i));

        }


    }

}

/*--------------------------
* desactive le scroll
--------------------------*/
void graph::wheelEvent( QWheelEvent * event ){

    event = NULL;
}




/*----------
*  SLOTS
-----------*/

void graph::zoomOrDezoom(){

    float zoomValue = (float)sliderZoom->value()/10000;
    this->setTransform(QTransform::fromScale(zoomValue,zoomValue));
    /*for(unsigned i =0;i<nodes->size();i++){
        node * n = nodes->at(i);
        n->deviceTransform(QTransform::fromScale(zoomValue,zoomValue));
    }*/
    this->update();
}




