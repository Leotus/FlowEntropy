
#include "entropy.h"

#if	0	// DEL-BY-LEETEN 01/22/2010-BEGIN
	#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
	#include <CGAL/Triangulation_3.h>
	#include <CGAL/Delaunay_triangulation_3.h>

	#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
	#include <CGAL/Delaunay_triangulation_3.h>
	#include <CGAL/Triangulation_vertex_base_with_info_3.h>
	#include <CGAL/IO/Color.h>
#endif	// DEL-BY-LEETEN 01/22/2010-END

#include <iostream>
#include <fstream>
#include <cassert>
#include <list>
#include <vector>
#include <windows.h>

#include "liblog.h"					// ADD-BY-LEETEN 02/02/2010

#include "FlowDiffusion3DConfig.h"	// ADD-BY-LEETEN 02/02/2010

// ADD-BY-LEETEN 2009/11/10-BEGIN
//#include "../myinterface/FlowDiffusionCuda.h"
// ADD-BY-LEETEN 2009/11/10-END

#if	0	// DEL-BY-LEETEN 01/22/2010-BEGIN
	typedef CGAL::Exact_predicates_inexact_constructions_kernel K;

	typedef CGAL::Triangulation_3<K>      Triangulation;

	typedef Triangulation::Cell_handle    Cell_handle;
	typedef Triangulation::Cell_iterator  Cell_iterator;
	typedef Triangulation::Vertex_handle  Vertex_handle;
	typedef Triangulation::Locate_type    Locate_type;
	typedef Triangulation::Point          Point;
	typedef Triangulation::Finite_cells_iterator Finite_cells_iterator; 
	  
	typedef CGAL::Exact_predicates_inexact_constructions_kernel K;

	typedef CGAL::Triangulation_vertex_base_with_info_3<CGAL::Color, K> Vb;
	typedef CGAL::Triangulation_data_structure_3<Vb>                    Tds;
	typedef CGAL::Delaunay_triangulation_3<K, Tds>                      Delaunay;

#define pi 3.14
float dot(Point p1, Point p2)
{
	return (p1.x()*p2.x()+p1.y()*p2.y());
}

#endif	// DEL-BY-LEETEN 01/22/2010-END

bool discardredundantstreamlines(float& cur_entropy,float eplison, list<vtListSeedTrace*> new_lines,
								 float* vectors, float* new_vectors,int* grid_res,
								 int* bin_my_vector,int* bin_new_vector,int binnum,
								 float* theta, float* phi,
								 int* histo_puv,int* histo_pv,float* pv,float* entropy_tmp)
{
	// MOD-BY-LEETEN 02/02/2010-FROM:
		// int* dummy=new int[grid_res[0]*grid_res[1]*grid_res[2]];
	// TO:
	int* dummy=NULL;
	// MOD-BY-LEETEN 02/02/2010-END
	float* tmp_new_vectors=new float[grid_res[0]*grid_res[1]*grid_res[2]*3];
	memcpy(tmp_new_vectors,new_vectors,sizeof(float)*grid_res[0]*grid_res[1]*grid_res[2]*3);
	
	float* kx=new float[grid_res[0]*grid_res[1]*grid_res[2]];
	float* ky=new float[grid_res[0]*grid_res[1]*grid_res[2]];
	float* kz=new float[grid_res[0]*grid_res[1]*grid_res[2]];

	float* b=new float[grid_res[0]*grid_res[1]*grid_res[2]];
	float* c1=new float[grid_res[0]*grid_res[1]*grid_res[2]];
	float* c2=new float[grid_res[0]*grid_res[1]*grid_res[2]];
	float* c3=new float[grid_res[0]*grid_res[1]*grid_res[2]];
	memset(kx,0,sizeof(float)*grid_res[0]*grid_res[1]*grid_res[2]);
	memset(ky,0,sizeof(float)*grid_res[0]*grid_res[1]*grid_res[2]);
	memset(kz,0,sizeof(float)*grid_res[0]*grid_res[1]*grid_res[2]);
	memset(b,0,sizeof(float)*grid_res[0]*grid_res[1]*grid_res[2]);
	memset(c1,0,sizeof(float)*grid_res[0]*grid_res[1]*grid_res[2]);
	memset(c2,0,sizeof(float)*grid_res[0]*grid_res[1]*grid_res[2]);
	memset(c3,0,sizeof(float)*grid_res[0]*grid_res[1]*grid_res[2]);

	reconstruct_field_GVF_3D(new_vectors,vectors,grid_res,new_lines,dummy,
							kx,ky,kz,b,c1,c2,c3);
	delete [] kx;
	delete [] ky;
	delete [] kz;
	delete [] b;
	delete [] c1;
	delete [] c2;
	delete [] c3;

	float entropy=0;
	int *new_bin;
	new_bin=new int [grid_res[0]*grid_res[1]*grid_res[2]];
	for(int i=0;i<grid_res[0]*grid_res[1]*grid_res[2];i++)
		{
			VECTOR3 newf,orif;
			//orif.Set(vectors[i*3+0],vectors[i*3+1],vectors[i*3+2]);
			newf.Set(new_vectors[i*3+0],new_vectors[i*3+1],new_vectors[i*3+2]);
			//if within error range, let it be teh same bin as the ori, otherwise select the bin num
			newf.Normalize();
			//float dotv=dot(newf,orif);
			new_bin[i]=get_bin_number_3D(newf,theta, phi,binnum);
		}
	entropy=calcRelativeEntropy6_new(vectors, new_vectors,  grid_res, VECTOR3(2,2,2),//do not count boundaries
								VECTOR3(grid_res[0]-2,grid_res[1]-2,grid_res[2]-2),theta,phi,bin_my_vector,new_bin,0,binnum,
								 histo_puv, histo_pv, pv,entropy_tmp,
						  0,0);
	// MOD-BY-LEETEN 02/04/2010-FROM:
		// printf(" entropy dif=%f\n",cur_entropy-entropy);
	// TO:
	printf(" entropy dif=%+f\n",cur_entropy-entropy);
	// MOD-BY-LEETEN 02/04/2010-END

	//printf(" cur_entropy=%f\n",cur_entropy);
	//printf(" entropy =%f\n",entropy);

	if(cur_entropy-entropy>eplison)//keep
	{
		cur_entropy=entropy;
		//memcpy(bin_new_vector,new_bin,sizeof(int)*grid_res[0]*grid_res[1]*grid_res[2]);
		// DEL-BY-LEETEN 02/02/2010		delete [] dummy;
		delete [] tmp_new_vectors;
		delete [] new_bin;
		return false;
	}
	else//discard
	{
		memcpy(new_vectors,tmp_new_vectors,sizeof(float)*grid_res[0]*grid_res[1]*grid_res[2]*3);
		delete [] tmp_new_vectors;
		// DEL-BY-LEETEN 02/02/2010		delete [] dummy;
		delete [] new_bin;
		return true;
	}
}
void dumpArray( vector<float> entropies,  char* filename)
{

	int num=entropies.size(); 


	FILE* fp=fopen(filename,"w");
	if(fp==0)
	{
		printf("can not open file to write\n");
		return ;//return;
	}
	fprintf(fp,"%d \n",num-1);
	for (int i=0; i<num-1; i++) 
	{
		fprintf(fp,"%f ", entropies[i+1]-entropies[i]);
		fprintf(fp,"\n");
	}
	fclose(fp);

}
void clearupStreamlines(list<vtListSeedTrace*> sl_list)
{
	std::list<vtListSeedTrace*>::iterator pIter; 

	pIter = sl_list.begin(); 
	for (; pIter!=sl_list.end(); pIter++) {
		vtListSeedTrace *trace = *pIter; 
		std::list<VECTOR3*>::iterator pnIter; 
		pnIter = trace->begin(); 
		for (; pnIter!= trace->end(); pnIter++) {
			delete [] *pnIter; 

		}
		trace->clear();
	}

	sl_list.clear(); 
}

void getVectorAt(int x, int y, float& fx, float& fy, int * grid_res,float* vectors)
{
	int idx=x+y*grid_res[0];
	fx=vectors[idx*3+0];
	fy=vectors[idx*3+1];
}

#if	0	// DEL-BY-LEETEN 01/22/2010-BEGIN
	void doTriangulation3D(list<vtListSeedTrace*> sl_list,std::vector<VECTOR3>& tetrahedras,int* grid_res)
	{

		std::list<Point> myPts;
		std::list<vtListSeedTrace*>::iterator pIter; 

		pIter = sl_list.begin(); 
		for (; pIter!=sl_list.end(); pIter++) {
			vtListSeedTrace *trace = *pIter; 
			std::list<VECTOR3*>::iterator pnIter; 
			pnIter = trace->begin(); 
			//int skip=0;
			for (; pnIter!= trace->end(); pnIter++) {
				VECTOR3 p = **pnIter; 
			//	printf(" %f %f %f ", p[0], p[1], p[2]); 
				//if(skip%150==0);
				myPts.push_back(Point(p[0], p[1],p[2]));
				//skip++;

			}
		}

		Delaunay T(myPts.begin(), myPts.end());
		Point p[4],q[4];

		tetrahedras.clear();
		Delaunay::Finite_cells_iterator vit;
		for (vit = T.finite_cells_begin(); vit != T.finite_cells_end(); ++vit)
		{
			for(int i=0; i<4; i++)
			tetrahedras.push_back(	VECTOR3(vit->vertex(i)->point().x(),
											vit->vertex(i)->point().y(),
											vit->vertex(i)->point().z()));	
		}
	}
#endif	// DEL-BY-LEETEN 01/22/2010-END

// ADD-BY-LEETEN 01/22/2010-BEGIN
#define	_USE_MATH_DEFINES
#include <math.h>
#define pi	M_PI
// ADD-BY-LEETEN 01/22/2010-END


