/****** Implementation of the class Adjacency (file adjacency.h). ******/
#include "adjacency.h"
#include "../Triangle/triangle.h"
Adjacency::Adjacency( Vertex *v1, Vertex *v2 )
{
    this->v[ADJ_VERTEX_ONE]          = v1;
    this->v[ADJ_VERTEX_TWO]          = v2;
    /* Updates the adjacency list of v1 and v2. */
    v1->adjList.push_back(this);
    v2->adjList.push_back(this);
    this->t[ADJ_TRIANGLE_ONE]        = NULL;
    this->t[ADJ_TRIANGLE_TWO]        = NULL;
    this->type                       = ADJ_SEG_ONLY_EDGE;
    this->coefficient                = 0;
}

Adjacency::~Adjacency()
{
    this->v[ADJ_VERTEX_ONE]->adjList.remove(this);
    this->v[ADJ_VERTEX_TWO]->adjList.remove(this);
}

Vertex *Adjacency::getVertex(int number)
{
    if(number != ADJ_VERTEX_ONE && number != ADJ_VERTEX_TWO)
    {
        cerr << "ERROR - Adjacency::getVertex" << endl;
        exit(EXIT_FAILURE);
    }
    return this->v[number];
}

Triangle *Adjacency::getTriangle(int number)
{
    if(number != ADJ_TRIANGLE_ONE && number != ADJ_TRIANGLE_TWO)
    {
        cerr << "ERROR - Adjacency::getTriangle" << endl;
        exit(EXIT_FAILURE);
    }
    return this->t[number];
}

void Adjacency::addTriangle(Triangle *triangle)
{
    if ( (this->t[ADJ_TRIANGLE_ONE] != triangle) && (this->t[ADJ_TRIANGLE_TWO] != triangle) ) 
    {
        if( this->t[ADJ_TRIANGLE_ONE] == NULL )
            this->t[ADJ_TRIANGLE_ONE] = triangle;
        else if( this->t[ADJ_TRIANGLE_TWO] == NULL )
            this->t[ADJ_TRIANGLE_TWO] = triangle;
        else
        {
            cerr << "ERROR - Adjacency::addTriangle" << endl;
            exit(EXIT_FAILURE);
        }        
    }    
}

void Adjacency::removeTriangle(Triangle *triangle)
{
    if( t[ADJ_TRIANGLE_ONE] == triangle )
        t[ADJ_TRIANGLE_ONE] = NULL;
    else if(t[ADJ_TRIANGLE_TWO] == triangle)
        t[ADJ_TRIANGLE_TWO] = NULL;
    else {
        cerr << "ERROR - Adjacency::removeTriangle" << endl;
        exit(EXIT_FAILURE);        
    }
}