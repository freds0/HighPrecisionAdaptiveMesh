#include <cmath>
#include <vector> // ReverseCuthillMcKee
#include <algorithm> // ReverseCuthillMcKee
#include "mesh.h"
Mesh::Mesh()
{
    this->first             = 0;
    this->last              = 0;
    this->numberOfTriangles = 0;
    this->numberOfVertices  = 0;
    this->vertexFunction    = 0;
}

Mesh::~Mesh()
{
    /* Deletes the adjacencies. */
    list<Adjacency *>::iterator i;
    for( i = edges.begin(); i != edges.end(); ++i )
        delete (*i);
    /* Deletes the triangles. */
    list<Triangle *>::iterator it;
    for( it = this->triangleList.begin(); it != this->triangleList.end(); it++ )
        delete (*it);
    /* Deletes the vertices. */
    Vertex *itv = first;
    while(itv != 0)
    {
        Vertex *aux = itv->next;
        delete itv;
        itv = aux;
    }
}
////////////////////////////////////////////////////////////////////////////////

// "INITIALIZATION" methods

////////////////////////////////////////////////////////////////////////////////
void Mesh::initialize(string fileName)
{
    ifstream inputFile;
    inputFile.open(fileName.data());
    inputFile >> this->numberOfVertices;    /** Read the number os vertices. **/
    /** ___________________________ Creating the vertices ___________________________ **/
    Vertex *vertices[this->numberOfVertices];
    list<Vertex *> insertList; /* List of vertices to be inserted in the triangulation. */
    mpfr::real<MPFR_BITS_PRECISION> x = 0, y = 0;
    int verticesToRead = this->numberOfVertices;
    for(int i = 0; i < verticesToRead; ++i)
    {
        inputFile >> x; // coordinate x
        inputFile >> y; // coordinate y
        vertices[i] = createVertex(x,y);
        insertList.push_back(vertices[i]);
    }
    /** ___________________________ Creating the segments ___________________________ **/
    int numberOfSegments = 0;
    inputFile >> numberOfSegments; /** Reads the number of segments. **/
    for(int i = 0; i < numberOfSegments; ++i)
    {
        int v0 = 0, 
            v1 = 0;
        inputFile >> v0; /* Reads the number of the vertices tha are the endpoins of the segment. */
        inputFile >> v1;

        createAdjacency(vertices[v0], vertices[v1], 0, 0, ADJ_SEG_NOT_TRI);
    }
    inputFile.close();
    createInitialTriangulation(); /* Creates the bounding box with two triangles. */
    /** Inserts the input vertices. **/
    for( list<Vertex *>::iterator it = insertList.begin(); it != insertList.end(); ++it )
        insertVertice(*it, NULL);    
    splitEncroachedSegments();
}

void Mesh::createInitialTriangulation()
{
    Vertex *v = this->first;
    /** Gets the 'max' and 'min' coordinates of the PSLG. **/
    int xmax = 0, xmin = 0, ymax = 0, ymin = 0;    
    while( v != NULL )
    {      
        mpfr::real<MPFR_BITS_PRECISION> *x = NULL, *y = NULL; 
        x = &(v->x);
        y = &(v->y);
        if( *x > xmax )
            (*x).conv(xmax);//xmax = x;
        else if(*x < xmin)
            (*x).conv(xmin);//xmin = x;
        if( *y > ymax )
            (*y).conv(ymax);//ymax = y;
        else if( *y < ymin )
            (*y).conv(ymin);//ymin = y;
        v = v->next;
    }
    /** Set the distance between the bounding box and the 'extreme' vertices of the PSLG. **/
    mpfr::real<MPFR_BITS_PRECISION> x_aux = 0, y_aux = 0;
    int distX_aux = 0, distY_aux = 0;
    distX_aux = (xmax - xmin)/16;
    distY_aux = (ymax - ymin)/16;    
    xmax += distX_aux;
    xmin -= distX_aux;
    ymax += distY_aux;
    ymin -= distY_aux;
    /* Creating the four vertices of the bounding box. */
    x_aux = xmin;
    y_aux = ymin;
    Vertex *vb0 = createVertex(x_aux, y_aux);
    x_aux = xmax;
    y_aux = ymin;    
    Vertex *vb1 = createVertex(x_aux, y_aux);
    x_aux = xmin;
    y_aux = ymax;      
    Vertex *vb2 = createVertex(x_aux, y_aux);
    x_aux = xmax;
    y_aux = ymax;     
    Vertex *vb3 = createVertex(x_aux, y_aux);
    /* Creating the two triangles of the bounding box. */
    Triangle *t0 = createTriangle(vb0, vb1, vb2);
    Triangle *t1 = createTriangle(vb3, vb1, vb2);
    createAdjacency(vb1, vb2, t0, t1, 0);
    setNeighbors(t0, t1);
    createAdjacency(vb0, vb1, t0, 0, 1);
    createAdjacency(vb0, vb2, t0, 0, 1);
    createAdjacency(vb3, vb1, t1, 0, 1);
    createAdjacency(vb3, vb2, t1, 0, 1);
}
////////////////////////////////////////////////////////////////////////////

// "DELAUNAY PROP." methods

////////////////////////////////////////////////////////////////////////////////

int Mesh::maintainsDelaunay(Triangle *t, Adjacency *edge)
{
    int totalFlips = 0;    
    Triangle *t0 = edge->getTriangle(ADJ_TRIANGLE_ONE);
    Triangle *t1 = edge->getTriangle(ADJ_TRIANGLE_TWO);
    if(t0 != NULL && t1 != NULL)
    {
        int number_t1_v0 = 0, number_t1_v1 = 0;
        mpfr::real<MPFR_BITS_PRECISION> *distSQR = NULL;
        if( t == t1 )
        {
            t1 = t0;
            t0 = t;
        }
        Vertex *v0 = edge->getVertex(ADJ_VERTEX_ONE);
        Vertex *v1 = edge->getVertex(ADJ_VERTEX_TWO);       
        number_t1_v0 = t1->getNumber(v0);
        number_t1_v1 = t1->getNumber(v1);
        /* Vertex of t1 that is not a vertex of the shared edge. */
        Vertex *vt1 = t1->getVertex( 3 - (number_t1_v0 + number_t1_v1) );
        /* Edges to be tested if the current edge is not a Delaunay edge. */
        Adjacency *ed0 = v0->getAdjacency(vt1);
        Adjacency *ed1 = v1->getAdjacency(vt1);
        /* Distance between the circumcenter of t0 and the vertex vt1 of t1. */        
        distSQR = Utils::distanceSQR(t0->cCenter[COORDINATE_X], t0->cCenter[COORDINATE_Y], vt1);
        if(*distSQR - t0->sqrRadius < 0) /* The raidius is larger, isEncroached = true */
        {    
            delete distSQR;
	    distSQR = NULL;
            // Flip of edges
            Adjacency *newEdge = swap(edge);
            totalFlips++;
            Triangle *nt0 = newEdge->getTriangle(ADJ_TRIANGLE_ONE);
            Triangle *nt1 = newEdge->getTriangle(ADJ_TRIANGLE_TWO);
            if(ed0->getTriangle(ADJ_TRIANGLE_ONE) == nt1 or ed0->getTriangle(ADJ_TRIANGLE_TWO) == nt1)
            {
                Triangle *aux = nt0;
                nt0 = nt1;
                nt1 = aux;
            }
            totalFlips += maintainsDelaunay(nt0, ed0);
            totalFlips += maintainsDelaunay(nt1, ed1);               
            return totalFlips;
        }
        delete distSQR;
    }
    return totalFlips;
}

int Mesh::maintainsDelaunay()
{
    int numberFlips = 0;
    this->toFlip.clear();
    for( list<Adjacency *>::iterator it = this->edges.begin(); it != this->edges.end(); ++it )
    {    
        mpfr::real<MPFR_BITS_PRECISION> *distSQR = NULL;
        Triangle *t0 = NULL, *t1 = NULL;     
        t0 = (*it)->getTriangle(ADJ_TRIANGLE_ONE);
        t1 = (*it)->getTriangle(ADJ_TRIANGLE_TWO);        
        Vertex *v0 = (*it)->getVertex(ADJ_VERTEX_ONE);
        Vertex *v1 = (*it)->getVertex(ADJ_VERTEX_TWO);               
        if ( (t0 != NULL) && (t1 != NULL) )
        {
            int number_t1_v0 = 0 , number_t1_v1 = 0;
            number_t1_v0 = t1->getNumber(v0);
            number_t1_v1 = t1->getNumber(v1);            
            /* Vertex of t1 that is not a vertex of the shared edge. */
            Vertex *vt1 = t1->getVertex( 3 - (number_t1_v0 + number_t1_v1) );            
            /* Distance between the circumcenter of t0 and the vertex vt1 of t1. */        
            distSQR = Utils::distanceSQR(t0->cCenter[COORDINATE_X], t0->cCenter[COORDINATE_Y], vt1);            
            if(*distSQR - t0->sqrRadius < 0) /* The raidius is larger, isEncroached = true */                    
                this->toFlip.push_back(*it);
            delete distSQR;
        }        
    }
//    int numberFlips = 0;
//    while ( !this->toFlip.empty() )
//    {
//        Triangle *t0 = NULL, *t1 = NULL;  
//        Adjacency *adj = this->toFlip.front();
//        this->toFlip.pop_front();    
//        t0 = adj->getTriangle(ADJ_TRIANGLE_ONE);
//        t1 = adj->getTriangle(ADJ_TRIANGLE_TWO);    
//        if (t0 != NULL) 
//        {
//            numberFlips += maintainsDelaunay(t0, adj);
//        }
//        else if (t1 != NULL)
//        {
//            numberFlips += maintainsDelaunay(t1, adj);
//        }
//    }   
    if (CONFIG_DEBUG_MODE)     
    {
        if (this->smartCheckMesh() )
            cout << "Malha OK." << endl;
        else 
            cout << "ERRO Malha. " << endl;                 
    }
    while ( !this->toFlip.empty() )
    {
        Triangle *t1 = NULL;  
        Adjacency *adj = this->toFlip.front();
        this->toFlip.pop_front();    
        t1 = adj->getTriangle(ADJ_TRIANGLE_TWO);    
        Vertex *v0 = adj->getVertex(ADJ_VERTEX_ONE);
        Vertex *v1 = adj->getVertex(ADJ_VERTEX_TWO);  
        int number_t1_v0 = 0 , number_t1_v1 = 0;
        number_t1_v0 = t1->getNumber(v0);
        number_t1_v1 = t1->getNumber(v1);
        /* Vertex of t1 that is not a vertex of the shared edge. */
        Vertex *vt1 = t1->getVertex( 3 - (number_t1_v0 + number_t1_v1) );
        /* Edges to be tested if the current edge is not a Delaunay edge. */
        Adjacency *ed0 = v0->getAdjacency(vt1);
        Adjacency *ed1 = v1->getAdjacency(vt1);        
        // Flip of edges
        swap(adj);
        numberFlips++;
         if ( needsToBeFlipped(ed0) ) 
            this->toFlip.push_front(ed0);
        if ( needsToBeFlipped(ed1) ) 
            this->toFlip.push_front(ed1);        
    }          
    return numberFlips;        
}

