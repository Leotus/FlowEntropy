#ifndef _ENTROPY__H
#define _ENTROPY__H
#include <stdlib.h>
#include <vector>
#include <math.h>
#include "OSUFlow.h"



class QuadNode
{
private:
	unsigned short XMin, XMax, YMin, YMax;
	//Is it a leave node?
	unsigned char LeaveNode;
	//All the children
	QuadNode *Children[4];
	int ChildrenNo;

public:
	void getLeaves(std::vector<VECTOR3>& leaves);

	//Empty constructor
	QuadNode()
	{
		XMin=XMax=YMin=YMax=0;
		LeaveNode=1;
		ChildrenNo=4;
		for (int i=0; i<ChildrenNo; i++)
			Children[i] = NULL;
	}

	//Another constructor
	QuadNode(int XMinValue, int XMaxValue, 
			  int YMinValue, int YMaxValue)
	{
		XMin = XMinValue;
		XMax = XMaxValue;
		YMin = YMinValue;
		YMax = YMaxValue;
		
		LeaveNode = 1;
		ChildrenNo=4;

		for (int i = 0; i < ChildrenNo; i++)
			Children[i] = NULL;
	}
	//Destructor
	~QuadNode()
	{
		for (int i = 0; i < ChildrenNo; i++)
			if (Children[i] != NULL)
				delete Children[i];		

	}


	//Get the dimension span of the octree node
	void GetDim(int &XMinValue, int &XMaxValue, 
				int &YMinValue, int &YMaxValue)
	{
		XMinValue = XMin;
		XMaxValue = XMax;
		YMinValue = YMin;
		YMaxValue = YMax;
	}

	//Assign the children of the octree node
	void Build(int xdim, int ydim, int MinLen);
	void drawself();
	void CalcEntropy(float* vectors,float* new_vectors, int* grid_res);
	void FindMaxEntropyNode(int& min_x, int& max_x, int& min_y, int& max_y);
	void CalcEntropy_FindMaxEntropyNode(float* vectors,float* new_vectors, int* grid_res, int& min_x, int& max_x, int& min_y, int& max_y,int* occupied,float hx);

	float m_entropy;
};

//////////////////////////////////////////////////////////////////////
//The class for BONO tree
//////////////////////////////////////////////////////////////////////
class QuadTree
{
private:
	unsigned short CellXDim, CellYDim;
	QuadNode *	Root;
public:
	int			m_minlen;
	void drawSelf();

	//Empty constructor
	QuadTree()
	{
		CellXDim = CellYDim =0;
		Root = NULL;
	}
	QuadTree(int XDimValue, int YDimValue)
	{
		CellXDim = XDimValue-1;
		CellYDim = YDimValue-1;
		Root = NULL;
	}

	//Destructor
	~QuadTree()
	{
		if (Root != NULL)
			delete Root;
	}

	//Get the dimension of the Bono tree
	void GetDim(int &XDimValue, int &YDimValue)
	{
		XDimValue = CellXDim+1;
		YDimValue = CellYDim+1;
	}
	//Build the Bono tree
	void Build(int MinLen);
	void CalcEntropy(float* vectors,float* new_vectors, int* grid_res)
	{
		Root->CalcEntropy(vectors, new_vectors, grid_res);
	}
	//Get the root of the Bono tree
	QuadNode *GetRoot()
	{
		return Root;
	}
	void FindMaxEntropyNode(int& min_x, int& max_x, int& min_y, int& max_y)
	{
		Root->FindMaxEntropyNode(min_x, max_x, min_y, max_y);
	}
	void CalcEntropy_FindMaxEntropyNode(float* vectors,float* new_vectors, int* grid_res, int& min_x, int& max_x, int& min_y, int& max_y,int* occupied,float hx)
	{
		Root->CalcEntropy_FindMaxEntropyNode(vectors, new_vectors, grid_res,min_x, max_x, min_y, max_y,occupied,hx);
	}
	void getLeaves(std::vector<VECTOR3>& leaves)
	{
		Root->getLeaves(leaves);
	}
};



