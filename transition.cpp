#include "transition.h"

const int WIDTHLINE  = 2;  //épaisseur de la ligne
const QString FONTNAME = "SansSerif"; //nom de la police (nom de la transition)
const int FONTSIZE     = 10; //taille de la police d'écriture
const QString TEXTCOLOR = "#e6e6e6";
const QString LINECOLOR = "#5084d9";
static const double Pi = 3.14159265358979323846264338327950288419717;
static double TwoPi = 2.0 * Pi;

transition::transition(node * n1, node * n2,QString name,QString lastId)
{
    this->n1 = n1;
    this->n2 = n2;
    this->name = new QString(name);
    this->id = new QString(generateId(lastId));
    this->p1 = new QPointF(getP1());
    this->p2 = new QPointF(getP2());

    nodesAreCircles = false;
}

transition::~transition(){

    delete p1;
    delete p2;
    delete name;
    delete id;
   // delete n1; normalement pas besoin de les supprimé car stocké dans le graph et le graph les supprime
   // delete n2;
}

QString transition::generateId(QString lastId){

    int idToInt = lastId.toInt();
    idToInt = idToInt +1;
    return QString::number(idToInt);
}

QString transition::getName(){

    return *(this->name);
}

QString transition::getId(){
   return *(this->id);
}

node * transition::getN1(){

    return this->n1;
}
node * transition::getN2(){

    return this->n2;
}

void transition::setNodesAreCircles(){

 this->nodesAreCircles = true;
}

void transition::setNodesAreRectangles(){

    this->nodesAreCircles = false;
}

QPointF transition::getP1(){

    // = point de départ de la flèche
    QPointF retPoint;


    if(!nodesAreCircles){
        /*-------------------------------
        *  Pour des noeud rectangulaires
        *------------------------------*/

        /*
               (p1)        T       (p2)
                  +----------------+
                  |                |
                L |     Noeud      | R
                  |                |
                  +----------------+
              (p4)       B         (p3)


                 *   EQ(1)        * EQ(2)
                   *           *
                     *       *
                       *   *
                         *
                      *    *
                   *         *
                *              *

        */

        QPointF T(n1->getX()+(n1->getWidth()/2),n1->getY()); //centre du T du noeud1
        QPointF R(n1->getX()+n1->getWidth(),n1->getY()+(n1->getHeight()/2));
        QPointF B(n1->getX()+(n1->getWidth()/2),n1->getY()+n1->getHeight());
        QPointF L(n1->getX(),n1->getY()+(n1->getHeight()/2));

        QPointF middleN1(n1->getX()+n1->getWidth()/2,n1->getY()+n1->getHeight()/2); //centre du noeud1
        QPointF middleN2(n2->getX()+n2->getWidth()/2,n2->getY()+n2->getHeight()/2); //centre du noeud2

        QPointF p1(n1->getX(),n1->getY());
        QPointF p2(n1->getX()+n1->getWidth(),n1->getY());
        QPointF p3(n1->getX()+n1->getWidth(),n1->getY()+n1->getHeight());
        QPointF p4(n1->getX(),n1->getY()+n1->getHeight());


        /*****************************************
        * Methode avec fonctions x=y et -x=y (P1)
        *****************************************/


        float aEq1 = ( (p1.y()-p3.y()) / (p1.x() - p3.x()) );
        float bEq1 = ( (-(aEq1 *p3.x())) + (p3.y()) );

        float aEq2 = ( (p4.y()-p2.y()) / (p4.x() - p2.x()) );
        float bEq2 = ( (-(aEq2 *p2.x())) + (p2.y()) );


        if(middleN2.y() < (aEq1*middleN2.x() + bEq1) &&  middleN2.y() < (aEq2*middleN2.x() + bEq2) ){
            retPoint = T;
        }
        else if(middleN2.y() <= (aEq1*middleN2.x() + bEq1) &&  middleN2.y() >= (aEq2*middleN2.x() + bEq2)){
            retPoint = R;
        }
        else if(middleN2.y() > (aEq1*middleN2.x() + bEq1) &&  middleN2.y() > (aEq2*middleN2.x() + bEq2)){
            retPoint = B;
        }
        else if(middleN2.y() >= (aEq1*middleN2.x() + bEq1) &&  middleN2.y() <= (aEq2*middleN2.x() + bEq2)){
            retPoint = L;
        }


    }
    else if(nodesAreCircles){
        /*----------------------------
        *  Pour des noeud circulaires (P1)
        *--------------------------*/

            QPointF middleN1(n1->getX()+n1->getWidth()/2,n1->getY()+n1->getHeight()/2); //centre du noeud1
            QPointF middleN2(n2->getX()+n2->getWidth()/2,n2->getY()+n2->getHeight()/2); //centre du noeud2

            //petites valezurs necessaires, longueure vecteur , rayon , ration longueur/rayon
            float longueur_vecteur = sqrt(pow(middleN2.x()-middleN1.x(),2)+pow(middleN2.y()-middleN1.y(),2));
            float longueur_rayon = n1->getWidth()/2;
            float ratio = longueur_vecteur/longueur_rayon;

            //calcul des coordonée du vecteur entre le milieur de N1 et milieu de n2 (sous forme d'un qPoint car ce n'est que deux int)
            QPointF v_MiddleN1_MiddleN2(middleN2.x()-middleN1.x(),middleN2.y()-middleN1.y());

            //calcul des coordonée du vecteur du rayon de N1
            QPointF v_RayonN1(v_MiddleN1_MiddleN2.x()/ratio,v_MiddleN1_MiddleN2.y()/ratio);

            //puis ajout la taille du vecteur du rayon au centre du cercle pour avoir le point sur le bord du cercle
            QPointF point_sur_cercle(middleN1.x()+v_RayonN1.x(),middleN1.y()+v_RayonN1.y());


            retPoint = point_sur_cercle;
    }

    return retPoint;

}