Adjacency *Mesh::swap(Adjacency *edge)
{  
    /** Getting the necessary data.  **/
    Triangle *t0 = edge->getTriangle(ADJ_TRIANGLE_ONE);
    Triangle *t1 = edge->getTriangle(ADJ_TRIANGLE_TWO);
    Vertex *ve0 = edge->getVertex(ADJ_VERTEX_ONE);
    Vertex *ve1 = edge->getVertex(ADJ_VERTEX_TWO);
    int number_ve0_t0 = 0, number_ve1_t0 = 0, number_ve0_t1 = 0, number_ve1_t1 = 0;
    number_ve0_t0 = t0->getNumber(ve0);
    number_ve1_t0 = t0->getNumber(ve1);
    number_ve0_t1 = t1->getNumber(ve0);
    number_ve1_t1 = t1->getNumber(ve1);
    Triangle *neig_t0_ve0 = t0->getNeighbor(number_ve0_t0);
    Triangle *neig_t0_ve1 = t0->getNeighbor(number_ve1_t0);
    Triangle *neig_t1_ve0 = t1->getNeighbor(number_ve0_t1);
    Triangle *neig_t1_ve1 = t1->getNeighbor(number_ve1_t1);
    /* Vertices of the new edge. */
    Vertex *vt0 = t0->getVertex( 3 - (number_ve0_t0 + number_ve1_t0) );
    Vertex *vt1 = t1->getVertex( 3 - (number_ve0_t1 + number_ve1_t1) );
    /**______________________________________________________________________**/
    /* Deleting the old triangles. */
    deleteTriangle(t0);
    deleteTriangle(t1);
    /* Creating the new triangles which will be formed with the swap */
    Triangle *newT0 = createTriangle(ve0, vt1, vt0);
    Triangle *newT1 = createTriangle(ve1, vt1, vt0);
    Adjacency *newEdge = createAdjacency(vt0, vt1, newT0, newT1, ADJ_SEG_ONLY_EDGE); /* The new edge. */
    this->removeEdge(edge);/* Removes the old edge. */
    /* Updating the four edges of the quadrilateral formed by the two triangles. */
    Adjacency *adj;
    adj = ve0->getAdjacency(vt0);
    adj->addTriangle(newT0);
    adj = ve0->getAdjacency(vt1);
    adj->addTriangle(newT0);
    adj = ve1->getAdjacency(vt0);
    adj->addTriangle(newT1);
    adj = ve1->getAdjacency(vt1);
    adj->addTriangle(newT1);
    /** _____________ Setting the neighborhood _____________ **/
    setNeighbors(newT1, newT0);
    if (neig_t0_ve1 != NULL) 
        setNeighbors(newT0, neig_t0_ve1);
    if (neig_t1_ve1 != NULL) 
        setNeighbors(newT0, neig_t1_ve1);
    if (neig_t0_ve0 != NULL) 
        setNeighbors(newT1, neig_t0_ve0);
    if (neig_t1_ve0 != NULL) 
        setNeighbors(newT1, neig_t1_ve0);
    return newEdge; /* Returns the new edge. */
}
////////////////////////////////////////////////////////////////////////////////

// "VERTEX INSERTION" methods

////////////////////////////////////////////////////////////////////////////////
bool Mesh::insertVertice(Vertex *v, Triangle *t)
{
    Triangle *containingTriangle;
    /* Find the triangle that contains the vertex. */
    if(t == NULL)
        containingTriangle = this->findTriangle(v);
    else
        containingTriangle = this->findTriangle(t, v->x, v->y);
    if (containingTriangle == NULL) 
    {        
        cerr << "ERROR - Mesh::insertVertice - don't find triangle containing vertex - x: " << v->x << " y: "<< v->y << endl;
        return false;
    }
    // Updates u value of u to the created vertex v
    Vertex *nearestVertex = t->getNearestVertexInsideTriangle(v->x, v->y, containingTriangle);            
    v->u = nearestVertex->u;
    v->previews_u = nearestVertex->previews_u;   
    Vertex *v0 = containingTriangle->getVertex(TRIANGLE_VERTEX_ONE);
    Vertex *v1 = containingTriangle->getVertex(TRIANGLE_VERTEX_TWO);
    Vertex *v2 = containingTriangle->getVertex(TRIANGLE_VERTEX_THREE);
    if ( ( (v->x == v0->x) && (v->y == v0->y) ) || 
         ( (v->x == v1->x) && (v->y == v1->y) ) ||
         ( (v->x == v2->x) && (v->y == v2->y) ) )
    {
        cerr << "ERROR - Mesh::insertVertice - Vertex already exists- x: " << v->x << " y: "<< v->y << endl;
        return false;        
    }
    mpfr::real<MPFR_BITS_PRECISION> *orientation1 = NULL, *orientation2 = NULL, *orientation3 = NULL;
    orientation1 = Utils::orientation(v->x, v->y, v0, v1);
    *orientation1 = fabs(*orientation1);
    orientation2 = Utils::orientation(v->x, v->y, v0, v2);
    *orientation2 = fabs(*orientation2);
    orientation3 = Utils::orientation(v->x, v->y, v1, v2);
    *orientation3 = fabs(*orientation3);
    //
    // Checks whether the point is located on an edge of the triangle. 
    // If true, divide the edge. Otherwise, divide the triangle.
    //
    // TODO verificar precisao desse valor
//    if( orientation1 < 0.00001 ) /* v, v0, v1 are collinear. */
    if( *orientation1 < 1e-20 )
    {
        Adjacency *adj = v0->getAdjacency(v1);
        splitEdge(v, adj);
    }
    // TODO verificar precisao desse valor
//    else if( orientation2 < 0.00001 ) /* v, v0, v2 are collinear. */
    else if( *orientation2 <  1e-20 )
    {
        Adjacency *adj = v0->getAdjacency(v2);
        splitEdge(v, adj);
    }
    // TODO verificar precisao desse valor
//    else if( orientation3  < 0.00001 ) /* v, v1, v2 are collinear. */
    else if( *orientation3  <  1e-20)
    {
        Adjacency *adj = v1->getAdjacency(v2);
        splitEdge(v, adj);
    }
    // Divide the triangle.
    else
    {
        splitTriangle(v, containingTriangle);
    }
    delete orientation1;
    delete orientation2;
    delete orientation3;
    return true;
}

void Mesh::insertCircuncenterOff(Triangle *t, mpfr::real<MPFR_BITS_PRECISION> *offCenter)
{
    Vertex *v;
    if( offCenter == NULL )    /** To insert in the circumcenter. **/
        v = createVertex(t->cCenter[COORDINATE_X], t->cCenter[COORDINATE_Y]);
    else                    /** To insert in the off-center. **/
        v = createVertex(offCenter[COORDINATE_X], offCenter[COORDINATE_Y]);    
    bool inserted = insertVertice(v, t);
    if (!inserted) 
    {
        this->toRefineByAngleCriterion.remove(t);
        this->toRefineByAdjCriterion.remove(t);
        this->deleteVertex(v);
    }
}

void Mesh::calculateOffCenter(Triangle *t, mpfr::real<MPFR_BITS_PRECISION> &b, mpfr::real<MPFR_BITS_PRECISION> *offCenter)
{
    /** SHORTEST EDGE **/
    Adjacency *edge = t->getSmallerEdge();
    Vertex *v0 = edge->getVertex(ADJ_VERTEX_ONE);
    Vertex *v1 = edge->getVertex(ADJ_VERTEX_TWO);
    /** Temporary vertex and triangle to verify if the off-center is the same as the circumcenter **/
    Vertex *vtmp = new Vertex(t->cCenter[COORDINATE_X], t->cCenter[COORDINATE_Y], 0);
    Triangle *tmp = new Triangle(v0, v1, vtmp);
    if( tmp->radiusEdgeRatio <= b ) /** Off-center = circuncenter. **/
    {
        offCenter[COORDINATE_X] = t->cCenter[COORDINATE_X];
        offCenter[COORDINATE_Y] = t->cCenter[COORDINATE_Y];
    }
    else
    {
        /* HALF/LENGTH OF THE SHORTEST EDGE */
        mpfr::real<MPFR_BITS_PRECISION> *length = Utils::distance(v0,v1);
        mpfr::real<MPFR_BITS_PRECISION> hlength = (*length)/2.0;
        /* MIDPOINT OF THE SHORTEST EDGE */
        mpfr::real<MPFR_BITS_PRECISION> midPoint[2];
        midPoint[COORDINATE_X] = ( v0->x + v1->x )/2.0;
        midPoint[COORDINATE_Y] = ( v0->y + v1->y )/2.0;
        /* NORMALIZED VECTOR FROM DE MIDPOINT TO THE CIRCUMCENTER */
        mpfr::real<MPFR_BITS_PRECISION> perpVector[2];
        mpfr::real<MPFR_BITS_PRECISION> *lvector = Utils::distance(t->cCenter[COORDINATE_X], t->cCenter[COORDINATE_Y], midPoint[COORDINATE_X], midPoint[COORDINATE_Y]);
        perpVector[COORDINATE_X] = (t->cCenter[COORDINATE_X] - midPoint[COORDINATE_X])/(*lvector);
        perpVector[COORDINATE_Y] = (t->cCenter[COORDINATE_Y] - midPoint[COORDINATE_Y])/(*lvector);
	delete lvector;
        /* RADIUS OF THE CIRCUMCIRCLE OF THE TRIANGLE WITH THE SHORTEST EDGE AND THE OFF-CENTER */
        mpfr::real<MPFR_BITS_PRECISION> radius = b * (*length);
	delete length;
        /* Distance between the midPoint and the circumcenter of the new triangle */
        mpfr::real<MPFR_BITS_PRECISION> radius1 = sqrt( (radius*radius) - (hlength*hlength) );
        /* */
        perpVector[COORDINATE_X] = perpVector[COORDINATE_X]*(radius1+radius);
        perpVector[COORDINATE_Y] = perpVector[COORDINATE_Y]*(radius1+radius);
        /* */
        offCenter[COORDINATE_X] = perpVector[COORDINATE_X] + midPoint[COORDINATE_X];
        offCenter[COORDINATE_Y] = perpVector[COORDINATE_Y] + midPoint[COORDINATE_Y];
    }
    delete tmp;
    delete vtmp;
}
////////////////////////////////////////////////////////////////////////////////

// RETRIANGULATION" methods

////////////////////////////////////////////////////////////////////////////////
/**
 * Lawsons's algorithm.
 */
