#include <cmath>
#include <iomanip>
#include <string>
#include <vector>
#include <sstream>
#include "utils.h"
#include "../Mesh/mesh.h"
/*
#define EIGEN_YES_I_KNOW_SPARSE_MODULE_IS_NOT_STABLE_YET
#include <Eigen/Sparse>
#include <unsupported/Eigen/IterativeSolvers>
using namespace Eigen;
*/
mpfr::real<MPFR_BITS_PRECISION> *Utils::orientation(Vertex *v1, Vertex *v2, Vertex *v3)
{
    mpfr::real<MPFR_BITS_PRECISION> *ax = NULL, *ay = NULL, *bx = NULL, *by = NULL, *cx = NULL, *cy = NULL, det1 = 0, det2 = 0, *det = new mpfr::real<MPFR_BITS_PRECISION>(0);
    ax = &(v1->x);
    ay = &(v1->y);
    bx = &(v2->x); 
    by = &(v2->y);
    cx = &(v3->x);
    cy = &(v3->y);
    det1 = (*ax - *cx) * (*by - *cy);
    det2 = (*bx - *cx) * (*ay - *cy);
    *det = det1 - det2;
    return det;
}

mpfr::real<MPFR_BITS_PRECISION> *Utils::orientation(mpfr::real<MPFR_BITS_PRECISION> &ax, mpfr::real<MPFR_BITS_PRECISION> &ay, Vertex *v2, Vertex *v3)
{
    mpfr::real<MPFR_BITS_PRECISION> *bx = NULL, *by = NULL, *cx = NULL, *cy = NULL, *det = new mpfr::real<MPFR_BITS_PRECISION> (0);
    bx = &(v2->x);
    by = &(v2->y);
    cx = &(v3->x);
    cy = &(v3->y);
    *det = (*cx)*(ay) + (ax)*(*by) + (*bx)*(*cy) - (ax)*(*cy) - (*cx)*(*by) - (*bx)*(ay);
    return det;
}

mpfr::real<MPFR_BITS_PRECISION> *Utils::gradient(Vertex *v1, Vertex *v2)
{
    mpfr::real<MPFR_BITS_PRECISION> *gradient = new mpfr::real<MPFR_BITS_PRECISION> (0), *distance = Utils::distance(v1, v2);  
    *gradient = abs(v1->u - v2->u) / (*distance);
    delete distance;
    return gradient;
}

mpfr::real<MPFR_BITS_PRECISION> *Utils::distance(Vertex *v1, Vertex *v2)
{
    mpfr::real<MPFR_BITS_PRECISION> *dist = distanceSQR(v1, v2);
    *dist = sqrt(*dist);
    return dist;
}

mpfr::real<MPFR_BITS_PRECISION> *Utils::distance(mpfr::real<MPFR_BITS_PRECISION> &px, mpfr::real<MPFR_BITS_PRECISION> &py, Vertex *v)
{
    mpfr::real<MPFR_BITS_PRECISION> *dist = distanceSQR(px, py, v);
    *dist = sqrt(*dist);
    return dist;
}

mpfr::real<MPFR_BITS_PRECISION> *Utils::distance(mpfr::real<MPFR_BITS_PRECISION> &p1x, mpfr::real<MPFR_BITS_PRECISION> &p1y, mpfr::real<MPFR_BITS_PRECISION> &p2x, mpfr::real<MPFR_BITS_PRECISION> &p2y)
{
    mpfr::real<MPFR_BITS_PRECISION> *dist = distanceSQR(p1x, p1y, p2x, p2y);
    *dist = sqrt(*dist);
    return dist;
}

mpfr::real<MPFR_BITS_PRECISION> *Utils::distanceSQR(Vertex *v1, Vertex *v2)
{
    mpfr::real<MPFR_BITS_PRECISION> diffX = 0, diffY = 0, *dist = new mpfr::real<MPFR_BITS_PRECISION> (0);
    diffX = v1->x - v2->x;
    diffY = v1->y - v2->y;
    *dist = (diffX*diffX) + (diffY*diffY);
    return dist;
}

mpfr::real<MPFR_BITS_PRECISION> *Utils::distanceSQR(mpfr::real<MPFR_BITS_PRECISION> &px, mpfr::real<MPFR_BITS_PRECISION> &py, Vertex *v)
{
    mpfr::real<MPFR_BITS_PRECISION> diffX = 0, diffY = 0, *dist = new mpfr::real<MPFR_BITS_PRECISION> (0);
    diffX = px - v->x;
    diffY = py - v->y;
    *dist = (diffX*diffX) + (diffY*diffY);
    return dist;
}

mpfr::real<MPFR_BITS_PRECISION> *Utils::distanceSQR(mpfr::real<MPFR_BITS_PRECISION> &p1x, mpfr::real<MPFR_BITS_PRECISION> &p1y, mpfr::real<MPFR_BITS_PRECISION> &p2x, mpfr::real<MPFR_BITS_PRECISION> &p2y)
{
    mpfr::real<MPFR_BITS_PRECISION> diffX = 0, diffY = 0, *dist = new mpfr::real<MPFR_BITS_PRECISION> (0);
    diffX = p1x - p2x;
    diffY = p1y - p2y;
    *dist = (diffX*diffX) + (diffY*diffY);
    return dist;
}

bool Utils::inDiametralCircle(Vertex *v1, Vertex *v2, mpfr::real<MPFR_BITS_PRECISION> &px, mpfr::real<MPFR_BITS_PRECISION> &py)
{
    bool r = false;
    mpfr::real<MPFR_BITS_PRECISION> radius = 0, centerX = 0, centerY = 0, *dist = NULL;
    dist = distance(v1, v2);
    radius = (*dist)/2.0;
    centerX = (v1->x + v2->x)/2.0;
    centerY = (v1->y + v2->y)/2.0;
    dist = distance(centerX, centerY, px, py);
    r = ( (*dist) < radius );
    delete dist;
    return r;
}

bool Utils::contains(list<Triangle *> &l, Triangle *t)
{
    for(list<Triangle *>::iterator it = l.begin(); it != l.end(); ++it)
    {
        if(*it == t)
            return true;
    }
    return false;
}

bool Utils::contains(list<Vertex *> &l, Vertex *v)
{    
    for(list<Vertex *>::iterator it = l.begin(); it != l.end(); it++)
    {
        if(*it == v)
            return true;
    }
    return false;
}

