/******************************************************************************
 * (c) 2010 AED
 * File Name: class2.c
 * Author: AED Team
 * Last Modified: PFF 2010-03-12
 * Revision: v20100312
 *
 * NAME
 *     class2 - test algorithms for solving the connectivity problem
 *
 * DESCRIPTION
 *      Main program to call the different connectivity algorithms
 *
 * COMMENTS
 *
 *****************************************************************************/

#include <stdlib.h>
#include <stdio.h>
#include "connectivity.h"

void usage(char *);


int main(int argc, char *argv[])
{

  int N, *id, choice = 0;
  FILE *fp;

  /* must have at least one argument besides the exec name */
  if (argc < 2) {
    usage(argv[0]);
    exit(0);
  }

  /* open file and read array size */
  fp = (FILE *) fopen(argv[1], "r");
  if (fp == (FILE *) NULL) {
    fprintf(stderr, "File %s cannot be read.  Please correct.\n", argv[1]);
    exit(0);
  }
  fscanf(fp, "%d", &N);
  if (N > 0) {
    id = (int *) calloc(N, sizeof(int));
  }

  /* query which algorithm to use */
  while (choice >= 0) {

    fprintf(stdout, "\tConnectivity Problem\n\nChoose an algorithm:\n");
    fprintf(stdout, "\t1 - Quick Find\n");
    fprintf(stdout, "\t2 - Quick Union\n");
    fprintf(stdout, "\t3 - Weighted Quick Union\n");
    fprintf(stdout, "\t4 - Weighted Quick Union with Compression\n");
    fprintf(stdout, "\t5 - Quicker Union (TESTING)\n");
    fprintf(stdout, "\n");
    fprintf(stdout, "\t0 - Exit\n");

    do {
      fprintf(stdout, "\nYour choice: (1,2,3,4,0) ");
      scanf("%d", &choice);
    } while (choice < 0 || choice > 5);

    if (choice == 0)
      break;

    switch (choice) {
    case 1:
      quick_find(id, N, fp);
      break;
    case 2:
      quick_union(id, N, fp);
      break;
    case 3:
      weighted_quick_union(id, N, fp);
      break;
    case 4:
      compressed_weighted_quick_union(id, N, fp);
      break;
    case 5:
      quicker_union(N, fp);
      break;
    default:
      fprintf(stderr, "Yikes\n");
      exit(1);
    }
    /* rewind the file; reread size (not needed but must consume input) */
    rewind(fp);
    fscanf(fp, "%d", &N);
  }

  exit(0);
}


void usage(char *prog)
{

  fprintf(stdout, "usage: %s filename\n", prog);
  return;
}
