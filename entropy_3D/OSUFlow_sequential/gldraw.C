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

#if	0	// MOD-BY-LEETEN 01/22/2010-FROM:
	#include <GLUT/glut.h> 
	#include <OpenGL/gl.h>
#else	// MOD-BY-LEETEN 01/22/2010-TO:
	#include <GL/glut.h> 
#endif	// MOD-BY-LEETEN 01/22/2010-END

#include "OSUFlow.h"

#include <list>
#include <iterator>

int binnum=360;
// ADD-BY-LEETEN 2009/11/10-BEGIN
//#include "../myinterface/FlowDiffusionCuda.h"
// ADD-BY-LEETEN 2009/11/10-END

#include "liblog.h"					// ADD-BY-LEETEN 02/02/2010

#include "FlowDiffusion3DConfig.h"	// ADD-BY-LEETEN 02/02/2010

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
float scale_size = 1; 

int toggle_entropy=1;
int xform_mode = 0; 

#define XFORM_NONE    0 
#define XFORM_ROTATE  1
#define XFORM_SCALE 2 

OSUFlow *osuflow; 
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

#if	0	// DEL-BY-LEETEN 01/22/2010-BEGIN
	#include <CGAL/Surface_mesh_default_triangulation_3.h>
	#include <CGAL/Complex_2_in_triangulation_3.h>
	#include <CGAL/make_surface_mesh.h>
	#include <CGAL/Implicit_surface_3.h>

	// default triangulation for Surface_mesher
	typedef CGAL::Surface_mesh_default_triangulation_3 Tr;

	// c2t3
	typedef CGAL::Complex_2_in_triangulation_3<Tr> C2t3;

	typedef Tr::Geom_traits GT;
	typedef GT::Sphere_3 Sphere_3;
	typedef GT::Point_3 Point_3;
	typedef GT::FT FT;

	typedef FT (*Function)(Point_3);

	typedef CGAL::Implicit_surface_3<GT, Function> Surface_3;

	FT sphere_function (Point_3 p) {
	  const FT x2=p.x()*p.x(), y2=p.y()*p.y(), z2=p.z()*p.z();
	  return x2+y2+z2-1;
	}
#endif	// DEL-BY-LEETEN 01/22/2010-END

typedef GLdouble TMatrix[16];

void 
SaveMatrix(char *szMatrixFilename)
{
	TMatrix tModelMatrix,tViewMatrix;
	glGetDoublev(GL_MODELVIEW_MATRIX, tModelMatrix);
	glGetDoublev(GL_MODELVIEW_MATRIX, tViewMatrix);

	FILE *fpMatrix;
	fpMatrix = fopen(szMatrixFilename, "wt");
	if( !fpMatrix )
		return;

	for(int i = 0,	r = 0; r < 4; r++)
	{
		for(int		c = 0; c < 4; c++, i++)
			fprintf(fpMatrix, "%f ", tModelMatrix[i]);
		fprintf(fpMatrix, "\n");
	}
	for(int i = 0,	r = 0; r < 4; r++)
	{
		for(int		c = 0; c < 4; c++, i++)
			fprintf(fpMatrix, "%f ", tViewMatrix[i]);
		fprintf(fpMatrix, "\n");
	}
	fclose(fpMatrix);
}

#if	0	// DEL-BY-LEETEN 01/22/2010-BEGIN
	void test() {
	  Tr tr;            // 3D-Delaunay triangulation
	  C2t3 c2t3 (tr);   // 2D-complex in 3D-Delaunay triangulation

	  // defining the surface
	  Surface_3 surface(sphere_function,             // pointer to function
						Sphere_3(CGAL::ORIGIN, 2.)); // bounding sphere

	  // defining meshing criteria
	  CGAL::Surface_mesh_default_criteria_3<Tr> criteria(30.,  // angular bound
														 0.8,  // radius bound
														 0.8); // distance bound
	  // meshing surface
	  CGAL::make_surface_mesh(c2t3, surface, criteria, CGAL::Non_manifold_tag());

	  std::cout << "Final number of points: " << tr.number_of_vertices() << "\n";
	  Tr::Finite_cells_iterator iter;
	  iter=tr.finite_cells_begin();
	  for(;iter!=tr.finite_cells_end(); iter++)
	  {
			for(int i=0; i<3; i++)
			{
			printf("pt=%f %f %f\n",			iter->vertex(i)->point().x(),
											iter->vertex(i)->point().y(),
											iter->vertex(i)->point().z());	

			sphere_mesh.push_back(VECTOR3(iter->vertex(i)->point().x(),
											iter->vertex(i)->point().y(),
											iter->vertex(i)->point().z()));
			}

	  }
	  printf("halted...\n");
	  getchar();
	}
#endif	// DEL-BY-LEETEN 01/22/2010-END

////////////////////////////////////////////////////////
void draw_cube(float r, float g, float b)
{
	glColor3f(r, g, b); 
//	glutSolidCube(0.1);   // draw a solid cube 
	glutWireSphere(1.0,15,15);   // draw a solid cube 

}
void draw_region_centers()
{
	for(int i=0; i<360;i++)
	{
		glPushMatrix();
		glTranslatef(xyz[i*3+0],xyz[i*3+1],xyz[i*3+2]); 
		glutSolidCube(0.02);
		glPopMatrix();
	}
}
void save_streamlines_to_file()
{
	int grid_res[3];
	float* vectors;
	FILE* fp=fopen(g_filename,"rb");
	fread(grid_res,sizeof(int),3,fp);

	vectors=new float[grid_res[0]*grid_res[1]*grid_res[2]*3];
	fread(vectors,sizeof(float),grid_res[0]*grid_res[1]*grid_res[2]*3,fp);
	fclose(fp);
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
	float*   mag=new float[total_ver_num];
	float*   alpha=new float[total_ver_num];
	
	int count=0;
	pIter = sl_list.begin(); 
	for (; pIter!=sl_list.end(); pIter++) {
		vtListSeedTrace *trace = *pIter; 
		std::list<VECTOR3*>::iterator pnIter; 
		pnIter = trace->begin(); 

		VECTOR3 q;
		for (; pnIter!= trace->end(); pnIter++) {
			VECTOR3 p = **pnIter; 
			ver[count].Set(p[0], p[1], p[2]); 
			count++;
		}
	}

	//get the velocity mag
	count=0;
	float mag_max=0;
	int line_count=0;
	pIter = sl_list.begin(); 
	for (; pIter!=sl_list.end(); pIter++) {
		vtListSeedTrace *trace = *pIter; 
		std::list<VECTOR3*>::iterator pnIter; 
		pnIter = trace->begin(); 

		VECTOR3 q;
		for (; pnIter!= trace->end(); pnIter++) {
			VECTOR3 p = **pnIter; 
			float vx,vy,vz;
			int idx=(int)p[0]+(int)p[1]*dim[0]+(int)p[2]*dim[0]*dim[1];
			vx=vectors[idx*3+0];
			vy=vectors[idx*3+1];
			vz=vectors[idx*3+2];

			if(line_count<50)
				alpha[count]=1;//(100-line_count)*0.005;
			else if(line_count<100)
				alpha[count]=(100-line_count)*0.02;
			else
			alpha[count]=0.05*2;

			mag[count]=sqrt(vx*vx+vy*vy+vz*vz);
			if(mag_max<mag[count])
				mag_max=mag[count];
			count++;
		}
		line_count++;
	}

	//RGBA COLOR FOR EACH VER
	VECTOR4* rgba=new VECTOR4[total_ver_num];
	for(int i=0; i<total_ver_num;i++)
	{
		//vertex
		float val=mag[i]/mag_max;
		float r,g,b;
		float thre1=0.4;
		float thre2=0.8;
		if(val<thre1)
			b=val;
		else if(val<thre2)
		{
			g=val;
			b=1-(val-thre1)/(thre2-thre1);
		}
		else 
		{
			r=val;
			g=1-(val-thre2)/(1-thre2);
		}

		rgba[i].Set(r,g,b,alpha[i]);
	}
	fp=fopen("velocity.dat","wb");
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
	

}
void get_line_color(int color,float& r,float& g,float& b,float& a,float totalf)
{
	a=1.0f;
	float group_total=10;
	int group_size=totalf/group_total;
	if(group_size==0)
		group_size=1;
	int group_id=color/group_size;

	switch(color%10)
//	switch(group_id)
	{
			case 0:
				r=0.8;b=0.4;g=0.2;
				break;
			case 1:
				r=0.5;b=0.5;g=0.25;
				break;
			case 2:
				r=1.0; b=0.0f; g=0.0f; 
				break;
			case 3:
				r=1.0; b=1.0f; g=0.0f; 
				break;
			case 4:
				r=0.0; b=1.0f; g=0.0f;
				break;
			case 5:
				r=0.0; b=0.0f; g=1.0f;
				break;
			case 6:
				r=0.0; b=1.0f; g=1.0f; 
				break;
			case 7:
				r=1.0; b=0.5f; g=0.5f; 
				break;
			case 8:
				r=0.1; b=0.25f; g=0.5f; 
				break;
			case 9:
				r=0.7; b=0.3f; g=0.1f; 
				break;

			default:
				r=0.8; b=0.08f; g=0.8f; 
				break;
	}
//	a=1-group_id/group_total;
	//r=0.5; g=0.5; b=0.5;
	a=0.5-color*(1/totalf)/2/10.0;

//	a=1-color*(1/totalf);
//	if(color>=100)//transparent lines
	{
		//r=0.8;b=0.8;g=0.8;
//		a=0.5;
	}
}

void save_streamlines_to_file_alpha_by_importance(std::vector<int> line_color,
												  std::vector<float> line_importance,
												  int nseeds, float* importance,
												   int* grid_res)
{
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
	float*   alpha=new float[total_ver_num];
	//RGBA COLOR FOR EACH VER
	VECTOR4* rgba=new VECTOR4[total_ver_num];

	float totalf=sl_list.size();
	int count=0;
	int line_no=0;

	pIter = sl_list.begin(); 
	for (; pIter!=sl_list.end(); pIter++) 
	{
		vtListSeedTrace *trace = *pIter; 
		std::list<VECTOR3*>::iterator pnIter; 
		pnIter = trace->begin(); 

		VECTOR3 q;
		for (; pnIter!= trace->end(); pnIter++) 
		{
			VECTOR3 p = **pnIter; 
			ver[count].Set(p[0], p[1], p[2]); 
			float r,g,b,a;
			get_line_color(line_color[line_no],r,g,b,a,(float)nseeds);
			
		//	int idx=(int)p[0]+((int)p[1])*grid_res[0]+((int)p[2])*grid_res[0]*grid_res[1];
		//	a=importance[idx];
		//	a=line_importance[line_no];
			rgba[count].Set(r,g,b,a);
			
			count++;
		}
		line_no++;
	}

	//fp=fopen("velocity.dat","wb");
	FILE* fp=fopen("F:\\xul\\illuminite lines\\examples\\viewer\\data\\velocity.dat","wb");
	
	fwrite(&line_num,sizeof(int),1,fp);
	fwrite(ver_num,sizeof(int),line_num,fp);
	fwrite(ver,sizeof(VECTOR3),total_ver_num,fp);
	fwrite(rgba,sizeof(VECTOR4),total_ver_num,fp);
	fclose(fp);
	delete [] ver;
	delete [] ver_num;
	delete [] rgba;
	delete [] alpha;

}
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

void hand_tune_alpha()
{

	/*FILE* fp=fopen("dif.vec","rb");
	if(fp==0)
	{
		printf("can not open reconstruct file to write \n");
		return ;//return;
	}
	int grid_res[3];
	fread(grid_res,sizeof(int),3,fp);
	unsigned char* dif=new unsigned char [grid_res[0]*grid_res[1]*grid_res[2]];
	fread(dif,sizeof(unsigned char),grid_res[0]*grid_res[1]*grid_res[2],fp);
//	fwrite(new_vector,sizeof(float),grid_res[0]*grid_res[1]*grid_res[2]*3,fp);
	fclose(fp);
*/

	printf("begin adjusting alpha\n");
	FILE* fp=fopen("F:\\xul\\illuminite lines\\examples\\viewer\\data\\velocity.dat","rb");
	
	int line_num,*ver_num,total_ver_num;
	fread(&line_num,sizeof(int),1,fp);
	ver_num=new int[line_num];
	fread(ver_num,sizeof(int),line_num,fp);
	
	total_ver_num=0;
	int focus_before=500;
	int my_focus=0;
	for(int i=0;i<line_num;i++)
	{
		total_ver_num+=ver_num[i];
		if(i<focus_before)
		my_focus+=ver_num[i];
			
	}

	//write the vertices
	VECTOR3* ver=new VECTOR3[total_ver_num];
	float*   alpha=new float[total_ver_num];
	//RGBA COLOR FOR EACH VER
	VECTOR4* rgba=new VECTOR4[total_ver_num];

	fread(ver,sizeof(VECTOR3),total_ver_num,fp);
	fread(rgba,sizeof(VECTOR4),total_ver_num,fp);
	fclose(fp);

	//adjust here
	/*
	for(int i=0; i<my_focus;i++)
//		rgba[i].Set(rgba[i][0],rgba[i][1],rgba[i][2],1);
	{
		float r,g,b,a;
		get_line_color(i,r,g,b,a,line_num);
		
		rgba[i].Set(0,0,0,1);
	}
	*/

	int grid_res[3];
	//Note: the vector is normalized
	//float* vectors=get_grid_vec_data(grid_res);//get vec data at each grid point
	fp=fopen("tsi.vec","rb");
	fread(grid_res,sizeof(int),3,fp);
	float* my_vectors=new float[grid_res[0]*grid_res[1]*grid_res[2]*3];
	fread(my_vectors,sizeof(float),grid_res[0]*grid_res[1]*grid_res[2]*3,fp);
	fclose(fp);


	float maxiv=0;
	float miniv=9999;
	float * my_mags=new float[grid_res[0]*grid_res[1]*grid_res[2]];
	for(int i=0; i<grid_res[0]*grid_res[1]*grid_res[2];i++)
	{
		float fx=my_vectors[i*3+0];
		float fy=my_vectors[i*3+1];
		float fz=my_vectors[i*3+2];

		float mag=fx*fx+fy*fy+fz*fz;
		if(mag>maxiv)
			maxiv=mag;
		if((mag<miniv)&& (mag>0))
			miniv=mag;

		my_mags[i]=sqrt(fx*fx+fy*fy+fz*fz);///.02;
	}
/*	
	fp=fopen("tsi.dat","wt");
	for(int i=0;i<grid_res[0]*grid_res[1]*grid_res[2];i++)
	{
		//if(mags[i]>0)
			fprintf(fp,"%f ",my_mags[i]);
	}
	//fwrite(mags,sizeof(float),grid_res[0]*grid_res[1]*grid_res[2],fp);
	fclose(fp);
*/
	maxiv=sqrt(maxiv);
//for(int i=0;i<25;i++)
//	printf("hist[%d]=%f\n",i,hist[i]/((float)grid_res[0]*grid_res[1]*grid_res[2]));
//printf("max vel mag=%f\n",maxiv);
//printf("min vel mag=%f\n",miniv);

	for(int i=0; i<total_ver_num;i++)
	{
		VECTOR3 tmp=ver[i];
		int x=tmp.x();
		int y=tmp.y();
		int z=tmp.z();
		//float tf=1-dif[z*grid_res[0]*grid_res[1]+y*grid_res[0]+x]/125;
		//rgba[i].Set(rgba[i][0],rgba[i][1],rgba[i][2],(rgba[i][3])*mask[x+y*grid_res[0]+z*grid_res[0]*grid_res[1]]);
		int idx=x+y*grid_res[0]+z*grid_res[0]*grid_res[1];
		float fx=my_vectors[idx*3+0];
		float fy=my_vectors[idx*3+1];
		float fz=my_vectors[idx*3+2];

		float mag=sqrt(fx*fx+fy*fy+fz*fz);
		mag*=6;
		if(mag>1)
			mag=1;
		float r,g,b;
		r=mag;
		b=1-mag;
		if(mag<.5)
			g=mag*2;
		else
			g=(1-mag)*2;
		//b=1-mag;
		//rgba[i].Set(.3+r/1.5,.3+g/1.5,.3+b/1.5,rgba[i][3]);
		rgba[i].Set(rgba[i][0],rgba[i][1],rgba[i][2],rgba[i][3]/2);
	//	if(i<my_focus)
	//	rgba[i].Set(0.4,0,.2,1);
	//	else
		//rgba[i].Set(.8,0,.4,rgba[i][3]/2);
		
	}
//	delete [] mask;
	delete [] my_vectors;

	//fp=fopen("velocity.dat","wb");
	fp=fopen("F:\\xul\\illuminite lines\\examples\\viewer\\data\\velocity.dat","wb");
//	fp=fopen("C:\\Documents and Settings\\xu\\Desktop\\illuminite lines\\IL\\examples\\viewer\\data\\velocity.dat","wb");
	
	fwrite(&line_num,sizeof(int),1,fp);
	fwrite(ver_num,sizeof(int),line_num,fp);
	fwrite(ver,sizeof(VECTOR3),total_ver_num,fp);
	fwrite(rgba,sizeof(VECTOR4),total_ver_num,fp);
	fclose(fp);
	delete [] ver;
	delete [] ver_num;
	delete [] rgba;
	delete [] alpha;
	printf("end adjusting alpha\n");
	
//	delete [] dif;


}
/*
float pt2SegDist(VECTOR3 p0, VECTOR3 p1, VECTOR3 q)
{
	VECTOR3 pq,vi;
	float t;

	vi.Set(p1.x()-p0.x(), p1.y()-p0.y(),p1.z()-p0.z());
	float sqrLen=vi.GetMag();

	VECTOR3 tmp;
	tmp.Set(q.x()-p0.x(),q.y()-p0.y(),q.z()-p0.z());

	t=(tmp.x()*vi.x()+tmp.y()*vi.y()+tmp.z()*vi.z())/sqrLen;
	t=min(max(t,0),1);
	//pq=p0+t*vi
	pq.Set(	p0.x()+t*vi.x(),
			p0.y()+t*vi.y(),
			p0.z()+t*vi.z());


	float dist=sqrtf(	(pq.x()-q.x())*(pq.x()-q.x())
						+(pq.y()-q.y())*(pq.y()-q.y())
						+(pq.z()-q.z())*(pq.z()-q.z())
						);
	return dist;
}
float pt2LineDist(VECTOR3 q, vtListSeedTrace * trace,VECTOR3& p)
{
	float minDist=9999;
	VECTOR3	p0,p1;

	std::list<VECTOR3*>::iterator pnIter; 
	pnIter = trace->begin(); 
	p0 = **pnIter; 
	pnIter++;
	for (; pnIter!= trace->end(); pnIter++) 
	{
		p1 = **pnIter; 
		float dist=pt2SegDist(p0, p1, q);
		if(dist<minDist)
		{
			minDist=dist;
			p.Set(p1.x()-p0.x(), p1.y()-p0.y(), p1.z()-p0.z());
		}
		
		p0=p1;
	}

	return minDist;
}
*/
int nearestStreamline_id(vtListSeedTrace * trace, list<vtListSeedTrace*> skeleton)
{
	int ret_id=-1;
	std::list<vtListSeedTrace*>::iterator pIter; 
	std::list<VECTOR3*>::iterator pnIter; 

	pnIter = trace->begin(); 
	float minDist=9999;
	for (; pnIter!= trace->end(); pnIter++) 
	{
		VECTOR3 p = **pnIter; 
		pIter = skeleton.begin(); 
		int lineid=0;
		for (; pIter!=skeleton.end(); pIter++) 
		{
			vtListSeedTrace *curline = *pIter; 
			VECTOR3 q;
			float dist=pt2LineDist(p, curline,q);
			if(dist<minDist)
			{
				minDist=dist;
				ret_id=lineid;
			}
			lineid++;
		}
	}

	return ret_id;
}

void compute_bundles(int bundle_num)
{

	float from[3], to[3]; 

	from[0] = minLen[0];   from[1] = minLen[1];   from[2] = minLen[2]; 
	to[0] = maxLen[0];   to[1] = maxLen[1];   to[2] = maxLen[2]; 

	printf("loading seeds...\n"); 
	int nSeeds; 
	//VECTOR3* seeds=loadSeedandRender(strcat(g_filename,".data"),nSeeds);
	VECTOR3* seeds=loadSeedfromFile("myseeds.seed",nSeeds);
	if(bundle_num>=nSeeds)
		return;//nth to be bundled, bundle number too large

	osuflow->SetEntropySeedPoints( seeds,nSeeds);
	seeds = osuflow->GetSeeds(nSeeds); 


	for (int i=0; i<nSeeds; i++) 
		printf(" seed no. %d : [%f %f %f]\n", i, seeds[i][0], 
		seeds[i][1], seeds[i][2]); 

	sl_list.clear(); 
	std::vector<int> line_color;
	list<vtListSeedTrace*> half_lines, long_lines; 

	printf("compute streamlines..\n"); 
	osuflow->SetIntegrationParams(1, 5); //small and large step size
	osuflow->GenStreamLines(half_lines , BACKWARD_AND_FORWARD, 500, 0); //maxi steps
	printf(" done integrations\n"); 
	printf("list size = %d\n", half_lines.size()); 
	
	combinehalflines(half_lines,long_lines);
	sl_list.merge(long_lines);

	//initial group_ids
	int* group_ids=new int[nSeeds];
	memset(group_ids,-1,sizeof(int)*nSeeds);
	//initial the group_ids for the focus
	for(int i=0;i<bundle_num;i++)
		group_ids[i]=i;
	
	list<vtListSeedTrace*> skeleton;
	

	std::list<vtListSeedTrace*>::iterator pIter; 
	int count=0;
	pIter = sl_list.begin(); 
	for (; pIter!=sl_list.end(); pIter++) 
	{
		if(count++ >= bundle_num)
			break;//get the focal streamlines
		vtListSeedTrace *trace = *pIter; 
		skeleton.push_back(trace);
	}

	count=0;
	pIter = sl_list.begin(); 
	for (; pIter!=sl_list.end(); pIter++) 
	{
		if(count < bundle_num)
		{	count++;
			continue;//skip the focal streamlines
		}

		vtListSeedTrace *trace = *pIter; 
		int id=nearestStreamline_id(trace, skeleton);
		group_ids[count]=id;
		printf("group id=%d\n",id);
		printf("%d/%d\r",count,sl_list.size());
		count++;
	}

	for(int i=0;i<sl_list.size();i++)
		line_color.push_back(group_ids[i]);
	save_streamlines_to_file_hand_tuning("test.dat",line_color,sl_list.size());


}