void save2PPM(char* filename, unsigned char* data, int xdim, int ydim)
{
	FILE* fp = fopen(filename, "w");
	if (fp == NULL)
		return;

	fprintf(fp, "P3\n%d %d\n255\n", xdim, ydim);

	for (int  i = 0; i <ydim; i++)
	{
		for (int j = 0; j < xdim; j++) 
		{
			int idx = i * xdim + j;

			int r,g,b; r=g=b=0;
			unsigned char val=data[idx];

			r=g=b=val;

			fprintf(fp, " %d %d %d", r,g,b); 
		}
	}


	fclose(fp);
}
void combinehalflines_check_stop_entropy(list<vtListSeedTrace*> lines, list<vtListSeedTrace*>& long_lines,int* grid_res,
										 float* entropies)
{
	int shortest_line=1;
	vtListSeedTrace* newlist;
	list<vtListSeedTrace*>::iterator pIter;
	pIter =lines.begin(); 
	int sou=0;
	for (; pIter!=lines.end(); pIter++) 
	{
		std::list<VECTOR3*>::iterator pnIter; 
		if(sou%2==0)//create new list
			newlist=new vtListSeedTrace();
		vtListSeedTrace *trace = *pIter; 
		pnIter = trace->begin(); 
		bool short_line=false;
		
		for (; pnIter!= trace->end(); pnIter++) 
		{
			VECTOR3 p = **pnIter; 
			int idx=(int)(p.x())+((int)(p.y()))*grid_res[0]+((int)(p.z()))*grid_res[0]*grid_res[1];
			
			if(sou%2==0)
			newlist->push_front(new VECTOR3(p.x(),p.y(),p.z()));
			else
			newlist->push_back(new VECTOR3(p.x(),p.y(),p.z()));

			float prob=entropies[idx];
			if(prob<0.1)
			{
			bool valid=false;
			float u = ((float)rand()/(float)RAND_MAX);
			float prob=entropies[idx];
			
			if(u<prob)
				valid=true;
			if (valid==false)//walk for several steps and stop
			{
				short_line=true;
				//break;
			}
			if(short_line==true)
			{
				if(newlist->size()>shortest_line)
					break;
			}
			}
		}
		if((sou%2==1)&&newlist->size()>10)
			long_lines.push_back(newlist);
		sou++;
	}

}
void calc_entropy( int* bins,int* grid_res, int binnum,float* entropies)
{
	int radius[3];
	// MOD-BY-LEETEN 02/02/2010-FROM:
		// radius[0]=radius[1]=8; radius[2]=8;
	// TO:
	radius[0]=KERNEL_HALF_WIDTH;
	radius[1]=KERNEL_HALF_HEIGHT;
	radius[2]=KERNEL_HALF_DEPTH;
	// MOD-BY-LEETEN 02/02/2010-END
	VECTOR3 startpt,endpt;
	for(int z=0;z<grid_res[2];z++)
	{
		for(int y=0;y<grid_res[1];y++)
		{
			for(int x=0;x<grid_res[0];x++)
			{
				float sx,sy,sz,ex,ey,ez;
				sx=max(0,x-radius[0]);	sy=max(0,y-radius[1]);	sz=max(0,z-radius[2]);
				ex=min(x+radius[0],grid_res[0]-1);
				ey=min(y+radius[1],grid_res[1]-1);
				ez=min(z+radius[2],grid_res[2]-1);

				startpt.Set(sx,sy,sz);
				endpt.Set(ex,ey,ez);

				float tmp=calcEntropy_known_bins(bins,grid_res, startpt,endpt, binnum);

				entropies[x+y*grid_res[0]+z*grid_res[0]*grid_res[1]]=tmp;
			}
		}
		printf("z=%d/%d\r",z,grid_res[2]);
	}
	dumpImportanceField("importance.bin",entropies, grid_res);

}
//crop the image manaually
void save2PPM_1(char* filename, unsigned char* data, int xdim, int ydim)
{
	FILE* fp = fopen(filename, "w");
	if (fp == NULL)
		return;
	int crop=0;//135;

	fprintf(fp, "P3\n%d %d\n255\n", xdim, ydim-crop*2);

	for (int  i = crop; i <ydim-crop; i++)
	{
		for (int j = 0; j < xdim; j++) 
		{
			int idx = i * xdim + j;

			int r,g,b; 
			r=data[idx];
			g=b=r;
			fprintf(fp, " %d %d %d", r,g,b); 
		}
	}


	fclose(fp);
}
//crop the image manaually
void save2PPM_3(char* filename, unsigned char* data, int xdim, int ydim)
{
	FILE* fp = fopen(filename, "w");
	if (fp == NULL)
		return;
	int crop=0;//135;

	fprintf(fp, "P3\n%d %d\n255\n", xdim, ydim-crop*2);

	for (int  i = ydim-crop-1;i>=crop; i--)
	{
		for (int j = 0; j < xdim; j++) 
		{
			int idx = i * xdim + j;

			int r,g,b; 
			r=data[idx*3+0];
			g=data[idx*3+1];
			b=data[idx*3+2];
			fprintf(fp, " %d %d %d", r,g,b); 
		}
	}


	fclose(fp);
}




void calcProbabilities(float* scalar, int datanum, int binnum, float* p)
{
	//1. calc histogram
	int * histo=new int[binnum];
	for(int i=0; i<binnum; i++)
		histo[i]=0;
	float min=9999; float max=0;
	for(int i=0; i<datanum; i++)
	{
		if(scalar[i]>max)
			max=scalar[i];
		if(scalar[i]<min)
			min=scalar[i];

	}
	if(max==min)
	{
		for(int i=0; i<binnum; i++)
			p[i]=(1.0/((float)(binnum)));
		return;
	}

	for(int i=0; i<datanum; i++)
	{
		float val=scalar[i];
		int j=(binnum-1)*val/(2*3.1415926);
		//int j=((val-min)/(max-min))*(binnum-1);//rescale to (0~1)
		if(j<0 || j>=binnum)
		{
			printf("sth wrong, bin id=%d\n", j);
			continue;
		}
		histo[j]+=1;
	}
	//2. calc probabilities
	//float* p=new float[binnum];
	for(int i=0; i<binnum; i++)
		p[i]=(((float)histo[i])/((float)(datanum)));

	delete [] histo;

}


float pt2LineDist(VECTOR3 q, vtListSeedTrace* trace,VECTOR3& p)
{
	float minDist=9999;
	std::list<VECTOR3*>::iterator pnIter; 
	pnIter = trace->begin(); 
	VECTOR3	p0,p1;
	p0=**pnIter++	;

	for (; pnIter!= trace->end(); pnIter++) {
		VECTOR3 pq;
		VECTOR3 p1 = **pnIter; 
		//printf(" %f %f %f ", p[0], p[1], p[2]); 
		float dist=pt2SegDist(p0, p1, q,pq);
		if(dist<minDist)
		{
			minDist=dist;
			//p.Set(p1.x()-p0.x(),p1.y()-p0.y(),0);
			p.Set(pq.x(),pq.y(),pq.z());
		}
		p0.Set(p1.x(),p1.y(),p1.z());
		//p0.Set(p1.x(),p1.y(),0);
	}
	
	return minDist;
}
float pt2SegDist(VECTOR3 p0, VECTOR3 p1, VECTOR3 q, VECTOR3& pq)
{


	VECTOR3 vi;
	float t;

	vi=p1-p0;
	float mag=vi.GetMag();
	float sqrLen=mag*mag;


	VECTOR3 tmp;
	tmp=q-p0;

	if(sqrLen==0)
		t=0;
	else
		t=dot(tmp,vi)/sqrLen;
	if(t<0)
		t=0;
	if(t>1)
		t=1;
	
	pq=p0+t*vi;//.Set(p0.x()+t*vi.x(),p0.y()+t*vi.y(),p0.z()+t*vi.z());

	tmp=q-pq;
	float dist=tmp.GetMag();
	return dist;
	
}
void calcDistanceField(char* filename, list<vtListSeedTrace*> lines, int* grid_res,int focus_num,float*f_dist )
{
	
	//float* f_dist=new float[grid_res[0]*grid_res[1]*grid_res[2]];
	float dist,mindist;

	VECTOR3 q;
	float maxdist=0;
	for(int z=0;z<grid_res[2];z++)
	{
	
		for(int y=0;y<grid_res[1];y++)
		{
			for(int x=0;x<grid_res[0];x++)
			{
				
				int idx=x+y*grid_res[0]+z*grid_res[0]*grid_res[1];
				q.Set(x,y,z);
				std::list<vtListSeedTrace*>::iterator pIter; 
				pIter = lines.begin(); 
				mindist=999999;
				int count=0;
				for (; pIter!=lines.end(); pIter++) 
				{
					if(count>=focus_num)
						break;
					count++;
					VECTOR3 p;
					vtListSeedTrace *trace = *pIter; 
					dist=pt2LineDist( q, trace,p);
					if(dist<mindist)
					{
						mindist=dist;
					}
					if(mindist>maxdist)
						maxdist=mindist;
					
				}
						
				f_dist[idx]=mindist;
			}
			
		}

		printf("calculating distance field, %d/%d is done\n", z, grid_res[2]);
				
	}

	//normalize distField first
	if(maxdist<=0)
		return;
	//we want to dump the dif between vectors and new_vectors
	for(int i=0;i<grid_res[0]*grid_res[1]*grid_res[2];i++)
	{
		f_dist[i]/=maxdist;
	}
	
	

}

	
	
/////////////////////////////////////////////////////////////////
///time varying part
////////////////////////////////////// ///////////////////////////

void dumpSeedandLines(VECTOR3* seeds,int nseed, OSUFlow* osuflow, char* filename)//crtical points excluded
{
	//1. gen streamlines

	osuflow->SetEntropySeedPoints( seeds,nseed);

	int nSeeds; 
	VECTOR3* printseeds = osuflow->GetSeeds(nSeeds); 
	for (int i=0; i<nSeeds; i++) 
		printf(" seed no. %d : [%f %f %f]\n", i, printseeds[i][0], 
		printseeds[i][1], printseeds[i][2]); 

	list<vtListSeedTrace*> line_list; 
	line_list.clear(); 

	printf("compute streamlines..\n"); 
	osuflow->SetIntegrationParams(1, 5); 
	osuflow->GenStreamLines(line_list , BACKWARD_AND_FORWARD, 500, 0); 
	printf(" done integrations\n"); 


	FILE* fp = fopen(filename, "wb"); //2d data
	if (fp == NULL) return; 
	fwrite(&nseed,sizeof(int),1,fp);	
	fwrite(seeds,sizeof(VECTOR3),nseed,fp);	

	//store the lines
	std::list<vtListSeedTrace*>::iterator pIter; 

	pIter = line_list.begin(); 
	for (; pIter!=line_list.end(); pIter++) {
		vtListSeedTrace *trace1 = *(pIter++); 
		vtListSeedTrace *trace2 = *(pIter); 
		int linelen=trace1->size()+trace2->size();
		fwrite(&linelen,sizeof(int),1,fp);	

		VECTOR3* tmpline=new VECTOR3[linelen];
		int c=0;
		std::list<VECTOR3*>::iterator pnIter; 
		pnIter = trace1->begin(); 
		for (; pnIter!= trace1->end(); pnIter++) {
			VECTOR3* p=*pnIter; 
			tmpline[c++].Set(p->x(),p->y(),0);
		}
		pnIter = trace2->begin(); 
		for (; pnIter!= trace2->end(); pnIter++) {
			VECTOR3* p=*pnIter; 
			tmpline[c++].Set(p->x(),p->y(),0);
		}
		fwrite(tmpline,sizeof(VECTOR3),linelen,fp);	
		delete [] tmpline;
	}

	line_list.clear(); 
	fclose(fp);

}

VECTOR3* loadSeedfromFile(char* fileseed, int& nSeed)
{
	FILE* fp=fopen(fileseed,"r");
	if(fp==0)
	{
		printf("seed file not found\n");
		0;//return;
	}
	int seednum;
	//	fread(&seednum, sizeof(int), 1, fp);
	fscanf(fp,"%d",&seednum);

	VECTOR3* seeds=new VECTOR3[seednum];

	for(int i=0; i<seednum;i++)
	{
		float fx,fy,fz;
		fscanf(fp,"%f",&fx);
		fscanf(fp,"%f",&fy);
		fscanf(fp,"%f",&fz);

		seeds[i].Set(fx,fy,fz);

	}
	//	fread(seeds, sizeof(VECTOR3), seednum, fp);			
	fclose(fp);
	nSeed=seednum;
	return seeds;
}


void dumpSeeds(vector<VECTOR3> seeds, char* filename)//crtical points excluded
{

	int seednum=seeds.size(); 


	FILE* fp=fopen(filename,"w");
	if(fp==0)
	{
		printf("seed file not found\n");
		return ;//return;
	}
	fprintf(fp,"%d \n",seednum);
	for (int i=0; i<seednum; i++) 
	{
		fprintf(fp,"%f ", seeds[i][0]);
		fprintf(fp,"%f ", seeds[i][1]);
		fprintf(fp,"%f ", seeds[i][2]);
		fprintf(fp,"\n");
	}
	fclose(fp);


}
VECTOR3* get_line_vertices(VECTOR3* vecs,int* ver_num, int line_no,int& length)
{
	int first_ver_num=0;
	for(int i=0;i<line_no;i++)
		first_ver_num+=ver_num[i];

	VECTOR3* ret=vecs+first_ver_num;
	length=ver_num[line_no];
	return ret;
}