void Mesh::splitTriangle(Vertex *v, Triangle *t)
{
    /** Getting the necessaty data. **/
    Vertex *v0 = t->getVertex(TRIANGLE_VERTEX_ONE);
    Vertex *v1 = t->getVertex(TRIANGLE_VERTEX_TWO);
    Vertex *v2 = t->getVertex(TRIANGLE_VERTEX_THREE);
    Triangle *t0 = t->getNeighbor(TRIANGLE_VERTEX_ONE);
    Triangle *t1 = t->getNeighbor(TRIANGLE_VERTEX_TWO);
    Triangle *t2 = t->getNeighbor(TRIANGLE_VERTEX_THREE);
    deleteTriangle(t);
    /*_______________________________*/
    /** THE NEW TRIANGLES. **/
    Triangle *newT1 = createTriangle(v, v0, v1);
    Triangle *newT2 = createTriangle(v, v0, v2);
    Triangle *newT3 = createTriangle(v, v1, v2);
    /** Setting the neighborhood. **/
    setNeighbors(newT1, newT2);
    setNeighbors(newT1, newT3);
    setNeighbors(newT2, newT3);
    if (t0 != NULL) 
        setNeighbors(newT3, t0);
    if (t1 != NULL) 
        setNeighbors(newT2, t1);
    if (t2 != NULL)
        setNeighbors(newT1, t2);
    /** _________________________ Setting adjacency lists. _________________________ **/
    createAdjacency(v, v0, newT1, newT2, 0);
    createAdjacency(v, v1, newT1, newT3, 0);
    createAdjacency(v, v2, newT2, newT3, 0);
    Adjacency *adj0 = v0->getAdjacency(v1);
    adj0->addTriangle(newT1);
    Adjacency *adj1 = v0->getAdjacency(v2);
    adj1->addTriangle(newT2);
    Adjacency *adj2 = v1->getAdjacency(v2);
    adj2->addTriangle(newT3);
    /* Maintaining the Delaunay properties. **/
    maintainsDelaunay(newT1, adj0);
    maintainsDelaunay(newT2, adj1);
    maintainsDelaunay(newT3, adj2);
}

void Mesh::splitEdge(Vertex *v, Adjacency *edge)
{
    /** Getting the necessary data. **/
    Triangle *t0 = edge->getTriangle(ADJ_TRIANGLE_ONE);
    Triangle *t1 = edge->getTriangle(ADJ_TRIANGLE_TWO);
    Vertex *ve0 = edge->getVertex(ADJ_VERTEX_ONE);
    Vertex *ve1 = edge->getVertex(ADJ_VERTEX_TWO);
    /**_____________________________**/
    /** Dividing the edge into two new edges **/
    Adjacency *adj_v_ve0 = createAdjacency(v, ve0, 0, 0, edge->type);
    Adjacency *adj_v_ve1 = createAdjacency(v, ve1, 0, 0, edge->type);
    this->edges.remove(edge);
    this->encroachedS.remove(edge);
    if(edge->type == ADJ_SEG_IN_TRI)
        this->segments.remove(edge);
    delete edge;
    /* Pointers to the new triangles. */
    Triangle *newT0 = NULL, *newT1 = NULL, *newT2 = NULL, *newT3 = NULL;
    /* Pointers to the new edges. */
    Adjacency *adj0, *adj1, *adj2, *adj3;
    if(t0 != NULL) /* If there is the triangles 't0'. */
    {
        int number_ve0_t0 = t0->getNumber(ve0);
        int number_ve1_t0 = t0->getNumber(ve1);
        Vertex *vt0 = t0->getVertex( 3 - (number_ve0_t0 + number_ve1_t0) );
        Triangle *neighbor0 = t0->getNeighbor(number_ve0_t0);
        Triangle *neighbor1 = t0->getNeighbor(number_ve1_t0);
        deleteTriangle(t0);
        /** NEW TRIANGLES **/
        newT0 = createTriangle( v, ve0, vt0 );
        newT1 = createTriangle( v, ve1, vt0 );
        setNeighbors(newT0, newT1);
        if (neighbor0 != NULL) 
            setNeighbors(newT1, neighbor0);
        if (neighbor1 != NULL) 
            setNeighbors(newT0, neighbor1);
        /* _________________________ Setting adjacency list _________________________ */
        createAdjacency(v, vt0, newT0, newT1, 0);
        adj_v_ve0->addTriangle(newT0);
        adj_v_ve1->addTriangle(newT1);
        adj0 = ve0->getAdjacency(vt0);
        adj0->addTriangle(newT0);
        adj1 = ve1->getAdjacency(vt0);
        adj1->addTriangle(newT1);
    }
    if(t1 != NULL) /* If there is the triangles 't1'. */
    {
        int number_ve0_t1 = t1->getNumber(ve0);
        int number_ve1_t1 = t1->getNumber(ve1);
        Vertex *vt1 = t1->getVertex( 3 - (number_ve0_t1 + number_ve1_t1) );
        Triangle *neighbor0 = t1->getNeighbor(number_ve0_t1);
        Triangle *neighbor1 = t1->getNeighbor(number_ve1_t1);
        deleteTriangle(t1);
        /** NEW TRIANGLES. **/
        newT2 = createTriangle(v, ve0, vt1);
        newT3 = createTriangle(v, ve1, vt1);
        setNeighbors(newT2, newT3);
        if (neighbor0 != NULL) 
            setNeighbors(newT3, neighbor0);
        if (neighbor1 != NULL) 
            setNeighbors(newT2, neighbor1);
        if (newT0 != NULL) 
            setNeighbors(newT0, newT2);
        if (newT1 != NULL) 
            setNeighbors(newT1, newT3);
        /* _________________________ Setting adjacency list _________________________ */
        createAdjacency(v, vt1, newT2, newT3, 0);
        adj_v_ve0->addTriangle(newT2);
        adj_v_ve1->addTriangle(newT3);
        adj2 = ve0->getAdjacency(vt1);
        adj2->addTriangle(newT2);
        adj3 = ve1->getAdjacency(vt1);
        adj3->addTriangle(newT3);
        maintainsDelaunay(newT2, adj2);
        maintainsDelaunay(newT3, adj3);
    }
    if(newT0 != NULL)
    {
        maintainsDelaunay(newT0, adj0);
        maintainsDelaunay(newT1, adj1);
    }
}
////////////////////////////////////////////////////////////////////////////////

// "DELAUNAY REFINEMENT" methods

////////////////////////////////////////////////////////////////////////////////
int Mesh::refine(mpfr::real<MPFR_BITS_PRECISION> &B, int refinementType)
{
    toRefineByAngleCriterion = triangleList;
    int totalRefined = 0;
    while( !(toRefineByAngleCriterion.empty()) )
    {
        splitEncroachedSegments(); /** Splits all encroached segments. **/
        Triangle *t = toRefineByAngleCriterion.front(); /** Get a candidate to refinement. **/    
//        if( t->angles[TRIANGLE_VERTEX_ONE]   < angle || 
//            t->angles[TRIANGLE_VERTEX_TWO]   < angle || 
//            t->angles[TRIANGLE_VERTEX_THREE] < angle ) 
//        {
        if (t->radiusEdgeRatio > B) /** If the triangle need to be refined. **/
        {
            totalRefined++;
            if( refinementType == MESH_RUPPERT_REFINEMENT ) /** Ruppert refinement. **/
            {
                if( !(splitEncroachedSegments( t->cCenter[COORDINATE_X], t->cCenter[COORDINATE_Y] ) ) )
                    insertCircuncenterOff(t, NULL);
            }
            else /**  Off-center refinement. **/
            {
                mpfr::real<MPFR_BITS_PRECISION> *offCenter = new mpfr::real<MPFR_BITS_PRECISION>[2];
                offCenter[COORDINATE_X] = 0;
                offCenter[COORDINATE_Y] = 0;
                calculateOffCenter(t, B, offCenter);
                if( !(splitEncroachedSegments( offCenter[COORDINATE_X], offCenter[COORDINATE_Y] ) ) )
                    insertCircuncenterOff(t, offCenter);
                delete[] offCenter;
            }
        }
        else
            toRefineByAngleCriterion.pop_front(); /** Removes the triangles only if is a good triangle (to avoid two removals). **/
    }
    if (CONFIG_DEBUG_MODE) 
        cout << "toRefineByAngleCriterion.size(): " << totalRefined << " ";
    return totalRefined;
}

void Mesh::refineTriangle(Triangle *t )
{
    /** Ruppert refinement. **/ 
    if( !(splitEncroachedSegments( t->cCenter[COORDINATE_X], t->cCenter[COORDINATE_Y] ) ) )
        insertCircuncenterOff(t, NULL);
    splitEncroachedSegments();

}
////////////////////////////////////////////////////////////////////////////////

// "ENCROACHED" methods

////////////////////////////////////////////////////////////////////////////////
bool Mesh::splitEncroachedSegments(mpfr::real<MPFR_BITS_PRECISION> px, mpfr::real<MPFR_BITS_PRECISION> py)
{
    bool r = false;
    list<Adjacency *> encroached1 = this->segments;
    while( !(encroached1.empty()) ) /* Split all encroached segments */
    {
        Adjacency *seg = encroached1.front();
        encroached1.pop_front(); /* Remove the segment */
        if( seg->type != ADJ_SEG_NOT_TRI && this->isEncroached(px, py, seg) ) /* If the segment is encroached. */
        {
            r = true;
            Vertex *v0 = seg->getVertex(ADJ_VERTEX_ONE);
            Vertex *v1 = seg->getVertex(ADJ_VERTEX_TWO);
            mpfr::real<MPFR_BITS_PRECISION> x = (v0->x + v1->x)/2;
            mpfr::real<MPFR_BITS_PRECISION> y = (v0->y + v1->y)/2;
            Vertex *v = createVertex( x, y );
            this->splitEdge(v, seg);
            Adjacency *newSeg0 = v->getAdjacency(v0);
            Adjacency *newSeg1 = v->getAdjacency(v1);
            if(newSeg0->type == ADJ_SEG_IN_TRI)
                encroached1.push_back(newSeg0);
            if(newSeg1->type == ADJ_SEG_IN_TRI)
                encroached1.push_back(newSeg1);
        }
    }
    return r;
}

void Mesh::splitEncroachedSegments()
{
    while( !(this->encroachedS.empty()) )
    {
        Adjacency *segment = this->encroachedS.front();
        Vertex *v0 = segment->getVertex(ADJ_VERTEX_ONE);
        Vertex *v1 = segment->getVertex(ADJ_VERTEX_TWO);
        mpfr::real<MPFR_BITS_PRECISION> x = (v0->x + v1->x)/2;
        mpfr::real<MPFR_BITS_PRECISION> y = (v0->y + v1->y)/2;
        Vertex *v = createVertex(x, y);
        if(segment->type == ADJ_SEG_NOT_TRI) // is a segment, but not in the triangulation
        {
            Triangle *t = v0->incList.front();
            createAdjacency(v, segment->getVertex(ADJ_VERTEX_ONE), 0, 0, ADJ_SEG_NOT_TRI);
            createAdjacency(v, segment->getVertex(ADJ_VERTEX_TWO), 0, 0, ADJ_SEG_NOT_TRI);
            removeEdge(segment);
            this->insertVertice(v, t);
        }
        else
            this->splitEdge(v, segment);
    }
}

bool Mesh::isEncroached(Adjacency *seg)
{
    Vertex *v0 = seg->getVertex(ADJ_VERTEX_ONE);
    Vertex *v1 = seg->getVertex(ADJ_VERTEX_TWO);
    int number_v0, number_v1, number_v3;
    Triangle *tmp = seg->getTriangle(ADJ_TRIANGLE_ONE);
    if(tmp == NULL)
        tmp = seg->getTriangle(ADJ_TRIANGLE_TWO);
    number_v0 = tmp->getNumber(v0);
    number_v1 = tmp->getNumber(v1);
    number_v3 = 3 - (number_v0 + number_v1);
    if( tmp->angles[number_v3] > 90 ) /* Verify if the segment opposite to a obtuse angle. */
        return true;
    return false;
}