//random lines
void compute_streamlines_random() {

	float from[3], to[3]; 

	from[0] = minLen[0];   from[1] = minLen[1];   from[2] = minLen[2]; 
	to[0] = maxLen[0];   to[1] = maxLen[1];   to[2] = maxLen[2]; 

//	from[0] = 42;   from[1] = 42;   from[2] = minLen[2]; 
//	to[0] = 84;   to[1] = 84;   to[2] = maxLen[2]; 


	printf("generating seeds...\n"); 
	osuflow->SetRandomSeedPoints(from, to, 100); 
	int nSeeds; 
	VECTOR3* seeds = osuflow->GetSeeds(nSeeds); 
	std::vector<VECTOR3> myseeds;

	for (int i=0; i<nSeeds; i++) 
	{
		printf(" seed no. %d : [%f %f %f]\n", i, seeds[i][0], 
		seeds[i][1], seeds[i][2]); 
		seed_list.push_back(VECTOR3(seeds[i][0], seeds[i][1], seeds[i][2]));
	}
	sl_list.clear(); 
	dumpSeeds( myseeds, "myseeds.seed");

	printf("compute streamlines..\n"); 
	osuflow->SetIntegrationParams(minstepsize, maxstepsize); //small and large step size

	list<vtListSeedTrace*> lines,new_lines; 
	osuflow->GenStreamLines(lines , BACKWARD_AND_FORWARD, maxi_stepnum, 0); //maxi steps

		//printf("compute streamlines..\n"); 
	combinehalflines(lines, sl_list);

/*
	float* vectors;
	int grid_res[3];
	FILE* fp=fopen(g_filename,"rb");
	fread(grid_res,sizeof(int),3,fp);

	vectors=new float[grid_res[0]*grid_res[1]*grid_res[2]*3];
	fread(vectors,sizeof(float),grid_res[0]*grid_res[1]*grid_res[2]*3,fp);
	fclose(fp);

	float * new_vectors=new float[grid_res[0]*grid_res[1]*grid_res[2]*3];
	memset(new_vectors,0,sizeof(float)*grid_res[0]*grid_res[1]*grid_res[2]*3);
	std::list<vtListSeedTrace*>::iterator pIter; 
	pIter = sl_list.begin(); 
	for (; pIter!=sl_list.end(); pIter++) 
	{
		vtListSeedTrace *trace = *pIter; 
		std::list<VECTOR3*>::iterator pnIter,pnIter2; 
	
		pnIter = trace->begin(); 
		for (; pnIter!= trace->end(); pnIter++) {
			VECTOR3 p = **pnIter; 
			int x=p.x(); int y=p.y(); int z=p.z();
			int idx=x+y*grid_res[0]+z*grid_res[0]*grid_res[1];
			new_vectors[3*idx+0]=vectors[3*idx+0];
			new_vectors[3*idx+1]=vectors[3*idx+1];
			new_vectors[3*idx+2]=vectors[3*idx+2];
		}

	}
	fp=fopen("test.vec","wb");//testing for gpu diffusion
	fwrite(grid_res,sizeof(int),3,fp);
	fwrite(new_vectors,sizeof(float),grid_res[0]*grid_res[1]*grid_res[2]*3,fp);
	fclose(fp);
	printf("done\n");
	delete [] new_vectors;
	delete [] vectors;
*/
	printf(" done integrations\n"); 
	printf("list size = %d\n", sl_list.size()); 
	myseeds.clear();
	//save_streamlines_to_file();
	std::vector<int> line_color;
	for(int i=0;i<sl_list.size();i++)
		line_color.push_back(i);
	int grid_res[3];
	float* vectors=get_grid_vec_data(grid_res);//get vec data at each grid point

	int* old_bin, *new_bin;
	old_bin=new int [grid_res[0]*grid_res[1]*grid_res[2]];

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
		printf("entropy calculation done\n");
	}		
	delete [] old_bin;
	delete [] vectors;
//	save_streamlines_to_file_hand_tuning("test.dat",line_color,sl_list.size());

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
VECTOR3	selectNextSeed(float* vectors,float* new_vectors, int* grid_res,OCTree* Tree, float* theta, float* phi,int* old_bin, int* new_bin)
{


	VECTOR3	selected_seed_id;
	int x_min,x_max,y_min,y_max, z_min, z_max;

	//printf("find max entropy node \n");
	
	Tree->CalcEntropy_FindMaxEntropyNode(vectors,new_vectors, grid_res,x_min,x_max,y_min,y_max,z_min,z_max ,theta,phi,old_bin,new_bin,NULL);
	
			//about to end?
	//float entropy=calcRelativeEntropy6(	vectors, new_vectors,  grid_res, VECTOR3(0,0,0),
	//								VECTOR3(grid_res[0],grid_res[1],grid_res[2]), theta, phi,old_bin,new_bin);
/*	float avg_entropy=0;	
	float target_entropy=1.31778;//0.1->1.31778  0.05->0.71  0.2->2.419
	printf("entropy=%f/%f\n",avg_entropy,target_entropy);
	if(avg_entropy<target_entropy)
		return VECTOR3(-1,-1,-1);*/
	//locate the cell with highest entropy
//	printf("maxi entropy node= %d %d %d %d %d %d\r",x_min,x_max,y_min,y_max,z_min,z_max);

	//calculate the max entropy
	VECTOR3 startpt,endpt;
	startpt.Set(x_min, y_min, z_min);
	endpt.Set(x_max, y_max, z_max);

//	float entropy=calcPoint2PointError( vectors, new_vectors, grid_res, 
//										startpt, endpt);

	//float entropy=calcRelativeEntropy6( vectors, new_vectors, grid_res, 
			//							startpt, endpt, theta, phi,old_bin,new_bin);
	//float target_entropy=0.71;//1.31778;//0.1->1.31778  0.05->0.71  0.2->2.419
	//printf("entropy=%f/%f\r",entropy,target_entropy);
	//if(entropy<target_entropy)
	//	return VECTOR3(-1,-1,-1);//stop the seeding
						
	
	//get the point of maximum error
	float max_error=0;
	VECTOR3 max_pt;
	max_pt.Set(x_min,y_min,z_min);
	bool found=false;
	for(int z=z_min; z<z_max;z++)
	{
		for(int y=y_min; y<y_max;y++)
		{
			for(int x=x_min; x<x_max;x++)
			{
				int idx=x+y*grid_res[0]+z*grid_res[0]*grid_res[1];
				VECTOR3 ori,newf, t;
				ori.Set(vectors[idx*3+0],vectors[idx*3+1],vectors[idx*3+2]);
				newf.Set(new_vectors[idx*3+0],new_vectors[idx*3+1],new_vectors[idx*3+2]);
				t=ori-newf;
				float mg=t.GetMag();
				if(mg<0.00001)//take as a cp, do not pick it
					continue;
				//do not pick boundary point either
				if( z_min==0 || x_min==0 || y_min==0 ||
					z_max==grid_res[2]-1 ||
					y_max==grid_res[1]-1 ||
					x_max==grid_res[0]-1 )
					continue;

				if(mg>max_error)//find the point of most error, careful do not hit critical point
				{
					max_error=mg;
					max_pt.Set(x,y,z);
					found=true;
				}
			}
		}
	}
	if(found==false)
	{
	//	printf("first trial looking for seed failed :( \n");
		//avoid select it again.
		for(int z=z_min; z<z_max;z++)
		{
			for(int y=y_min; y<y_max;y++)
			{
				for(int x=x_min; x<x_max;x++)
				{
					int idx=x+y*grid_res[0]+z*grid_res[0]*grid_res[1];
					for(int i=0; i<3; i++)
					new_vectors[idx*3+i]=vectors[idx*3+i];
					
				}
			}
		}
	}

	return max_pt;
}
typedef std::vector<int> vtListVertex;
/*
float evaluate_error(VECTOR3 pos,std::vector<vtListVertex*>	Graph,float* vectors,float* new_vectors,int* grid_res)
{

	int idx=pos.x()+((int)pos.y())*grid_res[0]+((int)pos.z())*grid_res[0]*grid_res[1];
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
			ori.Set(vectors[nodeid*3+0],vectors[nodeid*3+1],vectors[nodeid*3+2]);
			newf.Set(new_vectors[nodeid*3+0],new_vectors[nodeid*3+1],new_vectors[nodeid*3+2]);
			ori.Normalize();
			newf.Normalize();

			t=ori-newf;
			mg=mg+(t.GetMag()*fweight)*(4-i)*0.25;
		
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
}//gradient descend 
*/
float evaluate_error(VECTOR3 pos,std::vector<vtListVertex*>	Graph,float* vectors,float* new_vectors,int* grid_res)
{
	float mg=0;//error magnitude

	VECTOR3 seed;
	seed.Set(pos.x(),pos.y(),pos.z());
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
			
			int nodeid=xx+yy*grid_res[0]+zz*grid_res[0]*grid_res[1];

			VECTOR3 ori,newf, t;
			ori.Set(vectors[nodeid*3+0],vectors[nodeid*3+1],vectors[nodeid*3+2]);
			newf.Set(new_vectors[nodeid*3+0],new_vectors[nodeid*3+1],new_vectors[nodeid*3+2]);
			ori.Normalize();
			newf.Normalize();

			t=ori-newf;
			mg=mg+(t.GetMag());

		}
	}
	for (; pIter!=lines.end(); pIter++) 
	{
		vtListSeedTrace *trace = *pIter; 
		delete trace;
	}
	
	return mg;
}

void dump_entrpy_every_point(OCTree* Tree,float* entropy_every_point,int* grid_res,
							 float* vectors, float* new_vectors, float* theta, float* phi, int* oldbin, int* newbin,int* occupied,
							 VECTOR3& st, VECTOR3& ed,int* g_histo,int* g_histo_puv)
{
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
//printf("leaf size=%f\n",end.x()-start.x());
		float entropy=1;
		/*float entropy=calcRelativeEntropy6( vectors,new_vectors, grid_res, 
				VECTOR3(start.x(),start.y(),start.z()),
				VECTOR3(end.x(),end.y(),end.z()),
				theta, phi,
				oldbin, newbin,occupied,g_histo,g_histo_puv);
		*/if(occupied)//add density control
		{
			int nonempty=0;
			for(int z=start.z();z<end.z();z++)
			for(int y=start.y();y<end.y();y++)
			for(int x=start.x();x<end.x();x++)
				{
					if(occupied[x+y*grid_res[0]+z*grid_res[0]*grid_res[1]]!=0)
						nonempty++;
				}
	
			//float pv=((float)nonempty)/((float)(end.x()-start.x())*(end.y()-start.y())*(end.z()-start.z()));
		//	printf("density control=%f\n",1-pv);
			float pv=1;
			if(nonempty)
				pv=0;
		//	if(entropy<1)
		//	entropy=entropy*.2+pv*0.8;
		}	

		entropy_leaves[i]=entropy;

		if(max<entropy)
		{
			max=entropy;
			st=start;
			ed=end;
		}
		for(int z=start.z(); z<end.z();z++)
		for(int y=start.y(); y<end.y();y++)
		for(int x=start.x(); x<end.x();x++)
		{
			int idx=x+y*grid_res[0]+z*grid_res[0]*grid_res[1];
			entropy_every_point[idx]=entropy;
			
		}
	}
	if(max>0)
	{
		for(int i=0;i<leaves.size()/2;i++)
		{
			VECTOR3 start,end;
			start=leaves[i*2+0];
			end=leaves[i*2+1];

			for(int z=start.z(); z<end.z();z++)
			for(int y=start.y(); y<end.y();y++)
			for(int x=start.x(); x<end.x();x++)
			{
				int idx=x+y*grid_res[0]+z*grid_res[0]*grid_res[1];
				entropy_every_point[idx]/=max;
			}
		}
	}
}


VECTOR3	selectNextSeed3(float* vectors,float* new_vectors, int* grid_res,OCTree* Tree, float* theta, float* phi,int* old_bin, int* new_bin,
						std::vector<vtListVertex*>	Graph,int* occupied,int* g_histo,int* g_histo_puv)
{
	VECTOR3	selected_seed_id,st,ed;
	float entropy;
	int x_min,x_max,y_min,y_max, z_min, z_max;
	
//	Tree->CalcEntropy_LeafNode(vectors,new_vectors, grid_res,x_min,x_max,y_min,y_max,z_min,z_max ,theta,phi,old_bin,new_bin,occupied);
//	Tree->CalcEntropy_InnerNode();
//	Tree->FindMaxEntropyNode(x_min,x_max,y_min,y_max, z_min, z_max);

	dump_entrpy_every_point(Tree,entropy_every_point,grid_res,
							 vectors, new_vectors, theta, phi, old_bin, new_bin,occupied,st,ed,g_histo,g_histo_puv);

	x_min=st.x(); x_max=ed.x();
	y_min=st.y(); y_max=ed.y();
	z_min=st.z(); z_max=ed.z();
	
//	Tree->CalcEntropy_FindMaxEntropyNode(vectors,new_vectors, grid_res,x_min,x_max,y_min,y_max,z_min,z_max ,theta,phi,old_bin,new_bin,occupied);

	//get the point of maximum error
	float max_error=0;
	VECTOR3 max_pt;
	bool found=false;
	//for(int z=z_min; z<z_max;z++)
	//start from the middle
	int z=(z_min+z_max)/2;
	int y=(y_min+y_max)/2;
	int x=(x_min+x_max)/2;
	max_pt.Set(x,y,z);

	int idx=x+y*grid_res[0]+z*grid_res[0]*grid_res[1];
	float cur_err= evaluate_error(VECTOR3(x,y,z),Graph,vectors,new_vectors,grid_res);
	while(1)
	{
		bool found=false;
		for(int nz=-1;nz<=1;nz++)
		{
			for(int ny=-1;ny<=1;ny++)
			{
				for(int nx=-1;nx<=1;nx++)
				{
					if(nx+x<x_min||nx+x>x_max||ny+y<y_min||ny+y>y_max||nz+z<z_min||nz+z>z_max)
					{
						return max_pt;
					}
					
					if((nx==0) &&(ny==0)&&(nz==0))//self
						continue;
					int idx1=x+nx+(y+ny)*grid_res[0]+(z+nz)*grid_res[0]*grid_res[1];
					float new_err= evaluate_error(VECTOR3(x+nx,y+ny,z+nz),Graph,vectors,new_vectors,grid_res);
					if(new_err>cur_err)//walk
					{
						x=nx+x;
						y=ny+y;
						z=nz+z;
						max_pt.Set(x,y,z);
						cur_err=new_err;
				
					}
					
				}
				
			}
		}
		
		if(found==false)//did not find the next 
			break;
			
		
	}
	return max_pt;
}

/*

exhause search
VECTOR3	selectNextSeed3(float* vectors,float* new_vectors, int* grid_res,OCTree* Tree, float* theta, float* phi,int* old_bin, int* new_bin,
						std::vector<vtListVertex*>	Graph)
{
	VECTOR3	selected_seed_id;
	int x_min,x_max,y_min,y_max, z_min, z_max;

	//printf("find max entropy node \n");
	Tree->CalcEntropy_FindMaxEntropyNode(vectors,new_vectors, grid_res,x_min,x_max,y_min,y_max,z_min,z_max ,theta,phi,old_bin,new_bin);
	
	//get the point of maximum error
	float max_error=0;
	VECTOR3 max_pt;
	max_pt.Set(x_min,y_min,z_min);
	bool found=false;
	for(int z=z_min; z<z_max;z++)
	{
		for(int y=y_min; y<y_max;y++)
		{
			for(int x=x_min; x<x_max;x++)
			{
				int idx=x+y*grid_res[0]+z*grid_res[0]*grid_res[1];

				if( z_min==0 || x_min==0 || y_min==0 ||
					z_max==grid_res[2]-1 ||
					y_max==grid_res[1]-1 ||
					x_max==grid_res[0]-1 )
					continue;
				float mg=0;//error magnitude
				vtListVertex*	neighbors=Graph[idx];
				vtListVertex	current_neighbors=*neighbors;
				vtListVertex	next_level_neighbors;
				for(int i=0;i<4; i++)//see three layers of neighbors
				{
					for(int j=0;j<current_neighbors.size()/2;j++)
					{
						int nodeid=current_neighbors[j*2+0];
						int weight=current_neighbors[j*2+1];
						float fweight=((float)weight/100.0);
			//			int nodeid=idx;
						VECTOR3 ori,newf, t;
						ori.Set(vectors[nodeid*3+0],vectors[nodeid*3+1],vectors[nodeid*3+2]);
						newf.Set(new_vectors[nodeid*3+0],new_vectors[nodeid*3+1],new_vectors[nodeid*3+2]);
						ori.Normalize();
						newf.Normalize();

						t=ori-newf;
						mg=mg+t.GetMag()*fweight;
					
						vtListVertex	tmp_neighbors=*(Graph[nodeid]);
						for(int t=0;t<tmp_neighbors.size(); t++)
						{
							next_level_neighbors.push_back(tmp_neighbors[t]);
						}
					}
					current_neighbors.clear();
					current_neighbors=next_level_neighbors;
				}
				if(mg>max_error)//find the point of most error, careful do not hit critical point
				{
					max_error=mg;
					max_pt.Set(x,y,z);
					//found=true;
				}
			}
		}
	}

	return max_pt;
}
*/

void output_entropy(OCTree* Tree, float* vectors, int* grid_res, float* theta,float* phi)
{
	
	float* result=new float[grid_res[0]*grid_res[1]*grid_res[2]];
	for(int i=0; i<grid_res[0]*grid_res[1]*grid_res[2];i++)
		result[i]=0;
	Tree->calcEntropy( result,  vectors, grid_res,theta,phi,binnum);
	float maxi=0;
	for(int i=0; i<grid_res[0]*grid_res[1]*grid_res[2];i++)
		if(result[i]>maxi)
			maxi=result[i];
	for(int i=0; i<grid_res[0]*grid_res[1]*grid_res[2];i++)
		result[i]=result[i]/maxi;
	FILE* fp=fopen("scalar.dat","wb");
	fwrite(grid_res,sizeof(int),3,fp);
	fwrite(result, sizeof(float), grid_res[0]*grid_res[1]*grid_res[2],fp);
	fclose(fp);

	delete [] result;
	
}