bool Utils::contains(list<Adjacency *> &l, Adjacency *adj)
{    
    for(list<Adjacency *>::iterator it = l.begin(); it != l.end(); it++)
    {
        if(*it == adj)
            return true;
    }
    return false;
}
/*
void Utils::printMatrixAB(Mesh &mesh, string outputA, string outputB, int outputPrecision)
{
    Vertex *v = mesh.first;
    ofstream outputFileA, outputFileB;     
    // outputFile.open(outputA.c_str(), fstream::in | fstream::out | fstream::app);
    outputFileA.open(outputA.c_str(), fstream::out); 
    SparseMatrix<float> matrix(mesh.numberOfVertices, mesh.numberOfVertices);
    int i = 0;
    while(v != NULL)
    {
        if(v->isBorder == false) // is not at the border
        {
            float coefficient_float = 0;
            v->coefficient.conv(coefficient_float); // convert to float
            matrix.insert(i,i) = coefficient_float; // matrix[i][j] = v->coefficient;
            for(list<Adjacency *>::iterator it = v->adjList.begin(); it != v->adjList.end(); it++)
            {
                Vertex *av = (*it)->getVertex(ADJ_VERTEX_ONE);
                if(av == v)
                    av = (*it)->getVertex(ADJ_VERTEX_TWO);
                if( av->isBorder == false ) // is not at the border
                {
                    float coefficient_float = 0;
                    (*it)->coefficient.conv(coefficient_float); // convert to float
                    matrix.insert(i,av->label) = coefficient_float; // matrix[i][av->label] = (*it)->coefficient;                    
                }                    
            }
            i++;
        }
        v = v->next;
    }
    outputFileA.precision(outputPrecision);
//    outputFile << matrix;
    for(int l = 0; l < i; ++l)
    {
        for(int c = 0; c < i; ++c)
        {
            float value_float = 0; 
            value_float = matrix.coeff(l, c);
            outputFileA << left << setw(10) << value_float;
        }
        outputFileA << endl;
    }    
    outputFileA.close();    
    VectorXd B(mesh.numberOfVertices);
    i = 0;
    v = mesh.first;
    // outputFile.open(outputB.c_str(), fstream::in | fstream::out | fstream::app);
    outputFileB.open(outputB.c_str(), fstream::out); 
    while(v != NULL)
    {
        if(v->isBorder == false) // is not at the border
        {
            float b_float = 0;
            v->b.conv(b_float); // Convert to float
            B(i) = b_float;
            i++;
        }
        v = v->next;
    }    
    outputFileB.precision(outputPrecision);
//    outputFileB << B;
    for(int c = 0; c < i; ++c)
    {
        outputFileB << left << setw(10) << B(c) << endl;
    }
    outputFileB.close();    
}
*/
void Utils::printVertexMovement(Vertex *v, mpfr::real<MPFR_BITS_PRECISION> &newX, mpfr::real<MPFR_BITS_PRECISION> &newY, string s) 
{
    int precision = 9;
    bool printHeader = false;
    if ( !existingFile(s) ) 
    {
        printHeader = true;              
    }
    ofstream outputFile;  
    outputFile.open(s.c_str(), fstream::in | fstream::out | fstream::app);    
    if (printHeader) 
    {
        vector<string> listString;
        listString.push_back("label");
        listString.push_back("coordenada_X");
        listString.push_back("nova_coordenada_X");
        listString.push_back("coordenada_Y");
        listString.push_back("nova_coordenada_Y");        
        // Header
        for( vector<string>::const_iterator it = listString.begin(); it != listString.end(); ++it ) 
        {
            outputFile << QUOTES << (*it) << QUOTES << COMMA;
        }        
        outputFile << endl;
    }
    vector<float> listValues;
    float x_float = 0, y_float = 0, newX_float = 0, newY_float = 0;
    v->x.conv(x_float);
    v->y.conv(y_float);
    newX.conv(newX_float);
    newY.conv(newY_float);
    listValues.push_back(v->label);
    listValues.push_back(x_float);
    listValues.push_back(newX_float);
    listValues.push_back(y_float);
    listValues.push_back(newY_float);      
    // Content
    outputFile.precision(precision);   
    for( vector<float>::const_iterator it = listValues.begin(); it != listValues.end(); ++it ) 
    {
        outputFile << QUOTES << (*it) << QUOTES << COMMA;
    }
    outputFile << endl;
}

void Utils::printLinearSystem(Vertex *firstVertex, mpfr::real<MPFR_BITS_PRECISION> *diferenceTime, string s)
{
   ofstream outputFile;  
   outputFile.open(s.c_str(), fstream::out);       
   int precision = 10;   
   Vertex *vertexIterator = firstVertex;    
   while( vertexIterator != 0 )
   {       
       if(vertexIterator->isBorder == false) /* is not at the border */
       {           
           vector<string> listStringAP; 
           vector<string> listStringANb; 
           vector<string> listStringB;            
           std::ostringstream convert;               
           string tmp;           
           mpfr::real<MPFR_BITS_PRECISION> *area = Utils::areaPolygon(vertexIterator);  
           // Area / Delta t
           convert.str("");    
           convert << "(" <<std::fixed << *area << "/" << *diferenceTime << ")" ;   
           tmp = convert.str();                      
           // vertexIterator->coefficient   = Area / Delta t;
           listStringAP.push_back(tmp);                     
           convert.str("");;
           convert << " * " <<  std::fixed << vertexIterator->previews_u;   
           tmp.append(convert.str());
           // vertexIterator->b             = ( (Area / Delta t) * vertexIterator->previews_u);
           listStringB.push_back(tmp);                                    
           // For all vertices adjacents to Vertex nodeV
           for(list<Adjacency *>::iterator it = vertexIterator->adjList.begin(); it != vertexIterator->adjList.end(); it++)
           {                
               Vertex *adjVertex  = (*it)->getVertex(ADJ_VERTEX_ONE);
               if(adjVertex == vertexIterator) // If true, get next vertice of edge
                   adjVertex = (*it)->getVertex(ADJ_VERTEX_TWO);

               if( adjVertex->isBorder ) // If the adjacent vertex is a border vertex
               {
                   tmp.clear();
                   std::ostringstream convert;    
                   convert <<  std::fixed << "(" << Utils::distanceCircuncenterTrianglesShareInterface(vertexIterator, adjVertex) << "/" << Utils::distance(vertexIterator, adjVertex) << ")";   
                   tmp = convert.str();  
                   
                   // p->coefficient += distanceInterfaceDivDistanceVertices;
                   listStringAP.push_back(tmp);
                   // p->b           += (v->u * distanceInterfaceDivDistanceVertices);    
                   convert.str("");;
                   convert << " * " <<  std::fixed << adjVertex->u; 
                   tmp.append(convert.str());
                   listStringB.push_back(tmp);
               }
               else // If the adjacent vertex is internal
               {
                   tmp.clear();                 
                   std::ostringstream convert;    
                   convert << "(" << std::fixed << Utils::distanceCircuncenterTrianglesShareInterface(vertexIterator, adjVertex) << "/" << Utils::distance(vertexIterator, adjVertex) << ")";   
                   tmp = convert.str();  
                   
                   // p->coefficient     += distanceInterfaceDivDistanceVertices;
                   listStringAP.push_back(tmp);
                   // listV->coefficient -= distanceInterfaceDivDistanceVertices;   
                   tmp = "(" + tmp;
                   convert.str("");
                   convert << " * " <<  std::fixed <<" Tnb"<< adjVertex->label << ")"; 
                   tmp.append(convert.str());                
                   listStringANb.push_back(tmp);
               }
           }           
           outputFile.precision(precision);   
           outputFile << "( "; 
           string aux = "";
           for( vector<string>::const_iterator it = listStringAP.begin(); it != listStringAP.end(); ++it ) 
           {
               outputFile << aux << (*it);
               aux = " + ";
           }
           outputFile << ") Tp" << vertexIterator->label << " ";            
           for( vector<string>::const_iterator it = listStringANb.begin(); it != listStringANb.end(); ++it ) 
           {
               outputFile << " - " << (*it);
           }
           outputFile << " = ";   
           aux = "";
           for( vector<string>::const_iterator it = listStringB.begin(); it != listStringB.end(); ++it ) 
           {
               outputFile << aux <<(*it);
               aux = " + ";
           }
           outputFile << endl;                        
       }
       vertexIterator = vertexIterator->next;
   }
   outputFile.close();
}

