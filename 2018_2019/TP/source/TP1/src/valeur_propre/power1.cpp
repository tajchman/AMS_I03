#include <iostream>
#include <iomanip>
#include <cstring>
#include <cmath>
#include <ctime>
#include "Matrice.hpp"
#include "Vecteur.hpp"
#include "timer.hpp"

void init(Matrice &a, Vecteur & v)
{
  int i, j, n = v.size();
  
  std::srand(std::time(nullptr));
  
  for (i=0; i<n; i++)
    v(i) = std::rand();
  v.normalise();

  for (i=0; i<n; i++)
    for (j=0; j<n; j++)
      a(i,j) = 1.0/n;
  
  for (i=0; i<n; i++)
    a(i,i) = 5 + 1.0/n;
}

void produit_matrice_vecteur(Vecteur &w, Matrice &a, Vecteur & v)
{
  int n = a.n(),i,j;
  double s;
  
  w = 0.0;
  
  for (j=0; j<n; j++) {
    s = v(j);
    for (i=0; i<n; i++)
      w(i) += a(i,j) * s;
  }
}

double variation(double a, double b)
{
  return std::abs(a-b)/(std::abs(a) + std::abs(a) + 1.0);
}


int main(int argc, char **argv)
{
  Timer t_total;
  t_total.start();

  int n = argc > 1 ? strtol(argv[1], nullptr, 10) : 1000;

  {
    Timer t;
    t.start();

    Matrice a(n,n);
    Vecteur v(n), w(n);

    init(a, v);

    t.stop();
    std::cerr << "init    time : " << t.elapsed() << " s" << std::endl;

    t.reinit();
    t.start();
    
    double lambda = 0.0, lambda0;
    int k, kmax = 100;
    for(k=0; k < kmax; k++) {

      lambda0 = lambda;
      
      produit_matrice_vecteur(w, a, v);

      lambda = w.normalise();
      v = w;

      affiche(k, lambda);

      if (variation(lambda,lambda0) < 1e-12)
        break;
    }
    std::cerr << std::endl;
    t.stop();
    std::cerr << "compute time : " << t.elapsed() << " s"  << std::endl;
  }
  t_total.stop();
  std::cerr << "total time   : " << t_total.elapsed() << " s"  << std::endl;

  return 0;
}
