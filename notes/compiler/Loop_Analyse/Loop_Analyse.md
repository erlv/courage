# Loop Analyse and Transformation design and implementation in mordern compiler 

## Overview
  Loop optimization is very important for performance.  Therefore, mordern compilers usually have an powerful loop analyse
  and transformation support. Such support provide informations about the loop, the underlying target, and then loop
  transformation will take advantage of these information to transform the code.

## Loop Analyse implementation

### Loop Analyse of Open64

#### files

##### be/lno/soe.h soe.cxx
  SOE is short for SYSTEM_OF_EVALUATIONS. It represents an integer linear system of equations of the form
  (Ale)x <= (ble) and (Aeq)x = (beq). The entries of the matrix (upper case) are 32bit int, and the entries
  of the vector (lower case) are 64bit int. Each row represents one equation. Durin the computation process,
  32bit overflow is checked, but 64bit overflow is not.
  
  The actual system is represented by two IMAT and two arrays of int64. Extra space is typically allocated to
  the matrices (as specified in the constructor) so taht the system can grow. If the system grows too much,
  more space is automatically allocated.
  
  

##### be/lno/ara_loop.cxx
  This is a class for array def-use info of each loop nest. It contains array data flow information for a loop:
  - Upwardly-exposed array use of the loop
  - Kill (region must be defined by the loop
  - Private array regions of the loop.

##### be/lno/dep.cxx, dep.h
  This file contains basic routines to compute dependence information between two array statements. 
  These are basically interface routines that take access arrays as input, 
  use the SOEs(System of Evaluations) to compute the dependences
  and then output dependence vectors.
  
  

### Loop Analyse of LLVM



### Loop Analyse of Gcc
