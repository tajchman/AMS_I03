#include <vector>
#include <iostream>
#include <cstdlib>
#include <cmath>

#include "calcul.hxx"
#include "stat.hxx"
#include "sin.hxx"
#include "save.hxx"
#include "timer.hxx"
#include "GetPot.hxx"
#include <iostream>

int main(int argc, char **argv)
{
  Timer T_total, T_calcul, T_stat, T_save;
  T_total.start();
  
  {
    GetPot G(argc, argv);
    size_t n = G("n", 2000);
    size_t imax = G("imax", IMAX);

    set_terms(imax);
     
    std::cout << "\nn       : " << n
	      << "\n" << std::endl;

    std::vector<double> pos(n, 0), v1(n, 0), v2(n, 0);
   
    T_calcul.start();

    calcul(pos, v1, v2, 0, n);

    T_calcul.stop();
    T_stat.start();

    double m, e;
  
    stat(v1, v2, 0, n, m, e);

    m = m/n;
    e = sqrt(e/n - m*m);
    std::cout << "m = " << m << " e = " << e << std::endl << std::endl;

    T_stat.stop();
    T_save.start();

    if (n <= 5000)
      save("sinus_seq.dat", pos, v1, v2);

    T_save.stop();
  }
  T_total.stop();
  std::cout << "temps calcul : " << T_calcul.elapsed() << " s" << std::endl;
  std::cout << "temps stat   : " << T_stat.elapsed() << " s" << std::endl;
  std::cout << "temps save   : " << T_save.elapsed() << " s" << std::endl;
  std::cout << std::endl;
  std::cout << "temps total  : " << T_total.elapsed() << " s" << std::endl;
  return 0;
}