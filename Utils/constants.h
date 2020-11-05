/* 
 * File:   constants.h
 * Author: fred
 *
 * Created on 14 de Agosto de 2013, 09:47
 */
#ifndef CONSTANTS_H
#define	CONSTANTS_H
//
// Vertex constants
//
#define VERTEX_INTERNAL         false
#define VERTEX_BORDER           true
//
// Mesh constants
//
// Types of refinenment
#define MESH_RUPPERT_REFINEMENT 0  /**< Ruppert's refinement */
#define MESH_UNGOR_REFINEMENT   1  /**< Ungor's refinement */
// Types of starting vertex on Cuthill-McKee
#define UTILS_CM_RANDOM           0  
#define UTILS_CM_PSEUDOPERIPHERAL 1  
#define UTILS_CM_SMALLER_DEGREE   2  
// Type of monitor function
#define MONITOR_FUNCTION_LAMBDA    0  
#define MONITOR_FUNCTION_UPSILON   1 
#define MONITOR_FUNCTION_XI        2
#define MONITOR_FUNCTION_GAMMA     3
#define MONITOR_FUNCTION_PSI       4
//
// Triangle constants
//
// Vertex constants
#define TRIANGLE_VERTEX_ONE     0
#define TRIANGLE_VERTEX_TWO     1
#define TRIANGLE_VERTEX_THREE   2
// Coordinates constants
#define COORDINATE_X            0
#define COORDINATE_Y            1
//
// Adjacencies constantes
//
// Vertex constants
#define ADJ_VERTEX_ONE     0
#define ADJ_VERTEX_TWO     1

// Triangle  constants
#define ADJ_TRIANGLE_ONE   0
#define ADJ_TRIANGLE_TWO   1

// Segment constants
#define ADJ_SEG_ONLY_EDGE  0 /**< is a edge and can be deleted or flipped */
#define ADJ_SEG_IN_TRI     1 /**< is a segment and is present in the triangulation and can not be deleted or flipped */
#define ADJ_SEG_NOT_TRI    2 /**< is a segment, but not in the triangulation and can not be deleted or flipped */
//
// MPFR constants
//
#define MPFR_BITS_PRECISION     512  /**< The calculation is done with maximum of 4096 bits of precision */
//
// Print constants
//
#define QUOTES                  "\""
#define COMMA                   ","
#define CONFIG_OUTPUT_PRECISION 10
#define CONFIG_OUTPUT_DIRECTORY "./output/"
#define CONFIG_INPUT_DIRECTORY  "./input/"
#define CONFIG_DEBUG_MODE       false  
#endif	/* CONSTANTS_H */