bool Mesh::isEncroached(mpfr::real<MPFR_BITS_PRECISION> &px, mpfr::real<MPFR_BITS_PRECISION> &py, Adjacency *seg)
{   
    Vertex *v0 = seg->getVertex(ADJ_VERTEX_ONE);
    Vertex *v1 = seg->getVertex(ADJ_VERTEX_TWO);
    mpfr::real<MPFR_BITS_PRECISION> centerX = 0, centerY = 0;
    centerX = (v0->x + v1->x)/2.0;
    centerY = (v0->y + v1->y)/2.0;
    mpfr::real<MPFR_BITS_PRECISION> *radiusSQR = Utils::distanceSQR(centerX, centerY, v0);
    mpfr::real<MPFR_BITS_PRECISION> *distSqrToPoint = Utils::distanceSQR(px, py, centerX, centerY);
    bool r = ( (*distSqrToPoint - *radiusSQR) < 0);
    delete distSqrToPoint;
    delete radiusSQR;
    if (r)
        return true;    
    return false;
}
////////////////////////////////////////////////////////////////////////////////

// "SIZE" REFINEMENT methods

////////////////////////////////////////////////////////////////////////////////
bool Mesh::applyRefinement( functionAdj *adjCriterion, functionGetTriangle *getTriangle )
{
    bool refined = false;
    this->toRefineByAdjCriterion.clear();    
    for( list<Adjacency *>::iterator it = edges.begin(); it != edges.end(); ++it )
    {       
        if( (*it)->type != ADJ_SEG_IN_TRI && adjCriterion(*it) )
        {            
            Triangle *t = getTriangle(*it);
	    if( !(Utils::contains(this->toRefineByAdjCriterion, t)) ) 
		this->toRefineByAdjCriterion.push_back(t); /** Get all triangles that need to be refined **/    
        } 
    }  
    if (CONFIG_DEBUG_MODE)
        cout << "toRefineByAdjCriterion.size(): " << this->toRefineByAdjCriterion.size() << endl;
    while( !(this->toRefineByAdjCriterion.empty()) )
    {
      
        Triangle *t = this->toRefineByAdjCriterion.front();
	refineTriangle(t);

        refined = true;
    }
    return refined;
}

bool Mesh::applyRefinement(functionTri *triCriterion)
{
    bool refined = false;
    this->toRefineByAdjCriterion.clear();    
    for( list<Triangle *>::iterator it = triangleList.begin(); it != triangleList.end(); ++it )
    {
        if( triCriterion(*it) )
        {
            if( !(Utils::contains(this->toRefineByAdjCriterion, *it)) ) 
                this->toRefineByAdjCriterion.push_back(*it);  /** Get all triangles that need to be refined **/
        }
    }
    while( !(this->toRefineByAdjCriterion.empty()) )
    {
        refineTriangle(this->toRefineByAdjCriterion.front());
        refined = true;
    }
    return refined;
}
////////////////////////////////////////////////////////////////////////////////

// 'REPLACE'/'SET' methods

////////////////////////////////////////////////////////////////////////////////
void Mesh::replaceEdgeTriangle(Adjacency *edge, Triangle *oldT, Triangle *newT)
{
    edge->removeTriangle(oldT); /* Removes the old neighbor. */
    edge->addTriangle(newT);    /* Adds the new neighbor. */
}

void Mesh::replaceEdgeTriangle(Vertex *v0, Vertex *v1, Triangle *oldT, Triangle *newT)
{
    Adjacency *edge = v0->getAdjacency(v1);
    edge->removeTriangle(oldT); /* Removes the old neighbor. */
    edge->addTriangle(newT);    /* Adds the new neighbor. */
}

void Mesh::setNeighbors(Triangle *t0, Triangle *t1)
{
    if( t0 != NULL && t1 != NULL )
    {
        t0->setNeighbor(t1);
        t1->setNeighbor(t0);
    } 
    else 
        cerr << "ERROR - Mesh::setNeighbors - Neighbors == NULL" << endl;        
}
////////////////////////////////////////////////////////////////////////////////

// 'DELETE'/'CREATE' methods

////////////////////////////////////////////////////////////////////////////////
/* _____________________VERTEX_____________________ */
void Mesh::setVertexFunction(functionV *function)
{
    this->vertexFunction = function;
}

Vertex *Mesh::createVertex(mpfr::real<MPFR_BITS_PRECISION> &x, mpfr::real<MPFR_BITS_PRECISION> &y)
{
    Vertex *v = new Vertex(x, y, -1);
    if (CONFIG_DEBUG_MODE)     
    {
        Vertex *vertexIterator = this->first;
        while (vertexIterator != NULL) 
        {
            if ( (x == vertexIterator->x) && (y == vertexIterator->y) )
                cerr << "ERROR - Mesh::createVertex - Vertex already exist - x: " << x << " y: " << y << endl; 
            vertexIterator = vertexIterator->next;            
        }
    }    
    /* Updates the linked list. */
    if(this->first == NULL)
    {
        this->first = v;
        this->last = v;
    }
    else
    {
        this->last->next = v;
        this->last = v;
    }
    ++numberOfVertices;

    if( this->vertexFunction != NULL )
        this->vertexFunction(v);  
    return v;
}

void Mesh::deleteVertex(Vertex *v)
{    
    for( list<Adjacency *>::iterator it = v->adjList.begin(); it != v->adjList.end();)
    {
        this->edges.remove((*it));
        this->encroachedS.remove((*it));
        this->segments.remove((*it));
        Adjacency *tmp = *it;
        ++it;
        delete tmp;
    }
    Vertex *pre = NULL;
    Vertex *vi = this->first;
    while(vi != v && vi != NULL)
    {
        pre = vi;
        vi = vi->next;
    }
    if(pre != NULL)
        pre->next = vi->next;
    if(vi == last)
        last = pre;
    if(vi == first)
        first = vi->next;
    delete v;
    this->numberOfVertices--;
}

/* _____________________TRIANGLE_____________________ */
Triangle *Mesh::createTriangle(Vertex *v0, Vertex *v1, Vertex *v2)
{
    for( list<Triangle *>::iterator it = this->triangleList.begin(); it != this->triangleList.end(); ++it)
    {
        if (CONFIG_DEBUG_MODE) 
        {
            if ( (*it)->hasVertex(v0) && (*it)->hasVertex(v1) && (*it)->hasVertex(v2) ) 
            {
                cerr << "ERROR - mesh::createTriangle - Triangle alread exists." <<  endl;             
                exit(EXIT_FAILURE);   
                return *it;
            }            
        }
    }    
    Triangle *t = new Triangle(v0, v1, v2);
    ++numberOfTriangles;
    v0->incList.push_back(t); /** Updates the incidences lists **/
    v1->incList.push_back(t);
    v2->incList.push_back(t);
    this->triangleList.push_back(t);
    this->toRefineByAngleCriterion.push_back(t);
    return t;
}

void Mesh::deleteTriangle(Triangle *t)
{
    Vertex *v0 = t->getVertex(TRIANGLE_VERTEX_ONE);
    Vertex *v1 = t->getVertex(TRIANGLE_VERTEX_TWO);
    Vertex *v2 = t->getVertex(TRIANGLE_VERTEX_THREE);
    v0->incList.remove(t); /** Updates the incidences lists **/
    v1->incList.remove(t);
    v2->incList.remove(t);
    Triangle *t0 = t->getNeighbor(TRIANGLE_VERTEX_ONE); /** Updates the neighbors pointers. **/
    Triangle *t1 = t->getNeighbor(TRIANGLE_VERTEX_TWO);
    Triangle *t2 = t->getNeighbor(TRIANGLE_VERTEX_THREE);
    if(t0 != NULL)
        t0->replaceNeighbor(t, NULL);
    if(t1 != NULL)
        t1->replaceNeighbor(t, NULL);
    if(t2 != NULL)
        t2->replaceNeighbor(t, NULL);
    Adjacency *adj = v0->getAdjacency(v1); /** Updates the adjacencies **/
    if(adj != NULL)
        adj->removeTriangle(t);
    adj = v0->getAdjacency(v2);
    if(adj != NULL)
        adj->removeTriangle(t);
    adj = v2->getAdjacency(v1);
    if(adj != NULL)
        adj->removeTriangle(t);    
    this->triangleList.remove(t);
    this->toRefineByAngleCriterion.remove(t);
    this->toRefineByAdjCriterion.remove(t);
    this->toRemove.remove(t);
    --numberOfTriangles;
    delete t;
}

void Mesh::removeTriangles(functionR *function)
{
    this->toRemove.clear();
    list<Triangle *>::iterator it;
    for( it = this->triangleList.begin(); it != this->triangleList.end(); ++it )
    {
        if( function(*it) )
            this->toRemove.push_back(*it);
    }
    while( !(this->toRemove.empty()) )
    {
        Triangle *t = this->toRemove.front();
        Vertex *v0 = t->getVertex(TRIANGLE_VERTEX_ONE);
        Vertex *v1 = t->getVertex(TRIANGLE_VERTEX_TWO);
        Vertex *v2 = t->getVertex(TRIANGLE_VERTEX_THREE);
        deleteTriangle(t);
        if( v0 != NULL && v0->incList.empty() )
            deleteVertex(v0);
        if( v1 != NULL && v1->incList.empty() )
            deleteVertex(v1);
        if( v2 != NULL && v2->incList.empty() )
            deleteVertex(v2);
    }
}

Adjacency *Mesh::createAdjacency(Vertex *v0, Vertex *v1, Triangle *t0, Triangle *t1, int seg)
{
    Adjacency *newEdge = v0->getAdjacency(v1);
    if( newEdge == NULL )  /* The adjacency not exist yet. */
    {
        newEdge = new Adjacency(v0, v1);
        newEdge->type = seg;
        newEdge->addTriangle(t0);
        newEdge->addTriangle(t1);
        if(seg == ADJ_SEG_NOT_TRI)
            this->encroachedS.push_back(newEdge);
        else if(seg == ADJ_SEG_IN_TRI)
        {
            this->segments.push_back(newEdge);
            this->edges.push_back(newEdge);
            if(t0 != NULL || t1 != NULL)
            {
                if(isEncroached(newEdge) )
                    this->encroachedS.push_back(newEdge);
            }
        }
        else if(seg == ADJ_SEG_ONLY_EDGE)
            this->edges.push_back(newEdge);
    }
    else /* The adjacency exist (ignore the parameter 'type'. */
    {
        newEdge->addTriangle(t0);
        newEdge->addTriangle(t1);
        newEdge->type = ADJ_SEG_IN_TRI;
        this->edges.push_back(newEdge);
        this->segments.push_back(newEdge);
        if(t0 != NULL or t1 != NULL)
        {
            if( !(isEncroached(newEdge)) )
                this->encroachedS.remove(newEdge);
        }
    }
    return newEdge;
}