VECTOR4* get_line_rgba(VECTOR4* rgba,int* ver_num, int line_no)
{
	int first_ver_num=0;
	for(int i=0;i<line_no;i++)
		first_ver_num+=ver_num[i];
	VECTOR4* ret=rgba+first_ver_num;
	return ret;
}	
/*
void adjustLengthByEntropies(char*  entropyfile,char* streamlinefile,char* outputstreamlinesfile)
{
	int grid_res[3];
	FILE* fp=fopen(entropyfile,"rb");
	fread(grid_res,sizeof(int),3,fp);
	float* entropies=new float[grid_res[0]*grid_res[1]*grid_res[2]];
	fread(entropies,sizeof(float),grid_res[0]*grid_res[1]*grid_res[2],fp);
	fclose(fp);

	int line_num,*ver_num,total_ver_num=0;
	fp=fopen(streamlinefile,"rb");
	fread(&line_num,sizeof(int),1,fp);
	ver_num=new int[line_num];
	fread(ver_num,sizeof(int),line_num,fp);
	for(int i=0;i<line_num;i++)
		total_ver_num+=ver_num[i];
	VECTOR3* verts=new VECTOR3[total_ver_num];
	fread(verts,sizeof(VECTOR3),total_ver_num,fp);
	VECTOR4* rgba=new VECTOR4[total_ver_num];
	fread(rgba,sizeof(VECTOR4),total_ver_num,fp);
	fclose(fp);

	int* tmp_ver_num=new int[line_num];
	memcpy(tmp_ver_num,ver_num,sizeof(int)*line_num);

	// DEL-BY-LEETEN 01/22/2010 srand((unsigned)time(NULL));

	//start prunning
	for(int i=0;i<line_num;i++)
	{
		int length;
		VECTOR3* line=get_line_vertices(verts, ver_num, i,length);
		for(int j=0;j<length;j++)
		{
			VECTOR3 p=line[j];
			int idx=(int)p[0]+((int)p[1])*grid_res[0]+((int)p[2])*grid_res[0]*grid_res[1];
			float u = ((float)rand()/(float)RAND_MAX);
			float prob=entropies[idx];
			
			if(u>prob)
			{//cut the line short here
				tmp_ver_num[i]=j+1;
				break;
			}

		}
	}

	
	int selected_line_num=0;
	int total_selected_ver=0;
	for(int i=0;i<line_num;i++)
	{
		if(tmp_ver_num[i]>0)
			selected_line_num++;
		total_selected_ver+=tmp_ver_num[i];

	}

	int* selected_ver_num=new int[selected_line_num];
	int count=0;
	for(int i=0;i<line_num;i++)
	{
		if(tmp_ver_num[i]<=0)
			continue;
		selected_ver_num[count++]=tmp_ver_num[i];
	}
	VECTOR3* selected_verts=new VECTOR3[total_selected_ver];
	VECTOR4* selected_rgba=new VECTOR4[total_selected_ver];
	int start=0;
	for(int i=0;i<line_num;i++)
	{
		int length=tmp_ver_num[i];
		if(length<=0)
			continue;
		int ori_length;
		VECTOR3* line=get_line_vertices(verts, ver_num, i,ori_length);
		VECTOR4* rgbline=get_line_rgba(rgba, ver_num, i);
		
		for(int j=0;j<length;j++)
		{
			selected_verts[start+j]=line[j];
			selected_rgba[start+j]=rgbline[j];
		}
		start+=length;

	}
	fp=fopen(outputstreamlinesfile,"wb");
	fp=fopen("F:\\xul\\illuminite lines\\examples\\viewer\\data\\velocity.dat","wb");
	
	fwrite(&selected_line_num,sizeof(int),1,fp);
	fwrite(selected_ver_num,sizeof(int),selected_line_num,fp);
	fwrite(selected_verts,sizeof(VECTOR3),total_selected_ver,fp);
	fwrite(rgba,sizeof(VECTOR4),total_selected_ver,fp);
	fclose(fp);

	delete [] entropies;
	delete [] rgba;
	delete [] selected_rgba;
	delete [] selected_verts;
	delete [] tmp_ver_num;
}
*/

void adjustLengthByEntropies(char*  entropyfile,char* streamlinefile,char* outputstreamlinesfile)
{
	int grid_res[3];
	FILE* fp=fopen(entropyfile,"rb");
	fread(grid_res,sizeof(int),3,fp);
	float* entropies=new float[grid_res[0]*grid_res[1]*grid_res[2]];
	fread(entropies,sizeof(float),grid_res[0]*grid_res[1]*grid_res[2],fp);
	fclose(fp);

	int line_num,*ver_num,total_ver_num=0;
	fp=fopen(streamlinefile,"rb");
	fread(&line_num,sizeof(int),1,fp);
	ver_num=new int[line_num];
	fread(ver_num,sizeof(int),line_num,fp);
	for(int i=0;i<line_num;i++)
		total_ver_num+=ver_num[i];
	VECTOR3* verts=new VECTOR3[total_ver_num];
	fread(verts,sizeof(VECTOR3),total_ver_num,fp);
	VECTOR4* rgba=new VECTOR4[total_ver_num];
	fread(rgba,sizeof(VECTOR4),total_ver_num,fp);
	fclose(fp);

	// DEL-BY-LEETEN 01/22/2010 srand((unsigned)time(NULL));

	//start prunning
	for(int i=0;i<line_num;i++)
	{
		int length;
		VECTOR3* line=get_line_vertices(verts, ver_num, i,length);
		VECTOR4* line_rgba=get_line_rgba(rgba, ver_num, i);


		float maxi=0;int ind=0;
		for(int j=0;j<length;j++)
		{
			VECTOR3 p=line[j];
			int idx=(int)p[0]+((int)p[1])*grid_res[0]+((int)p[2])*grid_res[0]*grid_res[1];
			float prob=entropies[idx];
			if(prob>maxi)
			{
				maxi=prob;
				ind=j;
			}
		}
		//start walk
		for(int j=ind;j<length;j++)
		{
			VECTOR3 p=line[j];
			int idx=(int)p[0]+((int)p[1])*grid_res[0]+((int)p[2])*grid_res[0]*grid_res[1];
			float u = ((float)rand()/(float)RAND_MAX);
			float prob=entropies[idx];

			if(u>prob)//non-important regions
			{
				for(int k=j;k<length;k++)
				line_rgba[k][3]=prob*prob*prob;
				break;
			}
			else
				line_rgba[j][3]=prob;


		}
		//start walk
		for(int j=ind;j>=0;j--)
		{
			VECTOR3 p=line[j];
			int idx=(int)p[0]+((int)p[1])*grid_res[0]+((int)p[2])*grid_res[0]*grid_res[1];
			float u = ((float)rand()/(float)RAND_MAX);
			float prob=entropies[idx];
			if(u>prob*2)
			{
				for(int k=j;k>=0;k--)
				line_rgba[k][3]=0.1;
				break;
			}
			else
				line_rgba[j][3]=prob;

		}

	}

	fp=fopen(outputstreamlinesfile,"wb");
	fp=fopen("F:\\xul\\illuminite lines\\examples\\viewer\\data\\velocity.dat","wb");
	
	fwrite(&line_num,sizeof(int),1,fp);
	fwrite(ver_num,sizeof(int),line_num,fp);
	fwrite(verts,sizeof(VECTOR3),total_ver_num,fp);
	fwrite(rgba,sizeof(VECTOR4),total_ver_num,fp);
	fclose(fp);

	delete [] entropies;
	delete [] rgba;

}
void dumpEntropyField(char*  filename,float* importance, int* grid_res)
{
	FILE* fp=fopen(filename,"wb");
	if(fp==0)
	{
		printf("can not open importance file to write \n");
		return ;//return;

	}

	//normalize
	float max_imp=0;
	for(int i=0;i<grid_res[0]*grid_res[1]*grid_res[2];i++)
	{
		if(importance[i]>max_imp)
			max_imp=importance[i];
	}

	if(max_imp>0)
	{
		for(int i=0;i<grid_res[0]*grid_res[1]*grid_res[2];i++)
		{
			importance[i]=importance[i]/max_imp;
			//importance[i]=sqrt(sqrt(sqrt(sqrt(importance[i]))));
		}
	}	

	
	fwrite(grid_res,sizeof(int),3,fp);
	fwrite(importance,sizeof(float),grid_res[0]*grid_res[1]*grid_res[2],fp);
	fclose(fp);

}
void dumpImportanceField(char*  filename,float* importance, int* grid_res)
{
	FILE* fp=fopen(filename,"wb");
	if(fp==0)
	{
		printf("can not open importance file to write \n");
		return ;//return;

	}

	//normalize
	float max_imp=0;
	for(int i=0;i<grid_res[0]*grid_res[1]*grid_res[2];i++)
	{
		if(importance[i]>max_imp)
			max_imp=importance[i];
	}

	if(max_imp>0)
	{
		for(int i=0;i<grid_res[0]*grid_res[1]*grid_res[2];i++)
		{
			importance[i]=importance[i]/max_imp;
			//importance[i]=sqrt(sqrt(sqrt(sqrt(importance[i]))));
		}
	}	

	unsigned char* char_importance= new unsigned char[grid_res[0]*grid_res[1]*grid_res[2]];
	for(int i=0;i<grid_res[0]*grid_res[1]*grid_res[2];i++)
		char_importance[i]=(importance[i])*255;//flip, to use the volume rendering transfer function
	fwrite(grid_res,sizeof(int),3,fp);
	fwrite(char_importance,sizeof(unsigned char),grid_res[0]*grid_res[1]*grid_res[2],fp);
	fclose(fp);

	/*//dump the importance slice by slice
	for(int z=0;z<grid_res[2]-1;z++)
	{
		char filename[255];
		memset(filename,0,255);
		sprintf(filename,"entropy%d.ppm",z);
		save2PPM_1(filename, char_importance+z*grid_res[0]*grid_res[1], grid_res[0],grid_res[1]);
	}*/
	delete [] char_importance;
}
void dumpReconstruedField(char*  filename,float* new_vector, int* grid_res)
{
	FILE* fp=fopen(filename,"wb");
	if(fp==0)
	{
		printf("can not open reconstruct file to write \n");
		return ;//return;
	}
	//grid_res[2]=2;
	fwrite(grid_res,sizeof(int),3,fp);
	fwrite(new_vector,sizeof(float),grid_res[0]*grid_res[1]*grid_res[2]*3,fp);
//	fwrite(new_vector,sizeof(float),grid_res[0]*grid_res[1]*grid_res[2]*3,fp);
	fclose(fp);

	/*
	fp=fopen("debug.txt","w");
	if(fp==0)
	{
		printf("can not open reconstruct file to write \n");
		return ;//return;
	}
	grid_res[2]=2;
	fprintf(fp,"%d %d \n", grid_res[0], grid_res[1]);
	for(int i=0; i<grid_res[0]*grid_res[1];i++)
	{
	fprintf(fp,"%f %f\n",new_vector[i*3+0],new_vector[i*3+1]);
	}
	fclose(fp);
*/
}

