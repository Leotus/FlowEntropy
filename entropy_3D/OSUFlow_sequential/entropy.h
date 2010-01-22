#ifndef _ENTROPY__H
#define _ENTROPY__H
#include <stdlib.h>
#include <vector>
#include <math.h>
#include "OSUFlow.h"

class OCNode
{
private:
	unsigned short XMin, XMax, YMin, YMax, ZMin, ZMax;
	//Is it a leave node?
	unsigned char LeaveNode;
	//All the children
	OCNode *Children[8];
	int ChildrenNo;

public:

	//Empty constructor
	OCNode()
	{
		XMin=XMax=YMin=YMax=ZMin=ZMax=0;
		LeaveNode=1;
		ChildrenNo=8;
		for (int i=0; i<ChildrenNo; i++)
			Children[i] = NULL;
	}

	//Another constructor
	OCNode(int XMinValue, int XMaxValue, 
			  int YMinValue, int YMaxValue,
			  int ZMinValue, int ZMaxValue
			  )
	{
		XMin = XMinValue;
		XMax = XMaxValue;
		YMin = YMinValue;
		YMax = YMaxValue;
		ZMin = ZMinValue;
		ZMax = ZMaxValue;
		
		LeaveNode = 1;
		ChildrenNo=8;

		for (int i = 0; i < ChildrenNo; i++)
			Children[i] = NULL;
	}
	//Destructor
	~OCNode()
	{
		for (int i = 0; i < ChildrenNo; i++)
			if (Children[i] != NULL)
				delete Children[i];		

	}


	//Get the dimension span of the octree node
	void GetDim(int &XMinValue, int &XMaxValue, 
				int &YMinValue, int &YMaxValue,
				int &ZMinValue, int &ZMaxValue
				)
	{
		XMinValue = XMin;
		XMaxValue = XMax;
		YMinValue = YMin;
		YMaxValue = YMax;
		ZMinValue = ZMin;
		ZMaxValue = ZMax;
	}

	//Assign the children of the octree node
	void Build(int xdim, int ydim, int zdim, int MinLen);
	//void drawself();
	//void CalcEntropy(float* vectors,float* new_vectors, int* grid_res);
	void FindMaxEntropyNode(int& min_x, int& max_x, int& min_y, int& max_y, int& min_z, int& max_z );
	void CalcEntropy_FindMaxEntropyNode(float* vectors,float* new_vectors, int* grid_res, 
										int& min_x, int& max_x, int& min_y, int& max_y, int& min_z, int& max_z,
										float* theta, float* phi,
										int* oldbin, int* newbin,int* occupied);
	void CalcEntropy_LeafNode(float* vectors,float* new_vectors, int* grid_res, 
										int& min_x, int& max_x, int& min_y, int& max_y, int& min_z, int& max_z,
										float* theta, float* phi,
										int* oldbin, int* newbin,int* occupied);
	float CalcEntropy_InnerNode();
	void get_nodes_bounds(std::vector<VECTOR3>& lower_bound,std::vector<VECTOR3>& higher_bound);

	void calcEntropy( float* result, float* vectors,int* grid_res, float* theta,float* phi,int binnum);
	void getLeaves(std::vector<VECTOR3>& leaves);

	float m_entropy;
};

//////////////////////////////////////////////////////////////////////
//The class for BONO tree
//////////////////////////////////////////////////////////////////////
class OCTree
{
private:
	unsigned short CellXDim, CellYDim, CellZDim;
	OCNode *	Root;
public:
	int			m_minlen;
	//void drawSelf();

	//Empty constructor
	OCTree()
	{
		CellXDim = CellYDim =CellZDim=0;
		Root = NULL;
	}
	OCTree(int XDimValue, int YDimValue, int ZDimValue)
	{
		CellXDim = XDimValue-1;
		CellYDim = YDimValue-1;
		CellZDim = ZDimValue-1;
		Root = NULL;
	}

	//Destructor
	~OCTree()
	{
		if (Root != NULL)
			delete Root;
	}

