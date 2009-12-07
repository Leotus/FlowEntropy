//	File:		OSUFlow.h
//
//	Author:		Liya Li
//
//	Date:		Sept 2005
//
//	Description:	Definition of OSUFlow class. It contains the interface
//					between gui and underlying flow functions.
//
#ifndef _OSU_FLOW_H_
#define _OSU_FLOW_H_

#include "header.h"
#include "VectorMatrix.h"
#include "Field.h"
#include "Rake.h"
#include "FieldLine.h"

class CurvilinearGrid;
class OSUFlow
{
public:
	// constructor and destructor
	OSUFlow();
	~OSUFlow();
	void SetEntropySeedPoints(VECTOR3 * ptr, int num);

	void SetRandomSeedPoints(const float min[3], const float max[3], int numSeeds);
	void SetRegularSeedPoints(const float min[3], const float max[3], const size_t numSeeds[3]);
	void SetIntegrationParams(float initStepSize, float maxStepSize);
	void LoadData(const char* fname, bool bStatic);
	void LoadData(const char* fname, bool bStatic, VECTOR3 pMin, 
		      VECTOR3 pMax); 
	void Boundary(VECTOR3& minB, VECTOR3& maxB) { flowField->Boundary(minB, maxB); };
	void SetBoundary(VECTOR3 minB, VECTOR3 maxB) {flowField->SetBoundary(minB, maxB);}; 
	void InitStaticFlowField(void);
	void InitStaticFlowField(VECTOR3 minb, VECTOR3 maxB); 
	void InitStaticFlowField(float*, VECTOR3 minb, VECTOR3 maxB);
	void InitStaticFlowField(Solution*, CurvilinearGrid*,VECTOR3 minb, VECTOR3 maxB);
	
	void InitTimeVaryingFlowField(void);
	CVectorField* GetFlowField(void) { return flowField; }
	bool GenStreamLines(list<vtListSeedTrace*>&, TRACE_DIR, int, unsigned int);
	bool GenStreamLines(VECTOR3*, TRACE_DIR,const int, const int, list<vtListSeedTrace*>&);
	bool GenPathLines(list<vtListSeedTrace*>& listSeedTraces, int maxPoints,unsigned int randomSeed);
	VECTOR3 *GetSeeds(int& num) {num = numSeeds[0]*numSeeds[1]*numSeeds[2]; 
	                             return seedPtr;}

protected:
	void Reset(void);

private:
	float minRakeExt[3];					// minimal rake range 
	float maxRakeExt[3];					// maximal rake range
	unsigned int numSeeds[3];				// number of seeds
	int nSeeds; 
	VECTOR3 *seedPtr; 
	bool bUseRandomSeeds;					// whether use randomly or regularly generated seeds
	CVectorField* flowField;				// flow field data
	char* flowName; 					// name of file including information about field
	float initialStepSize;					// for integration
	float maxStepSize;
	VECTOR3 gMin, gMax; // global min/max range 
	VECTOR3 lMin, lMax; // local min/max range
	bool bStaticFlow;					// static flow
};

#endif
