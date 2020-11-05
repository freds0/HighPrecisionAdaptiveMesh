#ifndef CONJUGATEGRADIENT_H_INCLUDED
#define CONJUGATEGRADIENT_H_INCLUDED


#include "../Vertex/vertex.h"

class ConjugateGradient
{
public:

    ConjugateGradient();
    /**
     * Conjugate gradient method for symmetric positive definite matrix. The grid discretization matrix and all variables used in the conjugate gradient method are stored on the grid's Vertex, which improves performance.
     * @param firstVertex 
     * @param maxIterations
     */
    static int solve(Vertex *firstVertex, int maxIterations, long double *precision);
};
#endif // CONJUGATEGRADIENT_H_INCLUDED
