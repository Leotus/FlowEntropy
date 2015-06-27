////////////////////////////////////////////////////////
//
// 3D sample program
//
// Han-Wei Shen
//
////////////////////////////////////////////////////////
#include "entropy.h"

#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <GL/glew.h>
#include <GL/glu.h>

#include <GL/glut.h> 

#include "OSUFlowEntropy.h"

#include <list>
#include <iterator>

#include "FlowDiffusion3DConfig.h"	

int binnum=360;
float minstepsize=.5;
float maxstepsize=1;
int maxi_stepnum=500;
float* entropies=0;

float* g_vectors=0;
int g_grid_res[3];
float* g_new_vectors=0;
float* entropy_every_point=0;
//quatative
float* xyz=0;
float * theta=0;
float * phi  =0;
int my_vec3_location=0;
char* g_filename;
int winWidth=1000;
int winHeight=1000;
int press_x, press_y; 
int release_x, release_y; 
float x_angle = 0.0; 
float y_angle = 0.0; 
float scale_size = SCALE_SIZE; 

int toggle_entropy=1;
int xform_mode = 0; 

#define XFORM_NONE    0 
#define XFORM_ROTATE  1
#define XFORM_SCALE 2 

OSUFlowEntropy *osuflow;
VECTOR3 minLen, maxLen; 
list<vtListSeedTrace*> sl_list; 
list<vtListSeedTrace*> show_list; 
std::vector<VECTOR3> seed_list;
GLuint	triangle_list;
std::vector<VECTOR3> leaves;
std::vector<float> entropy_leaves;

bool toggle_draw_streamlines = false; 
bool toggle_animate_streamlines = false; 
float center[3], len[3]; 
int first_frame = 1; 

//draw grid
int dim[3];
float * grid_fx, *grid_fy, *grid_fz;
vector<VECTOR3> tetrahedras;
vector<VECTOR3> sphere_mesh;

typedef GLdouble TMatrix[16];

void normalized_entropy(int* grid_res,float* values)
{
	if(!values)
		return;

	int maxv=0;
	for(int z=0;z<grid_res[2];z++)
	for(int y=0;y<grid_res[1];y++)
	for(int x=0;x<grid_res[0];x++)
	{
		int idx=x+y*grid_res[0]+z*grid_res[0]*grid_res[1];
		if(values[idx]>maxv)
			maxv=values[idx];

	}
	if(maxv==0)
		return;

	for(int z=0;z<grid_res[2];z++)
	for(int y=0;y<grid_res[1];y++)
	for(int x=0;x<grid_res[0];x++)
	{
		int idx=x+y*grid_res[0]+z*grid_res[0]*grid_res[1];
		values[idx]=values[idx]/maxv;
	}
	
}
void get_color_entropy(float& r,float& g,float& b,float& a,VECTOR3 p,int* grid_res)
{
	if(!entropies)
		return;
	int x=p.x(); int y=p.y(); int z=p.z();
	int idx=x+y*grid_res[0]+z*grid_res[0]*grid_res[1];
	float val=entropies[idx];
	r=val;
	if(val<0.5)
		g=2*val;
	else
		g=2-2*val;
	b=1-val;
	a=val;

}
//load seeds
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
				osuflow->GetFlowField()->at_vert(i,j,k,0,data);//t=0, static data
				int idx=i+j*grid_res[0]+k*grid_res[0]*grid_res[1];

				data.Normalize();
				vectors[idx*3+0]=data.x();
				vectors[idx*3+1]=data.y();
				vectors[idx*3+2]=data.z();
			}
		}
	}
	return vectors;
}
void save_streamlines_to_file_hand_tuning(char* filename, std::vector<int> line_color,int nseeds)
{
	if(!entropies)
		return;
	int grid_res[3];
//	float* vectors;
//	FILE* fp=fopen(g_filename,"rb");
//	fread(grid_res,sizeof(int),3,fp);

//	vectors=new float[grid_res[0]*grid_res[1]*grid_res[2]*3];
//	fread(vectors,sizeof(float),grid_res[0]*grid_res[1]*grid_res[2]*3,fp);
//	fclose(fp);


	float* vectors=get_grid_vec_data(grid_res);//get vec data at each grid point

	int line_num=sl_list.size();
	int* ver_num=new int[line_num];


	int line_id=0;
	int total_ver_num=0;
	std::list<vtListSeedTrace*>::iterator pIter; 

	
	
	
	//get ver num for each line
	pIter = sl_list.begin(); 
	for (; pIter!=sl_list.end(); pIter++) {
		vtListSeedTrace *trace = *pIter; 
		ver_num[line_id]=trace->size();
		total_ver_num=total_ver_num+trace->size();
		line_id++;
	}
	//write the vertices
	VECTOR3* ver=new VECTOR3[total_ver_num];
	float*   mag=new float[total_ver_num];
	float*   alpha=new float[total_ver_num];
	//RGBA COLOR FOR EACH VER
	VECTOR4* rgba=new VECTOR4[total_ver_num];

	float totalf=sl_list.size();
	int count=0;
	int line_no=0;

	normalized_entropy(grid_res,entropies);

	pIter = sl_list.begin(); 
	for (; pIter!=sl_list.end(); pIter++) {
		vtListSeedTrace *trace = *pIter; 
		std::list<VECTOR3*>::iterator pnIter; 
		pnIter = trace->begin(); 

		VECTOR3 q;
		for (; pnIter!= trace->end(); pnIter++) {
			VECTOR3 p = **pnIter; 
			ver[count].Set(p[0], p[1], p[2]); 
			float r,g,b,a;
			{
				//get_line_color(line_color[line_no],r,g,b,a,(float)nseeds);
				get_color_entropy(r,g,b,a,p,grid_res);
				
				//int idx=(int)p[0]+((int)p[1])*grid_res[0]+((int)p[2])*grid_res[0]*grid_res[1];
				//r=(1+vectors[idx*3+0])/2;
				//g=(1+vectors[idx*3+1])/2;
				//b=(1+vectors[idx*3+2])/2;

				rgba[count].Set(r,g,b,a);
				
			}

			count++;
		}
		line_no++;
	}

	FILE* fp=fopen(filename,"wb");
	//fp=fopen("F:\\xul\\illuminite lines\\examples\\viewer\\data\\velocity.dat","wb");
	
	fwrite(&line_num,sizeof(int),1,fp);
	fwrite(ver_num,sizeof(int),line_num,fp);
	fwrite(ver,sizeof(VECTOR3),total_ver_num,fp);
	fwrite(rgba,sizeof(VECTOR4),total_ver_num,fp);
	fclose(fp);
	delete [] ver;
	delete [] ver_num;
	delete [] rgba;
	delete [] mag;
	delete [] vectors;
	delete [] alpha;
//	delete [] dist;

}