QPointF transition::getP2(){


    // = point sur le noeud d'arriver ( ou il y a la flèche )
    QPointF retPoint;

    if(!nodesAreCircles){
        /*-------------------------------
        *  Pour des noeud rectangulaires
        *------------------------------*/

            /*
                      T
              +----------------+
              |                |
            L |     Noeud      | R
              |                |
              +----------------+
                     B

            */

        QPointF T(n2->getX()+(n2->getWidth()/2),n2->getY()); //centre du T du noeud1
        QPointF R(n2->getX()+n2->getWidth(),n2->getY()+(n2->getHeight()/2));
        QPointF B(n2->getX()+(n2->getWidth()/2),n2->getY()+n2->getHeight());
        QPointF L(n2->getX(),n2->getY()+(n2->getHeight()/2));

        QPointF middleN1(n1->getX()+n1->getWidth()/2,n1->getY()+n1->getHeight()/2); //centre du noeud1
        QPointF middleN2(n2->getX()+n2->getWidth()/2,n2->getY()+n2->getHeight()/2); //centre du noeud2

        QPointF p1(n2->getX(),n2->getY());
        QPointF p2(n2->getX()+n2->getWidth(),n2->getY());
        QPointF p3(n2->getX()+n2->getWidth(),n2->getY()+n2->getHeight());
        QPointF p4(n2->getX(),n2->getY()+n2->getHeight());


        /*******************************************
        * Methode avec fonctions x=y et -x=y (P2)
        *******************************************/

        float aEq1 = ( (p3.y()-p1.y()) / (p3.x() - p1.x()) );
        float bEq1 = ( (-(aEq1 *p1.x())) + (p1.y()) );

        float aEq2 = ( (p4.y()-p2.y()) / (p4.x() - p2.x()) );
        float bEq2 = ( (-(aEq2 *p2.x())) + (p2.y()) );


        if(middleN1.y() < (aEq1*middleN1.x() + bEq1) &&  middleN1.y() < (aEq2*middleN1.x() + bEq2) ){
            retPoint = T;
        }
        else if(middleN1.y() <= (aEq1*middleN1.x() + bEq1) &&  middleN1.y() >= (aEq2*middleN1.x() + bEq2)){
            retPoint = R;
        }
        else if(middleN1.y() > (aEq1*middleN1.x() + bEq1) &&  middleN1.y() > (aEq2*middleN1.x() + bEq2)){
            retPoint = B;
        }
        else if(middleN1.y() >= (aEq1*middleN1.x() + bEq1) &&  middleN1.y() <= (aEq2*middleN1.x() + bEq2)){
            retPoint = L;
        }
    }
    if(nodesAreCircles){
        /*----------------------------------
        *  Pour des noeud circulaires (P2)
        *---------------------------------*/

            QPointF middleN1(n1->getX()+n1->getWidth()/2,n1->getY()+n1->getHeight()/2); //centre du noeud1
            QPointF middleN2(n2->getX()+n2->getWidth()/2,n2->getY()+n2->getHeight()/2); //centre du noeud2

            //petites valezurs necessaires, longueure vecteur , rayon , ration longueur/rayon
            float longueur_vecteur = sqrt(pow(middleN2.x()-middleN1.x(),2)+pow(middleN2.y()-middleN1.y(),2));
            float longueur_rayon = n2->getWidth()/2;
            float ratio = longueur_vecteur/longueur_rayon;

            //calcul des coordonée du vecteur entre le milieur de N1 et milieu de n2 (sous forme d'un qPoint car ce n'est que deux int)
            QPointF v_MiddleN1_MiddleN2(middleN2.x()-middleN1.x(),middleN2.y()-middleN1.y());

            //calcul des coordonée du vecteur du rayon de N1
            QPointF v_RayonN2(v_MiddleN1_MiddleN2.x()/ratio,v_MiddleN1_MiddleN2.y()/ratio);

            //puis ajout la taille du vecteur du rayon au centre du cercle pour avoir le point sur le bord du cercle
            QPointF point_sur_cercle(middleN2.x()-v_RayonN2.x(),middleN2.y()-v_RayonN2.y());


            retPoint = point_sur_cercle;

    }

    return retPoint;

}


