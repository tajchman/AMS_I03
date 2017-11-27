/*
 * scheme.cxx
 *
 *  Created on: 5 janv. 2016
 *      Author: tajchman
 */
#include "scheme.hxx"
#include <iostream>
#include <cmath>

double iterate(const Values & u1, Values & u2,
               double dt, Parameters &P)
{
  double du_sum = 0.0, du;
  double mu = 0.5*dt/P.dx(0);
  double dx2 = P.dx(0)*P.dx(0) + P.dx(1)*P.dx(1) + P.dx(2)*P.dx(2);
  double lambda = 0.25*dt/(dx2 + 1e-12);

  int i, j, k;
  int   di = P.di(0),     dj = P.di(1),     dk = P.di(2);
  int imin = P.imin(0), jmin = P.imin(1), kmin = P.imin(2);
  int imax = P.imax(0), jmax = P.imax(1), kmax = P.imax(2);

  bool diffusion = P.diffusion();
  bool convection = P.convection();
  
  for (i=imin; i<imax; i++)
    for (j=jmin; j<jmax; j++)
      for (k=kmin; k<kmax; k++) {
	du = 0;
	if (diffusion) {
	  du += lambda*(6*u1(i,j,k)
			- u1(i+di,j,k) - u1(i-di,j,k)
			- u1(i,j+dj,k) - u1(i,j-dj,k)
			- u1(i,j,k+dk) - u1(i,j,k-dk));
	}
	if (convection) {
	  du += mu*(u1(i,j,k) - u1(i-1, j, k));
	}
	du_sum += std::abs(du);
	u2(i,j,k) = u1(i,j,k) - du;
      }
   
  return du_sum;
}