typedef std::vector<int> vtListVertex;

void compute_streamlines_load_file() {

	float from[3], to[3]; 

	from[0] = minLen[0];   from[1] = minLen[1];   from[2] = minLen[2]; 
	to[0] = maxLen[0];   to[1] = maxLen[1];   to[2] = maxLen[2]; 

	printf("loading seeds...\n"); 
	int nSeeds; 
	//VECTOR3* seeds=loadSeedandRender(strcat(g_filename,".data"),nSeeds);
	VECTOR3* seeds=loadSeedfromFile("myseeds.seed",nSeeds);

	//nSeeds=25;
	osuflow->SetEntropySeedPoints( seeds,nSeeds);
	seeds = osuflow->GetSeeds(nSeeds); 

	for (int i=0; i<nSeeds; i++) 
	{	
		printf(" seed no. %d : [%f %f %f]\n", i, seeds[i][0], 
		seeds[i][1], seeds[i][2]); 
	
		seed_list.push_back(VECTOR3(seeds[i][0], seeds[i][1], seeds[i][2]));
	}

	sl_list.clear(); 
	std::vector<int> line_color;
	list<vtListSeedTrace*> half_lines, long_lines; 

	printf("compute streamlines..\n"); 
	osuflow->SetIntegrationParams(minstepsize, maxstepsize); //small and large step size
	osuflow->GenStreamLines(half_lines , BACKWARD_AND_FORWARD, maxi_stepnum, 0); //maxi steps
	printf(" done integrations\n"); 
	printf("list size = %d\n", half_lines.size()); 
	
	combinehalflines(half_lines,long_lines);
	sl_list.merge(long_lines);

	int grid_res[3];
	osuflow->GetFlowField()->getDimension(grid_res[0],grid_res[1],grid_res[2]);
//	grid_res[0]=64;grid_res[1]=64;grid_res[2]=64;
	

	
	//calculate the entropy 
	float* vectors;
	FILE* fp=fopen(g_filename,"rb");
//	int grid_res[3];
	fread(grid_res,sizeof(int),3,fp);

	vectors=new float[grid_res[0]*grid_res[1]*grid_res[2]*3];
	fread(vectors,sizeof(float),grid_res[0]*grid_res[1]*grid_res[2]*3,fp);
	fclose(fp);
	int* donot_change=new int[grid_res[0]*grid_res[1]*grid_res[2]];
	memset(donot_change,0,grid_res[0]*grid_res[1]*grid_res[2]);
	float* new_vectors=new float[grid_res[0]*grid_res[1]*grid_res[2]*3];
//	reconstruct_field_GVF_3D(new_vectors,vectors,grid_res,long_lines,donot_change);

	delete [] new_vectors;
	delete [] donot_change;

	for(int i=0; i<nSeeds;i++)
		line_color.push_back(i);
//	add_context_streamlines(line_color);
	//save_distance_volume();
}

void dumpEntropies(std::vector<float> mylist)
{
	
	FILE* fp=fopen("entropy_list.dat","wb");
	int entrynum=mylist.size();
	fwrite(&entrynum,sizeof(int),1,fp);
	for(int i=0;i<mylist.size();i++)
			fwrite(&(mylist[i]),sizeof(float),1,fp);
	fclose(fp);
}

int grid_res[3];
float* vectors,*new_vectors;
VECTOR3 newseed;
int* donot_change;
int* old_bin, *new_bin;
float* kx,*ky,*kz,*b,*c1,*c2,*c3;
OCTree* Tree;
std::vector<vtListVertex*>	Graph;
int* occupied;

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

