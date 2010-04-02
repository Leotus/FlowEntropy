////////////////////////////////////////////////////////
//
// 3D sample program
//
// Han-Wei Shen
//
////////////////////////////////////////////////////////
#include "entropy.h"

#include <stdio.h>
#include <windows.h>

#include <stdlib.h>
#include <math.h>

#if	0	// DEL-BY-LEETEN 01/22/2010-BEGIN
	#include <GLUT/glut.h> 
	#include <OpenGL/gl.h>
#endif	// DEL-BY-LEETEN 01/22/2010-END
#include <GL/glut.h>

#include "OSUFlow.h"

#include <list>
#include <iterator>

// ADD-BY-LEETEN 02/05/2010-BEGIN
#include <liblog.h>

#include "FlowDiffusion2DConfig.h"
// ADD-BY-LEETEN 02/05/2010-END


QuadTree* Tree=0;
int g_xdim=0;
int g_ydim=0;
char* g_filename;
int winWidth=500;
int winHeight=500;
int press_x, press_y; 
int release_x, release_y; 
float x_angle = 0.0; 
float y_angle = 0.0; 
// MOD-BY-LEETEN 03/19/2010-FROM:
	// float scale_size = 1; 
// TO:
float scale_size = 5; 
// MOD-BY-LEETEN 03/19/2010-END
float* entropies=0;

int xform_mode = 0; 

#define XFORM_NONE    0 
#define XFORM_ROTATE  1
#define XFORM_SCALE 2 

OSUFlow *osuflow; 
VECTOR3 minLen, maxLen; 
list<vtListSeedTrace*> sl_list; 
list<vtListSeedTrace*> diffuse_list; 
std::vector<VECTOR3> triangles;
std::vector<VECTOR3> import_pos;

GLuint	triangle_list;

bool toggle_draw_streamlines = false; 
bool toggle_animate_streamlines = false; 
float center[3], len[3]; 
int first_frame = 1; 

//draw grid
int dim[3];
float * grid_fx, *grid_fy, *grid_fz;

bool inside(int x,int y,int z,int xdim,int ydim)
{
	if(x<0 || x>xdim-1 || y<0 || y>ydim-1)
		return false;
	else
		return true;
}
typedef std::vector<int> vtListVertex;
int sign(float x)
{
	if (x>0)
		return 1;
	else if(x<0)
		return -1;
	else
		return 0;
}

void ConstructGraph_static_2D(char* filename, std::vector<vtListVertex*>& Graph)
{
	int mag_cof=100;
	int element_size=3;
	FILE* fp;
	int xdim,ydim,zdim;
	float* buf0,*buf;
	//construct graph
	fp=fopen(filename,"rb");
	fread(&xdim,sizeof(int),1,fp);
	fread(&ydim,sizeof(int),1,fp);
	fread(&zdim,sizeof(int),1,fp);


	buf=new float[xdim*ydim*zdim*element_size];
	int num;
	num=fread(buf,sizeof(float),xdim*ydim*zdim*element_size,fp);
	fclose(fp);
//	for(int z=0;z<block_dim[2]; z++)
	{
		for(int y=0;y<ydim; y++)
		{
			for(int x=0; x<xdim; x++)
			{
				vtListVertex* edges_of_a_vertex = new vtListVertex;
				Graph.push_back(edges_of_a_vertex);
			}

		}
	}

	//for(int z=0;z<block_dim[2]; z++)
	int z=0;
	{
		for(int y=0;y<ydim; y++)
		{
			for(int x=0; x<xdim; x++)
			{
				int idx=x+y*xdim;

				float u=buf[idx*element_size+0]*mag_cof;
				float v=buf[idx*element_size+1]*mag_cof;
				float w=buf[idx*element_size+2]*mag_cof;

				int idx2;
				int left, right, top,bottom, front,back;
				float uu,vv,ww;
				
				//left face
				if(inside(x-1,y,z,xdim,ydim))
				{
					left=x-1+y*xdim;
					uu=buf[left*element_size+0]*mag_cof;

					int weight=0;
					if(sign(u)<0)
						weight=abs((int)u);
					if(sign(uu)>0)
						weight+=abs((int)uu);
					{
						Graph[idx]->push_back(left);
						Graph[idx]->push_back(weight);

					}
					
				}
				//right face
				if(inside(x+1,y,z,xdim,ydim))
				{
					right=x+1+y*xdim;
					uu=buf[right*element_size+0]*mag_cof;

					int weight=0;
					if(sign(u)>0)
						weight=abs((int)u);
					if(sign(uu)<0)
						weight+=abs((int)uu);
					{
						Graph[idx]->push_back(right);
						Graph[idx]->push_back(weight);
					}
					
				}
				//top face
				if(inside(x,y-1,z,xdim,ydim))
				{
					top=x+(y-1)*xdim;
					vv=buf[top*element_size+1]*mag_cof;

					int weight=0;
					if(sign(v)<0)
						weight=abs((int)v);
					if(sign(vv)>0)
						weight+=abs((int)vv);
					//if(weight!=0)
					{					
						Graph[idx]->push_back(top);
						Graph[idx]->push_back(weight);
					}
					
				}
				//bottom face
				if(inside(x,y+1,z,xdim,ydim))
				{
					bottom=x+(y+1)*xdim;
					vv=buf[bottom*element_size+1]*mag_cof;

					int weight=0;
					if(sign(v)>0)
						weight=abs((int)v);
					if(sign(vv)<0)
						weight+=abs((int)vv);
					//if(weight!=0)
					{
						Graph[idx]->push_back(bottom);
						Graph[idx]->push_back(abs(weight));
					}
					
				}

			}
		}
	}

	delete [] buf;

}
////////////////////////////////////////////////////////
void draw_cube(float r, float g, float b)
{
	glColor3f(r, g, b); 
	glutWireCube(1.0);   // draw a solid cube 
}

//random lines
void compute_streamlines_random() {

	float from[3], to[3]; 

	from[0] = minLen[0];   from[1] = minLen[1];   from[2] = minLen[2]; 
	to[0] = maxLen[0];   to[1] = maxLen[1];   to[2] = maxLen[2]; 

	printf("generating seeds...\n"); 
	osuflow->SetRandomSeedPoints(from, to, 100); 
	int nSeeds; 
	VECTOR3* seeds = osuflow->GetSeeds(nSeeds); 
	dumpSeeds(seeds,nSeeds, osuflow,"myseeds.seed");//crtical points excluded


	for (int i=0; i<nSeeds; i++) 
		printf(" seed no. %d : [%f %f %f]\n", i, seeds[i][0], 
		seeds[i][1], seeds[i][2]); 

	sl_list.clear(); 

	printf("compute streamlines..\n"); 
	osuflow->SetIntegrationParams(.2, 1); //small and large step size
	osuflow->GenStreamLines(sl_list , BACKWARD_AND_FORWARD, 300, 0); //maxi steps
	printf(" done integrations\n"); 
	printf("list size = %d\n", sl_list.size()); 


	char filename[255];
	memset(filename,0,sizeof(char)*255);
	sprintf(filename,strcat(g_filename,".seed"));
	VECTOR3* cpyseeds=new VECTOR3[nSeeds];
	memcpy(cpyseeds,seeds,sizeof(VECTOR3)*nSeeds);
	//	dumpSeedandLines(cpyseeds,nSeeds, osuflow,  filename);//crtical points excluded

	//	dumpSeeds(cpyseeds,nSeeds, osuflow,filename);//crtical points excluded
	delete [] cpyseeds;
}
float* get_grid_vec_data(int* grid_res)//get vec data at each grid point
{
	osuflow->GetFlowField()->getDimension(grid_res[0],grid_res[1],grid_res[2]);
	float * vectors=new float[grid_res[0]*grid_res[1]*grid_res[2]*3];
	for(int k=0; k<grid_res[2];k++)
	{
		for(int j=0; j<grid_res[1];j++)
		{
			for(int i=0; i<grid_res[0];i++)
			{
				VECTOR3 data;
				osuflow->GetFlowField()->at_vert(i,j,k,0,data);
				int idx=i+j*grid_res[0]+k*grid_res[0]*grid_res[1];
				vectors[idx*3+0]=data.x();
				vectors[idx*3+1]=data.y();
				vectors[idx*3+2]=data.z();
			}
		}
	}
	return vectors;
}

void dumpEntropy(std::vector<float>entropies,char* filename)
{
	FILE* fp=fopen(filename,"wb");
	int size=entropies.size();
	fwrite(&size, sizeof(int),1,fp);
	for(int i=0;i<size;i++)
		fwrite(&(entropies[i]),sizeof(float),1,fp);
	fclose(fp);
}

void draw_streamlines_ppm(unsigned char* data,int* grid_res,list<vtListSeedTrace*> line_list)
{
	std::list<vtListSeedTrace*>::iterator pIter; 
	pIter = line_list.begin(); 
	for (; pIter!=line_list.end(); pIter++) 
	{
		vtListSeedTrace *trace = *pIter; 
		
		if(trace->size()<2)
			continue;
		std::list<VECTOR3*>::iterator pnIter; 
		pnIter = trace->begin(); 

		VECTOR3 p,q;
		p = **pnIter;
		pnIter++;
		for (; pnIter!= trace->end(); pnIter++) 
		{
			q = **pnIter; 
			int idx_p=(int)p.x()+grid_res[0]*((int)p.y());
			idx_p=max(0,idx_p);
			idx_p=min(grid_res[0]*grid_res[1]-1, idx_p);

			if(p.x()==q.x() && p.y()==q.y())
				data[idx_p]=255;
			else
			{
				float len=max(fabs(p.x()-q.x()),fabs(p.y()-q.y()));
				
				float xstep=(q.x()-p.x())/len;
				float ystep=(q.y()-p.y())/len;
				for(int i=0;i<len;i++)
				{
					VECTOR3 t;
					t.Set(q.x()+i*xstep,q.y()+i*ystep,0);
					int idx_q=(int)t.x()+grid_res[0]*((int)t.y());
					idx_q=max(0,idx_q);
					idx_q=min(grid_res[0]*grid_res[1]-1, idx_q);

					data[idx_q]=255;

				}
			}
			p=q;
		}
	}
}

void getbins(float* vecs,int* bins, int* grid_res,int binnum)
{
	float pi=3.1415926;
	for(int i=0;i<grid_res[0]*grid_res[1];i++)
	{
		VECTOR3 orif;
			
		orif.Set(vecs[i*3+0],vecs[i*3+1],0);
		float scalar;
		if((orif.x()==0) && (orif.y()==0))
			scalar=0;
		else
			scalar=pi+(atan2(orif.y(),orif.x()));

		bins[i]=(binnum-1)*scalar/(2*3.1415926);
	}
}
void compute_entropy_point_by_point_load_file() 
{

	printf("generating seeds...\n"); 
	int num=0;
	int grid_res[3];
	float* vectors=get_grid_vec_data(grid_res);//get vec data at each grid point
	grid_res[2]=1;


	printf("loading seeds...\n"); 
	int nSeeds; 
	//VECTOR3* seeds=loadSeedandRender(strcat(g_filename,".data"),nSeeds);
	VECTOR3* seeds=loadSeedfromFile("myseeds.seed",nSeeds);

	//osuflow->SetEntropySeedPoints( seeds,nSeeds);
	//seeds = osuflow->GetSeeds(nSeeds); 

	float* new_vectors=new float[grid_res[0]*grid_res[1]*grid_res[2]*3];
	memset(new_vectors,0,sizeof(float)*grid_res[0]*grid_res[1]*grid_res[2]*3);
	
//set the boundary
/*	int x=0;
	int y=0;
	for(y=0;y<grid_res[1];y++)
	{
		int idx=x+y*grid_res[0];
		new_vectors[3*idx+0]=vectors[3*idx+0];
		new_vectors[3*idx+1]=vectors[3*idx+1];
	}
	x=grid_res[0]-1;
	for(y=0;y<grid_res[1];y++)
	{
		int idx=x+y*grid_res[0];
		new_vectors[3*idx+0]=vectors[3*idx+0];
		new_vectors[3*idx+1]=vectors[3*idx+1];
	}
	y=0;
	for(x=0;x<grid_res[0];x++)
	{
		int idx=x+y*grid_res[0];
		new_vectors[3*idx+0]=vectors[3*idx+0];
		new_vectors[3*idx+1]=vectors[3*idx+1];
	}
	y=grid_res[1]-1;
	for(x=0;x<grid_res[0];x++)
	{
		int idx=x+y*grid_res[0];
		new_vectors[3*idx+0]=vectors[3*idx+0];
		new_vectors[3*idx+1]=vectors[3*idx+1];
	}
	*/
	sl_list.clear(); 
	printf("integrating streamlines...\n"); 

	std::vector<float> entropies;
	int* donotchange=new int[grid_res[0]*grid_res[1]];
	memset(donotchange,0,grid_res[0]*grid_res[1]);
	for (int i=0; i<nSeeds; i++) 
	{
		printf(" seed no. %d : [%f %f %f]\n", i, seeds[i][0], 
		seeds[i][1], seeds[i][2]); 

		VECTOR3 newseed;
		newseed.Set(seeds[i][0], seeds[i][1], seeds[i][2]); 
							//combine the half lines

		list<vtListSeedTrace*> lines,new_lines; 
		//printf("compute streamlines..\n"); 
		osuflow->SetEntropySeedPoints( &newseed,1);
		osuflow->SetIntegrationParams(1, 5);							//small and large step size
		osuflow->GenStreamLines(lines , BACKWARD_AND_FORWARD, 500, 0);	 //maxi steps
										//clear memory
		if(lines.size())
		sl_list.push_back(*(lines.begin()));
		if(lines.size()==2)
		sl_list.push_back(*(++lines.begin()));
	}

	printf("reconstructing...\n"); 

	double dwStart = GetTickCount();
	//2. update streamline foot step
	reconstruct_field_GVF_2D(new_vectors,vectors,grid_res,sl_list,donotchange);
	double elapsedTime= GetTickCount() - dwStart;
	printf("\n\n reconstruction time is %.3f milli-seconds.\n",elapsedTime); 	

	float* img_entropies=new float[grid_res[0]*grid_res[1]];
	
	dwStart = GetTickCount();
	float maximum=0;
	int* bin_vector, *bin_newvectors;
	bin_vector=new int[grid_res[0]*grid_res[1]];
	bin_newvectors=new int[grid_res[0]*grid_res[1]];

	// MOD-BY-LEETEN 03/15/2010-FROM:
		// int	binnum=60;
	// TO:
	int binnum = NR_OF_BINS;
	// MOD-BY-LEETEN 03/15/2010-END
	printf("calculate entropy...\n"); 

	getbins(vectors,bin_vector,  grid_res, binnum);
	getbins(new_vectors,bin_newvectors,  grid_res, binnum);

	// MOD-BY-LEETEN 03/15/2010-FROM:
		// int  kernel_size= 8;//conditional entropy kernel
	// TO:
	int  kernel_size = KERNEL_SIZE;//conditional entropy kernel
	// MOD-BY-LEETEN 03/15/2010-END

	for(int y=0;y<grid_res[1];y++)
	{
		for(int x=0;x<grid_res[0];x++)
		{
			VECTOR3 start,end;
			start.Set(max(0,x-kernel_size),max(0,y-kernel_size),0);
			end.Set(min(x+kernel_size,grid_res[0]-1),min(y+kernel_size,grid_res[1]-1),0);


			//float entropy=calcRelativeEntropy6_load_bins(bin_vector, bin_newvectors,
			//	 grid_res,  start, end);
			float entropy=calcEntropy( vectors, grid_res, start,end);

			//float entropy=calcRelativeEntropy6( vectors,new_vectors, grid_res, 
			//		start,end).x(),start.y(),start.z()),
			//		VECTOR3(end.x(),end.y(),end.z()),0);

			int idx=x+y*grid_res[0];
			img_entropies[idx]=entropy;
			if(entropy>maximum)
				maximum=entropy;
		}
		// MOD-BY-LEETEN 01/22/2010-FROM:
			// printf("y=%d/%d\n",y,grid_res[1]-1);
		// TO:
		// MOD-BY-LEETEN 03/28/2010-FROM:
			// printf("y=%d/%d\r",y,grid_res[1]-1);
		// TO:
		fprintf(stderr, "y=%d/%d\r",y,grid_res[1]-1);
		// MOD-BY-LEETEN 03/28/2010-END
		// MOD-BY-LEETEN 01/22/2010-END
	}
	// ADD-BY-LEETEN 01/22/2010-BEGIN
	printf("\n");
	// ADD-BY-LEETEN 01/22/2010-END
	delete [] bin_newvectors;
	delete [] bin_vector;

	elapsedTime= GetTickCount() - dwStart;
	printf("\n\n entorpy for each point time is %.3f milli-seconds.\n",elapsedTime); 	

	//save to image
	unsigned char* data;
	data = new unsigned char[grid_res[0]*grid_res[1]];
	for(int i=0;i<grid_res[0]*grid_res[1];i++)
	{	
		//img_entropies[i]=img_entropies[i];///maximum;
		data[i]=(unsigned char)(img_entropies[i]/maximum*255);
		
		//printf("data=%.2f ",img_entropies[i]);

	}
	
	save2PPM_3("entropy_image.ppm", data, grid_res[0],grid_res[1]);
	//save2file("entropy.data", data, grid_res[0],grid_res[1]);

	//memset(data,0,sizeof(unsigned char)*grid_res[0]*grid_res[1]);
	draw_streamlines_ppm(data,grid_res,sl_list);
	save2PPM_3("entropy_image_line.ppm", data, grid_res[0],grid_res[1]);

	unsigned char* new_data=new unsigned char[grid_res[0]*grid_res[1]*3];

	std::vector<VECTOR3> dump_seeds;
	for(int y=0;y<grid_res[1];y++)
		for(int x=0;x<grid_res[0];x++)
		{
			int kernel_size=1;
			int idx1=x+y*grid_res[0];
			float me=img_entropies[idx1];
			bool found=false;
			for(int ny=-kernel_size;ny<=kernel_size;ny++)
			{
				for(int nx=-kernel_size;nx<=kernel_size;nx++)
				{
					int newx=x+nx; int newy=y+ny;
					if(newx<0||newx>grid_res[0]-1||newy<0||newy>grid_res[1]-1)
						continue;
					int idx2=newx+newy*grid_res[0];
					if(nx==0 && ny==0)
						continue;
					float you=img_entropies[idx2];
					if(you>=me)
					{
						found=true;
						break;
					}
				}
				if(found==true)
					break;
			}
			if( (found==false)&&(img_entropies[idx1]>.5))//mark this point
			{
				dump_seeds.push_back(VECTOR3(x,y,0));
				new_data[idx1*3+0]=255;
				new_data[idx1*3+1]=0;
				new_data[idx1*3+2]=0;
			}
			else
			{
				new_data[idx1*3+0]=data[idx1];
				new_data[idx1*3+1]=data[idx1];
				new_data[idx1*3+2]=data[idx1];

			}
		}
	save2PPM_3_channels("local_max.ppm", new_data, grid_res[0],grid_res[1]);
	dumpReconstruedField("r.vec", new_vectors, grid_res);

	//dump seeds
	int snum=dump_seeds.size();
	FILE* fp=fopen("dump_local_max.seed","w");
	fprintf(fp,"%d\n",snum);
	for(int i=0;i<dump_seeds.size();i++)
	{
		fprintf(fp,"%.2f %.2f %.2f\n",dump_seeds[i].x(),dump_seeds[i].y(),0.0);
	}
	fclose(fp);

	delete [] new_data;
	delete [] data;
	delete [] img_entropies;
	delete [] new_vectors;
	delete [] donotchange;
}


//load seeds
void compute_streamlines_load_file_entropy() 
{

	printf("generating seeds...\n"); 
	int num=0;
	int grid_res[3];
	float* vectors=get_grid_vec_data(grid_res);//get vec data at each grid point
	grid_res[2]=1;


	printf("loading seeds...\n"); 
	int nSeeds; 
	//VECTOR3* seeds=loadSeedandRender(strcat(g_filename,".data"),nSeeds);
	VECTOR3* seeds=loadSeedfromFile("myseeds.seed",nSeeds);

	//osuflow->SetEntropySeedPoints( seeds,nSeeds);
	//seeds = osuflow->GetSeeds(nSeeds); 

	float* new_vectors=new float[grid_res[0]*grid_res[1]*grid_res[2]*3];
	memset(new_vectors,0,sizeof(float)*grid_res[0]*grid_res[1]*grid_res[2]*3);
	
//set the boundary
	int x=0;
	int y=0;
	for(y=0;y<grid_res[1];y++)
	{
		int idx=x+y*grid_res[0];
		new_vectors[3*idx+0]=vectors[3*idx+0];
		new_vectors[3*idx+1]=vectors[3*idx+1];
	}
	x=grid_res[0]-1;
	for(y=0;y<grid_res[1];y++)
	{
		int idx=x+y*grid_res[0];
		new_vectors[3*idx+0]=vectors[3*idx+0];
		new_vectors[3*idx+1]=vectors[3*idx+1];
	}
	y=0;
	for(x=0;x<grid_res[0];x++)
	{
		int idx=x+y*grid_res[0];
		new_vectors[3*idx+0]=vectors[3*idx+0];
		new_vectors[3*idx+1]=vectors[3*idx+1];
	}
	y=grid_res[1]-1;
	for(x=0;x<grid_res[0];x++)
	{
		int idx=x+y*grid_res[0];
		new_vectors[3*idx+0]=vectors[3*idx+0];
		new_vectors[3*idx+1]=vectors[3*idx+1];
	}
	sl_list.clear(); 
	std::vector<float> entropies;
	int* donotchange=new int[grid_res[0]*grid_res[1]];
	memset(donotchange,0,grid_res[0]*grid_res[1]);
	for (int i=0; i<nSeeds; i++) 
	{
		printf(" seed no. %d : [%f %f %f]\n", i, seeds[i][0], 
		seeds[i][1], seeds[i][2]); 

												//load seeds from file

		//set first seed as domain center
		VECTOR3 newseed;
		newseed.Set(seeds[i][0], seeds[i][1], seeds[i][2]); 
							//combine the half lines
	

		list<vtListSeedTrace*> lines,new_lines; 
		//printf("compute streamlines..\n"); 
		osuflow->SetEntropySeedPoints( &newseed,1);
		osuflow->SetIntegrationParams(.1, .5);							//small and large step size
		osuflow->GenStreamLines(lines , BACKWARD_AND_FORWARD, 500, 0);	 //maxi steps
										//clear memory
		if(lines.size())
		sl_list.push_back(*(lines.begin()));
		if(lines.size()==2)
		sl_list.push_back(*(++lines.begin()));
		

		//2. update streamline foot step
		reconstruct_field_GVF_2D(new_vectors,vectors,grid_res,sl_list,donotchange);
		
		float entropy=calcRelativeEntropy6(  vectors, new_vectors,  grid_res, VECTOR3(0,0,0),VECTOR3(grid_res[0],grid_res[1],0),0);
		entropies.push_back(entropy);

	}
	

	dumpEntropy(entropies,"entropy.bin");
	dumpReconstruedField("r.vec", new_vectors, grid_res);
//	dumpSeeds(seedlist,"myseeds.seed");//crtical points excluded
	delete [] new_vectors;
	delete [] donotchange;
}

