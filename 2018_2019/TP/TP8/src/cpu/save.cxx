#include <iostream>
#include <fstream>
#include <string>
#include "calcul.h"

void save(int ksave, const double *u, int n)
{
  int i,j;
  std::string name = "cpu_out_";
  name += std::to_string(ksave) + ".vtr";
  std::ofstream f(name.c_str());

  f << "<?xml version=\"1.0\"?>\n"
    << "<VTKFile type=\"RectilinearGrid\">\n"
    << "<RectilinearGrid WholeExtent=\"0 "
    << n-1 << " 0 " << n-1 << " 0 0\">\n"
    << "<Piece Extent=\"0 "
    << n-1 << " 0 " << n-1 << " 0 0\">\n";

  f << "<PointData Scalars=\"u\">\n"
    << "<DataArray type=\"Float32\" Name=\"u\" format=\"ascii\">\n";
  
  for (i=0; i<n; i++) {
    for (j=0; j<n; j++)
      f << " " << u[i*n+j];
    f << "\n";
  }
  
  f << "</DataArray>\n"
    << "</PointData>\n";
    
  f << "<Coordinates>\n";
  f << "<DataArray type=\"Float32\" format=\"ascii\" "
    << "RangeMin=\"0\" RangeMax=\"" << n-1 << "\">";
  
  for (i=0; i<n; i++)
    f << " " << i*1.0/n ;
  f << "</DataArray>\n";
  f << "<DataArray type=\"Float32\" format=\"ascii\" "
    << "RangeMin=\"0\" RangeMax=\"" << n-1 << "\">";
  
  for (i=0; i<n; i++)
    f << " " << i*1.0/n ;
  f << "</DataArray>\n";
  
  f << "<DataArray type=\"Float32\" format=\"ascii\">\n0.0\n</DataArray>\n";
  f << "</Coordinates>\n";
  
  f << "</Piece>\n</RectilinearGrid>\n</VTKFile>\n";
  
}