//speed up by reusing histograms
void selectStreamlines_by_distribution(float* vectors,float* new_vectors, int* grid_res, 
									   int* occupied,
									   std::vector<VECTOR3>& seeds,
									   float* theta, float* phi,
										int* bin_vector, int* bin_newvectors,
										int* g_histo,int* g_histo_puv,int round,
											std::vector<float>& line_importance, float in_entropy)
{
	int sample_number_allowed=50;
	int  kernel_size[3];
	kernel_size[0]=kernel_size[1]= 8;//this is radium, actuall kernel is 2 x kernel_size+1
	kernel_size[2]=8;
	float* img_entropies=new float[grid_res[0]*grid_res[1]*grid_res[2]];
	float sum=0;
	float maximum=0;
	
	int* bin_my_vector,*bin_new_vector;
	bin_my_vector=new int[grid_res[0]*grid_res[1]*grid_res[2]];
	bin_new_vector=new int[grid_res[0]*grid_res[1]*grid_res[2]];
	memcpy(bin_my_vector,bin_vector,sizeof(int)*grid_res[0]*grid_res[1]*grid_res[2]);
	memcpy(bin_new_vector,bin_newvectors,sizeof(int)*grid_res[0]*grid_res[1]*grid_res[2]);
//	memcpy(img_entropies,entropies,sizeof(float)*grid_res[0]*grid_res[1]*grid_res[2]);
//	for(int i=0;i<grid_res[0]*grid_res[1]*grid_res[2];i++)
//	{
//		if(maximum<img_entropies[i])
//			maximum=img_entropies[i];
//		sum=sum+img_entropies[i];
//	}
	int radius=1;//for update_occupied, not using
	
	printf("calculate entropy...\n"); 



	int *histo_puv=new int[binnum*binnum];
	int *histo_pv=new int[binnum];
	//float* puv=new float[binnum*binnum];
	float* pv=new float[binnum];
	float* entropy_tmp=new float[binnum];


	int z=0;
	double dwStart = GetTickCount();
	for(int y=0;y<grid_res[1];y++)
	{
		//double dwStart = GetTickCount();
		for(int x=0;x<grid_res[0];x++)
		{
			memset(histo_puv,0,sizeof(int)*binnum*binnum);
			memset(histo_pv,0,sizeof(int)*binnum);

			for(int z=0;z<grid_res[2];z++)
			{
				VECTOR3 startpt,endpt;
				startpt.Set(max(0,x-kernel_size[0]),max(0,y-kernel_size[1]),max(0,z-kernel_size[2]));
				endpt.Set(min(x+kernel_size[0],grid_res[0]-1),min(y+kernel_size[1],grid_res[1]-1),
						min(z+kernel_size[2],grid_res[2]-1));


				
				if(z==0)//calculate the first histogram
				{
					for(int tz=(int)startpt.z(); tz<=(int)endpt.z();tz++)
					{
						for(int ty=(int)startpt.y(); ty<=(int)endpt.y();ty++)
						{
							for(int tx=(int)startpt.x(); tx<=(int)endpt.x();tx++)
							{
								int idx=tx+ty*grid_res[0]+tz*grid_res[0]*grid_res[1];
								int bin_no_ori=bin_vector[idx];
								int bin_no_new=bin_newvectors[idx];

								if(bin_no_ori<0 || bin_no_ori>=binnum||
									bin_no_new<0 || bin_no_new>=binnum)
								{
									printf("sth wrong, bin id=%d  %d\n", bin_no_ori,bin_no_new);
								}
								idx=bin_no_ori+binnum*bin_no_new;
								histo_puv[idx]++;//p(x,y)
								histo_pv[bin_no_new]++;
							}
						}
					}
				}
				else//update the histogram
				{
					//most the oldest 
					for(int ty=(int)startpt.y(); ty<=(int)endpt.y();ty++)
					{
						for(int tx=(int)startpt.x(); tx<=(int)endpt.x();tx++)
						{
							if(z-kernel_size[2]>=0)
							{
								int tz=z-kernel_size[2];
								int idx=tx+ty*grid_res[0]+tz*grid_res[0]*grid_res[1];
								int bin_no_ori=bin_vector[idx];
								int bin_no_new=bin_newvectors[idx];

								if(bin_no_ori<0 || bin_no_ori>=binnum||
									bin_no_new<0 || bin_no_new>=binnum)
								{
									printf("sth wrong, bin id=%d  %d\n", bin_no_ori,bin_no_new);
								}
								idx=bin_no_ori+binnum*bin_no_new;
								if(histo_puv[idx]<=0 ||histo_pv[bin_no_new]<=0 )
									printf("something wrong x=%d y=%d z=%d\n",tx,ty,tz);
								histo_puv[idx]--;//p(x,y)
								histo_pv[bin_no_new]--;							
							}
							if(z+kernel_size[2]<grid_res[2])
							{
								int tz=z+kernel_size[2];
								int idx=tx+ty*grid_res[0]+tz*grid_res[0]*grid_res[1];
								int bin_no_ori=bin_vector[idx];
								int bin_no_new=bin_newvectors[idx];

								if(bin_no_ori<0 || bin_no_ori>=binnum||
									bin_no_new<0 || bin_no_new>=binnum)
								{
									printf("sth wrong, bin id=%d  %d\n", bin_no_ori,bin_no_new);
								}
								idx=bin_no_ori+binnum*bin_no_new;
								histo_puv[idx]++;//p(x,y)
								//p(y)
								histo_pv[bin_no_new]++;
							}
						}
					}
				}
			
				
				float entropy=calcRelativeEntropy6_by_known_histograms( vectors,new_vectors, grid_res, 
							startpt,
							endpt,
							theta, phi,
							bin_vector, bin_newvectors,occupied,binnum,
							histo_puv, histo_pv,  pv, entropy_tmp,
							g_histo,g_histo_puv);

				//if(round<=0 || round>=3)
					entropy=entropy*entropy;//*entropy*entropy*entropy;//enhance

				int idx=x+y*grid_res[0]+z*grid_res[0]*grid_res[1];
				
				img_entropies[idx]=entropy;
				if(entropy>maximum)
					maximum=entropy;
				sum=sum+entropy;
			}
		}
//		double elapsedTime= GetTickCount() - dwStart;
//		printf("\n\n entorpy for each point time is %.3f milli-seconds.\n",elapsedTime); 	
		printf("y=%d/%d\r",y,grid_res[1]);

	}

	
	double elapsedTime= GetTickCount() - dwStart;
	printf("\n\n time for entorpy calculation is %.3f milli-seconds.\n",elapsedTime); 	
//	printf("halted\n");
//	getchar();

	

	
	//save result to file
	unsigned char* data;
	float* importance=new float[grid_res[0]*grid_res[1]*grid_res[2]];
	data = new unsigned char[grid_res[0]*grid_res[1]*grid_res[2]];
	for(int i=0;i<grid_res[0]*grid_res[1]*grid_res[2];i++)
	{	
		if(maximum>0)
		{
		data[i]=(unsigned char)((img_entropies[i]/maximum)*255);
		importance[i]=img_entropies[i]/maximum;
		}
	}
	char szname[255];
	memset(szname,0,255);
	sprintf(szname,"importance%d.bin",round);
	dumpImportanceField(szname,importance, grid_res);

	//normalized it to (0~1)
	for(int i=0;i<grid_res[0]*grid_res[1]*grid_res[2];i++)
	{
		//if(round<=1)
				img_entropies[i]=img_entropies[i]/sum;
		//else
//				img_entropies[i]=1-img_entropies[i]/sum;
	}

	//start the importance sampling, acceptance-rejection method
//	dwStart= GetTickCount();
//	int* selX, *selY,*selZ;
//	selX=new int[sample_number_allowed];
//	selY=new int[sample_number_allowed];
//	selZ=new int[sample_number_allowed];

//	float M=(maximum/sum)*1.1;
//	int i=0;
//	srand((unsigned)time(NULL));
//	while(i<sample_number_allowed)
//	{
//		float randx = ((float)rand()/(float)RAND_MAX);
//		float randy = ((float)rand()/(float)RAND_MAX);
//		float randz = ((float)rand()/(float)RAND_MAX);
//		float randu = ((float)rand()/(float)RAND_MAX);

//		int x=(int)(floor(randx*(grid_res[0]-1)));
//		int y=(int)(floor(randy*(grid_res[1]-1)));
//		int z=(int)(floor(randz*(grid_res[2]-1)));
//		float u=randu;
//		if(u<(img_entropies[x+y*grid_res[0]+z*grid_res[0]*grid_res[1]] /M))
//		{
//			selX[i]=x;
//			selY[i]=y;
//			selZ[i]=z;
//			i++;


//		}
//	}

//	elapsedTime= GetTickCount() - dwStart;
//	printf("\n\n sampling time is %.3f milli-seconds.\n",elapsedTime); 	

	
	dwStart= GetTickCount();
	srand((unsigned)time(NULL));
	int* selX, *selY,*selZ;
	selX=new int[sample_number_allowed];
	selY=new int[sample_number_allowed];
	selZ=new int[sample_number_allowed];
	float* qz=new float[grid_res[2]];
	//get z marginal
	for(int k=0;k<grid_res[2];k++)
	{
		float sum=0;
		for(int j=0;j<grid_res[1];j++)
		{
			for(int i=0;i<grid_res[0];i++)
				sum=sum+img_entropies[i+j*grid_res[0]+k*grid_res[0]*grid_res[1]];
		}
		qz[k]=sum;
	}

	//get y,z joint probs
	float *qy=new float[grid_res[1]*grid_res[2]];
	for(int z=0;z<grid_res[2];z++)
	{
		if(qz[z]<=0)
		{
			for(int y=0;y<grid_res[1];y++)
			{
				qy[z*grid_res[1]+y] = 0.0;
			}
			continue;
		}

		for(int y=0;y<grid_res[1];y++)
		{
			float sumy = 0;
			for(int x=0;x<grid_res[0];x++)
			{
				int idx=x+y*grid_res[0]+z*grid_res[0]*grid_res[1];
				sumy = sumy + img_entropies[idx];
			}
			qy[z*grid_res[1]+y]=sumy/qz[z];
		}
	}

	//get x,y,z joint probs
	float *qx=new float[grid_res[1]*grid_res[2]*grid_res[0]];
	for(int z=0;z<grid_res[2];z++)
	{
		for(int y=0;y<grid_res[1];y++)
		{
			int idy = z*grid_res[1]+y;
			if(qy[idy]<=0.0)
			{
				for(int x=0;x<grid_res[0];x++)
				{
					int idx=x+y*grid_res[0]+z*grid_res[0]*grid_res[1];
					qx[idx] = 0.0;
				}
				continue;
			}

			for(int x=0;x<grid_res[0];x++)
			{
				int idx=x+y*grid_res[0]+z*grid_res[0]*grid_res[1];
				qx[idx] = img_entropies[idx]/(qy[idy] * qz[z]);
			}
		}
	}

	int i=0;
	while(i<sample_number_allowed)
	{
		selZ[i] = sample_from(qz,grid_res[2]);
		i++;
	}
	i=0;
	while(i<sample_number_allowed)
	{
		selY[i] = sample_from(qy+selZ[i]*grid_res[1],grid_res[1]);
		i++;
	}
	i=0;
	while(i<sample_number_allowed)
	{
		selX[i] = sample_from(qx+selZ[i]*grid_res[0]*grid_res[1]+selY[i]*grid_res[0],grid_res[0]);
		i++;
	}

	elapsedTime= GetTickCount() - dwStart;
	printf("\n\n sampling time is %.3f milli-seconds.\n",elapsedTime); 	

	float cur_entropy=in_entropy;
	//scan and get the distributions out
	list<vtListSeedTrace*> lines,new_lines; 
	for(i=0;i<sample_number_allowed;i++)//start pruning
	{
		lines.clear();
		new_lines.clear();
		VECTOR3 newseed;
		newseed.Set(selX[i],selY[i],selZ[i]);
		
		//if(occupied[(int)newseed.x()+((int)newseed.y())*grid_res[0]+((int)newseed.z())*grid_res[0]*grid_res[1]]==1)
		//	continue;//do not select this one

		osuflow->SetEntropySeedPoints( &newseed,1);
		osuflow->SetIntegrationParams(minstepsize, maxstepsize);							//small and large step size

		//adjust streamline length by its entropy value
		int idx=selX[i]+selY[i]*grid_res[0]+selZ[i]*grid_res[0]*grid_res[1];
		float length=entropies[idx]*maxi_stepnum;

		osuflow->GenStreamLines(lines , BACKWARD_AND_FORWARD,length, 0);	 //maxi steps
//		combinehalflines(lines, new_lines);//for display only, the streamline stops when gets too near to an existing
		combinehalflines_check_stop_entropy(lines, new_lines,grid_res,entropies);//for display only, the streamline stops when gets too near to an existing
		/*
		if(new_lines.size())
		{
			sl_list.push_back(*(new_lines.begin()));			
			seeds.push_back(VECTOR3(selX[i],selY[i],selZ[i]));
			UpdateOccupied(new_lines,occupied,grid_res,radius);
			
			//float max_entropy=getImportance(new_lines, grid_res, importance);
			//line_importance.push_back(max_entropy);
		}
		*/
		

		list<vtListSeedTrace*> tmp_whole_set_lines; 
		if(new_lines.size())
		{
			float epsilon=0.01;
			tmp_whole_set_lines=sl_list;
			tmp_whole_set_lines.push_back(*new_lines.begin());
			bool discard=discardredundantstreamlines(cur_entropy,epsilon,tmp_whole_set_lines, vectors,new_vectors,grid_res,
				bin_my_vector,bin_new_vector,binnum, theta,  phi, histo_puv, histo_pv, pv, entropy_tmp);

			
	
			if(discard==false)
			{
				sl_list.push_back(*(new_lines.begin()));			
				seeds.push_back(VECTOR3(selX[i],selY[i],selZ[i]));
				UpdateOccupied(new_lines,occupied,grid_res,0);
				line_importance.push_back(cur_entropy);

			}
			tmp_whole_set_lines.clear();
		//density control should depend on the information content too.
		}
		
	}
	//std::vector<int> line_color;
	//for(int i=0;i<sl_list.size();i++)
	//	line_color.push_back(i);
	//save_streamlines_to_file_alpha_by_importance(line_color,line_importance,sl_list.size(), importance, grid_res);

	delete [] pv;
	delete [] histo_puv;
	delete [] histo_pv;
	delete [] entropy_tmp;

	delete [] selX;
	delete [] selY;
	delete [] selZ;
	//save2PPM_3("cand_seeds_image.ppm", data, grid_res[0],grid_res[1]);
	delete [] data;
	delete [] img_entropies;
	delete [] importance;
	delete [] bin_my_vector;
	delete [] bin_new_vector;
	return;
}