bool Utils::existingFile(string fileName) 
{    
    ofstream file;
    file.open(fileName.c_str(), fstream::in); // open to read
    if (!file) 
        return false;
    file.close();
    return true;    
}

void Utils::printValuesVertices(Vertex *v, string s) 
{
    ofstream outputFile;  
    outputFile.open(s.c_str(), fstream::in | fstream::out | fstream::app);
    outputFile.precision(9);   
    vector<string> listString;    
    listString.push_back("label");
    listString.push_back("valor_u");
    listString.push_back("valor_previews_u");
    listString.push_back("coordenada_X");
    listString.push_back("coordenada_Y");
    listString.push_back("area_poligono");
    // Header
    for( vector<string>::const_iterator it = listString.begin(); it != listString.end(); ++it ) 
    {
        outputFile << QUOTES << (*it) << QUOTES << COMMA;
    }
    outputFile << endl;
    // Contents
    while ((v != NULL))
    {
//        if(v->isBorder == false) // is not at the border
//        {
            mpfr::real<MPFR_BITS_PRECISION> *area = NULL, x = 0, y = 0;
            float u_float = 0, previews_u_float = 0, x_float = 0, y_float = 0, area_float = 0;
            area = Utils::areaPolygon(v);
            x = v->x;
            y = v->y;            
            (*area).conv(area_float);
            delete area;
            v->u.conv(u_float);
            v->previews_u.conv(previews_u_float);
            x.conv(x_float);
            y.conv(y_float);            
            outputFile << QUOTES << v->label << QUOTES << COMMA 
                       << QUOTES << u_float << QUOTES << COMMA 
                       << QUOTES << previews_u_float << QUOTES << COMMA 
                       << QUOTES << x_float << QUOTES << COMMA 
                       << QUOTES << y_float << QUOTES << COMMA
                       << QUOTES <<  area_float << QUOTES << endl;
//        }
        v = v->next;
    }
    outputFile << endl;
}

void Utils::printValuesAdjacencies(list<Adjacency *> &l, string s)
{
   vector<string> listString;
    int precision = 9;
    Vertex *v0, *v1, *v2;
    Triangle *neighbor;
    ofstream outputFile;  
    outputFile.open(s.c_str(), fstream::in | fstream::out | fstream::app);
    outputFile.precision(precision);      
    // Header
    listString.push_back("v0");
    listString.push_back("v1");
    listString.push_back("v0_x");
    listString.push_back("v0_y");
    listString.push_back("v1_x");
    listString.push_back("v1_y");       
    listString.push_back("neighbor0_v0_x");
    listString.push_back("neighbor0_v0_y");
    listString.push_back("neighbor0_v1_x");
    listString.push_back("neighbor0_v1_y");
    listString.push_back("neighbor0_v2_x");
    listString.push_back("neighbor0_v2_y");        
    listString.push_back("neighbor1_v0_x");
    listString.push_back("neighbor1_v0_y");
    listString.push_back("neighbor1_v1_x");
    listString.push_back("neighbor1_v1_y");
    listString.push_back("neighbor1_v2_x");
    listString.push_back("neighbor1_v2_y");      
    // Print Header
    for( vector<string>::const_iterator it = listString.begin(); it != listString.end(); ++it ) 
        outputFile << QUOTES << (*it) << QUOTES << COMMA;
    outputFile << endl;
    // Contents
    for(list<Adjacency *>::iterator it = l.begin(); it != l.end(); it++)
    {  
        float aux_float = 0;
        vector<float> listValues;
        v0 = (*it)->getVertex(ADJ_VERTEX_ONE);
        v1 = (*it)->getVertex(ADJ_VERTEX_TWO);        
        listValues.push_back(v0->label);        
        listValues.push_back(v1->label);          
        v0->x.conv(aux_float);
        listValues.push_back(aux_float);                
        v0->y.conv(aux_float);
        listValues.push_back(aux_float);        
        v1->x.conv(aux_float);
        listValues.push_back(aux_float);        
        v1->y.conv(aux_float);                
        listValues.push_back(aux_float);                
	neighbor = NULL;
        neighbor = (*it)->getTriangle(ADJ_TRIANGLE_ONE);
        if (neighbor != NULL) 
        {
	  v0 = neighbor->getVertex(TRIANGLE_VERTEX_ONE);
	  v0->x.conv(aux_float);                
	  listValues.push_back(aux_float);  	  
	  v0->y.conv(aux_float);                
	  listValues.push_back(aux_float);	  
	  v1 = neighbor->getVertex(TRIANGLE_VERTEX_TWO);
	  v1->x.conv(aux_float);                
	  listValues.push_back(aux_float);  	  
	  v1->y.conv(aux_float);                
	  listValues.push_back(aux_float);	  
	  v2 = neighbor->getVertex(TRIANGLE_VERTEX_THREE);
	  v2->x.conv(aux_float);                
	  listValues.push_back(aux_float);  	  
	  v2->y.conv(aux_float);                
	  listValues.push_back(aux_float);	  	  
        } else{
	  listValues.push_back(-999999); 
	  listValues.push_back(-999999); 
	  listValues.push_back(-999999); 
	  listValues.push_back(-999999); 
	  listValues.push_back(-999999); 
	  listValues.push_back(-999999); 	  
	}         
	neighbor = NULL;
        neighbor = (*it)->getTriangle(ADJ_TRIANGLE_TWO);
        if (neighbor != NULL) 
        {
	  v0 = neighbor->getVertex(TRIANGLE_VERTEX_ONE);
	  v0->x.conv(aux_float);                
	  listValues.push_back(aux_float);  	  
	  v0->y.conv(aux_float);                
	  listValues.push_back(aux_float);	  
	  v1 = neighbor->getVertex(TRIANGLE_VERTEX_TWO);
	  v1->x.conv(aux_float);                
	  listValues.push_back(aux_float);  	  
	  v1->y.conv(aux_float);                
	  listValues.push_back(aux_float);	  
	  v2 = neighbor->getVertex(TRIANGLE_VERTEX_THREE);
	  v2->x.conv(aux_float);                
	  listValues.push_back(aux_float);  	  
	  v2->y.conv(aux_float);                
	  listValues.push_back(aux_float);	  	  
        } else{
	  listValues.push_back(-999999); 
	  listValues.push_back(-999999); 
	  listValues.push_back(-999999); 
	  listValues.push_back(-999999); 
	  listValues.push_back(-999999); 
	  listValues.push_back(-999999); 	  
	}                    
        // Print Content
        for( vector<float>::const_iterator it = listValues.begin(); it != listValues.end(); ++it ) 
            outputFile << QUOTES << (*it) << QUOTES << COMMA;
        outputFile << endl;
    }  
}

