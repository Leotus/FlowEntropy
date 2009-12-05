
#include "Lattice.h"

Lattice::Lattice(int xlen, int ylen, int zlen, int ghost, int np) {

  xdim = xlen; 
  ydim = ylen; 
  zdim = zlen; 
  vb_list = calc_subvolume(xlen, ylen, zlen, 
			   ghost, np, 
			   &lattice, 
			   idim, jdim, kdim); 
  npart = np; 
}

// look up the lattice[i,j,k] element to get the subdomain number (rank)
int Lattice::GetRank(int i, int j, int k) {

  if (i<0 || i >=idim) return(-1); 
  else if (j<0 || j >=jdim) return(-1); 
  else if (k<0 || k >=kdim) return(-1); 

  int idx = k*idim*jdim+j*idim+i; 
  return (lattice[idx]); 
}

// Find the subdomain that contains the physical location (x,y,z) 
int Lattice::GetRank(float x, float y, float z) {
  
  for (int i=0; i<npart; i++) {
    if (vb_list[i].xmin>x ||vb_list[i].xmax<x) continue; 
    else if (vb_list[i].ymin>y ||vb_list[i].ymax<y) continue; 
    else if (vb_list[i].zmin>z ||vb_list[i].zmax<z) continue; 
    return(i); 
  }
  return(-1); 
}

// find the indices of the lattice element that has its subdomain number = rank 
int Lattice::GetIndices(int rank, int &iidx, int &jidx, int &kidx) {
  
  if (rank <0 || rank >= npart) return(-1); 
  for (int i=0; i<npart; i++) {
    if (lattice[i] ==rank) {
      kidx = i/(idim*jdim) ; 
      jidx = (i % (idim*jdim))/idim; 
      iidx = i % idim; 
    } 
  }
  return(1); 
}
// find the indices of the lattice element that contanis (x,y,z) 
int Lattice::GetIndices(float x, float y, float z, int &iidx, int &jidx, int &kidx) {

  int rank = GetRank(x,y,z); 
  if (rank!=-1) {
    GetIndices(rank, iidx, jidx, kidx); 
    return(rank); 
  }
  else return(-1); 

}

// look up the volume bounds of lattice[i,j,k] 
int Lattice::GetBounds(int i, int j, int k, volume_bounds_type& vb)  {

  if (i<0 || i >=idim) return(-1); 
  else if (j<0 || j >=jdim) return(-1); 
  else if (k<0 || k >=kdim) return(-1); 
  
  int idx = k*idim*jdim+j*idim+i; 
  vb = vb_list[lattice[idx]]; 
  return(1); 
}

// look up the volume bounds of the subdomain 'rank' 
int Lattice::GetBounds(int rank, volume_bounds_type &vb) {

  if (rank <0 || rank >= npart) return(-1); 
  vb = vb_list[rank]; 
  return(1); 

}

//check if the point (x,y,z) is in the lattice element [i,j,k]
bool Lattice::isIn(float x, float y, float z, int i, int j, int k) 
{
  if (i<0 || i>idim-1 || j<0 || j>jdim-1 || k<0 || k>kdim-1) return(false); 
  int idx = k *idim*jdim + j*idim + k; 
  volume_bounds_type vb = vb_list[lattice[idx]]; 
  if (vb.xmin>x || vb.xmax<x) return (false); 
  else if (vb.ymin>y || vb.ymax<y) return (false); 
  else if (vb.zmin>z || vb.zmax<z) return (false); 
  return(true); 
}

int Lattice::CheckNeighbor(int myrank, float x, float y, float z)
{
  int i,j,k; 
  GetIndices(myrank, i, j, k); 
  if (isIn(x,y,z,i-1,j,k)==true) return(0);  //-X
  if (isIn(x,y,z,i+1,j,k)==true) return(1);  //+X
  if (isIn(x,y,z,i,j-1,k)==true) return(2);  //-Y
  if (isIn(x,y,z,i,j+1,k)==true) return(3);  //+Y
  if (isIn(x,y,z,i,j,k-1)==true) return(4);  //-Z
  if (isIn(x,y,z,i,j,k+1)==true) return(5);  //+Z
  return(-1); 
}

void Lattice::InitSeedLists() 
{
  seedlists = new list<VECTOR3>[npart]; 
  for (int i=0; i<npart; i++)
    seedlists[i].clear(); 
}

void Lattice::ResetSeedLists() 
{
  for (int i=0; i<npart; i++)
    seedlists[i].clear(); 
}

void Lattice::InsertSeed(int i, int j, int k, VECTOR3 p) 
{
  int rank = GetRank(i,j,k); 
  //  printf("Lattice::InsertSeed to %d\n", rank); 
  seedlists[rank].push_back(p); 
}