void Mesh::removeEdge(Adjacency *edge)
{
    this->edges.remove(edge);
    this->toFlip.remove(edge);
    if( edge->type == ADJ_SEG_ONLY_EDGE )
        delete edge;
    else if(edge->type == ADJ_SEG_IN_TRI) /* if is a input segment */
    {
        edge->type = ADJ_SEG_NOT_TRI;
        this->encroachedS.push_back(edge);
        this->segments.remove(edge);
        edge->removeTriangle(edge->getTriangle(ADJ_TRIANGLE_ONE));
        edge->removeTriangle(edge->getTriangle(ADJ_TRIANGLE_TWO));
    }
    else if(edge->type == ADJ_SEG_NOT_TRI)
    {
        this->encroachedS.remove(edge);
        delete edge;
    }
}
////////////////////////////////////////////////////////////////////////////////

// "FIND" methods

////////////////////////////////////////////////////////////////////////////////
Triangle *Mesh::findTriangle( Vertex *v )
{
    for( list<Triangle *>::iterator it = this->triangleList.begin(); it != this->triangleList.end(); ++it ) /* For all triangles in the mesh. */
    {
        Triangle *tmp = *it;       
        if( tmp->isInside( v->x, v->y ) )
            return tmp;
    }
    return NULL;
}

Triangle *Mesh::findTriangle(Triangle *t, mpfr::real<MPFR_BITS_PRECISION> &px, mpfr::real<MPFR_BITS_PRECISION> &py)
{
    queue<Triangle*> tqueue;
    tqueue.push(t); /** Start the search with t. **/
    list<Triangle *> list;
    list.push_back(t); /* List to "mark" the triangles already verified. */
    while( !tqueue.empty() )
    {
        Triangle *tmp = tqueue.front();
        tqueue.pop();
        if( tmp->isInside(px, py) )        /* if the point is inside t */
            return tmp;
        Triangle *t1 = tmp->getNeighbor(TRIANGLE_VERTEX_ONE);
        Triangle *t2 = tmp->getNeighbor(TRIANGLE_VERTEX_TWO);
        Triangle *t3 = tmp->getNeighbor(TRIANGLE_VERTEX_THREE);
        /* Insert the neighbors of t in the queue. */
        if(t1 != NULL && !Utils::contains(list, t1))
        {
            list.push_back(t1);
            tqueue.push(t1);
        }
        if(t2 != NULL && !Utils::contains(list, t2))
        {
            list.push_back(t2);
            tqueue.push(t2);
        }
        if(t3 != NULL && !Utils::contains(list, t3))
        {
            list.push_back(t3);
            tqueue.push(t3);
        }
    }
    return NULL;
}
////////////////////////////////////////////////////////////////////////////////

// "MOVING MESH" methods

////////////////////////////////////////////////////////////////////////////////
bool Mesh::movingMesh( monitorFunction monitorFunction) 
{
  bool r = monitorFunction(this);         
    // Update the circumcenter coordinates, the angles of the triangle and the radius-to-shortest edge ratio.
    for( list<Triangle *>::iterator it = this->triangleList.begin(); it != this->triangleList.end(); ++it )   
        (*it)->update();        
  return r;
}    

bool Mesh::needsToBeFlipped(Adjacency *edge)
{
    Triangle *t0 = edge->getTriangle(ADJ_TRIANGLE_ONE);
    Triangle *t1 = edge->getTriangle(ADJ_TRIANGLE_TWO);
    if(t0 != NULL && t1 != NULL)
    {
        int number_t1_v0 = 0, number_t1_v1 = 0;
        mpfr::real<MPFR_BITS_PRECISION> *distSQR = NULL;
        Vertex *v0 = edge->getVertex(ADJ_VERTEX_ONE);
        Vertex *v1 = edge->getVertex(ADJ_VERTEX_TWO);       
        number_t1_v0 = t1->getNumber(v0);
        number_t1_v1 = t1->getNumber(v1);
        /* Vertex of t1 that is not a vertex of the shared edge. */
        Vertex *vt1 = t1->getVertex( 3 - (number_t1_v0 + number_t1_v1) );
        /* Distance between the circumcenter of t0 and the vertex vt1 of t1. */        
        distSQR = Utils::distanceSQR(t0->cCenter[COORDINATE_X], t0->cCenter[COORDINATE_Y], vt1);
        if( (*distSQR - t0->sqrRadius) < 0) /* The raidius is larger, isEncroached = true */    
            return true;
        delete distSQR;
        int number_t0_v0 = 0, number_t0_v1 = 0;
        number_t0_v0 = t0->getNumber(v0);
        number_t0_v1 = t0->getNumber(v1);        
        Vertex *vt0 = t0->getVertex( 3 - (number_t0_v0 + number_t0_v1) );
        /* Distance between the circumcenter of t1 and the vertex vt0 of t0. */        
        distSQR = Utils::distanceSQR(t1->cCenter[COORDINATE_X], t1->cCenter[COORDINATE_Y], vt0);        
        if( (*distSQR - t1->sqrRadius) < 0) /* The raidius is larger, isEncroached = true */    
            return true;                
        delete distSQR;
    }
    return false;
}