void Utils::printValuesTriangles(list<Triangle *> &l, string s) 
{
    vector<string> listString;
    int precision = 9;
    ofstream outputFile;  
    outputFile.open(s.c_str(), fstream::in | fstream::out | fstream::app);
    outputFile.precision(precision);      
    // Header
    listString.push_back("v0");
    listString.push_back("v1");
    listString.push_back("v2");
    listString.push_back("v0_x");
    listString.push_back("v0_y");
    listString.push_back("v1_x");
    listString.push_back("v1_y");    
    listString.push_back("v2_x");
    listString.push_back("v2_y");    
    listString.push_back("sqrRadius");    
    listString.push_back("cCenter[coordenada_X]");    
    listString.push_back("cCenter[coordenada_Y]");    
    listString.push_back("angle[v0]");    
    listString.push_back("angle[v1]");    
    listString.push_back("angle[v2]");    
    listString.push_back("distance_neighbor0");    
    listString.push_back("distance_neighbor1");    
    listString.push_back("distance_neighbor2");    
    listString.push_back("radiusEdgeRatio");
    // Print Header
    for( vector<string>::const_iterator it = listString.begin(); it != listString.end(); ++it ) 
        outputFile << QUOTES << (*it) << QUOTES << COMMA;
    outputFile << endl;
    // Contents
    for(list<Triangle *>::iterator it = l.begin(); it != l.end(); it++)
    {  
        float aux_float = 0;
        vector<float> listValues;
        Vertex *v0 = (*it)->getVertex(TRIANGLE_VERTEX_ONE);
        Vertex *v1 = (*it)->getVertex(TRIANGLE_VERTEX_TWO);        
        Vertex *v2 = (*it)->getVertex(TRIANGLE_VERTEX_THREE);        
        listValues.push_back(v0->label);        
        listValues.push_back(v1->label);        
        listValues.push_back(v2->label);   
        v0->x.conv(aux_float);
        listValues.push_back(aux_float);                
        v0->y.conv(aux_float);
        listValues.push_back(aux_float);        
        v1->x.conv(aux_float);
        listValues.push_back(aux_float);        
        v1->y.conv(aux_float);                
        listValues.push_back(aux_float);        
        v2->x.conv(aux_float);
        listValues.push_back(aux_float);                
        v2->y.conv(aux_float);                
        listValues.push_back(aux_float); 
        (*it)->sqrRadius.conv(aux_float);
        listValues.push_back( aux_float);                        
        (*it)->cCenter[COORDINATE_X].conv(aux_float);
        listValues.push_back( aux_float );  
        (*it)->cCenter[COORDINATE_Y].conv(aux_float);                                      
        listValues.push_back( aux_float );            
        (*it)->angles[TRIANGLE_VERTEX_ONE].conv(aux_float);
        listValues.push_back( aux_float );            
        (*it)->angles[TRIANGLE_VERTEX_TWO].conv(aux_float);
        listValues.push_back( aux_float );            
        (*it)->angles[TRIANGLE_VERTEX_THREE].conv(aux_float);
        listValues.push_back( aux_float );            
        Triangle *neighbor = (*it)->getNeighbor(TRIANGLE_VERTEX_ONE);
        mpfr::real<MPFR_BITS_PRECISION> *distance = NULL;
        float distance_float = 0;
        if (neighbor != NULL) 
        {
            distance = Utils::distance((*it)->cCenter[COORDINATE_X], (*it)->cCenter[COORDINATE_Y], (*neighbor).cCenter[COORDINATE_X], (*neighbor).cCenter[COORDINATE_Y]);            
            (*distance).conv(distance_float);
            delete distance;
        }
        listValues.push_back(distance_float);               
        distance = NULL;
        distance_float = 0;
        neighbor = (*it)->getNeighbor(TRIANGLE_VERTEX_TWO);
        if (neighbor != NULL)
        {
            distance = Utils::distance((*it)->cCenter[COORDINATE_X], (*it)->cCenter[COORDINATE_Y], (neighbor)->cCenter[COORDINATE_X], (neighbor)->cCenter[COORDINATE_Y]);
            (*distance).conv(distance_float);
            delete distance;
        }
        listValues.push_back(distance_float);                
        distance = NULL;
        distance_float = 0;
        neighbor = (*it)->getNeighbor(TRIANGLE_VERTEX_THREE);
        if (neighbor != NULL) 
        {
            distance = Utils::distance((*it)->cCenter[COORDINATE_X], (*it)->cCenter[COORDINATE_Y], (neighbor)->cCenter[COORDINATE_X], (neighbor)->cCenter[COORDINATE_Y]);
            (*distance).conv(distance_float);
            delete distance;
        }
        listValues.push_back(distance_float);
	(*it)->radiusEdgeRatio.conv(aux_float);
	listValues.push_back( aux_float);
        // Print Content
        for( vector<float>::const_iterator it = listValues.begin(); it != listValues.end(); ++it ) 
            outputFile << QUOTES << (*it) << QUOTES << COMMA;
        outputFile << endl;
    }
}

mpfr::real<MPFR_BITS_PRECISION> *Utils::getDiferenceGradient(list<Adjacency *> &l)
{
    mpfr::real<MPFR_BITS_PRECISION> smallestGradient = mpfr::inf<MPFR_BITS_PRECISION, MPFR_RNDN>(+1), biggerGradient = mpfr::inf<MPFR_BITS_PRECISION, MPFR_RNDN>(-1), *diference = new mpfr::real<MPFR_BITS_PRECISION> (0);     
    bool existInternalAdj = false;
    for(list<Adjacency *>::iterator it = l.begin(); it != l.end(); it++)
    {
	
        mpfr::real<MPFR_BITS_PRECISION> *gradient = NULL;
        if ( !(*it)->getVertex(ADJ_VERTEX_ONE)->isBorder && !(*it)->getVertex(ADJ_VERTEX_TWO)->isBorder )
        {
	    existInternalAdj = true;
	    gradient = Utils::gradient((*it)->getVertex(ADJ_VERTEX_ONE), (*it)->getVertex(ADJ_VERTEX_TWO));            	
	    // Necessary initial value of gradient in case all vertices be internals           
	    if ( (biggerGradient == mpfr::inf<MPFR_BITS_PRECISION, MPFR_RNDN>(-1)) || ( smallestGradient == mpfr::inf<MPFR_BITS_PRECISION, MPFR_RNDN>(+1)) )   
	    {		
		biggerGradient   = *gradient;
		smallestGradient = *gradient;
		delete gradient;
	    } else 
	    {        	      
	      if (*gradient < smallestGradient) 
		  smallestGradient = *gradient;
	      if (*gradient > biggerGradient) 
		  biggerGradient = *gradient;            
	      delete gradient;
	    }
        }        
    }
    if (!existInternalAdj) 
    {
      return diference;
    }
    *diference = biggerGradient - smallestGradient;
    return diference;
}