void combinehalflines(list<vtListSeedTrace*> lines, list<vtListSeedTrace*>& long_lines)
{
	vtListSeedTrace* newlist;
	list<vtListSeedTrace*>::iterator pIter;
	pIter =lines.begin(); 
	int sou=0;
	for (; pIter!=lines.end(); pIter++) 
	{
		if(sou%2==0)//create new list
			newlist=new vtListSeedTrace();
		vtListSeedTrace *trace = *pIter; 
		std::list<VECTOR3*>::iterator pnIter; 
		pnIter = trace->begin(); 

		for (; pnIter!= trace->end(); pnIter++) 
		{
			VECTOR3 p = **pnIter; 
			if(sou%2==0)
			newlist->push_front(new VECTOR3(p.x(),p.y(),p.z()));
			else
			newlist->push_back(new VECTOR3(p.x(),p.y(),p.z()));
		}
		if(sou%2==1)//create new list
			long_lines.push_back(newlist);
		sou++;
	}
	//clear memory
	pIter =lines.begin(); 
	for (; pIter!=lines.end(); pIter++) 
	{
		vtListSeedTrace *trace = *pIter; 
		std::list<VECTOR3*>::iterator pnIter; 
		pnIter=trace->begin();
	
		for (; pnIter!=trace->end(); pnIter++) 
		delete *pnIter;

		delete  trace;
	}
	
}
void marktriangles(list<vtListSeedTrace*> line, int* footsteps, int* grid_res , int& next_id)
{

	std::list<vtListSeedTrace*>::iterator pIter; 
	pIter = line.begin(); 

	
	for (; pIter!=line.end(); pIter++) 
	{
		vtListSeedTrace *trace = *pIter; 
		std::list<VECTOR3*>::iterator pnIter; 
		pnIter = trace->begin(); 
		for (; pnIter!= trace->end(); pnIter++) 
		{
			VECTOR3 p = **pnIter; 
			footsteps[((int)(p.z()))*grid_res[0]*grid_res[1]+(int)p.y()*grid_res[0]+(int)p.x()]=next_id;
		}	
		next_id++;
	}


}
void createFootstep(list<vtListSeedTrace*> line, int* footsteps, int* grid_res )
{
	std::list<vtListSeedTrace*>::iterator pIter; 
	pIter = line.begin(); 

	int count=0;
	int id=0;
	for (; pIter!=line.end(); pIter++) 
	{
		vtListSeedTrace *trace = *pIter; 
		std::list<VECTOR3*>::iterator pnIter; 
		pnIter = trace->begin(); 
		
		VECTOR3 q = **pnIter; 
		for (; pnIter!= trace->end(); pnIter++) 
		{
			VECTOR3 p = **pnIter; 
			float xlen=fabs(p[0]-q[0]); float ylen=fabs(p[1]-q[1]);
			float maxlen=max(xlen,ylen);
			float dx=xlen/maxlen;
			float dy=ylen/maxlen;

			float x=min(q[0],p[0]); float y=min(q[1],p[1]);
			for(int i = 0; i < maxlen ; i++)
			{
				footsteps[(int)y*grid_res[0]+(int)x]=id;

				x += dx;
				y += dy;
			}
			q.Set(p.x(),p.y(),p.z());

		}	
	id++;
	}
}

void combine(std::list<vtListSeedTrace*> inlist, std::list<vtListSeedTrace*>& outlist)
{
	std::list<vtListSeedTrace*>::iterator pIter; 
	pIter = inlist.begin(); 
	int count=0;
	for (; pIter!=inlist.end(); pIter++) 
	{
		vtListSeedTrace *trace = *pIter; 
		outlist.push_back(trace);
	}
}
void save_streamline_steps(char* filename, int* data, int xdim, int ydim)
{
	FILE* fp = fopen(filename, "w");
	if (fp == NULL)
		return;
	int crop=0;//135;

	fprintf(fp, "P3\n%d %d\n255\n", xdim, ydim-crop*2);

	for (int  i = crop; i <ydim-crop; i++)
	{
		for (int j = 0; j < xdim; j++) 
		{
			int idx = i * xdim + j;

			int r,g,b; 
			if(data[idx]<0)
				r=0;
			else
				r=255;
			g=b=r;
			fprintf(fp, " %d %d %d", r,g,b); 
		}
	}


	fclose(fp);
}

float getAngle(float x, float y)
{

	if((x==0)&&(y==0))
		return 0;
	else
	{
		return (pi+(atan2(y,x)));
	}
}
float getAngle2(float x, float y)
{

	if((x==0)&&(y==0))
		return 0;
	else
	{
		return fabs(pi/2.0-(atan2(y,x)));
	}
}

int getBin(float angle, int binnum)
{
	return (binnum-1)*angle/(2*3.1415926);
}
/*
float calcPoint2PointError( float* vectors,float* new_vectors, int* grid_res, VECTOR3 startpt,VECTOR3 endpt)
{
	int binnum=360;

	float error_sum=0;
	int vol=(endpt.y()-startpt.y())*(endpt.x()-startpt.x())*(endpt.z()-startpt.z());
	for(int z=(int)startpt.z(); z<(int)endpt.z();z++)
	{
		for(int y=(int)startpt.y(); y<(int)endpt.y();y++)
		{
			for(int x=(int)startpt.x(); x<(int)endpt.x();x++)
			{
				VECTOR3 orif,newf;
				int idx=x+y*grid_res[0]+z*grid_res[0]*grid_res[1];
				orif.Set(vectors[idx*3+0],vectors[idx*3+1],vectors[idx*3+2]);
				newf.Set(new_vectors[idx*3+0],new_vectors[idx*3+1],new_vectors[idx*3+2]);
				
				float scalar[2];
				if((orif.x()==0) && (orif.y()==0))
					scalar[0]=0;
				else
					scalar[0]=pi+(atan2(orif.y(),orif.x()));

				if((newf.x()==0)&&(newf.y()==0))
				{
					scalar[1]=0;
//					vac_count++;
				}
				else
					scalar[1]=pi+(atan2(newf.y(),newf.x()));

				int bin_no_ori=(binnum-1)*scalar[0]/(2*3.1415926);
				int bin_no_new=(binnum-1)*scalar[1]/(2*3.1415926);

				//orif.Normalize();//already normalizedd
				//newf.Normalize();

				VECTOR3 error=orif-newf;
				error_sum=error_sum+error.GetMag();
				//float tmp=abs(bin_no_ori-bin_no_new);
				//tmp=tmp/360.0;
				//error_sum=error_sum+tmp;
	
			}
		}
	}
	return error_sum/vol;
}

*/

// ADD-BY-LEETEN 02/02/2010-BEGIN

int 
get_bin_by_angle(float mytheta, float myphi, int binnum, float* theta, float* phi)
{
	for(int i=0; i<binnum;i++)
	{
		if( (mytheta>=theta[i*2+0]) && (mytheta<=theta[i*2+1])&&
			(myphi>=phi[i*2+0]) && (myphi<=phi[i*2+1])
			)
		{
			return i;
		}
	}
	for(int i=0; i<binnum;i++)
	{
		if( ((mytheta+2*pi)>=theta[i*2+0]) && ((mytheta+2*pi)<=theta[i*2+1])&&
			(myphi>=phi[i*2+0]) && (myphi<=phi[i*2+1])
			)
		{
			return i;
		}
	}
	for(int i=0; i<binnum;i++)
	{
		if( ((mytheta)>=theta[i*2+0]) && ((mytheta)<=theta[i*2+1])&&
			((myphi+2*pi)>=phi[i*2+0]) && ((myphi+2*pi)<=phi[i*2+1])
			)
		{
			return i;
		}
	}
	for(int i=0; i<binnum;i++)
	{
		if( ((mytheta+2*pi)>=theta[i*2+0]) && ((mytheta+2*pi)<=theta[i*2+1])&&
			((myphi+2*pi)>=phi[i*2+0]) && ((myphi+2*pi)<=phi[i*2+1])
			)
		{
			return i;
		}
	}
	return -1;
}
// ADD-BY-LEETEN 02/02/2010-END

int get_bin_number_3D(VECTOR3 v, float* theta, float* phi,int binnum)
{
	float mytheta=getAngle(v.x(), v.y());//0~2pi
	float myphi=  getAngle2(sqrt(v.x()*v.x()+v.y()*v.y()), v.z());//0~pi

	if(!theta || !phi)
	{
		printf("read in the regions first\n");
		return -1;
	}
//	mytheta=4.7113109;
//myphi=0.00015759557;
	#if		!BIN_LOOKUP	// ADD-BY-LEETEN 02/02/20101
	int theta_id,phi_id;
	for(int i=0; i<binnum;i++)
	{
		if( (mytheta>=theta[i*2+0]) && (mytheta<=theta[i*2+1])&&
			(myphi>=phi[i*2+0]) && (myphi<=phi[i*2+1])
			)
		{
			return i;
		}
	}
	for(int i=0; i<binnum;i++)
	{
		if( ((mytheta+2*pi)>=theta[i*2+0]) && ((mytheta+2*pi)<=theta[i*2+1])&&
			(myphi>=phi[i*2+0]) && (myphi<=phi[i*2+1])
			)
		{
			return i;
		}
	}
	for(int i=0; i<binnum;i++)
	{
		if( ((mytheta)>=theta[i*2+0]) && ((mytheta)<=theta[i*2+1])&&
			((myphi+2*pi)>=phi[i*2+0]) && ((myphi+2*pi)<=phi[i*2+1])
			)
		{
			return i;
		}
	}
	for(int i=0; i<binnum;i++)
	{
		if( ((mytheta+2*pi)>=theta[i*2+0]) && ((mytheta+2*pi)<=theta[i*2+1])&&
			((myphi+2*pi)>=phi[i*2+0]) && ((myphi+2*pi)<=phi[i*2+1])
			)
		{
			return i;
		}
	}
	return -1;//should not be this, but...
	// ADD-BY-LEETEN 02/02/2010-BEGIN
	#else	// #if	!BIN_LOOKUP	
	static bool bIsAngleMapInitialized = false;
	static const int iNrOfThetas =	720;
	static const int iNrOfPhis =	360;
	static const float fNrOfThetas =	float(iNrOfThetas);
	static const float fNrOfPhis	=	float(iNrOfPhis);
	static int	piAngleMap[iNrOfThetas][iNrOfPhis];
	if( false == bIsAngleMapInitialized )
	{
		for(int t = 0; t < iNrOfThetas; t++)
			for(int p = 0; p < iNrOfPhis; p++)
			{
				float fTheta =	M_PI * 2.0f * float(t) / fNrOfThetas;
				float fPhi =	M_PI * float(p) / fNrOfPhis;
				int iBin = get_bin_by_angle(fTheta, fPhi, binnum, theta, phi);
				if( iBin >= 0 )
					piAngleMap[t][p] = iBin;

			}
		bIsAngleMapInitialized = true;
	}
	int iTheta	=	min(iNrOfThetas - 1,	int(fNrOfThetas	* mytheta	/ (M_PI * 2.0)));
	int iPhi	=	min(iNrOfPhis - 1,		int(fNrOfPhis	* myphi		/ M_PI));
	return piAngleMap[iTheta][iPhi];
	#endif	// #if	!BIN_LOOKUP		
	// ADD-BY-LEETEN 02/02/2010-END
}
float log2(float v)
{
	return log(v)/log(2.0);
}
float calcEntropy_known_bins( int* bins,int* grid_res, VECTOR3 startpt,VECTOR3 endpt,int binnum)
{


	//start calc p(u|v)
	int *histo_pv=new int[binnum];
	memset(histo_pv,0,sizeof(int)*binnum);

	int vol=(endpt.y()-startpt.y())*(endpt.x()-startpt.x())*(endpt.z()-startpt.z());

	for(int z=(int)startpt.z(); z<(int)endpt.z();z++)
	{
		for(int y=(int)startpt.y(); y<(int)endpt.y();y++)
		{
			for(int x=(int)startpt.x(); x<(int)endpt.x();x++)
			{
				VECTOR3 orif;
				int idx=x+y*grid_res[0]+z*grid_res[0]*grid_res[1];
				int bin_no_ori,bin_no_new;
				bin_no_ori=bins[idx];
				histo_pv[bin_no_ori]++;//p(v)
			}
		}
	}
	//calc probs.
	float* pv=new float[binnum];

	float entropy=0;
	for(int i=0; i<binnum; i++)
	{
		pv[i]=(((float)histo_pv[i])/((float)vol));
		entropy=entropy+pv[i];
		if(pv[i]>0)
		entropy-=(pv[i]*log2(pv[i]));
	}

	delete [] pv;
	delete [] histo_pv;
	return entropy;

}
float calcEntropy1( float* vectors,int* grid_res, VECTOR3 startpt,VECTOR3 endpt,float* theta, float* phi,int binnum)
{


	//start calc p(u|v)
	int *histo_pv=new int[binnum];
	memset(histo_pv,0,sizeof(int)*binnum);

	int vol=(endpt.y()-startpt.y())*(endpt.x()-startpt.x())*(endpt.z()-startpt.z());

	for(int z=(int)startpt.z(); z<(int)endpt.z();z++)
	{
		for(int y=(int)startpt.y(); y<(int)endpt.y();y++)
		{
			for(int x=(int)startpt.x(); x<(int)endpt.x();x++)
			{
				VECTOR3 orif;
				int idx=x+y*grid_res[0]+z*grid_res[0]*grid_res[1];
				orif.Set(vectors[idx*3+0],vectors[idx*3+1],vectors[idx*3+2]);

				int bin_no_ori,bin_no_new;
				bin_no_ori=get_bin_number_3D(orif,theta, phi,binnum);

				if(bin_no_ori<0 || bin_no_ori>=binnum)
				{
					printf("sth wrong, bin id=%d  %d\n", bin_no_ori,bin_no_new);
					continue;
				}

				histo_pv[bin_no_ori]++;//p(v)
			}
		}
	}
	//calc probs.
	float* pv=new float[binnum];

	float entropy=0;
	for(int i=0; i<binnum; i++)
	{
		pv[i]=(((float)histo_pv[i])/((float)vol));
		entropy=entropy+pv[i];
		if(pv[i]>0)
		entropy-=(pv[i]*log2(pv[i]));
	}

	delete [] pv;
	delete [] histo_pv;
	return entropy;

}
void UpdateOccupied(list<vtListSeedTrace*> lines, int* occupied,int* grid_res,float radius)
{
	vtListSeedTrace* newlist;
	list<vtListSeedTrace*>::iterator pIter;
	
	//update occupied
	pIter =lines.begin(); 
	for (; pIter!=lines.end(); pIter++) 
	{
		vtListSeedTrace *trace = *pIter; 
		std::list<VECTOR3*>::iterator pnIter; 
		pnIter = trace->begin(); 

		for (; pnIter!= trace->end(); pnIter++) 
		{
			VECTOR3 p = **pnIter; 
			for(int z=-radius;z<=radius;z++)
			for(int y=-radius;y<=radius;y++)
			for(int x=-radius;x<=radius;x++)
			{
				int nx,ny,nz;
				nx=p.x()+x; ny=p.y()+y;	nz=p.z()+z;

				if( nx<0||nx>grid_res[0]-1||
					ny<0||ny>grid_res[1]-1||
					nz<0||nz>grid_res[2]-1)
					continue;
				int idx=nx+ny*grid_res[0]+nz*grid_res[0]*grid_res[1];
				occupied[idx]=1;
			}
		}
	}

	
}

