#include <vector>
#include <iostream>
#include <cstdlib>
#include <cmath>

#include <mpi.h>

#ifdef _OPENMP
#include <omp.h>
#define NTHREADS omp_get_num_threads()
#define ITHREAD  omp_get_thread_num()
#else
#define NTHREADS 1
#define ITHREAD  0
#endif

#include "sin.hxx"
#include "timer.hxx"

void init(std::vector<double> & pos,
          std::vector<double> & v1,
          std::vector<double> & v2,
          int n1, int n2)
{
  double x, pi = 3.14159265;
  int i, n = pos.size();

#pragma omp parallel for private(x)
  for (i=n1; i<n2; i++) {
    x = i*2*pi/n;
    pos[i] = x;
    v1[i] = sinus_machine(x);
    v2[i] = sinus_taylor(x);
  }
}

void stat(const std::vector<double> & v1,
          const std::vector<double> & v2,
          int n1, int n2,
          double & sum1, double & sum2)
{
  double s1 = 0.0, s2 = 0.0, err;
  int i;

#pragma omp parallel for private(err) shared(s1,s2)
  for (i=n1; i<n2; i++) {
    err = v1[i] - v2[i];
    s1 += err;
    s2 += err*err;
  }

  sum1 = s1;
  sum2 = s2;
}

int main(int argc, char **argv)
{

  Timer time;
  time.start();
  
  size_t n = argc > 1 ? strtol(argv[1], NULL, 10) : 2000;
  int imax = argc > 2 ? strtol(argv[2], NULL, 10) : IMAX;
  set_terms(imax);

	int nprocs, irank;
  
  int required = MPI_THREAD_FUNNELED, provided;
  MPI_Init_thread(&argc, &argv, required, &provided);
  if (provided < required) {
     std::cerr << "Interaction MPI - OpenMP insuffisante" << std::endl;
     MPI_Finalize();
     return -1;
  }

  MPI_Comm_size(MPI_COMM_WORLD, &nprocs);
  MPI_Comm_rank(MPI_COMM_WORLD, &iproc);

  int di = n/nprocs;
  int i_start = di * irank;
  int i_end = (irank+di < n) ? irank+di : n;
  dt = i_end-i_start;

  std::vector<double> pos(dt), v1(dt), v2(dt);
   
  init(pos, v1, v2, i_init, i_end);

  double m_local, e_local;
  
  stat(v1, v2, i_init, i_end, m_local, e_local);

  double m,e;

  MPI_Allreduce(&m_local, &m, 1, MPI_DOUBLE, MPI_SUM,
             MPI_COMM_WORLD);
  MPI_Allreduce(&e_local, &e, 1, MPI_DOUBLE, MPI_SUM,
             MPI_COMM_WORLD);

  MPI_Finalise();
  
  m = m/n;
  e = sqrt(e/n - m*m);
  std::cout << "m = " << m << " e = " << e << std::endl;

  time.stop();

  std::cout << "Le temps a ete " << time.elapsed() << std::endl;
  return 0;
}
