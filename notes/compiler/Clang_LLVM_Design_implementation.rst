======================================
 CLang/LLVM Design and Implementation
======================================

===========
 Objective
===========
  This document want to record the learning of design and implementation of Clang and LLVM. 
  The Toolchain is a popular toolchain widely used for not only C/C++ programming, 
  but also some other kind of works like Tenzing, Impala,etc.

  All the following writing is based on Clang/LLVM 3.7.

=========
  Links
=========
   - “Clang” CFE Internals Manual[fn:1]
   -
   

* Clang/LLVM Basic
  



* LLVM/Clang Common Library
  - libSupport: provide many underlying libraries and data-structure
    - Command Line option processing
    - Various Container
    - a  System Abstraction layer for file system access

* Clang
Clang is the FE of the toolchain, support:
  - Std C
  - Std C++
  - Obj C
  - 

** Clang Internal Design
   This documents contains:
   - more important APIs
   - Internal Design Decision.[fn:1]

*** The Basic Library



**** The Diagnostic Subsystem
     - ''Clang/Basic/Diagnostic*Kinds.td'' used to:
       - specifiy a unique Diagnostic type.
       - Specify the format strings

**** SourceLocation and SourceManager class
     - SourceLocation: This object will be embedded into many AST node.
     - 

**** SourceRange and CharSourceRange class


*** The Driver Library
    The Driver Library driver design & Internal can be find here[fn:2].


*** Precompiled Headers
    Clang support two types of precompiled headers:
    - PCH(PreCompiled headers): Use LLVM BitStream Format to represent the header file.
    - PTH(PreTokenized headers): representation of the tokens.

    Here[fn:3] is the detail.

*** The Lexer and Preprocessor Library
 

**** Token Class
     Used to represent a single lexed token, and it mos often live on the stack as the parser is running.

**** Lexer Class
     It lexes tokens out of a source buffer and decides what they mean.

**** TokenLexer Class
     This is a token provider that returns tokens from a list of tokens that came from somewhere else.

**** MultipleIncludeOpt Class
     It implements a really simple little state machine used to detect the standard "#ifndef XX/ @define XX" idiom.

*** The Parser Library

*** The AST Library

**** type class and its subclasses

**** QualType class

**** Declaration Name

**** Declaration Contexts

**** CFG Class

***** Basic Blocks--CFGBlock class

***** Entry and Exit Blocks

***** Conditional Control-Flow



* LLVM 

** LLVM IR
   The language is:
   - based on SSA
   - Type safe
   - Low Level Operations

   The language can be used in the following forms:
   - In-memory Compiler IR
   - On-disk bitcode representation
   - Human readable assembly language representation

** LLVM Assembly Language
*** High Level Structure

*** Type System

** LLVM JIT


** LLVM Optimizer


* Best Practice
** How to add intrinsics in LLVM
   AMD guys proposed an intrisnic added [patch|https://groups.google.com/forum/#!topic/llvm-dev/S3P1Sh3H--Q] for LLVM which shows some idea about how to add intrinsic.



   
* Footnotes

[fn:1] http://clang.llvm.org/docs/InternalsManual.html

[fn:2] http://clang.llvm.org/docs/DriverInternals.html

[fn:3] http://clang.llvm.org/docs/PCHInternals.html