float getImportance(list<vtListSeedTrace*> lines,int* grid_res,float* entropies)
{
	float max_entropy=0;
	vtListSeedTrace* newlist;
	list<vtListSeedTrace*>::iterator pIter;
	
	//update occupied
	pIter =lines.begin(); 
	for (; pIter!=lines.end(); pIter++) 
	{
		vtListSeedTrace *trace = *pIter; 
		std::list<VECTOR3*>::iterator pnIter; 
		pnIter = trace->begin(); 

		for (; pnIter!= trace->end(); pnIter++) 
		{
			VECTOR3 p = **pnIter; 
			int idx=(int)(p.x())+((int)(p.y()))*grid_res[0]+((int)(p.z()))*grid_res[0]*grid_res[1];
			if(max_entropy<entropies[idx])
				max_entropy=entropies[idx];
		}
	}
	return max_entropy;

	
}

float calcRelativeEntropy6( float* vectors,float* new_vectors, int* grid_res, VECTOR3 startpt,VECTOR3 endpt,
						   float* theta, float* phi,
						   int* old_bin, int* new_bin,int* occupied,int binnum,
						   int* histo_puv,int* histo_pv,float* pv,float* entropy_tmp,
						   int* g_histo,int* g_histo_puv)
{
	if(histo_puv==0)
		return 0;

	
	float penalty=0;

	//start calc p(u|v)

	memset(histo_puv,0,sizeof(int)*binnum*binnum);
	memset(histo_pv,0,sizeof(int)*binnum);

	float error_probability=0;
	int vol=(endpt.y()-startpt.y())*(endpt.x()-startpt.x())*(endpt.z()-startpt.z());

	bool dif=false;
	for(int z=(int)startpt.z(); z<(int)endpt.z();z++)
	{
		for(int y=(int)startpt.y(); y<(int)endpt.y();y++)
		{
			for(int x=(int)startpt.x(); x<(int)endpt.x();x++)
			{
				//VECTOR3 orif,newf;
				int idx=x+y*grid_res[0]+z*grid_res[0]*grid_res[1];
			
				int bin_no_ori,bin_no_new;
				bin_no_ori=old_bin[idx];
				bin_no_new=new_bin[idx];
				
			/*	if(bin_no_ori<0 || bin_no_ori>=binnum||
					bin_no_new<0 || bin_no_new>=binnum)
				{
					printf("sth wrong, bin id=%d  %d\n", bin_no_ori,bin_no_new);
					continue;
				}
			*/
				int bin_idx=bin_no_ori+binnum*bin_no_new;
				histo_puv[bin_idx]++;//p(u|v)
				histo_pv[bin_no_new]++;//p(v)
				
			}
		}
	}
	

	//H(x|y=a)

	for(int y=0; y<binnum; y++)
	{
		entropy_tmp[y]=0;

		if(histo_pv[y]==0)
			continue;
		
		for(int x=0; x<binnum; x++)
		{
		
			if(histo_puv[x+binnum*y]==0)
				continue;
			//p(x|y=a)
			float p_tmp=histo_puv[x+binnum*y]/((float)(histo_pv[y]));
			//if(p_tmp>0)
			entropy_tmp[y]-=(p_tmp*log2(p_tmp));
		}
	}
	

	float entropy=0;
	for(int i=0; i<binnum; i++)
	{
		pv[i]=(((float)histo_pv[i])/((float)vol));
		entropy=entropy+pv[i]*entropy_tmp[i];
	}


	return entropy;

}
float calcRelativeEntropy6_by_known_histograms( float* vectors,float* new_vectors, int* grid_res, VECTOR3 startpt,VECTOR3 endpt,
						   float* theta, float* phi,
						   int* old_bin, int* new_bin,int* occupied,int binnum,
						   int* histo_puv,int* histo_pv,float* pv,float* entropy_tmp,
						   int* g_histo,int* g_histo_puv)
{
	if(histo_puv==0)
		return 0;

	
	float penalty=0;
	int vol=(endpt.y()-startpt.y())*(endpt.x()-startpt.x())*(endpt.z()-startpt.z());

	//H(x|y=a)

	for(int y=0; y<binnum; y++)
	{
		entropy_tmp[y]=0;
		if(histo_pv[y]==0)
			continue;
		
		for(int x=0; x<binnum; x++)
		{
		
			if(histo_puv[x+binnum*y]==0)
				continue;
			//p(x|y=a)
			float p_tmp=histo_puv[x+binnum*y]/((float)(histo_pv[y]));
			//if(p_tmp>0)
			entropy_tmp[y]-=(p_tmp*log2(p_tmp));
		}
	}
	

	float entropy=0;
	for(int i=0; i<binnum; i++)
	{
		pv[i]=(((float)histo_pv[i])/((float)vol));
		entropy=entropy+pv[i]*entropy_tmp[i];
	}


	return entropy;

}
float calcRelativeEntropy6_new( float* vectors,float* new_vectors, int* grid_res, VECTOR3 startpt,VECTOR3 endpt,
						   float* theta, float* phi,
						   int* old_bin, int* new_bin,int* occupied,int binnum,
						   int* histo_puv,int* histo_pv,float* pv,float* entropy_tmp,
						   int* g_histo,int* g_histo_puv)
{
	if(histo_puv==0)
		return 0;

	
	float penalty=0;

	//start calc p(u|v)

	memset(histo_puv,0,sizeof(int)*binnum*binnum);
	memset(histo_pv,0,sizeof(int)*binnum);

	float error_probability=0;
	int vol=(endpt.y()-startpt.y())*(endpt.x()-startpt.x())*(endpt.z()-startpt.z());

	bool dif=false;
	for(int z=(int)startpt.z(); z<(int)endpt.z();z++)
	{
		for(int y=(int)startpt.y(); y<(int)endpt.y();y++)
		{
			for(int x=(int)startpt.x(); x<(int)endpt.x();x++)
			{
				//VECTOR3 orif,newf;
				int idx=x+y*grid_res[0]+z*grid_res[0]*grid_res[1];
			
				int bin_no_ori,bin_no_new;
				bin_no_ori=old_bin[idx];
				bin_no_new=new_bin[idx];
				
			/*	if(bin_no_ori<0 || bin_no_ori>=binnum||
					bin_no_new<0 || bin_no_new>=binnum)
				{
					printf("sth wrong, bin id=%d  %d\n", bin_no_ori,bin_no_new);
					continue;
				}
			*/
				int bin_idx=bin_no_ori+binnum*bin_no_new;
				histo_puv[bin_idx]++;//p(u|v)
				histo_pv[bin_no_new]++;//p(v)
				
			}
		}
	}
	

	//H(x|y=a)

	for(int y=0; y<binnum; y++)
	{
		entropy_tmp[y]=0;

		if(histo_pv[y]==0)
			continue;
		
		for(int x=0; x<binnum; x++)
		{
		
			if(histo_puv[x+binnum*y]==0)
				continue;
			//p(x|y=a)
			float p_tmp=histo_puv[x+binnum*y]/((float)(histo_pv[y]));
			//if(p_tmp>0)
			entropy_tmp[y]-=(p_tmp*log2(p_tmp));
		}
	}
	

	float entropy=0;
	for(int i=0; i<binnum; i++)
	{
		pv[i]=(((float)histo_pv[i])/((float)vol));
		entropy=entropy+pv[i]*entropy_tmp[i];
	}


	return entropy;

}


void OCNode::getLeaves(std::vector<VECTOR3>& leaves)
{
	if(LeaveNode==1)
	{
		 VECTOR3 startpt, endpt;
		 startpt.Set(XMin,YMin,ZMin);
		 endpt.Set(XMax, YMax,ZMax);
		 leaves.push_back(startpt);
		 leaves.push_back(endpt);

		return;
	}
	else
	{
		for(int i=0; i<ChildrenNo;i++)
		{
			Children[i]->getLeaves( leaves);

		}
	}
}
		
void OCNode::calcEntropy( float* result, float* vectors,int* grid_res,float* theta,float* phi,int binnum)
{
	if(LeaveNode==1)
	{
		 VECTOR3 startpt, endpt;
		 startpt.Set(XMin,YMin,ZMin);
		 endpt.Set(XMax, YMax,ZMax);
		//write the result
		float entropy=calcEntropy1(  vectors, grid_res,  startpt, endpt, theta,  phi,binnum);
		for(int z=startpt.z(); z<endpt.z();z++)
		{
			for(int y=startpt.y(); y<endpt.y();y++)
			{
				for(int x=startpt.x(); x<endpt.x();x++)
				{
					int idx=x+y*grid_res[0]+z*grid_res[0]*grid_res[1];
					result[idx]=entropy;
				}
			}
		}
		return;
	}
	else
	{
		for(int i=0; i<ChildrenNo;i++)
		{
			Children[i]->calcEntropy( result,vectors, grid_res,
								theta,phi,binnum);

		}
	}
}
void OCNode::get_nodes_bounds(std::vector<VECTOR3>& lower_bound,std::vector<VECTOR3>& higher_bound)
{
	for(int i=0; i<ChildrenNo;i++)
	{
			//(Children[i]->m_entropy
			lower_bound.push_back(VECTOR3(Children[i]->XMin, Children[i]->YMin,Children[i]->ZMin));
			higher_bound.push_back(VECTOR3(Children[i]->XMax, Children[i]->YMax,Children[i]->ZMax));
	}
	for(int i=0; i<ChildrenNo;i++)
	{
		Children[i]->get_nodes_bounds( lower_bound, higher_bound);
	}
}