Mesh *Mesh::cloneMesh()
{
    Vertex *v0, *v1, *v2;
    Triangle *t0Found = NULL, *t1Found = NULL, *t0Find = NULL, *t1Find = NULL;
    Adjacency *aFind = NULL, *aFound = NULL;
    Mesh *newMesh = new Mesh();
    struct findMethods
    {  
	static Vertex *findVertex(Vertex *first, mpfr::real<MPFR_BITS_PRECISION> &x, mpfr::real<MPFR_BITS_PRECISION> &y)
	{
	    Vertex *vIterator = first;
	    while (vIterator != NULL) 
	    {
	      if ( ( vIterator->x == x) && ( vIterator->y == y) )
		return vIterator;
	      vIterator = vIterator->next;
	    }
	    return NULL;
	}
	static Triangle *findTriangle(list<Triangle *> tList, mpfr::real<MPFR_BITS_PRECISION> &x0, mpfr::real<MPFR_BITS_PRECISION> &y0, mpfr::real<MPFR_BITS_PRECISION> &x1, mpfr::real<MPFR_BITS_PRECISION> &y1, mpfr::real<MPFR_BITS_PRECISION> &x2, mpfr::real<MPFR_BITS_PRECISION> &y2)
	{
	    for( list<Triangle *>::iterator it = tList.begin(); it != tList.end(); ++it)
	    {
		Vertex *v0 = (*it)->getVertex(TRIANGLE_VERTEX_ONE);
		Vertex *v1 = (*it)->getVertex(TRIANGLE_VERTEX_TWO);
		Vertex *v2 = (*it)->getVertex(TRIANGLE_VERTEX_THREE);
		if ( ( v0->x == x0) && ( v0->y == y0) && ( v1->x == x1) && ( v1->y == y1) && ( v2->x == x2) && ( v2->y == y2) ) 
		{
		  return (*it);
		}
	    }
	    return NULL;
	}
	static Adjacency *findAdjacency(list<Adjacency *> aList, mpfr::real<MPFR_BITS_PRECISION> &x0, mpfr::real<MPFR_BITS_PRECISION> &y0, mpfr::real<MPFR_BITS_PRECISION> &x1, mpfr::real<MPFR_BITS_PRECISION> &y1)
	{
	    for( list<Adjacency *>::iterator it = aList.begin(); it != aList.end(); ++it)
	    {
		Vertex *v0 = (*it)->getVertex(ADJ_VERTEX_ONE);
		Vertex *v1 = (*it)->getVertex(ADJ_VERTEX_TWO);
		if ( ( v0->x == x0) && ( v0->y == y0) && ( v1->x == x1) && ( v1->y == y1)) 
		{
		  return (*it);
		}
	    }
	    return NULL;
	}	
    };        
    //
    // Cloning vertices
    //
    Vertex *vertexIterator = this->first;
    while (vertexIterator != NULL) 
    {
	Vertex *v       = newMesh->createVertex(vertexIterator->x, vertexIterator->y);
	v->isBorder     = vertexIterator->isBorder;
	v->label 	= vertexIterator->label;
	v->u      	= vertexIterator->u;
	v->previews_u   = vertexIterator->previews_u;
	vertexIterator = vertexIterator->next;
	v = NULL;
    }    
    //
    // Cloning triangles
    //    
    for( list<Triangle *>::iterator it = this->triangleList.begin(); it != this->triangleList.end(); ++it)
    {
      v0 = findMethods::findVertex(newMesh->first, (*it)->getVertex(TRIANGLE_VERTEX_ONE)->x, (*it)->getVertex(TRIANGLE_VERTEX_ONE)->y);
      v1 = findMethods::findVertex(newMesh->first, (*it)->getVertex(TRIANGLE_VERTEX_TWO)->x, (*it)->getVertex(TRIANGLE_VERTEX_TWO)->y);
      v2 = findMethods::findVertex(newMesh->first, (*it)->getVertex(TRIANGLE_VERTEX_THREE)->x, (*it)->getVertex(TRIANGLE_VERTEX_THREE)->y);
      if ( (v0 == NULL) || (v1 == NULL) || (v2 == NULL) )
      {
        cerr << "ERROR - Mesh::clonning triangles: vertex not found." << endl;
        exit(EXIT_FAILURE);	
      }
      Triangle *t                      = newMesh->createTriangle(v0, v1, v2);
      t->sqrRadius                     = (*it)->sqrRadius;
      t->angles[TRIANGLE_VERTEX_ONE]   = (*it)->angles[TRIANGLE_VERTEX_ONE];
      t->angles[TRIANGLE_VERTEX_TWO]   = (*it)->angles[TRIANGLE_VERTEX_TWO];
      t->angles[TRIANGLE_VERTEX_THREE] = (*it)->angles[TRIANGLE_VERTEX_THREE];
      t->radiusEdgeRatio               = (*it)->radiusEdgeRatio;
      t->cCenter[COORDINATE_X]         = (*it)->cCenter[COORDINATE_X];
      t->cCenter[COORDINATE_Y]         = (*it)->cCenter[COORDINATE_Y];  
    }       
    //
    // Clonning adjacencies
    //            
    for( list<Adjacency *>::iterator it = this->edges.begin(); it != this->edges.end(); ++it )
    {  
      v0 = v1 = NULL;
      t0Found = t1Found = t0Find = t1Find = NULL;
      v0 = findMethods::findVertex(newMesh->first, (*it)->getVertex(ADJ_VERTEX_ONE)->x, (*it)->getVertex(ADJ_VERTEX_ONE)->y);
      v1 = findMethods::findVertex(newMesh->first, (*it)->getVertex(ADJ_VERTEX_TWO)->x, (*it)->getVertex(ADJ_VERTEX_TWO)->y);      
      if ( (v0 == NULL) || (v1 == NULL) )
      {
        cerr << "ERROR - Mesh::clonning adjacencies: vertex not found." << endl;
        exit(EXIT_FAILURE);	     	
      }      
      t0Find = NULL;
      t0Find = (*it)->getTriangle(ADJ_TRIANGLE_ONE);      
      if (t0Find != NULL) 
      {
	t0Found = NULL;
	t0Found = findMethods::findTriangle(newMesh->triangleList, t0Find->getVertex(TRIANGLE_VERTEX_ONE)->x, t0Find->getVertex(TRIANGLE_VERTEX_ONE)->y, t0Find->getVertex(TRIANGLE_VERTEX_TWO)->x, t0Find->getVertex(TRIANGLE_VERTEX_TWO)->y, t0Find->getVertex(TRIANGLE_VERTEX_THREE)->x, t0Find->getVertex(TRIANGLE_VERTEX_THREE)->y);
	if (t0Found == NULL)
	{
	  cerr << "ERROR - Mesh::clonning adjacencies: triangle t0 not found." << endl;
	  exit(EXIT_FAILURE);	
	}  
	if (! (t0Found->hasVertex(v0) && t0Found->hasVertex(v1) ) )
	{
	  cerr << "ERROR - Mesh::clonning adjacencies: triangle t0 don't have vertex." << endl;
	  exit(EXIT_FAILURE);	
	}	
      }      
      t1Find = NULL;
      t1Find = (*it)->getTriangle(ADJ_TRIANGLE_TWO);      
      if (t1Find != NULL)
      {
	t1Found = NULL;
	t1Found = findMethods::findTriangle(newMesh->triangleList, t1Find->getVertex(TRIANGLE_VERTEX_ONE)->x, t1Find->getVertex(TRIANGLE_VERTEX_ONE)->y, t1Find->getVertex(TRIANGLE_VERTEX_TWO)->x, t1Find->getVertex(TRIANGLE_VERTEX_TWO)->y, t1Find->getVertex(TRIANGLE_VERTEX_THREE)->x, t1Find->getVertex(TRIANGLE_VERTEX_THREE)->y);
	if (t1Found == NULL)
	{
	  cerr << "ERROR - Mesh::clonning adjacencies: triangle t1 not found." << endl;
	  exit(EXIT_FAILURE);	
	}  
	if ( !(t1Found->hasVertex(v0) && t1Found->hasVertex(v1)) )
	{
	  cerr << "ERROR - Mesh::clonning adjacencies: triangle t1 don't have vertex." << endl;
	  exit(EXIT_FAILURE);	
	}	  
      }
      aFound = newMesh->createAdjacency(v0, v1, t0Found, t1Found, (*it)->type);   
      if ( (t0Find != NULL) && (t1Find != NULL) )
      {
	/*
	  int numbert0V0 = t0Find->getNumber((*it)->getVertex(ADJ_VERTEX_ONE));
	  int numbert0V1 = t0Find->getNumber((*it)->getVertex(ADJ_VERTEX_TWO));
	  int numbert1V0 = t1Find->getNumber((*it)->getVertex(ADJ_VERTEX_ONE));
	  int numbert1V1 = t1Find->getNumber((*it)->getVertex(ADJ_VERTEX_TWO));
	  if ( (t1Find->getNeighbor(3 - (numbert1V0 + numbert1V1)) == t0Found) && (t0Find->getNeighbor(3 - (numbert0V0 + numbert0V1)) == t1Found ) )
	  {   
	    */     	  
	    t0Found->setNeighbor(t1Found);
	    t1Found->setNeighbor(t0Found);	    
// 	  }
       }
//       v0->adjList.push_back(aFound);
//       v1->adjList.push_back(aFound);
    }
/*    vertexIterator = this->first;
    while (vertexIterator != NULL) 
    {
	Vertex *v = findMethods::findVertex(newMesh->first, vertexIterator->x, vertexIterator->y);
	if ( v == NULL )
	{
	  cerr << "ERROR - Mesh::clonning adjacencies list: vertex not found." << endl;
	  exit(EXIT_FAILURE);	     	
	}
	for(list<Adjacency *>::iterator it = vertexIterator->adjList.begin(); it != vertexIterator->adjList.end(); ++it)
	{	  
	    aFind = findMethods::findAdjacency(newMesh->edges, (*it)->getVertex(ADJ_VERTEX_ONE)->x, (*it)->getVertex(ADJ_VERTEX_ONE)->y, (*it)->getVertex(ADJ_VERTEX_TWO)->x, (*it)->getVertex(ADJ_VERTEX_TWO)->y);
	    if ( aFind == NULL )
	    {
	      cerr << "ERROR - Mesh::clonning adjacencies list: adjacency not found." << endl;
	      exit(EXIT_FAILURE);	     	
	    }
	    v0 = aFind->getVertex(ADJ_VERTEX_ONE);
	    bool inList = false;
	    for(list<Adjacency *>::iterator itA = v0->adjList.begin(); itA!= v0->adjList.end(); ++itA)
	    {
	      Vertex *vAux = (*itA)->getVertex(ADJ_VERTEX_ONE);
	      if (vAux == v0) vAux = (*itA)->getVertex(ADJ_VERTEX_TWO);	      
	      if (vAux == v1)
	      {
		  inList = true;
		  break;
	      }
	    }
	    if (!inList)	     
		v0->adjList.push_back(aFind);
	    v1 = aFind->getVertex(ADJ_VERTEX_TWO);
	    inList = false;
	    for(list<Adjacency *>::iterator itA = v1->adjList.begin(); itA!= v1->adjList.end(); ++itA)
	    {
	      Vertex *vAux = (*itA)->getVertex(ADJ_VERTEX_ONE);
	      if (vAux == v1) vAux = (*itA)->getVertex(ADJ_VERTEX_TWO);
	      if (vAux == v0)
	      {
		  inList = true;
		  break;
	      }
	    }
	    if (!inList)	         
		v1->adjList.push_back(aFind);
	}
	vertexIterator = vertexIterator->next;
    }    */    

    //
    // Clonning segments
    //
//     for( list<Adjacency *>::iterator it = this->segments.begin(); it != this->segments.end(); ++it )
//     {  
//       v0 = findMethods::findVertex(newMesh->first, (*it)->getVertex(ADJ_VERTEX_ONE)->x, (*it)->getVertex(ADJ_VERTEX_ONE)->y);
//       v1 = findMethods::findVertex(newMesh->first, (*it)->getVertex(ADJ_VERTEX_TWO)->x, (*it)->getVertex(ADJ_VERTEX_TWO)->y);      
//       if ( (v0 == NULL) || (v1 == NULL) )
//       {
//         cerr << "ERROR - Mesh::clonning adjacencies: vertex not found." << endl;
//         exit(EXIT_FAILURE);	     	
//       }      
//       t0Find = (*it)->getTriangle(ADJ_TRIANGLE_ONE);      
//       if (t0Find != NULL) 
//       {
// 	t0Found = findMethods::findTriangle(newMesh->triangleList, t0Find->getVertex(TRIANGLE_VERTEX_ONE)->x, t0Find->getVertex(TRIANGLE_VERTEX_ONE)->y, t0Find->getVertex(TRIANGLE_VERTEX_TWO)->x, t0Find->getVertex(TRIANGLE_VERTEX_TWO)->y, t0Find->getVertex(TRIANGLE_VERTEX_THREE)->x, t0Find->getVertex(TRIANGLE_VERTEX_THREE)->y);
// 	if (t0Found == NULL)
// 	{
// 	  cerr << "ERROR - Mesh::clonning adjacencies: triangle t0 not found." << endl;
// 	  exit(EXIT_FAILURE);	
// 	}     
//       }      
//       t1Find = (*it)->getTriangle(ADJ_TRIANGLE_TWO);      
//       if (t1Find != NULL)
//       {
// 	t1Found = findMethods::findTriangle(newMesh->triangleList, t1Find->getVertex(TRIANGLE_VERTEX_ONE)->x, t1Find->getVertex(TRIANGLE_VERTEX_ONE)->y, t1Find->getVertex(TRIANGLE_VERTEX_TWO)->x, t1Find->getVertex(TRIANGLE_VERTEX_TWO)->y, t1Find->getVertex(TRIANGLE_VERTEX_THREE)->x, t1Find->getVertex(TRIANGLE_VERTEX_THREE)->y);
// 	if (t1Found == NULL)
// 	{
// 	  cerr << "ERROR - Mesh::clonning adjacencies: triangle t1 not found." << endl;
// 	  exit(EXIT_FAILURE);	
// 	}     
//       }
//       aFind = findMethods::findAdjacency(newMesh->edges, (*it)->getVertex(ADJ_VERTEX_ONE)->x, (*it)->getVertex(ADJ_VERTEX_ONE)->y, (*it)->getVertex(ADJ_VERTEX_TWO)->x, (*it)->getVertex(ADJ_VERTEX_TWO)->y);
//       if (aFind == NULL)
// 	  newMesh->createAdjacency(v0, v1, t0Found, t1Found, (*it)->type);   
//       else 
// 	newMesh->segments.push_back(aFind);
//     }    
    //
    // Setting triangles neighbors
    //
//     for( list<Triangle *>::iterator it = this->triangleList.begin(); it != this->triangleList.end(); ++it)
//     {      
//       t0Find = (*it);
//       t0Found = NULL;
//       t0Found = findMethods::findTriangle(newMesh->triangleList, t0Find->getVertex(TRIANGLE_VERTEX_ONE)->x, t0Find->getVertex(TRIANGLE_VERTEX_ONE)->y, t0Find->getVertex(TRIANGLE_VERTEX_TWO)->x, t0Find->getVertex(TRIANGLE_VERTEX_TWO)->y, t0Find->getVertex(TRIANGLE_VERTEX_THREE)->x, t0Find->getVertex(TRIANGLE_VERTEX_THREE)->y);
//       if (t0Found == NULL)
//       {
// 	  cerr << "ERROR - Mesh::clonning mesh and set neighbor ONE: t0Found == NULL" << endl; 	
// 	  exit(EXIT_FAILURE);	
//       }      
//       v0 = t0Found->getVertex(TRIANGLE_VERTEX_ONE);
//       v1 = t0Found->getVertex(TRIANGLE_VERTEX_TWO);
//       v2 = t0Found->getVertex(TRIANGLE_VERTEX_THREE);
//       if ( (v0 == NULL) || (v1 == NULL) || (v2 == NULL) )
//       {
// 	cerr << "ERROR - Mesh::clonning neighbors triangles: vertex not found." << endl;
// 	exit(EXIT_FAILURE);	
//       }      
//       t0Find = (*it)->getNeighbor(TRIANGLE_VERTEX_ONE);
//       if (t0Find != NULL)
//       {
// 	  t1Found = NULL;
// 	  t1Found = findMethods::findTriangle(newMesh->triangleList, t0Find->getVertex(TRIANGLE_VERTEX_ONE)->x, t0Find->getVertex(TRIANGLE_VERTEX_ONE)->y, t0Find->getVertex(TRIANGLE_VERTEX_TWO)->x, t0Find->getVertex(TRIANGLE_VERTEX_TWO)->y, t0Find->getVertex(TRIANGLE_VERTEX_THREE)->x, t0Find->getVertex(TRIANGLE_VERTEX_THREE)->y);
// 	  if (t1Found == NULL)
// 	  {
// 	      cerr << "ERROR - Mesh::clonning mesh and set neighbor ONE: t1Found == NULL" << endl; 	
// 	      exit(EXIT_FAILURE);	
// 	  }	  
// 	  if ( ( t1Found->hasVertex(v0) && t1Found->hasVertex(v1) ) || 
// 	       ( t1Found->hasVertex(v0) && t1Found->hasVertex(v2) ) || 
// 	       ( t1Found->hasVertex(v1) && t1Found->hasVertex(v2) ) ) 
// 	  {
// 	      t0Found->setNeighbor(t1Found);
// 	      t1Found->setNeighbor(t0Found);
// 	  } 
// 	  else 
// 	  {
// 	      cerr << "ERROR - Mesh::clonning mesh and set neighbor ONE: Neighbors == NULL" << endl; 	
// 	      exit(EXIT_FAILURE);
// 	  }
//       }
//       t0Find = (*it)->getNeighbor(TRIANGLE_VERTEX_TWO);
//       if (t0Find != NULL)
//       {
// 	  t1Found = NULL;
// 	  t1Found = findMethods::findTriangle(newMesh->triangleList, t0Find->getVertex(TRIANGLE_VERTEX_ONE)->x, t0Find->getVertex(TRIANGLE_VERTEX_ONE)->y, t0Find->getVertex(TRIANGLE_VERTEX_TWO)->x, t0Find->getVertex(TRIANGLE_VERTEX_TWO)->y, t0Find->getVertex(TRIANGLE_VERTEX_THREE)->x, t0Find->getVertex(TRIANGLE_VERTEX_THREE)->y);
// 	  if (t1Found == NULL)
// 	  {
// 	      cerr << "ERROR - Mesh::clonning mesh and set neighbor TWO: t1Found == NULL" << endl; 	
// 	      exit(EXIT_FAILURE);	
// 	  }		  
// 	  if ( ( t1Found->hasVertex(v0) && t1Found->hasVertex(v1) ) || 
// 	       ( t1Found->hasVertex(v0) && t1Found->hasVertex(v2) ) || 
// 	       ( t1Found->hasVertex(v1) && t1Found->hasVertex(v2) ) ) 
// 	  {
// 	      t0Found->setNeighbor(t1Found);
// 	      t1Found->setNeighbor(t0Found);
// 	  } 
// 	  else 
// 	  {
// 	      cerr << "ERROR - Mesh::clonning mesh and set neighbor TWO: Neighbors == NULL" << endl;
// 	      exit(EXIT_FAILURE);	
// 	  }
//       }	
//       t0Find = (*it)->getNeighbor(TRIANGLE_VERTEX_THREE);		
//       if (t0Find != NULL)
//       {
// 	  t1Found = findMethods::findTriangle(newMesh->triangleList, t0Find->getVertex(TRIANGLE_VERTEX_ONE)->x, t0Find->getVertex(TRIANGLE_VERTEX_ONE)->y, t0Find->getVertex(TRIANGLE_VERTEX_TWO)->x, t0Find->getVertex(TRIANGLE_VERTEX_TWO)->y, t0Find->getVertex(TRIANGLE_VERTEX_THREE)->x, t0Find->getVertex(TRIANGLE_VERTEX_THREE)->y);
// 	  if (t1Found == NULL)
// 	  {
// 	      cerr << "ERROR - Mesh::clonning mesh and set neighbor THREE: t1Found == NULL" << endl; 	
// 	      exit(EXIT_FAILURE);	
// 	  }	  
// 	  if ( ( t1Found->hasVertex(v0) && t1Found->hasVertex(v1) ) || 
// 	       ( t1Found->hasVertex(v0) && t1Found->hasVertex(v2) ) || 
// 	       ( t1Found->hasVertex(v1) && t1Found->hasVertex(v2) ) ) 
// 	  {
// 	      t0Found->setNeighbor(t1Found);
// 	      t1Found->setNeighbor(t0Found);
// 	  } 
// 	  else 
// 	  {
// 	      cerr << "ERROR - Mesh::clonning mesh and set neighbor THREE: Neighbors == NULL" << endl; 	
// 	      exit(EXIT_FAILURE);	
// 	  }	  
//       }	
//     }    
    //
    // Clonning triangles lists on mesh
    //
//     newMesh->toRefineByAngleCriterion.clear();
//     for( list<Triangle *>::iterator it = this->toRefineByAngleCriterion.begin(); it != this->toRefineByAngleCriterion.end(); it++ )
//     {
// 	tFind = (*it);
// 	t0Found = findMethods::findTriangle(newMesh->triangleList, tFind->getVertex(TRIANGLE_VERTEX_ONE)->x, tFind->getVertex(TRIANGLE_VERTEX_ONE)->y, tFind->getVertex(TRIANGLE_VERTEX_TWO)->x, tFind->getVertex(TRIANGLE_VERTEX_TWO)->y, tFind->getVertex(TRIANGLE_VERTEX_THREE)->x, tFind->getVertex(TRIANGLE_VERTEX_THREE)->y);      
// 	if( t0Found == NULL)
// 	    cerr << "ERROR - Mesh::clonning mesh toRefineByAngleCriterion: t0Found == NULL" << endl; 		
// 	newMesh->toRefineByAngleCriterion.push_back(t0Found);
// 	t0Found = NULL;	
//     }
//     newMesh->toRefineByAdjCriterion.clear();
//     for( list<Triangle *>::iterator it = this->toRefineByAdjCriterion.begin(); it != this->toRefineByAdjCriterion.end(); it++ )
//     {
// 	tFind = (*it);
// 	t0Found = findMethods::findTriangle(newMesh->triangleList, tFind->getVertex(TRIANGLE_VERTEX_ONE)->x, tFind->getVertex(TRIANGLE_VERTEX_ONE)->y, tFind->getVertex(TRIANGLE_VERTEX_TWO)->x, tFind->getVertex(TRIANGLE_VERTEX_TWO)->y, tFind->getVertex(TRIANGLE_VERTEX_THREE)->x, tFind->getVertex(TRIANGLE_VERTEX_THREE)->y);      
// 	if( t0Found == NULL)
// 	    cerr << "ERROR - Mesh::clonning mesh toRefineByAdjCriterion: t0Found == NULL" << endl; 		
// 	newMesh->toRefineByAdjCriterion.push_back(t0Found);
// 	t0Found = NULL;	
//     }    
//     newMesh->toRemove.clear(); 
//     for( list<Triangle *>::iterator it = this->toRemove.begin(); it != this->toRemove.end(); it++ )
//     {
// 	tFind = (*it);
// 	t0Found = findMethods::findTriangle(newMesh->triangleList, tFind->getVertex(TRIANGLE_VERTEX_ONE)->x, tFind->getVertex(TRIANGLE_VERTEX_ONE)->y, tFind->getVertex(TRIANGLE_VERTEX_TWO)->x, tFind->getVertex(TRIANGLE_VERTEX_TWO)->y, tFind->getVertex(TRIANGLE_VERTEX_THREE)->x, tFind->getVertex(TRIANGLE_VERTEX_THREE)->y);      
// 	if( t0Found == NULL)
// 	    cerr << "ERROR - Mesh::clonning mesh toRemove: t0Found == NULL" << endl; 		
// 	newMesh->toRemove.push_back(t0Found);
// 	t0Found = NULL;	
//     }    
//     //
//     //Clonning adjacencies lists on mesh
//     //
//     newMesh->encroachedS.clear();
//     for( list<Adjacency *>::iterator it = this->encroachedS.begin(); it != this->encroachedS.end(); it++ )
//     {
// 	aFind = (*it);
// 	aFound = findMethods::findAdjacency(newMesh->edges, aFind->getVertex(ADJ_VERTEX_ONE)->x, aFind->getVertex(ADJ_VERTEX_ONE)->y, aFind->getVertex(ADJ_VERTEX_TWO)->x, aFind->getVertex(ADJ_VERTEX_TWO)->y);      
// 	if( aFound == NULL)
// 	{
// 	    aFound = findMethods::findAdjacency(newMesh->segments, aFind->getVertex(ADJ_VERTEX_ONE)->x, aFind->getVertex(ADJ_VERTEX_ONE)->y, aFind->getVertex(ADJ_VERTEX_TWO)->x, aFind->getVertex(ADJ_VERTEX_TWO)->y);      
// 	    if( aFound == NULL)	  
// 	      cerr << "ERROR - Mesh::clonning mesh encroachedS: aFound == NULL" << endl; 		
// 	}
// 	newMesh->encroachedS.push_back(aFound);
// 	aFound = NULL;    	
//     }
//     newMesh->toFlip.clear();  
//     for( list<Adjacency *>::iterator it = this->toFlip.begin(); it != this->toFlip.end(); it++ )
//     {
// 	aFind = (*it);
// 	aFound = findMethods::findAdjacency(newMesh->edges, aFind->getVertex(ADJ_VERTEX_ONE)->x, aFind->getVertex(ADJ_VERTEX_ONE)->y, aFind->getVertex(ADJ_VERTEX_TWO)->x, aFind->getVertex(ADJ_VERTEX_TWO)->y);      
// 	if( aFound == NULL)
// 	{
// 	    aFound = findMethods::findAdjacency(newMesh->segments, aFind->getVertex(ADJ_VERTEX_ONE)->x, aFind->getVertex(ADJ_VERTEX_ONE)->y, aFind->getVertex(ADJ_VERTEX_TWO)->x, aFind->getVertex(ADJ_VERTEX_TWO)->y);      
// 	    if( aFound == NULL)	  
// 	      cerr << "ERROR - Mesh::clonning mesh toFlip: aFound == NULL" << endl; 		
// 	}
// 	newMesh->toFlip.push_back(aFound);
// 	aFound = NULL;    	
//     }     
    newMesh->numberOfTriangles = this->numberOfTriangles;
    newMesh->numberOfVertices  = this->numberOfVertices;
    newMesh->setVertexFunction(this->vertexFunction);
    
    vertexIterator = this->first;
    int c = 0;
    while (vertexIterator != NULL) 
    {
	c++;
        cout << vertexIterator->adjList.size()<< " ";
	vertexIterator = vertexIterator->next;
    }    
    cout << "c: " << c << endl;
    cout << endl <<endl;
    vertexIterator = newMesh->first;
    c = 0;
    while (vertexIterator != NULL) 
    {
	c++;
        cout << vertexIterator->adjList.size()<< " ";
	vertexIterator = vertexIterator->next;
    }       
    cout << "c: " << c << endl;
cout << "triangleList: " << this->triangleList.size() << endl;
cout << "triangleList: " << newMesh->triangleList.size() << endl;
cout << "edges: " << this->edges.size() << endl;
cout << "edges: " << newMesh->edges.size() << endl;
cout << "segments: " << this->segments.size() << endl;
cout << "segments: " << newMesh->segments.size() << endl;

    vertexIterator = this->first;
    while (vertexIterator != NULL) 
    {
        cout << vertexIterator->incList.size()<< " ";
	vertexIterator = vertexIterator->next;
    }    
    cout << endl <<endl;
    vertexIterator = newMesh->first;
    while (vertexIterator != NULL) 
    {
        cout << vertexIterator->incList.size()<< " ";
	vertexIterator = vertexIterator->next;
    }       
    
    return newMesh;
}

