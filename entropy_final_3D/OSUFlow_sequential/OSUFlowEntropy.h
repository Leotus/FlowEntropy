#pragma once
#include <OSUFlow.h>

class OSUFlowEntropy:
    public OSUFlow
{
public:
    void SetEntropySeedPoints(VECTOR3* ptr, int num)
    {
        if (seedPtr!=NULL)  {
                delete [] seedPtr;
            }
        seedPtr=new VECTOR3 [num];
        for(int i=0; i<num; i++)
        {
            seedPtr[i].Set(ptr[i].x(),ptr[i].y(),ptr[i].z());
        }

        nSeeds=num;
        this->numSeeds[0] = num;
        this->numSeeds[1] = 1;
        this->numSeeds[2] = 1;
    }
};