void transition::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget){

    option = NULL; //pour éviter un warning
    widget = NULL; //pour éviter un warning

    /*---------------------
    * Variables du painter
    ---------------------*/
    QPen pen;
    pen.setWidth(WIDTHLINE);
    pen.setColor(LINECOLOR);

    QBrush brush;
    brush.setColor(LINECOLOR);
    brush.setStyle(Qt::SolidPattern);

    QFont font(FONTNAME, FONTSIZE);
    QFontMetrics fm(font);
    int widthOfName = fm.width(*name); //récupération des dimenssion du text
   // int heightOfName = fm.height();


    painter->setPen(pen);
    painter->setFont(font);

    /*-----------------------------------------------------
    * dessine la ligne de du polygon (triangle de la fleche)
    ------------------------------------------------------*/

    /*-- IMPLIQUE QUE ON A CONSTRUIE L'OBJET AVEC LES NOEUD EN PARAMETTRE */
    QLineF line(getP1(),getP2());
    painter->drawLine(line);

    QPointF destPoint = line.p2().toPoint();
    int arrowSize =  6;
    double angle = ::acos(line.dx() / line.length());
    if (line.dy() >= 0){
        angle = TwoPi - angle;
    }

    QPointF destArrowP1 = destPoint + QPointF(    sin(angle - Pi / 3)      * arrowSize, cos(angle - Pi / 3)      * arrowSize);
    QPointF destArrowP2 = destPoint + QPointF(    sin(angle - Pi + Pi / 3) * arrowSize, cos(angle - Pi + Pi / 3) * arrowSize);

    QPolygonF tmp_polygon;
    tmp_polygon << line.p2() << destArrowP1 << destArrowP2;

    QPainterPath path;
    path.addPolygon(tmp_polygon);

    painter->drawPolygon(tmp_polygon);
    painter->fillPath(path, brush);

    /*------------------
    * dessine le texte
    -------------------*/

    int x = (line.p2().x() + line.p1().x())/2;
    int y =  (line.p2().y() + line.p1().y())/2;
    QPointF midlePoint(x,y);

    QString tmp_name = " "+(*name); //met un espace avant le nom afin de le décalé un peut de la ligne
    pen.setColor(TEXTCOLOR);
    painter->setPen(pen);
    painter->drawText(midlePoint.x()-widthOfName/2,midlePoint.y(),tmp_name);


}