static int* done=NULL;
// static int* old_bin;
// static int* new_bin;
// static int* donot_change=NULL;
// static float* new_vectors=NULL;
static float* importance=NULL;

// static float* kx=NULL;	
// static float* ky=NULL;	
// static float* kz=NULL;	

// static float* b=NULL;	
// static float* c1=NULL;	
// static float* c2=NULL;	
// static float* c3=NULL;	

static bool bIsInitialized = false;

void compute_streamlines() 
{

	printf("generating seeds...\n"); 
	int num=0;
	int grid_res[3];
	float* vectors=get_grid_vec_data(grid_res);//get vec data at each grid point
	
										//load seeds from file
	int* donot_change=new int[grid_res[0]*grid_res[1]*grid_res[2]];
	memset(donot_change,0,sizeof(int)*grid_res[0]*grid_res[1]*grid_res[2]);
	float* new_vectors=new float[grid_res[0]*grid_res[1]*grid_res[2]*3];
	memset(new_vectors,0,sizeof(float)*grid_res[0]*grid_res[1]*grid_res[2]*3);

	int nSeeds; 
	//set first seed as domain center
	sl_list.clear(); 
	seed_list.clear();
	
	//set boundary condition
	for(int z=0;z<grid_res[2];z++)
	for(int y=0;y<grid_res[1];y++)
	for(int x=0;x<grid_res[0];x++)
	{
		if( x==0||x==grid_res[0]-1||
			y==0||y==grid_res[1]-1||
			z==0||z==grid_res[2]-1)
		{
			int idx=x+y*grid_res[0]+z*grid_res[0]*grid_res[1];
			new_vectors[idx*3+0]=vectors[idx*3+0];
			new_vectors[idx*3+1]=vectors[idx*3+1];
			new_vectors[idx*3+2]=vectors[idx*3+2];
			donot_change[idx]=1;
		}
	}

	int* old_bin, *new_bin;
	old_bin=new int [grid_res[0]*grid_res[1]*grid_res[2]];
	new_bin=new int [grid_res[0]*grid_res[1]*grid_res[2]];

	for(int i=0;i<grid_res[0]*grid_res[1]*grid_res[2];i++)
	{
		VECTOR3 orif;
		orif.Set(vectors[i*3+0],vectors[i*3+1],vectors[i*3+2]);
		old_bin[i]=get_bin_number_3D(orif,theta, phi,binnum);
		new_bin[i]=0.0;
	}


	if(!entropies)
	{
		printf("calculating every point entropies\n");
		entropies=new float[grid_res[0]*grid_res[1]*grid_res[2]];
		calc_entropy( old_bin,grid_res, binnum, entropies);
		dumpEntropyField("entropies.bin",entropies, grid_res);

		printf("entropy calculation done\n");
		
	}		
	int selected_line_num=0;
	float entropy=8888;
	std::vector<VECTOR3> seedlist,selected_list;
	srand((unsigned)time(NULL));			// initialize random number generator
	std::vector<float> entropies;

	int x_min=0,x_max=0,y_min=0,y_max=0;
	int* occupied=new int[grid_res[0]*grid_res[1]*grid_res[2]];
	memset(occupied,0,sizeof(int)*grid_res[0]*grid_res[1]*grid_res[2]);
	
	float* kx=new float[grid_res[0]*grid_res[1]*grid_res[2]];
	float* ky=new float[grid_res[0]*grid_res[1]*grid_res[2]];
	float* kz=new float[grid_res[0]*grid_res[1]*grid_res[2]];

	float* b=new float[grid_res[0]*grid_res[1]*grid_res[2]];
	float* c1=new float[grid_res[0]*grid_res[1]*grid_res[2]];
	float* c2=new float[grid_res[0]*grid_res[1]*grid_res[2]];
	float* c3=new float[grid_res[0]*grid_res[1]*grid_res[2]];

	// ADD-BY-LEETEN 2009/11/10-BEGIN
	_FlowDiffusionInit(grid_res[0], grid_res[1], grid_res[2]);
	int get_bin_by_angle(float mytheta, float myphi, int binnum, float* theta, float* phi);

	static const int iNrOfThetas = 720;
	static const int iNrOfPhis = 360;
	static const double dNrOfThetas = double(iNrOfThetas);
	static const double dNrOfPhis	= double(iNrOfPhis);
	static int	ppiAngleMap[iNrOfThetas][iNrOfPhis];
	for(int t = 0; t < iNrOfThetas; t++)
		for(int p = 0; p < iNrOfPhis; p++)
		{
			float fTheta =	M_PI * 2.0f * float(t) / float(iNrOfThetas);
			float fPhi =	M_PI * float(p) / float(iNrOfPhis);
			int iBin = get_bin_by_angle(fTheta, fPhi, binnum, theta, phi);
			if( iBin >= 0 )
				ppiAngleMap[t][p] = iBin;
		}

	_FlowDiffusionSetAngleMap(&ppiAngleMap[0][0], iNrOfPhis, iNrOfThetas);
	// ADD-BY-LEETEN 2009/11/10-END
	//initial entropy
	float error=0.05;
	float target_entropy=-error*log2(error)-(1-error)*log2(1-error)+error*log2(359);
	printf("entropy_target=%f\n",target_entropy);
	std::vector<int> line_color;

	int *histo_puv=new int[binnum*binnum];
	int *histo_pv=new int[binnum];
	float* entropy_tmp=new float[binnum];
	float* pv=new float[binnum];
	int line_num_thres=NR_OF_STREAMLINES;//27;											//want to select top line_num_thres lines

	memset(kx,0,sizeof(float)*grid_res[0]*grid_res[1]*grid_res[2]);
	memset(ky,0,sizeof(float)*grid_res[0]*grid_res[1]*grid_res[2]);
	memset(kz,0,sizeof(float)*grid_res[0]*grid_res[1]*grid_res[2]);
	memset(b,0,sizeof(float)*grid_res[0]*grid_res[1]*grid_res[2]);
	memset(c1,0,sizeof(float)*grid_res[0]*grid_res[1]*grid_res[2]);
	memset(c2,0,sizeof(float)*grid_res[0]*grid_res[1]*grid_res[2]);
	memset(c3,0,sizeof(float)*grid_res[0]*grid_res[1]*grid_res[2]);
	int round=0;

	std::vector<float> line_importance,entropy_list;

//	while(selected_line_num<line_num_thres )//&& entropy>.5)
	while(entropy>target_entropy)
	{
		VECTOR3 next_seed;
		std::vector<VECTOR3> seeds;
		printf("%d seeds selected,round=%d  entropy=%f\n",seed_list.size(), round, entropy);
		selectStreamlines_by_distribution(vectors,new_vectors, grid_res, occupied,seeds,
			theta, phi,old_bin,new_bin,0,0,round++,line_importance,entropy);


		//select new seed
		for(int i=0;i<seeds.size();i++)
			line_color.push_back(selected_line_num+i);
		selected_line_num+=seeds.size();

		//printf("calculate the bin number\r");
		for(int i=0;i<grid_res[0]*grid_res[1]*grid_res[2];i++)
		{
			VECTOR3 newf,orif;
			orif.Set(vectors[i*3+0],vectors[i*3+1],vectors[i*3+2]);
			newf.Set(new_vectors[i*3+0],new_vectors[i*3+1],new_vectors[i*3+2]);
			//if within error range, let it be teh same bin as the ori, otherwise select the bin num
			newf.Normalize();
			float dotv=dot(newf,orif);
			new_bin[i]=get_bin_number_3D(newf,theta, phi,binnum);
		}

		entropy=calcRelativeEntropy6_new(	vectors, new_vectors,  grid_res, VECTOR3(2,2,2),//do not count boundaries
									VECTOR3(grid_res[0]-2,grid_res[1]-2,grid_res[2]-2),theta,phi,old_bin,new_bin,0,binnum,histo_puv,histo_pv,
									pv,entropy_tmp);
	//	entropy_list.push_back(entropy);
		for(int i=0;i<seeds.size();i++)
		{
			selected_list.push_back(seeds[i]);
			//seedlist.push_back(seeds[i]);
			seed_list.push_back(seeds[i]);
		}
	
		//dumpEntropy(entropies,"entropy.bin");
		dumpSeeds(seed_list,"myseeds.seed");//crtical points excluded
		double dwStart= GetTickCount();

		//printf("streamline size=%d\n",sl_list.size());
		reconstruct_field_GVF_3D(new_vectors,vectors,grid_res,sl_list,donot_change,
						 kx,ky,kz,b,c1,c2,c3);//,importance);
		double elapsedTime= GetTickCount() - dwStart;
		printf("\n\n reconstruction time is %.3f milli-seconds.\n",elapsedTime); 	
		
		//clear the memory of sl_list;save memory for larger dataset
		/*std::list<vtListSeedTrace*>::iterator pIter; 
		pIter = sl_list.begin(); 
		
		for (; pIter!=sl_list.end(); pIter++) {
		
			vtListSeedTrace *trace = *pIter; 
			std::list<VECTOR3*>::iterator pnIter,pnIter2; 
			pnIter = trace->begin(); 
			for (; pnIter!= trace->end(); pnIter++) 
			 delete  *pnIter; 
			delete trace;
		}
		sl_list.clear();*/
		//dumpReconstruedField("r.vec", new_vectors, grid_res);
		
		unsigned char* dat=new unsigned char[grid_res[0]*grid_res[1]*grid_res[2]];
		for(int i=0;i<grid_res[0]*grid_res[1]*grid_res[2];i++)
			dat[i]=occupied[i]*255;
		/*FILE* test=fopen("impor.bin","wb");
		fwrite(grid_res,sizeof(int),3,test);
		fwrite(occupied,sizeof(unsigned char),grid_res[0]*grid_res[1]*grid_res[2],test);
		fclose(test);
		*/
		delete [] dat;

		for(int i=0;i<sl_list.size();i++)
		line_color.push_back(i);
		char filename[255];
		memset(filename,0,255);
		sprintf(filename,"streamlines%d.dat",round);
		save_streamlines_to_file_hand_tuning(filename,line_color,nSeeds);
		dumpEntropies(line_importance);
		//	getchar();
	//	printf("halted, save files now\n");
	}
	

	//dumpReconstruedField("r.vec", new_vectors, grid_res);
	dumpSeeds(seed_list,"myseeds.seed");//crtical points excluded
	delete [] histo_puv;
	delete [] histo_pv;
	delete [] pv;
	delete [] entropy_tmp;
	delete [] occupied;
	delete [] old_bin;
	delete [] new_bin;
	delete [] kx;
	delete [] ky;
	delete [] kz;
	delete [] b;
	delete [] c1;
	delete [] c2;
	delete [] c3;

	delete [] donot_change;
	delete [] vectors;
	delete [] new_vectors;
	#if	USE_CUDA	
	_FlowDiffusionFree();
	#endif
}

