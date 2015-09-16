/******************************************************************************
 * (c) 2010 AED
 * File Name: connectivity.h
 * Author:    AED Team
 * Last modified: PFF 2010-03-12
 * Revision:  v20100312
 *
 * NAME
 *      class2 - test algorithms for solving the connectivity problem
 *
 * DESCRIPTION
 *      Functions declarations for solving the connectivity problem
 *
 * COMMENTS
 *
 *****************************************************************************/
#ifndef __connectivity__
#define __connectivity__

void quick_find(int *id, int N, FILE * fp);
void quick_union(int *id, int N, FILE * fp);
void weighted_quick_union(int *id, int N, FILE * fp);
void compressed_weighted_quick_union(int *id, int N, FILE * fp);

#endif                          /* __connectivity__ */
/*****************************************************************************/