	//Get the dimension of the Bono tree
	void GetDim(int &XDimValue, int &YDimValue, int &ZDimValue)
	{
		XDimValue = CellXDim+1;
		YDimValue = CellYDim+1;
		ZDimValue = CellZDim+1;
	}
	//Build the Bono tree
	void Build(int MinLen);
	
	//Get the root of the Bono tree
	OCNode *GetRoot()
	{
		return Root;
	}
	void FindMaxEntropyNode(int& min_x, int& max_x, int& min_y, int& max_y, int& min_z, int& max_z)
	{
		Root->FindMaxEntropyNode(min_x, max_x, min_y, max_y, min_z, max_z);
	}
	void get_nodes_bounds(std::vector<VECTOR3>& lower_bound,std::vector<VECTOR3>& higher_bound)
	{
		Root->get_nodes_bounds(lower_bound,higher_bound);
	}
	void CalcEntropy_LeafNode(float* vectors,float* new_vectors, int* grid_res, 
										int& min_x, int& max_x, int& min_y, int& max_y, int& min_z, int& max_z,
										float* theta, float* phi,
										int* oldbin, int* newbin,int* occupied)
	{
		Root->CalcEntropy_LeafNode(vectors, new_vectors, grid_res,min_x, max_x, min_y, max_y,min_z, max_z,
			theta, phi,oldbin, newbin,occupied);
	}
	void CalcEntropy_InnerNode()
	{
		Root->CalcEntropy_InnerNode();
	}

	void CalcEntropy_FindMaxEntropyNode(float* vectors,float* new_vectors, int* grid_res, 
										int& min_x, int& max_x, int& min_y, int& max_y, int& min_z, int& max_z,
										float* theta, float* phi,
										int* oldbin, int* newbin,int* occupied)
	{
		Root->CalcEntropy_FindMaxEntropyNode(vectors, new_vectors, grid_res,min_x, max_x, min_y, max_y,min_z, max_z,
			theta, phi,oldbin, newbin,occupied);
	}

	void calcEntropy( float* result, float* vectors,int* grid_res,float* theta,float* phi,int binnum)
	{
		Root->calcEntropy(result,vectors, grid_res,
			theta,phi,binnum);

	}
	void getLeaves(std::vector<VECTOR3>& leaves)
	{
		Root->getLeaves(leaves);
	}

};
void getVectorAt(int x, int y, float& fx, float& fy, int * grid_res,float* vectors);

void write2Dot(char* filename, list<list<int>> graph);
void clearupStreamlines(list<vtListSeedTrace*> sl_list);
void calcProbabilities(float* scalar, int datanum, int binnum, float* p);
void save2PPM_1(char* filename, unsigned char* data, int xdim, int ydim);

void save2PPM_3(char* filename, unsigned char* data, int xdim, int ydim);
void save2PPM(char* filename, unsigned char* data, int xdim, int ydim);

/////////////////////////////////////////////////////////////////
///time varying part
/////////////////////////////////////////////////////////////////

void dumpSeedandLines(VECTOR3* seeds, int nseeds,OSUFlow* flow, char* filename);//crtical points excluded
VECTOR3* loadSeedfromFile(char* fileseed, int& nSeed);
void dumpSeeds(vector<VECTOR3> seeds, char* filename);

float pt2SegDist(VECTOR3 p0, VECTOR3 p1, VECTOR3 q, VECTOR3& pq);
float pt2LineDist(VECTOR3 q, vtListSeedTrace* trace,VECTOR3& p);

void dumpImportanceField(char*  filename,float* importance, int* grid_res);
void dumpReconstruedField(char* filename, float* new_vector, int* grid_res);
void calcDistanceField(char* filename, list<vtListSeedTrace*> lines, int* grid_res,int focus_num,float* dist);
/////////////////////////////////////////////////////////////////
//critical points
/////////////////////////////////////////////////////////////////
float calcRelativeEntropy6( float* vectors,float* new_vectors, int* grid_res, VECTOR3 startpt,
						   VECTOR3 endpt,float* theta, float* phi,int* oldbin, int* newbin,int* occupied,int binnum,
						   int * histo_puv=0,int * histo_pv=0,float* pv=0,float* entropy_tmp=0,
						   int*g_histo=0,
						   int* g_histo_uv=0);
