#include "node.h"

const int MARGINHEIGHT = 10; //bordure entre le texte et le bord du carde (hauteur)
const int MARGINWIDTH  = 10; //borure entre le texte et le bord du cadre (largeur)
const int WIDTHBORDER  = 2;  //épaisseur de la bordure du rectangle
const QString FONTNAME = "SansSerif"; //nom de la police
const int FONTSIZE     = 14; //taille de la police d'écriture
const QString TEXTCOLOR = "#e6e6e6";
const QString BORDERCOLOR = "#5084d9";


node::node(int x, int y,QString name,QString lastId):QGraphicsItem(NULL)
{
    this->x = x;
    this->y = y;
    this->name = new QString(name);
    this->id = new QString(generateId(lastId));
    this->circledNode = false;


    //variables for drag & drop
        //du graph entier
        leftButtonPressed = false;
        dragEnter = false;
        lastDragPositionX = 0;
        lastDragPositionY = 0;
        lastSceneRectPositionX = 0;
        lastSceneRectPositionY = 0;

        //var for layouts
        circledLayouted = false;



}

node::~node(){

    delete id;
    delete name;
}

QString node::generateId(QString lastId){

    int idToInt = lastId.toInt();
    idToInt = idToInt +1;
    return QString::number(idToInt);
}

QString node::getName(){
 return *(this->name);
}

QString node::getId(){
   return *(this->id);
}
int node::getX(){
   return this->x;
}

int node::getY(){
    return this->y;
}

void node::setX(int x){

    this->x = x;
}

void node::setY(int y){

    this->y = y;
}

int node::getHeight(){

    int ret = 0;

    //hauteur du text + marge en haut et en bas
    if(!circledNode){

        ret =  (MARGINHEIGHT*2 + FONTSIZE-6); //le -6 est un probleme d'interpretation de la taille de la police, une police de taille 16 ne fera pas 16px de haut
    }
    else if(circledNode){

        ret = boundingRect().height();
    }

    return ret;
}
int node::getWidth(){
    int ret = 0;

    //calcule de la largeur du texte + marge de chaque coté
    if(!circledNode){

        QFontMetrics fm(FONTNAME);
        int widthName =fm.width(*(this->name));
        ret = (MARGINWIDTH*2 +  (widthName)*1.1-6);
    }
    else if(circledNode){

        ret = boundingRect().width();
    }

    return ret;
}

void node::setRectangleNode(){
    this->circledNode = false;
}
void node::setCircledNode(){
    this->circledNode = true;
}


void node::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget){
      option = NULL; //pour éviter un warning
      widget = NULL; //pour éviter un warning

      QPen pen;
      pen.setWidth(WIDTHBORDER);
      pen.setColor(BORDERCOLOR);

      QFont font(FONTNAME, FONTSIZE);
      QFontMetrics fm(font);
      int heightOfName = fm.height();
      int widthOfName = fm.width(*name);

      painter->setPen(pen);
      painter->setFont(font);

      QRectF rectangle = boundingRect();  //cree un rectangle avec la taille du text + une margé déinie en vairable globale

      if(!circledNode){
          painter->drawRect(rectangle);
      }
      else if(circledNode){
          painter->drawEllipse(rectangle);
      }

      pen.setColor(TEXTCOLOR);
      painter->setPen(pen);

      if(!circledNode){ //centrage du texte dans un rectangle
          //peindre un texte au centre avec un carde qui a une taille définit
          //painter->drawText(x+(WIDTH/2-widthOfName/2)-WIDTHBORDER/2,y+(HEIGHT/2+heightOfName/2)-WIDTHBORDER/2,*name);
          painter->drawText(x+MARGINWIDTH/2-WIDTHBORDER/2,y+MARGINHEIGHT/2+heightOfName/1.3-WIDTHBORDER/2,*name);
      }
      else if(circledNode){//centrage du texte dans un cercle

          painter->drawText(x+rectangle.width()/2-widthOfName/2,y+rectangle.height()/2+heightOfName/3,*name);
      }

}

QRectF node::boundingRect() const{

    QFont font(FONTNAME, FONTSIZE);
    QFontMetrics fm(font);
    int widthOfName = fm.width(*name); //récupération des dimenssion du text
    int heightOfName = fm.height();


    QRectF rectangle(x,y,MARGINWIDTH,MARGINHEIGHT); //cree un rectangle avec la taille du text + une margé déinie en vairable globale
    rectangle.setWidth(rectangle.width()+widthOfName);
    rectangle.setHeight(rectangle.height()+heightOfName);

    //si on veux des cercle on cree un crée avec le plus long segment
    //on veux des rond pas des ovales
    if(circledNode){

        if(rectangle.width() > rectangle.height()){

            rectangle.setHeight(rectangle.width());

        }
        else if(rectangle.height() > rectangle.width()){

            rectangle.setWidth(rectangle.height());
        }
    }

    return rectangle;
}

bool node::sceneEventFilter(QGraphicsItem *watched, QEvent *event){
    //qDebug() <<"event !!! = "<< event->type();

    watched = NULL;

    /*---------------------------
     *  Drag an item
     * -----------------------*/
           if(event->type() == QEvent::GraphicsSceneMousePress){


               if(QGraphicsSceneMouseEvent * mouseEvent = static_cast<QGraphicsSceneMouseEvent*>(event)){

                   if(mouseEvent->button() == Qt::LeftButton){

                        leftButtonPressed = true;
                   }
               }

               return true;

           }
           if(event->type() == QEvent::GraphicsSceneMouseRelease){

                 leftButtonPressed =false;
                 dragEnter = false;
                 return true;

           }
           if(event->type() == QEvent::GraphicsSceneMouseMove &&  leftButtonPressed){


                if(QGraphicsSceneMouseEvent * mouseEvent = static_cast<QGraphicsSceneMouseEvent*>(event)){

                    //si il n'y a pas encore de position initial on la cree a la position actuelle du curseur
                    //si non on peut bouger la scene
                    if(!dragEnter){
                        lastDragPositionX = mouseEvent->scenePos().x();
                        lastDragPositionY = mouseEvent->scenePos().y();
                        dragEnter = true;
                       //this->setDragMode(QGraphicsView::ScrollHandDrag);

                    }
                    else{

                        int currentX = this->getX();
                        int currentY = this->getY();
                        int newX = currentX-(lastDragPositionX-mouseEvent->scenePos().x());
                        int newY = currentY-(lastDragPositionY-mouseEvent->scenePos().y());                        
                        this->setX(newX);
                        this->setY(newY);

                        lastDragPositionX = mouseEvent->scenePos().x();
                        lastDragPositionY = mouseEvent->scenePos().y();
                        //this->scene()->views().at(0)->update(); //le repaint vas mettre a jour les coordonées du noeud de maniere graphique
                        this->removeFromIndex(); //obligatoire si non c'est plus draggable
                        this->addToIndex();      //obligatoire si non c'est plus draggable
                        this->scene()->views().at(0)->viewport()->update();

                    }
                }
                return true;
         }

        return false;
}