bool Mesh::verifyNeighbors() {
    bool r = true;
    for( list<Triangle *>::iterator it = this->triangleList.begin(); it != this->triangleList.end(); it++ )
    {    
        list<Triangle *> listAux;
        (*it)->getNeighbors(listAux);
        for( list<Triangle *>::iterator ngb = listAux.begin(); ngb != listAux.end(); ngb++ )
        {            
            list<Triangle *> listAux1;
            (*ngb)->getNeighbors(listAux1);
            bool isNeighbor = false;
            for( list<Triangle *>::iterator nb = listAux1.begin(); nb != listAux1.end(); nb++ )
            {            
                if ( (*it) == (*nb) ) 
                {
                    isNeighbor = true;
                    Vertex *v0 = (*nb)->getVertex(TRIANGLE_VERTEX_ONE);
                    Vertex *v1 = (*nb)->getVertex(TRIANGLE_VERTEX_TWO);
                    Vertex *v2 = (*nb)->getVertex(TRIANGLE_VERTEX_THREE);
                    Adjacency *adj = NULL;
                    if ( (*ngb)->hasVertex(v0) &&  (*ngb)->hasVertex(v1) )
                    {
                        adj = (*v0).getAdjacency(v1);
                        
                    } else if ( (*ngb)->hasVertex(v0) &&  (*ngb)->hasVertex(v2) )
                    {
                        adj = (*v0).getAdjacency(v2);                        
                        
                    } else if ( (*ngb)->hasVertex(v1) &&  (*ngb)->hasVertex(v2) )
                    {
                        adj = (*v1).getAdjacency(v2);                        
                    } else 
                    {
                        cout << "ERRO Vertex" << endl;
                        break;
                    }
                    if (adj != NULL)                    
                    {
                        Triangle *t0 = (*adj).getTriangle(0);
                        Triangle *t1 = (*adj).getTriangle(1);

                        if ( ( t0 == (*nb) ) && (t1 == (*ngb)) )
                        {
//                            cout << "Adj OK" << endl;   
                        } else if ( ( t1 == (*nb) ) && (t0 == (*ngb)) )    
                        {
//                            cout << "Adj OK" << endl;   
                        } else 
                        {
                            r = false;
                            cout << "ERRO Adjacency" << endl;
                        }                        
                    }
                }
            }
            if (!isNeighbor)
            {
                r = false;
                cout << "Erro Triangle" << endl;
            }
        }
    }    
    return r;
}