QRectF transition::boundingRect() const{

    /*----------------------
    * Exactement le meme calcule que pour getP1 et getP2
    * Mais le const ne permet pas l'utilisation de
    * Fonctions internes à la classe
    -----------------------*/

    QPointF T1(n1->getX()+(n1->getWidth()/2),n1->getY()); //centre du T du noeud1
    QPointF R1(n1->getX()+n1->getWidth(),n1->getY()+(n1->getHeight()/2));
    QPointF B1(n1->getX()+(n1->getWidth()/2),n1->getY()+n1->getHeight());
    QPointF L1(n1->getX(),n1->getY()+(n1->getHeight()/2));

    QPointF T2(n2->getX()+(n2->getWidth()/2),n2->getY()); //centre du T du noeud1
    QPointF R2(n2->getX()+n2->getWidth(),n2->getY()+(n2->getHeight()/2));
    QPointF B2(n2->getX()+(n2->getWidth()/2),n2->getY()+n2->getHeight());
    QPointF L2(n2->getX(),n2->getY()+(n2->getHeight()/2));

    QPointF middleN1(n1->getX()+n1->getWidth()/2,n1->getY()+n1->getHeight()/2); //centre du noeud1
    QPointF middleN2(n2->getX()+n2->getWidth()/2,n2->getY()+n2->getHeight()/2); //centre du noeud2

    QPointF p1;
    QPointF p2;

    //si n1 est en dessou de n2
    if( middleN1.y() > middleN2.y() ){

        //si n1 est plus a gauche
        if( middleN1.x() < n2->getX()){

            p1 = L1;
        }
        //si n1 est plus a droite
        else if(middleN1.x() > n2->getX()+n2->getWidth()){

            p1 = R1;
        }
        //si non il est a peut pres au milieu
        else{

            p1 = B1;
        }

    }
    //si n1 est au dessus de n2
    else if( middleN1.y() < middleN2.y() ){

        //si n1 est plus a gauche
        if( middleN1.x() < n2->getX()){

            p1 = L1;
        }
        //si n1 est plus a droite
        else if(middleN1.x() > n2->getX()+n2->getWidth()){

            p1 = R1;
        }
        //si non il est a peut pres au milieu
        else{

            p1 = T1;
        }

    }
    //si les noeuds sont alignés horizontalement
    else if(middleN1.y() == middleN2.y()){

        //si n1 est plus a gauche
        if( middleN1.x() < n2->getX()){

            p1 = L1;
        }
        //si n1 est plus a droite
        else if(middleN1.x() > n2->getX()+n2->getWidth()){

            p1 = R1;
        }
    }
    //si les noeud sont alignés verticalement
    else if(middleN1.x() == middleN2.x()){

        //si n1 est au dessus de n2
        if(middleN1.y() < n2->getY()){
            p1 = T1;
        }
        //si n1 est en dessous de n2
        else if(middleN1.y() > n2->getY()+n2->getHeight()){
            p1 = B1;
        }
    }

    //si n1 est en dessou de n2
    if( middleN2.y() > middleN1.y() ){

        //si n1 est plus a gauche
        if( middleN2.x() < n1->getX()){

            p2 = L2;
        }
        //si n1 est plus a droite
        else if(middleN2.x() > n1->getX()+n1->getWidth()){

            p2 = R2;
        }
        //si non il est a peut pres au milieu
        else{

            p2 = B2;
        }

    }
    //si n1 est au dessus de n2
    else if( middleN2.y() < middleN1.y() ){

        //si n1 est plus a gauche
        if( middleN2.x() < n1->getX()){

            p2 = L2;
        }
        //si n1 est plus a droite
        else if(middleN2.x() > n1->getX()+n1->getWidth()){

            p2 = R2;
        }
        //si non il est a peut pres au milieu
        else{

            p2 = T2;
        }

    }
    //si les noeuds sont alignés horizontalement
    else if(middleN2.y() == middleN1.y()){

        //si n1 est plus a gauche
        if( middleN2.x() < n1->getX()){

            p2 = L2;
        }
        //si n1 est plus a droite
        else if(middleN2.x() > n1->getX()+n1->getWidth()){

            p2 = R2;
        }
    }
    //si les noeud sont alignés verticalement
    else if(middleN2.x() == middleN1.x()){

        //si n1 est au dessus de n2
        if(middleN2.y() < n1->getY()){
            p2 = T2;
        }
        //si n1 est en dessous de n2
        else if(middleN2.y() > n1->getY()+n1->getHeight()){
            p2 = B2;
        }
    }

    /*----------------------------
    *  Pour des noeud circulaires
    *--------------------------*/


   /* int xCenterNode1 = n1->getX()+n1->getWidth()/2;
    int yCenterNode1 = n1->getY()+n1->getHeight()/2;
    int xCenterNode2 = n2->getX()+n2->getWidth()/2;
    int yCenterNode2 = n2->getY()+n2->getHeight()/2;

    //determine les coordonés sur le cercle du 2e cercle
    int longueur_vecteur = sqrt(pow(xCenterNode2-xCenterNode1,2)+pow(yCenterNode2-yCenterNode1,2));
    int longueur_rayon = n2->getWidth()/2;
    int ratio = longueur_vecteur/longueur_rayon;
    int x1_2 = xCenterNode1-xCenterNode2; //corrdonée du grand vecteur p1->p2
    int y1_2 = yCenterNode1-yCenterNode2;
        //divise enssuite les coordonée du vecteur par le ratio
    int nx1_2 = x1_2/ratio;
    int ny1_2 = y1_2/ratio;

    int px2 = xCenterNode2+nx1_2;
    int py2 = yCenterNode2+ny1_2;

    //determine les coodronés sur le cercle du 1er crecle
    int px1 = nx1_2-xCenterNode1;
    int py1 = ny1_2-yCenterNode1;
    px1 = (-1)*px1;
    py1 = (-1)*py1;


    QPointF tmp_p1(px1,py1);
    QPointF tmp_p2(px2,py2);
    */

    QRectF rect(p1,p2);
    return rect;
}



