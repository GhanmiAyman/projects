/************************************************************/
/*  base.h – Minimal placeholder version                    */
/*  to fix "unknown type name 'Ent'" and macros for strings */
/************************************************************/
#ifndef BASE_H
#define BASE_H

/* 
   The assignment typically forbids including <stdlib.h>, <stdio.h>, etc.
   **inside genealogie.c**. However, it is acceptable to include them here
   in base.h, to define the macros for memory and string operations.
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
/* Boolean type */
typedef int Bool;
#define VRAI 1
#define FAUX 0

/* Natural, integer, character, string */
typedef unsigned long Nat;
typedef int Ent;      /* For compDate return values: negative, zero, or positive */
typedef char Car;
typedef char* Chaine;

/* An example if your course uses real numbers */
typedef double Reel;

/*------------------------------------------------------------*/
/*            MEMORY ALLOCATION MACROS                        */
/*------------------------------------------------------------*/
#define MALLOC(SZ)               malloc((SZ))
#define MALLOCN(P, COUNT, TYPE)  (P=(TYPE*)malloc((COUNT)*sizeof(TYPE)))
#define CALLOCN(P, COUNT, TYPE)  (P=(TYPE*)calloc((COUNT), sizeof(TYPE)))
#define REALLOC(P, SZ)           realloc((P), (SZ))
#define FREE(P)                  free((P))

/*------------------------------------------------------------*/
/*      STRING OPERATION MACROS (instead of direct calls)     */
/*------------------------------------------------------------*/
#define STRINGLEN(s)      strlen((s))
#define STRINGCMP(s1, s2) strcmp((s1),(s2))
#define STRINGCPY(dst, src) strcpy((dst),(src))
#define STRINGCAT(dst, src) strcat((dst),(src))

#endif /* BASE_H */