void draw_streamlines() {

	glEnable(GL_LINE_SMOOTH);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
//	glBlendFunc(GL_SRC_ALPHA_SATURATE,GL_ONE);

	glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
//	glEnable(GL_LINE_SMOOTH);
	glLineWidth(1);

	glPushMatrix(); 
//	draw_cube(1,1,1);

	// glScalef(3.0f, 3.0f, 3.0f); // TEST-ADD

	glScalef(1/(float)len[1], 1/(float)len[1], 1/(float)len[1]); 
	glTranslatef(-len[0]/2.0, -len[1]/2.0, -len[2]/2.0); 
	//printf("draw streamlines.\n"); 
	std::list<vtListSeedTrace*>::iterator pIter; 
	int count=0;

	glColor3f(0,0,1); 
//glEnable(GL_VERTEX_PROGRAM_POINT_SIZE);



	if(!g_vectors)
		g_vectors=get_grid_vec_data(g_grid_res);//get vec data at each grid point

	for(int i=0;i<seed_list.size();i++)
	{
		if(i==seed_list.size()-1)
			continue;

		int x=seed_list[i].x();int y=seed_list[i].y(); int z=seed_list[i].z();
		int idx=x+y*g_grid_res[0]+z*g_grid_res[0]*g_grid_res[1];
		float fx,fy,fz;
		fx=g_vectors[idx*3+0];
		fy=g_vectors[idx*3+1];
		fz=g_vectors[idx*3+2];

		glTranslatef(seed_list[i].x(),seed_list[i].y(),seed_list[i].z());

		float r,g,b,a;
		get_color_entropy(r,g,b, a,seed_list[i],grid_res);
		glColor3f(r,g,b); 
		VECTOR3 axis, v,zaxis;
		zaxis.Set(0,0,1);
		v.Set(fx,fy,fz);
		float theta=acos(dot(zaxis,v))* 180.0 / PI;
		axis=cross(zaxis,v);		
		axis.Normalize();
		glRotatef(theta,axis.x(),axis.y(),axis.z());
		glutSolidCone(.5f,3.0f,5,2);
		glRotatef(-theta,axis.x(),axis.y(),axis.z());


		glTranslatef(-seed_list[i].x(),-seed_list[i].y(),-seed_list[i].z());
	}
	int step=4;
	glLineWidth(1.0);
	glColor3f(0,0,0);
	glBegin(GL_LINES);
	for(int z=0;z<g_grid_res[2];z+=step)
	for(int y=0;y<g_grid_res[1];y+=step)
	for(int x=0;x<g_grid_res[0];x+=step)
	{
	
		float fx,fy,fz;
		int idx=x+y*g_grid_res[0]+z*g_grid_res[0]*g_grid_res[1];
		fx=g_vectors[idx*3+0];
		fy=g_vectors[idx*3+1];
		fz=g_vectors[idx*3+2];

		if(toggle_entropy)
		{
		//glVertex3f(x,y,z);
		//glVertex3f(x+fx,y+fy,z+fz);
		}

	}
	glEnd();

	if(g_new_vectors)
	{
		glColor3f(1,0,0);
		glBegin(GL_LINES);
		for(int z=0;z<g_grid_res[2];z+=step)
		for(int y=0;y<g_grid_res[1];y+=step)
		for(int x=0;x<g_grid_res[0];x+=step)
		{
	

			float fx,fy,fz;
			int idx=x+y*g_grid_res[0]+z*g_grid_res[0]*g_grid_res[1];
			float entropy=entropy_every_point[idx];
			//get color
			float r,g,b;
			r=entropy;
			b=1-entropy;
			if(entropy<0.5)
				g=entropy*2;
			else
				g=2-entropy*2;

			glColor3f(r,g,b);

			fx=g_new_vectors[idx*3+0];
			fy=g_new_vectors[idx*3+1];
			fz=g_new_vectors[idx*3+2];

			float len=fx*fx+fy*fy+fz*fz;
			if(len>0)
			{
				len=sqrt(len);
				fx=fx/len; fy=fy/len; fz=fz/len;
			}
			if(toggle_entropy)
			{
			//glVertex3f(x,y,z);
			//glVertex3f(x+2*fx,y+2*fy,z+2*fz);
			}
		}
		glEnd();
	}
	glEnable(GL_BLEND);

	glLineWidth(1.0);
	pIter = sl_list.begin(); 
	count=0;
	for (; pIter!=sl_list.end(); pIter++) {

		
		float r,g,b,a;
		//get_line_color(count, r, g, b, a,sl_list.size());

		vtListSeedTrace *trace = *pIter; 
		std::list<VECTOR3*>::iterator pnIter,pnIter2; 

		if(trace->size()<=2)
			continue;
		VECTOR3 q,p2;
		pnIter2 = trace->begin(); 
		pnIter2++;
		p2=**(pnIter2);
		glBegin(GL_LINE_STRIP);
		
		int s=0;
		
		pnIter = trace->begin(); 
		for (; pnIter!= trace->end(); pnIter++) {
			VECTOR3 p = **pnIter; 
			//printf(" %f %f %f ", p[0], p[1], p[2]); 
	
	//		glVertexAttrib3f(my_vec3_location, p2[0],p2[1],p2[2]);
			r=1;g=0;b=.5;a=1;
			get_color_entropy(r,g,b, a,p,grid_res);
			
			glColor4f(r,g,b,a); 

			glVertex3f(p[0], p[1], p[2]); 
			p2[0]=p[0];p2[1]=p[1];p2[2]=p[2];

			s++;
			if(s==trace->size()/2)
			{
				q[0]=p[0];q[1]=p[1];q[2]=p[2];
			}
		}

		glEnd(); 


		count++;
		if(count%2==0)
			continue;

		glRasterPos3f(q[0], q[1],q[2]);
		char str[10];
		memset(str,0,10);
		sprintf(str,"%d",(count/2));
		char *c;
		for (c=str; *c != '\0'; c++) {
			//	glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, *c);
		}


	}
	glDisable(GL_BLEND);

	glPopMatrix(); 

}

