#include <iostream>
#include <iomanip>
#include <cstring>
#include <cmath>
#include <ctime>
#include "MatriceBloc.hpp"
#include "Vecteur.hpp"
#include "util.h"
#include "timer.hpp"

void init(MatriceBloc &A, Vecteur & V)
{
  int i, j, k, l, n = V.size(), p = A.p(), q = A.q(), nb = A.n(), mb = A.m();
  
  std::srand(std::time(nullptr));
  
  for (i=0; i<n; i++)
    V(i) = std::rand();
  V.normalise();

  for (i=0; i<nb; i++) {
    for (j=0; j<mb; j++) {
      
      Matrice & bA = A(i,j);
      
      for (k=0; k<p; k++)
        for (l=0; l<q; l++) {
          bA(k,l) = 1.0/n;
        }
    }
    Matrice & bA = A(i,i);
    for (k=0; k<p; k++)
      bA(k,k) += 5.0;
  }
}

inline
double dot(double *a, double *b, int n)
{
  int l = 0, lmax = (n/4) * 4;
  double s = 0;
  // for (; l<lmax; l+=4)
  //    s += a[l] * b[l] + a[l+1] * b[l+1] + a[l+2] * b[l+2] + a[l+3] * b[l+3];
  for (; l<n; l++)
     s += a[l] * b[l];
  return s;
}

inline
void produit_matrice_vecteur(Vecteur &W, MatriceBloc &A, Vecteur & V)
{
  int nb = A.n(),mb = A.m(), i,j,k,l, p= A.p(), q = A.q();
  double s;
  
  for (i=0; i<nb; i++) {
    double * bW = &(W(i*p));
    for (k=0; k<p; k++) bW[k] = 0.0;
    
    for (j=0; j<mb; j++) {
      
      Matrice & bA = A(i,j);
      double * bV = &(V(j*q));
      
      for (k=0; k<p; k++)
	bW[k] += dot(&(bA(k,0)), bV, q);
    }
  }
}


inline
double variation(double a, double b)
{
  return std::abs(a-b)/(std::abs(a) + std::abs(a) + 1.0);
}


int main(int argc, char **argv)
{
  Timer t_total;
  t_total.start();

  int i, j;
  int n = argc > 1 ? strtol(argv[1], nullptr, 10) : 3000;
  int p = argc > 3 ? strtol(argv[3], nullptr, 10) : 100;  

  {
    Timer t;
    t.start();

    MatriceBloc a(n/p,n/p, p, p);
    Vecteur v(n), w(n);

    init(a, v);

    t.stop();
    std::cerr << "init    time : " << t.elapsed() << " s" << std::endl;

    t.reinit();
    t.start();
    
    double s, lambda = 0.0, lambda0;
    int k, kmax = 65;
    for(k=0; k < kmax; k++) {

      lambda0 = lambda;

      produit_matrice_vecteur(w, a, v);

      lambda = w.normalise();
      v = w;

      affiche(k, lambda);

      // if (variation(lambda,lambda0) < 1e-12)
      //   break;
    }
    std::cerr << std::endl;
    t.stop();
    std::cerr << "compute time : " << t.elapsed() << " s"  << std::endl;
  }
  t_total.stop();
  std::cerr << "total time   : " << t_total.elapsed() << " s"  << std::endl;

  return 0;
}