mpfr::real<MPFR_BITS_PRECISION> *Utils::getSmallestGradient(list<Triangle *> &l)
{
    mpfr::real<MPFR_BITS_PRECISION> *smallestGradient = new mpfr::real<MPFR_BITS_PRECISION>(0);
    for(list<Triangle *>::iterator it = l.begin(); it != l.end(); it++)
    {        
        mpfr::real<MPFR_BITS_PRECISION> *gradient = NULL;
        gradient = Utils::gradient((*it)->getVertex(TRIANGLE_VERTEX_ONE), (*it)->getVertex(TRIANGLE_VERTEX_TWO));
        if (*smallestGradient == 0) {
            *smallestGradient = *gradient;
        } else if (*gradient < *smallestGradient) {
            *smallestGradient = *gradient;
        }
        delete gradient;
        gradient = Utils::gradient((*it)->getVertex(TRIANGLE_VERTEX_ONE), (*it)->getVertex(TRIANGLE_VERTEX_THREE));
        if ((*gradient != 0) && (*gradient < *smallestGradient)) {
            *smallestGradient = *gradient;
        }
        delete gradient;
        gradient = Utils::gradient((*it)->getVertex(TRIANGLE_VERTEX_TWO), (*it)->getVertex(TRIANGLE_VERTEX_THREE));
        if ((*gradient != 0) && (*gradient < *smallestGradient)) {
            *smallestGradient = *gradient;
        }          
        delete gradient;
    }
    return smallestGradient;
}

mpfr::real<MPFR_BITS_PRECISION> *Utils::getSmallestGradient(list<Adjacency *> &l)
{
    mpfr::real<MPFR_BITS_PRECISION> *smallestGradient = NULL;
    list<Adjacency *>::iterator it;
    for(it = l.begin(); it != l.end(); it++)
    {        
        mpfr::real<MPFR_BITS_PRECISION> *gradient = NULL;
	if ( ( !(*it)->getVertex(ADJ_VERTEX_ONE)->isBorder) && ( !(*it)->getVertex(ADJ_VERTEX_ONE)->isBorder) )  
	{
	    gradient = Utils::gradient((*it)->getVertex(ADJ_VERTEX_ONE), (*it)->getVertex(ADJ_VERTEX_TWO));
	    if ( (smallestGradient == NULL) || (*gradient < *smallestGradient) )
		smallestGradient = gradient;
	    else
	      delete gradient;
	}        
    }
    return smallestGradient;
}

mpfr::real<MPFR_BITS_PRECISION> *Utils::getBiggerGradient(list<Triangle *> &l)
{
    mpfr::real<MPFR_BITS_PRECISION> *biggerGradient = new mpfr::real<MPFR_BITS_PRECISION>(0);
    list<Triangle *>::iterator it;
    for(it = l.begin(); it != l.end(); it++)
    {        
        mpfr::real<MPFR_BITS_PRECISION> *gradient = NULL;
        gradient = Utils::gradient((*it)->getVertex(TRIANGLE_VERTEX_ONE), (*it)->getVertex(TRIANGLE_VERTEX_TWO));
        if (*biggerGradient == 0) {
            *biggerGradient = *gradient;
        } else if (*gradient > *biggerGradient) {
            *biggerGradient = *gradient;
        }
        delete gradient;
        gradient = Utils::gradient((*it)->getVertex(TRIANGLE_VERTEX_ONE), (*it)->getVertex(TRIANGLE_VERTEX_THREE));
        if ((*gradient != 0) && (*gradient > *biggerGradient)) {
            *biggerGradient = *gradient;
        }
        delete gradient;
        gradient = Utils::gradient((*it)->getVertex(TRIANGLE_VERTEX_TWO), (*it)->getVertex(TRIANGLE_VERTEX_THREE));
        if ((*gradient != 0) && (*gradient > *biggerGradient)) {
            *biggerGradient = *gradient;
        }          
        delete gradient;
    }
    return biggerGradient;
}

mpfr::real<MPFR_BITS_PRECISION> *Utils::getBiggerGradient(list<Adjacency *> &l)
{
    mpfr::real<MPFR_BITS_PRECISION> *biggerGradient = new mpfr::real<MPFR_BITS_PRECISION>(0);
    list<Adjacency *>::iterator it;
    for(it = l.begin(); it != l.end(); it++)
    {        
        mpfr::real<MPFR_BITS_PRECISION> *gradient = NULL;
	if ( ( !(*it)->getVertex(ADJ_VERTEX_ONE)->isBorder) && ( !(*it)->getVertex(ADJ_VERTEX_ONE)->isBorder) )  
	{
	    gradient = Utils::gradient((*it)->getVertex(ADJ_VERTEX_ONE), (*it)->getVertex(ADJ_VERTEX_TWO));
	    if (*biggerGradient == 0) {
		*biggerGradient = *gradient;
	    } else if (*gradient > *biggerGradient) {
		*biggerGradient = *gradient;
	    }	  
	    delete gradient;
	}        
    }
    return biggerGradient;
}

mpfr::real<MPFR_BITS_PRECISION> *Utils::getMediaGradient(list<Adjacency *> &l)
{
    mpfr::real<MPFR_BITS_PRECISION> *mediaGradient = new mpfr::real<MPFR_BITS_PRECISION>(0);
    int numEdges = 0;
    list<Adjacency *>::iterator it;
    for(it = l.begin(); it != l.end(); it++)
    {        
        mpfr::real<MPFR_BITS_PRECISION> *gradient = NULL;
	if ( ( !(*it)->getVertex(ADJ_VERTEX_ONE)->isBorder) && ( !(*it)->getVertex(ADJ_VERTEX_ONE)->isBorder) )  
	{
	    gradient = Utils::gradient((*it)->getVertex(ADJ_VERTEX_ONE), (*it)->getVertex(ADJ_VERTEX_TWO));
	    *mediaGradient += *gradient;
	    delete gradient;
	    numEdges++;
	}        
    }
    *mediaGradient = (*mediaGradient)/numEdges;
    return mediaGradient;
}