void display()
{
	
	GLfloat		lightPosition[4] =  { -15.0f, -15.0f, 5.0f, 1.0f };

	glEnable(GL_DEPTH_TEST); 
	glClearColor(1,1,1,1); 
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT); 

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity(); 
	gluPerspective(60, 1, 1, 100); 


	glMatrixMode(GL_MODELVIEW); 
	glLoadIdentity(); 

	glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);
	gluLookAt(0,0,5,0,0,0,0,1,0); 

	glRotatef(x_angle, 0, 1,0); 
	glRotatef(y_angle, 1,0,0); 
	glScalef(scale_size, scale_size, scale_size); 


	
	draw_streamlines();

/*	glTranslatef(0,0,-1); 
	//if (toggle_draw_streamlines == true)
	drawBounds_inner();
	glDepthMask(GL_FALSE);
	drawBounds_outter();
	glDepthMask(GL_TRUE);
 */	
	//draw_triangulation();
	// else if (toggle_animate_streamlines == true)
	// animate_streamlines(); 

	//printf(" len %f %f %f\n", len[0], len[1], len[2]); 
/*
	glPushMatrix(); 
	glScalef(1/(float)len[0], 1/(float)len[0], 1/(float)len[0]); 
	//glScalef(1/110,1/110,1/110); 
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
	glPopMatrix(); 
*/
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