void hand_tuning_streamlines()
{
	
	printf("generating seeds...\n"); 
	int num=0;
	int grid_res[3];
	//Note: the vector is normalized
	float* vectors=get_grid_vec_data(grid_res);//get vec data at each grid point

												//load seeds from file
	int nSeeds; 
	std::vector<int> line_color;
	int c=0;
	int color=0;
	//set first seed as domain center
	vector<VECTOR3> seeds;

	int minx,miny,minz,maxx,maxy,maxz;
	VECTOR3* newseeds;
	list<vtListSeedTrace*> lines,new_lines; 
	int off=5;
	minx=48+off; miny=48+off; minz=0+off;
	maxx=63-off; maxy=63-off; maxz=15-off;
	for(int x=minx; x<maxx;x++)
	{
		for(int y=miny; y<maxy;y++)
		{
			for(int z=minz; z<maxz;z++)
			{
				seeds.push_back(VECTOR3(x,y,z));
				c++;
			}
		}
	}
		
	newseeds=new VECTOR3[seeds.size()];
	for(int i=0;i<seeds.size();i++)
	{
		newseeds[i].Set(seeds[i].x(),seeds[i].y(),seeds[i].z());
	}
	sl_list.clear(); 
	printf("compute streamlines..\n"); 
	osuflow->SetEntropySeedPoints( newseeds,c);
	osuflow->SetIntegrationParams(1, 5);							//small and large step size
	osuflow->GenStreamLines(lines , BACKWARD_AND_FORWARD, 500, 0);	 //maxi steps
	//printf(" done integrations\n"); 
	delete [] newseeds;
	//combine the half lines
	combinehalflines(lines, sl_list);
	for(int i=0;i<lines.size()/2;i++)
		line_color.push_back(color);
	//sl_list.merge(new_lines);
	color++;
	//line_color.push_back(color);


	///////////////////////////////////////
	c=0;
	seeds.clear();
	minx=48+off; miny=64+off; minz=0+off;
	maxx=63-off; maxy=79-off; maxz=15-off;
	for(int x=minx; x<maxx;x++)
	{
		for(int y=miny; y<maxy;y++)
		{
			for(int z=minz; z<maxz;z++)
			{
				seeds.push_back(VECTOR3(x,y,z));
				c++;
			}
		}
	}
	newseeds=new VECTOR3[seeds.size()];
	for(int i=0;i<seeds.size();i++)
	{
		newseeds[i].Set(seeds[i].x(),seeds[i].y(),seeds[i].z());
	}
	//lines.clear(); 

	printf("compute streamlines..\n"); 
	osuflow->SetEntropySeedPoints( newseeds,c);
	osuflow->SetIntegrationParams(1, 5);							//small and large step size
	osuflow->GenStreamLines(lines , BACKWARD_AND_FORWARD, 500, 0);	 //maxi steps
	printf(" done integrations\n"); 
	delete [] newseeds;
	combinehalflines(lines, sl_list);
	for(int i=0;i<lines.size()/2;i++)
		line_color.push_back(color);
	color++;
//	sl_list.merge(new_lines);

	///////////////////////////////////////
	c=0;
	seeds.clear();
	minx=64+off; miny=64+off; minz=0+off;
	maxx=79-off; maxy=79-off; maxz=15-off;
	for(int x=minx; x<maxx;x++)
	{
		for(int y=miny; y<maxy;y++)
		{
			for(int z=minz; z<maxz;z++)
			{
				seeds.push_back(VECTOR3(x,y,z));
				c++;
			}
		}
	}
	newseeds=new VECTOR3[seeds.size()];
	for(int i=0;i<seeds.size();i++)
	{
		newseeds[i].Set(seeds[i].x(),seeds[i].y(),seeds[i].z());
	}
	//lines.clear(); 

	printf("compute streamlines..\n"); 
	osuflow->SetEntropySeedPoints( newseeds,c);
	osuflow->SetIntegrationParams(1, 5);							//small and large step size
	osuflow->GenStreamLines(lines , BACKWARD_AND_FORWARD, 500, 0);	 //maxi steps
	printf(" done integrations\n"); 
	delete [] newseeds;
	//combine the half lines
	combinehalflines(lines, sl_list);
	for(int i=0;i<lines.size()/2;i++)
		line_color.push_back(color);
	color++;
//	sl_list.merge(new_lines);

off=5;
/*
	///////////////////////////////////////
	c=0;
	seeds.clear();
	minx=48+off; miny=32+off; minz=352+off;
	maxx=63-off; maxy=47-off; maxz=367-off;
	for(int x=minx; x<maxx;x++)
	{
		for(int y=miny; y<maxy;y++)
		{
			for(int z=minz; z<maxz;z++)
			{
				seeds.push_back(VECTOR3(x,y,z));
				c++;
			}
		}
	}
	newseeds=new VECTOR3[seeds.size()];
	for(int i=0;i<seeds.size();i++)
	{
		newseeds[i].Set(seeds[i].x(),seeds[i].y(),seeds[i].z());
	}
	//lines.clear(); 

	printf("compute streamlines..\n"); 
	osuflow->SetEntropySeedPoints( newseeds,c);
	osuflow->SetIntegrationParams(1, 5);							//small and large step size
	osuflow->GenStreamLines(lines , BACKWARD_AND_FORWARD, 500, 0);	 //maxi steps
	printf(" done integrations\n"); 
	delete [] newseeds;
	//combine the half lines
	combinehalflines(lines, sl_list);
	for(int i=0;i<lines.size()/2;i++)
		line_color.push_back(color);
	color++;*/
	//sl_list.merge(new_lines);

	///////////////////////////////////////
	c=0;
	seeds.clear();
	minx=48+off; miny=64+off; minz=352+off;
	maxx=63-off; maxy=79-off; maxz=367-off;
	for(int x=minx; x<maxx;x++)
	{
		for(int y=miny; y<maxy;y++)
		{
			for(int z=minz; z<maxz;z++)
			{
				seeds.push_back(VECTOR3(x,y,z));
				c++;
			}
		}
	}
	newseeds=new VECTOR3[seeds.size()];
	for(int i=0;i<seeds.size();i++)
	{
		newseeds[i].Set(seeds[i].x(),seeds[i].y(),seeds[i].z());
	}
	//lines.clear(); 

	printf("compute streamlines..\n"); 
	osuflow->SetEntropySeedPoints( newseeds,c);
	osuflow->SetIntegrationParams(1, 5);							//small and large step size
	osuflow->GenStreamLines(lines , BACKWARD_AND_FORWARD, 500, 0);	 //maxi steps
	printf(" done integrations\n"); 
	delete [] newseeds;
	//combine the half lines
	combinehalflines(lines, sl_list);
	for(int i=0;i<lines.size()/2;i++)
		line_color.push_back(color);
	color++;
	//sl_list.merge(new_lines);

///////////////////////////////////////
	c=0;
	seeds.clear();
	minx=32+off; miny=112+off; minz=400+off;
	maxx=47-off; maxy=124-off; maxz=415-off;
	for(int x=minx; x<maxx;x++)
	{
		for(int y=miny; y<maxy;y++)
		{
			for(int z=minz; z<maxz;z++)
			{
				seeds.push_back(VECTOR3(x,y,z));
				c++;
			}
		}
	}
	newseeds=new VECTOR3[seeds.size()];
	for(int i=0;i<seeds.size();i++)
	{
		newseeds[i].Set(seeds[i].x(),seeds[i].y(),seeds[i].z());
	}
	//lines.clear(); 

	printf("compute streamlines..\n"); 
	osuflow->SetEntropySeedPoints( newseeds,c);
	osuflow->SetIntegrationParams(1, 5);							//small and large step size
	osuflow->GenStreamLines(lines , BACKWARD_AND_FORWARD, 500, 0);	 //maxi steps
	printf(" done integrations\n"); 
	delete [] newseeds;
	//combine the half lines
	combinehalflines(lines, sl_list);
	for(int i=0;i<lines.size()/2;i++)
		line_color.push_back(color);
	color++;
	//sl_list.merge(new_lines);

///////////////////////////////////////
	c=0;
	seeds.clear();
	minx=96+off; miny=64+off; minz=400+off;
	maxx=111-off; maxy=79-off; maxz=415-off;
	for(int x=minx; x<maxx;x++)
	{
		for(int y=miny; y<maxy;y++)
		{
			for(int z=minz; z<maxz;z++)
			{
				seeds.push_back(VECTOR3(x,y,z));
				c++;
			}
		}
	}
	newseeds=new VECTOR3[seeds.size()];
	for(int i=0;i<seeds.size();i++)
	{
		newseeds[i].Set(seeds[i].x(),seeds[i].y(),seeds[i].z());
	}
	//lines.clear(); 

	printf("compute streamlines..\n"); 
	osuflow->SetEntropySeedPoints( newseeds,c);
	osuflow->SetIntegrationParams(1, 5);							//small and large step size
	osuflow->GenStreamLines(lines , BACKWARD_AND_FORWARD, 500, 0);	 //maxi steps
	printf(" done integrations\n"); 
	delete [] newseeds;
	//combine the half lines
	combinehalflines(lines, sl_list);
	for(int i=0;i<lines.size()/2;i++)
		line_color.push_back(color);
	color++;

	///////////////////////////////////////
	c=0;
	seeds.clear();
	minx=80+off; miny=16+off; minz=144+off;
	maxx=95-off; maxy=31-off; maxz=159-off;
	for(int x=minx; x<maxx;x++)
	{
		for(int y=miny; y<maxy;y++)
		{
			for(int z=minz; z<maxz;z++)
			{
				seeds.push_back(VECTOR3(x,y,z));
				c++;
			}
		}
	}
	newseeds=new VECTOR3[seeds.size()];
	for(int i=0;i<seeds.size();i++)
	{
		newseeds[i].Set(seeds[i].x(),seeds[i].y(),seeds[i].z());
	}
	//lines.clear(); 

	printf("compute streamlines..\n"); 
	osuflow->SetEntropySeedPoints( newseeds,c);
	osuflow->SetIntegrationParams(1, 5);							//small and large step size
	osuflow->GenStreamLines(lines , BACKWARD_AND_FORWARD, 500, 0);	 //maxi steps
	printf(" done integrations\n"); 
	delete [] newseeds;
	//combine the half lines
	combinehalflines(lines, sl_list);
	for(int i=0;i<lines.size()/2;i++)
		line_color.push_back(color);
	color++;

	///////////////////////////////////////
	c=0;
	seeds.clear();
	minx=16+off; miny=32+off; minz=128+off;
	maxx=31-off; maxy=47-off; maxz=143-off;
	for(int x=minx; x<maxx;x++)
	{
		for(int y=miny; y<maxy;y++)
		{
			for(int z=minz; z<maxz;z++)
			{
				seeds.push_back(VECTOR3(x,y,z));
				c++;
			}
		}
	}
	newseeds=new VECTOR3[seeds.size()];
	for(int i=0;i<seeds.size();i++)
	{
		newseeds[i].Set(seeds[i].x(),seeds[i].y(),seeds[i].z());
	}
	//lines.clear(); 

	printf("compute streamlines..\n"); 
	osuflow->SetEntropySeedPoints( newseeds,c);
	osuflow->SetIntegrationParams(1, 5);							//small and large step size
	osuflow->GenStreamLines(lines , BACKWARD_AND_FORWARD, 500, 0);	 //maxi steps
	printf(" done integrations\n"); 
	delete [] newseeds;
	//combine the half lines
	combinehalflines(lines, sl_list);
	for(int i=0;i<lines.size()/2;i++)
		line_color.push_back(color);
	color++;

	//add transparent random lines here
/*
	float from[3], to[3]; 

	from[0] =0;   from[1] = 0;   from[2] = 0; 
	to[0] = 125;   to[1] = 125;   to[2] = 511; 

	printf("generating seeds...\n"); 
	osuflow->SetRandomSeedPoints(from, to, 1000); 

	printf("compute streamlines..\n"); 
	osuflow->SetIntegrationParams(1, 5); //small and large step size
	osuflow->GenStreamLines(lines , BACKWARD_AND_FORWARD, 200, 0); //maxi steps
	combinehalflines(lines, new_lines);
	printf(" done integrations\n"); 
	for(int i=0;i<new_lines.size();i++)
		line_color.push_back(100);
	sl_list.merge(new_lines);
*/

}

bool isvalid_context(int* occupied, list<vtListSeedTrace*> context_list, int* dim)
{
	int dist_thres=2;//distance=thres*2

	//check each sample point on the line
	std::list<vtListSeedTrace*>::iterator pIter; 
	pIter = context_list.begin(); 

	vtListSeedTrace *trace = *pIter; 
	std::list<VECTOR3*>::iterator pnIter; 

	pnIter = trace->begin(); 
	for (; pnIter!= trace->end(); pnIter++) 
	{
		VECTOR3 p = **pnIter; 
		int x=p[0]; int y=p[1]; int z=p[2]; 
		for(int cx=x-dist_thres; cx<x+dist_thres;cx++)
		{
			for(int cy=y-dist_thres; cy<y+dist_thres;cy++)
			{
				for(int cz=z-dist_thres; cz<z+dist_thres;cz++)
				{
					//out of bound?
					if( cx<0 || cy<0 || cz<0 ||
						cx>=dim[0]-1 ||cy>=dim[1]-1 ||cz>=dim[2]-1 )
						continue;
					//is there another line nearby?
					int idx=cx+cy*dim[0]+cz*dim[0]*dim[1];
					if(occupied[idx]==1)
						return false;
				}
			}
		}

	}
	//update occupied
	pnIter = trace->begin(); 
	for (; pnIter!= trace->end(); pnIter++) 
	{
		VECTOR3 p = **pnIter; 
		int x=p[0]; int y=p[1]; int z=p[2]; 
		occupied[x+y*dim[0]+z*dim[0]*dim[1]]=1;
	}	
	return true;
}
/*
void add_context_streamlines(std::vector<int>& line_color)
{
	float from[3], to[3]; 

	from[0] = minLen[0];   from[1] = minLen[1];   from[2] = minLen[2]; 
	to[0] = maxLen[0];   to[1] = maxLen[1];   to[2] = maxLen[2]; 

	int nSeeds=100; 
	int count=0;
	int grid_res[3];
	grid_res[0]=to[0]-from[0];	grid_res[1]=to[1]-from[1];	grid_res[2]=to[2]-from[2];
	int * occupied=new int[grid_res[0]*grid_res[1]*grid_res[2]];
	memset(occupied,0,sizeof(int)*grid_res[0]*grid_res[1]*grid_res[2]);
	int iteration=0;
	while(count<nSeeds && iteration<100)
	{
		osuflow->SetRandomSeedPoints(from, to, 1); 
		list<vtListSeedTrace*> context_list_half,context_list; 
		
		//printf("compute streamlines..\n"); 
		osuflow->SetIntegrationParams(1, 5); //small and large step size
		osuflow->GenStreamLines(context_list_half , BACKWARD_AND_FORWARD, 500, 0); //maxi steps
		//printf(" done integrations\n"); 

		combinehalflines(context_list_half,context_list);
		if(true==isvalid_context(occupied, context_list,grid_res))
		{
			iteration=0;
			sl_list.push_back(*(context_list.begin()));
			line_color.push_back(100+count);//transparent seeds
			count++;

			printf("generating context seeds...%d\r",count); 
			fflush(stdout);			
		}
		iteration++;
	}

	delete [] occupied;

}
*/
void add_context_streamlines(std::vector<int>& line_color)
{
	float from[3], to[3]; 

	from[0] = minLen[0];   from[1] = minLen[1];   from[2] = minLen[2]; 
	to[0] = maxLen[0];   to[1] = maxLen[1];   to[2] = maxLen[2]; 

	int grid_res[3];
	grid_res[0]=to[0]-from[0];	grid_res[1]=to[1]-from[1];	grid_res[2]=to[2]-from[2];
	//1. read the saved seeds
	int snum;
	FILE* fp=fopen("myseeds.seed","rt");
	fscanf(fp,"%d",&snum);
	VECTOR3 * xyz;
	xyz=new VECTOR3[snum];
	for(int i=0;i<snum;i++)
	{
		float fx,fy,fz;
		fscanf(fp,"%f",&fx);
		fscanf(fp,"%f",&fy);
		fscanf(fp,"%f",&fz);

		xyz[i].Set(fx,fy,fz);
	}
	fclose(fp);
	
	//read the vectors
	float* vectors;
	fp=fopen(g_filename,"rb");
	fread(grid_res,sizeof(int),3,fp);

	vectors=new float[grid_res[0]*grid_res[1]*grid_res[2]*3];
	fread(vectors,sizeof(float),grid_res[0]*grid_res[1]*grid_res[2]*3,fp);
	fclose(fp);

	//2. set the seeds to form "ribbons"
	int ribbon_width=2;//half width

	int count=0;
	VECTOR3* newseeds=new VECTOR3[snum*(ribbon_width*2)*(ribbon_width*2)];
	for(int i=0;i<snum;i++)
	{
		VECTOR3 aseed=xyz[i];
		//get flow direction at the seed point
		int x=aseed.x(); int y=aseed.y(); int z=aseed.z(); 
		int idx=x+y*grid_res[0]+z*grid_res[0]*grid_res[1];
		float dir_x=vectors[idx*3+0];
		float dir_y=vectors[idx*3+1];


		for(int j=-ribbon_width;j<ribbon_width;j++)
		{
			float sx,sy,sz;
		/*	if(dir_x)
				sx=aseed.x()-dir_y/dir_x*j;
			else
				sx=aseed.x();
*/
			sx=min(max(0,aseed.x()+j),grid_res[0]-1);

			for(int k=-ribbon_width;k<ribbon_width;k++)
			{
				if(j==0 && k==0)
					continue;
				sz=aseed.z();//keep the original z
				sy=min(max(0,aseed.y()+k),grid_res[1]-1);//get the y dir
				newseeds[count++].Set(sx,sy,sz);
				line_color.push_back(i);//transparent seeds
			}
		}
		printf("generating context seeds...%d\r",i); 

	}
	delete [] xyz;
	delete [] vectors;

	list<vtListSeedTrace*> lines; 
	//printf("compute streamlines..\n"); 
	osuflow->SetEntropySeedPoints( newseeds,count);
	osuflow->SetIntegrationParams(1, 5);							//small and large step size
	osuflow->GenStreamLines(lines , BACKWARD_AND_FORWARD, 100, 0);	 //maxi steps
	//printf(" done integrations\n"); 
	delete [] newseeds;
	/*
	//cut boundary lines short
	int bound=15;//for plume 15, smaller data set should be zero
	std::list<vtListSeedTrace*>::iterator pIter; 
	pIter = lines.begin(); 
	for (; pIter!=lines.end(); pIter++) {
		vtListSeedTrace *trace = *pIter; 
		std::list<VECTOR3*>::iterator pnIter; 
		std::list<VECTOR3*>::iterator pnIter_end=trace->end(); 

		int count_s=0;
		pnIter = trace->begin(); 
		for (; pnIter!= trace->end(); pnIter++) {
			VECTOR3 p = **pnIter; 
			if(p.x()<bound || p.y()<bound || p.x()>grid_res[0]-bound ||p.y()>grid_res[1]-bound )//cut it short
			{
				if(count_s>5)
				{
				trace->erase(pnIter,pnIter_end);
				break;
				}
			}
			count_s++;
		}

	}
*/
	int oldlinenum=sl_list.size();
	combinehalflines(lines, sl_list);

//	for(int i=0;i<sl_list.size()-oldlinenum;i++)
//		line_color.push_back(100+i/(ribbon_width*2));//transparent seeds


}
	 
void update_score(float* score,float* vectors,float* new_vectors, list<vtListSeedTrace*> long_lines, 
				  int* grid_res, int line_num)
{
	std::list<vtListSeedTrace*>::iterator myIter; 
	myIter=long_lines.begin();
	vtListSeedTrace *next;
	int line_no=0;
	for(;myIter!=long_lines.end();myIter++)
	{
		vtListSeedTrace *trace = *myIter; 
		float newval=0;//calculate the point-to-point error

		std::list<VECTOR3*>::iterator pnIter; 

		pnIter = trace->begin(); 
		for (; pnIter!= trace->end(); pnIter++) {
			VECTOR3 p = **pnIter; 
			int x= p[0]; int y= p[1]; int z=p[2]; 
			int idx=x+y*grid_res[0]+z*grid_res[0]*grid_res[1];
			VECTOR3	oldv,newv,dif; 
			oldv.Set(vectors[idx*3+0],vectors[idx*3+1],vectors[idx*3+2]);
			newv.Set(new_vectors[idx*3+0],new_vectors[idx*3+1],new_vectors[idx*3+2]);
			oldv.Normalize(); newv.Normalize();
			dif=oldv-newv;
			newval+=dif.GetMag();
		}
		if(score!=0)//do not select a picked streamline again
		score[line_no++]=newval;
	}
}

/*
void compute_streamlines_select_salient_streamlines() {

	float from[3], to[3]; 

	from[0] = minLen[0];   from[1] = minLen[1];   from[2] = minLen[2]; 
	to[0] = maxLen[0];   to[1] = maxLen[1];   to[2] = maxLen[2]; 

	int grid_res[3];
	//Note: the vector is normalized
	float* vectors=get_grid_vec_data(grid_res);//get vec data at each grid point

	printf("loading seeds...\n"); 
	int nSeeds; 
	//VECTOR3* seeds=loadSeedandRender(strcat(g_filename,".data"),nSeeds);
	VECTOR3* seeds=loadSeedfromFile("myseeds.seed",nSeeds);

	//nSeeds=25;
	osuflow->SetEntropySeedPoints( seeds,nSeeds);
	seeds = osuflow->GetSeeds(nSeeds); 


	for (int i=0; i<nSeeds; i++) 
		printf(" seed no. %d : [%f %f %f]\n", i, seeds[i][0], 
		seeds[i][1], seeds[i][2]); 

	sl_list.clear(); 
	std::vector<int> line_color;
	list<vtListSeedTrace*> half_lines, long_lines; 

	printf("compute streamlines..\n"); 
	osuflow->SetIntegrationParams(1, 5); //small and large step size
	osuflow->GenStreamLines(half_lines , BACKWARD_AND_FORWARD, 100, 0); //maxi steps
	printf(" done integrations\n"); 
	printf("list size = %d\n", half_lines.size()); 
	
	combinehalflines(half_lines,long_lines);
	
//	for(int i=0; i<nSeeds;i++)
//		line_color.push_back(i);
//	add_context_streamlines(line_color);
//	save_streamlines_to_file_hand_tuning(line_color,nSeeds);

	//start selection here
	float * score=new float[long_lines.size()];
	memset(score,-1.0,sizeof(float)*long_lines.size());

	//select the first, pick the longest streamline
	std::list<vtListSeedTrace*>::iterator myIter; 
	myIter=long_lines.begin();
	vtListSeedTrace *next;
	int picked=-1;

	int first_len=0;
	int p=0;
	for(;myIter!=long_lines.end();myIter++)
	{
		vtListSeedTrace *trace = *myIter; 
		if(trace->size()>first_len)
		{
			first_len=trace->size();
			next=trace;
			picked=p;
		}
		p++;
	}
//	sl_list.push_back(next);


	float* new_vectors=new float[grid_res[0]*grid_res[1]*grid_res[2]*3];
	memset(new_vectors,0,sizeof(float)*grid_res[0]*grid_res[1]*grid_res[2]*3);
	
	int* donot_change=new int[grid_res[0]*grid_res[1]*grid_res[2]];
	memset(donot_change,0,sizeof(int)*grid_res[0]*grid_res[1]*grid_res[2]);
	
	int* old_bin, *new_bin;
	old_bin=new int [grid_res[0]*grid_res[1]*grid_res[2]];
	new_bin=new int [grid_res[0]*grid_res[1]*grid_res[2]];

	for(int i=0;i<grid_res[0]*grid_res[1]*grid_res[2];i++)
	{
		VECTOR3 orif;
		orif.Set(vectors[i*3+0],vectors[i*3+1],vectors[i*3+2]);
		old_bin[i]=get_bin_number_3D(orif,theta, phi);
	}


	float entropy=9999;
	float target_entropy=0.09 ;//0.15->1.883016;//1.31778;//0.1->1.31778  0.05->0.71  0.2->2.419  0.01->0.165672 0.15->1.883016

	int line_threshold=5000;
	int selected_line_num=0;
	vector<VECTOR3> myseeds;
	
	while(selected_line_num<line_threshold)
	{
		//start the circle
		list<vtListSeedTrace*> new_lines; 
		new_lines.push_back(next);
		reconstruct_field_GVF_3D(new_vectors,vectors,grid_res,new_lines,donot_change);

		//printf("calculate the bin number\r");
		for(int i=0;i<grid_res[0]*grid_res[1]*grid_res[2];i++)
		{
			VECTOR3 newf,orif;
			orif.Set(vectors[i*3+0],vectors[i*3+1],vectors[i*3+2]);
			newf.Set(new_vectors[i*3+0],new_vectors[i*3+1],new_vectors[i*3+2]);
			//if within error range, let it be teh same bin as the ori, otherwise select the bin num
			newf.Normalize();
			float dotv=dot(newf,orif);
			if(acos(dotv)<(3.14*0.028))//less than 5 degree difference
				new_bin[i]=old_bin[i];
			else
				new_bin[i]=get_bin_number_3D(newf,theta, phi);
		}
		//about to end?
		 entropy=calcRelativeEntropy6(	vectors, new_vectors,  grid_res, VECTOR3(2,2,2),//do not count boundaries
									VECTOR3(grid_res[0]-2,grid_res[1]-2,grid_res[2]-2), theta, phi,old_bin,new_bin);
		
	
		 sl_list.push_back(next);
		 selected_line_num++;
		 printf("entropy=%f /%f the %dth streamline\n",entropy, target_entropy, selected_line_num);
		 if(entropy<target_entropy)
			 break;
		
		 //update the score for each streamline
		 score[picked]=0.0;//the streamline just selected, do not select it again
		 update_score(score,vectors,new_vectors,long_lines,grid_res, long_lines.size());
		 //select the highest score/error line as the next
		float highest=0;
		for(int i=0; i<long_lines.size();i++)
		{
			if(score[i]>highest)
			{
				highest=score[i];
				picked=i;
			}
		}
		//add the line with the highest score
		if(highest>0 && picked>=0 && picked<long_lines.size())
		{
			myIter=long_lines.begin();
			for(int i=0; i< picked; i++)
				myIter++;
			vtListSeedTrace *trace = *myIter; 
			next=trace;
			
			myseeds.push_back(seeds[picked]);
			
			dumpSeeds( myseeds, "myseeds.seed");

		}
	}
	
	delete [] score;
	delete [] vectors;
	delete [] donot_change;
	delete [] old_bin;
	delete [] new_bin;
}
*/
		list<vtListSeedTrace*> half_lines, long_lines; 

