/****** Implementation of the class Vertex (in vertex.h) ******/

#include "vertex.h"
#include "../Adjacency/adjacency.h"

Vertex::Vertex(mpfr::real<MPFR_BITS_PRECISION> &x, mpfr::real<MPFR_BITS_PRECISION> &y, int isBorder)
{
    setXY(x, y);
    this->next           = NULL;
    this->label          = -1;         /**< Label of vertex. -1 is a invalid label. */     
    /**< Variables used in Cuthill–McKee algorithm */
    this->visited        = false;
    this->levelStructure = 0;
    this->eccentricity   = 0;          
    this->isBorder       = isBorder;   /**< FALSE vertex is internal, TRUE vertex is located at border*/
    this->u              = 0;          /**< Value of temperature */
    this->previews_u     = 0;          /**< Value of temperature in previews timestep*/
    /**< Conjugate gradient variable */
    this->b              = 0;
    this->coefficient    = 0;
    this->r              = 0;
    this->d              = 0;
    this->Au             = 0;
    this->Ad             = 0;
}

void Vertex::setXY(mpfr::real<MPFR_BITS_PRECISION> &x, mpfr::real<MPFR_BITS_PRECISION> &y)
{
    this->x = x;
    this->y = y;
}

Adjacency *Vertex::getAdjacency(Vertex *v)
{
    list<Adjacency *>::iterator it;

    for(it = this->adjList.begin(); it != this->adjList.end(); it++)
    {
        if( (*it)->getVertex(ADJ_VERTEX_ONE) == v or (*it)->getVertex(ADJ_VERTEX_TWO) == v )
            return *it;
    }
    return NULL;
}

Adjacency *Vertex::getAdjacency()
{
    if (!this->adjList.empty()) {
        return this->adjList.front();
    }
    return NULL;
}