///////////////////////////////////////////////////////////////

void mykey(unsigned char key, int x, int y)
{
	switch(key) {
	case 27:
		exit(0);
		break;

	case 'S': 
	case 's': 
	//	compute_streamlines_by_click();
		compute_streamlines(); 
		//compute_streamlines_load_file() ;
//hand_tuning_streamlines();
		glutPostRedisplay(); 
		break; 
	case 'L': 
	case 'l': 
		//compute_streamlines(); 
		compute_streamlines_load_file() ;

		glutPostRedisplay(); 
		break; 	case 'D': 
	case 'd': 
		toggle_draw_streamlines = !toggle_draw_streamlines; 
		toggle_animate_streamlines = false; 

		break; 
	case'A': 
	case'a': 
		toggle_animate_streamlines = !toggle_animate_streamlines; 
		toggle_draw_streamlines = false; 
		first_frame = 1; 
		break;

	case 'T':
	case 't':
		//testReconstruction();
		toggle_entropy==0?toggle_entropy=1:toggle_entropy=0;
		glutPostRedisplay(); 

		break;
	}
}
///////////////////////////////////////////////////////////////

void readPatches_region()
{
	double pdPatch[] = {
#include "patch.txt"
	};
	size_t uNrOfPatches = (sizeof(pdPatch)/sizeof(pdPatch[0]))/4;
	if(!theta)
		theta = new float[2*uNrOfPatches];

	if(!phi)
		phi = new float[2*uNrOfPatches]; 

	for(size_t p = 0; p < uNrOfPatches; p++)
	{
		theta[p*2+0] = (float)pdPatch[p*4+0];
		theta[p*2+1] = (float)pdPatch[p*4+1];
		phi[p*2+0] = (float)pdPatch[p*4+2];
		phi[p*2+1] = (float)pdPatch[p*4+3];
	}
}

