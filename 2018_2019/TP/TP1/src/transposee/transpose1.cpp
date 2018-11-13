#include <cstring>
#include <cmath>
#include "Matrice.hpp"
#include "timer.hpp"

int main(int argc, char **argv)
{
  int i, j;
  int n = argc > 1 ? strtol(argv[1], nullptr, 10) : 1000;
  int m = argc > 2 ? strtol(argv[2], nullptr, 10) : 2000;

  Matrice a(n,m), b(m,n);

  init(a);

  Timer t;

  t.start();

  for (i=0; i<n; i++)
    for (j=0; j<m; j++)
      b(i,j) = a(j,i);

  t.stop();

  if (n<10 && m<10) {
     std::cout << "A" << std::endl << a << std::endl;
     std::cout << "B" << std::endl << b << std::endl;
  }
 return 0;
}