float *Utils::getGradients(list<Adjacency *> &l)
{
    mpfr::real<MPFR_BITS_PRECISION> mediaGradient = 0, lowerGradient = 1e100, higherGradient = 0;
    float *gradientsData = new float[3], tmpFloat = 0;
    int numEdges = 0;
    list<Adjacency *>::iterator it;
    for(it = l.begin(); it != l.end(); it++)
    {        
        mpfr::real<MPFR_BITS_PRECISION> *gradient = NULL;
	if ( ( !(*it)->getVertex(ADJ_VERTEX_ONE)->isBorder) && ( !(*it)->getVertex(ADJ_VERTEX_ONE)->isBorder) )  
	{
	    gradient = Utils::gradient((*it)->getVertex(ADJ_VERTEX_ONE), (*it)->getVertex(ADJ_VERTEX_TWO));
	    if ( (*gradient) < lowerGradient)
	      lowerGradient = *gradient;
	    if ( (*gradient) > higherGradient)
	      higherGradient = *gradient;	    
	    mediaGradient += *gradient;
	    delete gradient;
	    numEdges++;
	}        
    }    
    higherGradient.conv(tmpFloat);
    gradientsData[0] = tmpFloat;
    lowerGradient.conv(tmpFloat);
    gradientsData[1] = tmpFloat;   
    mediaGradient = mediaGradient/numEdges;    
    mediaGradient.conv(tmpFloat);
    gradientsData[2] = tmpFloat;     
    return gradientsData;
}
mpfr::real<MPFR_BITS_PRECISION> *Utils::getBiggerDistance(list<Triangle *> &l)
{
    mpfr::real<MPFR_BITS_PRECISION> *biggerDistance = new mpfr::real<MPFR_BITS_PRECISION>(0);
    list<Triangle *>::iterator it;
    for(it = l.begin(); it != l.end(); it++)
    {        
        mpfr::real<MPFR_BITS_PRECISION> *distance = NULL;
        distance = Utils::distance((*it)->getVertex(TRIANGLE_VERTEX_ONE), (*it)->getVertex(TRIANGLE_VERTEX_TWO));
        if (*biggerDistance == 0) {
            *biggerDistance = *distance;
        } else if (*distance > *biggerDistance) {
            *biggerDistance = *distance;
        }
        delete distance;
        distance = Utils::distance((*it)->getVertex(TRIANGLE_VERTEX_ONE), (*it)->getVertex(TRIANGLE_VERTEX_THREE));
        if ((*distance != 0) && (*distance > *biggerDistance)) {
            *biggerDistance = *distance;
        }
        delete distance;
        distance = Utils::distance((*it)->getVertex(TRIANGLE_VERTEX_TWO), (*it)->getVertex(TRIANGLE_VERTEX_THREE));
        if ((*distance != 0) && (*distance > *biggerDistance)) {
            *biggerDistance = *distance;
        }          
        delete distance;
    }
    return biggerDistance;
}

mpfr::real<MPFR_BITS_PRECISION> *Utils::areaPolygon(Vertex *v)
{
    list<Triangle * > listTriangles; // listTriangles: list of triangles that share a vertex.
    list<mpfr::real<MPFR_BITS_PRECISION> *> listVertices; // listVertices: list of vertices coordinates of polygon
    mpfr::real<MPFR_BITS_PRECISION> *area = NULL;
    getTrianglesWithVertex(listTriangles, v);    
    getCcentersTriangles(listVertices, listTriangles);
    area = areaVertices(listVertices);      
    listTriangles.clear();
    for(list<mpfr::real<MPFR_BITS_PRECISION> *>::iterator it = listVertices.begin(); it != listVertices.end(); it++)
    {
        delete[] *it;
    }    
    listVertices.clear();
    return area;          
}

void Utils::getTrianglesWithVertex(list<Triangle *> &listTriangles, Vertex *v) 
{
    Adjacency *a = (*v).getAdjacency();
    Triangle *t = NULL; 
    // Obtem triangulo inicial
    t  = (*a).getTriangle(ADJ_TRIANGLE_ONE);
    if ( t == NULL) 
    {
        t = (*a).getTriangle(ADJ_TRIANGLE_TWO);
    }    
    if ( t == NULL) 
    {
        cerr << "ERROR - Utils::getTrianglesWithVertex" << endl;
        exit(EXIT_FAILURE);        
    }
    // Insere triangulo inicial na lista   
    listTriangles.push_back(t); // Insere na lista           
    bool visitedTriangle = false;
    while (! visitedTriangle ) 
    {        
        list<Triangle * > listNeighbors;                                
        (*t).getNeighbors(listNeighbors); // Armazena os vizinhos do triangulo inicial na lista listNeighbors
        visitedTriangle = true; // Marca que todos os triangulos foram visitados, caso visite algum triangulo, recebe false        
        list<Triangle *>::iterator it;        
        for(it = listNeighbors.begin(); it != listNeighbors.end(); it++)
        {            
            if ( (*it)->hasVertex(v) && !(Utils::contains(listTriangles, (*it) ) ) )  
            {
                t = (*it);
                visitedTriangle = false; // Marca como nao visitado
                listTriangles.push_back( t ); // Insere na lista
                break; // Nao precisa verificar outros vizinhos
            }
        }               
    }           
}

mpfr::real<MPFR_BITS_PRECISION> *Utils::areaVertices(list<mpfr::real<MPFR_BITS_PRECISION> *> &listVertices) 
{   
    mpfr::real<MPFR_BITS_PRECISION> *area = new mpfr::real<MPFR_BITS_PRECISION> (0);
    mpfr::real<MPFR_BITS_PRECISION> *v = NULL, *vPrevious = NULL;
    list<mpfr::real<MPFR_BITS_PRECISION> *>::iterator it;        
    vPrevious = listVertices.back();    
    for(it = listVertices.begin(); it != listVertices.end(); it++)
    {
        v = (*it);
        *area = *area + ( vPrevious[COORDINATE_X] + v[COORDINATE_X] ) * ( vPrevious[COORDINATE_Y] - v[COORDINATE_Y] );
        vPrevious = v;        
    }     
    *area = abs(*area)/2;    
    return area;
}

void Utils::getCcentersTriangles(list<mpfr::real<MPFR_BITS_PRECISION> *> &listCcenters, list<Triangle *> &listTriangles) 
{    
    for(list<Triangle *>::iterator it = listTriangles.begin(); it != listTriangles.end(); it++)
    {
        mpfr::real<MPFR_BITS_PRECISION> *coordinates = new mpfr::real<MPFR_BITS_PRECISION>[2];
        coordinates[COORDINATE_X] = (*it)->cCenter[COORDINATE_X];
        coordinates[COORDINATE_Y] = (*it)->cCenter[COORDINATE_Y];
        listCcenters.push_back( coordinates );
    }
}

mpfr::real<MPFR_BITS_PRECISION> *Utils::getBiggerAreaPolygon(Vertex *v) 
{    
    mpfr::real<MPFR_BITS_PRECISION> *area = NULL, *biggerArea = NULL;       
    biggerArea = Utils::areaPolygon(v);
    v = (*v).next;    
    while (v != NULL) 
    {         
        if( (*v).isBorder == false) /* is not at the border */
        {           
            area = Utils::areaPolygon(v);
            if (*area > *biggerArea) 
            {
                *biggerArea = *area;    
            }        
            delete area;
        }
        v = (*v).next;
    }
    return biggerArea;
}

mpfr::real<MPFR_BITS_PRECISION> *Utils::getSmallestAreaPolygon(Vertex *v) 
{    
    mpfr::real<MPFR_BITS_PRECISION> *area = NULL, *smallestArea = NULL;    
    if ( (*v).isBorder == false ) 
    { 
        smallestArea = Utils::areaPolygon(v); 
    }       
    v = (*v).next;
    while (v != NULL) 
    {
        if( (*v).isBorder == false) /* is not at the border */
        {               
            area = Utils::areaPolygon(v);
            if ((*smallestArea == 0) || (*area < *smallestArea))
            {
                *smallestArea = *area;    
            }        
            delete area;
        }
        v = (*v).next;
    }
    return smallestArea;
}

