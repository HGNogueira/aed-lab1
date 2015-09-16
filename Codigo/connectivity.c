/******************************************************************************
 * (c) 2010 AED
 * File Name: connectivity.c
 * Author:    AED Team
 * Last modified: PFF 2010-03-12
 * Revision:  v20100312
 *
 * NAME
 *      connectivity - algorithms for solving the connectivity problem
 *
 * DESCRIPTION
 *      QF QU WQU CWQU, counts number of entry pairs and the number of links
 *      for each method.
 *
 * COMMENTS
 *
 *****************************************************************************/

#include<stdio.h>
#include<stdlib.h>

/******************************************************************************
 * quick_find()
 *
 * Arguments: id - array with connectivity information
 *            N - size of array
 *            fp - file pointer to read data from
 * Returns: (void)
 * Side-Effects: pairs of elements are read and the connectivity array is
 *               modified
 *
 * Description: Quick Find algorithm
 *
 *****************************************************************************/
void quick_find(int *id, int N, FILE * fp)
{
  int op_cnt = 0;
  int i, p, q, t;
  int pairs_cnt = 0;            /* connection pairs counter */
  int links_cnt = 0;            /* number of links counter */

  /* initialize; all disconnected */
  for (i = 0; i < N; i++)
    id[i] = i;

  /* read while there is data */
  while (fscanf(fp, "%d %d", &p, &q) == 2) {
    pairs_cnt++;
    /* do search first */
    if (id[p] == id[q]) {
      /* already in the same set; discard */
      op_cnt++;
      printf("\t%d %d\n", p, q);
      continue;
    }

    /* pair has new info; must perform union */
    for (t = id[p], i = 0; i < N; i++)
      op_cnt++;
      if (id[i] == t)
        id[i] = id[q];
    links_cnt++;
    printf(" %d %d\n", p, q);
  }
  printf("QF: The number of links performed is %d for %d input pairs.\n",
         links_cnt, pairs_cnt);
  printf("Total number of table rw operations: %d\n", op_cnt);
}


/******************************************************************************
 * quick_union()
 *
 * Arguments: id - array with connectivity information
 *            N - size of array
 *            fp - file pointer to read data from
 * Returns: (void)
 * Side-Effects: pairs of elements are read and the connectivity array is
 *               modified
 *
 * Description: Quick Union algorithm
 *
 *****************************************************************************/
void quick_union(int *id, int N, FILE * fp)
{

  int i, j, p, q;
  int pairs_cnt = 0;            /* connection pairs counter */
  int links_cnt = 0;            /* number of links counter */

  /* initialize; all disconnected */
  for (i = 0; i < N; i++)
    id[i] = i;

  /* read while there is data */
  while (fscanf(fp, "%d %d", &p, &q) == 2) {
    pairs_cnt++;
    i = p;
    j = q;

    /* do search first */
    while (i != id[i])
      i = id[i];
    while (j != id[j])
      j = id[j];

    if (i == j) {
      /* already in the same set; discard */
      printf("\t%d %d\n", p, q);
      continue;
    }

    /* pair has new info; must perform union */
    id[i] = j;
    links_cnt++;

    printf(" %d %d\n", p, q);
  }
  printf("QU: The number of links performed is %d for %d input pairs.\n",
         links_cnt, pairs_cnt);
}


/******************************************************************************
 * weighted_quick_union()
 *
 * Arguments: id - array with connectivity information
 *            N - size of array
 *            fp - file pointer to read data from
 * Returns: (void)
 * Side-Effects: pairs of elements are read and the connectivity array is
 *               modified
 *
 * Description: Weighted Quick Union algorithm
 *
 *****************************************************************************/
void weighted_quick_union(int *id, int N, FILE * fp)
{
  int op_cnt = 0;
  int i, j, p, q;
  int *sz = (int *) malloc(N * sizeof(int));
  int pairs_cnt = 0;            /* connection pairs counter */
  int links_cnt = 0;            /* number of links counter */

  /* initialize; all disconnected */
  for (i = 0; i < N; i++) {
    id[i] = i;
    sz[i] = 1;
  }

  /* read while there is data */
  while (fscanf(fp, "%d %d", &p, &q) == 2) {
    pairs_cnt++;

    /* do search first */
    for (i = p; i != id[i]; i = id[i]);
    for (j = q; j != id[j]; j = id[j]);

    if (i == j) {
      /* already in the same set; discard */
      printf("\t%d %d\n", p, q);
      continue;
    }

    /* pair has new info; must perform union; pick right direction */
    if (sz[i] < sz[j]) {
      id[i] = j;
      sz[j] += sz[i];
    }
    else {
      id[j] = i;
      sz[i] += sz[j];
    }
    links_cnt++;

    printf(" %d %d\n", p, q);
  }
  printf("WQU: The number of links performed is %d for %d input pairs.\n",
         links_cnt, pairs_cnt);
}


/******************************************************************************
 * compressed_weighted_quick_union()
 *
 * Arguments: id - array with connectivity information
 *            N - size of array
 *            fp - file pointer to read data from
 * Returns: (void)
 * Side-Effects: pairs of elements are read and the connectivity array is
 *               modified
 *
 * Description: Compressed Weighted Quick Union algorithm
 *
 *****************************************************************************/
void compressed_weighted_quick_union(int *id, int N, FILE * fp)
{

  int i, j, p, q, t, x;
  int *sz = (int *) malloc(N * sizeof(int));
  int pairs_cnt = 0;            /* connection pairs counter */
  int links_cnt = 0;            /* number of links counter */

  /* initialize; all disconnected */
  for (i = 0; i < N; i++) {
    id[i] = i;
    sz[i] = 1;
  }

  /* read while there is data */
  while (fscanf(fp, "%d %d", &p, &q) == 2) {
    pairs_cnt++;

    /* do search first */
    for (i = p; i != id[i]; i = id[i]);
    for (j = q; j != id[j]; j = id[j]);

    if (i == j) {
      /* already in the same set; discard */
      printf("\t%d %d\n", p, q);
      continue;
    }

    /* pair has new info; must perform union; pick right direction */
    if (sz[i] < sz[j]) {
      id[i] = j;
      sz[j] += sz[i];
      t = j;
    }
    else {
      id[j] = i;
      sz[i] += sz[j];
      t = i;
    }
    links_cnt++;

    /* retrace the path and compress to the top */
    for (i = p; i != id[i]; i = x) {
      x = id[i];
      id[i] = t;
    }
    for (j = q; j != id[j]; j = x) {
      x = id[j];
      id[j] = t;
    }
    printf(" %d %d\n", p, q);
  }
  printf("CWQU: The number of links performed is %d for %d input pairs.\n",
         links_cnt, pairs_cnt);
}
