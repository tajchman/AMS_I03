#include <stdio.h>
#include <getopt.h>
#include <stdlib.h>
#include <time.h>

#include "count.h"
#include "random.h"

#if defined(_OPENMP)
#include <omp.h>
#endif

void setOptions(int argc, char **argv, int * nThreads, long long * nSamples)
{
  char c;
  *nSamples = 1000L * 200000L;
  *nThreads = 1;
  while ((c = getopt(argc , argv, "n:t:")) != -1)
    switch (c) {
    case 'n':
      *nSamples = 1000L * strtoll(optarg, NULL, 10);
      break;

    case 't':
#if defined(_OPENMP)
      *nThreads = strtol(optarg, NULL, 10);
#endif
      break;

    default:
      abort ();
    }
}

int main(int argc, char **argv)
{
  long long nSamples;
  int nThreads;
  double *s;
  long iSamples;
  
  fprintf(stderr, "\nTP2 %s\n", argv[0]);
  setOptions(argc, argv, &nThreads, &nSamples);

#if defined(_OPENMP)
  omp_set_num_threads(nThreads);
#endif
  printf(" %d threads\n\n", nThreads);
  printf(" %lld samples\n", nSamples);
  
  s = countAllocate();
  initRandom(time(NULL), 0, NVAL);
  
#pragma omp parallel for private(iSamples) shared(s, nSamples)
    for (iSamples=0; iSamples<nSamples; iSamples++) {
    s[nextRnd()] += 1.0;
  }
  
 countNormalize(s);
  countPrint(s);
  
  countDelete(&s);
  
  return 0;
}