#if	0	// MOD-BY-LEETEN 02/06/2010-FROM:
void save_streamlines_to_file()
{
#else	// MOD-BY-LEETEN 02/06/2010-TO:
void save_streamlines_to_file(list<vtListSeedTrace*>& sl_list, char *szVectorFilename = NULL)
{
#endif	// MOD-BY-LEETEN 02/06/2010-END
	int line_num=sl_list.size();
	int* ver_num=new int[line_num];

	//get ver num for each line
	int line_id=0;
	int total_ver_num=0;
	std::list<vtListSeedTrace*>::iterator pIter; 
	pIter = sl_list.begin(); 
	for (; pIter!=sl_list.end(); pIter++) {
		vtListSeedTrace *trace = *pIter; 
		ver_num[line_id]=trace->size();
		total_ver_num=total_ver_num+trace->size();
		line_id++;
	}
	//write the vertices
	VECTOR3* ver=new VECTOR3[total_ver_num];
	int grid_res[3];
	osuflow->GetFlowField()->getDimension(grid_res[0],grid_res[1],grid_res[2]);;
	
	int count=0;
	pIter = sl_list.begin(); 
	for (; pIter!=sl_list.end(); pIter++) {
		vtListSeedTrace *trace = *pIter; 
		std::list<VECTOR3*>::iterator pnIter; 
		pnIter = trace->begin(); 

		VECTOR3 q;
		for (; pnIter!= trace->end(); pnIter++) {
			VECTOR3 p = **pnIter; 
			int x,y,idx;
			x=p[0];y=p[1];
			idx=x+y*grid_res[0];

			ver[count].Set(p[0], p[1], entropies[idx]);//p[2]); 
			count++;
		}
	}

	// MOD-BY-LEETEN 02/05/2010-FROM:
		// FILE* fp=fopen("streamlines.dat","wb");
	// TO:
	static char szFilename[1024+1];
	if( szVectorFilename )
		strcpy(szFilename, szVectorFilename);
	else
		sprintf(szFilename, "%s_streamlines.dat", g_filename);
	FILE* fp=fopen(szFilename,"wb");
	// ADD-BY-LEETEN 02/05/2010-END

	fwrite(&line_num,sizeof(int),1,fp);
	fwrite(ver_num,sizeof(int),line_num,fp);
	fwrite(ver,sizeof(VECTOR3),total_ver_num,fp);
	fclose(fp);
	delete [] ver;
	delete [] ver_num;
}



void compute_streamlines_load_file() {

	float from[3], to[3]; 

	from[0] = minLen[0];   from[1] = minLen[1];   from[2] = minLen[2]; 
	to[0] = maxLen[0];   to[1] = maxLen[1];   to[2] = maxLen[2]; 

	printf("loading seeds...\n"); 
	int nSeeds; 
	//VECTOR3* seeds=loadSeedandRender(strcat(g_filename,".data"),nSeeds);
	VECTOR3* seeds=loadSeedfromFile("myseeds.seed",nSeeds);

	osuflow->SetEntropySeedPoints( seeds,nSeeds);
	seeds = osuflow->GetSeeds(nSeeds); 


	for (int i=0; i<nSeeds; i++) 
		printf(" seed no. %d : [%f %f %f]\n", i, seeds[i][0], 
		seeds[i][1], seeds[i][2]); 

	sl_list.clear(); 

	printf("compute streamlines..\n"); 
	osuflow->SetIntegrationParams(.1,.5); //small and large step size
	osuflow->GenStreamLines(sl_list , BACKWARD_AND_FORWARD, 500, 0); //maxi steps
	// MOD-BY-LEETEN 02/06/2010-FROM:
		// save_streamlines_to_file();
	// TO:
	save_streamlines_to_file(sl_list);
	// MOD-BY-LEETEN 02/06/2010-END
/*
	int grid_res[3];
	float* vectors=get_grid_vec_data(grid_res);
	int* donotchange=new int[grid_res[0]*grid_res[1]];
	memset(donotchange,0,sizeof(int)*grid_res[0]*grid_res[1]);
	float* new_vectors=new float[grid_res[0]*grid_res[1]*grid_res[2]*3];
	memset(new_vectors,0,sizeof(float)*grid_res[0]*grid_res[1]*grid_res[2]*3);
	
	//reconstruct_field_GVF_2D_gaussian(new_vectors,vectors,grid_res,sl_list,donotchange);
	reconstruct_field_GVF_2D(new_vectors,vectors,grid_res,sl_list,donotchange);
	for(int i=0;i<grid_res[0]*grid_res[1]*grid_res[2];i++)
	{
		VECTOR3 ori,now,tmp;
		now.Set(new_vectors[i*3+0],new_vectors[i*3+1],0);
		now.Normalize();
		new_vectors[i*3+0]=now.x();
		new_vectors[i*3+1]=now.y();
	}

	dumpReconstruedField("r.vec", new_vectors, grid_res);
*
	delete [] vectors;
	delete [] new_vectors;
	delete [] donotchange;
*/	printf(" done integrations\n"); 
	printf("list size = %d\n", sl_list.size()); 
}
//entropy based
/*
void compute_streamlines() {

	float from[3], to[3]; 


	from[0] = minLen[0];   from[1] = minLen[1];   from[2] = minLen[2]; 
	to[0] = maxLen[0];   to[1] = maxLen[1];   to[2] = maxLen[2]; 

	printf("generating seeds...\n"); 
	int num=0;
	int grid_res[3];
	float* vectors;
	osuflow->GetFlowField()->getDimension(grid_res[0],grid_res[1],grid_res[2]);
	vectors=new float[grid_res[0]*grid_res[1]*3];
	int k=0;
	for(int j=0; j<grid_res[1];j++)
	{
		for(int i=0; i<grid_res[0];i++)
		{
			VECTOR3 data;
			osuflow->GetFlowField()->at_vert(i,j,k,0,data);
			int idx=i+j*grid_res[0];
			vectors[idx*3+0]=data.x();
			vectors[idx*3+1]=data.y();
			vectors[idx*3+2]=data.z();
		}
	}


	//1. drop dense streamlines
	printf("loading seeds...\n"); 
	int nSeeds; 
	//VECTOR3* seeds=loadSeedandRender(strcat(g_filename,".data"),nSeeds);
	VECTOR3* seeds=loadSeedfromFile(strcat(g_filename,".seed"),nSeeds);

	osuflow->SetEntropySeedPoints( seeds,nSeeds);
	seeds = osuflow->GetSeeds(nSeeds); 


	for (int i=0; i<nSeeds; i++) 
		printf(" seed no. %d : [%f %f %f]\n", i, seeds[i][0], 
		seeds[i][1], seeds[i][2]); 

	sl_list.clear(); 
	list<vtListSeedTrace*> lines; 
	printf("compute streamlines..\n"); 
	osuflow->SetIntegrationParams(1, 5); //small and large step size
	osuflow->GenStreamLines(lines , BACKWARD_AND_FORWARD, 500, 0); //maxi steps
	printf(" done integrations\n"); 
	printf("list size = %d\n", lines.size()); 

	//combine the half lines
	combinehalflines(lines, sl_list);

	//add bounds 
	//push the boundaries  in the boundinglist

	//mark bound foot step
	int * footsteps=new int[grid_res[0]*grid_res[1]];
	memset(footsteps,-1,grid_res[0]*grid_res[1]);


	//mark boundaries 
	int count=sl_list.size();

	vtListSeedTrace* b0=new vtListSeedTrace;
	vtListSeedTrace* b1=new vtListSeedTrace;
	vtListSeedTrace* b2=new vtListSeedTrace;
	vtListSeedTrace* b3=new vtListSeedTrace;

	b0->push_back(new VECTOR3(0,0,0));
	b0->push_back(new VECTOR3(grid_res[0]-1,0,0));

	b1->push_back(new VECTOR3(grid_res[0]-1,0,0));
	b1->push_back(new VECTOR3(grid_res[0]-1,grid_res[1]-1,0));

	b2->push_back(new VECTOR3(grid_res[0]-1,grid_res[1]-1,0));
	b2->push_back(new VECTOR3(0,grid_res[1]-1,0));

	b3->push_back(new VECTOR3(0,grid_res[1]-1,0));
	b3->push_back(new VECTOR3(0,0,0));

	sl_list.push_back(b0);
	sl_list.push_back(b1);
	sl_list.push_back(b2);
	sl_list.push_back(b3);
	//2. streamline foot step
	marktriangles(sl_list,footsteps ,grid_res);
	int x,y;
	y=0;
	for(x=0; x<grid_res[0];x++)
	{
		int idx=x+y*grid_res[0];
		footsteps[idx]=count;
	}
	count++;

	y=grid_res[1]-1;
	for(x=0; x<grid_res[0];x++)
	{
		int idx=x+y*grid_res[0];
		footsteps[idx]=count;
	}
	count++;
	x=0;
	for(y=0; y<grid_res[1];y++)
	{
		int idx=x+y*grid_res[0];
		footsteps[idx]=count;
	}
	count++;
	x=grid_res[0]-1;
	for(y=0; y<grid_res[1];y++)
	{
		int idx=x+y*grid_res[0];
		footsteps[idx]=count;
	}
	float* new_vectors=new float[grid_res[0]*grid_res[1]*3];
	memset(new_vectors,0,sizeof(float)*grid_res[0]*grid_res[1]*3);

	Graph graph;
	reconstruct_field(new_vectors,vectors,grid_res,sl_list,footsteps,graph);

	dumpReconstruedField("reconstructed.vec", new_vectors, grid_res);
	printf("*** reconstructed field saved*** \n");
	float entropy;
	entropy=calcRelativeEntropy6(  vectors,new_vectors, grid_res);
	printf("entropy=%f\n", entropy);

	printf("press key to continue\n");
	getchar();
	//selectively display streamlines
}
*/


void initial_boundaries(list<vtListSeedTrace*>& lines, int* footsteps, int* grid_res, int id_in, int& id_out)
{
	vtListSeedTrace* b0=new vtListSeedTrace;						//push boundaries
	vtListSeedTrace* b1=new vtListSeedTrace;
	vtListSeedTrace* b2=new vtListSeedTrace;
	vtListSeedTrace* b3=new vtListSeedTrace;

	b0->push_back(new VECTOR3(0,0,0));
	b0->push_back(new VECTOR3(grid_res[0]-1,0,0));

	b1->push_back(new VECTOR3(grid_res[0]-1,0,0));
	b1->push_back(new VECTOR3(grid_res[0]-1,grid_res[1]-1,0));

	b2->push_back(new VECTOR3(grid_res[0]-1,grid_res[1]-1,0));
	b2->push_back(new VECTOR3(0,grid_res[1]-1,0));

	b3->push_back(new VECTOR3(0,grid_res[1]-1,0));
	b3->push_back(new VECTOR3(0,0,0));

	lines.push_back(b0);
	lines.push_back(b1);
	lines.push_back(b2);
	lines.push_back(b3);
	int x,y;
	y=0;
	int count=id_in;
	for(x=0; x<grid_res[0];x++)
	{
		int idx=x+y*grid_res[0];
		footsteps[idx]=count;
	}
	count++;

	y=grid_res[1]-1;
	for(x=0; x<grid_res[0];x++)
	{
		int idx=x+y*grid_res[0];
		footsteps[idx]=count;
	}
	count++;
	x=0;
	for(y=0; y<grid_res[1];y++)
	{
		int idx=x+y*grid_res[0];
		footsteps[idx]=count;
	}
	count++;
	x=grid_res[0]-1;
	for(y=0; y<grid_res[1];y++)
	{
		int idx=x+y*grid_res[0];
		footsteps[idx]=count;
	}
	count++;
	id_out=count;
}

vtListSeedTrace* getSelectedLine(list<vtListSeedTrace*>sl_list, int line_id)
{
	list<vtListSeedTrace*>::iterator pIter = sl_list.begin(); 
	int count=0;
	for (; pIter!=sl_list.end(); pIter++) 
	{
		vtListSeedTrace *trace = *pIter; 
		if(count==line_id)
			return trace;
		count++;
	}
}

void QuadNode::drawself()
{
	if(LeaveNode)//leaf node
	{
		glLineWidth(1);
		glColor4f(0.8f, 0.8f, 0.8f, 0.0f);	         

		glBegin(GL_LINES);
		
		int pBBoxMin[3], pBBoxMax[3];
		pBBoxMin[0]=XMin; pBBoxMin[1]=YMin; pBBoxMin[2]=0;
		pBBoxMax[0]=XMax; pBBoxMax[1]=YMax; pBBoxMax[2]=0;

		glVertex3f(XMin, YMin, 0);
		glVertex3f(XMax, YMin, 0);

		glVertex3f(XMax, YMin, 0);
		glVertex3f(XMax, YMax, 0);

		glVertex3f(XMax, YMax, 0);
		glVertex3f(XMin, YMax, 0);

		glVertex3f(XMin, YMax, 0);
		glVertex3f(XMin, YMin, 0);

		glEnd();		
	}
	else
	{
		for (int i = 0; i < ChildrenNo; i++)
		{
			if (Children[i] == NULL)
				continue;

			Children[i]->drawself();
		}
	}
}




VECTOR3	selectRandomSeed(float* vectors,float* new_vectors, int* grid_res,QuadTree* Tree)
{
	VECTOR3	selected_seed_id;
	float entropy;
	//int finestlevel=2;
	int x_min,x_max,y_min,y_max;
	
	//Tree->CalcEntropy(vectors,new_vectors, grid_res);
	//printf("find max entropy node\n");
	//Tree->CalcEntropy_FindMaxEntropyNode(vectors,new_vectors, grid_res,x_min,x_max,y_min,y_max );
	
	//locate the cell with highest entropy
//	Tree->FindMaxEntropyNode(x_min,x_max,y_min,y_max);
	printf("maxi entropy node= %d %d %d %d\n",x_min,x_max,y_min,y_max);

	float ranx = ((float)rand()/(float)RAND_MAX)*(grid_res[0]-1);
	float rany = ((float)rand()/(float)RAND_MAX)*(grid_res[1]-1);


	//read the seed idx in the box
	return 	VECTOR3(ranx,
					rany,0	);//selected_seed_id;
}


VECTOR3	selectNextSeed(float* vectors,float* new_vectors, int* grid_res,QuadTree* Tree)
{
	VECTOR3	selected_seed_id;
	float entropy;
	//int finestlevel=2;
	int x_min,x_max,y_min,y_max;
	
	//Tree->CalcEntropy(vectors,new_vectors, grid_res);
	//printf("find max entropy node\n");
	Tree->CalcEntropy_FindMaxEntropyNode(vectors,new_vectors, grid_res,x_min,x_max,y_min,y_max,NULL ,0);
	
//	printf("maxi entropy node= %d %d %d %d\n",x_min,x_max,y_min,y_max);
	//get the point of maximum error
	float max_error=0;
	VECTOR3 max_pt;
	max_pt.Set(x_min,y_min,0);
	bool found=false;
	//for(int z=z_min; z<z_max;z++)
	{
		for(int y=y_min; y<y_max;y++)
		{
			for(int x=x_min; x<x_max;x++)
			{
				int idx=x+y*grid_res[0];//+z*grid_res[0]*grid_res[1];
				VECTOR3 ori,newf, t;
				ori.Set(vectors[idx*3+0],vectors[idx*3+1],0);
				newf.Set(new_vectors[idx*3+0],new_vectors[idx*3+1],0);
				t=ori-newf;
				float mg=t.GetMag();
				
				if(mg>max_error)//find the point of most error, careful do not hit critical point
				{
					max_error=mg;
					max_pt.Set(x,y,0);
					//found=true;
				}
			}
		}
	}

	return max_pt;
}

VECTOR3	selectNextSeed2(float* vectors,float* new_vectors, int* grid_res,QuadTree* Tree)
{

	float avg_error=0;
	float max_error=0;
	VECTOR3 max_pt;
	max_pt.Set(-1,-1,0);
	
	bool selected=false;
	
	int max_idx=0;
	for(int y=5; y<grid_res[1]-5;y++)
	{
		for(int x=5; x<grid_res[0]-5;x++)
		{
			
			int idx=x+y*grid_res[0];//+z*grid_res[0]*grid_res[1];
			VECTOR3 ori,newf, t;
			ori.Set(vectors[idx*3+0],vectors[idx*3+1],0);
			newf.Set(new_vectors[idx*3+0],new_vectors[idx*3+1],0);
			t=ori-newf;
			float mg=t.GetMag();
			
			if(mg>max_error)//find the point of most error, careful do not hit critical point
			{
				max_error=mg;
				avg_error+=mg;
				max_pt.Set(x,y,0);
				max_idx=idx;
				//found=true;
			}
		}
	}

	float area=(grid_res[0]-5)*(grid_res[1]-5);
	printf("avg err=%f max err=%f\n", avg_error/area, max_error);
	printf("before vector=%f %f\n",vectors[max_idx*3+0],vectors[max_idx*3+1]);
	printf("after  vector=%f %f\n",new_vectors[max_idx*3+0],new_vectors[max_idx*3+1]);
	return max_pt;
}


float evaluate_error(int idx,std::vector<vtListVertex*>	Graph,float* vectors,float* new_vectors)
{
	float mg=0;//error magnitude
	vtListVertex*	neighbors=Graph[idx];
	vtListVertex	current_neighbors=*neighbors;
	vtListVertex	next_level_neighbors;
	for(int i=0;i<3; i++)//see three layers of neighbors
	{
		for(int j=0;j<current_neighbors.size()/2;j++)
		{
			int nodeid=current_neighbors[j*2+0];
			int weight=current_neighbors[j*2+1];
			float fweight=((float)weight)/100.0;
			VECTOR3 ori,newf, t;
			ori.Set(vectors[nodeid*3+0],vectors[nodeid*3+1],0);
			newf.Set(new_vectors[nodeid*3+0],new_vectors[nodeid*3+1],0);
			ori.Normalize();
			newf.Normalize();

			t=ori-newf;
			mg=mg+(t.GetMag()*fweight)*(((float)(3-i))/3.0);
		
			vtListVertex	tmp_neighbors=*(Graph[nodeid]);
			for(int t=0;t<tmp_neighbors.size(); t++)
			{
				next_level_neighbors.push_back(tmp_neighbors[t]);
			}
		}
		current_neighbors.clear();
		current_neighbors=next_level_neighbors;
	}
	return mg;
}

void dump_entrpy_every_point(QuadTree* Tree,int* grid_res,
							 float* vectors, float* new_vectors, int* occupied,
							 VECTOR3& st, VECTOR3& ed,float hx)
{
	std::vector<VECTOR3> leaves;
	std::vector<float> entropy_leaves;
	float target_entropy=0.1;
	if(leaves.size()==0)
	{
		Tree->getLeaves(leaves);
		for(int i=0;i<leaves.size()/2; i++)
			entropy_leaves.push_back(9999);
	}

	float max=0;

	unsigned char* entropy_img=new unsigned char[grid_res[0]*grid_res[1]];
	for(int i=0;i<leaves.size()/2;i++)
	{
		if(entropy_leaves[i]<target_entropy)
			continue;
		VECTOR3 start,end;
		start=leaves[i*2+0];
		end=leaves[i*2+1];

		float entropy=calcRelativeEntropy6( vectors,new_vectors, grid_res, 
				VECTOR3(start.x(),start.y(),start.z()),
				VECTOR3(end.x(),end.y(),end.z()),
				occupied);
		float area=(end.y()-start.y())*(end.x()-start.x());
		float alpha=1;//1;//.2;//0: emhpasize density, 1: emphasize entropy
		int density=1;
		if(occupied)//add density control
		{			
			bool found=false;
			for(int y=start.y();y<end.y();y++)
			{
				for(int x=start.x();x<end.x();x++)
				{
					if(occupied[x+y*grid_res[0]]!=0)//one cell count once
					{
						found=true;
						break;
					}
				}
				if(found==true)
					break;
					
			}	
			
			float densityv=0;
			if(found==false)
				densityv=1;
			else
				densityv=0;

		//	if(entropy<1)//fill in
			entropy=(entropy/hx)*alpha+(1-alpha)*densityv;
		}	

		for(int y=start.y();y<end.y();y++)
		{
			for(int x=start.x();x<end.x();x++)
			{
				entropy_img[x+y*grid_res[0]]=(unsigned char)(entropy*255);
				
			}
		}
		

		
		entropy_leaves[i]=entropy;

		if(max<entropy)
		{
			max=entropy;
			st=start;
			ed=end;
			
		}
		
	}
	save2PPM("entropy_blocks.ppm", entropy_img, grid_res[0],grid_res[1]);
	delete [] entropy_img;
	
}
//gradient descend 
VECTOR3	selectNextSeed3_graph(float* vectors,float* new_vectors, int* grid_res,QuadTree* Tree,std::vector<vtListVertex*>	Graph,
						int& x_min,int& x_max,int& y_min,int& y_max,int* occupied,float hx)
{
	VECTOR3	selected_seed_id;
	float entropy;

	VECTOR3 st,ed;
	dump_entrpy_every_point(Tree,grid_res,
							 vectors, new_vectors, occupied,st,ed,hx);
	x_min=st.x(); x_max=ed.x();
	y_min=st.y(); y_max=ed.y();

//	Tree->CalcEntropy_FindMaxEntropyNode(vectors,new_vectors, grid_res,x_min,x_max,y_min,y_max,occupied );

	//delete [] tmpv;
	//get the point of maximum error
	float max_error=0;
	VECTOR3 max_pt;
	max_pt.Set(x_min,y_min,0);
	bool found=false;
	//for(int z=z_min; z<z_max;z++)
	//start from the middle
	int y=(y_min+y_max)/2;
	int x=(x_min+x_max)/2;
	max_pt.Set(x,y,0);

	int idx=x+y*grid_res[0];
	float cur_err= evaluate_error(idx,Graph,vectors,new_vectors);
	while(1)
	{
		bool found=false;
		for(int ny=-1;ny<=1;ny++)
		{
			for(int nx=-1;nx<=1;nx++)
			{
				if(nx+x<x_min||nx+x>x_max||ny+y<y_min||ny+y>y_max)
				{
					return max_pt;
				}
				
				if((nx==0) &&(ny==0))//self
					continue;
				int idx1=x+nx+(y+ny)*grid_res[0];
				float new_err= evaluate_error(idx1,Graph,vectors,new_vectors);
				if(new_err>cur_err)//walk
				{
					x=nx+x;
					y=ny+y;
					max_pt.Set(x,y,0);
					cur_err=new_err;
					//found=true;
					//break;
				}
				
			}
			//if(found==true)
			//	break;
		}
		if(found==false)//did not find the next step
			break;
		
	}
	return max_pt;
}
//exhaustiv esearch the maximum
VECTOR3	selectNextSeed3_local_maximum(float* vectors,float* new_vectors, int* grid_res,QuadTree* Tree,std::vector<vtListVertex*>	Graph,
						int& x_min,int& x_max,int& y_min,int& y_max,int* occupied,float hx)

{
	VECTOR3	selected_seed_id;
	float entropy;
	
	//Tree->CalcEntropy_FindMaxEntropyNode(vectors,new_vectors, grid_res,x_min,x_max,y_min,y_max,occupied );

	VECTOR3 st,ed;
	dump_entrpy_every_point(Tree,grid_res,
							 vectors, new_vectors, occupied,st,ed,hx);
	x_min=st.x(); x_max=ed.x();
	y_min=st.y(); y_max=ed.y();

	

//	return (VECTOR3( (x_min+x_max)/2, (y_min+y_max)/2,0));

	printf("the region picked=%d %d %d %d\n",x_min,x_max,y_min,y_max);
	//get the point of maximum error
	float max_error=0;
	VECTOR3 max_pt;
	max_pt.Set((x_min+x_max)/2,(y_min+y_max)/2,0);
	bool found=false;
	int width=abs((int)(ed.x()-st.x())); int height=abs((int)(ed.y()-st.y()));
	float* entropy_point_by_point=new float[width*height];
	memset(entropy_point_by_point,-1,sizeof(float));
	// MOD-BY-LEETEN 03/15/2010-FROM:
		// int  kernel_size= 8;//conditional entropy kernel
	// TO:
	int  kernel_size = KERNEL_SIZE;//conditional entropy kernel
	// MOD-BY-LEETEN 03/15/2010-END

	double dwStart = GetTickCount();

	//for(int z=z_min; z<z_max;z++)
	{
		for(int y=y_min; y<y_max;y++)
		{
			for(int x=x_min; x<x_max;x++)
			{
				int idx=x+y*grid_res[0];//+z*grid_res[0]*grid_res[1];
				VECTOR3 start,end;
				start.Set(max(0,x-kernel_size),max(0,y-kernel_size),0);
				end.Set(min(x+kernel_size,grid_res[0]-1),min(y+kernel_size,grid_res[1]-1),0);

				
				float entropy=calcRelativeEntropy6(vectors,new_vectors,grid_res,start, end,0);
				int idx2=(int)(x-x_min)+((int)(y-y_min)*width);
				entropy_point_by_point[idx2]=entropy;
			}

		}
	}
	double elapsedTime= GetTickCount() - dwStart;
	printf("\n\n local entropy compute time is %.3f milli-seconds.\n",elapsedTime); 	
	//look for the local maximum
	dwStart = GetTickCount();
	float maxi_entropy=0.1;
	for(int y=0;y<height;y++)
		for(int x=0;x<width;x++)
		{
			int kernel_size=1;
			int idx1=x+y*width;
			float me=entropy_point_by_point[idx1];
			bool found=false;
			for(int ny=-kernel_size;ny<=kernel_size;ny++)
			{
				for(int nx=-kernel_size;nx<=kernel_size;nx++)
				{
					int newx=x+nx; int newy=y+ny;
					if(newx<0||newx>width-1||newy<0||newy>height-1)
						continue;
					int idx2=newx+newy*width;
					if(nx==0 && ny==0)
						continue;
					float you=entropy_point_by_point[idx2];
					if(you>=me)
					{
						found=true;
						break;
					}
				}
				if(found==true)
					break;
			}
			if( (found==false)&&(entropy_point_by_point[idx1]>maxi_entropy))//mark this point
			{
				maxi_entropy=entropy_point_by_point[idx1];
				max_pt.Set(x+x_min,y+y_min,0);
			}
			
		}

		elapsedTime= GetTickCount() - dwStart;
		printf("\n\n looking for local maximum compute time is %.3f milli-seconds.\n",elapsedTime); 	
	
/*
	
	int * bin_vector=new int[grid_res[0]*grid_res[1]];
	int * bin_newvectors=new int[grid_res[0]*grid_res[1]];
	int binnum=60;
	getbins(vectors,bin_vector,  grid_res, binnum);
	getbins(new_vectors,bin_newvectors,  grid_res, binnum);

	float* img_entropies=new float[grid_res[0]*grid_res[1]];
//	int kernel_size=5; 
	float maximum=0.1;
	for(int y=0;y<grid_res[1];y++)
	{
		for(int x=0;x<grid_res[0];x++)
		{
			VECTOR3 start,end;
			start.Set(max(0,x-kernel_size),max(0,y-kernel_size),0);
			end.Set(min(x+kernel_size,grid_res[0]-1),min(y+kernel_size,grid_res[1]-1),0);


			float entropy=calcRelativeEntropy6_load_bins(bin_vector, bin_newvectors,
				 grid_res,  start, end);

			//float entropy=calcRelativeEntropy6( vectors,new_vectors, grid_res, 
			//		start,end).x(),start.y(),start.z()),
			//		VECTOR3(end.x(),end.y(),end.z()),0);

			int idx=x+y*grid_res[0];
			img_entropies[idx]=entropy;
			if(entropy>maximum)
				maximum=entropy;
		}
	}
	delete [] bin_newvectors;
	delete [] bin_vector;

//	elapsedTime= GetTickCount() - dwStart;
//	printf("\n\n entorpy for each point time is %.3f milli-seconds.\n",elapsedTime); 	

	//save to image
	unsigned char* data;
	data = new unsigned char[grid_res[0]*grid_res[1]];
	for(int i=0;i<grid_res[0]*grid_res[1];i++)
	{	
		//img_entropies[i]=img_entropies[i];///maximum;
		data[i]=(unsigned char)(img_entropies[i]/maximum*255);
		
		//printf("data=%.2f ",img_entropies[i]);

	}
	delete [] img_entropies;
	//the bound
	x_min=st.x(); x_max=ed.x();
	y_min=st.y(); y_max=ed.y();

	int x,y;
	for(x=x_min;x<x_max;x++)
	{
		int idx=x+y_min*grid_res[0];
		int idx2=x+y_max*grid_res[0];
		data[idx]=255;
		data[idx2]=255;
	}
	
	for(y=y_min;y<y_max;y++)
	{
		int idx=x_min+y*grid_res[0];
		int idx2=x_max+y*grid_res[0];
		data[idx]=255;
		data[idx2]=255;
	}
	//the seed
	int idx=(int)max_pt.x()+((int)(max_pt.y())*grid_res[0]);
	data[idx]=255;
	save2PPM_3("entropy_image.ppm", data, grid_res[0],grid_res[1]);

	//memset(data,0,sizeof(unsigned char)*grid_res[0]*grid_res[1]);
	draw_streamlines_ppm(data,grid_res,sl_list);
	save2PPM_3("entropy_image_line.ppm", data, grid_res[0],grid_res[1]);
*/

	
	return max_pt;
}
//

/*
//exhaustiv esearch the maximum
VECTOR3	selectNextSeed3(float* vectors,float* new_vectors, int* grid_res,QuadTree* Tree,std::vector<vtListVertex*>	Graph,
						int& x_min,int& x_max,int& y_min,int& y_max,int* occupied)

{
	VECTOR3	selected_seed_id;
	float entropy;
	
	Tree->CalcEntropy_FindMaxEntropyNode(vectors,new_vectors, grid_res,x_min,x_max,y_min,y_max,occupied );
	
	//get the point of maximum error
	float max_error=0;
	VECTOR3 max_pt;
	max_pt.Set(x_min,y_min,0);
	bool found=false;
	//for(int z=z_min; z<z_max;z++)
	{
		for(int y=y_min; y<y_max;y++)
		{
			for(int x=x_min; x<x_max;x++)
			{
				int idx=x+y*grid_res[0];//+z*grid_res[0]*grid_res[1];

				float mg=0;//error magnitude
				vtListVertex*	neighbors=Graph[idx];
				vtListVertex	current_neighbors=*neighbors;
				vtListVertex	next_level_neighbors;
				int neighbor_layer_num=3;
				for(int i=0;i<neighbor_layer_num; i++)//see three layers of neighbors
				{
					for(int j=0;j<current_neighbors.size()/2;j++)
					{
						int nodeid=current_neighbors[j*2+0];
						int weight=current_neighbors[j*2+1];
						float fweight=((float)weight)/100.0;
						VECTOR3 ori,newf, t;
						ori.Set(vectors[nodeid*3+0],vectors[nodeid*3+1],0);
						newf.Set(new_vectors[nodeid*3+0],new_vectors[nodeid*3+1],0);
						ori.Normalize();
						newf.Normalize();

						t=ori-newf;
						mg=mg+(t.GetMag()*fweight*(neighbor_layer_num+1-i)*(1.0/((float)neighbor_layer_num)));
					
						
						{
							vtListVertex	tmp_neighbors=*(Graph[nodeid]);
							for(int t=0;t<tmp_neighbors.size(); t++)
							{
								next_level_neighbors.push_back(tmp_neighbors[t]);
							}
						}
					}
					current_neighbors.clear();
					current_neighbors=next_level_neighbors;
				}
				if(mg>max_error)//find the point of most error, careful do not hit critical point
				{
					max_error=mg;
					max_pt.Set(x,y,0);
					//found=true;
				}
			}
		}
	}

	return max_pt;
}
*/

/*
void get_flow_in_neighbor(VECTOR3 pos, std::vector<vtListVertex*> Graph, int& neighbors,int* grid_res)
{
	int idx=(int)pos.x()+((int)pos.y()*grid_res[0]);
	vtListVertex	adjlist=*(Graph[idx]);

	float minw=9999; float maxw=0;
	for(int i=0;i<adjlist.size()/2;i++)
	{
		int id=adjlist[i*2+0];
		int weight=adjlist[i*2+1];
		if(weight<minw)
		{
			minw=weight;
			neighbors=id;
		}
	}

}
void get_flow_out_neighbor(VECTOR3 pos, std::vector<vtListVertex*> Graph,int& neighbors,int* grid_res)
{
	int idx=(int)pos.x()+((int)pos.y()*grid_res[0]);
	vtListVertex	adjlist=*(Graph[idx]);

	float minw=9999; float maxw=0;
	for(int i=0;i<adjlist.size()/2;i++)
	{
		int id=adjlist[i*2+0];
		int weight=adjlist[i*2+1];
		if(weight>maxw)
		{
			maxw=weight;
			neighbors=id;
		}
	}
}

void get_neighbor_by_graph(VECTOR3 pos, std::vector<vtListVertex*> Graph, std::vector<VECTOR3>& neighbors,int* grid_res)
{
	VECTOR3 p,q;
	p=q=pos;

	int flowin_neighbor,flowout_neighbor;
	int neighbor=20;
	for(int i=0;i<neighbor;i++)
	{
		get_flow_in_neighbor(p, Graph,flowin_neighbor, grid_res);
		get_flow_out_neighbor(q, Graph,flowout_neighbor, grid_res);

		VECTOR3 in,out;
		in.Set(flowin_neighbor%grid_res[0],flowin_neighbor/grid_res[0],0);
		out.Set(flowout_neighbor%grid_res[0],flowout_neighbor/grid_res[0],0);
		neighbors.push_back(out);
		neighbors.insert(neighbors.begin(),in);

		p=in;
		q=out;
	}
}
VECTOR3	selectNextSeed3(float* vectors,float* new_vectors, int* grid_res,QuadTree* Tree,std::vector<vtListVertex*>	Graph,
						int& x_min,int& x_max,int& y_min,int& y_max,int* occupied)

{
	VECTOR3	selected_seed_id;
	float entropy;
	
	Tree->CalcEntropy_FindMaxEntropyNode(vectors,new_vectors, grid_res,x_min,x_max,y_min,y_max,occupied );
	
	//get the point of maximum error
	float max_error=0;
	VECTOR3 max_pt;
	max_pt.Set(x_min,y_min,0);
	bool found=false;
	//for(int z=z_min; z<z_max;z++)
	{
		for(int y=y_min; y<y_max;y++)
		{
			for(int x=x_min; x<x_max;x++)
			{
				int idx=x+y*grid_res[0];//+z*grid_res[0]*grid_res[1];

				float mg=0;//error magnitude
				VECTOR3 pos; pos.Set(x,y,0);
				std::vector<VECTOR3> neighbors;
				get_neighbor_by_graph(pos, Graph, neighbors, grid_res);

				for (int i=0;i<neighbors.size();i++)
				{
					VECTOR3 tmp=neighbors[i];
					int xx=tmp[0]; int yy=tmp[1];
					int nodeid=xx+yy*grid_res[0];

					VECTOR3 ori,newf, t;
					ori.Set(vectors[nodeid*3+0],vectors[nodeid*3+1],0);
					newf.Set(new_vectors[nodeid*3+0],new_vectors[nodeid*3+1],0);
					ori.Normalize();
					newf.Normalize();

					t=ori-newf;
					mg=mg+(t.GetMag());

				}
				if(mg>max_error)//find the point of most error, careful do not hit critical point
				{
					max_error=mg;
					max_pt.Set(x,y,0);
				}
			}
		}
	}

	return max_pt;
}
*/

/*
void dump_entrpy_every_point(QuadTree* Tree,int* grid_res,
							 float* vectors, float* new_vectors, int* occupied,
							 VECTOR3& st, VECTOR3& ed,float hx)
{
	std::vector<VECTOR3> leaves;
	std::vector<float> entropy_leaves;
	float target_entropy=0.1;
	if(leaves.size()==0)
	{
		Tree->getLeaves(leaves);
		for(int i=0;i<leaves.size()/2; i++)
			entropy_leaves.push_back(9999);
	}

	float max=0;

	for(int i=0;i<leaves.size()/2;i++)
	{
		if(entropy_leaves[i]<target_entropy)
			continue;
		VECTOR3 start,end;
		start=leaves[i*2+0];
		end=leaves[i*2+1];

		float entropy=calcRelativeEntropy6( vectors,new_vectors, grid_res, 
				VECTOR3(start.x(),start.y(),start.z()),
				VECTOR3(end.x(),end.y(),end.z()),
				occupied);
		float alpha=.5;
		if(occupied)//add density control
		{			
			int celldim=(end.x()-start.x())/2;
			if(celldim>8)
				celldim=8;
			int subdomain_num=(end.x()-start.x())/celldim;

			int nonempty=0;
			int count=0;
			std::vector<int> histo;
			for(int y=start.y();y<end.y()-celldim;y+=celldim)
				for(int x=start.x();x<end.x()-celldim;x+=celldim)
				{
					int value=0,value_count=0;
					bool found=false;
					for(int ny=0;ny<celldim;ny++)
					{
						for(int nx=0;nx<celldim;nx++)
						{
							if(occupied[x+nx+(y+ny)*grid_res[0]]!=0)//one cell count once
							{
								nonempty++;
								found=true;
								break;
							}
						}
						if(found==true)
							break;
					}

					count++;//the number of cells
					
				}
			
			float pv=((float)nonempty)/((float)count);
			float densityv=1-pv;
			if(densityv<0)
			{
				printf("sth wrong pv=%f \n",pv);
				densityv=0;
			}
			entropy=(entropy)*alpha+(1-alpha)*densityv;
		}	
		

		entropy_leaves[i]=entropy;

		if(max<entropy)
		{
			max=entropy;
			st=start;
			ed=end;
			if(st.x()<0.5&&st.y()<0.5)
				max=max;
		}
		
	}
	
}
*/



//longer neighborhool by weight, exhaustiv esearch the maximum
VECTOR3	selectNextSeed3_streamline(float* vectors,float* new_vectors, int* grid_res,QuadTree* Tree,std::vector<vtListVertex*>	Graph,
						int& x_min,int& x_max,int& y_min,int& y_max,int* occupied,float hx)

{
	VECTOR3	selected_seed_id,st,ed;
	float entropy;
	
	dump_entrpy_every_point(Tree,grid_res,
							 vectors, new_vectors, occupied,st,ed,hx);
	x_min=st.x(); x_max=ed.x();
	y_min=st.y(); y_max=ed.y();

//	Tree->CalcEntropy_FindMaxEntropyNode(vectors,new_vectors, grid_res,x_min,x_max,y_min,y_max,occupied ,hx);
/*	int width=abs((int)(ed.x()-st.x())); int height=abs((int)(ed.y()-st.y()));
	float* entropy_point_by_point=new float[width*height];
	memset(entropy_point_by_point,-1,sizeof(float));
	int kernel_size=8; 
	double dwStart = GetTickCount();

	//for(int z=z_min; z<z_max;z++)
	{
		for(int y=y_min; y<y_max;y++)
		{
			for(int x=x_min; x<x_max;x++)
			{
				int idx=x+y*grid_res[0];//+z*grid_res[0]*grid_res[1];
				VECTOR3 start,end;
				start.Set(max(0,x-kernel_size),max(0,y-kernel_size),0);
				end.Set(min(x+kernel_size,grid_res[0]-1),min(y+kernel_size,grid_res[1]-1),0);

				
				float entropy=calcRelativeEntropy6(vectors,new_vectors,grid_res,start, end,0);
				int idx2=(int)(x-x_min)+((int)(y-y_min)*width);
				entropy_point_by_point[idx2]=entropy;
			}

		}
	}
*/
	//get the point of maximum error
	float max_error=0;
	VECTOR3 max_pt;
	max_pt.Set((x_min+x_max)/2,(y_min+y_max)/2,0);
	bool found=false;
	//for(int z=z_min; z<z_max;z++)
	{
		for(int y=y_min; y<y_max;y++)
		{
			for(int x=x_min; x<x_max;x++)
			{
				int idx=x+y*grid_res[0];//+z*grid_res[0]*grid_res[1];

				float mg=0;//error magnitude
//				vtListVertex*	neighbors=Graph[idx];
//				vtListVertex	current_neighbors=*neighbors;
//				vtListVertex	next_level_neighbors;

				VECTOR3 seed;
				seed.Set(x,y,0);
				list<vtListSeedTrace*> lines,new_lines; 
				osuflow->SetEntropySeedPoints( &seed,1);
				osuflow->SetIntegrationParams(1, 1);							//small and large step size
				osuflow->GenStreamLines(lines , BACKWARD_AND_FORWARD, 5, 0);	 //maxi steps

				std::list<vtListSeedTrace*>::iterator pIter; 
				pIter = lines.begin(); 
				for (; pIter!=lines.end(); pIter++) 
				{
					vtListSeedTrace *trace = *pIter; 
					std::list<VECTOR3*>::iterator pnIter; 
					pnIter = trace->begin(); 
					VECTOR3 q;
					for (; pnIter!= trace->end(); pnIter++) {
						VECTOR3 p = **pnIter; 
						int xx=p[0]; int yy=p[1]; int zz=p[2]; 
						
						int nodeid=xx+yy*grid_res[0];//+z*grid_res[0]*grid_res[1];

						VECTOR3 ori,newf, t;
						ori.Set(vectors[nodeid*3+0],vectors[nodeid*3+1],0);
						newf.Set(new_vectors[nodeid*3+0],new_vectors[nodeid*3+1],0);
						ori.Normalize();
						newf.Normalize();

						t=ori-newf;
						mg=mg+(t.GetMag());
						
						/*if(xx>x_min && xx<x_max && yy>y_min && yy<y_max&&(occupied[nodeid]!=1))
						{
						int nodeid_2=(xx-x_min)+(yy-y_min)*grid_res[0];//+z*grid_res[0]*grid_res[1];
						mg=mg+entropy_point_by_point[nodeid_2];
						}
						*/

					}
				}
				for (; pIter!=lines.end(); pIter++) 
				{
					vtListSeedTrace *trace = *pIter; 
					delete trace;
				}
				if(mg>max_error)//find the point of most error, careful do not hit critical point
				{
					max_error=mg;
					max_pt.Set(x,y,0);
				}
			}
		}
	}

	//delete [] entropy_point_by_point;
	return max_pt;
}

/*
//level of detail point of view
void compute_streamlines() 
{

	printf("generating seeds...\n"); 
	int num=0;
	int grid_res[3];
	float* vectors=get_grid_vec_data(grid_res);//get vec data at each grid point


												//load seeds from file
	int nSeeds; 
	VECTOR3* seeds=loadSeedfromFile(strcat(g_filename,".seed"),nSeeds);

	osuflow->SetEntropySeedPoints( seeds,nSeeds);
	seeds = osuflow->GetSeeds(nSeeds); 

												//advect streamlines
	for (int i=0; i<nSeeds; i++) 
		printf(" seed no. %d : [%f %f %f]\n", i, seeds[i][0], 
		seeds[i][1], seeds[i][2]); 

	sl_list.clear(); 
	list<vtListSeedTrace*> lines, all_streamlines; 
	printf("compute streamlines..\n"); 
	osuflow->SetIntegrationParams(1, 5);							//small and large step size
	osuflow->GenStreamLines(lines , BACKWARD_AND_FORWARD, 500, 0);	 //maxi steps
	printf(" done integrations\n"); 
																	//combine the half lines
	combinehalflines(lines, all_streamlines);

	int next_id;
	int * footsteps=new int[grid_res[0]*grid_res[1]];
	memset(footsteps,-1,grid_res[0]*grid_res[1]);
	initial_boundaries(sl_list, footsteps, grid_res, 0, next_id);
	

	int line_num_thres=2;											//want to select top line_num_thres lines
	int selected_line_num=0;
																	//select first seed, pick the one with the longest streamline 
	std::list<vtListSeedTrace*> line_selected;
	VECTOR3* newseed= new VECTOR3[1];
	std::list<vtListSeedTrace*>::iterator pIter; 
	int id=0;
	float maxlen=0;
	int selected_seed_id;
	pIter = all_streamlines.begin(); 
	for (; pIter!=all_streamlines.end(); pIter++) 
	{
		vtListSeedTrace *trace = *pIter; 
		float len=trace->size();
		if(len>maxlen)
		{
			maxlen=len;
			selected_seed_id=id;
		}
		id++;
	}
	//get the selected lines
	vector<int> vector_selected_seed_id;
	vector_selected_seed_id.push_back(selected_seed_id);

	selected_line_num++;
	float* new_vectors=new float[grid_res[0]*grid_res[1]*3];
	memset(new_vectors,0,sizeof(float)*grid_res[0]*grid_res[1]*3);
	
	while(selected_line_num<line_num_thres)
	{
		vtListSeedTrace* new_lines=getSelectedLine(all_streamlines, selected_seed_id);
		list<vtListSeedTrace*> tmplist;
		tmplist.push_back(new_lines);
		//2. update streamline foot step
		marktriangles(tmplist,footsteps ,grid_res,next_id );

		sl_list.push_back(new_lines);						//push back the new line for display

		printf("start reconstructing the  fieldn");
	
		reconstruct_field(new_vectors,vectors,grid_res,sl_list,footsteps);

		dumpReconstruedField("reconstructed.vec", new_vectors, grid_res);
		printf("*** reconstructed field saved*** \n");

		selected_seed_id=selectNextSeed(vectors,new_vectors, grid_res,footsteps);
																//select new seed
		selected_line_num++;
	}
	printf("press key to continue\n");
	getchar();
	//selectively display streamlines
}
*/
	



void dumphistogram(int* bin, int* grid_res, int binnum, int n)
{
	int* histo=new int[binnum];
	memset(histo,0,binnum*sizeof(int));

	grid_res[2]=1;
	for(int i=0;i<grid_res[0]*grid_res[1]*grid_res[2];i++)
	{
		histo[bin[i]]+=1;
	}
	char filename[250];
	memset(filename,0,250);
	sprintf(filename,"histo%d.dat",n);
	FILE* fp=fopen(filename,"wb");
	binnum-=2;
	fwrite(&binnum,sizeof(int),1,fp);
	fwrite(histo,sizeof(int),binnum,fp);
	fclose(fp);
}

void getbinnums(float* vecsori, float* vecs, int* grid_res, int binnum, int* outbins)
{
	for(int idx=0;idx<grid_res[0]*grid_res[1];idx++)
	{
		VECTOR3	newf,orif;
		orif.Set(vecsori[idx*3+0],vecsori[idx*3+1],0);
		newf.Set(vecs[idx*3+0],vecs[idx*3+1],0);
			
		newf.Normalize();
		float scalar[2];
		if((orif.x()==0) && (orif.y()==0))
			scalar[0]=0;
		else
			scalar[0]=3.1415926+(atan2(orif.y(),orif.x()));

		if((newf.x()==0) && (newf.y()==0))
			scalar[1]=0;
		else
			scalar[1]=3.1415926+(atan2(newf.y(),newf.x()));

		if(fabs(scalar[1]-scalar[0])<3.1415926/((float)binnum)*2)
				scalar[1]=scalar[0];
		outbins[idx]=(binnum-1)*scalar[1]/(2*3.1415926);
	}
}

float * cumsum(float * p, int num)
{
         if (num<=0)
             return NULL;

         float * a=new float [num];
         a[0] = p[0];
         for ( int i=1; i<num; i++)
         {
                  a[i]=a[i-1] + p[i];
         }
         return a;
}


// ADD-BY-LEETEN 03/23/2010-BEGIN
float FSampleFrom(float* p,int num)
{
	#if	0	// MOD-BY-LEETEN 03/25/2010-FROM:
		float * a=cumsum(p,num);
		 float fI;
		 float u = ((float )rand()/( float )RAND_MAX);
		 if( 0.0f == u )
			 fI = 0.0f;
		 else
		 {
			 int i ;
			 for(i = 0; i < num; i++)
				 if( u < a[i] )
				 {
					 break;
				 }

			 if( num == i )
				 fI = float(num);
			 else
			 {
				 float fMin, fMax;
				 if( 0 == i )
				 {
					 fMin = 0.0f;
					 fMax = a[i];
				 }
				 else
				 {
					 fMin = a[i - 1];
					 fMax = a[i];
				 }

				 fI = float(i) + (u - fMin)/(fMax - fMin);

				 // TEST-DEBUG	LOG(printf("%f in (%f, %f)", u, fMin, fMax));
			 }
		}
		delete [] a;
	#else	// MOD-BY-LEETEN 03/25/2010-TO:

	float u = float(rand())/ float(RAND_MAX - 1);
	float fI = float(num - 1);
	float fMin = 0.0f;
	float fMax = 0.0f;
	for(int i = 0; i < num; i++)
	{
		fMax += p[i];
		if( fMin <= u && u < fMax )
		{
			fI = float(i) + (u - fMin)/(fMax - fMin);
			break;
		}
		fMin = fMax;
	}
	fI = min(max(fI, 0.0f), float(num - 1));
	#endif	// MOD-BY-LEETEN 03/25/2010-END
	return fI;
}
// ADD-BY-LEETEN 03/23/2010-END

int sample_from( float* p,int num)
{
         float * a=cumsum(p,num);


         float u = ((float )rand()/( float )RAND_MAX);
		 int i=num-2;
         for ( i=num-2; i>=0; i--)
         {
                  if (a[i]<u)
                        break;
         }


         delete [] a;
         return i+1;
}

// ADD-BY-LEETEN 03/16/2010-BEGIN
void 
_DumpField2D(int grid_res[3], float *pfField2D, char *szFilename)
{
	// open the file
	FILE *fpFile;
	fpFile = fopen(szFilename, "wb");
	assert(fpFile);

	// write the res.
	fwrite(grid_res, sizeof(grid_res[0]), 2, fpFile);

	// make the third res. as 1
	int iOne = 1;
	fwrite(&iOne, sizeof(iOne), 1, fpFile);

	// write the data
	fwrite(pfField2D, sizeof(pfField2D[0]), grid_res[0] * grid_res[1], fpFile);

	// close the file
	fclose(fpFile);
}

int IWrapCoord(int tx, int ty, int grid_res[2])
{
	int idx;
	#if		WRAP_MODE==WRAP_MODE_CLAMP

	if( tx < 0 || tx >= grid_res[0] ||
		ty < 0 || ty >= grid_res[1] )
	{
		idx = -1;
		// idx = 0;
	}
	else
		idx = tx+ty*grid_res[0];

	#elif	WRAP_MODE==WRAP_MODE_MIRROR

	int tx_mirrored = (tx<0)?(-tx):( (tx<grid_res[0])?tx:(grid_res[0]-1 - (tx-(grid_res[0]-1))) );
	int ty_mirrored = (ty<0)?(-ty):( (ty<grid_res[1])?ty:(grid_res[1]-1 - (ty-(grid_res[1]-1))) );
	idx = tx_mirrored+ty_mirrored*grid_res[0];

	#elif	WRAP_MODE==WRAP_MODE_REPEAT

	tx = tx % grid_res[0];
	ty = ty % grid_res[1];
	idx = tx + ty * grid_res[0];

	#elif	WRAP_MODE==WRAP_MODE_CLAMP_TO_BORDER

	tx = min(max(tx, 0), grid_res[0] - 1);
	ty = min(max(ty, 0), grid_res[1] - 1);
	idx = tx+ty*grid_res[0];

	#endif	// #if	WRAP_MODE
	return idx;
}
// ADD-BY-LEETEN 03/16/2010-END

// ADD-BY-LEETEN 03/24/2010-BEGIN
int IQSortImportances(const void *p0, const void *p1)
{
	float4 *pf4P0 = (float4*)p0;
	float4 *pf4P1 = (float4*)p1;
	if( pf4P0->w > pf4P1->w )
		return +1;
	if( pf4P0->w < pf4P1->w )
		return -1;
	return 0;
}
// ADD-BY-LEETEN 03/24/2010-END

//speed up by reusing histogram partially
void selectStreamlines_by_distribution(float* vectors,float* new_vectors, int* grid_res, 
									   int x_min,int x_max,int y_min,int y_max,int* occupied,float hx,
									   std::vector<VECTOR3>& seeds,
									   PointRef* m_grid, float in_entropy)
{

	// MOD-BY-LEETEN 02/05/2010-FROM:
		// int sample_number_allowed=100;
	// TO:
	int sample_number_allowed=NR_OF_SAMPLES;
	// ADD-BY-LEETEN 03/19/2010-BEGIN
	#if	SAMPLE_LOCAL_MAX_FIRST
		// extend the pool in order to store the extrat seeds arond the critical points
	// MOD-BY-LEETEN 03/22/2010-FROM:
		// sample_number_allowed *= 9;
	// TO:
	sample_number_allowed *= KERNEL_SIZE_AROUND_CRITICAL_POINT;
	// MOD-BY-LEETEN 03/22/2010-END
	#endif	// #if	SAMPLE_LOCAL_MAX_FIRST
	// ADD-BY-LEETEN 03/19/2010-END

	// MOD-BY-LEETEN 02/05/2010-END
	// MOD-BY-LEETEN 03/15/2010-FROM:
		// int  kernel_size= 8;//conditional entropy kernel
	// TO:
	int  kernel_size = KERNEL_SIZE;//conditional entropy kernel
	// MOD-BY-LEETEN 03/15/2010-END

	int radius=1;		//not using distance control
	float* img_entropies=new float[grid_res[0]*grid_res[1]];
	
	double dwStart = GetTickCount();
	float maximum=0;
	int* bin_vector, *bin_newvectors;
	bin_vector=new int[grid_res[0]*grid_res[1]];
	bin_newvectors=new int[grid_res[0]*grid_res[1]];

	// MOD-BY-LEETEN 03/15/2010-FROM:
		// int	binnum=60;
	// TO:
	int binnum = NR_OF_BINS;
	// MOD-BY-LEETEN 03/15/2010-END

	printf("calculate entropy...\n"); 

	getbins(vectors,bin_vector,  grid_res, binnum);
	getbins(new_vectors,bin_newvectors,  grid_res, binnum);

	float sum=0;
	int* histo_pxy,*histo_py;
	histo_pxy=new int[binnum*binnum];histo_py=new int[binnum];

	// ADD-BY-LEETEN 03/16/2010-BEGIN
	float* pfErrorImage = NULL;
	pfErrorImage = (float*)calloc(grid_res[0] * grid_res[1], sizeof(pfErrorImage[0]));
	// MOD-BY-LEETEN 03/18/2010-FROM:
		// assert(fErrorImage);
	// TO:
	assert(pfErrorImage);
	// MOD-BY-LEETEN 03/18/2010-END
	// ADD-BY-LEETEN 03/16/2010-END

	for(int y=0;y<grid_res[1];y++)
	{
		memset(histo_pxy,0,sizeof(int)*binnum*binnum);
		memset(histo_py,0,sizeof(int)*binnum);
		for(int x=0;x<grid_res[0];x++)
		{
			// ADD-BY-LEETEN 03/16/2010-BEGIN
			int p = x + y * grid_res[0];

			float pfVec[3], pfNewVec[3];
			float fVecLength = 0.0f, fNewVecLength = 0.0f;
			for(int i = 0; i < 2; i++)
			{
				fVecLength += vectors[3*p + i] * vectors[3*p + i];
				fNewVecLength += new_vectors[3*p + i] * new_vectors[3*p + i];
			}

			if( fVecLength )
			{
				fVecLength = sqrtf(fVecLength);
				for(int i = 0; i < 2; i++)
					pfVec[i] = vectors[3*p + i] / fVecLength;
			}

			if( fNewVecLength )
			{
				fNewVecLength = sqrtf(fNewVecLength);
				for(int i = 0; i < 2; i++)
					pfNewVec[i] = new_vectors[3*p + i] / fNewVecLength;
			}

			float fError = 0.0f;
			for(int i = 0; i < 2; i++)
			{
				float fD = pfVec[i] - pfNewVec[i];
				fError += fD * fD;
			}
			pfErrorImage[p] = sqrtf(fError);
			// ADD-BY-LEETEN 03/16/2010-END

			VECTOR3 startpt,endpt;

			#if	0	// MOD-BY-LEETEN 03/16/2010-FROM:
				startpt.Set(max(0,x-kernel_size),max(0,y-kernel_size),0);
				endpt.Set(min(x+kernel_size,grid_res[0]-1),min(y+kernel_size,grid_res[1]-1),0);
			#else	// MOD-BY-LEETEN 03/16/2010-TO:
			startpt.Set(x - kernel_size, y - kernel_size,0);
			endpt.Set(x + kernel_size, y + kernel_size, 0);
			#endif	// MOD-BY-LEETEN 03/16/2010-END

			if(x==0)//calculate the initial histogram
			{
				for(int ty=(int)startpt.y(); ty<=(int)endpt.y();ty++)
				{
					for(int tx=(int)startpt.x(); tx<=(int)endpt.x();tx++)
					{
						//VECTOR3 p=point_cloud[i];
						// MOD-BY-LEETEN 03/16/2010-FROM:
							// int idx=tx+ty*grid_res[0];
						// TO:
						int idx = IWrapCoord(tx, ty, grid_res);
						if( idx < 0 )
							continue;
						// MOD-BY-LEETEN 03/16/2010-END

						int bin_no_ori=bin_vector[idx];
						int bin_no_new=bin_newvectors[idx];

						//if the point is on boundary, do not consider
						#if	0	// DEL-BY-LEETEN 03/16/2010-BEGIN
							if( (tx<=0) || tx>=grid_res[0]-1 ||
								 ty<=0  || ty>=grid_res[1]-1)
								 bin_no_ori=bin_no_new;
						#endif	// DEL-BY-LEETEN 03/16/2010-END

						if(bin_no_ori<0 || bin_no_ori>=binnum||
							bin_no_new<0 || bin_no_new>=binnum)
						{
							LOG(printf("sth wrong, bin id=%d  %d\n", bin_no_ori,bin_no_new));
						}
						idx=bin_no_ori+binnum*bin_no_new;
						histo_pxy[idx]++;//p(x,y)
						//p(y)
						histo_py[bin_no_new]++;
					}
				}
			}
	
			else//update the histogram
			{
				//move the oldest column

				for(int ty=(int)startpt.y(); ty<=(int)endpt.y();ty++)
				{
					#if		0	// MOD-BY-LEETEN 03/16/2010-FROM:
						if(x-kernel_size>=0)
						{
							int tx=x-kernel_size;
							int idx=tx+ty*grid_res[0];
					#else	// MOD-BY-LEETEN 03/16/2010-TO:
					{
						int tx = x - 1 - kernel_size;
						int idx = IWrapCoord(tx, ty, grid_res);
						if( idx < 0 )
							continue;
					#endif	// MOD-BY-LEETEN 03/16/2010-END
						int bin_no_ori=bin_vector[idx];
						int bin_no_new=bin_newvectors[idx];

						#if	0	// DEL-BY-LEETEN 03/16/2010-BEGIN
							//if the point is on boundary, do not consider
							if( (tx<=0) || tx>=grid_res[0]-1 ||
								 ty<=0  || ty>=grid_res[1]-1)
								 bin_no_ori=bin_no_new;
						#endif		// DEL-BY-LEETEN 03/16/2010-END

						if(bin_no_ori<0 || bin_no_ori>=binnum||
							bin_no_new<0 || bin_no_new>=binnum)
						{
							printf("sth wrong, bin id=%d  %d\n", bin_no_ori,bin_no_new);
						}
						idx=bin_no_ori+binnum*bin_no_new;
		
						#if	0	// DEL-BY-LEETEN 03/16/2010-BEGIN
							if(histo_pxy[idx]<=0 ||histo_py[bin_no_new]<=0 )
								printf("something wrong x=%d y=%d\n",tx,ty);
						#endif	// DEL-BY-LEETEN 03/16/2010-END
						histo_pxy[idx]--;//p(x,y)
						//p(y)
						histo_py[bin_no_new]--;
					}

					#if		0	// MOD-BY-LEETEN 03/16/2010-FROM:
						if(x+kernel_size<grid_res[0])
						{
							int tx=x+kernel_size;
							int idx=tx+ty*grid_res[0];
					#else		// MOD-BY-LEETEN 03/16/2010-TO:
					{
						int tx = x + kernel_size;
						int idx = IWrapCoord(tx, ty, grid_res);
						if( idx < 0 )
							continue;
					#endif		// MOD-BY-LEETEN 03/16/2010-END
						int bin_no_ori=bin_vector[idx];
						int bin_no_new=bin_newvectors[idx];

						#if	0	// DEL-BY-LEETEN 03/16/2010-BEGIN
							//if the point is on boundary, do not consider
							if( (tx<=0) || tx>=grid_res[0]-1 ||
								 ty<=0  || ty>=grid_res[1]-1)
								 bin_no_ori=bin_no_new;

							if(bin_no_ori<0 || bin_no_ori>=binnum||
								bin_no_new<0 || bin_no_new>=binnum)
							{
								printf("sth wrong, bin id=%d  %d\n", bin_no_ori,bin_no_new);
							}
						#endif	// DEL-BY-LEETEN 03/16/2010-END

						idx=bin_no_ori+binnum*bin_no_new;
						histo_pxy[idx]++;//p(x,y)
						//p(y)
						histo_py[bin_no_new]++;
					}
				}
		
			
			}
			float entropy=calcRelativeEntropy6_load_histograms(bin_vector, bin_newvectors,
				 grid_res,  startpt, endpt,histo_pxy,histo_py);
			//float entropy=calcEntropy(vectors,  grid_res,  start, end);

			// DEL-BY-LEETEN 03/15/2010-BEGIN
				// entropy=entropy*entropy;//*entropy*entropy;
			// DEL-BY-LEETEN 03/15/2010-END
			int idx=x+y*grid_res[0];
			img_entropies[idx]=entropy;
			if(entropy>maximum)
				maximum=entropy;
			sum=sum+entropy;
		}
		// MOD-BY-LEETEN 01/22/2010-FROM:
			// printf("y=%d/%d\n",y,grid_res[1]-1);
		// TO:
		// MOD-BY-LEETEN 03/28/2010-FROM:
			// printf("y=%d/%d\r",y,grid_res[1]-1);
		// TO:
		fprintf(stderr, "y=%d/%d\r",y,grid_res[1]-1);
		// MOD-BY-LEETEN 03/28/2010-END

		// MOD-BY-LEETEN 01/22/2010-END
	}
	// ADD-BY-LEETEN 01/22/2010-BEGIN
	printf("\n");
	// ADD-BY-LEETEN 01/22/2010-END

	delete [] bin_newvectors;
	delete [] bin_vector;

	double elapsedTime= GetTickCount() - dwStart;
	printf("\n\n entorpy for each point time is %.3f milli-seconds.\n",elapsedTime); 	

	// ADD-BY-LEETEN 03/16/2010-BEGIN
	maximum = log2(float(binnum));
	// ADD-BY-LEETEN 03/16/2010-END

	//save result to file
	unsigned char* data;
	data = new unsigned char[grid_res[0]*grid_res[1]];
	for(int i=0;i<grid_res[0]*grid_res[1];i++)
	{	
		// MOD-BY-LEETEN 03/16/2010-FROM:
			// data[i]=(unsigned char)((img_entropies[i]/maximum)*255);
		// TO:
		data[i]=(unsigned char)(255.0f * min(float(img_entropies[i])/maximum, 1.0f));
		// MOD-BY-LEETEN 03/16/2010-END
	//	data[i]=(unsigned char)((img_entropies[i]/40)*255);

	}
	printf("maximum=%f\n",maximum);
//	save2PPM_3("entropy.ppm", data, grid_res[0],grid_res[1]);
//	save2file("entropy.data", data, grid_res[0],grid_res[1]);
	
	unsigned char* data_color=new unsigned char[grid_res[0]*grid_res[1]*3];
	for(int i=0;i<grid_res[0]*grid_res[1];i++)
	{	
		data_color[3*i+0]=data[i];
		// MOD-BY-LEETEN 03/16/2010-FROM:
			// if(data[i]<(255/2))
		// TO:
		if(data[i]<=(255/2))
		// MOD-BY-LEETEN 03/16/2010-END
			data_color[3*i+1]=2*data[i];
		else
			data_color[3*i+1]=(255*2-2*data[i]);
		data_color[3*i+2]=255-data[i];
	

	}	

	// ADD-BY-LEETEN 03/16/2010-BEGIN
	static int iRound = 0;
	iRound++;

	// ADD-BY-LEETEN 03/28/2010-BEGIN
	LOG_VAR(iRound);
	// ADD-BY-LEETEN 03/28/2010-END

	char szFilename[1024];
	// ADD-BY-LEETEN 03/16/2010-END

	// MOD-BY-LEETEN 03/16/2010-FROM:
		// save2PPM_3_channels("entropy_color.ppm", data_color, grid_res[0],grid_res[1]);
	// TO:
	sprintf(szFilename, "%s_entropy%d.bin", g_filename, iRound);
	_DumpField2D(grid_res, img_entropies, szFilename);

	sprintf(szFilename, "%s_disparity%d.bin", g_filename, iRound);
	_DumpField2D(grid_res, pfErrorImage, szFilename);
	free(pfErrorImage);
	// MOD-BY-LEETEN 03/16/2010-END

	delete [] data_color;
//	printf("done\n");
//	getchar();
	//normalized it to (0~1)
	for(int i=0;i<grid_res[0]*grid_res[1];i++)
		img_entropies[i]=img_entropies[i]/sum;

	// ADD-BY-LEETEN 03/22/2010-BEGIN
	float fMaxProb = -(float)HUGE_VAL;
	for(int p = 0,	j = 0; j < grid_res[1]; j++)
		for(int		i = 0; i < grid_res[0]; i++, p++)
			fMaxProb = max(fMaxProb, img_entropies[p]);
	// ADD-BY-LEETEN 03/22/2010-END

	dwStart= GetTickCount();

	// ADD-BY-LEETEN 03/19/2010-BEGIN
	#if	0	// MOD-BY-LEETEN 03/23/2010-FROM:
		int* sx=new int[sample_number_allowed];
		int* sy=new int[sample_number_allowed];
	#else	// MOD-BY-LEETEN 03/23/2010-TO:
	float* sx=new float[sample_number_allowed];
	float* sy=new float[sample_number_allowed];
	#endif	// MOD-BY-LEETEN 03/23/2010-END
	int iNrOfSampledPoints = 0;

	#if	SAMPLE_LOCAL_MAX_FIRST

	#if	0	// DEL-BY-LEETEN 03/22/2010-BEGIN
	float fMaxProb = -(float)HUGE_VAL;
	for(int p = 0,	j = 0; j < grid_res[1]; j++)
		for(int		i = 0; i < grid_res[0]; i++, p++)
			fMaxProb = max(fMaxProb, img_entropies[p]);
	#endif	// DEL-BY-LEETEN 03/22/2010-END
	float fM = fMaxProb;

	// ADD-BY-LEETEN 03/24/2010-BEGIN
	// an array to store the importances and indices of selected seeds
	float4 *pf4SeedImportances;
	pf4SeedImportances = (float4*)calloc(sample_number_allowed, sizeof(pf4SeedImportances[0]));
	assert(pf4SeedImportances);
	// ADD-BY-LEETEN 03/24/2010-END

	// ADD-BY-LEETEN 03/23/2010-BEGIN
	// MOD-BY-LEETEN 03/24/2010-FROM:
		// double dRadius = M_SQRT2 * double(KERNEL_SIZE + 1);
	// TO:
	double dRadius = M_SQRT2 * double(SEPARATION_DISTANCE + 1);
	// MOD-BY-LEETEN 03/24/2010-END
	double ppdCircularMask[KERNEL_SIZE_AROUND_CRITICAL_POINT][2];
	for(int b = 0; b < KERNEL_SIZE_AROUND_CRITICAL_POINT; b++)
	{
		// MOD-BY-LEETEN 03/24/2010-FROM:
			// double dAngle = 2.0 * M_PI * double(b) / double(KERNEL_SIZE_AROUND_CRITICAL_POINT);
		// TO:
		double dAngle = 2.0 * M_PI * double(b) / double(KERNEL_SIZE_AROUND_CRITICAL_POINT - 1);
		// MOD-BY-LEETEN 03/24/2010-END

		double dI = cos(dAngle);
		double dJ = sin(dAngle);

		#if	0	// MOD-BY-LEETEN 03/25/2010-FROM:
			ppdCircularMask[b][0] = dI * dRadius;
			ppdCircularMask[b][1] = dJ * dRadius;
		#else	// MOD-BY-LEETEN 03/25/2010-TO:
		double dR = ( 0 == b % 2 )?dRadius:dRadius*2.0;
		ppdCircularMask[b][0] = dI * dR;
		ppdCircularMask[b][1] = dJ * dR;
		#endif	// MOD-BY-LEETEN 03/25/2010-END
	
		// ADD-BY-LEETEN 03/24/2010-BEGIN
		if( 0 == b )
		{
			ppdCircularMask[b][0] = 0.0;
			ppdCircularMask[b][1] = 0.0;
		}
		// ADD-BY-LEETEN 03/24/2010-END
	}
	// ADD-BY-LEETEN 03/23/2010-END

	// ADD-BY-LEETEN 03/23/2010-BEGIN
	// only sample the local max in the 1st round
	if( 1 == iRound )
	// ADD-BY-LEETEN 03/23/2010-END

	for(int	p = 0,	y=0; y < grid_res[1]; y++)
	{
		for(int		x=0; x < grid_res[0]; x++, p++)
		{
			float fU = float(rand()) / float(RAND_MAX);
			if( 0 == y || 0 == x || grid_res[1] - 1 == y || grid_res[0] - 1 == x )
				continue;

			// ADD-BY-LEETEN 03/22/2010-BEGIN
			if( img_entropies[p] < fMaxProb * LOCAL_MAX_THRESHOLD )
				continue;
			// ADD-BY-LEETEN 03/22/2010-END

			#if	0	// MOD-BY-LEETEN 03/25/2010-FROM:
				bool bLocalMax = true;
				for(int j = -1; j <= 1; j++)
				{
					for(int i = -1; i <= 1; i++)
					{
						if( 0 == i && 0 == j )
							continue;

						if( img_entropies[p] <= img_entropies[(x+i) + (y+j) * grid_res[0]])
						{
							bLocalMax = false;
							break;
						}
					}
					if( false == bLocalMax )
						break;
				}
				if( true == bLocalMax )
			#else	// MOD-BY-LEETEN 03/25/2010-TO:
			bool bIsLocalMax = true;
			for(int j = -1; j <= 1; j++)
				for(int i = -1; i <= 1; i++)
				{
					if( 0 == i && 0 == j )
						continue;

					if( img_entropies[p] <= img_entropies[(x+i) + (y+j) * grid_res[0]])
						bIsLocalMax = false;
				}
			if( true == bIsLocalMax )
			#endif	// MOD-BY-LEETEN 03/25/2010-END
			{
				#if	0	// DEL-BY-LEETEN 03/22/2010-BEGIN
					float fP = img_entropies[p];
					if( fU >= fP / fM )
						continue;
				#endif	// DEL-BY-LEETEN 03/22/2010-END

				#if	0	// MOD-BY-LEETEN 03/22/2010-FROM:
					if( iNrOfSampledPoints + 9 >= sample_number_allowed )
						continue;

					for(int		j = -1; j <= 1; j++)
						for(int i = -1; i <= 1; i++)
						{
							sx[iNrOfSampledPoints] = x + i;
							sy[iNrOfSampledPoints] = y + j;
							iNrOfSampledPoints++;
						}
				#else	// MOD-BY-LEETEN 03/22/2010-TO:
				if( iNrOfSampledPoints + KERNEL_SIZE_AROUND_CRITICAL_POINT >= sample_number_allowed )
					continue;

				#if	0	// MOD-BY-LEETEN 03/22/2010-FROM:
					for(int r = 0; r <= KERNEL_RADIUS_AROUND_CRITICAL_POINT; r++) 
					{
						for(int		j = -r; j <= r; j++)
							for(int i = -r; i <= r; i++)
							{
								if( r > 0 && -r < i < r && -r < j < r )
									continue;

								if( x + i < 0 || x + i >= grid_res[0] || y + j < 0 || y + j >= grid_res[1] )
									continue;

								sx[iNrOfSampledPoints] = x + i;
								sy[iNrOfSampledPoints] = y + j;
								iNrOfSampledPoints++;
							}
					}
				#else	// MOD-BY-LEETEN 03/22/2010-TO:

				#if	0	// MOD-BY-LEETEN 03/23/2010-FROM:
					int r = KERNEL_RADIUS_AROUND_CRITICAL_POINT;
					for(int		j = -r; j <= r; j++)
						for(int i = -r; i <= r; i++)
						{
							if( x + i < 0 || x + i >= grid_res[0] || y + j < 0 || y + j >= grid_res[1] )
								continue;

							sx[iNrOfSampledPoints] = x + i;
							sy[iNrOfSampledPoints] = y + j;
							iNrOfSampledPoints++;
						}
				#else	// MOD-BY-LEETEN 03/23/2010-TO:

				#if	KERNEL_SHAPE	== KERNEL_SHAPE_SQUARE		

				for(int r = 0; r <= KERNEL_RADIUS_AROUND_CRITICAL_POINT; r++) 
					for(int		j = -r; j <= r; j++)
						for(int i = -r; i <= r; i++)
							if( -r == i || +r == i || -r == j || +r == j )
							{
								sx[iNrOfSampledPoints] = min(max(double(x + M_SQRT2 * SEPARATION_DISTANCE * i), 0.0), double(grid_res[0]-1));
								sy[iNrOfSampledPoints] = min(max(double(y + M_SQRT2 * SEPARATION_DISTANCE * j), 0.0), double(grid_res[1]-1));
								iNrOfSampledPoints++;
							}

				// ADD-BY-LEETEN 03/25/2010-BEGIN
				#elif	KERNEL_SHAPE	== KERNEL_SHAPE_DIAMOND
				for(int r = 0; r <= KERNEL_RADIUS_AROUND_CRITICAL_POINT; r++) 
					for(int		j = -1; j <= 1; j++)
						for(int i = -1; i <= 1; i++)
						{
							int iManhattanDist = abs(i) + abs(j);
							int iWeight = (2 + 1 - iManhattanDist) * r;
							/*
							if( iWeight > 2 * KERNEL_RADIUS_AROUND_CRITICAL_POINT )
								continue;
							*/

							double dWeight = double(iWeight) * M_SQRT2 * SEPARATION_DISTANCE;

							sx[iNrOfSampledPoints] = min(max(double(x) + dWeight * double(i), 0.0), double(grid_res[0]-1));
							sy[iNrOfSampledPoints] = min(max(double(y) + dWeight * double(j), 0.0), double(grid_res[1]-1));
							iNrOfSampledPoints++;
						}
				// ADD-BY-LEETEN 03/25/2010-END
				#elif	KERNEL_SHAPE	== KERNEL_SHAPE_CIRCLE

				for(int b = 0; b < KERNEL_SIZE_AROUND_CRITICAL_POINT; b++)
				{
					sx[iNrOfSampledPoints] = min(max(double(x) + ppdCircularMask[b][0], 0.0), double(grid_res[0]-1));
					sy[iNrOfSampledPoints] = min(max(double(y) + ppdCircularMask[b][1], 0.0), double(grid_res[1]-1));
					iNrOfSampledPoints++;
				}

				#endif	// #if	KERNEL_SHAPE	

				#endif	// MOD-BY-LEETEN 03/23/2010-END
				#endif	// MOD-BY-LEETEN 03/22/2010-END
				#endif	// MOD-BY-LEETEN 03/22/2010-END
			}
		}
	}
	LOG_VAR( iNrOfSampledPoints );
	if( iNrOfSampledPoints >= sample_number_allowed )
		LOG(printf("Warning! All points have been sampled!"));

	// if not so many seeds are needed, shrink the pool back to the original size
	// MOD-BY-LEETEN 03/22/2010-FROM:
		// sample_number_allowed = sample_number_allowed / 9 + iNrOfSampledPoints;
	// TO:
	// MOD-BY-LEETEN 03/23/2010-FROM:
		// sample_number_allowed = min(sample_number_allowed, sample_number_allowed / KERNEL_SIZE_AROUND_CRITICAL_POINT + iNrOfSampledPoints);
	// TO:
	if( 1 == iRound )
		sample_number_allowed = min(sample_number_allowed, iNrOfSampledPoints);
	else
		sample_number_allowed = sample_number_allowed / KERNEL_SIZE_AROUND_CRITICAL_POINT;
	// MOD-BY-LEETEN 03/23/2010-END
	// MOD-BY-LEETEN 03/22/2010-END
	#endif	// 	#if	SAMPLE_LOCAL_MAX_FIRST

	// ADD-BY-LEETEN 03/19/2010-END

	// ADD-BY-LEETEN 03/15/2010-BEGIN
	#if	IMPORTANCE_SAMPLING	== IMPORTANCE_SAMPLING_CHAIN_RULE
	// ADD-BY-LEETEN 03/15/2010-END

	float* qy=new float[grid_res[1]];
	//get y marginal
	for(int j=0;j<grid_res[1];j++)
	{
		float sum=0;
		for(int i=0;i<grid_res[0];i++)
			sum=sum+img_entropies[i+j*grid_res[0]];
		qy[j]=sum;
	}

	float *qx_normalized=new float[grid_res[0]*grid_res[1]];
	for(int y=0;y<grid_res[1];y++)
	{
		float sum=0;
		for(int x=0;x<grid_res[0];x++)
		{
			int idx=x+y*grid_res[0];
			sum=sum+img_entropies[idx];
		}
		for(int x=0;x<grid_res[0];x++)
		{
			int idx=x+y*grid_res[0];
			if(sum>0)
				qx_normalized[idx]=img_entropies[idx]/sum;
			else
				qx_normalized[idx]=0;
		}
	}
	#if	0	// MOD-BY-LEETEN 03/19/2010-FROM:
		int* sx=new int[sample_number_allowed];
		int* sy=new int[sample_number_allowed];
		int i=0;
		while(i<sample_number_allowed)
			sy[i++] = sample_from(qy,grid_res[1]);
		i=0;
		while(i<sample_number_allowed)
		{
			sx[i] = sample_from(qx_normalized+sy[i]*grid_res[0],grid_res[0]);
			i++;
		}
	#else	// MOD-BY-LEETEN 03/19/2010-TO:
	#if	0	// MOD-BY-LEETEN 03/23/2010-FROM:
		for(int i = iNrOfSampledPoints; i<sample_number_allowed; i++)
			sy[i] = sample_from(qy,grid_res[1]);
		for(int i = iNrOfSampledPoints; i<sample_number_allowed; i++)
			sx[i] = sample_from(qx_normalized+sy[i]*grid_res[0],grid_res[0]);
	#else	// MOD-BY-LEETEN 03/23/2010-TO:
	for(int i = iNrOfSampledPoints; i<sample_number_allowed; i++)
		sy[i] = FSampleFrom(qy, grid_res[1]);
	for(int i = iNrOfSampledPoints; i<sample_number_allowed; i++)
		sx[i] = FSampleFrom(&qx_normalized[int(sy[i])*grid_res[0]], grid_res[0]);
	#endif	// MOD-BY-LEETEN 03/23/2010-END
	#endif	// MOD-BY-LEETEN 03/19/2010-END

	// ADD-BY-LEETEN 03/15/2010-BEGIN
	#elif IMPORTANCE_SAMPLING	== IMPORTANCE_SAMPLING_REJECTION_METHOD	

	#if	0	// DEL-BY-LEETEN 03/19/2010-BEGIN
	int* sx=new int[sample_number_allowed];
	int* sy=new int[sample_number_allowed];
	#endif	// DEL-BY-LEETEN 03/19/2010-END

	float* qy=new float[grid_res[1]];
	float *qx_normalized=new float[grid_res[0]*grid_res[1]];

	#if	0	// DEL-BY-LEETEN 03/23/2010-BEGIN
		float fMaxProb = -(float)HUGE_VAL;
		for(int p = 0,	j = 0; j < grid_res[1]; j++)
			for(int		i = 0; i < grid_res[0]; i++, p++)
				fMaxProb = max(fMaxProb, img_entropies[p]);
		float fM = fMaxProb;
	#endif	// DEL-BY-LEETEN 03/23/2010-END

	#if	0	// MOD-BY-LEETEN 03/25/2010-FROM:
		int i;
		for(i = 0; i < sample_number_allowed; )
	#else	// MOD-BY-LEETEN 03/25/2010-TO:
	for(int i = iNrOfSampledPoints; i<sample_number_allowed; )
	#endif	// MOD-BY-LEETEN 03/25/2010-END
	{
		float fU = float(rand()) / float(RAND_MAX);
		#if	0	// MOD-BY-LEETEN 03/23/2010-FROM:
			int iX = min(int(float(grid_res[0] * rand()) / float(RAND_MAX)), grid_res[0] - 1);
			int iY = min(int(float(grid_res[1] * rand()) / float(RAND_MAX)), grid_res[1] - 1);
			float fP =  img_entropies[iX + iY * grid_res[0]];
			if( fU < fP / fM )
			{
				sx[i] = iX;
				sy[i] = iY;
				i++;
			}
		#else	// MOD-BY-LEETEN 03/23/2010-TO:
		float fX = min(float(grid_res[0] * rand()) / float(RAND_MAX), grid_res[0] - 1);
		float fY = min(float(grid_res[1] * rand()) / float(RAND_MAX), grid_res[1] - 1);
		float fP =  img_entropies[int(fX) + int(fY) * grid_res[0]];

		if( fU < fP / fM )
		{
			sx[i] = fX;
			sy[i] = fY;
			i++;
		}
		#endif	// MOD-BY-LEETEN 03/23/2010-END
	}
	#endif	// #if IMPORTANCE_SAMPLING
	// ADD-BY-LEETEN 03/15/2010-END

	// ADD-BY-LEETEN 03/24/2010-BEGIN
	for(int s = 0; s < sample_number_allowed; s++)
	{
		int i = int(sx[s]) + int(sy[s]) * grid_res[0];
		pf4SeedImportances[s].x = sx[s];
		pf4SeedImportances[s].y = sy[s];
		#if	0	// MOD-BY-LEETEN 04/01/2010-FROM:
			pf4SeedImportances[s].w = img_entropies[i];
		#else	// MOD-BY-LEETEN 04/01/2010-TO:
		if( 1 < iRound )
		{
			pf4SeedImportances[s].w = img_entropies[i];
		}
		else
		{
			static float fLocalMax;
			if( 0 == s % KERNEL_SIZE_AROUND_CRITICAL_POINT )
			{
				fLocalMax = -(float)HUGE_VAL;
				for(int sj = 0; sj < KERNEL_SIZE_AROUND_CRITICAL_POINT ; sj++)
				{
					int j = int(sx[s+sj]) + int(sy[s+sj]) * grid_res[0];
					fLocalMax = max(fLocalMax, img_entropies[j]);
				}
			}

			pf4SeedImportances[s].w = fLocalMax * fMaxProb + img_entropies[i];
		}
		#endif	// MOD-BY-LEETEN 04/01/2010-END
	}
	// sort the seeds in descent order of importances
	qsort(pf4SeedImportances, sample_number_allowed, sizeof(pf4SeedImportances[0]), IQSortImportances);
	for(int s = 0; s < sample_number_allowed; s++)
	{
		sx[s] = pf4SeedImportances[sample_number_allowed - 1 - s].x;
		sy[s] = pf4SeedImportances[sample_number_allowed - 1 - s].y;
	}
	free(pf4SeedImportances);
	// ADD-BY-LEETEN 03/24/2010-END

	elapsedTime= GetTickCount() - dwStart;
	printf("\n\n sampling time is %.3f milli-seconds.\n",elapsedTime); 	

	// ADD-BY-LEETEN 03/23/2010-BEGIN
	// dump the coordinates of the seeds
	sprintf(szFilename, "%s_seeds.bin", g_filename);
	{
		FILE *fpSeeds;
		if( 1 == iRound )
			fpSeeds = fopen(szFilename, "wb");
		else
			fpSeeds = fopen(szFilename, "ab");

		fwrite(&sample_number_allowed, sizeof(sample_number_allowed), 1, fpSeeds);
		fwrite(sx, sizeof(sx[0]), sample_number_allowed, fpSeeds);
		fwrite(sy, sizeof(sy[0]), sample_number_allowed, fpSeeds);

		fclose(fpSeeds);
	}
	// ADD-BY-LEETEN 03/23/2010-END

	// MOD-BY-LEETEN 03/19/2010-FROM:
		// for( i=0;i<sample_number_allowed;i++)
	// TO:
	for(int i=0;i<sample_number_allowed;i++)
	// MOD-BY-LEETEN 03/19/2010-END
	{
		// MOD-BY-LEETEN 03/23/2010-FROM:
			// data[sx[i]+sy[i]*grid_res[0]]=125;
		// TO:
		data[int(sx[i])+int(sy[i])*grid_res[0]]=125;
		// MOD-BY-LEETEN 03/23/2010-END
		
	}

	float cur_entropy=in_entropy;
	//scan and get the distributions out
	int streamlineId=0;
	list<vtListSeedTrace*> lines,new_lines,tmp_whole_set_lines; 
	// MOD-BY-LEETEN 03/19/2010-FROM:
		// for(i=0;i<sample_number_allowed;i++)//start pruning
	// TO:
	for(int i=0;i<sample_number_allowed;i++)//start pruning
	// MOD-BY-LEETEN 03/19/2010-END
	{
		lines.clear();
		new_lines.clear();
		VECTOR3 newseed;
		newseed.Set(sx[i],sy[i],0);

		#if	0	// DEL-BY-LEETEN 02/08/2010-BEGIN
			// ADD-BY-LEETEN 02/05/2010-BEGIN
			LOG(printf("Seed %d = (%d, %d)", i, sx[i], sy[i]));
			// ADD-BY-LEETEN 02/05/2010-END
		#endif	// DEL-BY-LEETEN 02/08/2010-END

		int idx=(int)newseed.x()+((int)newseed.y())*grid_res[0];
		if(occupied[idx]==1)
			continue;//do not select this one

		int x=(int)newseed.x();
		int y=(int)newseed.y();
		
		
		osuflow->SetEntropySeedPoints( &newseed,1);
		// MOD-BY-LEETEN 03/24/2010-FROM:
			// osuflow->SetIntegrationParams(.1, .5);							//small and large step size
		// TO:
		osuflow->SetIntegrationParams(STREAMLINE_INIT_STEP_SIZE, STREAMLINE_MAX_STEP_SIZE);							//small and large step size
		// MOD-BY-LEETEN 03/24/2010-END
		// MOD-BY-LEETEN 02/05/2010-FROM:
			// osuflow->GenStreamLines(lines , BACKWARD_AND_FORWARD, 10,0);	 //maxi steps
		// TO:
		#if	GENERATE_PRIMITIVE == GENERATE_PRIMITIVE_GLYPHS
		osuflow->GenStreamLines(lines , BACKWARD_AND_FORWARD, 10,0);	 //maxi steps
		#endif

		#if	GENERATE_PRIMITIVE == GENERATE_PRIMITIVE_STREAMLINES
		osuflow->GenStreamLines(lines , BACKWARD_AND_FORWARD, 200,0);	 //maxi steps
		#endif
		// MOD-BY-LEETEN 02/05/2010-END

		//osuflow->GenStreamLines(lines , BACKWARD_AND_FORWARD, 2,0);	 //maxi steps

//		combinehalflines(lines, new_lines,grid_res);
		// MOD-BY-LEETEN 02/06/2010-FROM:
			// combinehalflines_check_stop(lines, new_lines,grid_res,m_grid,streamlineId);//for display only, the streamline stops when gets too near to an existing
		// TO:
		// ADD-BY-LEETEN 03/18/2010-BEGIN

		// DEL-BY-LEETEN 03/19/2010-BEGIN
		// #if	PRUNING_MODE == PRUNING_MODE_KEEP_WHEN_DISTANT	
		// DEL-BY-LEETEN 03/19/2010-END
		// ADD-BY-LEETEN 03/18/2010-END
		// MOD-BY-LEETEN 03/19/2010-FROM:
			// combinehalflines_check_stop(lines, new_lines,grid_res,m_grid,streamlineId);//for display only, the streamline stops when gets too near to an existing
		// TO:
		combinehalflines_check_stop(
			lines, 
			new_lines,
			grid_res,
			m_grid,
			streamlineId,
			// ADD-BY-LEETEN 03/26/2010-BEGIN
			fMaxProb,
			// ADD-BY-LEETEN 03/26/2010-END
			img_entropies);
		// MOD-BY-LEETEN 03/19/2010-END

		// DEL-BY-LEETEN 03/19/2010-BEGIN
		/*
		// ADD-BY-LEETEN 03/18/2010-BEGIN
		#else	// #if	PRUNING_MODE == PRUNING_MODE_KEEP_WHEN_DISTANT	
		combinehalflines(lines, new_lines,grid_res);
		#endif	// #if	PRUNING_MODE == PRUNING_MODE_KEEP_WHEN_DISTANT	
		// ADD-BY-LEETEN 03/18/2010-END
		*/
		// DEL-BY-LEETEN 03/19/2010-END
		// MOD-BY-LEETEN 02/06/2010-END

//		combinehalflines_check_stop_entropy(lines, new_lines,grid_res,entropies);//for display only, the streamline stops when gets too near to an existing
//		streamlineId++;


		
		if(new_lines.size())
		{
			// MOD-BY-LEETEN 02/05/2010-FROM:
				// float epsilon = 0.002;
			// TO:
			float epsilon = PRUNING_THRESHOLD;
			// MOD-BY-LEETEN 02/05/2010-END
	
			tmp_whole_set_lines=sl_list;
			tmp_whole_set_lines.push_back(*new_lines.begin());
			#if	0	// MOD-BY-LEETEN 02/05/2010-FROM:
				//bool discard=discardredundantstreamlines(cur_entropy,epsilon,tmp_whole_set_lines, vectors,new_vectors,grid_res);
				bool discard=false;
			#else	// MOD-BY-LEETEN 02/05/2010-TO:

			// ADD-BY-LEETEN 03/16/2010-BEGIN
			// MOD-BY-LEETEN 03/18/2010-FROM:
				// #if	ENABLE_PRUNING	
			// TO:
			#if	PRUNING_MODE == PRUNING_MODE_COND_ENTROPY
			// MOD-BY-LEETEN 03/18/2010-END
			// ADD-BY-LEETEN 03/16/2010-END
			bool discard=discardredundantstreamlines(cur_entropy,epsilon,tmp_whole_set_lines, vectors,new_vectors,grid_res);

			// ADD-BY-LEETEN 03/16/2010-BEGIN
			#else	// #if	ENABLE_PRUNING	
			bool discard = false;
			#endif	// #if	ENABLE_PRUNING	
			// ADD-BY-LEETEN 03/16/2010-END
			#endif	// MOD-BY-LEETEN 02/05/2010-END


			// ADD-BY-LEETEN 02/06/2010-BEGIN
			#if	DUMP_WHEN_ENTROPY_INCREASE
			if( true == discard )
			{
				save_streamlines_to_file(sl_list,			"streamline_set.dat");
				save_streamlines_to_file(new_lines,			"streamline_new.dat");
			}
			#endif	// #if	DUMP_WHEN_ENTROPY_INCREASE
			// ADD-BY-LEETEN 02/06/2010-END

			if(discard==false)
			{
				sl_list.push_back(*(new_lines.begin()));			
				seeds.push_back(VECTOR3(sx[i],sy[i],0));
				UpdateOccupied(new_lines,occupied,grid_res,0);
			}
			tmp_whole_set_lines.clear();
		//density control should depend on the information content too.
		}
	}
	delete [] qy;
	delete [] qx_normalized;
	delete [] sx;
	delete [] sy;
	save2PPM_3("cand_seeds_image.ppm", data, grid_res[0],grid_res[1]);
	delete [] img_entropies;

	return;
}

/*
//ori
void selectStreamlines_by_distribution(float* vectors,float* new_vectors, int* grid_res, 
									   int x_min,int x_max,int y_min,int y_max,int* occupied,float hx,
									   std::vector<VECTOR3>& seeds)
{

	int sample_number_allowed=50;
	int  kernel_size= 8;//conditional entropy kernel
	int radius=1;		//distance control
	float* img_entropies=new float[grid_res[0]*grid_res[1]];
	
	double dwStart = GetTickCount();
	float maximum=0;
	int* bin_vector, *bin_newvectors;
	bin_vector=new int[grid_res[0]*grid_res[1]];
	bin_newvectors=new int[grid_res[0]*grid_res[1]];

	int binnum=60;
	printf("calculate entropy...\n"); 

	getbins(vectors,bin_vector,  grid_res, binnum);
	getbins(new_vectors,bin_newvectors,  grid_res, binnum);

	float sum=0;
	for(int y=0;y<grid_res[1];y++)
	{
		for(int x=0;x<grid_res[0];x++)
		{
			VECTOR3 start,end;
			start.Set(max(0,x-kernel_size),max(0,y-kernel_size),0);
			end.Set(min(x+kernel_size,grid_res[0]-1),min(y+kernel_size,grid_res[1]-1),0);

			float entropy=calcRelativeEntropy6_load_bins(bin_vector, bin_newvectors,
				 grid_res,  start, end);
			//float entropy=calcEntropy(vectors,  grid_res,  start, end);

			entropy=entropy*entropy;//*entropy;
			int idx=x+y*grid_res[0];
			img_entropies[idx]=entropy;
			if(entropy>maximum)
				maximum=entropy;
			sum=sum+entropy;
		}
		printf("y=%d/%d\n",y,grid_res[1]-1);
	}
	delete [] bin_newvectors;
	delete [] bin_vector;

	double elapsedTime= GetTickCount() - dwStart;
	printf("\n\n entorpy for each point time is %.3f milli-seconds.\n",elapsedTime); 	

	
	//save result to file
	unsigned char* data;
	data = new unsigned char[grid_res[0]*grid_res[1]];
	for(int i=0;i<grid_res[0]*grid_res[1];i++)
	{	
		data[i]=(unsigned char)((img_entropies[i]/maximum)*255);
	}
	save2PPM_3("entropy.ppm", data, grid_res[0],grid_res[1]);
	save2file("entropy.data", data, grid_res[0],grid_res[1]);
	
	//normalized it to (0~1)
	for(int i=0;i<grid_res[0]*grid_res[1];i++)
		img_entropies[i]=img_entropies[i]/sum;

	float* qy=new float[grid_res[1]];
	//get y marginal
	for(int j=0;j<grid_res[1];j++)
	{
		float sum=0;
		for(int i=0;i<grid_res[0];i++)
			sum=sum+img_entropies[i+j*grid_res[0]];
		qy[j]=sum;
	}

	float *qx_normalized=new float[grid_res[0]*grid_res[1]];
	for(int y=0;y<grid_res[1];y++)
	{
		float sum=0;
		for(int x=0;x<grid_res[0];x++)
		{
			int idx=x+y*grid_res[0];
			sum=sum+img_entropies[idx];
		}
		for(int x=0;x<grid_res[0];x++)
		{
			int idx=x+y*grid_res[0];
			if(sum>0)
				qx_normalized[idx]=img_entropies[idx]/sum;
			else
				qx_normalized[idx]=0;
		}
	}
	int* sx=new int[sample_number_allowed];
	int* sy=new int[sample_number_allowed];
	int i=0;
	while(i<sample_number_allowed)
		sy[i++] = sample_from(qy,grid_res[1]);
	i=0;
	while(i<sample_number_allowed)
	{
		sx[i] = sample_from(qx_normalized+sy[i]*grid_res[0],grid_res[0]);
		i++;
	}

	for( i=0;i<sample_number_allowed;i++)
	{
		data[sx[i]+sy[i]*grid_res[0]]=125;
		
	}

	//scan and get the distributions out
	list<vtListSeedTrace*> lines,new_lines; 
	for(i=0;i<sample_number_allowed;i++)//start pruning
	{
		lines.clear();
		new_lines.clear();
		VECTOR3 newseed;
		newseed.Set(sx[i],sy[i],0);

		int idx=(int)newseed.x()+((int)newseed.y())*grid_res[0];
		if(occupied[idx]==1)//need to remove this when collecting imagse for importance sampling
			continue;//do not select this one

		int x=(int)newseed.x();
		int y=(int)newseed.y();
		
		
		osuflow->SetEntropySeedPoints( &newseed,1);
		osuflow->SetIntegrationParams(.1, 2);							//small and large step size
		osuflow->GenStreamLines(lines , BACKWARD_AND_FORWARD, 50, 0);	 //maxi steps
		combinehalflines(lines, new_lines,grid_res);//for display only, the streamline stops when gets too near to an existing
		if(new_lines.size())
		sl_list.push_back(*(new_lines.begin()));			
		
		seeds.push_back(VECTOR3(sx[i],sy[i],0));
		UpdateOccupied(lines,occupied,grid_res,1);
		//density control should depend on the information content too.

	}
	delete [] qy;
	delete [] qx_normalized;
	delete [] sx;
	delete [] sy;
	save2PPM_3("cand_seeds_image.ppm", data, grid_res[0],grid_res[1]);
	delete [] img_entropies;

	return;
}
*/
/*
//change the sampling g(.)=fx*fy
void selectStreamlines_by_distribution(float* vectors,float* new_vectors, int* grid_res, 
									   int x_min,int x_max,int y_min,int y_max,int* occupied,float hx,
									   std::vector<VECTOR3>& seeds,
									   PointRef* m_grid)
{

	int sample_number_allowed=10000;
	int  kernel_size= 8;//conditional entropy kernel
	int radius=1;		//distance control
	float* img_entropies=new float[grid_res[0]*grid_res[1]];
	
	double dwStart = GetTickCount();
	float maximum=0;
	int* bin_vector, *bin_newvectors;
	bin_vector=new int[grid_res[0]*grid_res[1]];
	bin_newvectors=new int[grid_res[0]*grid_res[1]];

	int binnum=60;
	printf("calculate entropy...\n"); 

	getbins(vectors,bin_vector,  grid_res, binnum);
	getbins(new_vectors,bin_newvectors,  grid_res, binnum);

	float sum=0;
	for(int y=0;y<grid_res[1];y++)
	{
		for(int x=0;x<grid_res[0];x++)
		{
			VECTOR3 start,end;
			start.Set(max(0,x-kernel_size),max(0,y-kernel_size),0);
			end.Set(min(x+kernel_size,grid_res[0]-1),min(y+kernel_size,grid_res[1]-1),0);

			float entropy=calcRelativeEntropy6_load_bins(bin_vector, bin_newvectors,
				 grid_res,  start, end);
			//float entropy=calcEntropy(vectors,  grid_res,  start, end);

			entropy=entropy;//*entropy;//*entropy;
			int idx=x+y*grid_res[0];
			img_entropies[idx]=entropy;
			if(entropy>maximum)
				maximum=entropy;
			sum=sum+entropy;
		}
		printf("y=%d/%d\n",y,grid_res[1]-1);
	}
	delete [] bin_newvectors;
	delete [] bin_vector;

	double elapsedTime= GetTickCount() - dwStart;
	printf("\n\n entorpy for each point time is %.3f milli-seconds.\n",elapsedTime); 	

	
	//save result to file
	unsigned char* data;
	data = new unsigned char[grid_res[0]*grid_res[1]];
	for(int i=0;i<grid_res[0]*grid_res[1];i++)
	{	
		data[i]=(unsigned char)((img_entropies[i]/maximum)*255);
	}
	
	//normalized it to (0~1)
	for(int i=0;i<grid_res[0]*grid_res[1];i++)
		img_entropies[i]=img_entropies[i]/sum;

	dwStart = GetTickCount();
	float* px=new float[grid_res[0]];
	float* qy=new float[grid_res[1]];
	for(int i=0;i<grid_res[0];i++)
	{
		float sum=0;
		for(int j=0;j<grid_res[1];j++)
			sum=sum+img_entropies[i+j*grid_res[0]];
		px[i]=sum;
	}
	for(int j=0;j<grid_res[1];j++)
	{
		float sum=0;
		for(int i=0;i<grid_res[0];i++)
			sum=sum+img_entropies[i+j*grid_res[0]];
		qy[j]=sum;
	}
		
	//start the importance sampling, acceptance-rejection method
	int* selX, *selY;
	selX=new int[sample_number_allowed];
	selY=new int[sample_number_allowed];
	maximum=0;
	for(int y=0;y<grid_res[1];y++)
	for(int x=0;x<grid_res[0];x++)
	{
		int idx=x+y*grid_res[0];
		float tmp=img_entropies[idx]/(px[x]*qy[y]);
		if(tmp>maximum)
			maximum=tmp;
	}
	float M=maximum*1.01;

	int i=0;
	// DEL-BY-LEETEN 02/05/2010 srand((unsigned)time(NULL));
	while(i<sample_number_allowed)
	{
		int x = sample_from(px,grid_res[0]);
		int y = sample_from(qy,grid_res[1]);
		float randu = ((float)rand()/(float)RAND_MAX);

		//int x=(int)(floor(randx*grid_res[0]));
		//int y=(int)(floor(randy*grid_res[1]));
		float u=randu;
		if(u<((img_entropies[x+y*grid_res[0]]/(px[x]*qy[y]))/M))
		{
			selX[i]=x;
			selY[i]=y;
			i++;

			data[x+y*grid_res[0]]=125;
		}
	}
	elapsedTime= GetTickCount() - dwStart;
	printf("\n\n sample time is %.3f milli-seconds.\n",elapsedTime); 	

	//scan and get the distributions out
	list<vtListSeedTrace*> lines,new_lines; 
	for(i=0;i<sample_number_allowed;i++)//start pruning
	{
		lines.clear();
		new_lines.clear();
		VECTOR3 newseed;
		newseed.Set(selX[i],selY[i],0);
		if(occupied[(int)newseed.x()+((int)newseed.y())*grid_res[0]]==1)
			continue;//do not select this one

		int x=(int)newseed.x();
		int y=(int)newseed.y();
		
		
		osuflow->SetEntropySeedPoints( &newseed,1);
		osuflow->SetIntegrationParams(.1, .5);							//small and large step size
		osuflow->GenStreamLines(lines , BACKWARD_AND_FORWARD, 500, 0);	 //maxi steps
		combinehalflines(lines, new_lines,grid_res);//for display only, the streamline stops when gets too near to an existing
		if(new_lines.size())
		sl_list.push_back(*(new_lines.begin()));			
		
		seeds.push_back(VECTOR3(selX[i],selY[i],0));
		UpdateOccupied(lines,occupied,grid_res,radius);

	}
	delete [] selX;
	delete [] selY;
	save2PPM_3("cand_seeds_image.ppm", data, grid_res[0],grid_res[1]);
	delete [] img_entropies;

	return;
}
*/
/*
//original g(.) as uniform distribution
void selectStreamlines_by_distribution(float* vectors,float* new_vectors, int* grid_res, 
									   int x_min,int x_max,int y_min,int y_max,int* occupied,float hx,
									   std::vector<VECTOR3>& seeds)
{

	//memset(occupied,0,grid_res[0]*grid_res[1]*sizeof(int));
	float* img_entropies=new float[grid_res[0]*grid_res[1]];
	
	double dwStart = GetTickCount();
	float maximum=0;
	int* bin_vector, *bin_newvectors;
	bin_vector=new int[grid_res[0]*grid_res[1]];
	bin_newvectors=new int[grid_res[0]*grid_res[1]];

	int binnum=60;
	printf("calculate entropy...\n"); 

	getbins(vectors,bin_vector,  grid_res, binnum);
	getbins(new_vectors,bin_newvectors,  grid_res, binnum);

	int  kernel_size= 8;
	float sum=0;
	for(int y=0;y<grid_res[1];y++)
	{
		for(int x=0;x<grid_res[0];x++)
		{
			VECTOR3 start,end;
			start.Set(max(0,x-kernel_size),max(0,y-kernel_size),0);
			end.Set(min(x+kernel_size,grid_res[0]-1),min(y+kernel_size,grid_res[1]-1),0);

			float entropy=calcRelativeEntropy6_load_bins(bin_vector, bin_newvectors,
				 grid_res,  start, end);
			//float entropy=calcEntropy(vectors,  grid_res,  start, end);

			entropy=entropy;//*entropy*entropy;
			int idx=x+y*grid_res[0];
			img_entropies[idx]=entropy;
			if(entropy>maximum)
				maximum=entropy;
			sum=sum+entropy;
		}
		printf("y=%d/%d\n",y,grid_res[1]-1);
	}
	delete [] bin_newvectors;
	delete [] bin_vector;

	double elapsedTime= GetTickCount() - dwStart;
	printf("\n\n entorpy for each point time is %.3f milli-seconds.\n",elapsedTime); 	

	
	//save result to file
	unsigned char* data;
	data = new unsigned char[grid_res[0]*grid_res[1]];
	for(int i=0;i<grid_res[0]*grid_res[1];i++)
	{	
		data[i]=(unsigned char)((img_entropies[i]/maximum)*255);
	}
	
	//normalized it to (0~1)
	for(int i=0;i<grid_res[0]*grid_res[1];i++)
		img_entropies[i]=img_entropies[i]/sum;

	//start the importance sampling, acceptance-rejection method
	int sample_number_allowed=100;
	int* selX, *selY;
	selX=new int[sample_number_allowed];
	selY=new int[sample_number_allowed];
	float M=(maximum/sum*grid_res[0]*grid_res[1])*1.1;

	int i=0;
	// DEL-BY-LEETEN 02/05/2010 srand((unsigned)time(NULL));
	while(i<sample_number_allowed)
	{
		float randx = ((float)rand()/(float)RAND_MAX);
		float randy = ((float)rand()/(float)RAND_MAX);
		float randu = ((float)rand()/(float)RAND_MAX);

		int x=(int)(floor(randx*grid_res[0]));
		int y=(int)(floor(randy*grid_res[1]));
		float u=randu;
		if(u<(img_entropies[x+y*grid_res[0]]*grid_res[0]*grid_res[1]/M))
		{
			selX[i]=x;
			selY[i]=y;
			i++;

			data[x+y*grid_res[0]]=125;
		}
	}

	//scan and get the distributions out
	list<vtListSeedTrace*> lines,new_lines; 
	for(i=0;i<sample_number_allowed;i++)//start pruning
	{
		lines.clear();
		new_lines.clear();
		VECTOR3 newseed;
		newseed.Set(selX[i],selY[i],0);
		if(occupied[(int)newseed.x()+((int)newseed.y())*grid_res[0]]==1)
			continue;//do not select this one

		osuflow->SetEntropySeedPoints( &newseed,1);
		osuflow->SetIntegrationParams(.1, .5);							//small and large step size
		osuflow->GenStreamLines(lines , BACKWARD_AND_FORWARD, 500, 0);	 //maxi steps
		combinehalflines(lines, new_lines,grid_res);//for display only, the streamline stops when gets too near to an existing
		if(new_lines.size())
		sl_list.push_back(*(new_lines.begin()));			
		int radius=2;
		seeds.push_back(VECTOR3(selX[i],selY[i],0));
		UpdateOccupied(lines,occupied,grid_res,radius);

	}
	delete [] selX;
	delete [] selY;
	save2PPM_3("cand_seeds_image.ppm", data, grid_res[0],grid_res[1]);
	delete [] img_entropies;

	return;
}
*/

void calc_entropy(float* vectors, int* grid_res,float* entropies)
{
	int radius=8;
	VECTOR3 startpt,endpt;
	for(int y=0;y<grid_res[1];y++)
	for(int x=0;x<grid_res[0];x++)
	{
		int sx,sy,ex,ey;
		sx=max(0,x-radius); sy=max(0,y-radius); 
		ex=min(x+radius,grid_res[0]-1);
		ey=min(y+radius,grid_res[1]-1);
		startpt.Set(sx,sy,0);
		endpt.Set(ex,ey,0);
		float temp=calcEntropy(  vectors,  grid_res,  startpt, endpt);
		entropies[x+y*grid_res[0]]=temp;
	}
	
}
void	norm_entropy(float* entropies, int* grid_res)
{
	float maxt=0;
	for(int y=0;y<grid_res[1];y++)
	for(int x=0;x<grid_res[0];x++)
	{
		float tmp=entropies[x+y*grid_res[0]];
		if(tmp>maxt)
			maxt=tmp;
	}
	if(maxt==0)
		return;

	for(int y=0;y<grid_res[1];y++)
	for(int x=0;x<grid_res[0];x++)
		entropies[x+y*grid_res[0]]=entropies[x+y*grid_res[0]]/maxt;
		

}
//allow select multiple seed in one pass
void compute_streamlines() 
{
	// ADD-BY-LEETEN 03/18/2010-BEGIN
	double dwComputeStreamlineBegin = GetTickCount();
	// ADD-BY-LEETEN 03/18/2010-END

	printf("generating seeds...\n"); 
	int num=0;
	int grid_res[3];
	float* vectors=get_grid_vec_data(grid_res);//get vec data at each grid point
	grid_res[2]=1;

	g_xdim=grid_res[0]; g_ydim=grid_res[1];
	if(!entropies)
	{
		entropies=new float[grid_res[0]*grid_res[1]];
		calc_entropy(vectors, grid_res,entropies);
		norm_entropy(entropies,grid_res);
	}

												//load seeds from file
	int* donotchange=new int[grid_res[0]*grid_res[1]];
	memset(donotchange,0,sizeof(int)*grid_res[0]*grid_res[1]);
	int nSeeds; 
	//set first seed as domain center
	sl_list.clear(); 
							//combine the half lines
	
	//build quadtree
//	Tree = new QuadTree(grid_res[0]-1, grid_res[1]-1);
	Tree = new QuadTree(grid_res[0], grid_res[1]);
	printf("build quad tree\n");
//	int finestlevel=16+1;//16;
//	int finestlevel=64+1;
//	Tree->Build(finestlevel);


	float* new_vectors=new float[grid_res[0]*grid_res[1]*grid_res[2]*3];
	memset(new_vectors,0,sizeof(float)*grid_res[0]*grid_res[1]*grid_res[2]*3);
	
//set the boundary
	int x=0;
	int y=0;
	#if	0	// DEL-BY-LEETEN 03/16/2010-BEGIN
		for(y=0;y<grid_res[1];y++)
		{
			int idx=x+y*grid_res[0];
			new_vectors[3*idx+0]=vectors[3*idx+0];
			new_vectors[3*idx+1]=vectors[3*idx+1];
		}
		x=grid_res[0]-1;
		for(y=0;y<grid_res[1];y++)
		{
			int idx=x+y*grid_res[0];
			new_vectors[3*idx+0]=vectors[3*idx+0];
			new_vectors[3*idx+1]=vectors[3*idx+1];
		}
		y=0;
		for(x=0;x<grid_res[0];x++)
		{
			int idx=x+y*grid_res[0];
			new_vectors[3*idx+0]=vectors[3*idx+0];
			new_vectors[3*idx+1]=vectors[3*idx+1];
		}
		y=grid_res[1]-1;
		for(x=0;x<grid_res[0];x++)
		{
			int idx=x+y*grid_res[0];
			new_vectors[3*idx+0]=vectors[3*idx+0];
			new_vectors[3*idx+1]=vectors[3*idx+1];
		}
	#endif	// DEL-BY-LEETEN 03/16/2010-END

	// ADD-BY-LEETEN 02/05/2010-BEGIN
	#if		USE_CUDA
	_FlowDiffusionInit(grid_res[0], grid_res[1], grid_res[2]);
	#endif	// #if	USE_CUDA
	// ADD-BY-LEETEN 02/05/2010-END

	int selected_line_num=0;
	float old_entropy=9999;
	float entropy=8888;
	float firstentropy=1;
	int first=1;
	std::vector<VECTOR3> seedlist;
	// DEL-BY-LEETEN 02/05/2010 srand((unsigned)time(NULL));			// initialize random number generator
	import_pos.clear();
	VECTOR3 oldseed;

	std::vector<VECTOR3> selected_list;
	//std::vector<float> entropies;

//	 std::vector<vtListVertex*>	Graph;
//	ConstructGraph_static_2D(g_filename, Graph);

	int x_min=0,x_max=0,y_min=0,y_max=0;
	int* occupied=new int[grid_res[0]*grid_res[1]];
	memset(occupied,0,sizeof(int)*grid_res[0]*grid_res[1]);
	
	//initial entropy
	float hx=calcEntropy( vectors,  grid_res, VECTOR3(0,0,0),VECTOR3(grid_res[0],grid_res[1],grid_res[2]));
	printf("entropy=%f\n",hx);
	int* output_bins=new int[grid_res[0]*grid_res[1]];
	// MOD-BY-LEETEN 02/05/2010-FROM:
		// float error=0.01;//0.01;
	// TO:
	float error = TARGET_ERROR;//0.01;
	// MOD-BY-LEETEN 02/05/2010-END

	// MOD-BY-LEETEN 02/05/2010-FROM:
		// float target=-error*log2(error)-(1-error)*log2(1-error)+error*log2(60-1);
	// TO:
	float target=-error*log2(error)-(1-error)*log2(1-error)+error*log2(60);
	// MOD-BY-LEETEN 02/05/2010-END
printf("p(e)=%f target entorpy=%f\n",error,target);
	PointRef* m_grid = new PointRef[grid_res[0]*grid_res[1]];
	for(int iFor = 0; iFor < (grid_res[0]*grid_res[1]); iFor++)
		m_grid[iFor].push_back(GridPoint(VECTOR3(-1.0, -1.0, -1.0), VECTOR3(-1.0, -1.0, -1.0), -1, -1, -1, false));	// dummy vector

	//while(entropy>target )	
	int line_num_thres=500;//27;						
	int selected_line_num_old=0;//want to select top line_num_thres lines
	int round=0;
	std::vector<float> entropy_list;

	// MOD-BY-LEETEN 02/05/2010-FROM:
		// while(selected_line_num<line_num_thres )
	// TO:
	bool bIsConverged = false;
	for(
		bool bIsConverged = false;
		// MOD-BY-LEETEN 03/15/2010-FROM:
			// entropy > target && false == bIsConverged;
		// TO:
		// MOD-BY-LEETEN 04/01/2010-FROM:
			// false == bIsConverged;
		// TO:
		false == bIsConverged && round < MAX_NR_OF_ITERATIONS;
		// MOD-BY-LEETEN 04/01/2010-END
		// MOD-BY-LEETEN 03/15/2010-END
	)
	// MOD-BY-LEETEN 02/05/2010-END
//	while(entropy>target )
	{
		round++;
		//if( selected_line_num-selected_line_num_old>2&& selected_line_num_old>0)//no progress		
		//	break;
		//getbinnums(vectors,vectors, grid_res, my_bin_num, output_bins);//binnum=60 for 2D
		//dumphistogram(output_bins, grid_res, my_bin_num, selected_line_num);
	
		VECTOR3 next_seed;
		
		std::vector<VECTOR3> seeds;
		selectStreamlines_by_distribution(vectors,new_vectors, grid_res, x_min,x_max,y_min,y_max,occupied,hx,seeds,m_grid,entropy);

		double dwStart= GetTickCount();
		reconstruct_field_GVF_2D(new_vectors,vectors,grid_res,sl_list,donotchange);
		double elapsedTime= GetTickCount() - dwStart;
		printf("\n\n reconstruction time is %.3f milli-seconds.\n",elapsedTime); 	

		//select new seed
		selected_line_num_old=selected_line_num;
		selected_line_num+=seeds.size();
		old_entropy=entropy;
		entropy=calcRelativeEntropy6(  vectors, new_vectors,  grid_res, VECTOR3(0,0,0),VECTOR3(grid_res[0],grid_res[1],0),occupied);
		entropy_list.push_back(entropy);
		if(first==1)
		{
			firstentropy=entropy;
			first=0;
		}
		
		for(int i=0;i<seeds.size();i++)
		{
			selected_list.push_back(seeds[i]);
			seedlist.push_back(seeds[i]);
			import_pos.push_back(seeds[i]);
		}
		printf("%d seeds selected, entropy=%f/%f\n",selected_line_num, entropy,target);
	
		// ADD-BY-LEETEN 02/05/2010-BEGIN
		// MOD-BY-LEETEN 03/15/2010-FROM:
			// bIsConverged = (0 == seeds.size())?true:false;
		// TO:
		static float fPrevEntropy;
		bIsConverged = false;
		if( round > 1 )
		{
			float fEntropyRate = entropy / fPrevEntropy;
			if( 0.0f == entropy || 0.995 < fEntropyRate && fEntropyRate <= 1.0 )
				bIsConverged = true;
		}
		fPrevEntropy = entropy;
		// MOD-BY-LEETEN 03/15/2010-END

		// ADD-BY-LEETEN 02/05/2010-END

		// MOD-BY-LEETEN 02/06/2010-FROM:
			// save_streamlines_to_file();
		// TO:
		// MOD-BY-LEETEN 03/15/2010-FROM:
			// save_streamlines_to_file(sl_list);
		// TO:
		char szFilename[1024];
		// MOD-BY-LEETEN 03/16/2010-FROM:
			// sprintf(szFilename, "%s_streamlines%d.dat", g_filename, round);
		// TO:
		sprintf(szFilename, "%s_streamlines.dat", g_filename);
		// MOD-BY-LEETEN 03/16/2010-END
		save_streamlines_to_file(sl_list, szFilename);
		// MOD-BY-LEETEN 03/15/2010-END
		// MOD-BY-LEETEN 02/06/2010-END

		// ADD-BY-LEETEN 03/16/2010-BEGIN
		// output #streamlines up to current round
		FILE *fpFile;
		sprintf(szFilename, "%s_streamlines.log", g_filename);
		char szCommand[1024];
		#if	0	// MOD-BY-LEETEN 03/18/2010-FROM:
			if( 1 == round )
				sprintf(szCommand, "echo %d > %s", sl_list.size(), szFilename);
			else
				sprintf(szCommand, "echo %d >> %s", sl_list.size(), szFilename);
		#else	// MOD-BY-LEETEN 03/18/2010-TO:
		if( 1 == round )
		{
			sprintf(szCommand, "echo %d %f > %s", NR_OF_SAMPLES, hx, szFilename);
			system(szCommand);
		}
		sprintf(szCommand, "echo %d %f >> %s", sl_list.size(), entropy, szFilename);
		#endif	// MOD-BY-LEETEN 03/18/2010-END

		system(szCommand);
		// ADD-BY-LEETEN 03/16/2010-END

		//dumpEntropy(entropies,"entropy.bin");
	//	dumpSeeds(seedlist,"myseeds.seed");//crtical points excluded
	//	printf("halted, save files now\n");
	//	getchar();
//	printf("posed\n");
//	getchar();

		// ADD-BY-LEETEN 03/28/2010-BEGIN
		double dwIterationEnd = GetTickCount();
		double dwAccumulatedTime = dwIterationEnd - dwComputeStreamlineBegin;
		LOG(printf("Time up to Round %d: %.3f milli-seconds.", round, dwAccumulatedTime)); 	
		// ADD-BY-LEETEN 03/28/2010-END
	}
	
	// MOD-BY-LEETEN 02/05/2010-FROM:
		// dumpEntropy(entropy_list,"entropy.bin");
	// TO:
	static char szFilename[1024+1];
	sprintf(szFilename, "%s_entropy.bin", g_filename);
	dumpEntropy(entropy_list, szFilename);
	// MOD-BY-LEETEN 02/05/2010-END

	//dumpReconstruedField("r.vec", new_vectors, grid_res);
	//dumpSeeds(seedlist,"myseeds.seed");//crtical points excluded
	delete [] new_vectors;
	delete [] donotchange;
	delete [] occupied;
	delete [] output_bins;
	delete [] m_grid;

	// ADD-BY-LEETEN 02/05/2010-BEGIN
	#if		USE_CUDA	
	_FlowDiffusionFree();
	#endif	// #if	USE_CUDA	
	// ADD-BY-LEETEN 02/05/2010-END

	// ADD-BY-LEETEN 03/18/2010-BEGIN
	double dwComputeStreamlineEnd = GetTickCount();
	double dwComputeStreamlineTime = dwComputeStreamlineEnd - dwComputeStreamlineBegin;
	printf("\n\n Time for %s: %.3f milli-seconds.\n", __FUNCTION__, dwComputeStreamlineTime); 	
	// ADD-BY-LEETEN 03/18/2010-END
}

/*
// select one seed only
void compute_streamlines() 
{

	printf("generating seeds...\n"); 
	int num=0;
	int grid_res[3];
	float* vectors=get_grid_vec_data(grid_res);//get vec data at each grid point
	grid_res[2]=1;


												//load seeds from file
	int* donotchange=new int[grid_res[0]*grid_res[1]];
	memset(donotchange,0,sizeof(int)*grid_res[0]*grid_res[1]);
	int nSeeds; 
	//set first seed as domain center
	VECTOR3* newseed=new VECTOR3;
	newseed->Set(grid_res[0]/2, grid_res[1]/2,0);

	sl_list.clear(); 
							//combine the half lines
	
	//build quadtree
//	Tree = new QuadTree(grid_res[0]-1, grid_res[1]-1);
	Tree = new QuadTree(grid_res[0], grid_res[1]);
	printf("build quad tree\n");
	int finestlevel=16+1;//16;
//	int finestlevel=64+1;
	Tree->Build(finestlevel);


	float* new_vectors=new float[grid_res[0]*grid_res[1]*grid_res[2]*3];
	memset(new_vectors,0,sizeof(float)*grid_res[0]*grid_res[1]*grid_res[2]*3);
	
//set the boundary
	int x=0;
	int y=0;
	for(y=0;y<grid_res[1];y++)
	{
		int idx=x+y*grid_res[0];
		new_vectors[3*idx+0]=vectors[3*idx+0];
		new_vectors[3*idx+1]=vectors[3*idx+1];
	}
	x=grid_res[0]-1;
	for(y=0;y<grid_res[1];y++)
	{
		int idx=x+y*grid_res[0];
		new_vectors[3*idx+0]=vectors[3*idx+0];
		new_vectors[3*idx+1]=vectors[3*idx+1];
	}
	y=0;
	for(x=0;x<grid_res[0];x++)
	{
		int idx=x+y*grid_res[0];
		new_vectors[3*idx+0]=vectors[3*idx+0];
		new_vectors[3*idx+1]=vectors[3*idx+1];
	}
	y=grid_res[1]-1;
	for(x=0;x<grid_res[0];x++)
	{
		int idx=x+y*grid_res[0];
		new_vectors[3*idx+0]=vectors[3*idx+0];
		new_vectors[3*idx+1]=vectors[3*idx+1];
	}
//	reconstruct_field_GVF_2D(new_vectors,vectors,grid_res,sl_list,donotchange);


	int selected_line_num=0;
	float old_entropy=9999;
	float entropy=8888;
	float firstentropy=1;
	int first=1;
	std::vector<VECTOR3> seedlist;
	// DEL-BY-LEETEN 02/05/2010 srand((unsigned)time(NULL));			// initialize random number generator
	import_pos.clear();
	VECTOR3 oldseed;

	std::vector<VECTOR3> selected_list;
	std::vector<float> entropies;

	 std::vector<vtListVertex*>	Graph;
	ConstructGraph_static_2D(g_filename, Graph);

	int x_min=0,x_max=0,y_min=0,y_max=0;
	int* occupied=new int[grid_res[0]*grid_res[1]];
	memset(occupied,0,sizeof(int)*grid_res[0]*grid_res[1]);
	int line_num_thres=50;//27;											//want to select top line_num_thres lines
	
	//initial entropy
	float hx=calcEntropy( vectors,  grid_res, VECTOR3(0,0,0),VECTOR3(grid_res[0],grid_res[1],grid_res[2]));
	printf("entropy=%f\n",hx);
	int* output_bins=new int[grid_res[0]*grid_res[1]];
	float error=0.01;
	float target=-error*log2(error)-(1-error)*log2(1-error)+error*log2(60-1);
	//while(entropy>target )	
	while(selected_line_num<line_num_thres )//&& entropy>.5)
//while(old_entropy-entropy>.005*firstentropy)	
//	while(entropy/firstentropy>.1)	
	{
	
		printf("no %d seed selected = %f %f\n", selected_line_num, newseed->x(), newseed->y());

		list<vtListSeedTrace*> lines,new_lines; 
		//printf("compute streamlines..\n"); 
		osuflow->SetEntropySeedPoints( newseed,1);
		osuflow->SetIntegrationParams(.1, .5);							//small and large step size
//		osuflow->SetIntegrationParams(1, 5);							//small and large step size
		oldseed.Set(newseed->x(),newseed->y(),newseed->z());
		osuflow->GenStreamLines(lines , BACKWARD_AND_FORWARD, 500, 0);	 //maxi steps
		//printf(" done integrations\n"); 
										//clear memory
		list<vtListSeedTrace*>::iterator pIter;
	
		//Trimhalflines(lines,occupied,grid_res);
		combinehalflines(lines, new_lines,grid_res);//for display only, the streamline stops when gets too near to an existing
		if(new_lines.size())
		sl_list.push_back(*(new_lines.begin()));
		else
		{
			//ignore this block for future searching
			for(int yy=y_min; yy<y_max;yy++)
			{
				for(int xx=x_min; xx<x_min; xx++)
				{
					int idx=xx+yy*grid_res[0];
					new_vectors[idx*3+0]=vectors[idx*3+0];
					new_vectors[idx*3+1]=vectors[idx*3+1];
					new_vectors[idx*3+2]=vectors[idx*3+2];
				}
			}
		//	break;
		}
		//2. update streamline foot step
		double dwStart= GetTickCount();
		reconstruct_field_GVF_2D(new_vectors,vectors,grid_res,sl_list,donotchange);
		double elapsedTime= GetTickCount() - dwStart;
		printf("\n\n reconstruction time is %.3f milli-seconds.\n",elapsedTime); 	
	
		VECTOR3 next_seed;
	//	if(selected_line_num<line_num_thres/2)
	//	next_seed=selectNextSeed(vectors,new_vectors, grid_res, Tree);
	//	else
	//		next_seed=selectNextSeed2(vectors,new_vectors, grid_res, Tree);
	
	
		UpdateOccupied(lines,occupied,grid_res);
		//next_seed=selectNextSeed3_graph(vectors,new_vectors, grid_res, Tree,Graph,x_min,x_max,y_min,y_max,occupied,hx);
	//	next_seed=selectNextSeed3_streamline(vectors,new_vectors, grid_res, Tree,Graph,x_min,x_max,y_min,y_max,occupied,hx);
//		next_seed=selectNextSeed3_local_maximum(vectors,new_vectors, grid_res, Tree,Graph,x_min,x_max,y_min,y_max,occupied,hx);
		
		std::vector<VECTOR3> seeds;
		next_seed=selectNextSeed_by_distribution(vectors,new_vectors, grid_res, x_min,x_max,y_min,y_max,occupied,hx,seeds);
		
		
		if(next_seed.x()<0)
		{
			next_seed=selectRandomSeed( vectors, new_vectors, grid_res,Tree);
		}
		newseed->Set(next_seed.x(),next_seed.y(),0);

		if( (oldseed.x()==newseed->x() )&&
			(oldseed.y()==newseed->y() ))//selected the same seed, see what happened
		{

			//ignore this block for future searching
			for(int yy=y_min; yy<y_max;yy++)
			{
				for(int xx=x_min; xx<x_min; xx++)
				{
					int idx=xx+yy*grid_res[0];
					new_vectors[idx*3+0]=vectors[idx*3+0];
					new_vectors[idx*3+1]=vectors[idx*3+1];
					new_vectors[idx*3+2]=vectors[idx*3+2];
				}
			}
			selected_list.push_back(oldseed);
			int ijk=0;
		
			//jitter it
			float ranx = ((float)rand()/(float)RAND_MAX)*4-2;
			float rany = ((float)rand()/(float)RAND_MAX)*4-2;
			float new_x,new_y;
			new_x=newseed->x()+ranx;
			new_y=newseed->y()+rany;
			if(new_x<0)	new_x=0;
			if(new_x>grid_res[0]-1)	new_x=grid_res[0]-1;
			if(new_y<0)	new_y=0;
			if(new_y>grid_res[1]-1)	new_y=grid_res[1]-1;
			newseed->Set(	new_x, new_y, newseed->z());

		}

		selected_list.push_back(*newseed);
		//select new seed
		selected_line_num++;
		old_entropy=entropy;
		entropy=calcRelativeEntropy6(  vectors, new_vectors,  grid_res, VECTOR3(0,0,0),VECTOR3(grid_res[0],grid_res[1],0),occupied);
		entropies.push_back(entropy);
		if(first==1)
		{
			firstentropy=entropy;
			first=0;
		}
		
		seedlist.push_back(VECTOR3(next_seed.x(),next_seed.y(),0));
		import_pos.push_back(VECTOR3(next_seed.x(),next_seed.y(),0));
		printf("entropy=%f/%f\n",entropy,target);
	
	
	
	dumpEntropy(entropies,"entropy.bin");
	dumpSeeds(seedlist,"myseeds.seed");//crtical points excluded

	}
	

	dumpEntropy(entropies,"entropy.bin");
	dumpReconstruedField("r.vec", new_vectors, grid_res);
	dumpSeeds(seedlist,"myseeds.seed");//crtical points excluded
	delete [] new_vectors;
	delete [] donotchange;
	delete [] occupied;
	delete [] output_bins;
	//selectively display streamlines
}
*/


void copyFrameBuffer(char* filename, int XDim,int YDim)
{
	unsigned char* pdata=new unsigned char[XDim*YDim*3];
	glReadBuffer(GL_FRONT);

	glReadPixels(0,0,
		XDim,YDim, 
		GL_RGB, 
		GL_UNSIGNED_BYTE, 
		pdata);

	//crop it
	int xdim,ydim;
	int crop=0;//135+8;
	xdim=XDim; ydim=YDim;//-2*crop;
	unsigned char* crop_data=new unsigned char[xdim*ydim*3];
	for(int y=0; y<ydim;y++)
	{
		for(int x=0; x<xdim;x++)
		{
			crop_data[(x+y*xdim)*3+0]=pdata[(x+(y+crop)*xdim)*3+0];
			crop_data[(x+y*xdim)*3+1]=pdata[(x+(y+crop)*xdim)*3+1];
			crop_data[(x+y*xdim)*3+2]=pdata[(x+(y+crop)*xdim)*3+2];
		}
	}

	save2PPM_3_channels(filename, crop_data,xdim,ydim);
	delete [] crop_data;
	delete [] pdata;
}
void draw_grid()
{
	glPushMatrix(); 

	glColor3f(1,1,1); 

	//	for(int k=0; k<dim[2]-1;k++)
	int k=0;
	{
		for(int j=0; j<dim[1]-1;j++)  
		{
			for(int i=0; i<dim[0]-1;i++)
			{
				glBegin(GL_LINES);
				int id0,id1;
				id0=i+j*dim[0]+k*dim[0]*dim[1];
				id1=i+1+j*dim[0]+k*dim[0]*dim[1];
				glVertex3f(grid_fx[id0], grid_fy[id0], grid_fz[id0]); 
				glVertex3f(grid_fx[id1], grid_fy[id1], grid_fz[id1]); 

				id0=i+j*dim[0]+k*dim[0]*dim[1];
				id1=i+(j+1)*dim[0]+k*dim[0]*dim[1];
				glVertex3f(grid_fx[id0], grid_fy[id0], grid_fz[id0]); 
				glVertex3f(grid_fx[id1], grid_fy[id1], grid_fz[id1]); 

				id0=i+j*dim[0]+k*dim[0]*dim[1];
				id1=i+j*dim[0]+(k+1)*dim[0]*dim[1];
				glVertex3f(grid_fx[id0], grid_fy[id0], grid_fz[id0]); 
				glVertex3f(grid_fx[id1], grid_fy[id1], grid_fz[id1]); 

				glEnd();
			}
		}
	}

	glPopMatrix(); 
}

#if	0	// DEL-BY-LEETEN 01/22/2010-BEGIN
	void draw_triangulation()
	{
		//	glEnable(GL_TEXTURE_2D);
		//	if(triangles.size()==0)
		//		return;
		//if(!glIsList(triangle_list))
		{
			glPushMatrix(); 

			glScalef(1/(float)len[0], 1/(float)len[0], 1/(float)len[0]); 
			glTranslatef(-len[0]/2.0, -len[1]/2.0, -len[2]/2.0); 

			glColor4f(1.0f, 0.0f, 0.0f,0.5f);

			VECTOR3 p[3];

			int t[3]; t[0]=len[0]; t[1]=len[1]; t[2]=len[2];

			triangles.clear();
			doTriangulation(sl_list, triangles, t);


			//triangle_list=glGenLists(1);

			//glNewList(triangle_list, GL_COMPILE);


			int linez=1;

			int count=0;


			for(int i=0; i<triangles.size()/3;i++)
			{
				glBegin(GL_LINE_STRIP);
	//			glBegin(GL_TRIANGLES);


				glVertex3f(triangles[i*3+0].x(), triangles[i*3+0].y(), triangles[i*3+0].z());
				glVertex3f(triangles[i*3+1].x(), triangles[i*3+1].y(), triangles[i*3+1].z());
				glVertex3f(triangles[i*3+2].x(), triangles[i*3+2].y(), triangles[i*3+2].z());

				glEnd();
			}


			//	glEndList();
		}
		//glCallList(triangle_list);
		//	glDisable(GL_TEXTURE_2D);

		glPopMatrix(); 


	}
#endif	// DEL-BY-LEETEN 01/22/2010-END

void draw_seeds() {


	glPushMatrix(); 
	glScalef(1/(float)len[1], 1/(float)len[1], 1/(float)len[1]); 
	glTranslatef(-len[0]/2.0, -len[1]/2.0, -len[2]/2.0); 

	glColor3f(.8,0,0); 

	for(int i=0;i<import_pos.size();i++)
	{

		VECTOR3 p=import_pos[i];
		glPointSize(5);
		//glBegin(GL_POINT);
		
		//printf(" %f %f %f ", p[0], p[1], p[2]); 
		//glVertex3f(p[0], p[1], p[2]); 
		glTranslatef(p[0], p[1], p[2]); 
		glutSolidSphere(5,12,12);
		//glEnd(); 
	}

	glPopMatrix(); 

}


void get_color_entropy(float& r,float& g,float& b,float& a, VECTOR3 p,int* grid_res)
{
	int x=p.x(); int y=p.y();
	if(!entropies)
	{
		r=1;g=1;b=1;
		return;
	}
	float val=entropies[x+y*grid_res[0]];
	r=val;
	if(val<0.5)
		g=2*val;
	else
		g=2-2*val;
	b=1-val;
	a=val;
}

void draw_streamlines() {

	glEnable(GL_LINE_SMOOTH);
	glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
	glLineWidth(3);

	//	glEnable (GL_LINE_SMOOTH);
	glEnable (GL_BLEND);
	//glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	//glHint (GL_LINE_SMOOTH_HINT, GL_DONT_CARE);

	glPushMatrix(); 
/*
	//draw_cube(1,0,0);
	glColor3f(0,0,0);
	glBegin(GL_LINE_STRIP);
	glVertex3f(0.5,-0.5,0);
	glVertex3f(0.5,0.5,0);
	glVertex3f(-0.5,0.5,0);
	glVertex3f(-0.5,-0.5,0);
	glVertex3f(0.5,-0.5,0);

	glEnd();
*/	
	glScalef(1/(float)len[1], 1/(float)len[1], 1/(float)len[1]); 
	//float sfactor=100;
	//glScalef(1/sfactor, 1/sfactor, 1/sfactor); 
	glTranslatef(-len[0]/2.0, -len[1]/2.0, -len[2]/2.0); 

	//printf("draw streamlines.\n"); 
	std::list<vtListSeedTrace*>::iterator pIter; 
	
	//if(Tree)
	//	Tree->drawSelf();
	glColor4f(0,0,0,.5);
	glLineWidth(1);
	int count=0;
	int grid_res[3];
	grid_res[0]=g_xdim;
	grid_res[1]=g_ydim;
	grid_res[2]=0;
	pIter = sl_list.begin(); 
	for (; pIter!=sl_list.end(); pIter++) 
	{
		vtListSeedTrace *trace = *pIter; 
		//if(trace->size()<10)
		//	continue;
		std::list<VECTOR3*>::iterator pnIter; 
		pnIter = trace->begin(); 

	/*	float graylevel=1/((float)sl_list.size())*count-.3;
		printf("gray=%f\n",graylevel);
		if(graylevel<0)
			graylevel=0;
		glColor3f(graylevel,graylevel,graylevel);
*/
	
		VECTOR3 q;
		glBegin(GL_LINE_STRIP);
		int s=0;
		for (; pnIter!= trace->end(); pnIter++) {

			float r,g,b,a;
			VECTOR3 p = **pnIter; 
			//printf(" %f %f %f ", p[0], p[1], p[2]); 
			get_color_entropy(r,g,b,a,p,grid_res);
			//glColor4f(r,g,b,a);
			glColor3f(r,g,b);
			glVertex3f(p[0], p[1], p[2]); 
		}
		count++;

		glEnd(); 
	}
	

	for(int i=0;i<import_pos.size();i++)
	{

		VECTOR3 p=import_pos[i];
		
		//glBegin(GL_POINT);
		
		//printf(" %f %f %f ", p[0], p[1], p[2]); 
		//glVertex3f(p[0], p[1], p[2]); 
		//glutSolidSphere(1,6,6);
		VECTOR3 data;
		osuflow->GetFlowField()->at_vert((int)p[0],(int)p[1],(int)p[2],0,data);
		data.Normalize();
		
		glTranslatef(p[0], p[1], p[2]); 
		glRotatef(90,0,1,0);
		float ang=90;
		ang=atan2(data.y(),data.x())*180/3.14159;

		glRotatef(ang,-1,0,0);
	
		float r,g,b,a;
		get_color_entropy(r,g,b,a,p,grid_res);
		glColor4f(r,g,b,a);
		//glColor3f(r,g,b);		
		
		glutSolidCone(.25f,1.5f,5,2);
			
		glRotatef(-ang,-1,0,0);
		glRotatef(-90,0,1,0);
		glTranslatef(-p[0], -p[1], -p[2]); 


		//if(i>import_pos.size()*6/7 )
		{
		glRasterPos3f(p[0], p[1],p[2]);
		char str[10];
		memset(str,0,10);
		sprintf(str,"%d",i);
		char *c;
		for (c=str; *c != '\0'; c++) {
		//	glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, *c);
		}
		}


		//glEnd(); 
	}

	glDisable(GL_BLEND);

	glPopMatrix(); 

}

void animate_streamlines() {

	std::list<vtListSeedTrace*>::iterator pIter; 
	vtListSeedTrace *trace; 
	static std::list<VECTOR3*>::iterator *pnIter; 
	static int frame = 0; 

	glPushMatrix(); 

	glScalef(1/(float)len[0], 1/(float)len[0], 1/(float)len[0]); 
	glTranslatef(-len[0]/2.0, -len[1]/2.0, -len[2]/2.0); 

	glColor3f(1,1,0); 

	pIter = sl_list.begin(); 
	int num_lines = sl_list.size(); 
	printf(" animate %d streamlines\n", num_lines); 
	if (first_frame==1) {
		pnIter = new std::list<VECTOR3*>::iterator[num_lines]; 
	}
	int count = 0; 
	int max_len = 0; 
	for (; pIter!=sl_list.end(); pIter++) {
		trace = *pIter; 
		int sz = trace->size(); 
		if (sz> max_len) {
			max_len = sz;
		}
		pnIter[count] = trace->begin(); 
		count++; 
	}
	if (first_frame ==1) {
		frame = 0; 
	}
	else frame = (frame+1)%max_len; 
	printf(" *** max len = %d frame time = %d \n", max_len, frame); 

	pIter = sl_list.begin(); 

	count = 0; 
	for (; pIter!=sl_list.end(); pIter++) {
		trace = *pIter; 
		int sz = trace->size(); 
		//    if (frame >sz) {count++; continue; }
		int frame_count = 0; 
		glBegin(GL_LINE_STRIP); 
		for (; pnIter[count]!= trace->end(); pnIter[count]++) {
			VECTOR3 p = **pnIter[count]; 
			//printf(" %f %f %f ", p[0], p[1], p[2]); 
			glVertex3f(p[0], p[1], p[2]); 
			frame_count++; 
			if (frame_count > frame) break; 
		}
		glEnd(); 
		count++; 
	}
	glPopMatrix(); 
	frame++; 
	if (first_frame == 1) first_frame = 0; 
	// Sleep(.5); 
}

////////////////////////////////////////////// 


//////////////////////////////////////////////////////

void display()
{
	glEnable(GL_DEPTH_TEST); 
	glClearColor(1,1,1,1); 
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT); 

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity(); 
	gluPerspective(60, 1, .1, 100); 

	glMatrixMode(GL_MODELVIEW); 
	glLoadIdentity(); 
	gluLookAt(0,0,5,0,0,0,0,1,0); 

	glRotatef(x_angle, 0, 1,0); 
	glRotatef(y_angle, 1,0,0); 
	glScalef(scale_size, scale_size, scale_size); 

	//if (toggle_draw_streamlines == true)
	//	draw_grid();
	//draw_seeds();
	draw_streamlines(); 
	//draw_triangulation();
	// else if (toggle_animate_streamlines == true)
	// animate_streamlines(); 

	//printf(" len %f %f %f\n", len[0], len[1], len[2]); 

	glPushMatrix(); 
	//	glScalef(1/(float)len[0], 1/(float)len[0], 1/(float)len[0]); 
	glScalef(1/110,1/110,1/110); 
	glTranslatef(-len[0]/2.0, -len[1]/2.0, -len[2]/2.0); 

	glColor3f(1,1,1); 
	glBegin(GL_LINES); 
	glVertex3f(0,0,0); 
	glVertex3f(len[0],0,0); 
	glVertex3f(0,0,0);
	glVertex3f(0,len[1],0); 
	glVertex3f(0,0,0); 
	glVertex3f(0,0,1); 

	glVertex3f(len[0],len[1],0); 
	glVertex3f(len[0],0,0); 
	glVertex3f(len[0],len[1],0); 
	glVertex3f(0,len[1],0); 

	glEnd(); 

	glutSwapBuffers(); 

}

void timer(int val) {
	//printf("call idle....\n"); 
	if (toggle_animate_streamlines == true) {
		//    animate_streamlines(); 
		glutPostRedisplay(); 
	}
	glutTimerFunc(10, timer, 0); 
}

///////////////////////////////////////////////////////////

void mymouse(int button, int state, int x, int y)
{
	if (state == GLUT_DOWN) {
		press_x = x; press_y = y; 
		if (button == GLUT_LEFT_BUTTON)
			xform_mode = XFORM_ROTATE; 
		else if (button == GLUT_RIGHT_BUTTON) 
			xform_mode = XFORM_SCALE; 
	}
	else if (state == GLUT_UP) {
		xform_mode = XFORM_NONE; 
	}
}

/////////////////////////////////////////////////////////

void mymotion(int x, int y)
{
	if (xform_mode==XFORM_ROTATE) {
		x_angle += (x - press_x)/5.0; 
		if (x_angle > 180) x_angle -= 360; 
		else if (x_angle <-180) x_angle += 360; 
		press_x = x; 

		y_angle += (y - press_y)/5.0; 
		if (y_angle > 180) y_angle -= 360; 
		else if (y_angle <-180) y_angle += 360; 
		press_y = y; 
	}
	else if (xform_mode == XFORM_SCALE){
		float old_size = scale_size;
		scale_size *= (1+ (y - press_y)/60.0); 
		if (scale_size <0) scale_size = old_size; 
		press_y = y; 
	}
	glutPostRedisplay(); 
}
VECTOR3* get_line_vertices(VECTOR3* vecs,int* ver_num, int line_no,int& length)
{
	if(line_no<0)
		return 0;
	int first_ver_num=0;
	for(int i=0;i<line_no;i++)
		first_ver_num+=ver_num[i];

	VECTOR3* ret=vecs+first_ver_num;
	length=ver_num[line_no];
	return ret;
}
void Streamline_entorpy_sorting()
{
	int line_num,*ver_num,total_ver_num=0;
	FILE* fp=fopen(strcat(g_filename,"_streamline.dat"),"rb");
	fread(&line_num,sizeof(int),1,fp);
	ver_num=new int[line_num];
	fread(ver_num,sizeof(int),line_num,fp);
	for(int i=0;i<line_num;i++)
		total_ver_num+=ver_num[i];
	VECTOR3* verts=new VECTOR3[total_ver_num];
	fread(verts,sizeof(VECTOR3),total_ver_num,fp);
	fclose(fp);


	int* picked=new int[line_num];
	memset(picked,0,sizeof(int)*line_num);
	
	//start checking
	int grid_res[3];
	float* vectors=get_grid_vec_data(grid_res);//get vec data at each grid point
	grid_res[2]=1;

	g_xdim=grid_res[0]; g_ydim=grid_res[1];
												//load seeds from file
	int* donotchange=new int[grid_res[0]*grid_res[1]];
	memset(donotchange,0,sizeof(int)*grid_res[0]*grid_res[1]);
	int nSeeds; 
	sl_list.clear(); 

	float* new_vectors=new float[grid_res[0]*grid_res[1]*grid_res[2]*3];
	memset(new_vectors,0,sizeof(float)*grid_res[0]*grid_res[1]*grid_res[2]*3);

	float* new_vectors_best=new float[grid_res[0]*grid_res[1]*grid_res[2]*3];
	memset(new_vectors_best,0,sizeof(float)*grid_res[0]*grid_res[1]*grid_res[2]*3);

	float* new_vectors_prev=new float[grid_res[0]*grid_res[1]*grid_res[2]*3];
	memset(new_vectors_prev,0,sizeof(float)*grid_res[0]*grid_res[1]*grid_res[2]*3);

	
	
//set the boundary
	int x=0;
	int y=0;
	for(y=0;y<grid_res[1];y++)
	{
		int idx=x+y*grid_res[0];
		new_vectors[3*idx+0]=vectors[3*idx+0];
		new_vectors[3*idx+1]=vectors[3*idx+1];
	}
	x=grid_res[0]-1;
	for(y=0;y<grid_res[1];y++)
	{
		int idx=x+y*grid_res[0];
		new_vectors[3*idx+0]=vectors[3*idx+0];
		new_vectors[3*idx+1]=vectors[3*idx+1];
	}
	y=0;
	for(x=0;x<grid_res[0];x++)
	{
		int idx=x+y*grid_res[0];
		new_vectors[3*idx+0]=vectors[3*idx+0];
		new_vectors[3*idx+1]=vectors[3*idx+1];
	}
	y=grid_res[1]-1;
	for(x=0;x<grid_res[0];x++)
	{
		int idx=x+y*grid_res[0];
		new_vectors[3*idx+0]=vectors[3*idx+0];
		new_vectors[3*idx+1]=vectors[3*idx+1];
	}

	int selected_line_num=0;
	std::vector<VECTOR3> seedlist;
	// DEL-BY-LEETEN 02/05/2010 srand((unsigned)time(NULL));			// initialize random number generator
	VECTOR3 oldseed;

	int x_min=0,x_max=0,y_min=0,y_max=0;
	int* occupied=new int[grid_res[0]*grid_res[1]];
	memset(occupied,0,sizeof(int)*grid_res[0]*grid_res[1]);
	
	int* output_bins=new int[grid_res[0]*grid_res[1]];
	float error=0.005;//0.01;
	float target=-error*log2(error)-(1-error)*log2(1-error)+error*log2(60-1);

	memcpy(new_vectors_prev,new_vectors,sizeof(float)*grid_res[0]*grid_res[1]*grid_res[2]*3);
	memcpy(new_vectors_best,new_vectors,sizeof(float)*grid_res[0]*grid_res[1]*grid_res[2]*3);

	//while(entropy>target )	
	float old_entropy=9999;
	float tmp_old_entropy=9999;
	int line_num_thres=line_num;//27;	
	std::vector<int> selected_ids;
	std::vector<int> selected_length;
	std::vector<VECTOR3*> selected_line_verts;
	std::vector<float> min_entropy_list;
	while(selected_line_num<line_num_thres && selected_line_num<line_num)
	{
		memcpy(new_vectors_prev,new_vectors_best,sizeof(float)*grid_res[0]*grid_res[1]*grid_res[2]*3);

		VECTOR3 next_seed;
		float maxi_drop=0;
		float min_entropy=9999;
		int selected=0;
		for(int i=0;i<line_num;i++)
		{
			if(picked[i]==1)
				continue;

			double dwStart= GetTickCount();

			int length;
			VECTOR3* line_vert=	get_line_vertices(verts, ver_num,i,length);
			selected_line_verts.push_back(line_vert);
			selected_length.push_back(length);	
	
			reconstruct_field_GVF_2D_tmp(new_vectors,vectors,grid_res,selected_line_verts,selected_length,donotchange);
			selected_line_verts.pop_back();
			selected_length.pop_back();
			double elapsedTime= GetTickCount() - dwStart;
			//printf("\n\n reconstruction time is %.3f milli-seconds.\n",elapsedTime); 	
			//select new seed
			float entropy=calcRelativeEntropy6(  vectors, new_vectors,  grid_res, VECTOR3(0,0,0),VECTOR3(grid_res[0],grid_res[1],0),occupied);
			//entropies.push_back(entropy);
			if(entropy<min_entropy)
			{
				min_entropy=entropy;
				selected=i;
				memcpy(new_vectors_best,new_vectors,sizeof(float)*grid_res[0]*grid_res[1]*grid_res[2]*3);
			}
			memcpy(new_vectors,new_vectors_prev,sizeof(float)*grid_res[0]*grid_res[1]*grid_res[2]*3);
		}

		min_entropy_list.push_back(min_entropy);
		printf("%d streamline selected, entropy=%f/%f\n",selected_line_num++, min_entropy,target);
		picked[selected]=1;
		selected_ids.push_back(selected);
		int length;
		VECTOR3* line_vert=	get_line_vertices(verts, ver_num,selected,length);
		selected_line_verts.push_back(line_vert);
		selected_length.push_back(length);	
		printf("selected line=%d\n",selected);
		//memcpy(new_vectors,new_vectors_best,sizeof(float)*grid_res[0]*grid_res[1]*grid_res[2]*3);//starting point of the next round reconstruction
		
	}
	//save the selected streamlines
	selected_line_num;
	int* selected_ver_num=new int[selected_line_num];
	int total_selected_ver=0;
	for(int i=0;i<selected_line_num;i++)
	{
		selected_ver_num[i]=ver_num[selected_ids[i]];
		total_selected_ver+=selected_ver_num[i];
	}
	VECTOR3* selected_verts=new VECTOR3[total_selected_ver];
	int start=0;
	for(int i=0;i<selected_line_num;i++)
	{
		int length;
		
		int line=selected_ids[i];
		VECTOR3* ret=get_line_vertices(verts, ver_num,line,length);
		for(int j=0;j<length;j++)
			selected_verts[start+j]=ret[j];
		start+=length;

	}
	fp=fopen(strcat(g_filename,"streamlines_selected.dat"),"wb");
	fwrite(&selected_line_num,sizeof(int),1,fp);
	fwrite(selected_ver_num,sizeof(int),selected_line_num,fp);
	fwrite(selected_verts,sizeof(VECTOR3),total_selected_ver,fp);
	fclose(fp);

	fp=fopen(strcat(g_filename,"order.dat"),"wb");
	int size=selected_ids.size();
	fwrite(&size,sizeof(int),1,fp);
	for(int i=0;i<selected_ids.size();i++)
		fwrite(&(selected_ids[i]),sizeof(int),1,fp);
	fclose(fp);

	fp=fopen(strcat(g_filename,"entropy_drop.dat"),"wb");
	size=min_entropy_list.size();
	fwrite(&size,sizeof(int),1,fp);
	for(int i=0;i<min_entropy_list.size();i++)
		fwrite(&(min_entropy_list[i]),sizeof(float),1,fp);
	fclose(fp);

	printf("streamline sorting is done\n");
	delete [] selected_ver_num;
	delete [] selected_verts;
	delete [] ver_num;
	delete [] verts;
	delete [] picked;
	delete [] new_vectors_best;
	delete [] new_vectors;
	delete [] new_vectors_prev;
	delete [] vectors;
	delete [] donotchange;
}

void Streamline_entorpy_calculation_loadfile()
{
	int line_num,*ver_num,total_ver_num=0;
//	FILE* fp=fopen(strcat(g_filename,"_streamline.dat"),"rb");
	FILE* fp=fopen("streamlines.dat","rb");
	fread(&line_num,sizeof(int),1,fp);
	ver_num=new int[line_num];
	fread(ver_num,sizeof(int),line_num,fp);
	for(int i=0;i<line_num;i++)
		total_ver_num+=ver_num[i];
	VECTOR3* verts=new VECTOR3[total_ver_num];
	fread(verts,sizeof(VECTOR3),total_ver_num,fp);
	fclose(fp);


	//start checking
	int grid_res[3];
	float* vectors=get_grid_vec_data(grid_res);//get vec data at each grid point
	grid_res[2]=1;

	g_xdim=grid_res[0]; g_ydim=grid_res[1];
												//load seeds from file
	int* donotchange=new int[grid_res[0]*grid_res[1]];
	memset(donotchange,0,sizeof(int)*grid_res[0]*grid_res[1]);
	int nSeeds; 
	sl_list.clear(); 

	float* new_vectors=new float[grid_res[0]*grid_res[1]*grid_res[2]*3];
	memset(new_vectors,0,sizeof(float)*grid_res[0]*grid_res[1]*grid_res[2]*3);

//set the boundary
	int x=0;
	int y=0;
	for(y=0;y<grid_res[1];y++)
	{
		int idx=x+y*grid_res[0];
		new_vectors[3*idx+0]=vectors[3*idx+0];
		new_vectors[3*idx+1]=vectors[3*idx+1];
	}
	x=grid_res[0]-1;
	for(y=0;y<grid_res[1];y++)
	{
		int idx=x+y*grid_res[0];
		new_vectors[3*idx+0]=vectors[3*idx+0];
		new_vectors[3*idx+1]=vectors[3*idx+1];
	}
	y=0;
	for(x=0;x<grid_res[0];x++)
	{
		int idx=x+y*grid_res[0];
		new_vectors[3*idx+0]=vectors[3*idx+0];
		new_vectors[3*idx+1]=vectors[3*idx+1];
	}
	y=grid_res[1]-1;
	for(x=0;x<grid_res[0];x++)
	{
		int idx=x+y*grid_res[0];
		new_vectors[3*idx+0]=vectors[3*idx+0];
		new_vectors[3*idx+1]=vectors[3*idx+1];
	}

	int selected_line_num=0;
	std::vector<VECTOR3> seedlist;
	// DEL-BY-LEETEN 02/05/2010 srand((unsigned)time(NULL));			// initialize random number generator
	VECTOR3 oldseed;

	int x_min=0,x_max=0,y_min=0,y_max=0;
	int* occupied=new int[grid_res[0]*grid_res[1]];
	memset(occupied,0,sizeof(int)*grid_res[0]*grid_res[1]);
	
	int* output_bins=new int[grid_res[0]*grid_res[1]];
	float error=0.005;//0.01;
	float target=-error*log2(error)-(1-error)*log2(1-error)+error*log2(60-1);


	//while(entropy>target )	
	float old_entropy=9999;
	float tmp_old_entropy=9999;
	int line_num_thres=line_num;//27;	
	std::vector<float> entropies;
	
	
	VECTOR3 next_seed;
	float maxi_drop=0;
	float min_entropy=9999;
	int selected=0;
	std::vector<int> length_list;
	std::vector<VECTOR3*> line_vert_list;	
	for(int i=0;i<line_num;i++)
	{

		double dwStart= GetTickCount();


		int length;
		VECTOR3* line_vert=	get_line_vertices(verts, ver_num,i,length);
		line_vert_list.push_back(line_vert);
		length_list.push_back(length);

		reconstruct_field_GVF_2D_tmp(new_vectors,vectors,grid_res,line_vert_list,length_list,donotchange);
		double elapsedTime= GetTickCount() - dwStart;
		float entropy=calcRelativeEntropy6(  vectors, new_vectors,  grid_res, VECTOR3(0,0,0),VECTOR3(grid_res[0],grid_res[1],0),occupied);
		entropies.push_back(entropy);
		printf("%d streamline processed, entropy=%f\n",i,entropy);
	}
	
	
	fp=fopen(strcat(g_filename,"entropy_drop_ori.dat"),"wb");
	int size=entropies.size();
	fwrite(&size,sizeof(int),1,fp);
	for(int i=0;i<entropies.size();i++)
		fwrite(&(entropies[i]),sizeof(float),1,fp);
	fclose(fp);

	//printf("streamline sorting is done\n");

	delete [] ver_num;
	delete [] verts;
	delete [] new_vectors;
	delete [] vectors;
	delete [] donotchange;
}
///////////////////////////////////////////////////////////////

void mykey(unsigned char key, int x, int y)
{
	switch(key) {
	case 'q': exit(1);
		break; 
	case 's': 
		compute_streamlines(); 
		glutPostRedisplay(); 
		break; 
	case 'r': 
		compute_streamlines_random();
		glutPostRedisplay(); 
		break; 
	case 'd': 
		toggle_draw_streamlines = !toggle_draw_streamlines; 
		toggle_animate_streamlines = false; 

		break; 
	case'a': 
		toggle_animate_streamlines = !toggle_animate_streamlines; 
		toggle_draw_streamlines = false; 
		first_frame = 1; 
		break;
	case 'w':
		copyFrameBuffer(strcat(g_filename,".ppm"),winWidth,winHeight);
		break;
	case 'l':
		compute_streamlines_load_file_entropy();
		break;
	case 'o':
		compute_streamlines_load_file();
		break;
	case 'p':
		//compute_entropy_point_by_point_load_file() ;
		Streamline_entorpy_sorting();
		break;
	case 'c':
		Streamline_entorpy_calculation_loadfile();
		break;

	}
}
///////////////////////////////////////////////////////////////


void convert()
{
	//convert from little endian to large endian
	int xdim, ydim,zdim;
	unsigned char s[4],c[4];
	FILE* fI = fopen("blunt.soln", "rb");
	FILE* fo = fopen("blunt_conv.soln", "wb");

	int num=1;
	while(num!=0)
	{
		num=fread(s,sizeof(unsigned char),4,fI);
		c[0]=s[3]; 	c[1]=s[2];
		c[2]=s[1]; 	c[3]=s[0];
		fwrite(c,sizeof(unsigned char),4,fo);
	}
	fclose(fI);
	fclose(fo);
}

void readGrid()
{
	FILE* fIn = fopen("shuttle_conv.grid", "rb");


	fread(&dim[0], sizeof(int), 1, fIn);
	fread(&dim[1], sizeof(int), 1, fIn);
	fread(&dim[2], sizeof(int), 1, fIn);

	int nodeNum=dim[0]*dim[1]*dim[2];

	grid_fx = (float *)malloc(sizeof(float) * nodeNum);
	grid_fy = (float *)malloc(sizeof(float) * nodeNum);
	grid_fz = (float *)malloc(sizeof(float) * nodeNum);


	fread(grid_fx, sizeof(float), nodeNum, fIn);					// X for nodes

	fread(grid_fy, sizeof(float), nodeNum, fIn);					// Y for nodes

	fread(grid_fz, sizeof(float), nodeNum, fIn);					// Z for nodes
	/*
	//get data to test locate function
	VECTOR3 cell; cell[0]=10; cell[1]=10;cell[2]=10;
	int celldim[3];
	float avg[3];

	for(int i=0; i<3; i++)
	{
	celldim[i]=dim[i]-1;
	avg[i]=0;
	}


	for(int k=0; k<2;k++)
	{
	for(int j=0; j<2; j++)
	{
	for(int i=0; i<2; i++)
	{
	int id[3];
	id[0]=cell[0]+i;
	id[1]=cell[1]+j;
	id[2]=cell[2]+k;
	int idx=id[0]+id[1]*celldim[0]+id[2]*celldim[0]*celldim[1];

	avg[0]+=grid_fx[idx];
	avg[1]+=grid_fy[idx];
	avg[2]+=grid_fz[idx];


	}
	}
	}

	for(int i=0; i<3; i++)
	avg[i]/=8;
	*/
	fclose(fIn);
}
/*
void batchconvert()
{
char sz[255];
for(int i=0; i<=36;i++)
{
memset(sz,0,255);
sprintf(sz,"Time%.2d.vec",i);
FILE* fp = fopen(sz, "rb"); //2d data

if (fp == NULL) return; 

int grid_res[3]; 

int num=fread(grid_res, sizeof(int), 2, fp); 
XDim=grid_res[0];YDim=grid_res[1];grid_res[2]=1;ZDim=1;//grid_res[2]+1;

vectors = new float[grid_res[0]*grid_res[1]*grid_res[2]*2]; 
num=fread(vectors,sizeof(float),grid_res[0]*grid_res[1]*grid_res[2]*2,fp);

fclose(fp);

float* tmpvec=new float[grid_res[0]*grid_res[1]*3];
for(int y=0; y<grid_res[1];y++)
{
for(int x=0; x<grid_res[0];x++)
{
int idx=x+y*grid_res[0];
tmpvec[idx*3+0]=vectors[idx*2+0];
tmpvec[idx*3+1]=vectors[idx*2+1];
tmpvec[idx*3+2]=0;
}
}
memset(sz,0,255);
sprintf(sz,"Time%.2d_fake_3D.vec",i);
fopen(sz,"wb");
grid_res[2]=2;
fwrite(grid_res,sizeof(int),3,fp);
fwrite(tmpvec,sizeof(float),XDim*YDim*3,fp);
fwrite(tmpvec,sizeof(float),XDim*YDim*3,fp);
fclose(fp);
delete [] tmpvec;
}

}

*/

void ascii2bin(char* filename)
{
	char sz[255];
	//for(int i=1; i<=16;i++)
	{
		memset(sz,0,255);
//		sprintf(sz,".\\data_farthest\\%d.vec",i);
		FILE* fp = fopen(filename, "rt"); //2d data

		if (fp == NULL) return; 

		int grid_res[3]; 
		grid_res[2]=2;
		fscanf(fp,"%d %d", &grid_res[0],&grid_res[1]);
		float* vectors;
		vectors = new float[grid_res[0]*grid_res[1]*3]; 
		for(int j=0; j<grid_res[0]*grid_res[1];j++)
		{
			float fu,fv;
			fscanf(fp,"%f %f", &fu,&fv);
			vectors[j*3]=fu;
			vectors[j*3+1]=fv;
			vectors[j*3+2]=0;
			
		}
		fclose(fp);
		
		memset(sz,0,255);
		//sprintf(sz,"bin.vec",i);
		fp=fopen("bin.vec","wb");
		fwrite(grid_res,sizeof(int),3,fp);
		fwrite(vectors,sizeof(float),grid_res[0]*grid_res[1]*3,fp);
		fwrite(vectors,sizeof(float),grid_res[0]*grid_res[1]*3,fp);

		fclose(fp);

		delete [] vectors;

	}
}

void bin2ascii()
{
	char sz[255];
	for(int i=0;i<37;i++)
	{
		char sz[200];
		memset(sz,0,sizeof(char)*200);
		sprintf(sz,"cropped%.2d.vec",i);
	//for(int i=1; i<=30;i++)
//	{
//		memset(sz,0,255);
//		sprintf(sz,"isabel20_z%d.vec",i);
//		sprintf(sz,"%s","plume_y_half.vec");
		
		FILE* fp = fopen(sz, "rb"); //2d data
		if (fp == NULL) return; 

		int grid_res[3]; 
		fread(grid_res,sizeof(int),3,fp);
		float* vectors;
		vectors = new float[grid_res[0]*grid_res[1]*3]; 
		fread(vectors,sizeof(float),grid_res[0]*grid_res[1]*3,fp);
		fclose(fp);
		
		memset(sz,0,255);
		//sprintf(sz,"isabel20_z%d_ascii.vec",i);
	//	sprintf(sz,"%s","plume_y_half_ascii.vec");
		sprintf(sz,"cropped%.2d_ascii.vec",i);
		fp=fopen(sz,"wt");
		fprintf(fp,"%d %d\n",grid_res[1],grid_res[0]);
		for(int j=0; j<grid_res[0]*grid_res[1];j++)
		fprintf(fp,"%f %f ",vectors[j*3+1],vectors[j*3+0]);
		fclose(fp);

	
		delete [] vectors;

	}
}

void swapxy(char* filename)
{
		FILE* fp = fopen(filename, "rb"); //2d data
		if (fp == NULL) return; 

		int grid_res[3]; 
		fread(grid_res,sizeof(int),3,fp);
		float* vectors;
		vectors = new float[grid_res[0]*grid_res[1]*3]; 
		fread(vectors,sizeof(float),grid_res[0]*grid_res[1]*3,fp);
		fclose(fp);
		
		for(int y=0;y<grid_res[1];y++)
		{
			for(int x=0; x<grid_res[0];x++)
			{
				int idx=x+y*grid_res[0];
				float tmp=vectors[3*idx+0];
				vectors[3*idx+0]=vectors[3*idx+1];
				vectors[3*idx+1]=tmp;
			}
		}

		fp = fopen("swapped.vec", "wb"); //2d data

		fwrite(grid_res,sizeof(int), 3,fp);
		fwrite(vectors,sizeof(float),grid_res[0]*grid_res[1]*3,fp);
		fwrite(vectors,sizeof(float),grid_res[0]*grid_res[1]*3,fp);
		fclose(fp);

	
		delete [] vectors;

	
}

float* enlarge_dataset(OSUFlow* osuflow, int times)//get vec data at each grid point
{
	int grid_res[3],grid_res_new[3];
	osuflow->GetFlowField()->getDimension(grid_res[0],grid_res[1],grid_res[2]);

	grid_res_new[0]=grid_res[0]*times;
	grid_res_new[1]=grid_res[1]*times;
	grid_res_new[2]=grid_res[2]*times;

	float * vectors=new float[grid_res_new[0]*grid_res_new[1]*grid_res_new[2]*3];
	float step=1.0/((float)times);
	for(int k=0; k<grid_res[2]-1;k++)
	{
		for(int j=0; j<grid_res[1]-1;j++)
		{
			for(int i=0; i<grid_res[0]-1;i++)
			{
				
				for(int ti=0;ti<times; ti++)
				{
					VECTOR3 data,pos;
					pos.Set(i+0*step,j+0*step,k+0*step);
					osuflow->GetFlowField()->at_phys(pos,0,data);//t=0, static data
					int idx=(i*times+ti)+(j*times+ti)*grid_res[0]+(k*times+ti)*grid_res[0]*grid_res[1];

					data.Normalize();
					vectors[idx*3+0]=data.x();
					vectors[idx*3+1]=data.y();
					vectors[idx*3+2]=data.z();
				}
			}
		}
	}
	FILE* fp=fopen("larger.vec","wb");
	fwrite(grid_res_new,sizeof(int),3,fp);
	fwrite(vectors,sizeof(float),3*grid_res_new[0]*grid_res_new[1]*grid_res_new[2],fp);
	fclose(fp);
	return vectors;
}
void crop_data_set()
{
	for(int i=0;i<37;i++)
	{
		char sz[200];
		memset(sz,0,sizeof(char)*200);
		sprintf(sz,"Time%.2d_fake_3D.vec",i);
		FILE* fp = fopen(sz, "rb"); //2d data
		if (fp == NULL) return; 

		int grid_res[3],new_grid_res[3];

		fread(grid_res,sizeof(int),3,fp);
		float* vectors,*new_vectors;
		vectors = new float[grid_res[0]*grid_res[1]*3]; 
		fread(vectors,sizeof(float),grid_res[0]*grid_res[1]*3,fp);
		fclose(fp);
		
		int crop[4];
		crop[0]=5;
		crop[1]=250;
		crop[2]=5;
		crop[3]=105;
		new_grid_res[0]=grid_res[0]-(crop[0]+crop[1]);
		new_grid_res[1]=grid_res[1]-(crop[2]+crop[3]);
		new_grid_res[2]=grid_res[2];
	
		new_vectors=new float[new_grid_res[0]*new_grid_res[1]*3]; 
		int idx2=0;
		for(int y=crop[2];y<grid_res[1]-crop[3];y++)
		{
			for(int x=crop[0]; x<grid_res[0]-crop[1];x++)
			{
				int idx1=x+y*grid_res[0];
				new_vectors[3*idx2+0]=vectors[3*idx1+0];
				new_vectors[3*idx2+1]=vectors[3*idx1+1];
				new_vectors[3*idx2+2]=vectors[3*idx1+2];
				idx2++;
			}
		}

		memset(sz,0,sizeof(char)*200);
		sprintf(sz,"cropped%.2d.vec",i);		
		fp = fopen(sz, "wb"); //2d data

		fwrite(new_grid_res,sizeof(int), 3,fp);
		fwrite(new_vectors,sizeof(float),new_grid_res[0]*new_grid_res[1]*3,fp);
		fwrite(new_vectors,sizeof(float),new_grid_res[0]*new_grid_res[1]*3,fp);
		fclose(fp);

		printf("crop %d done\n",i);
	
		delete [] vectors;
	}
printf("crop all done\n");
getchar();
}


int main(int argc, char** argv) 
{
//	crop_data_set();
	//convert();
//	 bin2ascii();
//	 swapxy(argv[1]);
	 //printf("done\n");
//getchar();

	//ascii2bin("13-larger.vec");

	VECTOR3 minB, maxB; 

	//readGrid();

	osuflow = new OSUFlow(); 
	printf("read file %s\n", argv[1]); 
	g_filename=argv[1];
	minB[0] = 0; minB[1] = 0; minB[2] = 0; 
	maxB[0] = 202; maxB[1] = 202; maxB[2] = 2;  
	osuflow->LoadData((const char*)argv[1], true);//, minB, maxB); //true: a steady flow field 
/*
	int test[3];
float* tmp=get_grid_vec_data(test);//get vec data at each grid point
FILE* my=fopen("norm.vec","wb");
fwrite(test,sizeof(int),3,my);
fwrite(tmp,sizeof(float),test[0]*test[1]*test[2]*3,my);
fclose(my);
	delete tmp;
	*/
	//int times=2;
	//float* tmp=enlarge_dataset( osuflow, times);//enlarge  vec data 
	//delete [] tmp;

	//  osuflow->LoadData((const char*)argv[1], true); //true: a steady flow field 
	osuflow->Boundary(minLen, maxLen); // get the boundary 
	minB[0] = minLen[0]; minB[1] = minLen[1];  minB[2] = minLen[2];
	maxB[0] = maxLen[0]; maxB[1] = maxLen[1];  maxB[2] = maxLen[2];
	osuflow->SetBoundary(minB, maxB);  // set the boundary. just to test
	// the subsetting feature of OSUFlow
	printf(" volume boundary X: [%f %f] Y: [%f %f] Z: [%f %f]\n", 
		minLen[0], maxLen[0], minLen[1], maxLen[1], 
		minLen[2], maxLen[2]); 

	center[0] = (minLen[0]+maxLen[0])/2.0; 
	center[1] = (minLen[1]+maxLen[1])/2.0; 
	center[2] = (minLen[2]+maxLen[2])/2.0; 
	printf("center is at %f %f %f \n", center[0], center[1], center[2]); 
	len[0] = maxLen[0]-minLen[0]; 
	len[1] = maxLen[1]-minLen[1]; 
	len[2] = maxLen[2]-minLen[2]; 

	int xdim,ydim,zdim;
	osuflow->GetFlowField()->getDimension(xdim,ydim,zdim);
	//	reseeding(xdim, ydim,"Time00_fake_3D.vec.data", "Time02_fake_3D.vec.data","seed1.data","seed2.data");

	glutInit(&argc, argv); 
	glutInitDisplayMode(GLUT_RGB|GLUT_DOUBLE|GLUT_DEPTH); 
	glutInitWindowSize(winWidth,winHeight); 

	glutCreateWindow("Display streamlines"); 
	glutDisplayFunc(display); 
	//  glutIdleFunc(idle); 
	glutTimerFunc(10, timer, 0); 
	glutMouseFunc(mymouse); 
	glutMotionFunc(mymotion);
	glutKeyboardFunc(mykey); 
	// ADD-BY-LEETEN 03/10/2010-BEGIN
	#if	ENTER_GLUT_LOOP	
	// ADD-BY-LEETEN 03/10/2010-END
	glutMainLoop(); 
	// ADD-BY-LEETEN 03/10/2010-BEGIN
	#else
	compute_streamlines();
	#endif
	// ADD-BY-LEETEN 03/10/2010-END
	return 0;
}

/*

$Log: not supported by cvs2svn $
Revision 1.12  2010/03/29 04:21:09  leeten

[03/28/2010]
1. [MOD] Redirect the output of y indices to stderr.
2. [ADD] Dump the current round and the accumulated execution time.

Revision 1.11  2010/03/26 14:55:32  leeten

[03/26/2010]
1. [MOD] Change the implementation of the function FSampleFrom() to clamp the range of the sampled coordinate.
2. [MOD] Alter the radius of the samples in the circular kernel.
3. [MOD] Change the name of the variable bLocalMax to bIsLocalMax.
4. [ADD] Add the implemetnation of a new kernel shape KERNEL_SHAPE_DIAMOND.
5. [MOD] When using rejection-based importance sample, begin the sample from the index iNrOfSampledPoints.
6. [ADD] Pass fMAxProb to the function combinehalflines_check_stop().

Revision 1.10  2010/03/24 16:16:14  leeten

[03/23/2010]
1. [ADD] Add a center in the circular kernel for local max.
2. [ADD] Sort the seeds by their importances.
3. [MOD] Specify the line integral step size by two preprocessors STREAMLINE_INIT_STEP_SIZE and STREAMLINE_MAX_STEP_SIZE.

Revision 1.9  2010/03/23 19:39:21  leeten

[03/23/2010]
1. [MOD] Change the type of the seed coordinate from integers to floating points.
2. [MOD] Change the sampling so if the local max will be only detected in the first round of the sampling, and there is no importance sampling in the 1st round.
3. [MOD] Specify the kernel near the local max by the preprocessor KERNEL_SHAPE(KERNEL_SHAPE_SQUARE or KERNEL_SHAPE_CIRCLE).
4. [ADD] Dump the seed location into a file.

Revision 1.8  2010/03/22 13:50:30  leeten

[03/22/2010]
1. [MOD] Use the new preprocessor KERNEL_SIZE_AROUND_CRITICAL_POINT to decide the kernel size.
2. [ADD] Only consider the local max. whose prob is larger than LOCAL_MAX_THRESHOLD times max. prob.
3. [MOD] Do not make the pool of seeds exceed the allocated buffer.

Revision 1.7  2010/03/19 19:45:27  leeten

[03/19/2010]
1. [ADD] Place 9 seeds around the critical point before importance sampling.
2. [MOD] Pass the normalized entropy field as one parameter to the function combinehalflines_check_stop().

Revision 1.6  2010/03/18 16:13:50  leeten

[03/18/2010]
1. [MOD] Fix the variable name fErrorImage to pfErrorImage.
2. [ADD] When the preprocessor PRUNING_MODE is PRUNING_MODE_KEEP_WHEN_DISTANT, call the function combinehalflines_check_stop().
3. [MOD] Change the preprocessor ENABLE_PRUNING to PRUNING_MODE_COND_ENTROPY.
4. [ADD] Compute the timing for compute_streamlines().
5. [ADD] Output #seeds.round and initial entriopy to the 1st line of the log.

Revision 1.5  2010/03/16 15:44:52  leeten

[03/16/2010]
1. [ADD] Define a new function IWrapCoord to wrapt the field around the boundary.
2. [ADD] Disable the pruning when the preprocessor ENABLE_PRUNING is 0.
3. [ADD] Define a new function _DumpField2D to dump the specified field to a file.
4. [ADD] Dump the vector disaprity to files.
5. [MOD] Dump the rcond. entropy as its raw format other than PPM images.
6. [MOD] Change the max. of cond. entropy to log2(binnum).
7. [MOD] When convert the entropy into image, change the condition if(data[i]<(255/2)) to if(data[i]<=(255/2)) to avoid discontinuity.
8. [MOD] Output the streamlines to the same file, and output the accoumated numbers of streamlines to a log.

Revision 1.4  2010/03/15 18:58:48  leeten

[03/15/2010]
1. [MOD] Use the new preprocess KERNEL_SIZE to define the size of the neighborhood.
2. [MOD] Use the new preprocess NR_OF_BINS to define #histogram bins.
3. [DEL] Do not take the 2nd power of the entropy when build the pdf.
4. [ADD] Add the implmentation of rejection method for imporance sampling. This method will be used when the preprocessor IMPORTANCE_SAMPLING is equal to IMPORTANCE_SAMPLING_REJECTION_METHOD.
5. [MOD] Do not use the target entropy as the stop criteria. Instead, the algorithm stops when the cond. entropy converges.
6. [MOD] Add the round number to the end of the filenames for the streamlines.

Revision 1.3  2010/03/10 20:24:39  leeten

[03/10/2010]
1. [ADD] If the preprocessor ENTER_GLUT_LOOP is zero, immediate call compute_streamlines().

Revision 1.2  2010/02/09 04:15:02  leeten

[02/02/2010]
1. [ADD] Include the header 'FlowDiffusion2DConfig.h' and 'liblog.h.'
2. [MOD] Modify the function save_streamlines_to_file in order to save the specify set of streamlines to a given file.
3. [MOD] Control #sample by the preprocessor NR_OF_SAMPLES.
4. [MOD] Specify the advent length to 200 if the preprocessor GENERATE_PRIMITIVE is GENERATE_PRIMITIVE_STREAMLINES.
5. [MOD] Control the pruning threshold by the preprocessor PRUNING_THRESHOLD.
6. [MOD] Call the function combinehalflines() instead.
7. [ADD] Call the function discardredundantstreamlines().
8. [DEL] Comment srand().
9. [MOD] Specify the target error by the threshold TARGET_THRESHOLD.
10. [ADD] Stop streamline generation if no more seeds can be generated.
11. [MOD] Save the entropy to the folder of the input vector field,.

Revision 1.1  2010/01/22 21:09:12  leeten

[01/22/2010]
1. [1ST] First time checkin.


*/