bool Mesh::verifyPointsInsideTriangles() 
{
    bool r = true;
    for( list<Triangle *>::iterator it = this->triangleList.begin(); it != this->triangleList.end(); it++ )
    { 
        Vertex *vertexIterator = this->first;
        while (vertexIterator != NULL) 
        {
            if ( !(*it)->hasVertex(vertexIterator) )
            {
                if ( (*it)->isInside(vertexIterator->x, vertexIterator->y) ) 
                {
                    r = false;
                }                
            }
            vertexIterator = vertexIterator->next;
        }        
    }    
    return r;
}
bool Mesh::bruteForceCheckMesh() 
{
    bool isInsideOK = true, isNeighborhoodOK = true;
    for( list<Triangle *>::iterator it = this->triangleList.begin(); it != this->triangleList.end(); it++ )
    {         
        // Checking vertices inside triangles
        Vertex *vertexIterator = this->first;
        while (vertexIterator != NULL) 
        {
            if ( !(*it)->hasVertex(vertexIterator) )
            {
                if ( (*it)->isInside(vertexIterator->x, vertexIterator->y) ) 
                {
                    cout << "ERRO - Checking vertices inside triangles." << endl;
                    isInsideOK = false;                    
                }                
            }
            vertexIterator = vertexIterator->next;
        }                
       // Checking neighborhood 
       list<Triangle *> listAux;
        (*it)->getNeighbors(listAux);
        for( list<Triangle *>::iterator ngb = listAux.begin(); ngb != listAux.end(); ngb++ )
        {            
            list<Triangle *> listAux1;
            (*ngb)->getNeighbors(listAux1);
            for( list<Triangle *>::iterator nb = listAux1.begin(); nb != listAux1.end(); nb++ )
            {            
                if ( (*it) == (*nb) ) 
                {
                    Vertex *v0 = (*nb)->getVertex(TRIANGLE_VERTEX_ONE);
                    Vertex *v1 = (*nb)->getVertex(TRIANGLE_VERTEX_TWO);
                    Vertex *v2 = (*nb)->getVertex(TRIANGLE_VERTEX_THREE);
                    Adjacency *adj = NULL;
                    if ( (*ngb)->hasVertex(v0) &&  (*ngb)->hasVertex(v1) )
                        adj = (*v0).getAdjacency(v1);                        
                    else if ( (*ngb)->hasVertex(v0) &&  (*ngb)->hasVertex(v2) )
                        adj = (*v0).getAdjacency(v2);                                                
                    else if ( (*ngb)->hasVertex(v1) &&  (*ngb)->hasVertex(v2) )
                        adj = (*v1).getAdjacency(v2);                        
                    else 
                    {
                        isNeighborhoodOK = false;
                        cout << "ERRO - Checking neighborhood: Vertices not found." << endl;
                        break;
                    }
                    if (adj != NULL)                    
                    {
                        Triangle *t0 = (*adj).getTriangle(ADJ_TRIANGLE_ONE);
                        Triangle *t1 = (*adj).getTriangle(ADJ_TRIANGLE_TWO);
                        if ( !( ( t0 == (*nb) ) && (t1 == (*ngb) ) ) && !( ( t1 == (*nb) ) && (t0 == (*ngb)) ) )
                        {
                            isNeighborhoodOK = false;
                            cout << "ERRO - Checking neighborwood: triangles of Adjacency are differents." << endl;
                        }                        
                    }
                }
            }
        }        
    }
    return isInsideOK && isNeighborhoodOK;    
}
bool Mesh::smartCheckMesh() 
{
    for( list<Triangle *>::iterator itTriangle = this->triangleList.begin(); itTriangle != this->triangleList.end(); itTriangle++ )
    {
	list<Vertex *> verticesToCheck;
	list<Triangle *> trianglesToCheck; 
	
	if ((*itTriangle)->getNeighbor(TRIANGLE_VERTEX_ONE) != NULL)
	    trianglesToCheck.push_back( (*itTriangle)->getNeighbor(TRIANGLE_VERTEX_ONE) );
	if ((*itTriangle)->getNeighbor(TRIANGLE_VERTEX_TWO) != NULL)
	    trianglesToCheck.push_back( (*itTriangle)->getNeighbor(TRIANGLE_VERTEX_TWO) );
	if ((*itTriangle)->getNeighbor(TRIANGLE_VERTEX_THREE) != NULL)
	    trianglesToCheck.push_back( (*itTriangle)->getNeighbor(TRIANGLE_VERTEX_THREE) );
	for( list<Triangle *>::iterator itNeighbor = trianglesToCheck.begin(); itNeighbor != trianglesToCheck.end(); itNeighbor++ )
	{
	    list<Triangle *> neighbors;
	    if ( ! (*itTriangle)->hasVertex( (*itNeighbor)->getVertex(TRIANGLE_VERTEX_ONE) ) )
		verticesToCheck.push_back( (*itNeighbor)->getVertex(TRIANGLE_VERTEX_ONE) );
	    if ( ! (*itTriangle)->hasVertex( (*itNeighbor)->getVertex(TRIANGLE_VERTEX_TWO) ) )
		verticesToCheck.push_back( (*itNeighbor)->getVertex(TRIANGLE_VERTEX_TWO) );
	    if ( ! (*itTriangle)->hasVertex( (*itNeighbor)->getVertex(TRIANGLE_VERTEX_THREE) ) )
		verticesToCheck.push_back( (*itNeighbor)->getVertex(TRIANGLE_VERTEX_THREE) );
	    (*itNeighbor)->getNeighbors(neighbors);
	    for( list<Triangle *>::iterator itN = neighbors.begin(); itN != neighbors.end(); itN++ )
	    {
		if ( (*itN) != (*itTriangle) ) 
		{
		  verticesToCheck.push_back( (*itN)->getVertex(TRIANGLE_VERTEX_ONE) );
		  verticesToCheck.push_back( (*itN)->getVertex(TRIANGLE_VERTEX_TWO) );
		  verticesToCheck.push_back( (*itN)->getVertex(TRIANGLE_VERTEX_THREE) );	      
		}
	    }
	}
	for( list<Vertex *>::iterator v = verticesToCheck.begin(); v != verticesToCheck.end(); v++ )
	{	
	  if ( !(*itTriangle)->hasVertex(*v) ) 
	  {
	    if ( (*itTriangle)->isInside( (*v)->x, (*v)->y) ) 
	      return false;
	  }
	}
    }
    return true;
}
void Mesh::cuthillMcKee(bool reverse, int startingVertex) 
{     
    list<Vertex *> f;
    // Sort list of vertices by degree
    Vertex *v               = this->first,  // UTILS_CM_RANDOM
            *vertexIterator = this->first;
    int totalVertices = 0; 
    while (vertexIterator != NULL)
    {
        vertexIterator->visited = false;        
        if (!vertexIterator->isBorder)
            totalVertices++;
        vertexIterator = vertexIterator->next;          
        if (startingVertex == UTILS_CM_SMALLER_DEGREE)
        {        
            if (vertexIterator != NULL) // Get first vertex of Cuthill-mckee algorithm
            {
               if (Utils::compare(vertexIterator, v) ) 
                   v = vertexIterator;
            }        
        }
    }       
    if (startingVertex == UTILS_CM_PSEUDOPERIPHERAL)
    {
        Utils::findPseudoPeripheral(*this); 
        v = this->first;        
    }
    v->visited = true;
    this->first = NULL;
    f.push_back(v);
    int newLabel = 0;
    if (reverse) 
        newLabel = totalVertices;
    while (!f.empty())
    {
        v = f.front();
        f.pop_front();
        // Insert vertex in back     
        if (!v->isBorder)
        {
            if (reverse)
                newLabel--;            
            v->label = newLabel;            
            if(!reverse)
                newLabel++;
        }   
        if(this->first == NULL)
        {            
            this->first = v;
            this->last = v;
        }
        else
        {
            if (reverse) // Stack: insert on begin
            {
                v->next = this->first;
                this->first = v;                                           
            }
            else // Queue: insert on end
            {        
                this->last->next = v;
                this->last = v;                     
            }
        }    
        this->last->next = NULL;        
        // Making adjacency vertices list
        list<Vertex *> adjL;
        for( list<Adjacency *>::iterator adj = v->adjList.begin(); adj != v->adjList.end(); adj++ )
        {        
            Vertex *w = (*adj)->getVertex(ADJ_VERTEX_ONE);
            if (w == v)
                w = (*adj)->getVertex(ADJ_VERTEX_TWO);
            if (!w->visited) 
                adjL.push_back(w);
        }            
        adjL.sort(Utils::compare);  // Sorting adjacency vertices list by degree
        // For each adjacency vertex
        for( list<Vertex *>::iterator w = adjL.begin(); w != adjL.end(); w++ )
        {        
            if (!(*w)->visited) 
            {
                (*w)->visited = true;
                f.push_back(*w);
            }
        }            
    }   
}