void set_time_step(char* filename,float t)
{
	FILE* fp = fopen(filename, "rb"); //2d data

	if (fp == NULL) return; 

	int grid_res[3]; 

	int num=fread(grid_res, sizeof(int), 3, fp); 


	float* vectors = new float[grid_res[0]*grid_res[1]*grid_res[2]*3]; 
	num=fread(vectors,sizeof(float),grid_res[0]*grid_res[1]*grid_res[2]*3,fp);

	fclose(fp);

	for(int z=0; z<grid_res[2];z++)
	{
		for(int y=0; y<grid_res[1];y++)
		{
			for(int x=0; x<grid_res[0];x++)
			{
				int idx=x+y*grid_res[0]+z*grid_res[0]*grid_res[1];
				vectors[idx*3+2]=t;
			}
		}
	}
	fp=fopen("time.vec","wb");
	fwrite(grid_res,sizeof(int),3,fp);
	fwrite(vectors,sizeof(float),grid_res[0]*grid_res[1]*grid_res[2]*3,fp);
	fclose(fp);

	delete [] vectors;

}

int main(int argc, char** argv) 
{
		g_filename=argv[1];


//	testTriangulation();
//downsampling();

//	float error=0.005;
//printf("value=%f\n",-error*log2(error)-(1-error)*log2(1-error)+error*log2(359));
//getchar();

readPatches_region();


	VECTOR3 minB, maxB; 

	osuflow = new OSUFlowEntropy(); 
	//printf("read file %s\n", argv[1]); 
//	g_filename=argv[1];
	osuflow->LoadData((const char*)argv[1], true);//, minB, maxB); //true: a steady flow field 

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
	glutPositionWindow(700,50);
	glutDisplayFunc(display); 
	//  glutIdleFunc(idle); 
	glutTimerFunc(10, timer, 0); 
	glutMouseFunc(mymouse); 
	glutMotionFunc(mymotion);
	glutKeyboardFunc(mykey); 
	//SetShaders();

	#if	ENTER_GLUT_LOOP	
	glutMainLoop();

	#else
	compute_streamlines();
	#endif
	return 0;
}