void getVectorAt(int x, int y, float& fx, float& fy, int * grid_res,float* vectors);
void locateCriticalPts(std::vector<float>& crtpts, int* grid_res, float* vectors);

void doTriangulation(list<vtListSeedTrace*> sl_list, vector<VECTOR3>& triangles,int* grid_res);
//void initialGraph(int* grid_res, float* vectors, unsigned char* marker, Graph& graph);
void write2Dot(char* filename, list<list<int>> graph);
void clearupStreamlines(list<vtListSeedTrace*> sl_list);
void calcProbabilities(float* scalar, int datanum, int binnum, float* p);
void save2PPM_1(char* filename, unsigned char* data, int xdim, int ydim);
//void get_non_critical_regions(list<vtListSeedTrace*> l_list, int XDim, int YDim,unsigned char * data);
//void get_bounding_lines_seeds(unsigned char* marker,int XDim, int YDim,VECTOR3& myseed1,VECTOR3& myseed2);

void save2PPM_3(char* filename, unsigned char* data, int xdim, int ydim);
void save2PPM(char* filename, unsigned char* data, int xdim, int ydim);
void save2PPM_3_channels(char* filename, unsigned char* data, int xdim, int ydim);

/////////////////////////////////////////////////////////////////
///time varying part
/////////////////////////////////////////////////////////////////

void dumpSeedandLines(VECTOR3* seeds, int nseeds,OSUFlow* flow, char* filename);//crtical points excluded
void reseeding(int XDim, int YDim,char* filename1, char* filename2, char* fileseed1, char* fileseed2);
VECTOR3* loadSeedfromFile(char* fileseed, int& nSeed);
void dumpSeeds(VECTOR3* seeds,int seednum,  OSUFlow* osuflow,char* filename);
void dumpSeeds(std::vector<VECTOR3> seeds, char* filename);
float pt2SegDist(VECTOR3 p0, VECTOR3 p1, VECTOR3 q, VECTOR3& pq);
float pt2LineDist(VECTOR3 q, vtListSeedTrace* trace,VECTOR3& p);

void dumpReconstruedField(char* filename, float* new_vector, int* grid_res);

/////////////////////////////////////////////////////////////////
//critical points
/////////////////////////////////////////////////////////////////
//void prepare_seedingcpts(int* grid_res, float* vectors, unsigned char* marker, Graph& graph);//refine regions
//void seedingCriticalpoints(float* vectors, unsigned char* marker,list<VECTOR3* >& seedlist, int* grid_res, Graph graph);
//void reconstruct_vector(float* new_vectors,float* vectors, int* grid_res,list<vtListSeedTrace*> l_list,int* footsteps,Graph graph);
//float calcRelativeEntropy5(  vector<VECTOR3*> pcloud,float* vectors,float* new_vectors, int* grid_res);
float calcRelativeEntropy6( float* vectors,float* new_vectors, int* grid_res, VECTOR3 startpt,VECTOR3 endpt,int* occupied);
float calcEntropy( float* vectors, int* grid_res, VECTOR3 startpt,VECTOR3 endpt);
bool getnewseed(VECTOR3* newseed,float* new_vectors,float* vectors,int* grid_res,float thres);
void createFootstep(list<vtListSeedTrace*> line, int* footsteps, int* grid_res );
void combinehalflines(list<vtListSeedTrace*> lines, list<vtListSeedTrace*>& long_lines,int* grid_res);

void Trimhalflines(list<vtListSeedTrace*> lines, int* occupied,int* grid_res);
void UpdateOccupied(list<vtListSeedTrace*> lines, int* occupied,int* grid_res, int radius=0);

