#include <cmath>
#include "ConjugateGradient.h"
#include "../Mesh/mesh.h"
/*______________________________________________________________________________
                        FUNCTION conjugateGradient()
    Conjugate gradient method for symmetric positive definite matrix.
    The grid discretization matrix and all variables used in the conjugate gradient method are stored on the grid's Vertex, which improves performance.
______________________________________________________________________________*/
int ConjugateGradient::solve( Vertex *firstVertex, int maxIterations, long double  *precision)
{
    int numberOfIterations = 0;
    mpfr::real<MPFR_BITS_PRECISION>  error = 0.0, // Norm of resídue b - Au after each iteration.
                                     rTr,
                                     r1Tr1,
                                     dTAd,
                                     alpha,
                                     beta,
		                     normError = 1.0;
    /* ========================================== Initial Computations ========================================== */
    /* Computes vector Ax
     * residue r = b - Au
     * scalar rTr = r^T * r
     * Sets initial search direction d
    */
    Vertex *iVertex = firstVertex;
    rTr = 0.0;
    while ( iVertex != NULL )                      /* for all vertices */
    {
        if(iVertex->isBorder == false) // is not at the border 
        {
            iVertex->Au = 0.0;
            /* ===================== Calculates the vertex Au = coefficient * u ===================== */
            iVertex->Au += iVertex->coefficient * iVertex->u;            
            for(list<Adjacency *>::iterator it = iVertex->adjList.begin(); it != iVertex->adjList.end(); it++)  /* for all adjacent vertices */
            {
                Vertex *adjVertex = (*it)->getVertex(ADJ_VERTEX_ONE);
                if(adjVertex == iVertex)
                    adjVertex = (*it)->getVertex(ADJ_VERTEX_TWO); /**< Get next vertex */
                if ( adjVertex->isBorder == false )  // is not at the border 
                {
                    /* Au = Au + nv->coefficient * nv->u */
                    iVertex->Au += (*it)->coefficient * adjVertex->u;
                }
            }
            /* ======== r_0 = b - Au_0 ======== */
            iVertex->r = iVertex->b - iVertex->Au;
            /* ========== d_0 = r_0 ========== */
            iVertex->d = iVertex->r;
            /* ======== rTr = r^T * r ======== */
            rTr += iVertex->r * iVertex->r;
            if( fabs( iVertex->r ) > error )
                error = fabs( iVertex->r );
        }
        iVertex = iVertex->next;
    }
    /* ============================ Conjugate gradient iterations ============================ */
    if( error >= (*precision) ) // Maximum precision which error must not exceed.
    {
        while( ( normError >= (*precision) )   && ( numberOfIterations < maxIterations ) )  
        {
            dTAd = 0.0;
            iVertex = firstVertex;
            /* ================= CALCULATES THE PRODUCT Ad AND dTAd ================= */
            while ( iVertex != NULL )                   /* for all vertices */
            {
                if(iVertex->isBorder == false) // is not at the border 
                {
                    /* ============== Calculates Ad ============== */
                    iVertex->Ad = 0.0;
                    iVertex->Ad += iVertex->coefficient * iVertex->d;
                    for(list<Adjacency *>::iterator it = iVertex->adjList.begin(); it != iVertex->adjList.end(); it++)  /* for all adjacent vertices */
                    {
                        Vertex *adjVertex = (*it)->getVertex(ADJ_VERTEX_ONE);
                        if( adjVertex == iVertex )
                            adjVertex = (*it)->getVertex(ADJ_VERTEX_TWO);
                        if(adjVertex->isBorder == false)   // is not at the border 
                            iVertex->Ad += (*it)->coefficient * adjVertex->d;
                    }
                    /* ___________________________________________ */
                    /* ===== dTAd = d^T * Ad ===== */
                    dTAd += iVertex->d * iVertex->Ad;
                }
                iVertex = iVertex->next;
            }
            /* ========== ALPHA = ( r^T*r )/( d^T * Ad) ========== */	    
            alpha = rTr/dTAd;
            /** **************** CALCULATES THE NEXT APROXIMATION OF SOLUTION ***************** **/
	    normError = 0;
            iVertex = firstVertex;
            while ( iVertex != NULL )
            {
                if(iVertex->isBorder == false) // is not at the border 
		{
		    // Calculates infinit norm of error
		    mpfr::real<MPFR_BITS_PRECISION>  normErrorTmp = abs( ( (iVertex->u + alpha * iVertex->d) - iVertex->u ) / (iVertex->u + alpha * iVertex->d) );
		    if (  normErrorTmp > normError)
		      normError = normErrorTmp; 
		    // Calculates new aproximation of solution
                    iVertex->u += alpha * iVertex->d; // u = u + alpha . d
		}
                iVertex = iVertex->next;
            }
            iVertex = firstVertex;
            r1Tr1 = 0.0;
            /* ==============  CALCULATES THE NEXT RESIDUAL AND r1Tr1 = (r,r) ============== */
            while( iVertex != NULL )                       /* for all vertices */
            {
                if(iVertex->isBorder == false) // is not at the border 
                {
                    iVertex->r -= alpha * iVertex->Ad;        /* r(i+1) = r(i) - alpha * Ad */
                    r1Tr1 += iVertex->r * iVertex->r;         /* r1Tr1T = r(i+1)^T * r(i+1) */
                }
                iVertex = iVertex->next;
            }
            /* ========== BETA = ( r(i+1)^T*r(i+1) )/( r(i)^T * r(i) ) ========== */
            beta = r1Tr1/rTr;
            rTr = r1Tr1;
            iVertex = firstVertex;
            // =================== CALCULATES THE NEXT VECTOR d ===================
            while( iVertex != NULL )                       /* for all vertices */
            {
                if(iVertex->isBorder == false) // is not at the border 
                {
                    mpfr::real<MPFR_BITS_PRECISION> d1 = iVertex->r + beta * iVertex->d; 
                    iVertex->d = d1;
                }
                iVertex = iVertex->next;
            }
            numberOfIterations++;                      
        }    
    }
    return numberOfIterations;
}