float OCNode::CalcEntropy_InnerNode()
{

	if(LeaveNode==1)
	{
		return m_entropy;
	}

	if(LeaveNode!=1)
	{
		m_entropy=0;
		for(int i=0; i<ChildrenNo;i++)
		{
			float tmp=Children[i]->CalcEntropy_InnerNode();
			m_entropy+=tmp;
		}
		printf("inner entrpy=%f\n",m_entropy);
		return m_entropy;
	}
}


void OCNode::CalcEntropy_FindMaxEntropyNode(float* vectors,float* new_vectors, int* grid_res, 
											int& min_x, int& max_x, int& min_y, int& max_y, int& min_z, int& max_z,
											 float* theta, float* phi,int* oldbin, int* newbin,int * occupied)
{
	if(LeaveNode==1)
	{
		min_x=XMin; max_x=XMax; min_y=YMin; max_y=YMax;min_z=ZMin; max_z=ZMax;
		return;
	}

	if(LeaveNode!=1)
	{
 
		float maxentropy=0;
		int maxi=0;
		for(int i=0; i<ChildrenNo;i++)
		{
			//(Children[i]->m_entropy
			float entropy=calcRelativeEntropy6( vectors,new_vectors, grid_res, 
				VECTOR3(Children[i]->XMin, Children[i]->YMin,Children[i]->ZMin),
				VECTOR3(Children[i]->XMax, Children[i]->YMax,Children[i]->ZMax), 
				theta, phi,
				oldbin, newbin,occupied,NULL);

			
//			if(entropy<0.00001)
//				entropy=point_error;
			if(occupied)//add density control
			{
				int nonempty=0;
				for(int z=Children[i]->ZMin;z<Children[i]->ZMax;z++)
				for(int y=Children[i]->YMin;y<Children[i]->YMax;y++)
				for(int x=Children[i]->XMin;x<Children[i]->XMax;x++)
					{
						if(occupied[x+y*grid_res[0]+z*grid_res[0]*grid_res[1]]!=0)
							nonempty++;
					}
		
				float pv=((float)nonempty)/((float)((Children[i]->ZMax - Children[i]->ZMin)*(Children[i]->YMax - Children[i]->YMin)*(Children[i]->XMax - Children[i]->XMin)));
			//	printf("density control=%f\n",1-pv);
				entropy=entropy*(1-pv);
			}	
			if(entropy > maxentropy)
			{	
				maxentropy=entropy;
				maxi=i;
			}
		}
		Children[maxi]->CalcEntropy_FindMaxEntropyNode( vectors, new_vectors,  grid_res,
														min_x,  max_x,  min_y,  max_y,min_z,  max_z,
														theta, phi,
														oldbin, newbin,occupied);
	}
}


void OCNode::CalcEntropy_LeafNode(float* vectors,float* new_vectors, int* grid_res, 
											int& min_x, int& max_x, int& min_y, int& max_y, int& min_z, int& max_z,
											 float* theta, float* phi,int* oldbin, int* newbin,int * occupied)
{
	if(LeaveNode==1)
	{
 
		float maxentropy=0;
		int maxi=0;
		float entropy=calcRelativeEntropy6( vectors,new_vectors, grid_res, 
			VECTOR3(XMin, YMin,ZMin),
			VECTOR3(XMax, YMax,ZMax), 
			theta, phi,
			oldbin, newbin,occupied,NULL);

		
		if(occupied)//add density control
		{
			int nonempty=0;
			for(int z=ZMin;z<ZMax;z++)
			for(int y=YMin;y<YMax;y++)
			for(int x=XMin;x<XMax;x++)
				{
					if(occupied[x+y*grid_res[0]+z*grid_res[0]*grid_res[1]]!=0)
						nonempty++;
				}
	
			float pv=((float)nonempty)/((float)((ZMax - ZMin)*(YMax - YMin)*(XMax - XMin)));
			entropy=entropy*(1-pv);
		}	
		m_entropy=entropy;
		
	}
	else
	{
		for(int i=0; i<ChildrenNo;i++)
		Children[i]->CalcEntropy_LeafNode( vectors, new_vectors,  grid_res,
														min_x,  max_x,  min_y,  max_y,min_z,  max_z,
														theta, phi,
														oldbin, newbin,occupied);
	}
}

void OCNode::FindMaxEntropyNode(int& min_x, int& max_x, int& min_y, int& max_y, int& min_z, int& max_z)
{
	if(LeaveNode==1)
	{
		min_x=XMin; max_x=XMax; min_y=YMin; max_y=YMax;
		min_z=ZMin; max_z=ZMax;
		return;
	}

	float maxentropy=0;
	int maxi=0;
	for(int i=0; i<ChildrenNo;i++)
	{
		if(Children[i]->m_entropy > maxentropy)
		{		maxentropy=Children[i]->m_entropy;
			maxi=i;
		}
	}
	Children[maxi]->FindMaxEntropyNode( min_x,  max_x,  min_y,  max_y,  min_z,  max_z);

}
//Function for the BonoNode class
//Assign the children of the BonoNode
void OCNode::Build(int xdim, int ydim,int zdim,int MinLen)
{
	int XLen = XMax-XMin+1;
	int YLen = YMax-YMin+1;
	int ZLen = ZMax-ZMin+1;
	
	//Already reach the finest level
	if (XLen <= MinLen && YLen <= MinLen && ZLen <= MinLen)
		return;

	//The node is not a leave node
	LeaveNode = 0;

	// 3 2 
	// 0 1
	//Build the children of the node

	//Child 000
	Children[0] = new OCNode   (XMin, (XMin + XMax)/2,
								YMin, (YMin + YMax)/2,
								ZMin, (ZMin + ZMax)/2);
	Children[0]->Build(xdim, ydim, zdim, MinLen);

	//Child 010
	Children[1] = new OCNode ((XMin + XMax)/2, XMax,
								YMin, (YMin + YMax)/2,
								ZMin, (ZMin + ZMax)/2);
	Children[1]->Build(xdim, ydim, zdim, MinLen);

	//Child 100
	Children[2] = new OCNode ((XMin + XMax)/2, XMax,
								(YMin + YMax)/2, YMax,
								ZMin, (ZMin + ZMax)/2);
	Children[2]->Build(xdim, ydim,zdim,  MinLen);

	//Child 110
	Children[3] = new OCNode (XMin, (XMin + XMax)/2,
								(YMin + YMax)/2, YMax,
								ZMin, (ZMin + ZMax)/2);
	Children[3]->Build(xdim, ydim,zdim,  MinLen);

	//Child 001
	Children[4] = new OCNode   (XMin, (XMin + XMax)/2,
								YMin, (YMin + YMax)/2,
								(ZMin + ZMax)/2, ZMax);
	Children[4]->Build(xdim, ydim, zdim, MinLen);

	//Child 011
	Children[5] = new OCNode ((XMin + XMax)/2, XMax,
								YMin, (YMin + YMax)/2,
								(ZMin + ZMax)/2, ZMax);
	Children[5]->Build(xdim, ydim, zdim, MinLen);

	//Child 101
	Children[6] = new OCNode ((XMin + XMax)/2, XMax,
								(YMin + YMax)/2, YMax,
								(ZMin + ZMax)/2, ZMax);
	Children[6]->Build(xdim, ydim,zdim,  MinLen);

	//Child 111
	Children[7] = new OCNode (XMin, (XMin + XMax)/2,
								(YMin + YMax)/2, YMax,
								(ZMin + ZMax)/2, ZMax);
	Children[7]->Build(xdim, ydim,zdim,  MinLen);
}


