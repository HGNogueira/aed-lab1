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
  int set_cnt;
  int f_cnt = 0, u_cnt = 0;
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
    f_cnt++;/* find operation */
    if (id[p] == id[q]) {
      /* already in the same set; discard */
      printf("\t%d %d\n", p, q);
      continue;
    }

    /* pair has new info; must perform union */
    for (t = id[p], i = 0; i < N; i++) {
      u_cnt++;/* reading union operation */
      if (id[i] == t) {
        u_cnt++;/* writing union op */
        id[i] = id[q];
      }
    }   
    links_cnt++;
    printf(" %d %d\n", p, q);
  }

  fprintf(stdout,"\n");
  set_cnt = N - links_cnt;/* total sets given by N - links */

  t = -1;/* set label */
  int j = 0;
  for(j = 0; j < set_cnt; j++) {
    for(i = 0; i < N; i++) {
      if(id[i] != -1) { /* if node has not yet been printed */
        if(t == -1) {
          t = id[i];
          fprintf(stdout, "%d", i);
          id[i] = -1;
        } else if( t == id[i]) {
          fprintf(stdout, "-%d", i);
          id[i] = -1;
        }
      }
    }
    fprintf(stdout, "\n");
    t = -1;
  }

  fprintf(stdout, "Total sets: %d\n\n", set_cnt);
  fprintf(stdout, "QF: The number of links performed is %d for %d input pairs.\n",
         links_cnt, pairs_cnt);
  fprintf(stdout, "\tNodes (N): %d\n\tFind operations: %d\n\tUnion operations: %d\n\tTotal operations: %d\n",N ,f_cnt, u_cnt, f_cnt + u_cnt);
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
  int f_cnt = 0, u_cnt = 0;
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
    while (i != id[i]) {
      f_cnt++;/* finding p's root node */
      i = id[i];
    }
    while (j != id[j]) {
      f_cnt++;/* finding q's root node */
      j = id[j];
    }

    f_cnt++;/* find whether they are in same set */
    if (i == j) {
      /* already in the same set; discard */
      printf("\t%d %d\n", p, q);
      continue;
    }

    /* pair has new info; must perform union */
    u_cnt++;/* performing union if same root node */
    id[i] = j;
    links_cnt++;

    printf(" %d %d\n", p, q);
  }
  printf("QU: The number of links performed is %d for %d input pairs.\n",
         links_cnt, pairs_cnt);
  fprintf(stdout, "Number of nodes (N): %d\n\tFind operations: %d\n\tUnion operations: %d\n\tTotal operations: %d\n",N ,f_cnt, u_cnt, f_cnt + u_cnt);
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
  int f_cnt = 0, u_cnt = 0;
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
    f_cnt+=2;/* account for both first table read */
    for (i = p; i != id[i]; i = id[i])
        f_cnt++;/* finding the root node */
    for (j = q; j != id[j]; j = id[j])
        f_cnt++;/* finding the root node */

    f_cnt++;/* (find op), know whether same root node */
    if (i == j) {
      /* already in the same set; discard */
      printf("\t%d %d\n", p, q);
      continue;
    }

    /* pair has new info; must perform union; pick right direction */
    u_cnt++;/* reading from size tables */
    if (sz[i] < sz[j]) {
      u_cnt++;/* (union op) p pointing to q */
      id[i] = j;
      u_cnt++;/* (union op) adding new info to size table */
      sz[j] += sz[i];
    }
    else {
      u_cnt++;/* (union op) q pointing to p */
      id[j] = i;
      u_cnt++;/* (union op) adding new info to size table */
      sz[i] += sz[j];
    }
    links_cnt++;

    printf(" %d %d\n", p, q);
  }
  printf("WQU: The number of links performed is %d for %d input pairs.\n",
         links_cnt, pairs_cnt);
  fprintf(stdout, "Number of nodes (N): %d\n\tFind operations: %d\n\tUnion operations: %d\n\tTotal operations: %d\n",N ,f_cnt, u_cnt, f_cnt + u_cnt);
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
  int f_cnt = 0, u_cnt = 0;
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
    for (i = p; i != id[i]; i = id[i])
      f_cnt++;/* finding the root node */
    for (j = q; j != id[j]; j = id[j])
      f_cnt++;/* finding the root node */

    f_cnt++;/* (find op), know whether same root node */
    if (i == j) {
      /* already in the same set; discard */
      printf("\t%d %d\n", p, q);
      continue;
    }

    /* pair has new info; must perform union; pick right direction */
    if (sz[i] < sz[j]) {
      u_cnt++;/* (union op) p pointing to q */
      id[i] = j;
      u_cnt++;/* (union op) adding new info to size table */
      sz[j] += sz[i];
      t = j;
    }
    else {
      u_cnt++;/* (union op) q pointing to p */
      id[j] = i;
      u_cnt++;/* (union op) adding new info to size table */
      sz[i] += sz[j];
      t = i;
    }
    links_cnt++;

    /* retrace the path and compress to the top */
    for (i = p; i != id[i]; i = x) {
      x = id[i];
      u_cnt++;/* compression procedure */
      id[i] = t;
    }
    for (j = q; j != id[j]; j = x) {
      x = id[j];
      u_cnt++;/* compression procedure */
      id[j] = t;
    }
    printf(" %d %d\n", p, q);
  }
  printf("CWQU: The number of links performed is %d for %d input pairs.\n",
         links_cnt, pairs_cnt);
  fprintf(stdout, "Number of nodes (N): %d\n\tFind operations: %d\n\tUnion operations: %d\n\tTotal operations: %d\n",N ,f_cnt, u_cnt, f_cnt + u_cnt);
}