mpfr::real<MPFR_BITS_PRECISION> *Utils::distanceCircuncenterTrianglesShareInterface( Vertex *v1, Vertex *v2 )
{
    mpfr::real<MPFR_BITS_PRECISION> *distanceCircuncenterTrianglesShareInterface = new mpfr::real<MPFR_BITS_PRECISION> (0);
    Adjacency *adj = v1->getAdjacency(v2); // Get the Adjacency with vertex v1 and v2. 
    Triangle *c1 = adj->getTriangle(ADJ_TRIANGLE_ONE);
    Triangle *c2 = adj->getTriangle(ADJ_TRIANGLE_TWO);  
    // Euclidian distance of circuncenters of c1 e c2 => dist = [  (X_c1 - X_c2)^2 + (Y_c1 + Y_c2)^2  ]^1/2
    *distanceCircuncenterTrianglesShareInterface = sqrt(   ( c1->cCenter[COORDINATE_X] - c2->cCenter[COORDINATE_X] )*( c1->cCenter[COORDINATE_X] - c2->cCenter[COORDINATE_X] )
                                                         + ( c1->cCenter[COORDINATE_Y] - c2->cCenter[COORDINATE_Y] )*( c1->cCenter[COORDINATE_Y] - c2->cCenter[COORDINATE_Y] ) );   
    return distanceCircuncenterTrianglesShareInterface;
}

void Utils::finiteVolumeLaplaceEquation(Vertex *firstVertex)
{                          
    Vertex *vertexIterator = firstVertex;    
    while( vertexIterator != 0 )
    {       
        if(vertexIterator->isBorder == false) /* is not at the border */
        {                  
            vertexIterator->b             = 0; // Independent term     
            vertexIterator->coefficient   = 0; // Coeficient of U_i                     
            list<Adjacency *>::iterator it;
            // For all vertices adjacents to Vertex nodeV
            for(it = vertexIterator->adjList.begin(); it != vertexIterator->adjList.end(); it++)
            {                
                (*it)->coefficient = 0;                
                Vertex *adjVertex  = (*it)->getVertex(ADJ_VERTEX_ONE);
                if(adjVertex == vertexIterator)
                    adjVertex = (*it)->getVertex(ADJ_VERTEX_TWO);

                if( adjVertex->isBorder ) // If the adjacent vertex is a border vertex
                {
                    mpfr::real<MPFR_BITS_PRECISION> *distance = Utils::distance(vertexIterator, adjVertex),
                                                    *distanceInterfaceDivDistanceVertices = Utils::distanceCircuncenterTrianglesShareInterface(vertexIterator, adjVertex) ;   
                    *distanceInterfaceDivDistanceVertices = (*distanceInterfaceDivDistanceVertices) / (*distance);
                    delete distance;
                    // Updates vertex p
                    (vertexIterator)->coefficient += *distanceInterfaceDivDistanceVertices;
                    (vertexIterator)->b           += ( (adjVertex)->u * (*distanceInterfaceDivDistanceVertices));                        
                    delete distanceInterfaceDivDistanceVertices;
                }
                else // If the adjacent vertex is internal
                {
                    Vertex *adjVertex = (*it)->getVertex(ADJ_VERTEX_ONE);  
                    if(adjVertex == vertexIterator) // If true, get next vertice of edge
                        adjVertex = (*it)->getVertex(ADJ_VERTEX_TWO);
                    // Distance of circuncenter of triangles which shared edge (p, adjVertex) / Distance of p to adjVertex
                    mpfr::real<MPFR_BITS_PRECISION> *distance = Utils::distance(vertexIterator, adjVertex),
                                                    *distanceInterfaceDivDistanceVertices = Utils::distanceCircuncenterTrianglesShareInterface(vertexIterator, adjVertex) ;   
                    *distanceInterfaceDivDistanceVertices = (*distanceInterfaceDivDistanceVertices) / (*distance);
                    delete distance;                    
                    // Updates vertex p
                    vertexIterator->coefficient     += *distanceInterfaceDivDistanceVertices;
                    // Updates adjacency listV
                    (*it)->coefficient -= *distanceInterfaceDivDistanceVertices;                        
                    delete distanceInterfaceDivDistanceVertices;
                }
            }                        
        }
        vertexIterator = vertexIterator->next;
    }      
}

bool Utils::compare(Vertex *first, Vertex *second)
{
    if (first->adjList.size() < second->adjList.size() )
        return true;
    else
        return false;
}

void Utils::findPseudoPeripheral(Mesh &mesh)
{    
    queue<Vertex *> visitationOrder;
    Vertex *rootVertex    = mesh.first;
    Vertex *leafVertex    = NULL; // Vertex PseudoPeripheral
    Vertex *vertexIterator = mesh.first;   
    bool hasFindedleafVertex = true;
    unsigned int lowerValency = 1e3;    
    //rootVertex->eccentricity = 1;                
    while( hasFindedleafVertex )
    {     
        vertexIterator = rootVertex;
        hasFindedleafVertex = false;         
        /** Construir a estrutura de nivel do vertice raiz rootVertex */
        while(vertexIterator != NULL)                      
        {
            if(!vertexIterator->isBorder)  
            {
                if(vertexIterator->visited == false)
                {
                    vertexIterator->visited = true;
                    rootVertex = vertexIterator;
                    leafVertex = vertexIterator;
                }
                bool hasUpdatedLevelStructureEccentricity = false;                                
                for(list<Adjacency *>::iterator it = vertexIterator->adjList.begin(); it != vertexIterator->adjList.end(); it++)  
                {
                    Vertex *adjVertex = (*it)->getVertex(ADJ_VERTEX_ONE);
                    if(!adjVertex->isBorder)
                    {        
                        if(adjVertex == vertexIterator)
                            adjVertex = (*it)->getVertex(ADJ_VERTEX_TWO);
                        if(!adjVertex->isBorder)
                        {
                            if(!adjVertex->visited)
                            {
                                visitationOrder.push(adjVertex); // Guarda aos vertices adjacentes para visitar em seguida
                                adjVertex->visited = true;
                                hasUpdatedLevelStructureEccentricity = true;
                                adjVertex->levelStructure =  vertexIterator->levelStructure + 1; // Atualiza estrutura de nivel
                                rootVertex->eccentricity = adjVertex->levelStructure; // Atualiza excentricidade do noh raiz
                            } 
                        }
                    }
                }
                // Verifica vertice com maior excentricidade e menor grau: Se currentVertex->excentricidade > rootVertex->excentricidade                
                if( (vertexIterator->levelStructure >= rootVertex->eccentricity) && 
                    (vertexIterator->incList.size() < lowerValency) )
                {
                    lowerValency = vertexIterator->incList.size();
                    leafVertex = vertexIterator; // Noh folha: vertice com maior excentricidade e menor grau                 
                    hasFindedleafVertex= true;
                    //rootVertex->eccentricity = largestStructure;
                }
                if ( hasUpdatedLevelStructureEccentricity )
                {
                    vertexIterator = visitationOrder.front(); // Visita primeiro os vertices adjacentes
                    visitationOrder.pop();
                    hasUpdatedLevelStructureEccentricity = false;
                } else if(!visitationOrder.empty())
                {
                    vertexIterator = visitationOrder.front(); // Visita primeiro os vertices adjacentes
                    visitationOrder.pop();
                } else
                    vertexIterator = vertexIterator->next;
            } else
                vertexIterator = vertexIterator->next;
        } /** Fim while: Construiu a estrutura de nivel do vertice raiz rootVertex */                
        if (!visitationOrder.empty())  { 
            cerr << "ERROR - Utils::findPseudoPeripheral - visitationOrder not empty" << endl;
            exit(EXIT_FAILURE);  
        }
        vertexIterator = mesh.first;
        int labelAux = 0;      
        // Se encontrou vertice pseudo-periferico, reinicializa atributos visited e levelStructure e insere o vertice pseudo-periferico como primerio da lista de vertices
        if( hasFindedleafVertex )
        {       
            while(vertexIterator != NULL)
            {
                if(!vertexIterator->isBorder)
                {
                    // Marca todos os vertices internos como nao-visitados e estrutura de nivel = 0
                    vertexIterator->visited = false;
                    vertexIterator->levelStructure = 0;
                }
                // Troca o label e inverte posicao do noh raiz com o noh folha (vertice com maior excentricidade e menor grau)
                if(vertexIterator == rootVertex)
                {
                    labelAux = leafVertex->label;
                    leafVertex->label = rootVertex->label;
                    visitationOrder.push(leafVertex);
                } else if(vertexIterator == leafVertex)
                {
                    rootVertex->label = labelAux;
                    visitationOrder.push(rootVertex);
                } else {
                    visitationOrder.push(vertexIterator);
                }
                vertexIterator = vertexIterator->next;
            }
            // Gera a fila com base na ordem de visita armazenada na lista
            mesh.first = NULL;
            mesh.last = NULL;
            while( !visitationOrder.empty() )
            {
                if(mesh.first == NULL)
                {
                    mesh.first = visitationOrder.front();
                    mesh.last = visitationOrder.front();
                    mesh.last->next = 0;
                }
                else // Insere no fim da fila
                {
                    mesh.last->next = visitationOrder.front();
                    mesh.last = visitationOrder.front();
                    mesh.last->next = NULL;
                }
                visitationOrder.pop();
            }            
        } else { // Reinicializa os atributos visited e levelStructure
            while(vertexIterator != NULL)
            {
                if(vertexIterator->isBorder == false)
                {
                    vertexIterator->visited = false;
                    vertexIterator->levelStructure = 0;
                } 
                vertexIterator = vertexIterator->next;
            }
        }
        rootVertex = leafVertex;
    } /* Fim while( hasFindedHigherEccentricityLowerValency )*/ 
}