//Functions for the BonoTree class
//Build the Bono tree
void OCTree::Build(int MinLen)
{
	Root = new OCNode(0, CellXDim-1, 0, CellYDim-1, 0, CellZDim-1);
	Root->Build(CellXDim, CellYDim, CellZDim, MinLen);

}
bool isZeroVec(float* tmp_new_vectors, int idx)
{

	if(	(tmp_new_vectors[idx*3+0]==0)&&
						(tmp_new_vectors[idx*3+1]==0)&&
						(tmp_new_vectors[idx*3+2]==0))
	{
		return true;
	}
	else
		return false;
		
}
/*
//faster, diffuse from the streamline points. 
void reconstruct_field_GVF_3D(float* new_vectors,float* vectors, int* grid_res,list<vtListSeedTrace*> l_list,int* donot_change, 
							  float* kx, float* ky, float* kz,
							  float* b, float* c1, float* c2, float* c3,
							  float* importance)
{
	int * occupied=new int[grid_res[0]*grid_res[1]*grid_res[2]];
	memset(occupied,0,sizeof(int)*grid_res[0]*grid_res[1]*grid_res[2]);


	list<vtListSeedTrace*>::iterator pIter;
	std::queue<VECTOR3> myqueue;
	pIter = l_list.begin();
	for (; pIter!=l_list.end(); pIter++) 
	{

		vtListSeedTrace *trace = *pIter; 
		std::list<VECTOR3*>::iterator pnIter; 
		int count=0;
		VECTOR3 q;
		pnIter = trace->begin(); 
		for (; pnIter!= trace->end(); pnIter++) 
		{

			VECTOR3* p=*(pnIter);
			

			std::vector<VECTOR3> tmplist;
			tmplist.clear();
			if(!count)//first point
			{
				myqueue.push(*p);
				tmplist.push_back(*p);
			}
			else//not the first, push the line segment in
			{
				float step_num=(sqrt(	(p->x()-q.x())*(p->x()-q.x())+
										(p->y()-q.y())*(p->y()-q.y())+
										(p->z()-q.z())*(p->z()-q.z())
									));
				if(step_num>1)
				{
					float step_size_x,step_size_y,step_size_z;
					step_size_x=(q.x()-p->x())/step_num;
					step_size_y=(q.y()-p->y())/step_num;
					step_size_z=(q.z()-p->z())/step_num;
					
					for(int i=0; i<step_num; i++)
					{
						VECTOR3 midp;
						midp.Set(	p->x()+i*step_size_x,
									p->y()+i*step_size_y,
									p->z()+i*step_size_z);
						myqueue.push(midp);
						tmplist.push_back(midp);

					}
				}
				else
				{
						myqueue.push(*p);
						tmplist.push_back(*p);
				}

			}
			q=*p;//save the end points

			for(int j=0;j<tmplist.size();j++)
			{
				int idx=(int)tmplist[j].x()+((int)tmplist[j].y())*grid_res[0]+((int)tmplist[j].z())*grid_res[0]*grid_res[1];

				float vx=vectors[idx*3+0];
				float vy=vectors[idx*3+1];
				float vz=vectors[idx*3+2];
				//normalize
				float len=sqrt(vx*vx+vy*vy+vz*vz);
				if(len>0)
				{
					vx=vx/len;	vy=vy/len; vz=vz/len;
				}
	//			if(len==0)
	//				printf("found!\n");
				//rotate by 90 degree
				//calc kx and ky
				kx[idx]=vx;
				ky[idx]=vy;
				kz[idx]=vz;
				//initial GVF  to kx ky
				if(donot_change[idx]==0)
				{
					new_vectors[idx*3+0]=kx[idx];
					new_vectors[idx*3+1]=ky[idx];
					new_vectors[idx*3+2]=kz[idx];

					donot_change[idx]=1;
					occupied[idx]=1;
					
					//importance[idx]=1;

				}
			}

			count++;
		}
	}

	//other initials
	for(int z=0; z<grid_res[2];z++)
	{
		for(int y=0; y<grid_res[1];y++)
		{
			for(int x=0; x<grid_res[0];x++)
			{
				int idx=x+y*grid_res[0]+z*grid_res[0]*grid_res[1];
				b[idx]=kx[idx]*kx[idx]+ky[idx]*ky[idx]+kz[idx]*kz[idx];
				c1[idx]=b[idx]*kx[idx];
				c2[idx]=b[idx]*ky[idx];
				c3[idx]=b[idx]*kz[idx];
//				c4[idx]=b[idx]*1;


			}
		}
	}
	//update the GVF
	float* tmp_new_vectors=new float[grid_res[0]*grid_res[1]*grid_res[2]*3];
	for(int i=0; i<grid_res[0]*grid_res[1]*grid_res[2]*3;i++)
		tmp_new_vectors[i]=new_vectors[i];

	
	//parameter setting:
	float mu=0.1;
	//iteration by iteration
	while(myqueue.size())
	{
		//printf("myqueue size=%d\n", myqueue.size());
		VECTOR3	np=myqueue.front(); 
		myqueue.pop();
		int x=np.x();	int y=np.y(); int z=np.z();

		if(x<0||y<0||z<0||x>grid_res[0]-1||y>grid_res[1]-1||z>grid_res[2]-1)//hit boundary
		continue;

		int idx=x+y*grid_res[0]+z*grid_res[0]*grid_res[1]; 

		int idx_1,idx_2,idx_3,idx_4,idx_5,idx_6;
		
		idx_1=idx_2=idx_3=idx_4=idx_5=idx_6=x+y*grid_res[0]+z*grid_res[0]*grid_res[1];

		if(!(x+1>grid_res[0]-1))
			idx_1=x+1+y*grid_res[0]+z*grid_res[0]*grid_res[1]; 
		if(!(y+1>grid_res[1]-1))
			idx_2=x+(y+1)*grid_res[0]+z*grid_res[0]*grid_res[1]; 
		if(!(z+1>grid_res[2]-1))
			idx_3=x+y*grid_res[0]+(z+1)*grid_res[0]*grid_res[1]; 
		if(!(x-1<0))
			idx_4=x-1+y*grid_res[0]+z*grid_res[0]*grid_res[1]; 
		if(!(y-1<0))
			idx_5=x+(y-1)*grid_res[0]+z*grid_res[0]*grid_res[1]; 
		if(!(z-1<0))
			idx_6=x+y*grid_res[0]+(z-1)*grid_res[0]*grid_res[1]; 

//		if(x-1<0||y-1<0||z-1<0||x+1>grid_res[0]-1||y+1>grid_res[1]-1||z+1>grid_res[2]-1)//hit boundary
//		cointinue;

		
		float fx =	(1-b[idx])*tmp_new_vectors[idx*3+0]+
			mu*(tmp_new_vectors[idx_1*3+0]+tmp_new_vectors[idx_2*3+0]+tmp_new_vectors[idx_3*3+0]+tmp_new_vectors[idx_4*3+0]+tmp_new_vectors[idx_5*3+0]+tmp_new_vectors[idx_6*3+0]
		-6*tmp_new_vectors[idx*3+0])+
			c1[idx];

		float fy =	(1-b[idx])*tmp_new_vectors[idx*3+1]+
			mu*(tmp_new_vectors[idx_1*3+1]+tmp_new_vectors[idx_2*3+1]+tmp_new_vectors[idx_3*3+1]+tmp_new_vectors[idx_4*3+1]+tmp_new_vectors[idx_5*3+1]+tmp_new_vectors[idx_6*3+1]
		-6*tmp_new_vectors[idx*3+1])+
			c2[idx];

		float fz =	(1-b[idx])*tmp_new_vectors[idx*3+2]+
			mu*(tmp_new_vectors[idx_1*3+2]+tmp_new_vectors[idx_2*3+2]+tmp_new_vectors[idx_3*3+2]+tmp_new_vectors[idx_4*3+2]+tmp_new_vectors[idx_5*3+2]+tmp_new_vectors[idx_6*3+2]
		-6*tmp_new_vectors[idx*3+2])+
			c3[idx];



		tmp_new_vectors[idx*3+0]=fx;//saddle 0.1
		tmp_new_vectors[idx*3+1]=fy;
		tmp_new_vectors[idx*3+2]=fz;

	
		if(occupied[idx_1]==0)
			myqueue.push(VECTOR3(x+1,y,z));
		if(occupied[idx_2]==0)
			myqueue.push(VECTOR3(x,y+1,z));
		if(occupied[idx_3]==0)
			myqueue.push(VECTOR3(x,y,z+1));
		if(occupied[idx_4]==0)
			myqueue.push(VECTOR3(x-1,y,z));
		if(occupied[idx_5]==0)
			myqueue.push(VECTOR3(x,y-1,z));
		if(occupied[idx_6]==0)
			myqueue.push(VECTOR3(x,y,z-1));


		occupied[idx_1]=1;
		occupied[idx_2]=1;
		occupied[idx_3]=1;
		occupied[idx_4]=1;
		occupied[idx_5]=1;
		occupied[idx_6]=1;


	}
	int zerocount=0;
	int notocu=0;


	//update the GVF
	for(int z=0; z<grid_res[2];z++)
	{
		for(int y=0; y<grid_res[1];y++)
		{
			for(int x=0; x<grid_res[0];x++)
			{
				int idx=x+y*grid_res[0]+z*grid_res[0]*grid_res[1]; 
				if(donot_change[idx]==0)
				{
					//normalize
					VECTOR3 tmp;
					tmp.Set(tmp_new_vectors[idx*3+0],
							tmp_new_vectors[idx*3+1],
							tmp_new_vectors[idx*3+2]
					);
					//tmp.Normalize();
					new_vectors[idx*3+0]=tmp.x();
					new_vectors[idx*3+1]=tmp.y();
					new_vectors[idx*3+2]=tmp.z();
					if(	(tmp_new_vectors[idx*3+0]==0)&&
						(tmp_new_vectors[idx*3+1]==0)&&
						(tmp_new_vectors[idx*3+2]==0))
						zerocount++;

					if(occupied[idx]==0)
						notocu++;

				
				}

				

			}
		}
	}



//printf("\n ** zero count=%d **\n",zerocount);
//printf("\n ** empty cells=%d **\n",notocu);
	delete [] tmp_new_vectors;
//	delete [] tmp_importance;
	delete [] occupied;

}

*/

// ADD-BY-LEETEN 02/02/2010-BEGIN

float* tmp_new_vectors;

void 
quit_reconstruct_field_GVF_3D()
{
	FREE(tmp_new_vectors);
}

// ADD-BY-LEETEN 02/02/2010-END