//void createAdjacencyGraph(std::vector<VECTOR3>& triangles, AdjacencyGraph& graph, int * streamline_foot_print, int* grid_res,list<vtListSeedTrace*> sl_list);
void marktriangles(list<vtListSeedTrace*> line, int* footsteps, int* grid_res , int& next_id);

//void reconstruct_field(float* new_vectors,float* vectors, int* grid_res,list<vtListSeedTrace*> l_list,int* footsteps, int curid);

	
void reconstruct_field_GVF_2D_tmp(float* new_vectors,float* vectors, int* grid_res,
								  std::vector<VECTOR3*> selected_line_verts,std::vector<int> vernum,int* donotchange);
void reconstruct_field_GVF_2D(float* new_vectors,float* vectors, int* grid_res,list<vtListSeedTrace*> l_list,int* donotchange);
void reconstruct_field_GVF_3D(float* new_vectors,float* vectors, int* grid_res,list<vtListSeedTrace*> l_list);
void reconstruct_field_GVF_2D_gaussian(float* new_vectors,float* vectors, int* grid_res,list<vtListSeedTrace*> l_list,int* donotchange);
float log2(float ang);
float calcRelativeEntropy6_load_bins(int* bin_vector, int* bin_newvectors,int* grid_res, VECTOR3 startpt,VECTOR3 endpt);
void save2file(char* filename, unsigned char* data, int xdim, int ydim);
float calcRelativeEntropy6_load_histograms(int* bin_vector, int* bin_newvectors,int* grid_res, VECTOR3 startpt,VECTOR3 endpt,
										   int* histo_pxy,int *histo_py);
typedef struct GridPoint
{
	VECTOR3 posObj;				// position in object space
	VECTOR3 posImage;			// position in image space
	int streamlineId;			// in current run, the streamlineId that is valid (include valid points)
	int streamlineIndex;		// index into m_listTraces
	int pntIndex;				// index into its streamline
	bool validFlag;				// whether this point is still valid

	GridPoint(VECTOR3 p, VECTOR3 pImg, int id, int index, int pntIdx, bool flag)
	{ posObj = p; posImage = pImg; streamlineId = id; streamlineIndex = index; pntIndex = pntIdx; validFlag = flag; }
};

typedef vector<GridPoint> PointRef;
void PutPointInGrid(VECTOR3 p, int streamlineId, int index, int pntIdx,int* grid_res,PointRef* m_grid);
bool CheckValidness(VECTOR3 p, VECTOR3 pObj,PointRef* m_grid, float m_sepDist,float m_sepMinDist,int* grid_res);
// MOD-BY-LEETEN 03/19/2010-FROM:
	// void combinehalflines_check_stop(list<vtListSeedTrace*> lines, list<vtListSeedTrace*>& long_lines,int* grid_res,PointRef* m_grid,int streamlineId);
// TO:
void 
combinehalflines_check_stop(
	list<vtListSeedTrace*> lines, 
	list<vtListSeedTrace*>& long_lines,
	int* grid_res,
	PointRef* m_grid,
	int streamlineId,
	// ADD-BY-LEETEN 03/26/2010-BEGIN
	float fMaxEntropy,	// the max. normalized entropy 
	// ADD-BY-LEETEN 03/26/2010-END
	float img_entropies[]);
// MOD-BY-LEETEN 03/19/2010-END
void combinehalflines_check_stop_entropy(list<vtListSeedTrace*> lines, list<vtListSeedTrace*>& long_lines,int* grid_res,
										 float* entropies);
bool discardredundantstreamlines(float& cur_entropy,float eplison,list<vtListSeedTrace*> new_lines,
								 float* vectors, float* new_vectors,int* grid_res);
		

#endif

/*

$Log: not supported by cvs2svn $
Revision 1.2  2010/03/19 19:42:19  leeten

[03/19/2010]
1. [MOD] Pass the normalized entropy field as one parameter to the function combinehalflines_check_stop().

Revision 1.1  2010/01/22 21:09:12  leeten

[01/22/2010]
1. [1ST] First time checkin.


*/
