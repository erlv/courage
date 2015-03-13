# Loop Analyse and Transformation design and implementation in mordern compiler 

## Overview
  Loop optimization is very important for performance.  Therefore, mordern compilers usually have an powerful loop analyse
  and transformation support. Such support provide informations about the loop, the underlying target, and then loop
  transformation will take advantage of these information to transform the code.

## Loop Analyse implementation

### Loop Analyse of Open64

#### files

##### be/lno/ara_loop.cxx
  This is a class for array def-use info of each loop nest. It contains array data flow information for a loop:
  - Upwardly-exposed array use of the loop
  - Kill (region must be defined by the loop
  - Private array regions of the loop.


### Loop Analyse of LLVM



### Loop Analyse of Gcc