void reconstruct_field_GVF_3D(float* new_vectors,float* vectors, int* grid_res,list<vtListSeedTrace*> l_list,int* donot_change, 
							  float* kx, float* ky, float* kz,
							  float* b, float* c1, float* c2, float* c3,
							  float* importance)
{
	// ADD-BY-LEETEN 02/02/2010-BEGIN
	#if	0	// DEL-BY-LEETEN 02/04/2010-BEGIN
		static bool bIsInitialized = false;
		if( false == bIsInitialized )
		{
			atexit(quit_reconstruct_field_GVF_3D);
			bIsInitialized = true;
		}
	#endif	// DEL-BY-LEETEN 02/04/2010-END
	// ADD-BY-LEETEN 02/02/2010-END

	// ADD-BY-LEETEN 02/04/2010-BEGIN
	CLOCK_INIT(CLOCK_reconstruct_field_GVF_3D, __FUNCTION__ ":");
	CLOCK_BEGIN(CLOCK_reconstruct_field_GVF_3D);
	// ADD-BY-LEETEN 02/04/2010-END

	// MOD-BY-LEETEN 02/06/2010-FROM:
		// int iter=max(grid_res[0],max(grid_res[1],grid_res[2]))*2;
	// TO:
	int iter=max(grid_res[0],max(grid_res[1],grid_res[2])) * 3 * 3;
	// MOD-BY-LEETEN 02/06/2010-END

	//initial GVF, combine with input new_vectors
	list<vtListSeedTrace*>::iterator pIter;

	
	pIter = l_list.begin();
	for (; pIter!=l_list.end(); pIter++) 
	{

		vtListSeedTrace *trace = *pIter; 
		std::list<VECTOR3*>::iterator pnIter; 
		pnIter = trace->begin(); 
		for (; pnIter!= trace->end(); pnIter++) 
		{

			VECTOR3* p=*(pnIter);
			int idx=(int)p->x()+((int)p->y())*grid_res[0]+((int)p->z())*grid_res[0]*grid_res[1];
			float vx=vectors[idx*3+0];
			float vy=vectors[idx*3+1];
			float vz=vectors[idx*3+2];
			//normalize
			float len=sqrt(vx*vx+vy*vy+vz*vz);
			if(len>0)
			{
				vx=vx/len;	vy=vy/len; vz=vz/len;
			}
			//rotate by 90 degree
			//calc kx and ky
			kx[idx]=vx;
			ky[idx]=vy;
			kz[idx]=vz;
			//initial GVF  to kx ky
			// DEL-BY-LEETEN 01/22/2010-BEGIN
				// if(donot_change[idx]==0)
			// DEL-BY-LEETEN 01/22/2010-END
			{
			new_vectors[idx*3+0]=kx[idx];
			new_vectors[idx*3+1]=ky[idx];
			new_vectors[idx*3+2]=kz[idx];
			// DEL-BY-LEETEN 02/02/2010			donot_change[idx]=1;
			}
		}
	}

	//other initials
	for(int z=0; z<grid_res[2];z++)
	{
		for(int y=0; y<grid_res[1];y++)
		{
			for(int x=0; x<grid_res[0];x++)
			{
				int idx=x+y*grid_res[0]+z*grid_res[0]*grid_res[1];
				b[idx]=kx[idx]*kx[idx]+ky[idx]*ky[idx]+kz[idx]*kz[idx];
				c1[idx]=b[idx]*kx[idx];
				c2[idx]=b[idx]*ky[idx];
				c3[idx]=b[idx]*kz[idx];
		

			}
		}
	}
	//update the GVF
	//	MOD-BY-LEETEN 02/02/2010-FROM:
		// float* tmp_new_vectors=new float[grid_res[0]*grid_res[1]*grid_res[2]*3];
	// TO:
	MALLOC(tmp_new_vectors, float, grid_res[0]*grid_res[1]*grid_res[2]*3);
	// MOD-BY-LEETEN 02/02/2010-END
	#if	0	// MOD-BY-LEETEN 02/06/2010-FROM:
		for(int i=0; i<grid_res[0]*grid_res[1]*grid_res[2]*3;i++)
			tmp_new_vectors[i]=new_vectors[i];
	#else	// MOD-BY-LEETEN 02/06/2010-TO:
	memset(tmp_new_vectors, 0 ,sizeof(tmp_new_vectors[0])*grid_res[0]*grid_res[1]*grid_res[2]*3);
	#endif	// MOD-BY-LEETEN 02/06/2010-END

	//parameter setting:
	float mu=0.1;

	// ADD-BY-LEETEN 02/04/2010-BEGIN
	CLOCK_END(CLOCK_reconstruct_field_GVF_3D, false);
	CLOCK_BEGIN(CLOCK_reconstruct_field_GVF_3D);
	// ADD-BY-LEETEN 02/04/2010-END

	// ADD-BY-LEETEN 2009/11/23-BEGIN
	// MOD-BY-LEETEN 02/02/2010-FROM:
		// #if	USE_CUDA	
	// TO:
	#if	DIFFUSION_ON_CUDA
	// MOD-BY-LEETEN 02/02/2010-END
	#if	0	// MOD-BY-LEETEN 02/02/2010-FROM:
		_FlowFusion(
			mu, 
			iter, 
			grid_res[0], grid_res[1], grid_res[2],
			16,					// iBlockZSize
			b,					// float *pfWeightVolume,
			c1,					// float *pfOffsetVolume,
			c2,					// 
			c3,					//
			tmp_new_vectors,	// float *pfSrcVolume,
			new_vectors,		// float *pfDstVolume
			donot_change		
		);
	#else	// MOD-BY-LEETEN 02/02/2010-TO:

	_FlowDiffusion(
		mu, 
		iter, 
		grid_res[0], grid_res[1], grid_res[2],
		16,					// iBlockZSize
		b,					// float *pfWeightVolume,
		c1,					// float *pfOffsetVolume,
		c2,					// 
		c3,					//
		tmp_new_vectors,	// float *pfSrcVolume,
		new_vectors,		// float *pfDstVolume
		donot_change		
	);
	#endif	// MOD-BY-LEETEN 02/02/2010-END

	// ADD-BY-LEETEN 02/04/2010-BEGIN
	#elif DIFFUSION_ON_GLSL
	_FlowDiffusionGLGL(
		mu, 
		iter, 
		b,					// float *pfWeightVolume,
		c1,					// float *pfOffsetVolume,
		c2,					// 
		c3,					//
		tmp_new_vectors,	// float *pfSrcVolume
		new_vectors			// float *pfDstVolume
	);

	// ADD-BY-LEETEN 02/04/2010-END

	#else	// #if	DIFFUSION_ON_CUDA
	// ADD-BY-LEETEN 2009/11/23-END

	//iteration by iteration
	for(int i=0; i<iter; i++)
	{
		//start iterations
		//update the GVF
//double dwStart = GetTickCount();

		#if	0	// MOD-BY-LEETEN 02/02/2010-FROM:
			for(int z=1; z<grid_res[2]-1;z++)
			{
				for(int y=1; y<grid_res[1]-1;y++)
				{
					for(int x=1; x<grid_res[0]-1;x++)//cut boundary for now
					{
						int idx=x+y*grid_res[0]+z*grid_res[0]*grid_res[1]; 
						int idx_1=x+1+y*grid_res[0]+z*grid_res[0]*grid_res[1]; 
						int idx_2=x+(y+1)*grid_res[0]+z*grid_res[0]*grid_res[1]; 
						int idx_3=x+y*grid_res[0]+(z+1)*grid_res[0]*grid_res[1]; 
						int idx_4=x-1+y*grid_res[0]+z*grid_res[0]*grid_res[1]; 
						int idx_5=x+(y-1)*grid_res[0]+z*grid_res[0]*grid_res[1]; 
						int idx_6=x+y*grid_res[0]+(z-1)*grid_res[0]*grid_res[1]; 
		#else	// MOD-BY-LEETEN 02/02/2010-TO:		
		for(int z=0; z<grid_res[2];z++)
		{
			for(int y=0; y<grid_res[1];y++)
			{
				for(int x=0; x<grid_res[0];x++)//cut boundary for now
				{
					int idx=x+y*grid_res[0]+z*grid_res[0]*grid_res[1]; 
					int idx_1=min(x+1, grid_res[0]-1)+y*grid_res[0]+z*grid_res[0]*grid_res[1]; 
					int idx_2=x+min(y+1, grid_res[1]-1)*grid_res[0]+z*grid_res[0]*grid_res[1]; 
					int idx_3=x+y*grid_res[0]+min(z+1,grid_res[2]-1)*grid_res[0]*grid_res[1]; 
					int idx_4=max(x-1, 0)+y*grid_res[0]+z*grid_res[0]*grid_res[1]; 
					int idx_5=x+max(y-1, 0)*grid_res[0]+z*grid_res[0]*grid_res[1]; 
					int idx_6=x+y*grid_res[0]+max(z-1, 0)*grid_res[0]*grid_res[1]; 
		#endif	// MOD-BY-LEETEN 02/02/2010-END

					#if	0	// MOD-BY-LEETEN 02/08/2010-FROM:
						tmp_new_vectors[idx*3+0]=	(1-b[idx])*new_vectors[idx*3+0]+
													mu*(new_vectors[idx_1*3+0]+new_vectors[idx_2*3+0]+new_vectors[idx_3*3+0]+new_vectors[idx_4*3+0]+new_vectors[idx_5*3+0]+new_vectors[idx_6*3+0]
														-6*new_vectors[idx*3+0])+
													c1[idx];

						tmp_new_vectors[idx*3+1]=	(1-b[idx])*new_vectors[idx*3+1]+
													mu*(new_vectors[idx_1*3+1]+new_vectors[idx_2*3+1]+new_vectors[idx_3*3+1]+new_vectors[idx_4*3+1]+new_vectors[idx_5*3+1]+new_vectors[idx_6*3+1]
														-6*new_vectors[idx*3+1])+
													c2[idx];

						tmp_new_vectors[idx*3+2]=	(1-b[idx])*new_vectors[idx*3+2]+
													mu*(new_vectors[idx_1*3+2]+new_vectors[idx_2*3+2]+new_vectors[idx_3*3+2]+new_vectors[idx_4*3+2]+new_vectors[idx_5*3+2]+new_vectors[idx_6*3+2]
														-6*new_vectors[idx*3+2])+
													c3[idx];
					#else	// MOD-BY-LEETEN 02/08/2010-TO:
					for(int j = 0; j < 3; j++)
						new_vectors[idx*3+j]=	(1-b[idx])*tmp_new_vectors[idx*3+j]+
							mu*(tmp_new_vectors[idx_1*3+j]+
								tmp_new_vectors[idx_2*3+j]+
								tmp_new_vectors[idx_3*3+j]+
								tmp_new_vectors[idx_4*3+j]+
								tmp_new_vectors[idx_5*3+j]+
								tmp_new_vectors[idx_6*3+j]-6*tmp_new_vectors[idx*3+j]);
					
					new_vectors[idx*3+0] += c1[idx];
					new_vectors[idx*3+1] += c2[idx];
					new_vectors[idx*3+2] += c3[idx];
					#endif	// MOD-BY-LEETEN 02/08/2010-END
				}
			}
		}
//double elapsedTime= GetTickCount() - dwStart;
//printf("\n\n reconstruction time is %.3f milli-seconds.\n",elapsedTime); 	
		
		#if	0	// MOD-BY-LEETEN 02/08/2010-FROM:
			int zerocount=0;
			//update the GVF
			
			for(int z=0; z<grid_res[2];z++)
			{
				for(int y=0; y<grid_res[1];y++)
				{
					for(int x=0; x<grid_res[0];x++)
					{
						int idx=x+y*grid_res[0]+z*grid_res[0]*grid_res[1]; 
						// DEL-BY-LEETEN 01/22/2010-BEGIN
							// if(donot_change[idx]==0)
						// DEL-BY-LEETEN 01/22/2010-END
						{
						new_vectors[idx*3+0]=tmp_new_vectors[idx*3+0];
						new_vectors[idx*3+1]=tmp_new_vectors[idx*3+1];
						new_vectors[idx*3+2]=tmp_new_vectors[idx*3+2];
						}
					}
				}
			}
			
			//memcpy(new_vectors,tmp_new_vectors,sizeof(float)*grid_res[0]*grid_res[1]*grid_res[2]*3);
		#else	// MOD-BY-LEETEN 02/08/2010-TO:
		static double dPrevError;
		double dError = 0.0;
		for(int		idx = 0,	z=0; z<grid_res[2];z++)
			for(int				y=0; y<grid_res[1];y++)
				for(int			x=0; x<grid_res[0];x++, idx++)
					for(int j = 0; j < 3; j++)
					{
						double dDiff = double(new_vectors[idx*3+j] - tmp_new_vectors[idx*3+j]);
						dError += dDiff * dDiff;
						tmp_new_vectors[idx*3+j] = new_vectors[idx*3+j];
					}

		dError = sqrt(dError / (3.0 * grid_res[0] * grid_res[1] * grid_res[2]));
		// ADD-BY-LEETEN 02/16/2010-BEGIN
		printf("Error = %e", dError);
		if( 0 == i )
			printf("\n");
		else
			printf("\r");
		// ADD-BY-LEETEN 02/16/2010-END

		if( i > max(max(grid_res[0], grid_res[1]), grid_res[2]) )
		{
			double dErrorRate = dError / dPrevError;
			if( DIFFUSION_CONVERGE_THRESHOLD < dErrorRate && dErrorRate <= 1.0 )
			{
				printf("Diffusion takes %d iterations to converge. ", i);
				break;
			}
		}
		// ADD-BY-LEETEN 02/16/2010-BEGIN
		else if( i > 0 )
		{
			double dErrorRate = dError / dPrevError;
			if( dErrorRate > 1.0 )
			{
				printf("Warning: Diffusion error increases. Stop. " );
				break;
			}
		}
		// ADD-BY-LEETEN 02/16/2010-END

		dPrevError = dError;
		#endif	// MOD-BY-LEETEN 02/08/2010-END
	}
	// ADD-BY-LEETEN 02/16/2010-BEGIN
	printf("\n");
	// ADD-BY-LEETEN 02/16/2010-END

	// ADD-BY-LEETEN 02/08/2010-BEGIN
	memcpy(new_vectors, tmp_new_vectors, sizeof(tmp_new_vectors[0]) * 3 * grid_res[0] * grid_res[1] * grid_res[2]);
	// ADD-BY-LEETEN 02/08/2010-END

	// ADD-BY-LEETEN 2009/11/23-BEGIN
	#endif	// DIFFUSION_ON_CUDA

	// ADD-BY-LEETEN 2009/11/23-END
	// ADD-BY-LEETEN 02/04/2010-BEGIN
	CLOCK_END(CLOCK_reconstruct_field_GVF_3D, false);
	CLOCK_BEGIN(CLOCK_reconstruct_field_GVF_3D);
	// ADD-BY-LEETEN 02/04/2010-END

	// DEL-BY-LEETEN 02/02/2010-BEGIN
		// delete [] tmp_new_vectors;
	// ADD-BY-LEETEN 02/02/2010-END
	// ADD-BY-LEETEN 02/04/2010-BEGIN
	quit_reconstruct_field_GVF_3D();
	// ADD-BY-LEETEN 02/04/2010-END

	// ADD-BY-LEETEN 02/04/2010-BEGIN
	CLOCK_END(CLOCK_reconstruct_field_GVF_3D, false);
	CLOCK_PRINT(CLOCK_reconstruct_field_GVF_3D);
	// ADD-BY-LEETEN 02/04/2010-END
}

/*

$Log: not supported by cvs2svn $
Revision 1.4  2010/02/09 00:48:38  leeten

[02/08/2010]
1. [MOD] Change #iterations for CPU-based diffusion.
2. [MOD] Reset the vector field tmp_new_vectors to 0 before diffusion.
3. [MOD] Save the latest diffused vector field to new_vectors.
4. [MOD] Stop diffusion when MSE is converged.

Revision 1.3  2010/02/05 01:31:23  leeten

[02/02/2010]
1. [ADD] Add macros to print timing for the function reconstruct_reconstruct_field_GVF_3D.
2. [MOD] Change the preprocessors BIN_LOOKUP_ON_GPU, DIFFUSION_ON_GPU, ENTROPY_ON_GPU to BIN_LOOKUP_ON_CUDA, DIFFUSION_ON_CUDA, ENTROPY_ON_CUDA, respectively.
3. [ADD] Call the function quit_reconstruct_reconstruct_field_GVF_3D at the end of the function reconstruct_reconstruct_field_GVF_3D.

Revision 1.2  2010/02/02 23:47:07  leeten

[02/02/2010]
1. [ADD] Add the header liblog.h and FlowDiffusion3DConfig.h.
2. [MOD] Set the pointer "dummy" to NULL since it won't be used.
3. [MOD] Use preprocessor KERNEL_HALF_WIDTH, KERNEL_HALF_HEIGHT and KERNEL_HALF_DEPTH to control the kernel size.
4. [ADD] Define a function get_bin_by_angle to convert the 3D coordinate from Cartesian to spherical one.
5. [MOD] If the preprocessor BIN_LOOKUP is not zero, the bin is obtained via a lookup table.
6. [ADD] Define a function quit_reconstruct_field_GVF_3D to free the memory used in the function reconstruct_field_GVF_3D.
7. [MOD] All arrays used in the function reconstruct_field_GVF_3D are done by MALLOC.
8. [MOD] Apply diffusion to voxels along the boundary too.

Revision 1.1  2010/01/22 20:53:36  leeten

[01/22/2010]
1. [1ST] First time checkin.


*/