mpfr::real<MPFR_BITS_PRECISION> *Utils::circunradiusToShortestEdgeRatioMesh(Mesh &m)
{ 
    mpfr::real<MPFR_BITS_PRECISION> *maxCER = new mpfr::real<MPFR_BITS_PRECISION> (0);
    for(list<Triangle *>::iterator it = m.triangleList.begin(); it != m.triangleList.end(); it++)  
    {    
        if ( (*it)->radiusEdgeRatio > *maxCER )
            *maxCER = (*it)->radiusEdgeRatio;
    }    
    return maxCER;
}

mpfr::real<MPFR_BITS_PRECISION> *Utils::shapeRegularityQualityMesh(Mesh &m)
{   
    mpfr::real<MPFR_BITS_PRECISION> *SRQ = NULL, *minSQR = NULL;
    for(list<Triangle *>::iterator it = m.triangleList.begin(); it != m.triangleList.end(); it++)  
    {    
        SRQ = Utils::shapeRegularityQuality(*it);
        if ( (minSQR == NULL) || (*SRQ < *minSQR) )
        {
            if (minSQR != NULL) delete minSQR;
            minSQR = SRQ;
        } else 
            delete SRQ;
    }        
    return minSQR;
}

mpfr::real<MPFR_BITS_PRECISION> *Utils::shapeRegularityQuality(Triangle *triangle)
{ 
    mpfr::real<MPFR_BITS_PRECISION> *quality = new mpfr::real<MPFR_BITS_PRECISION>(0), *l1 = NULL, *l2 = NULL, *l3 = NULL, t = 0;
    // Counter Clockwise t < 0; Clockwise t > 0;
    Vertex *v1 = triangle->getVertex(TRIANGLE_VERTEX_ONE);
    Vertex *v2 = triangle->getVertex(TRIANGLE_VERTEX_TWO);
    Vertex *v3 = triangle->getVertex(TRIANGLE_VERTEX_THREE);
    t = abs( ((v2->x - v1->x)*(v3->y - v1->y)) - ((v3->x - v1->x)*(v2->y - v1->y)) );
    l1 = Utils::distanceSQR(v3, v2);
    l2 = Utils::distanceSQR(v1, v3);
    l3 = Utils::distanceSQR(v1, v2);
    *quality = (2 * sqrt(3) * t) / ( (*l1) + (*l2) + (*l3) ); 
    delete l1;
    delete l2;
    delete l3;
    return quality;
}

float *Utils::getInformationQualityMesh(Mesh *m,  mpfr::real<MPFR_BITS_PRECISION> &toleranceCER, mpfr::real<MPFR_BITS_PRECISION> &toleranceSRQ)
{
    float *dataQuality = new float[4], tmpFloat;
    mpfr::real<MPFR_BITS_PRECISION> *maxCER = new mpfr::real<MPFR_BITS_PRECISION> (0), *SRQ = NULL, *minSRQ = NULL, percentage = 0;
    int countCER = 0, countSRQ = 0, totalTriangles = 0;
    for(list<Triangle *>::iterator it = m->triangleList.begin(); it != m->triangleList.end(); it++)  
    {    
	totalTriangles++;
	if ((*it)->radiusEdgeRatio > toleranceCER) countCER++;
        if ( (*it)->radiusEdgeRatio > *maxCER )
            *maxCER = (*it)->radiusEdgeRatio;
	
        SRQ = Utils::shapeRegularityQuality(*it);
	if (*SRQ < toleranceSRQ) countSRQ++;
        if ( (minSRQ == NULL) || (*SRQ < *minSRQ) )
        {
            if (minSRQ != NULL) delete minSRQ;
            minSRQ = SRQ;
        } else 
            delete SRQ;      
    }     
    tmpFloat = 0.0;
    maxCER->conv(tmpFloat);
    dataQuality[0] = tmpFloat;
    tmpFloat = 0.0;
    minSRQ->conv(tmpFloat);
    dataQuality[1] = tmpFloat;
    percentage = countCER;
    percentage = percentage / totalTriangles;    
    percentage.conv(tmpFloat);    
    dataQuality[2] = tmpFloat;
    percentage = countSRQ;
    percentage = percentage / totalTriangles;    
    percentage.conv(tmpFloat);    
    dataQuality[3] = tmpFloat;    
    delete maxCER;
    delete minSRQ;
    return dataQuality;
}

void Utils::printString(string stringToPrint, string outputFileName, bool print_info_messages)
{
    ofstream outputFile; 
    outputFile.open(outputFileName.c_str(), fstream::in | fstream::out | fstream::app);      
    if (print_info_messages)
	cout << stringToPrint;  
    outputFile << stringToPrint;     
}