float dsim(std::vector<VECTOR3> mylist,std::vector<VECTOR3> yourlist, VECTOR3 p, VECTOR3 q,float a)
{
	VECTOR3 t=p-q;
	float pq=t.GetMag();

	float sum=0;
	for(int i=0;i<mylist.size();i++)
	{
		VECTOR3 m,y;
		m=mylist[i]; y=yourlist[i];
		t=m-y;
		float pkqk=t.GetMag();
		sum=sum+fabs(pkqk-pq);
	}
	float d=pq+a*sum/((float)mylist.size());
	return d;
}
void findNearestPoint(VECTOR3 p,vtListSeedTrace* trace,VECTOR3& q)
{
	float mindist=9999;
	std::list<VECTOR3*>::iterator myIter;
	myIter = trace->begin(); //the new streamline
	for (; myIter!= trace->end(); myIter++) //check point by point
	{
		VECTOR3 t = **myIter;
		VECTOR3 tt=t-p;
		float dist=tt.GetMag();
		if(dist<mindist)
		{
			mindist=dist;
			q.Set(t.x(),t.y(),t.z());
		}
	}
}
//if not enough, repeat points 
void get_sublist_at_point(VECTOR3 p,vtListSeedTrace* trace, int half_win_size, std::vector<VECTOR3>& outlist)
{
	std::list<VECTOR3*>::iterator myIter,centerIter,tmpIter;
	myIter = trace->begin(); //the new streamline
	for (; myIter!= trace->end(); myIter++) //check point by point
	{
		VECTOR3 t = **myIter;
		if(t.x()==p.x() && t.y()==p.y() && t.z()==p.z())
		{
			centerIter=myIter;
			break;
		}
	}
	int count=0;tmpIter=centerIter;
	VECTOR3 tt;
	tt.Set(p.x(),p.y(),p.z());
	for (; tmpIter!= trace->end(); tmpIter++) //check point by point
	{
		tt = **tmpIter;
		if(count<=half_win_size)
			outlist.push_back(tt);
		else 
			break;
		count++;
	}
	for(int i=count;i<=half_win_size;i++)
		outlist.push_back(tt);

	count=0;tmpIter=centerIter;
	tt.Set(p.x(),p.y(),p.z());
	for (; tmpIter!= trace->begin(); tmpIter--) //check point by point
	{
		tt = **tmpIter;
		if((count<=(half_win_size)) && (count>0))
			outlist.push_back(tt);
		else 
			break;
		count++;
	}
	for(int i=count;i<half_win_size;i++)
		outlist.push_back(tt);

}
void check_distance_and_add(list<vtListSeedTrace*> line, list<vtListSeedTrace*>& line_list,int* grid_res)
{
	VECTOR3 p,q,pk[5],qk[5];
	float d_sep=0.08*min(grid_res[0],min(grid_res[1],grid_res[2]));
	float alpha=2;
	float w=5*d_sep;
	int   win=(int)w;
	float d_selfsep=d_sep/10.0;
	float d_min=5*d_selfsep;
	float minLen=w*2;
	
	bool pass=true;
	list<vtListSeedTrace*>::iterator pIter;
	vtListSeedTrace *intrace = *(line.begin());
	vtListSeedTrace *newtrace = new vtListSeedTrace; 

	std::list<VECTOR3*>::iterator inTraceIter;
	inTraceIter = intrace->begin(); //the new streamline

	float min_dist=9999;
	for (; inTraceIter!= intrace->end(); inTraceIter++) //check point by point
	{
		VECTOR3 p = **inTraceIter;
		std::vector<VECTOR3> mypoints;
		get_sublist_at_point(p, intrace, win,mypoints);

		pIter = line_list.begin(); //distance with each existing
		for (; pIter!=line_list.end(); pIter++) 
		{
			vtListSeedTrace *trace = *pIter; 
			//find the nearest point
			//get a sublist surrounding the point
			std::list<VECTOR3*>::iterator pnIter;
			pnIter = trace->begin(); 
			int count=0;
			std::vector<VECTOR3> neighborpoints;
			for (; pnIter!= trace->end(); pnIter++) //check with all existing lines
			{
				VECTOR3 q; //find the start point, w/2+1
				findNearestPoint(p,trace,q);
				neighborpoints.clear();
				get_sublist_at_point(q, trace, win,neighborpoints);
				float d=dsim( mypoints,neighborpoints, p, q,alpha);
				//printf("dist %f/%f\n",d,d_sep);
				if(d<d_sep)//stop criteria
				{
					pass=false;
					break;
				}
				if(d<min_dist)
					min_dist=d;
			}
			if(	pass==false)//do not exam other lines
				break;
		}
		if(pass==true)
		newtrace->push_back(new VECTOR3(p.x(),p.y(),p.z()));

	}
		
	//printf("dist=%f/%f\n",min_dist,d_sep);
	if(newtrace->size()>minLen)
		line_list.push_back(newtrace);
	delete intrace;
}

void compute_streamlines_select_salient_streamlines() {

	
	int grid_res[3];
	//Note: the vector is normalized
	float* vectors=get_grid_vec_data(grid_res);//get vec data at each grid point

	int nSeeds=1000; 

	float from[3], to[3]; 
	from[0] =  from[1] = from[2] = 0; 
	to[0] = grid_res[0];   to[1] = grid_res[1];   to[2] = grid_res[2]; 

	printf("generating seeds...\n"); 
	osuflow->SetRandomSeedPoints(from, to, nSeeds); 
	
	VECTOR3* seeds = osuflow->GetSeeds(nSeeds); 
	VECTOR3* cpy_seeds=new VECTOR3[nSeeds];
	for (int i=0; i<nSeeds; i++) 
	{
		if(i==0)
			cpy_seeds[i].Set(grid_res[0],grid_res[1],grid_res[2]);
		else
			cpy_seeds[i].Set(seeds[i][0],	seeds[i][1], seeds[i][2]); 
	}

	//parameters

	VECTOR3* seed=new VECTOR3[1];
	for(int i=0;i<nSeeds;i++)
	{
		seed->Set(cpy_seeds[i][0],cpy_seeds[i][1],cpy_seeds[i][2]);
		osuflow->SetEntropySeedPoints( seed,1);

		printf(" seed no. %d : [%f %f %f]\n", i, cpy_seeds[i][0], 
		cpy_seeds[i][1], cpy_seeds[i][2]); 
		//sl_list.clear(); 
		std::vector<int> line_color;
		list<vtListSeedTrace*> half_lines, long_lines; 

		printf("compute streamlines..\n"); 
		osuflow->SetIntegrationParams(1, 1); //small and large step size
		osuflow->GenStreamLines(half_lines , BACKWARD_AND_FORWARD, 200, 0); //maxi steps
		printf(" done integrations\n"); 
		if( half_lines.size())
		{
			combinehalflines(half_lines,long_lines);
			check_distance_and_add(long_lines, sl_list,grid_res);
		}
		
	}
	printf("the number of streamlines=%d\n",sl_list.size());	
	std::vector<int> line_color;
	for(int i=0; i<nSeeds;i++)
		line_color.push_back(i);
	save_streamlines_to_file_hand_tuning("test.dat",line_color,nSeeds);

	delete seed;
	delete [] cpy_seeds;
	delete [] vectors;
}


void save_distance_volume()
{
	//sl_list;
}

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
//	dumpReconstruedField("r_diff.vec", new_vectors, grid_res);

	delete [] new_vectors;
	delete [] donot_change;

	for(int i=0; i<nSeeds;i++)
		line_color.push_back(i);
//	add_context_streamlines(line_color);
	//save_streamlines_to_file_hand_tuning(line_color,nSeeds);
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
void compute_streamlines_calc_entropy() {

	float from[3], to[3]; 

	from[0] = minLen[0];   from[1] = minLen[1];   from[2] = minLen[2]; 
	to[0] = maxLen[0];   to[1] = maxLen[1];   to[2] = maxLen[2]; 

	int grid_res[3];
	//Note: the vector is normalized
	float* vectors=get_grid_vec_data(grid_res);//get vec data at each grid point

	printf("loading seeds...\n"); 
	int nSeeds; 
	//VECTOR3* seeds=loadSeedandRender(strcat(g_filename,".data"),nSeeds);
	VECTOR3* seeds=loadSeedfromFile("myseeds.seed",nSeeds);


	float* new_vectors=new float[grid_res[0]*grid_res[1]*grid_res[2]*3];
	memset(new_vectors,0,sizeof(float)*grid_res[0]*grid_res[1]*grid_res[2]*3);
	
	int* donot_change=new int[grid_res[0]*grid_res[1]*grid_res[2]];
	memset(donot_change,0,sizeof(int)*grid_res[0]*grid_res[1]*grid_res[2]);
	
	float* importance=new float[grid_res[0]*grid_res[1]*grid_res[2]];
	memset(importance,0,sizeof(float)*grid_res[0]*grid_res[1]*grid_res[2]);

	int* old_bin, *new_bin;
	old_bin=new int [grid_res[0]*grid_res[1]*grid_res[2]];
	new_bin=new int [grid_res[0]*grid_res[1]*grid_res[2]];

	for(int i=0;i<grid_res[0]*grid_res[1]*grid_res[2];i++)
	{
		VECTOR3 orif;
		orif.Set(vectors[i*3+0],vectors[i*3+1],vectors[i*3+2]);
		old_bin[i]=get_bin_number_3D(orif,theta, phi,binnum);
	}


	float entropy=9999;
	int line_threshold=5000;
	int selected_line_num=0;
	vector<VECTOR3> myseeds;
	std::vector<float> entropies;
	while(selected_line_num<nSeeds)
	{
		//start the circle
		list<vtListSeedTrace*> new_lines; 
		osuflow->SetEntropySeedPoints( &(seeds[selected_line_num]),1);

		//sl_list.clear(); 
		std::vector<int> line_color;
		list<vtListSeedTrace*> half_lines, long_lines; 

		//printf("compute streamlines..\n"); 
		osuflow->SetIntegrationParams(1, 5); //small and large step size
		osuflow->GenStreamLines(half_lines , BACKWARD_AND_FORWARD, 100, 0); //maxi steps
		//printf(" done integrations\n"); 
		//printf("list size = %d\n", half_lines.size()); 
		
		combinehalflines(half_lines,long_lines);

//		reconstruct_field_GVF_3D(new_vectors,vectors,grid_res,long_lines,donot_change,importance);

		//printf("calculate the bin number\r");
		for(int i=0;i<grid_res[0]*grid_res[1]*grid_res[2];i++)
		{
			VECTOR3 newf,orif;
			orif.Set(vectors[i*3+0],vectors[i*3+1],vectors[i*3+2]);
			newf.Set(new_vectors[i*3+0],new_vectors[i*3+1],new_vectors[i*3+2]);
			//if within error range, let it be teh same bin as the ori, otherwise select the bin num
			newf.Normalize();
			float dotv=dot(newf,orif);
			if(acos(dotv)<(3.14*0.028))//less than 5 degree difference
				new_bin[i]=old_bin[i];
			else
				new_bin[i]=get_bin_number_3D(newf,theta, phi,binnum);
		}
		//about to end?
		 entropy=calcRelativeEntropy6(	vectors, new_vectors,  grid_res, VECTOR3(2,2,2),//do not count boundaries
									VECTOR3(grid_res[0]-2,grid_res[1]-2,grid_res[2]-2), theta, phi,old_bin,new_bin,NULL,NULL);
		
		 printf("line %d/%d entropy=%f\n",selected_line_num, nSeeds, entropy);
		 entropies.push_back(entropy);
	

		 selected_line_num++;
//		 printf("entropy=%f /%f the %dth streamline\n",entropy, target_entropy, selected_line_num);
//		 if(entropy<target_entropy)
//			 break;

		 dumpEntropies(entropies);
	}
//	delete [] score;
	delete [] importance;
	delete [] vectors;
	delete [] donot_change;
	delete [] old_bin;
	delete [] new_bin;
}

void trim_dull_segments(list<vtListSeedTrace*> lines, int* new_bin, int* old_bin, int* grid_res)
{
	vtListSeedTrace* newlist;
	list<vtListSeedTrace*>::iterator pIter;
	pIter =lines.begin(); 


	for (; pIter!=lines.end(); pIter++) 
	{
		vtListSeedTrace *trace = *pIter; 
		std::list<VECTOR3*>::iterator pnIter; 
		pnIter = trace->begin(); 

		int count=0;
		int count_idx=0;
		int old_idx=-1;
		for (; pnIter!= trace->end(); pnIter++) 
		{
			VECTOR3 p = **pnIter; 
			int x,y,z;
			x=p.x(); y=p.y(); z=p.z();
			int idx=x+y*grid_res[0]+z*grid_res[0]*grid_res[1];
	
			if(idx==old_idx)
				count_idx++;
			else
				count_idx=0;//reset
			if(count_idx>10)//step on the same point, trim the streamline short
			{
				for (; pnIter!= trace->end(); ) 
					trace->erase(pnIter++);
				break;

			}
			old_idx=idx;
		}

		count=0;
		pnIter = trace->begin(); 
		for (; pnIter!= trace->end(); pnIter++) 
		{
			VECTOR3 p = **pnIter; 
			int x,y,z;
			x=p.x(); y=p.y(); z=p.z();
			int idx=x+y*grid_res[0]+z*grid_res[0]*grid_res[1];
			if(new_bin[idx]==old_bin[idx])
				count++;
			else
				count=0;//reset
			if(count>10)//entered a stable area, erase the remaining
			{
				for (; pnIter!= trace->end(); ) 
					trace->erase(pnIter++);
				break;

			}
		}
	}
	
}

