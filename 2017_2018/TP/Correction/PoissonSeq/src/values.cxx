#include "values.hxx"
#include <fstream>
#include <sstream>
#include <cstdlib>
#include <cstring>

Values::Values(const Parameters * prm)
{
  m_p = prm;
  int i, nn = 1;
  int n = m_p->n(0), m = m_p->n(1), p = m_p->n(2);
  for (i=0; i<3; i++)
    nn *= (m_n[i] = m_p->n(i));

  n1 = m_n[2];      // nombre de points dans la premiere direction
  n2 = m_n[1] * n1; // nombre de points dans le plan des 2 premieres directions
  
  allocate(nn);
}

void Values::init(double (*f)(double, double, double))
{
  int i, j, k;
   int imin = prm->imin[0];
   int jmin = 1;
   int kmin = 1;

   int imax = n-1;
   int jmax = m-1;
   int kmax = p-1;

  if (f) {
    double dx = m_p->dx(0), dy = m_p->dx(1), dz = m_p->dx(2);
    double xmin =  m_p->xmin(0);
    double ymin =  m_p->xmin(1);
    double zmin =  m_p->xmin(2);

    for (i=0; i<n; i++)
      for (j=0; j<m; j++)
        for (k=0; k<p; k++)
          operator()(i,j,k) = f(xmin + i*dx, ymin + j*dy, zmin + k*dz);
  }
  else {
    for (i=0; i<n; i++)
      for (j=0; j<m; j++)
        for (k=0; k<p; k++)
          operator()(i,j,k) = 0.0;

  }
}

void Values::print(std::ostream & f) const
{
    size_t i, n = m_n[0];
    size_t j, m = m_n[1];
    size_t k, p = m_n[2];

    for (i=0; i<n; i++) {
      for (j=0; j<m; j++) {
        for (k=0; k<p; k++)
          f << " " << operator()(i,j,k);
        f << std::endl;
        }
        f << std::endl;
      }
}

void Values::swap(Values & other)
{
  double * dtemp = m_u;
  m_u = other.m_u;
  other.m_u = dtemp;

  int i, temp;
  for (i=0; i<3; i++) {
    temp = m_n[i];
    m_n[i] = other.m_n[i];
    other.m_n[i] = temp;
  }
}

void Values::plot(int order) const {

  std::ostringstream s;
  int i, j, k;
  s << m_p->resultPath() << "plot_"
    << order << ".vtr";
  std::ofstream f(s.str().c_str());

  f << "<?xml version=\"1.0\"?>\n";
  f << "<VTKFile type=\"RectilinearGrid\" version=\"0.1\" byte_order=\"LittleEndian\">\n"
    << "<RectilinearGrid WholeExtent=\""
    << 0 << " " << m_p->n(0) - 1  << " " 
    << 0 << " " << m_p->n(1) - 1  << " " 
    << 0 << " " << m_p->n(2) - 1 
    << "\">\n"
    << "<Piece Extent=\""
    << 0 << " " << m_p->n(0) - 1 << " " 
    << 0 << " " << m_p->n(1) - 1 << " " 
    << 0 << " " << m_p->n(2) - 1 
    << "\">\n";

  f << "<PointData Scalars=\"values\">\n";
  f << "  <DataArray type=\"Float64\" Name=\"values\" format=\"ascii\">\n";
  
  for (k=0; k< m_p->n(2); k++) {
    for (j=0; j< m_p->n(1); j++) {
      for (i=0; i< m_p->n(0); i++) {
        f << " " << operator()(i,j,k);
      }
      f << "\n";
    }
  }
  f << " </DataArray>\n";
   
  f << "</PointData>\n";

  f << " <Coordinates>\n";
  
  for (k=0; k<3; k++) {
    f << "   <DataArray type=\"Float64\" Name=\"" << char('X' + k) << "\"" 
      << " format=\"ascii\">";
    
    for (i=0; i<m_p->n(k); i++)
      f << " " << i * m_p->dx(k);
    f << "   </DataArray>\n";
  }
  f << " </Coordinates>\n";
  
  f << "</Piece>\n"
    << "</RectilinearGrid>\n"
    << "</VTKFile>\n" <<std::endl;
}

void Values::allocate(size_t n)
{
	deallocate();
	m_u = new double [n];
}

void Values::deallocate()
{
	if (m_u == NULL) {
       delete [] m_u;
       m_u = NULL;
	}
}

void Values::operator= (const Values &other)
{
  int i;
  size_t nn = 1;
  
  for (i=0; i<3; i++)
    nn *= (m_n[i] = other.m_n[i]);
  
  allocate(nn);
  memcpy(m_u, other.m_u, nn*sizeof(double));
}