float calcRelativeEntropy6_new( float* vectors,float* new_vectors, int* grid_res, VECTOR3 startpt,
						   VECTOR3 endpt,float* theta, float* phi,int* oldbin, int* newbin,int* occupied,int binnum,
						   int * histo_puv=0,int * histo_pv=0,float* pv=0,float* entropy_tmp=0,
						   int*g_histo=0,
						   int* g_histo_uv=0);
float calcRelativeEntropy6_by_known_histograms( float* vectors,float* new_vectors, int* grid_res, VECTOR3 startpt,
						   VECTOR3 endpt,float* theta, float* phi,int* oldbin, int* newbin,int* occupied,int binnum,
						   int * histo_puv=0,int * histo_pv=0,float* pv=0,float* entropy_tmp=0,
						   int*g_histo=0,
						   int* g_histo_uv=0);

//float calcPoint2PointError( float* vectors,float* new_vectors, int* grid_res, VECTOR3 startpt,VECTOR3 endpt);
//void createFootstep(list<vtListSeedTrace*> line, int* footsteps, int* grid_res );
void combinehalflines(list<vtListSeedTrace*> lines, list<vtListSeedTrace*>& long_lines);
void marktriangles(list<vtListSeedTrace*> line, int* footsteps, int* grid_res , int& next_id);

void doTriangulation3D(list<vtListSeedTrace*> sl_list,std::vector<VECTOR3>& tetrahedras,int* grid_res);
void reconstruct_field_GVF_3D(float* new_vectors,float* vectors, int* grid_res,list<vtListSeedTrace*> l_list,int* donot_change,
							   float* kx, float* ky, float* kz,
							  float* b, float* c1, float* c2, float* c3,
							  float* importance=NULL);
void diffuse_importance_value(float* importance,int* grid_res,list<vtListSeedTrace*> new_lines);

int get_bin_number_3D(VECTOR3 v, float* theta, float* phi,int binnum);
void combinehalflines_check_stop_entropy(list<vtListSeedTrace*> lines, list<vtListSeedTrace*>& long_lines,int* grid_res,
						float* entropies);
void combinehalflines_check_stop_entropy(list<vtListSeedTrace*> lines, list<vtListSeedTrace*>& long_lines,int* grid_res,
										 float* entropies);
//int get_bin_number_3D(float mytheta, float myphi, float* theta, float* phi);

float calcEntropy_known_bins( int* bins,int* grid_res, VECTOR3 startpt,VECTOR3 endpt,int binnum);
float calcEntropy1( float* vectors,int* grid_res, VECTOR3 startpt,VECTOR3 endpt,float* theta, float* phi,int binnum);
float log2(float v);
void UpdateOccupied(list<vtListSeedTrace*> lines, int* occupied,int* grid_res,float r);
float getImportance(list<vtListSeedTrace*> lines,int* grid_res,float* entropies);
void calc_entropy( int* bins,int* grid_res, int binnum,float* entropies);
void dumpEntropyField(char*  filename,float* importance, int* grid_res);
void adjustLengthByEntropies(char*  entropyfile,char* streamlinefile,char* outputstreamlinesfile);
VECTOR3* get_line_vertices(VECTOR3* vecs,int* ver_num, int line_no,int& length);
bool discardredundantstreamlines(float& cur_entropy,float eplison,list<vtListSeedTrace*> new_lines,
								 float* vectors, float* new_vectors,int* grid_res,int* bin_my_vector,int* bin_new_vector,
								 int binnum,float* theta, float* phi,int* histo_puv, int* histo_pv, float* pv,float*  entropy_tmp);

	
#endif

 /*

$Log: not supported by cvs2svn $

*/