void dumphistogram(int* bin, int* grid_res, int binnum, int n)
{
	int* histo=new int[binnum];
	memset(histo,0,binnum*sizeof(int));

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


bool inside(int x, int y,int z,int xdim,int ydim,int zdim)
{
	if(x<0 || x>xdim-1 || y<0||y>ydim-1 || z<0||z>zdim-1)
		return false;
	else
		return true;
}
int sign(float x)
{
	if (x>0)
		return 1;
	else if(x<0)
		return -1;
	else
		return 0;
}
void ConstructGraph_static_3D(char* filename, std::vector<vtListVertex*>& Graph)
{
	int element_size=3;
	FILE* fp;
	int xdim,ydim,zdim;
	float* buf0,*buf;
	//construct graph
	fp=fopen(filename,"rb");
	fread(&xdim,sizeof(int),1,fp);
	fread(&ydim,sizeof(int),1,fp);
	fread(&zdim,sizeof(int),1,fp);
	float mag_cof=100;

	buf=new float[xdim*ydim*zdim*element_size];
	int num;
	num=fread(buf,sizeof(float),xdim*ydim*zdim*element_size,fp);
	fclose(fp);
	for(int z=0;z<zdim; z++)
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

	for(int z=0;z<zdim; z++)
	//int z=0;
	{
		for(int y=0;y<ydim; y++)
		{
			for(int x=0; x<xdim; x++)
			{
				int idx=x+y*xdim+z*xdim*ydim;

				float u=buf[idx*element_size+0]*mag_cof;
				float v=buf[idx*element_size+1]*mag_cof;
				float w=buf[idx*element_size+2]*mag_cof;

				int idx2;
				int left, right, top,bottom, front,back;
				float uu,vv,ww;
				
				//left face
				if(inside(x-1,y,z,xdim,ydim,zdim))
				{
					left=x-1+y*xdim+z*xdim*ydim;
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
				if(inside(x+1,y,z,xdim,ydim,zdim))
				{
					right=x+1+y*xdim+z*xdim*ydim;
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
				if(inside(x,y-1,z,xdim,ydim,zdim))
				{
					top=x+(y-1)*xdim+z*xdim*ydim;
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
				if(inside(x,y+1,z,xdim,ydim,zdim))
				{
					bottom=x+(y+1)*xdim+z*xdim*ydim;
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
				//front face
				if(inside(x,y,z-1,xdim,ydim,zdim))
				{
					front=x+y*xdim+(z-1)*xdim*ydim;
					ww=buf[front*element_size+2]*mag_cof;

					int weight=0;
					if(sign(w)<0)
						weight=abs((int)w);
					if(sign(ww)>0)
						weight+=abs((int)ww);
					//if(weight!=0)
					{
						Graph[idx]->push_back(front);
						Graph[idx]->push_back(weight);
					}
					
				}
				//back face
				if(inside(x,y,z+1,xdim,ydim,zdim))
				{
					back=x+y*xdim+(z+1)*xdim*ydim;
					ww=buf[back*element_size+2]*mag_cof;

					int weight=0;
					if(sign(w)>0)
						weight=abs((int)w);
					if(sign(ww)<0)
						weight+=abs((int)ww);
					//if(weight!=0)
					{
						Graph[idx]->push_back(back);
						Graph[idx]->push_back(weight);
					}
					
				}

			}
		}
	}

	delete [] buf;

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
void compute_streamlines_by_click_initialize() 
{
	
	vectors=get_grid_vec_data(grid_res);//get vec data at each grid point
	newseed.Set(grid_res[0]/2, grid_res[1]/2,grid_res[2]/2);
	donot_change=new int[grid_res[0]*grid_res[1]*grid_res[2]];
	memset(donot_change,0,sizeof(int)*grid_res[0]*grid_res[1]*grid_res[2]);
	new_vectors=new float[grid_res[0]*grid_res[1]*grid_res[2]*3];
	memset(new_vectors,0,sizeof(float)*grid_res[0]*grid_res[1]*grid_res[2]*3);

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
	old_bin=new int [grid_res[0]*grid_res[1]*grid_res[2]];
	new_bin=new int [grid_res[0]*grid_res[1]*grid_res[2]];
	printf("calculate the bin number\n");
	for(int i=0;i<grid_res[0]*grid_res[1]*grid_res[2];i++)
	{
		VECTOR3 orif;
		orif.Set(vectors[i*3+0],vectors[i*3+1],vectors[i*3+2]);
		old_bin[i]=get_bin_number_3D(orif,theta, phi,binnum);
	}

	kx=new float[grid_res[0]*grid_res[1]*grid_res[2]];
	ky=new float[grid_res[0]*grid_res[1]*grid_res[2]];
	kz=new float[grid_res[0]*grid_res[1]*grid_res[2]];
	b=new float[grid_res[0]*grid_res[1]*grid_res[2]];
	c1=new float[grid_res[0]*grid_res[1]*grid_res[2]];
	c2=new float[grid_res[0]*grid_res[1]*grid_res[2]];
	c3=new float[grid_res[0]*grid_res[1]*grid_res[2]];

	ConstructGraph_static_3D(g_filename, Graph);
	Tree = new OCTree(grid_res[0], grid_res[1],grid_res[2]);

	//build quadtree
	printf("build octree tree\n");
	int finestlevel=8+1;//may play some role in the result, diffrerent data set, dif setting may help?
	Tree->Build(finestlevel);
	g_new_vectors=new_vectors;

	occupied=new int[grid_res[0]*grid_res[1]*grid_res[2]];
	memset(occupied,0,sizeof(int)*grid_res[0]*grid_res[1]*grid_res[2]);
	
	entropy_every_point=new float [grid_res[0]*grid_res[1]*grid_res[2]];

}

void compute_streamlines_by_click_cleanup() 
{
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
}
bool initialized=false;
void compute_streamlines_by_click() 
{
	

	printf("generating seeds...\n"); 
	int num=0;

	if(initialized==false)
	{
		compute_streamlines_by_click_initialize();
		initialized=true;
	}

	//load seeds from file
	int nSeeds; 
	//set first seed as domain center
	vector<VECTOR3> seeds;
	seeds.push_back(newseed);
//	sl_list.clear(); 
	//combine the half lines
	int next_id=0;

	
	double dwStart;
	double elapsedTime;
	list<vtListSeedTrace*> lines,new_lines; 


	int selected_line_num=0;
	// DEL-BY-LEETEN 01/22/2010 srand((unsigned)time(NULL));			// initialize random number generator
	
	float entropy=9999;
	std::vector<int> line_color;

	//int line_num_thres=30; //source
	float dt_entropy,ori_entropy,old_entropy;
	bool first=true;
	dt_entropy=ori_entropy=entropy;
	
	//dumphistogram(old_bin,grid_res, 360,0);
	//used to check entropy
	VECTOR3 startpt, endpt;
	//	startpt.Set(0,0,0); endpt.Set(grid_res[0]-1,grid_res[1]-1,grid_res[2]-1);
	//float data_entropy=calcEntropy1( vectors, grid_res, startpt,endpt,theta, phi);
	//printf("data entropy=%f\n",data_entropy);
		
	std::vector<float> entropies;
	float error=0.25;
	float target_entropy=0.1;//-error*log2(error)-(1-error)*log2(1-error)+error*log2(359);
	printf("entropy_target=%f\n",target_entropy);


	//while(entropy>target_entropy)

	int line_num_thres=1; 
	
	while(line_num_thres>selected_line_num)
	{

		memset(kx,0,sizeof(float)*grid_res[0]*grid_res[1]*grid_res[2]);
		memset(ky,0,sizeof(float)*grid_res[0]*grid_res[1]*grid_res[2]);
		memset(kz,0,sizeof(float)*grid_res[0]*grid_res[1]*grid_res[2]);
		memset(b,0,sizeof(float)*grid_res[0]*grid_res[1]*grid_res[2]);
		memset(c1,0,sizeof(float)*grid_res[0]*grid_res[1]*grid_res[2]);
		memset(c2,0,sizeof(float)*grid_res[0]*grid_res[1]*grid_res[2]);
		memset(c3,0,sizeof(float)*grid_res[0]*grid_res[1]*grid_res[2]);

	//	printf("\n\n no %d seed selected = %f %f %f\n", selected_line_num, newseed->x(), newseed->y(),newseed->z());

		list<vtListSeedTrace*> lines,new_lines; 
		//printf("compute streamlines..\n"); 
		osuflow->SetEntropySeedPoints( &newseed,1);
		osuflow->SetIntegrationParams(1, 5);							//small and large step size
		osuflow->GenStreamLines(lines , BACKWARD_AND_FORWARD, 500, 0);	 //maxi steps
	
		UpdateOccupied(lines, occupied,grid_res,0);
		combinehalflines(lines, new_lines);

		sl_list.push_back(*(new_lines.begin()));

//dwStart = GetTickCount();
		reconstruct_field_GVF_3D(new_vectors,vectors,grid_res,new_lines,donot_change,
						 kx,ky,kz,b,c1,c2,c3);//,importance);
//elapsedTime= GetTickCount() - dwStart;
//printf("\n reconstruction time is %.3f milli-seconds.\n",elapsedTime); 	
	

		//printf("calculate the bin number\r");
		for(int i=0;i<grid_res[0]*grid_res[1]*grid_res[2];i++)
		{
			VECTOR3 newf,orif;
			orif.Set(vectors[i*3+0],vectors[i*3+1],vectors[i*3+2]);
			newf.Set(new_vectors[i*3+0],new_vectors[i*3+1],new_vectors[i*3+2]);
			//if within error range, let it be teh same bin as the ori, otherwise select the bin num
			newf.Normalize();
			float dotv=dot(newf,orif);
	//		if(acos(dotv)<(3.14*0.0255))//less than 3 degree difference
	//			new_bin[i]=old_bin[i];
	//		else
				new_bin[i]=get_bin_number_3D(newf,theta, phi,binnum);
		}
	
		entropy=calcRelativeEntropy6(	vectors, new_vectors,  grid_res, VECTOR3(2,2,2),//do not count boundaries
									VECTOR3(grid_res[0]-2,grid_res[1]-2,grid_res[2]-2), theta, phi,old_bin,new_bin,occupied,NULL);
//		float point_dist=calcPoint2PointError(	vectors, new_vectors,  grid_res, VECTOR3(2,2,2),//do not count boundaries
//									VECTOR3(grid_res[0]-2,grid_res[1]-2,grid_res[2]-2));
		entropies.push_back(entropy);
		printf("seed %d selected, entropy=%f/%f \n",selected_line_num, entropy,target_entropy);

		//VECTOR3 next_seed=selectNextSeed(vectors,new_vectors, grid_res, Tree,theta,phi,old_bin,new_bin);
		VECTOR3 next_seed=selectNextSeed3(vectors,new_vectors, grid_res, Tree,theta,phi,old_bin,new_bin,Graph,
			occupied,NULL,0);
		if(next_seed.x()<0 || ((next_seed.x()==newseed.x()) && (next_seed.y()==newseed.y()) &&(next_seed.z()==newseed.z())))
		{
			break;
		}
		seed_list.push_back(next_seed);
		newseed.Set(next_seed.x(),next_seed.y(),next_seed.z());
		seeds.push_back(newseed);

		int seed_id=(int)next_seed.x()+((int)next_seed.y())*grid_res[0]
					+((int)next_seed.z())*grid_res[0]*grid_res[1];
		new_vectors[seed_id*3+0]=vectors[seed_id*3+0];
		new_vectors[seed_id*3+1]=vectors[seed_id*3+1];
		new_vectors[seed_id*3+2]=vectors[seed_id*3+2];
		donot_change[seed_id]=1;

		line_color.push_back(selected_line_num);
		selected_line_num++;
		VECTOR3 st,ed;
		//dump_entrpy_every_point(Tree,entropy_every_point,grid_res,
		//					 vectors, new_vectors, theta, phi, old_bin, new_bin,occupied,st,ed);

		dumpSeeds( seeds, "myseeds.seed");
		dumpEntropies(entropies);
		//dumphistogram(new_bin, grid_res, 360, selected_line_num);

	//save_streamlines_to_file_hand_tuning(line_color,selected_line_num);
	
	}

	//dumpReconstruedField("r.vec", new_vectors, grid_res);
	
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

// ADD-BY-LEETEN 02/02/2010-BEGIN
int* bin_my_vector,*bin_new_vector;
int *histo_puv;
int *histo_pv;
float* pv;
float* entropy_tmp;
int* selX, *selY,*selZ;
float* qx;
float* qy;
float* qz;

void 
quit_selectStreamlines_by_distribution()
{
	FREE(bin_my_vector);
	FREE(bin_new_vector);
	FREE(histo_puv);
	FREE(histo_pv);
	FREE(pv);
	FREE(entropy_tmp);
	FREE(selX);
	FREE(selY);
	FREE(selZ);
	FREE(qx);
	FREE(qy);
	FREE(qz);
}
// ADD-BY-LEETEN 02/02/2010-END

// ADD-BY-LEETEN 03/18/2010-BEGIN
int IWrapCoord3D(int tx, int ty, int tz, int grid_res[3])
{
	int idx;
	#if		WRAP_MODE==WRAP_MODE_CLAMP

	if( tx < 0 || tx >= grid_res[0] ||
		ty < 0 || ty >= grid_res[1] || 
		tz < 0 || tz >= grid_res[2] || 
		)
	{
		return -1;
	}

	#elif	WRAP_MODE==WRAP_MODE_MIRROR

	tx = (tx<0)?(-tx):( (tx<grid_res[0])?tx:(grid_res[0]-1 - (tx-(grid_res[0]-1))) );
	ty = (ty<0)?(-ty):( (ty<grid_res[1])?ty:(grid_res[1]-1 - (ty-(grid_res[1]-1))) );
	tz = (tz<0)?(-tz):( (tz<grid_res[2])?tz:(grid_res[2]-1 - (tz-(grid_res[2]-1))) );

	#elif	WRAP_MODE==WRAP_MODE_REPEAT

	tx = tx % grid_res[0];
	ty = ty % grid_res[1];
	tz = tz % grid_res[2];

	#elif	WRAP_MODE==WRAP_MODE_CLAMP_TO_BORDER

	tx = min(max(tx, 0), grid_res[0] - 1);
	ty = min(max(ty, 0), grid_res[1] - 1);
	tz = min(max(tz, 0), grid_res[2] - 1);

	#endif	// #if	WRAP_MODE
	idx = tx + ty*grid_res[0] + tz*grid_res[0]*grid_res[1];
	return idx;
}
// ADD-BY-LEETEN 03/18/2010-END

//speed up by reusing histograms
void selectStreamlines_by_distribution(float* vectors,float* new_vectors, int* grid_res, 
									   int* occupied,
									   std::vector<VECTOR3>& seeds,
									   float* theta, float* phi,
										int* bin_vector, int* bin_newvectors,
										int* g_histo,int* g_histo_puv,int round,
											std::vector<float>& line_importance, float in_entropy)
{
	#if	0	// DEL-BY-LEETEN 02/04/2010-BEGIN
		// ADD-BY-LEETEN 02/02/2010-BEGIN
		static bool bIsInitialized = false;
		if( false == bIsInitialized  )
		{
			atexit(quit_selectStreamlines_by_distribution);
			bIsInitialized  = true;
		}
		// ADD-BY-LEETEN 02/02/2010-END
	#endif	// DEL-BY-LEETEN 02/04/2010-END

	// MOD-BY-LEETEN 02/04/2010-FROM:
		// int sample_number_allowed=50;
	// TO:
	int sample_number_allowed = NR_OF_SAMPLES;
	// DEL-BY-LEETEN 02/04/2010-END
	int  kernel_size[3];
	#if	0	// MOD-BY-LEETEN 02/02/2010-FROM:
		kernel_size[0]=kernel_size[1]= 8;//this is radium, actuall kernel is 2 x kernel_size+1
		kernel_size[2]=8;
	#else	// MOD-BY-LEETEN 02/02/2010-TO:
	kernel_size[0]=KERNEL_HALF_WIDTH;
	kernel_size[1]=KERNEL_HALF_HEIGHT;
	kernel_size[2]=KERNEL_HALF_DEPTH;//this is radium, actuall kernel is 2 x kernel_size+1
	#endif	// MOD-BY-LEETEN 02/02/2010-END
	float* img_entropies=new float[grid_res[0]*grid_res[1]*grid_res[2]];
	float sum=0;
	float maximum=0;

	#if	0	// MOD-BY-LEETEN 02/02/2010-FROM:
		int* bin_my_vector,*bin_new_vector;
		bin_my_vector=new int[grid_res[0]*grid_res[1]*grid_res[2]];
		bin_new_vector=new int[grid_res[0]*grid_res[1]*grid_res[2]];
	#else	// MOD-BY-LEETEN 02/02/2010-TO:
	MALLOC(bin_my_vector, int, grid_res[0]*grid_res[1]*grid_res[2]);
	MALLOC(bin_new_vector, int, grid_res[0]*grid_res[1]*grid_res[2]);
	#endif	// MOD-BY-LEETEN 02/02/2010-END
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

	#if	0	// MOD-BY-LEETEN 02/02/2010-FROM:
		int *histo_puv=new int[binnum*binnum];
		int *histo_pv=new int[binnum];
		//float* puv=new float[binnum*binnum];
		float* pv=new float[binnum];
		float* entropy_tmp=new float[binnum];
	#else	// MOD-BY-LEETEN 02/02/2010-TO:
	MALLOC(histo_puv,	int,	binnum*binnum);
	MALLOC(histo_pv,	int,	binnum);
	MALLOC(pv,			float,	binnum);
	MALLOC(entropy_tmp,	float,	binnum);
	#endif	// MOD-BY-LEETEN 02/02/2010-END

	int z=0;
	double dwStart = GetTickCount();
	#if		!ENTROPY_ON_CUDA		// ADD-BY-LEETEN 02/02/2010
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
				#if	0	// MOD-BY-LEETEN 03/18/2010-FROM:
					startpt.Set(max(0,x-kernel_size[0]),max(0,y-kernel_size[1]),max(0,z-kernel_size[2]));
					endpt.Set(min(x+kernel_size[0],grid_res[0]-1),min(y+kernel_size[1],grid_res[1]-1),
							min(z+kernel_size[2],grid_res[2]-1));
				#else	// MOD-BY-LEETEN 03/18/2010-TO:
				startpt.Set(x-kernel_size[0], y-kernel_size[1], z-kernel_size[2]);
				endpt.Set(	x+kernel_size[0], y+kernel_size[1],	z+kernel_size[2]);
				#endif	// MOD-BY-LEETEN 03/18/2010-END
				
				if(z==0)//calculate the first histogram
				{
					for(int tz=(int)startpt.z(); tz<=(int)endpt.z();tz++)
					{
						for(int ty=(int)startpt.y(); ty<=(int)endpt.y();ty++)
						{
							for(int tx=(int)startpt.x(); tx<=(int)endpt.x();tx++)
							{
								// MOD-BY-LEETEN 03/18/2010-FROM:
									// int idx=tx+ty*grid_res[0]+tz*grid_res[0]*grid_res[1];
								// TO:
								int idx = IWrapCoord3D(tx, ty, tz, grid_res);
								if( idx < 0 )
									continue;
								// MOD-BY-LEETEN 03/18/2010-END

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
#if	0	
							if(z-kernel_size[2]>=0)
							{
								int tz=z-kernel_size[2];
								int idx=tx+ty*grid_res[0]+tz*grid_res[0]*grid_res[1];
#else
						{
							int tz = z - 1 - kernel_size[2];
							int idx = IWrapCoord3D(tx, ty, tz, grid_res);
							if( idx < 0 )
								continue;
#endif
								int bin_no_ori=bin_vector[idx];
								int bin_no_new=bin_newvectors[idx];

								if(bin_no_ori<0 || bin_no_ori>=binnum||
									bin_no_new<0 || bin_no_new>=binnum)
								{
									printf("sth wrong, bin id=%d  %d\n", bin_no_ori,bin_no_new);
								}
								idx=bin_no_ori+binnum*bin_no_new;
								#if	0	// DEL-BY-LEETEN 03/18/2010-BEGIN
									if(histo_puv[idx]<=0 ||histo_pv[bin_no_new]<=0 )
										printf("something wrong x=%d y=%d z=%d\n",tx,ty,tz);
								#endif	// DEL-BY-LEETEN 03/18/2010-END
								histo_puv[idx]--;//p(x,y)
								histo_pv[bin_no_new]--;							
							}
							#if	0	// MOD-BY-LEETEN 03/18/2010-FROM:
								if(z+kernel_size[2]<grid_res[2])
								{
									int tz=z+kernel_size[2];
									int idx=tx+ty*grid_res[0]+tz*grid_res[0]*grid_res[1];
							#else	// MOD-BY-LEETEN 03/18/2010-TO:
							{
								int tz = z + kernel_size[2];
								int idx = IWrapCoord3D(tx, ty, tz, grid_res);
								if( idx < 0 )
									continue;
							#endif	// MOD-BY-LEETEN 03/18/2010-END
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
				// DEL-BY-LEETEN 03/18/2010-BEGIN
					// entropy=entropy*entropy;//*entropy*entropy*entropy;//enhance
				// DEL-BY-LEETEN 03/18/2010-END

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
	// ADD-BY-LEETEN 02/02/2010-BEGIN
	#else	// #if	!ENTROPY_ON_CUDA	
	LOG(printf("_ComputeJointEntropyVolume(): Enter"));
	void
	_ComputeJointEntropyVolume
	(
		int iNrOfSrcBins,
		int iNrOfDstBins,
		int iKernelWidth, int iKernelHeight, int iKernelDepth,
		float *pfEntropyVolume_host
	);

	_ComputeJointEntropyVolume
	(
		binnum, 
		binnum, 
		kernel_size[0], 
		kernel_size[1], 
		kernel_size[2], 
		img_entropies
	);
	LOG(printf("_ComputeJointEntropyVolume(): End"));

	//normalized it to (0~1)
	sum = 0.0f;
	for(int i=0;i<grid_res[0]*grid_res[1]*grid_res[2];i++)
	{
		sum += img_entropies[i];
	}
	#endif	// #if	!ENTROPY_ON_CUDA	
	// ADD-BY-LEETEN 02/02/2010-END

	
	double elapsedTime= GetTickCount() - dwStart;
	printf("\n\n time for entorpy calculation is %.3f milli-seconds.\n",elapsedTime); 	
//	printf("halted\n");
//	getchar();

	// ADD-BY-LEETEN 03/18/2010-BEGIN
	maximum = log2(float(binnum));
	// ADD-BY-LEETEN 03/18/2010-END

	//save result to file
	#if	0	// DEL-BY-LEETEN 02/02/2010-BEGIN
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
	#endif	// DEL-BY-LEETEN 02/02/2010-END

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
//	// DEL-BY-LEETEN 01/22/2010 srand((unsigned)time(NULL));
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
	// DEL-BY-LEETEN 01/22/2010 srand((unsigned)time(NULL));
	#if	0	// MOD-BY-LEETEN 02/02/2010-FROM:
		int* selX, *selY,*selZ;
		selX=new int[sample_number_allowed];
		selY=new int[sample_number_allowed];
		selZ=new int[sample_number_allowed];
		float* qz=new float[grid_res[2]];
	#else	// MOD-BY-LEETEN 02/02/2010-TO:
	MALLOC(selX,	int,	sample_number_allowed);
	MALLOC(selY,	int,	sample_number_allowed);
	MALLOC(selZ,	int,	sample_number_allowed);
	MALLOC(qz,		float,	grid_res[2]);
	#endif	// MOD-BY-LEETEN 02/02/2010-END
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
	// MOD-BY-LEETEN 02/02/2010-FROM:
		// float *qy=new float[grid_res[1]*grid_res[2]];
	// TO:
	MALLOC(qy, float, grid_res[1]*grid_res[2]);
	// MOD-BY-LEETEN 02/02/2010-END
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
	// MOD-BY-LEETEN 02/02/2010-FROM:
		// float *qx=new float[grid_res[1]*grid_res[2]*grid_res[0]];
	// TO:
	MALLOC(qx, float, grid_res[0]*grid_res[1]*grid_res[2]);
	// MOD-BY-LEETEN 02/02/2010-END
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

	// ADD-BY-LEETEN 02/06/2010-BEGIN
	FREE(qx);
	FREE(qy);
	FREE(qz);
	// ADD-BY-LEETEN 02/06/2010-END

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
		// MOD-BY-LEETEN 02/06/2010-FROM:
			// combinehalflines_check_stop_entropy(lines, new_lines,grid_res,entropies);//for display only, the streamline stops when gets too near to an existing
		// TO: 
#if PRUNING_MODE==PRUNING_MODE_COND_ENTROPY
		combinehalflines(lines, new_lines);	//for display only, the streamline stops when gets too near to an existing
#else
		void _CombineHalfLinesNotOccupied(list<vtListSeedTrace*> lines, list<vtListSeedTrace*>& long_lines, int *occupied, int grid_res[3]);
		_CombineHalfLinesNotOccupied(lines, new_lines, occupied, grid_res);	//for display only, the streamline stops when gets too near to an existing
#endif
		// MOD-BY-LEETEN 02/06/2010-END
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
			// MOD-BY-LEETEN 02/04/2010-FROM:
				// float epsilon=0.01;
			// TO:
			float epsilon = PRUNING_THRESHOLD;
			// MOD-BY-LEETEN 02/04/2010-END
			tmp_whole_set_lines=sl_list;
			tmp_whole_set_lines.push_back(*new_lines.begin());
			
			// ADD-BY-LEETEN 03/18/2010-BEGIN
			#if	PRUNING_MODE == PRUNING_MODE_COND_ENTROPY	
			// ADD-BY-LEETEN 03/18/2010-END
			bool discard=discardredundantstreamlines(cur_entropy,epsilon,tmp_whole_set_lines, vectors,new_vectors,grid_res,
				bin_my_vector,bin_new_vector,binnum, theta,  phi, histo_puv, histo_pv, pv, entropy_tmp);

			// ADD-BY-LEETEN 03/18/2010-BEGIN
			#else	//	#if	PRUNING_MODE == PRUNING_MODE_COND_ENTROPY	
			bool discard=false;
			#endif	//	#if	PRUNING_MODE == PRUNING_MODE_COND_ENTROPY	
			// ADD-BY-LEETEN 03/18/2010-END

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

	#if	0	// DEL-BY-LEETEN 02/02/2010-BEGIN
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
	#endif	// DEL-BY-LEETEN 02/02/2010-END

	// ADD-BY-LEETEN 02/04/2010-BEGIN
	quit_selectStreamlines_by_distribution();
	// ADD-BY-LEETEN 02/04/2010-END

	return;
}

/*
//original
void selectStreamlines_by_distribution(float* vectors,float* new_vectors, int* grid_res, 
									   int* occupied,
									   std::vector<VECTOR3>& seeds,
									   float* theta, float* phi,
										int* oldbin, int* newbin,
										int* g_histo,int* g_histo_puv,int round,
											std::vector<float>& line_importance)

{

	int sample_number_allowed=1000;
	int  kernel_size= 8;//attention, the kernel size  can not be larger than 32, because the histogram is calcualted using short int
						//to save time calculating 

	float* img_entropies=new float[grid_res[0]*grid_res[1]*grid_res[2]];
	
	float maximum=0;
	int radius=5;
	
	int binnum=360;
	printf("calculate entropy...\n"); 


	float sum=0;

	int *histo_puv=new int[binnum*binnum];
	int *histo_pv=new int[binnum];
	//float* puv=new float[binnum*binnum];
	float* pv=new float[binnum];
	float* entropy_tmp=new float[binnum];

	double dwStart = GetTickCount();
	for(int z=0;z<grid_res[2];z++)
	{
		for(int y=0;y<grid_res[1];y++)
		{
			for(int x=0;x<grid_res[0];x++)
			{
				VECTOR3 start,end;
				start.Set(max(0,x-kernel_size),max(0,y-kernel_size),max(0,z-kernel_size));
				end.Set(min(x+kernel_size,grid_res[0]-1),min(y+kernel_size,grid_res[1]-1),
						min(z+kernel_size,grid_res[2]-1));

				float entropy=calcRelativeEntropy6_by_known_histograms( vectors,new_vectors, grid_res, 
							start,
							end,
							theta, phi,
							oldbin, newbin,occupied,binnum,
							histo_puv, histo_pv,  pv, entropy_tmp,
							g_histo,g_histo_puv);

				entropy=entropy*entropy*entropy*entropy*entropy*entropy;
				int idx=x+y*grid_res[0]+z*grid_res[0]*grid_res[1];
				img_entropies[idx]=entropy;
				if(entropy>maximum)
					maximum=entropy;
				sum=sum+entropy;
				

			}
			//printf("y=%d/%d\n",y,grid_res[0]-1);
		}
		printf("z=%d/%d\n",z,grid_res[2]-1);

	}

	double elapsedTime= GetTickCount() - dwStart;
	printf("\n\n entorpy for each point time is %.3f milli-seconds.\n",elapsedTime); 	

	delete [] pv;
	delete [] histo_puv;
	delete [] histo_pv;
	delete [] entropy_tmp;


	
	//save result to file
	unsigned char* data;
	float* importance=new float[grid_res[0]*grid_res[1]*grid_res[2]];
	data = new unsigned char[grid_res[0]*grid_res[1]*grid_res[2]];
	for(int i=0;i<grid_res[0]*grid_res[1]*grid_res[2];i++)
	{	
		data[i]=(unsigned char)((img_entropies[i]/maximum)*255);
		importance[i]=img_entropies[i]/maximum;
	}
	dumpImportanceField("importance.bin",importance, grid_res);

	//normalized it to (0~1)
	for(int i=0;i<grid_res[0]*grid_res[1]*grid_res[2];i++)
		img_entropies[i]=img_entropies[i]/sum;

	//start the importance sampling, acceptance-rejection method
	int* selX, *selY,*selZ;
	selX=new int[sample_number_allowed];
	selY=new int[sample_number_allowed];
	selZ=new int[sample_number_allowed];
//	float M=(maximum/sum*grid_res[0]*grid_res[1]*grid_res[2])*1.1;
	float M=(maximum/sum)*1.1;

	int i=0;
	// DEL-BY-LEETEN 01/22/2010 srand((unsigned)time(NULL));
	while(i<sample_number_allowed)
	{
		float randx = ((float)rand()/(float)RAND_MAX);
		float randy = ((float)rand()/(float)RAND_MAX);
		float randz = ((float)rand()/(float)RAND_MAX);
		float randu = ((float)rand()/(float)RAND_MAX);

		int x=(int)(floor(randx*(grid_res[0]-1)));
		int y=(int)(floor(randy*(grid_res[1]-1)));
		int z=(int)(floor(randz*(grid_res[2]-1)));
		float u=randu;
		if(u<(img_entropies[x+y*grid_res[0]+z*grid_res[0]*grid_res[1]] /M))
		{
			selX[i]=x;
			selY[i]=y;
			selZ[i]=z;
			i++;

			//data[x+y*grid_res[0]]=125;
		}
	}

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
		osuflow->SetIntegrationParams(1, 5);							//small and large step size
		osuflow->GenStreamLines(lines , BACKWARD_AND_FORWARD, 500, 0);	 //maxi steps
		combinehalflines(lines, new_lines);//for display only, the streamline stops when gets too near to an existing
		if(new_lines.size())
		sl_list.push_back(*(new_lines.begin()));			
		seeds.push_back(VECTOR3(selX[i],selY[i],selZ[i]));
		UpdateOccupied(new_lines,occupied,grid_res,radius);

	}
	std::vector<int> line_color;
	for(int i=0;i<sl_list.size();i++)
		line_color.push_back(i);
//	save_streamlines_to_file_alpha_by_importance(line_color,sl_list.size(), importance, grid_res);

	delete [] selX;
	delete [] selY;
	delete [] selZ;
	//save2PPM_3("cand_seeds_image.ppm", data, grid_res[0],grid_res[1]);
	delete [] data;
	delete [] img_entropies;
	delete [] importance;
	return;
}
*/

//allow select multiple seed in one pass
void compute_streamlines() 
{
	// ADD-BY-LEETEN 03/18/2010-BEGIN
	double dwComputeStreamlinesBegin = GetTickCount();
	// ADD-BY-LEETEN 03/18/2010-END

	printf("generating seeds...\n"); 
	int num=0;
	int grid_res[3];
	float* vectors=get_grid_vec_data(grid_res);//get vec data at each grid point
	
										//load seeds from file
	#if	0	// MOD-BY-LEETEN 02/02/2010-FROM:
		int* donot_change=new int[grid_res[0]*grid_res[1]*grid_res[2]];
		memset(donot_change,0,sizeof(int)*grid_res[0]*grid_res[1]*grid_res[2]);
	#else	// MOD-BY-LEETEN 02/02/2010-TO:
	int* donot_change=NULL;
	#endif	// MOD-BY-LEETEN 02/02/2010-END
	float* new_vectors=new float[grid_res[0]*grid_res[1]*grid_res[2]*3];
	memset(new_vectors,0,sizeof(float)*grid_res[0]*grid_res[1]*grid_res[2]*3);

	int nSeeds; 
	//set first seed as domain center
	sl_list.clear(); 
	seed_list.clear();
	
	//set boundary condition
	#if	0	// DEL-BY-LEETEN 02/10/2010-BEGIN
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
				// DEL-BY-LEETEN 02/02/2010		donot_change[idx]=1;
			}
		}
	#endif	// DEL-BY-LEETEN 02/10/2010-END

	int* old_bin, *new_bin;
	old_bin=new int [grid_res[0]*grid_res[1]*grid_res[2]];
	new_bin=new int [grid_res[0]*grid_res[1]*grid_res[2]];

	// ADD-BY-LEETEN 02/02/2010-BEGIN
	#if	USE_CUDA
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
	#endif	//	#if	USE_CUDA
	// ADD-BY-LEETEN 02/02/2010-END

	// ADD-BY-LEETEN 02/04/2010-BEGIN
	#if		DIFFUSION_ON_GLSL
	_FlowDiffusionGLGLInit(grid_res[0], grid_res[1], grid_res[2]);
	#endif	// #if	DIFFUSION_ON_GLSL
	// ADD-BY-LEETEN 02/04/2010-END

	#if	!BIN_LOOKUP_ON_CUDA		// ADD-BY-LEETEN 02/02/2010
	for(int i=0;i<grid_res[0]*grid_res[1]*grid_res[2];i++)
	{
		VECTOR3 orif;
		orif.Set(vectors[i*3+0],vectors[i*3+1],vectors[i*3+2]);
		old_bin[i]=get_bin_number_3D(orif,theta, phi,binnum);
		new_bin[i]=0.0;
	}
	// ADD-BY-LEETEN 02/02/2010-BEGIN
	#else	// #if	!BIN_LOOKUP_ON_CUDA		
	memset(old_bin, 0, sizeof(old_bin[0]) * grid_res[0] * grid_res[1] * grid_res[2]);
	memset(new_bin, 0, sizeof(new_bin[0]) * grid_res[0] * grid_res[1] * grid_res[2]);

	_ComputeSrcBinVolume
	(
		grid_res[0],
		grid_res[1],
		grid_res[2],
		1,
		vectors
	);
	_GetSrcBinVolume(old_bin);
	#endif	// #if	!BIN_LOOKUP_ON_CUDA	
	// ADD-BY-LEETEN 02/02/2010-END

	if(!entropies)
	{
		printf("calculating every point entropies\n");
		entropies=new float[grid_res[0]*grid_res[1]*grid_res[2]];
		#if			!ENTROPY_ON_CUDA		// ADD-BY-LEETEN 02/02/2010
		calc_entropy( old_bin,grid_res, binnum, entropies);
		#if	0	// DEL-BY-LEETEN 02/02/2010-BEGIN
			dumpEntropyField("entropies.bin",entropies, grid_res);
			printf("entropy calculation done\n");
		#endif	// DEL-BY-LEETEN 02/02/2010-END
		// ADD-BY-LEETEN 02/02/2010-BEGIN
		#else	// #if	!ENTROPY_ON_CUDA	
		_ComputeSrcEntropyVolume(binnum, KERNEL_HALF_WIDTH, KERNEL_HALF_HEIGHT, KERNEL_HALF_DEPTH, &entropies[0]);
		#endif	// #if	!ENTROPY_ON_CUDA	
		// ADD-BY-LEETEN 02/02/2010-END
	}		
	int selected_line_num=0;
	float entropy=8888;
	std::vector<VECTOR3> seedlist,selected_list;
	// DEL-BY-LEETEN 01/22/2010 srand((unsigned)time(NULL));			// initialize random number generator
	// DEL-BY-LEETEN 02/02/2010		std::vector<float> entropies;

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
	#if	0	// DEL-BY-LEETEN 02/02/2010-BEGIN
		#if	USE_CUDA	
		_FlowFusionInit(grid_res[0], grid_res[1], grid_res[2]);
		#endif
	#endif	// DEL-BY-LEETEN 02/02/2010-END
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
	int line_num_thres=5000;//27;											//want to select top line_num_thres lines

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
	// MOD-BY-LEETEN 02/02/2010-FROM:
		// while(entropy>target_entropy)
	// TO:
	// MOD-BY-LEETEN 02/04/2010-FROM:
		// for(int iIter = 0; entropy>target_entropy; iIter++)
	// TO:
	bool bIsConverged = false;
	// MOD-BY-LEETEN 03/18/2010-FROM:
		// for(int iIter = 0; false == bIsConverged && entropy>target_entropy; iIter++)
	// TO:
	for(int iIter = 0; false == bIsConverged; iIter++)
	// MOD-BY-LEETEN 03/18/2010-END
	// MOD-BY-LEETEN 02/04/2010-END
	// MOD-BY-LEETEN 02/02/2010-END
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
		#if	!BIN_LOOKUP_ON_CUDA	// ADD-BY-LEETEN 02/02/2010

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

		// ADD-BY-LEETEN 02/02/2010-BEGIN
		#else	//	#if	!BIN_LOOKUP_ON_CUDA
		if( iIter > 0 )
			_GetDstBinVolume(new_bin);
		#endif	//	#if	!BIN_LOOKUP_ON_CUDA
		// ADD-BY-LEETEN 02/02/2010-END

		#if	0	// MOD-BY-LEETEN 03/10/2010-FROM:
			entropy=calcRelativeEntropy6_new(	vectors, new_vectors,  grid_res, VECTOR3(2,2,2),//do not count boundaries
										VECTOR3(grid_res[0]-2,grid_res[1]-2,grid_res[2]-2),theta,phi,old_bin,new_bin,0,binnum,histo_puv,histo_pv,
										pv,entropy_tmp);
		#else	// MOD-BY-LEETEN 03/10/2010-TO:
		entropy=calcRelativeEntropy6_new(	vectors, new_vectors,  grid_res, VECTOR3(0,0,0),//do not count boundaries
									VECTOR3(grid_res[0],grid_res[1],grid_res[2]),theta,phi,old_bin,new_bin,0,binnum,histo_puv,histo_pv,
									pv,entropy_tmp);
		#endif	// MOD-BY-LEETEN 03/10/2010-END

	//	entropy_list.push_back(entropy);
		for(int i=0;i<seeds.size();i++)
		{
			selected_list.push_back(seeds[i]);
			//seedlist.push_back(seeds[i]);
			seed_list.push_back(seeds[i]);
		}
	
		//dumpEntropy(entropies,"entropy.bin");
		// DEL-BY-LEETEN 02/04/2010-BEGIN
			// dumpSeeds(seed_list,"myseeds.seed");//crtical points excluded
		// DEL-BY-LEETEN 02/04/2010-END
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
		
		#if	0	// DEL-BY-LEETEN 02/02/2010-BEGIN
			unsigned char* dat=new unsigned char[grid_res[0]*grid_res[1]*grid_res[2]];
			for(int i=0;i<grid_res[0]*grid_res[1]*grid_res[2];i++)
				dat[i]=occupied[i]*255;
			/*FILE* test=fopen("impor.bin","wb");
			fwrite(grid_res,sizeof(int),3,test);
			fwrite(occupied,sizeof(unsigned char),grid_res[0]*grid_res[1]*grid_res[2],test);
			fclose(test);
			*/
			delete [] dat;
		#endif	// DEL-BY-LEETEN 02/02/2010-END

		for(int i=0;i<sl_list.size();i++)
		line_color.push_back(i);
		#if	0	// MOD-BY-LEETEN 02/04/2010-FROM:
			char filename[255];
			memset(filename,0,255);
			sprintf(filename,"streamlines%d.dat",round);
		#else	// MOD-BY-LEETEN 02/04/2010-TO:
		char filename[1024];
		// MOD-BY-LEETEN 03/18/2010-FROM:
			// sprintf(filename,"%s_streamlines%d.dat",g_filename, round);
		// TO:
		char *szFilename = filename;
		sprintf(szFilename, "%s_streamlines.dat", g_filename);
		// MOD-BY-LEETEN 03/18/2010-END
		#endif	// MOD-BY-LEETEN 02/04/2010-END

		save_streamlines_to_file_hand_tuning(filename,line_color,nSeeds);
		dumpEntropies(line_importance);

		// ADD-BY-LEETEN 03/18/2010-BEGIN
		sprintf(szFilename, "%s_streamlines.log", g_filename);
		char szCommand[1024];
		if( 1 == round )
		{
			sprintf(szCommand, "echo %d %f > %s", NR_OF_SAMPLES, log2(binnum), szFilename);
			system(szCommand);
		}
		sprintf(szCommand, "echo %d %f >> %s", sl_list.size(), entropy, szFilename);
		system(szCommand);
		// ADD-BY-LEETEN 03/18/2010-END

		//	getchar();
	//	printf("halted, save files now\n");
		// ADD-BY-LEETEN 02/04/2010-BEGIN
		// MOD-BY-LEETEN 03/18/2010-FROM:
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
		// MOD-BY-LEETEN 03/18/2010-END
		// ADD-BY-LEETEN 02/04/2010-END

		// ADD-BY-LEETEN 02/06/2010-BEGIN
		printf("seed %d selected, entropy=%f/%f \n",selected_line_num, entropy,target_entropy);
		// ADD-BY-LEETEN 02/06/2010-END
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

	// DEL-BY-LEETEN 02/02/2010		delete [] donot_change;
	delete [] vectors;
	delete [] new_vectors;
	// ADD-BY-LEETEN 2009/11/10-BEGIN

	#if	0		// MOD-BY-LEETEN 02/02/2010-FROM:
		#if	USE_CUDA	
		_FlowFusionFree();
		#endif
	#else		// MOD-BY-LEETEN 02/02/2010-TO:
	#if		USE_CUDA	
	_FlowDiffusionFree();
	#endif	// #if	USE_CUDA	
	#endif	// MOD-BY-LEETEN 02/02/2010-END
	// ADD-BY-LEETEN 2009/11/10-END

	// ADD-BY-LEETEN 03/18/2010-BEGIN
	double dwComputeStreamlinesEnd = GetTickCount();
	printf("\n\n %s takes %.3f milli-seconds.\n", __FUNCTION__, dwComputeStreamlinesEnd - dwComputeStreamlinesBegin); 	
	// ADD-BY-LEETEN 03/18/2010-END
}
/*
//not distribution based seeding
void compute_streamlines() 
{
	//test();//CGAL triangle meshing
	int binnum=360;


	printf("generating seeds...\n"); 
	int num=0;
	int grid_res[3];
	//Note: the vector is normalized
	float* vectors=get_grid_vec_data(grid_res);//get vec data at each grid point
	
	
	int nSeeds; 
	//set first seed as domain center
	vector<VECTOR3> seeds;
	VECTOR3* newseed=new VECTOR3;
	newseed->Set(grid_res[0]/2, grid_res[1]/2,grid_res[2]/2);
	seeds.push_back(*newseed);

	sl_list.clear(); 
							//combine the half lines
	int next_id=0;

	
	double dwStart;
	double elapsedTime;
	list<vtListSeedTrace*> lines,new_lines; 



	int selected_line_num=0;
	// DEL-BY-LEETEN 01/22/2010 srand((unsigned)time(NULL));			// initialize random number generator

	int* donot_change=new int[grid_res[0]*grid_res[1]*grid_res[2]];
	memset(donot_change,0,sizeof(int)*grid_res[0]*grid_res[1]*grid_res[2]);
	float* new_vectors=new float[grid_res[0]*grid_res[1]*grid_res[2]*3];
	memset(new_vectors,0,sizeof(float)*grid_res[0]*grid_res[1]*grid_res[2]*3);
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
	
	float entropy=9999;
	std::vector<int> line_color;

	//int line_num_thres=30; //source
	float dt_entropy,ori_entropy,old_entropy;
	bool first=true;
	dt_entropy=ori_entropy=entropy;
	
	printf("calculate the bin number\n");
	int* old_bin, *new_bin;
	old_bin=new int [grid_res[0]*grid_res[1]*grid_res[2]];
	new_bin=new int [grid_res[0]*grid_res[1]*grid_res[2]];

	for(int i=0;i<grid_res[0]*grid_res[1]*grid_res[2];i++)
	{
		VECTOR3 orif;
		orif.Set(vectors[i*3+0],vectors[i*3+1],vectors[i*3+2]);
		old_bin[i]=get_bin_number_3D(orif,theta, phi);
	}
	//dumphistogram(old_bin,grid_res, 360,0);

	//used to check entropy
	VECTOR3 startpt, endpt;

		
	std::vector<float> entropies;
//	while(1)
	float error=0.25;
	float target_entropy=0.1;//-error*log2(error)-(1-error)*log2(1-error)+error*log2(359);
	printf("entropy_target=%f\n",target_entropy);

	float* kx=new float[grid_res[0]*grid_res[1]*grid_res[2]];
	float* ky=new float[grid_res[0]*grid_res[1]*grid_res[2]];
	float* kz=new float[grid_res[0]*grid_res[1]*grid_res[2]];

	float* b=new float[grid_res[0]*grid_res[1]*grid_res[2]];
	float* c1=new float[grid_res[0]*grid_res[1]*grid_res[2]];
	float* c2=new float[grid_res[0]*grid_res[1]*grid_res[2]];
	float* c3=new float[grid_res[0]*grid_res[1]*grid_res[2]];

	// ADD-BY-LEETEN 2009/11/10-BEGIN
	#if	USE_CUDA	
	_FlowFusionInit(grid_res[0], grid_res[1], grid_res[2]);
	#endif
	// ADD-BY-LEETEN 2009/11/10-END

	 std::vector<vtListVertex*>	Graph;

	OCTree* Tree = new OCTree(grid_res[0]+1, grid_res[1]+1,grid_res[2]+1);
	printf("build octree tree\n");

	int finestlevel=16+1;//may play some role in the result, diffrerent data set, dif setting may help?
	Tree->Build(finestlevel);

	//set boundary condition
	g_new_vectors=new_vectors;
	int line_num_thres=2000; 
	int* occupied=new int[grid_res[0]*grid_res[1]*grid_res[2]];
	memset(occupied,0,sizeof(int)*grid_res[0]*grid_res[1]*grid_res[2]);
	
	entropy_every_point=new float [grid_res[0]*grid_res[1]*grid_res[2]];

//	int binnum=360;
	int* g_histo=0;//new int[binnum];
	int *g_histo_puv=0;//new int[binnum*binnum];
	int* g_histo_old=0;//new int[binnum];


	while(line_num_thres>selected_line_num && entropy>0.1)
	{

		memset(kx,0,sizeof(float)*grid_res[0]*grid_res[1]*grid_res[2]);
		memset(ky,0,sizeof(float)*grid_res[0]*grid_res[1]*grid_res[2]);
		memset(kz,0,sizeof(float)*grid_res[0]*grid_res[1]*grid_res[2]);
		memset(b,0,sizeof(float)*grid_res[0]*grid_res[1]*grid_res[2]);
		memset(c1,0,sizeof(float)*grid_res[0]*grid_res[1]*grid_res[2]);
		memset(c2,0,sizeof(float)*grid_res[0]*grid_res[1]*grid_res[2]);
		memset(c3,0,sizeof(float)*grid_res[0]*grid_res[1]*grid_res[2]);

	//	printf("\n\n no %d seed selected = %f %f %f\n", selected_line_num, newseed->x(), newseed->y(),newseed->z());

		list<vtListSeedTrace*> lines,new_lines; 
		//printf("compute streamlines..\n"); 
		osuflow->SetEntropySeedPoints( newseed,1);
		osuflow->SetIntegrationParams(1, 1);							//small and large step size
		osuflow->GenStreamLines(lines , BACKWARD_AND_FORWARD, 500, 0);	 //maxi steps
		//printf(" done integrations\n"); 

		//trim the segments that enters low entropy region
		//trim_dull_segments(lines, new_bin, old_bin, grid_res);

		//combine the half lines
	
		UpdateOccupied(lines, occupied,grid_res);
		combinehalflines(lines, new_lines);

		sl_list.push_back(*(new_lines.begin()));


	//	printf("start reconstructing the  fieldn");

dwStart = GetTickCount();
//		reconstruct_field_GVF_3D(new_vectors,vectors,grid_res,sl_list,donot_change);
//testing a faster reconstruction	
//for(int kk=0;kk<5;kk++)
reconstruct_field_GVF_3D(new_vectors,vectors,grid_res,new_lines,donot_change,
						 kx,ky,kz,b,c1,c2,c3);//,importance);
elapsedTime= GetTickCount() - dwStart;
printf("\n reconstruction time is %.3f milli-seconds.\n",elapsedTime); 	
	




	//printf("calculate the bin number\r");
	for(int i=0;i<grid_res[0]*grid_res[1]*grid_res[2];i++)
	{
		VECTOR3 newf,orif;
		orif.Set(vectors[i*3+0],vectors[i*3+1],vectors[i*3+2]);
		newf.Set(new_vectors[i*3+0],new_vectors[i*3+1],new_vectors[i*3+2]);
		//if within error range, let it be teh same bin as the ori, otherwise select the bin num
		newf.Normalize();
		float dotv=dot(newf,orif);
//		if(acos(dotv)<(3.14*0.0255))//less than 3 degree difference
//			new_bin[i]=old_bin[i];
//		else
			new_bin[i]=get_bin_number_3D(newf,theta, phi,binnum);
	}
	
	


	//	VECTOR3 next_seed=selectNextSeed(vectors,new_vectors, grid_res, Tree,theta,phi,old_bin,new_bin);
		VECTOR3 next_seed=selectNextSeed3(vectors,new_vectors, grid_res, Tree,theta,phi,old_bin,new_bin,
			Graph,occupied,g_histo,g_histo_puv);
		if(next_seed.x()<0 )
		{
			printf("invalid seed\n");
			break;
		}
		if ((next_seed.x()==newseed->x()) && (next_seed.y()==newseed->y()) &&(next_seed.z()==newseed->z()))
		{
			//delete [] donot_change;
			//delete [] new_vectors;		
			printf("found the same seed\n");
			break;
		}
		seed_list.push_back(next_seed);

dwStart = GetTickCount();
elapsedTime= GetTickCount() - dwStart;
//printf("\n *1 entropy calc time is %f milli-seconds.\n",elapsedTime); 

//		printf("seed %d selected, entropy=%f/%f \n",selected_line_num, entropy,target_entropy);
//dwStart = GetTickCount();
//for(int i=0;i<10;i++)
		entropy=calcRelativeEntropy6(	vectors, new_vectors,  grid_res, VECTOR3(2,2,2),//do not count boundaries
									VECTOR3(grid_res[0]-2,grid_res[1]-2,grid_res[2]-2), theta, phi,old_bin,new_bin,occupied,0);
//		float point_dist=calcPoint2PointError(	vectors, new_vectors,  grid_res, VECTOR3(2,2,2),//do not count boundaries
//									VECTOR3(grid_res[0]-2,grid_res[1]-2,grid_res[2]-2));
		entropies.push_back(entropy);
		printf("seed %d selected, entropy=%f/%f \n",selected_line_num, entropy,target_entropy);

//elapsedTime= (double)GetTickCount() - dwStart;
//printf("\n * 2entropy calc time is %f milli-seconds.\n",elapsedTime); 	

		newseed->Set(next_seed.x(),next_seed.y(),next_seed.z());
		seeds.push_back(*newseed);

		int seed_id=(int)next_seed.x()+((int)next_seed.y())*grid_res[0]
					+((int)next_seed.z())*grid_res[0]*grid_res[1];
		new_vectors[seed_id*3+0]=vectors[seed_id*3+0];
		new_vectors[seed_id*3+1]=vectors[seed_id*3+1];
		new_vectors[seed_id*3+2]=vectors[seed_id*3+2];
		donot_change[seed_id]=1;



		line_color.push_back(selected_line_num);
																//select new seed
		selected_line_num++;
	dumpSeeds( seeds, "myseeds.seed");
	//dumpEntropies(entropies);
	//dumphistogram(new_bin, grid_res, 360, selected_line_num);
//	dump_entrpy_every_point(Tree,entropy_every_point,grid_res,
//							 vectors, new_vectors, theta, phi, old_bin, new_bin,occupied);

	//save_streamlines_to_file_hand_tuning(line_color,selected_line_num);
	
	}

	if(g_histo)
	delete [] g_histo;
	if(g_histo_puv)
	delete [] g_histo_puv;
	if(g_histo_old)
	delete [] g_histo_old;
	//dump_entrpy_every_point(Tree,entropy_every_point,grid_res,
	//						 vectors, new_vectors, theta, phi, old_bin, new_bin,occupied);
//	dumpReconstruedField("r.vec", new_vectors, grid_res);
	//dumpImportanceField("importance.bin",importance,grid_res);
	//printf("*** reconstructed field saved*** \n");	
	//dump entropy
//	dumpArray( entropies, "myentropy.txt");//dump the drop of entropy

	//add_context_streamlines(line_color);
	//save_streamlines_to_file_hand_tuning(line_color,selected_line_num);
	save_streamlines_to_file_hand_tuning(line_color,nSeeds);

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
	//delete [] new_vectors;
	delete [] vectors;
	//delete [] importance;

	// ADD-BY-LEETEN 2009/11/10-BEGIN
	#if	USE_CUDA	
	_FlowFusionFree();
	#endif
	// ADD-BY-LEETEN 2009/11/10-END
}
*/
void testReconstruction()
{


	printf("generating seeds...\n"); 
	int num=0;
	int grid_res[3];
	float* vectors=get_grid_vec_data(grid_res);//get vec data at each grid point


												//load seeds from file
	int nSeeds; 
	//set first seed as domain center
	VECTOR3* newseed=new VECTOR3;

	sl_list.clear(); 
							//combine the half lines
	int next_id;
	float * nearest_dist=new float[grid_res[0]*grid_res[1]*grid_res[2]];
	for(int i=0; i<grid_res[0]*grid_res[1]*grid_res[2];i++)
		nearest_dist[i]=9999.0;
//	initial_boundaries(sl_list, footsteps, grid_res, 0, next_id); //donot think about bounds for now
	float* new_vectors=new float[grid_res[0]*grid_res[1]*grid_res[2]*3];
	memset(new_vectors,0,sizeof(float)*grid_res[0]*grid_res[1]*grid_res[2]*3);

	float* importance=new float[grid_res[0]*grid_res[1]*grid_res[2]];
	memset(importance,0,sizeof(float)*grid_res[0]*grid_res[1]*grid_res[2]);
	
	double dwStart;
	double elapsedTime;
	list<vtListSeedTrace*> lines,new_lines; 
	
	int nSeeds_load; 
	//VECTOR3* seeds=loadSeedandRender(strcat(g_filename,".data"),nSeeds);
	VECTOR3* seeds=loadSeedfromFile("myseeds.seed",nSeeds_load);


	int* donot_change=new int[grid_res[0]*grid_res[1]*grid_res[2]];
	memset(donot_change,0,sizeof(int)*grid_res[0]*grid_res[1]*grid_res[2]);
	
	int line_num_thres=5;											//want to select top line_num_thres lines
	int selected_line_num=0;
	// DEL-BY-LEETEN 01/22/2010 srand((unsigned)time(NULL));			// initialize random number generator
	int count=0;
	while(count<nSeeds_load)
	{
		float ranx = ((float)rand()/(float)RAND_MAX)*grid_res[0];
		float rany = ((float)rand()/(float)RAND_MAX)*grid_res[1];
		float ranz = ((float)rand()/(float)RAND_MAX)*grid_res[2];

		//read the seed idx in the box
		newseed->Set(	seeds[count].x(),
						seeds[count].y(),
						seeds[count].z());//selected_seed_id;

		count++;

		printf("no %d seed selected = %f %f %f\n", selected_line_num, newseed->x(), newseed->y(),newseed->z());

		list<vtListSeedTrace*> lines,new_lines; 
		printf("compute streamlines..\n"); 
		osuflow->SetEntropySeedPoints( newseed,1);
		osuflow->SetIntegrationParams(1, 5);							//small and large step size
		osuflow->GenStreamLines(lines , BACKWARD_AND_FORWARD, 100, 0);	 //maxi steps
		printf(" done integrations\n"); 
																	//combine the half lines
		combinehalflines(lines, new_lines);
		sl_list.push_back(*(new_lines.begin()));

		printf("start reconstructing the  fieldn");

		vtListSeedTrace* trace=*(new_lines.begin());
		printf("line len=%d\n", trace->size());
//dwStart = GetTickCount();
//		reconstruct_field_GVF_3D(new_vectors,vectors,grid_res,new_lines,donot_change,importance);
		//reconstruct_field_direct(new_vectors,nearest_dist, vectors,grid_res,new_lines);//no triangulation is used
//elapsedTime= GetTickCount() - dwStart;
//printf("\n\n reconstruction time is %.3f milli-seconds.\n",elapsedTime); 	
	
		printf("*** reconstructed field saved*** \n");

		selected_line_num++;
		
	}
	dumpReconstruedField("r_diff.vec", new_vectors, grid_res);
	printf("press key to continue\n");
	getchar();	

	delete [] nearest_dist;
	delete [] new_vectors;
	delete [] donot_change;
	delete [] importance;

}


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
	int crop=135+8;
	xdim=XDim; ydim=YDim-2*crop;
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

	save2PPM_3(filename, crop_data,xdim,ydim);
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

void draw_sphere_mesh()
{
	glPushMatrix(); 
//	glScalef(1/(float)len[0], 1/(float)len[0], 1/(float)len[0]); 
//	glTranslatef(-len[0]/2.0, -len[1]/2.0, -len[2]/2.0); 

	glColor4f(1.0f, 0.0f, 0.0f,0.5f);

	for(int i=0; i<sphere_mesh.size()/3; i+=30)
	{
	glBegin(GL_LINES);
		glVertex3f(sphere_mesh[i*3+0].x(),sphere_mesh[i*3+0].y(),sphere_mesh[i*3+0].z());
		glVertex3f(sphere_mesh[i*3+1].x(),sphere_mesh[i*3+1].y(),sphere_mesh[i*3+1].z());

		glVertex3f(sphere_mesh[i*3+1].x(),sphere_mesh[i*3+1].y(),sphere_mesh[i*3+1].z());
		glVertex3f(sphere_mesh[i*3+2].x(),sphere_mesh[i*3+2].y(),sphere_mesh[i*3+2].z());

		glVertex3f(sphere_mesh[i*3+2].x(),sphere_mesh[i*3+2].y(),sphere_mesh[i*3+2].z());
		glVertex3f(sphere_mesh[i*3+0].x(),sphere_mesh[i*3+0].y(),sphere_mesh[i*3+0].z());
	glEnd();
	}

	glPopMatrix();
}
void draw_triangulation()
{
		glPushMatrix(); 
		glScalef(1/(float)len[0], 1/(float)len[0], 1/(float)len[0]); 
		glTranslatef(-len[0]/2.0, -len[1]/2.0, -len[2]/2.0); 

	//if(!glIsList(triangle_list))
	{

	//	triangle_list=glGenLists(1);

	//	glNewList(triangle_list, GL_COMPILE);

		glColor4f(1.0f, 0.0f, 0.0f,0.5f);

		VECTOR3 p[3];

		int t[3]; t[0]=len[0]; t[1]=len[1]; t[2]=len[2];


		int grid_res[3];
		grid_res[0]=len[0]; grid_res[1]=len[1]; grid_res[2]=len[2];

		//vector<VECTOR3> tetrahedras;
		//doTriangulation3D(sl_list, tetrahedras, grid_res);

		//get the point inside each triangles

		glBegin(GL_LINES);

		for(int i=0; i<tetrahedras.size()/4;i++)
		{
			if(i%4==0)
			glColor4f(1.0f, 0.0f, 0.0f,0.5f);
			else if(i%4==1)
			glColor4f(1.0f, 0.0f, 1.0f,0.5f);
			else if(i%4==2)
			glColor4f(0.0f, 0.0f, 1.0f,0.5f);
			else if(i%4==3)
			glColor4f(0.0f, 1.0f, 0.0f,0.5f);

			//glBegin(GL_LINE_STRIP);

			bool found=false;
			int id[4],v[4];
			VECTOR3 p[4],q;

			int j,k;
			j=0; 
			for(k=1; k<=3; k++)
			{
			glVertex3f(tetrahedras[i*4+j].x(), tetrahedras[i*4+j].y(), tetrahedras[i*4+j].z());
			glVertex3f(tetrahedras[i*4+k].x(), tetrahedras[i*4+k].y(), tetrahedras[i*4+k].z());
			}
			j=1; k=2; 
			glVertex3f(tetrahedras[i*4+j].x(), tetrahedras[i*4+j].y(), tetrahedras[i*4+j].z());
			glVertex3f(tetrahedras[i*4+k].x(), tetrahedras[i*4+k].y(), tetrahedras[i*4+k].z());
			j=2; k=3; 
			glVertex3f(tetrahedras[i*4+j].x(), tetrahedras[i*4+j].y(), tetrahedras[i*4+j].z());
			glVertex3f(tetrahedras[i*4+k].x(), tetrahedras[i*4+k].y(), tetrahedras[i*4+k].z());
			j=1; k=3; 
			glVertex3f(tetrahedras[i*4+j].x(), tetrahedras[i*4+j].y(), tetrahedras[i*4+j].z());
			glVertex3f(tetrahedras[i*4+k].x(), tetrahedras[i*4+k].y(), tetrahedras[i*4+k].z());

		}
	//	glEndList();
			glEnd();

	}
//	glCallList(triangle_list);
		glPopMatrix(); 

}
///glsl
char *textFileRead(char *fn) {


	FILE *fp;
	char *content = NULL;

	int count=0;

	if (fn != NULL) {
		fp = fopen(fn,"rt");

		if (fp != NULL) {
      
      fseek(fp, 0, SEEK_END);
      count = ftell(fp);
      rewind(fp);

			if (count > 0) {
				content = (char *)malloc(sizeof(char) * (count+1));
				count = fread(content,sizeof(char),count,fp);
				content[count] = '\0';
			}
			fclose(fp);
		}
	}
	return content;
}
void SetShaders()
{
	if (glewInit()!=GLEW_OK) 
	{
		printf("Glew init error\n");
	    exit(-1);
	}

	if (GLEW_ARB_vertex_shader && GLEW_ARB_fragment_shader)
		printf("Ready for GLSL\n");
	else {
		printf("No GLSL support\n");
		exit(1);
	}

	glewInit();
	if (GLEW_ARB_vertex_shader && GLEW_ARB_fragment_shader)
		printf("Ready for GLSL\n");
	else {
		printf("No GLSL support\n");
		//exit(1);
	}

	char *vs = NULL,*fs = NULL,*fs2 = NULL;
	GLhandleARB v,f,f2,p;

	v = glCreateShaderObjectARB(GL_VERTEX_SHADER_ARB);
	f = glCreateShaderObjectARB(GL_FRAGMENT_SHADER_ARB);


	vs = textFileRead("shaders\\vertexshader.glsl");
	fs = textFileRead("shaders\\fragmentshader.glsl");

	const char * vv = vs;
	const char * ff = fs;

	glShaderSourceARB(v, 1, &vv,NULL);
	glShaderSourceARB(f, 1, &ff,NULL);

	free(vs);
	free(fs);

	glCompileShaderARB(v);
	glCompileShaderARB(f);


	p = glCreateProgramObjectARB();
	glAttachObjectARB(p,v);
	glAttachObjectARB(p,f);

	glLinkProgramARB(p);
	glUseProgramObjectARB(p);

 	my_vec3_location = glGetAttribLocationARB(p, "vernext");

	char szMsg[100];
	GLenum err = glGetError();
    if (err != GL_NO_ERROR)
		sprintf(szMsg,"OpenGL error: %s ", gluErrorString(err));
}


void draw_streamlines_load_file() {
	//SetShaders();
	glEnable(GL_LINE_SMOOTH);
	glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
	glLineWidth(2);
	glEnable(GL_BLEND);

	FILE* fp=fopen("data\\velocity.dat","rb");
	int line_num;
	fread(&line_num,sizeof(int),1,fp);
	int* vernums=new int[line_num];
	fread(vernums,sizeof(int),line_num,fp);
	int ver_num=0;
	for(int i=0;i<line_num;i++)
		ver_num=ver_num+vernums[i];

	float* vertices=new float[3*ver_num];
	fread(vertices,sizeof(float),3*ver_num,fp);
	fclose(fp);

	
	//draw the lines
	int cur_ver=0;
	for(int i=0;i<line_num;i++)
	{
		glBegin(GL_LINE_STRIP);
		for(int j=0;j<vernums[i];j++)
		{
			float x,y,z;
			x=vertices[(cur_ver+j)*3+0];
			y=vertices[(cur_ver+j)*3+1];
			z=vertices[(cur_ver+j)*3+2];

			if(j==vernums[i]-1)
				glVertexAttrib3f(my_vec3_location, vertices[(cur_ver+j-1)*3+0],
													vertices[(cur_ver+j-1)*3+1],
													vertices[(cur_ver+j-1)*3+2]);
		
			else
				glVertexAttrib3f(my_vec3_location,	vertices[(cur_ver+j+1)*3+0],
													vertices[(cur_ver+j+1)*3+1],
													vertices[(cur_ver+j+1)*3+2]);
		
			glVertex3f(x,y,z);
		}
		glEnd();
		cur_ver=cur_ver+vernums[i];
	}
	glDisable(GL_BLEND);
	delete [] vertices;
	glUseProgramObjectARB(0);

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

	// ADD-BY-LEETEN 01/22/2010-BEGIN
	glScalef(3.0f, 3.0f, 3.0f); 
	// ADD-BY-LEETEN 01/22/2010-END

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
int getDepth(const GLfloat *v, GLfloat *mvp)
{
	GLfloat	depth;

	depth  = mvp[2] * v[0] + mvp[6] * v[1] + mvp[10] * v[2] + mvp[14];
	depth /= mvp[3] * v[0] + mvp[7] * v[1] + mvp[11] * v[2] + mvp[15];

	/* depth now lies in the range [-1, +1].
	 * Map it to the integer range [-INT_MAX / 4, +INT_MAX / 4]. */

	return ((int)(depth * INT_MAX) / 4);
}
struct DepthSorter
{
	int	*depths;

	DepthSorter(int *depths)
	{
		this->depths = depths;
	}

	bool operator()(const int &i, const int &j) const
	{
		return depths[i] > depths[j];
	}
};

int * zsort_bound()
	{
		int			i, j;
		int			arrSize;
		int			segCount;
		int			idx;
		int			*indices, *indicesIter;
		int			*depths;
		GLfloat		model[16], proj[16], mvp[16];
		GLenum		matrixMode;

		/* Compute the modelview-projection matrix and
		 * restore the rendering state. */
		glGetIntegerv(GL_MATRIX_MODE, (GLint *)&matrixMode);
		glGetFloatv(GL_MODELVIEW_MATRIX, model);
		glGetFloatv(GL_PROJECTION_MATRIX, proj);
		glMatrixMode(GL_PROJECTION);
		glMultMatrixf(model);
		glGetFloatv(GL_PROJECTION_MATRIX, mvp);
		glLoadMatrixf(proj);
		glMatrixMode(matrixMode);

		int vernum,trinum,tetnum;
		FILE* fIn = fopen("data\\b8.graph.b.outter.grid","rb");

		fread(&vernum, sizeof(int), 1, fIn);
		fread(&trinum, sizeof(int), 1, fIn);
		fread(&tetnum, sizeof(int), 1, fIn);

		float* vertices;
		unsigned int *faces;
		vertices=new float[vernum*3];
		fread(vertices, sizeof(float), vernum*3, fIn);
		faces=new unsigned int[trinum*3];
		fread(faces, sizeof(int), trinum*3, fIn);
		fclose(fIn);

		indices = new int[trinum];
		for( i=0;i<trinum;i++)
			indices[i]=i;
		depths = new int[trinum];

			float mindist=0;
			int count=0;
		for (i = 0; i < trinum; i++)
		{
			//for(int j=0;j<3;j++)
			{
				idx = faces[i*3+0];//the first vertex
				GLfloat homVertices[4];
				homVertices[0]=vertices[idx*3+0];
				homVertices[1]=vertices[idx*3+1];
				homVertices[2]=vertices[idx*3+2];
				homVertices[3]=1;

				depths[i] = getDepth(homVertices, mvp);

				

				
			}

		}
		//printf("maxz=%f\n",mindist);


		std::sort(indices, indices+trinum-1 , DepthSorter(depths));
//#endif

		delete [] depths;
		delete [] vertices;
		delete [] faces;
		return (indices);
	}
void drawBounds_inner()
{
	glShadeModel(GL_SMOOTH);
	GLfloat light_specular[] = {1,1,1, 1.0};
	GLfloat light_diffuse[] = {.41,.41,.41, 1.0};
	GLfloat light_ambient[] = {0.2,0.2,0.2, 1.0};	

	glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);

	glEnable(GL_COLOR_MATERIAL);
   // glColorMaterial ( GL_FRONT_AND_BACK, GL_SPECULAR ) ;

	GLfloat mat_specular[] = {1,1,1, 1.0};
	GLfloat mat_diffuse[] = {.4,.4,.4, 1.0};	
	GLfloat mat_ambient[] = {0.1, 0.1, 0.1, 1.0};
	GLfloat mat_shininess[] = {3000};
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, mat_specular);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, mat_diffuse);
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, mat_ambient);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, mat_shininess);

	glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, 1);		


	glEnable ( GL_LIGHTING ) ;
	glEnable(GL_LIGHT0);
	glEnable( GL_NORMALIZE );
	//glDisable(GL_BLEND);


	int vernum,trinum,tetnum;
	FILE* fIn = fopen("data\\b8.graph.b.inner.grid","rb");

	fread(&vernum, sizeof(int), 1, fIn);
	fread(&trinum, sizeof(int), 1, fIn);
	fread(&tetnum, sizeof(int), 1, fIn);

	float* vertices;
	unsigned int *faces;
	vertices=new float[vernum*3];
	fread(vertices, sizeof(float), vernum*3, fIn);
	faces=new unsigned int[trinum*3];
	fread(faces, sizeof(int), trinum*3, fIn);
	fclose(fIn);

	//sort the boundary
	//glEnable(GL_BLEND);

	glColor4f(1,1,1,1);
	glBegin(GL_TRIANGLES);
	for(int i=0;i<trinum;i++)
	{
	
	
		for(int j=0;j<3;j++)
		{

			VECTOR3 n,v1,v2;

			int id0,id1,id2;
			id0=faces[i*3+j];//indictes
			if(j==0)
			{
			id1=faces[i*3+1];
			id2=faces[i*3+2];
			}
			else if(j==1)
			{
			id1=faces[i*3+2];
			id2=faces[i*3+0];
			}
			else
			{
			id1=faces[i*3+0];
			id2=faces[i*3+1];
			}
			//2-0
			v1=	VECTOR3(	vertices[id2*3+0]-vertices[id0*3+0],
							vertices[id2*3+1]-vertices[id0*3+1],
							vertices[id2*3+2]-vertices[id0*3+2]);
			//1-0
			v2= VECTOR3(	vertices[id1*3+0]-vertices[id0*3+0],
							vertices[id1*3+1]-vertices[id0*3+1],
							vertices[id1*3+2]-vertices[id0*3+2]);

			VECTOR3 norm=cross(v1,v2);
			norm.Normalize();

			int verid=faces[i*3+j];//indictes
			glNormal3f(-norm.x(),-norm.y(),-norm.z());
			glVertex3f(vertices[verid*3+0],vertices[verid*3+1],vertices[verid*3+2]);
		}	
	}
	glEnd();
	delete [] vertices;
	delete [] faces;

	//glDisable(GL_BLEND);
	glDisable(GL_COLOR_MATERIAL);
	glDisable( GL_NORMALIZE );

	glDisable(GL_LIGHTING);


}
void drawBounds_outter()
{
	GLfloat light_specular[] = {1,1,1, 1.0};
	GLfloat light_diffuse[] = {.1,.1,.1, 1.0};
	GLfloat light_ambient[] = {0,0,0, 1.0};	

	glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);

	glEnable(GL_COLOR_MATERIAL);
    glColorMaterial ( GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE ) ;

	GLfloat mat_specular[] = {1,1,1, 1.0};
	GLfloat mat_diffuse[] = {0.1, 0.1, 0.1, 1.0};	
	GLfloat mat_ambient[] = {0,0,0, 1.0};
	GLfloat mat_shininess[] = {1000};
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, mat_specular);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, mat_diffuse);
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, mat_ambient);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, mat_shininess);

	glEnable ( GL_LIGHTING ) ;
	glEnable(GL_LIGHT0);
	glEnable( GL_NORMALIZE );

	glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, 1);		



	FILE* fIn = fopen("data\\b8.graph.b.outter.grid","rb");

	int vernum,trinum,tetnum;
	float* vertices;
	unsigned int* faces;
	fread(&vernum, sizeof(int), 1, fIn);
	fread(&trinum, sizeof(int), 1, fIn);
	fread(&tetnum, sizeof(int), 1, fIn);

	vertices=new float[vernum*3];
	fread(vertices, sizeof(float), vernum*3, fIn);
	faces=new unsigned int[trinum*3];
	fread(faces, sizeof(int), trinum*3, fIn);
	fclose(fIn);

	//sort the boundary
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	int* indices = zsort_bound();
	glColor4f(1,1,1,0.15);
	glBegin(GL_TRIANGLES);
	for(int i=0;i<trinum/2;i++)
	{
	
		VECTOR3 n,v1,v2;
		int id0=faces[indices[i]*3+0];//indictes
		int id1=faces[indices[i]*3+1];
		int id2=faces[indices[i]*3+2];

		//2-0
		v1=	VECTOR3(	vertices[id2*3+0]-vertices[id0*3+0],
						vertices[id2*3+1]-vertices[id0*3+1],
						vertices[id2*3+2]-vertices[id0*3+2]);
		//1-0
		v2= VECTOR3(	vertices[id1*3+0]-vertices[id0*3+0],
						vertices[id1*3+1]-vertices[id0*3+1],
						vertices[id1*3+2]-vertices[id0*3+2]);

		VECTOR3 norm=cross(v1,v2);
		norm.Normalize();
		for(int j=0;j<3;j++)
		{
			int verid=faces[indices[i]*3+j];//indictes
			glNormal3f(-norm.x(),-norm.y(),-norm.z());
			glVertex3f(vertices[verid*3+0],vertices[verid*3+1],vertices[verid*3+2]);
		}	
	}
	glEnd();
	
	draw_streamlines_load_file();

	glEnable(GL_BLEND);
	glColor4f(1,1,1,0.15);
	glBegin(GL_TRIANGLES);
	for(int i=trinum/2;i<trinum;i++)
	{
	
		VECTOR3 n,v1,v2;
		int id0=faces[indices[i]*3+0];//indictes
		int id1=faces[indices[i]*3+1];
		int id2=faces[indices[i]*3+2];

		//2-0
		v1=	VECTOR3(	vertices[id2*3+0]-vertices[id0*3+0],
						vertices[id2*3+1]-vertices[id0*3+1],
						vertices[id2*3+2]-vertices[id0*3+2]);
		//1-0
		v2= VECTOR3(	vertices[id1*3+0]-vertices[id0*3+0],
						vertices[id1*3+1]-vertices[id0*3+1],
						vertices[id1*3+2]-vertices[id0*3+2]);

		VECTOR3 norm=cross(v1,v2);
		norm.Normalize();
		for(int j=0;j<3;j++)
		{
			int verid=faces[indices[i]*3+j];//indictes
			glNormal3f(-norm.x(),-norm.y(),-norm.z());
			glVertex3f(vertices[verid*3+0],vertices[verid*3+1],vertices[verid*3+2]);
		}	
	}
	glEnd();
	glDisable(GL_BLEND);
	glDisable(GL_COLOR_MATERIAL);
	glDisable( GL_NORMALIZE );

	glDisable(GL_LIGHTING);

	delete [] vertices;
	delete [] faces;
	delete [] indices;
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
		// ADD-BY-LEETEN 03/18/2010-BEGIN
		case 27:
			exit(0);
			break;
		// ADD-BY-LEETEN 03/18/2010-END

	case 'Q': 
	case 'q': 
		compute_streamlines_by_click_cleanup();
		//exit(1);
		break; 
	case 'R':
	case 'r':
		compute_streamlines_random();
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
	case 'W':
	case 'w':
		copyFrameBuffer(strcat(g_filename,".ppm"),winWidth,winHeight);
		break;
	case 'T':
	case 't':
		//testReconstruction();
		toggle_entropy==0?toggle_entropy=1:toggle_entropy=0;
		glutPostRedisplay(); 

		break;

	case 'P':
	case 'p':
		compute_streamlines_select_salient_streamlines();
		break;

	case 'H':
	case 'h':
		hand_tune_alpha();
		break;

	case 'B':
	case 'b':
		compute_bundles(10);
		break;

	case 'N':
	case 'n':
		compute_streamlines_by_click();
		glutPostRedisplay(); 
		break;

	case 'J':
	case 'j':
	printf("adjust begin\n");
	adjustLengthByEntropies("entropies.bin","streamlines.dat","velocity.dat");
	printf("adjust done\n");
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
void readPatches_center()
{
	if(!xyz)
		xyz=new float[3*360];
	float x,y,z;
	FILE* fp=fopen("xyz.txt","r");
	for(int i=0;i<360; i++)
	{
		fscanf(fp,"%f, %f, %f", &x,&y,&z);
		xyz[i*3+0]=x;
		xyz[i*3+1]=y;
		xyz[i*3+2]=z;
	}
	fclose(fp);
}
void readPatches_region()
{
	if(!theta)
		theta=new float[2*360];

	if(!phi)
		phi=new float[2*360];

	float x[2],y[2];
	FILE* fp=fopen("patch.txt","r");
	for(int i=0;i<360; i++)
	{
		fscanf(fp,"%f, %f", &x[0],&y[0]);
		fscanf(fp,"%f, %f", &x[1],&y[1]);
	
		theta[i*2+0]=x[0];
		theta[i*2+1]=y[0];
		phi[i*2+0]=x[1];
		phi[i*2+1]=y[1];
	}
	fclose(fp);
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

void process_time_varying()
{

}

void downsampling()
{
	int times=4;
	//down sampling karman
	FILE* in=fopen("plume.vec","rb");
	FILE* out=fopen("plume_s.vec","wb");
	int ds[3],dss[3]; 
	fread(ds,sizeof(int),3,in);
	float* vecs=new float[ds[0]*ds[1]*ds[2]*3];
	fread(vecs,sizeof(float),ds[0]*ds[1]*ds[2]*3,in);

	dss[0]=ds[0]/times;
	dss[1]=ds[1]/times;
	dss[2]=ds[2]/times;

	float* vecs_s=new float[dss[0]*dss[1]*dss[2]*3];

	for(int z=0;z<dss[2];z++)
	{
		for(int y=0;y<dss[1];y++)
		{
			for(int x=0;x<dss[0];x++)
			{
				int idx_s=(x+y*dss[0]+z*dss[0]*dss[1]);
				int idx  =(x*times+y*times*ds[0]+z*times*ds[0]*ds[1]);
				vecs_s[idx_s*3+0]=vecs[idx*3+0];
				vecs_s[idx_s*3+1]=vecs[idx*3+1];
				vecs_s[idx_s*3+2]=vecs[idx*3+2];
			}
		}
	}

	fwrite(dss,sizeof(int),3,out);
	for(int z=0;z<ds[2]*2;z+=2)
	{
		fwrite(vecs_s,sizeof(float),dss[0]*dss[1]*dss[2]*3,out);
	}
	fclose(in);
	fclose(out);
	delete [] vecs;
	delete [] vecs_s;
	printf("done\n");
	getchar();

}

#if	0	// DEL-BY-LEETEN 01/22/2010-BEGIN
	void testTriangulation()
	{

		//read in the velocity field

		int grid_res[3];
		float* vectors;
		FILE* fp1=fopen(g_filename,"rb");
		fread(grid_res,sizeof(int),3,fp1);

		vectors=new float[grid_res[0]*grid_res[1]*grid_res[2]*3];
		fread(vectors,sizeof(float),grid_res[0]*grid_res[1]*grid_res[2]*3,fp1);
		fclose(fp1);

		typedef CGAL::Exact_predicates_inexact_constructions_kernel K;

		typedef CGAL::Triangulation_3<K>      Triangulation;

		typedef Triangulation::Cell_handle    Cell_handle;
		typedef Triangulation::Vertex_handle  Vertex_handle;
		typedef Triangulation::Locate_type    Locate_type;
		typedef Triangulation::Point          Point;


		// construction from a list of points :
		std::list<Point> L;

		FILE* fp=fopen("velocity.dat","rb");

		int line_num, *ver_num, total_ver_num;
		total_ver_num=0;
		fread(&line_num,sizeof(int),1,fp);
		ver_num=new int[line_num];
		fread(ver_num,sizeof(int),line_num,fp);

		for(int i=0;i<line_num;i++)
			total_ver_num+=ver_num[i];

		VECTOR3* ver=new VECTOR3[total_ver_num];
		fread(ver,sizeof(VECTOR3),total_ver_num,fp);
		fclose(fp);



		for(int i=0;i<total_ver_num;i++)
		{
			L.push_front(Point(	ver[i].x(),
								ver[i].y(),
								ver[i].z()));
		}

		
	  Triangulation T(L.begin(), L.end());


		//reconstructed field
		float * new_vectors=new float[grid_res[0]*grid_res[1]*grid_res[2]*3];
		for(int z=0;z<grid_res[2];z++)
		{
			for(int y=0;y<grid_res[1];y++)
			{
				for(int x=0;x<grid_res[0];x++)
				{
					//locate the value to be interpolate from 
					Point p0(x,y,z);
					Locate_type lt;
					int li, lj;
					VECTOR3 vec;
					vec.Set(0,0,0);
					Cell_handle c = T.locate(p0, lt, li, lj);
					int idx=x+y*grid_res[0]+z*grid_res[0]*grid_res[1];

					if( lt == Triangulation::VERTEX )
					{
						vec.Set(vectors[idx*3+0],vectors[idx*3+1],vectors[idx*3+2]);
					}
					else if(	lt == Triangulation::EDGE ||
								lt == Triangulation::FACET ||
								lt == Triangulation::CELL)
					{
						
						//interpolate from the four vertices
						Point vp[4];
						VECTOR3 vertices_vec[4];
						std::vector<float>  distance;
						std::vector<int>	index;
						for(int i=0;i<4;i++)
						{
							vp[i]= c->vertex( i )->point();


							distance.push_back(sqrt((vp[i].x()-p0.x())*(vp[i].x()-p0.x())+
											(vp[i].y()-p0.y())*(vp[i].y()-p0.y())+
											(vp[i].z()-p0.z())*(vp[i].z()-p0.z())));
							index.push_back(i);
							int idx=(int)vp[i].x()+	((int)vp[i].y())*grid_res[0]+
													((int)vp[i].z())*grid_res[0]*grid_res[1];
							vertices_vec[i].Set(vectors[3*idx+0],
								vectors[3*idx+1],
								vectors[3*idx+2]);

						}

						for(int i=0;i<3; i++)
						{
							for(int j=i+1; j<4; j++)
							{
								if(distance[i]>distance[j])
								{
									float t=distance[i]; distance[i]=distance[j];distance[j]=distance[i];
									int   it =index[i]; index[i]=index[j];index[j]=index[i];
								}
							}

						}


						float total_dist=distance[0]+distance[1]+distance[2]+distance[3];
						
						vec.Set(	vertices_vec[0].x()*distance[3]/total_dist+
									vertices_vec[1].x()*distance[2]/total_dist+
									vertices_vec[2].x()*distance[1]/total_dist+
									vertices_vec[3].x()*distance[0]/total_dist,
									
									vertices_vec[0].y()*distance[3]/total_dist+
									vertices_vec[1].y()*distance[2]/total_dist+
									vertices_vec[2].y()*distance[1]/total_dist+
									vertices_vec[3].y()*distance[0]/total_dist, 
									
									vertices_vec[0].z()*distance[3]/total_dist+
									vertices_vec[1].z()*distance[2]/total_dist+
									vertices_vec[2].z()*distance[1]/total_dist+
									vertices_vec[3].z()*distance[0]/total_dist);


					}

					new_vectors[idx*3+0]=vec.x();
					new_vectors[idx*3+1]=vec.y();
					new_vectors[idx*3+2]=vec.z();
				}
			}

		}


		dumpReconstruedField("r.vec", new_vectors, grid_res);
	//	


		delete [] ver_num;
		delete [] ver;
		delete [] vectors;
		delete [] new_vectors;
		return ;

	}
#endif	// DEL-BY-LEETEN 01/22/2010-END

void combine_data()
{
	int xdim,ydim,zdim,grid_res[3];
	xdim=187; ydim=93; zdim=25;
	grid_res[0]=xdim; grid_res[1]=ydim; grid_res[2]=zdim;

	char szfilename[255];
	float* uf=new float[xdim*ydim*zdim];
	float* vf=new float[xdim*ydim*zdim];
	float* wf=new float[xdim*ydim*zdim];
	float* uvwf=new float[xdim*ydim*zdim*3];

	FILE* fp;
	for(int i=1;i<=227;i++)
	{
		//read uvw
		memset(szfilename,0,255);
//		sprintf(szfilename,"Isabel\\downsampled3\\UF%d.BIN",i);
		sprintf(szfilename,"viscontest2006\\downsampled_4times\\x_%.3d.BIN",i);
		fp=fopen(szfilename,"rb");
		fread(uf,sizeof(float),xdim*ydim*zdim,fp);
		fclose(fp);
		memset(szfilename,0,255);
//		sprintf(szfilename,"Isabel\\downsampled3\\VF%d.BIN",i);
		sprintf(szfilename,"viscontest2006\\downsampled_4times\\y_%.3d.BIN",i);	
		fp=fopen(szfilename,"rb");
		fread(vf,sizeof(float),xdim*ydim*zdim,fp);
		fclose(fp);
		memset(szfilename,0,255);
//		sprintf(szfilename,"Isabel\\downsampled3\\WF%d.BIN",i);
		sprintf(szfilename,"viscontest2006\\downsampled_4times\\z_%.3d.BIN",i);
		fp=fopen(szfilename,"rb");
		fread(wf,sizeof(float),xdim*ydim*zdim,fp);
		fclose(fp);

		for(int i=0;i<xdim*ydim*zdim;i++)
		{	
			uvwf[3*i+0]=uf[i];
			uvwf[3*i+1]=vf[i];
			uvwf[3*i+2]=wf[i];
		}
		memset(szfilename,0,255);
//		sprintf(szfilename,"Isabel\\downsampled3\\UVWF%d.vec",i);
		sprintf(szfilename,"viscontest2006\\downsampled_4times\\xyz_%.3d.vec",i);	
		fp=fopen(szfilename,"wb");
		fwrite(grid_res,sizeof(int),3,fp);
		fwrite(uvwf,sizeof(float),xdim*ydim*zdim*3,fp);
		fclose(fp);
		printf("combining %d/%d\n",i,227);
	}

	delete [] uf;
	delete [] vf;
	delete [] wf;
	delete [] uvwf;
	printf("combine done\n");
}
void convert_vis_data(char* filename)
{
	int grid_res[3];
	grid_res[0]=150; grid_res[1]=62; grid_res[2]=62;
	float* vec=new float[grid_res[0]*grid_res[1]*grid_res[2]*3];
	FILE* fp=fopen(filename,"rb");
	fread(vec,sizeof(float),grid_res[0]*grid_res[1]*grid_res[2]*3,fp);
	fclose(fp);

	fp=fopen(strcat(filename,".vec"),"wb");
	fwrite(grid_res,sizeof(int),3,fp);
	fwrite(vec,sizeof(float),grid_res[0]*grid_res[1]*grid_res[2]*3,fp);
	fclose(fp);
	delete [] vec;
}
int main(int argc, char** argv) 
{
//combine_data();

	/*
	//2d
	for(int i=0;i<5;i++)
	{
		if(i%2==0)
		{
			for(int j=0;j<=i;j++)
				printf("(%d, %d)",i,j);
			for(int j=i-1;j>=0;j--)
				printf("(%d, %d)",j,i);
		}
		else
		{
			for(int j=0;j<=i;j++)
				printf("(%d, %d)",j,i);
			for(int j=i-1;j>=0;j--)
				printf("(%d, %d)",i,j);
			
		}

	}
	*/

	g_filename=argv[1];


//	testTriangulation();
//downsampling();

//	float error=0.005;
//printf("value=%f\n",-error*log2(error)-(1-error)*log2(1-error)+error*log2(359));
//getchar();

readPatches_region();


	VECTOR3 minB, maxB; 

	osuflow = new OSUFlow(); 
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

	// ADD-BY-LEETEN 02/04/2010-BEGIN
	#if	DIFFUSION_ON_GLSL
	glewInit();
	#endif	// #if	DIFFUSION_ON_GLSL
	// ADD-BY-LEETEN 02/04/2010-END

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
void reconstruct_field_GVF_3D_tmp(float* new_vectors,float* vectors, int* grid_res,
								  std::vector<VECTOR3*> verts,
								  std::vector<int> vernum,int* donotchange)
{
	int iter=2*max(grid_res[0],grid_res[1]);

	//initial GVF, combine with input new_vectors
	list<vtListSeedTrace*>::iterator pIter;
	float* kx=new float[grid_res[0]*grid_res[1]];
	float* ky=new float[grid_res[0]*grid_res[1]];

	float* b=new float[grid_res[0]*grid_res[1]];
	float* c1=new float[grid_res[0]*grid_res[1]];
	float* c2=new float[grid_res[0]*grid_res[1]];

	for(int i=0; i<grid_res[0]*grid_res[1];i++)
	{
		kx[i]=ky[i]=b[i]=c1[i]=c2[i]=0;
	}
	for(int t=0;t<verts.size();t++)
	{
		VECTOR3* vert_tmp=verts[t];
		int num=vernum[t];
		for(int i=0;i<num;i++)
		{
			VECTOR3 p=vert_tmp[i];
			int idx=(int)p.x()+((int)p.y())*grid_res[0];
			float vx=vectors[idx*3+0];
			float vy=vectors[idx*3+1];
			//normalize
			float len=sqrt(vx*vx+vy*vy);
			if(len>0)
			{
				vx=vx/len;	vy=vy/len;
			}
			
			//calc kx and ky
			kx[idx]=vx;
			ky[idx]=vy;
			//initial GVF  to kx ky
			new_vectors[idx*3+0]=kx[idx];
			new_vectors[idx*3+1]=ky[idx];
			donotchange[idx]=1;
			
		}
	}
	//other initials
	for(int y=0; y<grid_res[1];y++)
	{
		for(int x=0; x<grid_res[0];x++)
		{
			int idx=x+y*grid_res[0];
			b[idx]=kx[idx]*kx[idx]+ky[idx]*ky[idx];
			c1[idx]=b[idx]*kx[idx];
			c2[idx]=b[idx]*ky[idx];
	

		}
	}
	
	//update the GVF
	float* tmp_new_vectors=new float[grid_res[0]*grid_res[1]*3];
	for(int i=0; i<grid_res[0]*grid_res[1]*3;i++)
		tmp_new_vectors[i]=new_vectors[i];

	//parameter setting:
	float mu=0.1;
	//iteration by iteration
	for(int i=0; i<iter; i++)
	{
		//start iterations
		//update the GVF
		for(int y=1; y<grid_res[1]-1;y++)
		{
			for(int x=1; x<grid_res[0]-1;x++)//cut boundary for now
			{
				int idx=x+y*grid_res[0]; 
				int idx_1=x+1+y*grid_res[0]; 
				int idx_2=x+(y+1)*grid_res[0]; 
				int idx_3=x-1+y*grid_res[0]; 
				int idx_4=x+(y-1)*grid_res[0]; 

				tmp_new_vectors[idx*3+0]=	(1-b[idx])*new_vectors[idx*3+0]+
											mu*(new_vectors[idx_1*3+0]+new_vectors[idx_2*3+0]+new_vectors[idx_3*3+0]+new_vectors[idx_4*3+0]-4*new_vectors[idx*3+0])+
											c1[idx];

				tmp_new_vectors[idx*3+1]=	(1-b[idx])*new_vectors[idx*3+1]+
											mu*(new_vectors[idx_1*3+1]+new_vectors[idx_2*3+1]+new_vectors[idx_3*3+1]+new_vectors[idx_4*3+1]-4*new_vectors[idx*3+1])+
											c2[idx];

			}
		}
		
		//update the GVF
		for(int y=0; y<grid_res[1];y++)
		{
			for(int x=0; x<grid_res[0];x++)
			{
				int idx=x+y*grid_res[0]; 
				//only update when the value improves
				float error_before, error_now;
				VECTOR3 before,now,input,tmp;
				before.Set(new_vectors[idx*3+0],new_vectors[idx*3+1],new_vectors[idx*3+2]);
				now.Set(tmp_new_vectors[idx*3+0],tmp_new_vectors[idx*3+1],tmp_new_vectors[idx*3+2]);
				input.Set(vectors[idx*3+0],vectors[idx*3+1],vectors[idx*3+2]);
				before.Normalize(); now.Normalize();
				tmp=before-input;
				error_before=tmp.GetMag();
				tmp=now-input;
				error_now=tmp.GetMag();
				//if((error_before>error_now))//&&(donotchange[idx]==0))
				{
				new_vectors[idx*3+0]=tmp_new_vectors[idx*3+0];
				new_vectors[idx*3+1]=tmp_new_vectors[idx*3+1];
				}

	
			}
		}
	}
	delete [] tmp_new_vectors;
	delete [] kx;
	delete [] ky;
	delete [] b;
	delete [] c1;
	delete [] c2;
}
*/
/*
void Streamline_entorpy_calculation_loadfile()
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


	//start checking
	int grid_res[3];
	float* vectors=get_grid_vec_data(grid_res);//get vec data at each grid point
	//grid_res[2]=1;

	//g_xdim=grid_res[0]; g_ydim=grid_res[1];
												//load seeds from file
	int* donotchange=new int[grid_res[0]*grid_res[1]*grid_res[2]];
	memset(donotchange,0,sizeof(int)*grid_res[0]*grid_res[1]*grid_res[2]);
	int nSeeds; 
	sl_list.clear(); 

	float* new_vectors=new float[grid_res[0]*grid_res[1]*grid_res[2]*3];
	memset(new_vectors,0,sizeof(float)*grid_res[0]*grid_res[1]*grid_res[2]*3);

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


	int selected_line_num=0;
	std::vector<VECTOR3> seedlist;
	// DEL-BY-LEETEN 01/22/2010 srand((unsigned)time(NULL));			// initialize random number generator
	VECTOR3 oldseed;

	int x_min=0,x_max=0,y_min=0,y_max=0,z_min=0,z_max=0;
	int* occupied=new int[grid_res[0]*grid_res[1]*grid_res[2]];
	memset(occupied,0,sizeof(int)*grid_res[0]*grid_res[1]*grid_res[2]);
	
	int* output_bins=new int[grid_res[0]*grid_res[1]*grid_res[2]];
	float error=0.005;//0.01;
	float target=-error*log2(error)-(1-error)*log2(1-error)+error*log2(360-1);


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

		reconstruct_field_GVF_3D_tmp(new_vectors,vectors,grid_res,line_vert_list,length_list,donotchange);
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
*/

/*

$Log: not supported by cvs2svn $
Revision 1.6  2010/03/10 20:29:13  leeten

[03/10/2010]
1. [MOD] Consider the voxels along the boundary when compute the entropy/cond. entropy.

Revision 1.5  2010/02/16 19:55:59  leeten

[02/16/2010]
1. [DEL] Do not initialize the vector along the boundary.

Revision 1.4  2010/02/09 00:50:08  leeten

[02/08/2010]
1. [ADD] Free the marginal histograms to reduce the memory requirement.
2. [MOD] Call combinehalflines() instead of combinehalflines_check_stop_entropy().
3. [ADD] Print the target entropy.

Revision 1.3  2010/02/05 01:35:25  leeten

[02/02/2010]
1. [MOD] Specify the samples by the preprocessor NR_OF_SAMPLES.
2. [MOD] Specify the pruning threshold via the preprocessor PRUNING_THRESHOLD.
3. [ADD] Call the function quit_selectStreamlines_by_distribution() at the end of the function selectStreamlines_by_distribution().
4. [MOD] Stop the generation of seeds when no seed can be generated.
5. [MOD] Output the generated streamlines to the path that is extended from the inpur vector's path.
6. [ADD] Call glewInit in the function main().

Revision 1.2  2010/02/02 23:51:29  leeten

[02/02/2010]
1. [ADD] Add the header liblog.h and FlowDiffusion3DConfig.h.
2. [MOD] Use preprocessor KERNEL_HALF_WIDTH, KERNEL_HALF_HEIGHT and KERNEL_HALF_DEPTH to control the kernel size.
3. [ADD] Define a function quit_selectStreamlines_by_distribution to free the memory used in the function selectStreamlines_by_distribution.
4. [MOD] All arrays used in the function selectStreamlines_by_distribution are done by MALLOC.
5. [ADD] If the preprocessor ENTROPY_ON_CUDA is not zero, use GPU to compute the entropy field.
6. [ADD] If the preprocessor BIN_LOOKUP_ON_CUDA is not zero, use GPU to convert the vectors into bins.
7. [DEL] Disable the dumpping of immediate result.
8. [DEL] Set the pointer donot_change to NULL.

Revision 1.1  2010/01/22 20:53:36  leeten

[01/22/2010]
1. [1ST] First time checkin.


*/
