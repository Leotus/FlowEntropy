
#include "entropy.h"

// ADD-BY-LEETEN 02/05/2010-BEGIN
#include "FlowDiffusion2DConfig.h"	
// ADD-BY-LEETEN 02/05/2010-END

#if	0	// DEL-BY-LEETEN 01/22/2010-BEGIN
	#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
	#include <CGAL/Triangulation_2.h>
	#include <CGAL/Regular_triangulation_euclidean_traits_2.h>
	#include <CGAL/Regular_triangulation_filtered_traits_2.h>
	#include <CGAL/Regular_triangulation_2.h>
	#include <CGAL/Delaunay_triangulation_2.h>

	struct K : CGAL::Exact_predicates_inexact_constructions_kernel {};

	typedef CGAL::Delaunay_triangulation_2<K>  Triangulation;
	typedef Triangulation::Face_iterator  Face_iterator;
	typedef Triangulation::Point          Point;
	typedef Triangulation::Face_handle          Face_handle;

	Triangulation T;




	float dot(Point p1, Point p2)
	{
		return (p1.x()*p2.x()+p1.y()*p2.y());
	}
	bool inTriangle(VECTOR3 A,VECTOR3 B,VECTOR3 C,VECTOR3 P)
	{
		// Compute vectors        
		Point v0=Point(C.x()-A.x(), C.y()-A.y());
		Point v1=Point(B.x()-A.x(), B.y()-A.y());
		Point v2=Point(P.x()-A.x(), P.y()-A.y());

		// Compute dot products
		float dot00 = dot(v0, v0);
		float dot01 = dot(v0, v1);
		float dot02 = dot(v0, v2);
		float dot11 = dot(v1, v1);
		float dot12 = dot(v1, v2);

		// Compute barycentric coordinates

		float invDenom = 1 / (dot00 * dot11 - dot01 * dot01);

		if(!invDenom)
			return false;

		float u = (dot11 * dot02 - dot01 * dot12) * invDenom;
		float v = (dot00 * dot12 - dot01 * dot02) * invDenom;

		// Check if point is in triangle
		return (u >= 0) && (v >= 0) && (u + v <= 1);
	}
#endif	// DEL-BY-LEETEN 01/22/2010-END

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

void locateCriticalPts(std::vector<float>& crtpts, int* grid_res, float* vectors )
{


	int x,y;
	float fx[4], fy[4];

	crtpts.clear();


	for(y=2; y<grid_res[1]-2; y++)
	{
		for(x=2; x<grid_res[0]-2; x++)
		{
			getVectorAt(x-1,y-1,fx[0], fy[0],grid_res, vectors); 
			getVectorAt(x-1,y,fx[1], fy[1],grid_res, vectors); 
			getVectorAt(x,y-1,fx[3], fy[3],grid_res, vectors); 
			getVectorAt(x,y,fx[2], fy[2],grid_res, vectors); 


			//change of sign in at least one direction?

			int xposnum,yposnum;
			xposnum=yposnum=0;

			bool haschange=true;
			for(int k=0; k<4; k++)
			{
				if(fx[k]>0)
					xposnum++;
				if(fy[k]>0)
					yposnum++;

			}

			if(xposnum==0 || xposnum==4)//is y change of sign for the pair
				haschange=false;

			if(yposnum==0 || yposnum==4)
				haschange=false;



			if(haschange==true)
			{


				//solve equation here
				float A,B,C,D,E,F,G,H;
				float u1,u2,u3,u4,v1,v2,v3,v4;

				u1=fx[0]; u2=fx[1]; u3=fx[2]; u4=fx[3];
				v1=fy[0]; v2=fy[1]; v3=fy[2]; v4=fy[3];
				A=u1;
				B=-u1+u4;
				C=u1-u2;
				D=-u1+u2-u3+u4;
				E=v1;
				F=-v1+v4;
				G=-v1+v2;
				H=v1-v2+v3-v4;

				float a,b,c;
				a=F*D+B*H;
				b=E*D+F*C+G*B+A*H;
				c=E*C+G*A;

				float vv0,vv1;
				float tmp=(b*b-4*a*c);//odd, should be the other way, check derivation 
				if(tmp>=0)//has solution
				{

					//getVectorAt(x,y,vv0,vv1,grid_res, vectors);

					for(int kkk=0; kkk<2;kkk++)
					{
						float cof=1.0;
						if(kkk>0)
							cof=-1.0;
						//select the solution (0~1)
						float xsol=(-b+cof*sqrtf((tmp)))/(2*a);
						float ysol=(A+B*xsol)/(C+D*xsol);

						if( (xsol>=0) && (xsol<=1) &&
							(ysol>=0) && (ysol<=1) )
						{
							crtpts.push_back(x-1+xsol);
							crtpts.push_back(y-1+ysol);

						}
						if(tmp==0)
							break;//two identical roots
					}

				}

			}
		}
	}

}

#if	0	// DEL-BY-LEETEN 01/22/2010-BEGIN
	void doTriangulation(list<vtListSeedTrace*> sl_list,std::vector<VECTOR3>& triangles,int* grid_res)
	{

		std::vector<Point> myPts;
		std::list<vtListSeedTrace*>::iterator pIter; 

		pIter = sl_list.begin(); 
		for (; pIter!=sl_list.end(); pIter++) {
			vtListSeedTrace *trace = *pIter; 
			std::list<VECTOR3*>::iterator pnIter; 
			pnIter = trace->begin(); 
			for (; pnIter!= trace->end(); pnIter++) {
				VECTOR3 p = **pnIter; 
				//printf(" %f %f %f ", p[0], p[1], p[2]); 
				myPts.push_back(Point(p[0], p[1]));
			}
		}


		//push the boundary pts
		for(int x=0; x<grid_res[0]-1; x+=20)
		myPts.push_back(Point(x,0));
		myPts.push_back(Point(grid_res[0]-1,0));

		for(int x=0; x<grid_res[0]-1; x+=20)
		myPts.push_back(Point(x,grid_res[1]-1));
		myPts.push_back(Point(grid_res[0]-1,grid_res[1]-1));

		for(int y=0; y<grid_res[1]-1; y+=20)
		myPts.push_back(Point(0,y));
		myPts.push_back(Point(0,grid_res[1]-1));

		for(int y=0; y<grid_res[1]-1; y+=20)
		myPts.push_back(Point(grid_res[0]-1,y));
		myPts.push_back(Point(grid_res[0]-1,grid_res[1]-1));
		
		T.insert(myPts.begin(), myPts.end());

		Face_iterator f =(T.faces_begin());
		Point p[4],q[4];


		for ( ; f !=T.faces_end(); ++f) 
		{
			triangles.push_back(VECTOR3(f->vertex(0)->point().x(),f->vertex(0)->point().y(),0));
			triangles.push_back(VECTOR3(f->vertex(1)->point().x(),f->vertex(1)->point().y(),0));
			triangles.push_back(VECTOR3(f->vertex(2)->point().x(),f->vertex(2)->point().y(),0));
		}
	}
	void dumpSeeds(std::vector<VECTOR3> seeds, char* filename)
	{
		int nSeeds=seeds.size(); 

		//VECTOR3* printseeds = osuflow->GetSeeds(nSeeds); 


		FILE* fp=fopen(filename,"w");
		if(fp==0)
		{
			printf("seed file not found\n");
			return ;//return;
		}
		fprintf(fp,"%d \n",nSeeds);
		for (int i=0; i<nSeeds; i++) 
		{
			fprintf(fp,"%f ", seeds[i][0]);
			fprintf(fp,"%f ", seeds[i][1]);
			fprintf(fp,"%f ", seeds[i][2]);
			fprintf(fp,"\n");
		}
		fclose(fp);


	}
	/*
	void createAdjacencyGraph(std::vector<VECTOR3>& triangles, AdjacencyGraph& graph, int * streamline_foot_print, int* grid_res,list<vtListSeedTrace*> sl_list)
	{

		vector<int*> node_ids;
															//creation of nodes
		for(int i=0; i<triangles.size()/3;i++)
		{
			int id[3],v[3];
			VECTOR3 p[3],q;
			for(int j=0; j<3;j++)							//get the streamlines ids on the three vertices
			{
				p[j].Set(triangles[i*3+j].x(),triangles[i*3+j].y(),0);
				id[j]=(int)p[j].x()+(int)p[j].y()*grid_res[0];
				v[j]=streamline_foot_print[id[j]];
			}

			//ignore boundary triangles
			int off=5;
			if(p[0].x()<off || p[0].x()>(grid_res[0]-off)||p[0].y()<off ||p[0].y()>(grid_res[1]-off) ||
				p[1].x()<off || p[1].x()>(grid_res[0]-off)||p[1].y()<off ||p[1].y()>(grid_res[1]-off) ||
				p[2].x()<off || p[2].x()>(grid_res[0]-off)||p[2].y()<off ||p[2].y()>(grid_res[1]-off) )
				continue;

															//sort v[j] for convinient of comparison
			for(int w=0; w<3; w++)
			{
				for(int u=0; u<3; u++)
				{
					if(v[w]<v[u])
					{
						int tmp=v[w]; v[w]=v[u]; v[u]=tmp;
					}
				}
			}												
															//start calculating membership of each triangles
															//calc boundingbox
			float lowx,lowy,highx,highy;
			lowx=grid_res[0]; lowy=grid_res[1]; highx=0; highy=0;
			for(int j=0; j<3; j++)		
			{	
				q.Set(p[j].x(),p[j].y(),p[j].z());
				if(q.x()<lowx)		lowx=q.x();
				if(q.x()>highx)		highx=q.x();
				if(q.y()<lowy)		lowy=q.y();
				if(q.y()>highy)		highy=q.y();
			}
			//igore boundary triangles, otherwise wrong graph and wrong graph, crop the bound
			int crop=0;
			if(lowx<crop || highx>grid_res[0]-crop ||
				lowy<crop || highy>grid_res[1]-crop )
				continue;
			std::vector<VECTOR3> tmp_vector;
			tmp_vector.clear();

														//get the points belong to the triangle
			for(int y=lowy; y<=highy;y++)
			{
				for(int x=lowx; x<=highx;x++)
				{
					if(true==inTriangle(p[0],p[1],p[2],VECTOR3(x,y,0)))
					{
						tmp_vector.push_back(VECTOR3(x,y,0));
					}
				}
			}

			bool newnode=true; int nodeid;
															//if the node created,return the nodeid and add to it,otherwise create a new node
			for(int n=0; n<node_ids.size();n++)
			{
				int * ids=node_ids[n];
				if( (ids[0]==v[0]) && (ids[2]==v[2]))// && (ids[2]==v[2]))//max and min equal
				{
					newnode=false;
					nodeid=n;
					break;//already exist;
				}
			}
			if(newnode==true)
			{
				//add big sized node only
				int* newid=new int[3];
				newid[0]=v[0];newid[1]=v[1];newid[2]=v[2];
				node_ids.push_back(newid);
			}
			VECTOR3* pcloud=new VECTOR3[tmp_vector.size()];	//if the node exists, add to its point_cloud, otherwise create a new node
			for( int tv=0 ; tv<tmp_vector.size() ;tv++)
			{
				pcloud[tv]=tmp_vector[tv];
			}
			Node* pnode;
			if(newnode==true)
			{
				pnode=new Node();
				pnode->setPointCloud(pcloud,tmp_vector.size());
				graph.addnodes(pnode);

				//set bounds for the node
				for(int vv=0;vv<3; vv++)
				{
					
					int id=v[vv];
					if(id<0 || id>sl_list.size()-1)
						continue;//foot mark of boundaries
					list<vtListSeedTrace*>::iterator myiter;
					myiter=sl_list.begin();
					int c=0;
					for(;myiter!=sl_list.end(); myiter++)
					{
						if(c==id)
							break;
						c++;
					}
					pnode->m_bounds.remove(*myiter);
					pnode->m_bounds.push_back(*myiter);

				}

			}
			else
			{
				graph.m_node_list[nodeid]->setPointCloud(pcloud,tmp_vector.size());
			}
		}
															//debug of the point_clouds for each node
		unsigned char* data=new unsigned char[grid_res[0]*grid_res[1]*3];
		memset(data,0,sizeof(unsigned char)*grid_res[0]*grid_res[1]*3);
		float c[]={	1,0,0,
				0,1,0,
				0,0,1,
				1,1,0,
				1,0,1,
				0,1,1,
				0.5,0.5,0,
				0.5,0.5,0.5,
				0,0.5,0.5,
				0.5,0,0.5,
				0.5,1,0.5,
				0.5,0.5,1,
				1,0.5,0.5,
				1,1,0.5,
				1,0.5,1,
				0.5,1,1
		
		};//color to use

		for(int d=0; d<graph.m_node_list.size(); d++)
		{
			Node* pnode=graph.m_node_list[d];
			int num;
			VECTOR3* pcloud=pnode->getPointCloud(num);
			for(int n=0; n<num;n++)
			{
				int x=pcloud[n].x();
				int y=pcloud[n].y();

				int idx=x+y*grid_res[0];
				data[idx*3+0]=c[(d%16)*3+0]*255;
				data[idx*3+1]=c[(d%16)*3+1]*255;
				data[idx*3+2]=c[(d%16)*3+2]*255;
			}

		}
		save2PPM_3("point_cloud.ppm", data, grid_res[0],grid_res[1]);
		delete [] data;




		//creation of edges
		for(int i=0; i<triangles.size()/3;i++)
		{
			VECTOR3 p0,p1,p2;
			p0.Set(triangles[i*3+0].x(),triangles[i*3+0].y(),0);
			p1.Set(triangles[i*3+1].x(),triangles[i*3+1].y(),0);
			p2.Set(triangles[i*3+2].x(),triangles[i*3+2].y(),0);

			int id[3];
			id[0]=(int)p0.x()+(int)p0.y()*grid_res[0];
			id[1]=(int)p1.x()+(int)p1.y()*grid_res[0];
			id[2]=(int)p2.x()+(int)p2.y()*grid_res[0];

			int v[3];
			v[0]=streamline_foot_print[id[0]];
			v[1]=streamline_foot_print[id[1]];
			v[2]=streamline_foot_print[id[2]];


			if(v[0]<0 || v[1]<0 || v[2]<0 )
				continue;//sth is wrong
			for(int i=0; i<3; i++)
			{
				for(int j=0; j<3; j++)
				{
	//				if(v[i]>=v[j] && v[i]<g_lines.size())
	//					continue;
					graph.addEdge(v[i],v[j]);
				}
			}

		}

	}

	*/
#endif	// DEL-BY-LEETEN 01/22/2010-END

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
			r=data[idx]*255;
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

	for (int  i = 0;i<ydim;i++)
	{
		for (int j = 0; j < xdim; j++) 
		{
			int idx = i * xdim + j;

			int r,g,b; 
			r=data[idx];
			g=data[idx];
			b=data[idx];
			fprintf(fp, " %d %d %d", r,g,b); 
		}
	}


	fclose(fp);
}
void save2file(char* filename, unsigned char* data, int xdim, int ydim)
{
	FILE* fp = fopen(filename, "wb");
	if (fp == NULL)
		return;
	int crop=0;//135;

	fwrite(&xdim,sizeof(int),1,fp);
	fwrite(&ydim,sizeof(int),1,fp);
	fwrite(data,sizeof(unsigned char),xdim*ydim,fp);
	fclose(fp);
}
void save2PPM_3_channels(char* filename, unsigned char* data, int xdim, int ydim)
{
	FILE* fp = fopen(filename, "w");
	if (fp == NULL)
		return;
	int crop=0;//135;

	fprintf(fp, "P3\n%d %d\n255\n", xdim, ydim-crop*2);

	for (int  i = 0;i<ydim;i++)
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




//new, streamline->edge, region->nodeg

void write2Dot(char* filename, list<list<int>> graph)
{
	FILE* fp = fopen(filename, "w");
	if (fp == NULL)
		return;
	fprintf(fp, "digraph mygraph{\n");

	fprintf(fp, "[overlap=scale]\n");//for neato layout only

	int i;

	char edgefrom[20];
	char edgeto[20];

	list<list<int>>::iterator pIter; 

	pIter = graph.begin(); 
	int from=0;
	for (; pIter!=graph.end(); pIter++) {
		memset(edgefrom,0,20);
		sprintf(edgefrom,"%d",from);

		list<int> trace = *pIter; 
		list<int>::iterator pnIter; 
		pnIter = trace.begin(); 
		for (; pnIter!= trace.end(); pnIter++) {
			int to = *pnIter; 
			//printf(" %f %f %f ", p[0], p[1], p[2]); 
			sprintf(edgeto,"%d",to);
			if(to>0)
			{
				fprintf(fp, edgefrom);
				fprintf(fp, " -> ");
				fprintf(fp, edgeto);
			}
			else
			{
				fprintf(fp, edgeto);
				fprintf(fp, " -> ");
				fprintf(fp, edgefrom);
			}
			fprintf(fp, " edge [color=navyblue];\n");

		}
		from++;
	}


	/*			memset(edgeto,0,20);
	if(nlist[j+1]>=0)
	{
	sprintf(edgeto,"%d",nlist[j+1]);
	fprintf(fp, edgefrom);
	fprintf(fp, " -> ");
	fprintf(fp, edgeto);
	}
	else
	{
	sprintf(edgeto,"%d",-nlist[j+1]);
	fprintf(fp, edgeto);
	fprintf(fp, " -> ");
	fprintf(fp, edgefrom);

	}
	*/

	for (i = 0; i <graph.size()/2; i++)
	{
		fprintf(fp,"node %d [color=red, style=filled, fixedsize=true];\n", i);

	}


	fprintf(fp, "}\n");

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
			p.Set(pq.x(),pq.y(),0);
		}
		p0.Set(p1.x(),p1.y(),0);
		p0.Set(p1.x(),p1.y(),0);
	}
	/*
	p0.x=line[0]; p0.y=line[1];
	for(int i=1; i<line.size()/2;i++)
	{
	p1.x=line[i*2+0];
	p1.y=line[i*2+1];

	float dist=pt2SegDist(p0, p1, q);
	if(dist<minDist)
	{
	minDist=dist;
	p.x=p1.x-p0.x; p.y=p1.y-p0.y;
	}

	p0.x=p1.x;	p0.y=p1.y;
	}
	*/
	return minDist;
}
float pt2SegDist(VECTOR3 p0, VECTOR3 p1, VECTOR3 q, VECTOR3& pq)
{
	VECTOR3 vi;
	float t;

	vi.Set(p1.x()-p0.x(),p1.y()-p0.y(),0);
	float sqrLen=(vi.x()*vi.x()+vi.y()*vi.y());

	//	if(len>0)//normalize it
	//	{
	//		vi.x/=len;
	//		vi.y/=len;
	//	}
	VECTOR3 tmp;
	tmp.Set(q.x()-p0.x(),q.y()-p0.y(),0);

	t=(tmp.x()*vi.x()+tmp.y()*vi.y())/(sqrLen);
	if(t<0)
		t=0;
	if(t>1)
		t=1;
	//t=min(max(t,0),1);
	//pq=p0+t*vi
	pq.Set(p0.x()+t*vi.x(),p0.y()+t*vi.y(),0);

	float dist=sqrtf((pq.x()-q.x())*(pq.x()-q.x())+(pq.y()-q.y())*(pq.y()-q.y()));
	return dist;
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

void dumpSeeds(VECTOR3* seeds,int seednum,  OSUFlow* osuflow,char* filename)//crtical points excluded
{

	int nSeeds; 

	VECTOR3* printseeds = osuflow->GetSeeds(nSeeds); 


	FILE* fp=fopen(filename,"w");
	if(fp==0)
	{
		printf("seed file not found\n");
		return ;//return;
	}
	fprintf(fp,"%d \n",seednum);
	for (int i=0; i<seednum; i++) 
	{
		fprintf(fp,"%f ", printseeds[i][0]);
		fprintf(fp,"%f ", printseeds[i][1]);
		fprintf(fp,"%f ", printseeds[i][2]);
		fprintf(fp,"\n");
	}
	fclose(fp);


}

void dumpReconstruedField(char*  filename,float* new_vector, int* grid_res)
{

	FILE* fp=fopen(filename,"wb");
	if(fp==0)
	{
		printf("can not open reconstruct file to write \n");
		return ;//return;
	}
	grid_res[2]=2;
	fwrite(grid_res,sizeof(int),3,fp);
	fwrite(new_vector,sizeof(float),grid_res[0]*grid_res[1]*3,fp);
	fwrite(new_vector,sizeof(float),grid_res[0]*grid_res[1]*3,fp);
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

void UpdateOccupied(list<vtListSeedTrace*> lines, int* occupied,int* grid_res, int radius)
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
			int idx=(int)(p.x())+((int)(p.y()))*grid_res[0];
			{
				for(int ny=-radius;ny<=radius;ny++)
					for(int nx=-radius; nx<=radius; nx++)
					{
						int x=nx+(int)p.x();
						int y=ny+(int)p.y();
						if(x<0||x>grid_res[0]-1||y<0||y>grid_res[1]-1)
							continue;
						int myind=x+y*grid_res[0];
						occupied[myind]=1;
					}
			}
		}
	}

	
}

void Trimhalflines(list<vtListSeedTrace*> lines, int* occupied,int* grid_res)
{
	vtListSeedTrace* newlist;
	list<vtListSeedTrace*>::iterator pIter;
	pIter =lines.begin(); 
	int sou=0;
	for (; pIter!=lines.end(); pIter++) 
	{
		vtListSeedTrace *trace = *pIter; 
		std::list<VECTOR3*>::iterator pnIter; 
		pnIter = trace->begin(); 

		for (; pnIter!= trace->end(); pnIter++) 
		{
			VECTOR3 p = **pnIter; 
			int idx=(int)(p.x())+((int)(p.y()))*grid_res[0];
			/*if(occupied[idx]==1)
			{
				trace->erase(pnIter,trace->end());
				break;
			}
			*/

		}
	
	}

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
			int idx=(int)(p.x())+((int)(p.y()))*grid_res[0];
			{
				for(int ny=0;ny<=0;ny++)
					for(int nx=0; nx<=0; nx++)
					{
						int x=nx+(int)p.x();
						int y=ny+(int)p.y();
						if(x<0||x>grid_res[0]-1||y<0||y>grid_res[1]-1)
							continue;
						int myind=x+y*grid_res[0];
						occupied[myind]=1;
					}
			}
		}
	}

	
}
void FindNeighbors(VECTOR3 pImage, int stride, int* neighbors, int* grid_res)
{
	int xidx, yidx, index, cellNum, iFor, jFor, count;

	int xdim=grid_res[0];
	int ydim=grid_res[1];
	
	xidx=pImage.x();
	yidx=pImage.y();

	index = yidx*xdim+xidx;
	cellNum = stride*stride;

	// getting index to neighboring cells and itself
	stride = (stride-1)/2;
	count = 0;
	for(jFor = -stride; jFor <= stride; jFor++)
	{
		for(iFor = -stride; iFor <= stride; iFor++)
		{
			index = (yidx+jFor) * xdim + (xidx+iFor);
			if(((yidx+jFor) < 0) || ((xidx + iFor) < 0) || ((yidx+jFor) >= ydim) || ((xidx+iFor) >= xdim))
				neighbors[count++] = -1;
			else
				neighbors[count++] = index;
		}
	}
}

bool IsDistValid(VECTOR3& p1, VECTOR3& p2, float dist, bool& bNeedDelete)
{
	float d;
	d = sqrt((p1[0]-p2[0])*(p1[0]-p2[0]) + (p1[1]-p2[1])*(p1[1]-p2[1]));
	bNeedDelete = false;

	if(d < dist)
		return false;
	else
		return true;
}
void PutPointInGrid(VECTOR3 p, int streamlineId, int index, int pntIdx,int* grid_res,PointRef* m_grid)
{
	int xidx, yidx, idx,xdim;
	xdim=grid_res[0];
	idx=(int)p.x()+((int)p.y())*xdim;
	m_grid[idx].push_back(GridPoint(p, p, streamlineId, index, pntIdx, true));
}

bool CheckValidness(VECTOR3 p, VECTOR3 pObj,PointRef* m_grid, float m_sepDist,float m_sepMinDist,int* grid_res)
{
	//int xidx, yidx;
	int iFor, jFor, cellNum, stride, count;
	int* neighbors;
	int xdim=grid_res[0];

	// deciding searching neighbors
	stride = (int)ceil(m_sepDist/m_sepMinDist)*2 + 1;
	cellNum = stride*stride;
	neighbors = new int[cellNum];
	FindNeighbors(p, stride, neighbors,grid_res);
	
	// checking current cell, and 8 neighboring cells
	for(iFor = 0; iFor < cellNum; iFor++)
	{
		if(neighbors[iFor] != -1)		// valid neighbor
		{
			vector<GridPoint>::iterator pIter;
			pIter = m_grid[neighbors[iFor]].begin();
			pIter++;					// skip the first dummy vector
			for(; pIter != m_grid[neighbors[iFor]].end(); pIter++)
			{
				VECTOR3 pPos;
				bool bNeedDelete;
				pPos = (*pIter).posImage;
				if(!IsDistValid(p, pPos, m_sepDist, bNeedDelete))
				{
					delete[] neighbors;
					return false;
				}
			}
		}
	}

	delete[] neighbors;
	return true;			
}

// ADD-BY-LEETEN 02/06/2010-BEGIN
void
_SaveVectorField2D(int* grid_res, float *pf3Vectors, char szFilename[])
{
	FILE *fpFile = fopen(szFilename, "wb");
	assert(fpFile);

	fwrite(&grid_res[0], sizeof(grid_res[0]), 3, fpFile);
	fwrite(pf3Vectors, sizeof(pf3Vectors), 3 * grid_res[0] * grid_res[1], fpFile);
	fwrite(pf3Vectors, sizeof(pf3Vectors), 3 * grid_res[0] * grid_res[1], fpFile);
	fclose(fpFile);
}
// ADD-BY-LEETEN 02/06/2010-END

bool discardredundantstreamlines(float& cur_entropy,float eplison, list<vtListSeedTrace*> new_lines,
								 float* vectors, float* new_vectors,int* grid_res)
{
	int* dummy=new int[grid_res[0]*grid_res[1]*grid_res[2]];
	float* tmp_new_vectors=new float[grid_res[0]*grid_res[1]*grid_res[2]*3];
	memcpy(tmp_new_vectors,new_vectors,sizeof(float)*grid_res[0]*grid_res[1]*grid_res[2]*3);
	reconstruct_field_GVF_2D(new_vectors,vectors,grid_res,new_lines,dummy);
	float entropy=calcRelativeEntropy6(  vectors, new_vectors,  grid_res, VECTOR3(0,0,0),VECTOR3(grid_res[0],grid_res[1],0),0);
	// MOD-BY-LEETEN 02/06/2010-FROM:
		// printf(" entropy dif=%f\n",cur_entropy-entropy);
	// TO:
	printf("entropy dif=%+f\n",cur_entropy-entropy);
	// MOD-BY-LEETEN 02/06/2010-END

	if(cur_entropy-entropy>eplison)
	{
		cur_entropy=entropy;
		delete [] dummy;
		delete [] tmp_new_vectors;
		return false;
	}
	else
	{
		// ADD-BY-LEETEN 02/06/2010-BEGIN
		#if		DUMP_WHEN_ENTROPY_INCREASE
		_SaveVectorField2D(grid_res, tmp_new_vectors,	"tmp_vector.vec");
		_SaveVectorField2D(grid_res, new_vectors,		"new_vector.vec");
		#endif	// #if	DUMP_WHEN_ENTROPY_INCREASE
		// ADD-BY-LEETEN 02/06/2010-END

		memcpy(new_vectors,tmp_new_vectors,sizeof(float)*grid_res[0]*grid_res[1]*grid_res[2]*3);
		delete [] tmp_new_vectors;
		delete [] dummy;
		return true;
	}
}

void combinehalflines_check_stop(list<vtListSeedTrace*> lines, list<vtListSeedTrace*>& long_lines,int* grid_res,PointRef* m_grid,int streamlineId)
{
	float m_sepDist, m_sepMinDist;
	m_sepDist=m_sepMinDist=1;
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

		for (; pnIter!= trace->end(); pnIter++) 
		{
			VECTOR3 p = **pnIter; 
			int idx=(int)(p.x())+((int)(p.y()))*grid_res[0];
			
			if(sou%2==0)
			newlist->push_front(new VECTOR3(p.x(),p.y(),0));
			else
			newlist->push_back(new VECTOR3(p.x(),p.y(),0));


			bool valid=CheckValidness(p,p, m_grid, m_sepDist,m_sepMinDist, grid_res);
			if(valid==false)
				break;
		}
		if((sou%2==1)&&newlist->size()>10)
			long_lines.push_back(newlist);
		sou++;
	}
	int pntIdx=0;
	pIter = long_lines.begin(); 
	for (; pIter!=long_lines.end(); pIter++) 
	{
		vtListSeedTrace *trace = *pIter; 
		std::list<VECTOR3*>::iterator pnIter; 
		pnIter = trace->begin(); 
		for (; pnIter!= trace->end(); pnIter++) 
		{
			VECTOR3 p = **pnIter; 
			PutPointInGrid(p,  streamlineId,streamlineId, pntIdx++, grid_res, m_grid);
		}
	}
	
}
//stop when entering a dull region
void combinehalflines_check_stop_entropy(list<vtListSeedTrace*> lines, list<vtListSeedTrace*>& long_lines,int* grid_res,
										 float* entropies)
{
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

		for (; pnIter!= trace->end(); pnIter++) 
		{
			VECTOR3 p = **pnIter; 
			int idx=(int)(p.x())+((int)(p.y()))*grid_res[0];
			
			if(sou%2==0)
			newlist->push_front(new VECTOR3(p.x(),p.y(),0));
			else
			newlist->push_back(new VECTOR3(p.x(),p.y(),0));

	
			bool valid=false;
			float u = ((float)rand()/(float)RAND_MAX);
			float prob=entropies[idx];
			if(prob>0.5)
				valid=true;
			else 
			if(u<sqrt(prob))
				valid=true;
			if (valid==false)
				break;
		}
		if((sou%2==1)&&newlist->size()>10)
			long_lines.push_back(newlist);
		sou++;
	}

}

void combinehalflines(list<vtListSeedTrace*> lines, list<vtListSeedTrace*>& long_lines,int* grid_res)
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
			int idx=(int)(p.x())+((int)(p.y()))*grid_res[0];
			
			if(sou%2==0)
			newlist->push_front(new VECTOR3(p.x(),p.y(),0));
			else
			newlist->push_back(new VECTOR3(p.x(),p.y(),0));
		}
		if(sou%2==1)
			long_lines.push_back(newlist);
		sou++;
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
			footsteps[(int)p.y()*grid_res[0]+(int)p.x()]=next_id;
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

void reconstruct_field_GVF_2D_tmp(float* new_vectors,float* vectors, int* grid_res,
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

//reconstruct by diffusion
void reconstruct_field_GVF_2D(float* new_vectors,float* vectors, int* grid_res,list<vtListSeedTrace*> l_list,
							  int* donotchange)
{
	// MOD-BY-LEETEN 02/06/2010-FROM:
		// int iter=2*max(grid_res[0],grid_res[1]);
	// TO:
	int iter = 4 * max(grid_res[0],grid_res[1]);
	// MOD-BY-LEETEN 02/06/2010-END

	//initial GVF, combine with input new_vectors
	list<vtListSeedTrace*>::iterator pIter;
	float* kx=new float[grid_res[0]*grid_res[1]];
	float* ky=new float[grid_res[0]*grid_res[1]];

	float* b=new float[grid_res[0]*grid_res[1]];
	float* c1=new float[grid_res[0]*grid_res[1]];
	float* c2=new float[grid_res[0]*grid_res[1]];

	// ADD-BY-LEETEN 02/06/2010-BEGIN
	float* tmp_new_vectors=(float*)calloc(sizeof(float), grid_res[0]*grid_res[1]*3);
	// ADD-BY-LEETEN 02/06/2010-END

	for(int i=0; i<grid_res[0]*grid_res[1];i++)
	{
		kx[i]=ky[i]=b[i]=c1[i]=c2[i]=0;
	}
	pIter = l_list.begin();
	for (; pIter!=l_list.end(); pIter++) 
	{

		vtListSeedTrace *trace = *pIter; 
		std::list<VECTOR3*>::iterator pnIter; 
		pnIter = trace->begin(); 
		for (; pnIter!= trace->end(); pnIter++) 
		{

			VECTOR3* p=*(pnIter);
			int idx=(int)p->x()+((int)p->y())*grid_res[0];
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
			// ADD-BY-LEETEN 02/06/2010-BEGIN
			#if		IMPLEMENTED_BY_XUL	
			// ADD-BY-LEETEN 02/06/2010-END
				new_vectors[idx*3+0]=kx[idx];
				new_vectors[idx*3+1]=ky[idx];
				donotchange[idx]=1;
			// ADD-BY-LEETEN 02/06/2010-BEGIN
			#else	// #if	IMPLEMENTED_BY_XUL
			tmp_new_vectors[idx*3+0]=kx[idx];
			tmp_new_vectors[idx*3+1]=ky[idx];
			donotchange[idx]=1;
			#endif	// #if	IMPLEMENTED_BY_XUL
			// ADD-BY-LEETEN 02/06/2010-END
		}
	}

	// ADD-BY-LEETEN 02/08/2010-BEGIN
	#if	INIT_BOUNDARY_AS_STREAMLINE	
	// set the boundary
	for(int		idx = 0,	y = 0; y < grid_res[1]; y++)
		for(int				x = 0; x < grid_res[0]; x++, idx++)
			if( 0 == x || grid_res[0]- 1 == x || 0 == y || grid_res[1]- 1 == y )
				for(int j = 0; j < 2; j++)
					new_vectors[3*idx + j] = tmp_new_vectors[3*idx+j] = vectors[3*idx+j];
	#endif	// #if	INIT_BOUNDARY_AS_STREAMLINE	
	// ADD-BY-LEETEN 02/08/2010-END

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
	// ADD-BY-LEETEN 02/06/2010-BEGIN
	#if		IMPLEMENTED_BY_XUL	
	// ADD-BY-LEETEN 02/06/2010-END
		// DEL-BY-LEETEN 02/06/2010-BEGIN
			// float* tmp_new_vectors=new float[grid_res[0]*grid_res[1]*3];
		// DEL-BY-LEETEN 02/06/2010-END
		for(int i=0; i<grid_res[0]*grid_res[1]*3;i++)
			tmp_new_vectors[i]=new_vectors[i];
	// ADD-BY-LEETEN 02/06/2010-BEGIN
	#endif	// #if		IMPLEMENTED_BY_XUL	
	// ADD-BY-LEETEN 02/06/2010-END

	#if	0	// TEST-DEBUG
	#if		DUMP_WHEN_ENTROPY_INCREASE
	_SaveVectorField2D(grid_res, new_vectors,		"vector_new.vec");
	_SaveVectorField2D(grid_res, tmp_new_vectors,	"vector_init.vec");
	#endif	// #if	DUMP_WHEN_ENTROPY_INCREASE
	#endif

	//parameter setting:
	float mu=0.1;

	// ADD-BY-LEETEN 02/05/2010-BEGIN
	#if	DIFFUSION_ON_CUDA	
	_FlowDiffusion(
		mu, 
		iter, 
		grid_res[0], grid_res[1], grid_res[2],
		16,					// iBlockZSize
		b,					// float *pfWeightVolume,
		c1,					// float *pfOffsetVolume,
		c2,					// 
		NULL,					//
		tmp_new_vectors,	// float *pfSrcVolume,
		new_vectors,		// float *pfDstVolume
		NULL
	);
	#else	// #if	DIFFUSION_ON_CUDA	
	// ADD-BY-LEETEN 02/05/2010-END

	//iteration by iteration
	for(int i=0; i<iter; i++)
	{
		//start iterations
		//update the GVF
		// ADD-BY-LEETEN 02/05/2010-BEGIN
		#if	DONOT_APPLY_DIFFUSION_ON_BOUNDARY		
		// ADD-BY-LEETEN 02/05/2010-END
		for(int y=1; y<grid_res[1]-1;y++)
		{
			for(int x=1; x<grid_res[0]-1;x++)//cut boundary for now
			{
				int idx=x+y*grid_res[0]; 
				int idx_1=x+1+y*grid_res[0]; 
				int idx_2=x+(y+1)*grid_res[0]; 
				int idx_3=x-1+y*grid_res[0]; 
				int idx_4=x+(y-1)*grid_res[0]; 
		// ADD-BY-LEETEN 02/05/2010-BEGIN
		#else	// #if	DONOT_APPLY_DIFFUSION_ON_BOUNDARY
		for(int y=0; y<grid_res[1];y++)
		{
			for(int x=0; x<grid_res[0];x++)//cut boundary for now
			{
				int idx=x+y*grid_res[0]; 
				int idx_1=min(x+1, grid_res[0]-1)+y*grid_res[0]; 
				int idx_2=x+min(y+1, grid_res[1]-1)*grid_res[0]; 
				int idx_3=max(x-1, 0)+y*grid_res[0]; 
				int idx_4=x+max(y-1, 0)*grid_res[0]; 
		#endif	// #if	DONOT_APPLY_DIFFUSION_ON_BOUNDARY
		// ADD-BY-LEETEN 02/05/2010-END

				// ADD-BY-LEETEN 02/06/2010-BEGIN
				#if	IMPLEMENTED_BY_XUL	
				// ADD-BY-LEETEN 02/06/2010-END
					tmp_new_vectors[idx*3+0]=	(1-b[idx])*new_vectors[idx*3+0]+
												mu*(new_vectors[idx_1*3+0]+new_vectors[idx_2*3+0]+new_vectors[idx_3*3+0]+new_vectors[idx_4*3+0]-4*new_vectors[idx*3+0])+
												c1[idx];

					tmp_new_vectors[idx*3+1]=	(1-b[idx])*new_vectors[idx*3+1]+
												mu*(new_vectors[idx_1*3+1]+new_vectors[idx_2*3+1]+new_vectors[idx_3*3+1]+new_vectors[idx_4*3+1]-4*new_vectors[idx*3+1])+
												c2[idx];
				// ADD-BY-LEETEN 02/06/2010-BEGIN
				#else	// #if	IMPLEMENTED_BY_XUL
				new_vectors[idx*3+0]=	(1-b[idx])*tmp_new_vectors[idx*3+0]+
											mu*(tmp_new_vectors[idx_1*3+0]+tmp_new_vectors[idx_2*3+0]+tmp_new_vectors[idx_3*3+0]+tmp_new_vectors[idx_4*3+0]-4*tmp_new_vectors[idx*3+0])+
											c1[idx];

				new_vectors[idx*3+1]=	(1-b[idx])*tmp_new_vectors[idx*3+1]+
											mu*(tmp_new_vectors[idx_1*3+1]+tmp_new_vectors[idx_2*3+1]+tmp_new_vectors[idx_3*3+1]+tmp_new_vectors[idx_4*3+1]-4*tmp_new_vectors[idx*3+1])+
											c2[idx];
				#endif	// #if	IMPLEMENTED_BY_XUL
				// ADD-BY-LEETEN 02/06/2010-END
			}
		}
		
		//update the GVF
		#if		0	// MOD-BY-LEETEN 02/06/2010-FROM:
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
		#else	// MOD-BY-LEETEN 02/06/2010-TO:
		static double dPrevError;
		double dError = 0.0;
		for(int		idx = 0,	z=0; z<grid_res[2];z++)
			for(int				y=0; y<grid_res[1];y++)
				for(int			x=0; x<grid_res[0];x++, idx++)
					for(int j = 0; j < 3; j++)
					{
						double dDiff = double(new_vectors[idx*3+j] - tmp_new_vectors[idx*3+j]);
						dError += dDiff * dDiff;

						#if		IMPLEMENTED_BY_XUL
						new_vectors[idx*3+j] = tmp_new_vectors[idx*3+j];
						#else	// #if		IMPLEMENTED_BY_XUL
						tmp_new_vectors[idx*3+j] = new_vectors[idx*3+j];
						#endif	// #if		IMPLEMENTED_BY_XUL
					}

		dError = sqrt(dError / (3 * grid_res[0] * grid_res[1] * grid_res[2]));
		if( i > max(max(grid_res[0], grid_res[1]), grid_res[2]) )
		{
			double dErrorRate = dError / dPrevError;
			if( DIFFUSION_CONVERGE_THRESHOLD < dErrorRate && dErrorRate <= 1.0 )
			{
				printf("Diffusion takes %d iterations to converge. ", i);
				break;
			}
		}
		#endif	// MOD-BY-LEETEN 02/06/2010-END
	}
	// ADD-BY-LEETEN 02/06/2010-BEGIN
	#if		!IMPLEMENTED_BY_XUL	
	memcpy(new_vectors, tmp_new_vectors, sizeof(new_vectors[0]) * grid_res[0] * grid_res[1] * 3);
	#endif	// !IMPLEMENTED_BY_XUL	
	// ADD-BY-LEETEN 02/06/2010-END

	// ADD-BY-LEETEN 02/05/2010-BEGIN
	#endif	// #if	DIFFUSION_ON_CUDA
	// ADD-BY-LEETEN 02/05/2010-END
	delete [] tmp_new_vectors;
	delete [] kx;
	delete [] ky;
	delete [] b;
	delete [] c1;
	delete [] c2;

}



//reconstruct by gaussion
void reconstruct_field_GVF_2D_gaussian(float* new_vectors,float* vectors, int* grid_res,list<vtListSeedTrace*> l_list,
							  int* donotchange)
{

	float pi=3.14159;
	float t;
	int iter=.5;//max(grid_res[0],grid_res[1]);

	t=.25;
	//initial GVF, combine with input new_vectors
	list<vtListSeedTrace*>::iterator pIter;

	//update the GVF
	float* tmp_new_vectors=new float[grid_res[0]*grid_res[1]*3];
	for(int i=0; i<grid_res[0]*grid_res[1]*3;i++)
		tmp_new_vectors[i]=new_vectors[i];

	//update the GVF
	for(int y=0; y<grid_res[1];y++)
	{
		for(int x=0; x<grid_res[0];x++)
		{
			int idx=x+y*grid_res[0]; 
			float fx,fy;
			fx=0; fy=0;
			
			float line_len=0;
			for (pIter=l_list.begin(); pIter!=l_list.end(); pIter++) 
			{

			vtListSeedTrace *trace = *pIter; 
			line_len=line_len+trace->size();
			std::list<VECTOR3*>::iterator pnIter; 
			pnIter = trace->begin(); 
			for (; pnIter!= trace->end(); pnIter++) 
			{

				VECTOR3* p=*(pnIter);
				int idx1=(int)p->x()+((int)p->y())*grid_res[0];
				float vx=vectors[idx1*3+0];
				float vy=vectors[idx1*3+1];
				//normalize
				float len=sqrt(vx*vx+vy*vy);
				if(len>0)
				{
					vx=vx/len;	vy=vy/len;

				}
				float dist_sqr=((x-p->x())*(x-p->x())+
								(y-p->y())*(y-p->y()));
				float guass_xy;
				//guass_xy=exp(-dist_sqr/(4*t))/(sqrt(4*pi*t));
				guass_xy=exp(-dist_sqr/(2*t))/(2*pi*t);

				fx=fx+vx*guass_xy;
				fy=fy+vy*guass_xy;
			}
			}
			fx=fx/line_len;
			fy=fy/line_len;

			tmp_new_vectors[idx*3+0]=fx;
			tmp_new_vectors[idx*3+1]=fy;
		}
	}


	//parameter setting:
	float mu=0.1;
	//iteration by iteration
	
		
	//update the GVF
	for(int y=0; y<grid_res[1];y++)
	{
		for(int x=0; x<grid_res[0];x++)
		{
			int idx=x+y*grid_res[0]; 
			//only update when the value improves
			new_vectors[idx*3+0]=tmp_new_vectors[idx*3+0];
			new_vectors[idx*3+1]=tmp_new_vectors[idx*3+1];
	
		}
	}
	
	delete [] tmp_new_vectors;
	
}

void reconstruct_field_GVF_3D(float* new_vectors,float* vectors, int* grid_res,list<vtListSeedTrace*> l_list)
{

	int iter=40;

	//initial GVF, combine with input new_vectors
	list<vtListSeedTrace*>::iterator pIter;
	float* kx=new float[grid_res[0]*grid_res[1]*grid_res[2]];
	float* ky=new float[grid_res[0]*grid_res[1]*grid_res[2]];
	float* kz=new float[grid_res[0]*grid_res[1]*grid_res[2]];

	float* b=new float[grid_res[0]*grid_res[1]*grid_res[2]];
	float* c1=new float[grid_res[0]*grid_res[1]*grid_res[2]];
	float* c2=new float[grid_res[0]*grid_res[1]*grid_res[2]];
	float* c3=new float[grid_res[0]*grid_res[1]*grid_res[2]];

	for(int i=0; i<grid_res[0]*grid_res[1]*grid_res[2];i++)
	{
		kx[i]=ky[i]=kz[i]=b[i]=c1[i]=c2[i]=c3[i]=0;
	}
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
			new_vectors[idx*3+0]=kx[idx];
			new_vectors[idx*3+1]=ky[idx];
			new_vectors[idx*3+2]=kz[idx];
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
	float* tmp_new_vectors=new float[grid_res[0]*grid_res[1]*grid_res[2]*3];
	for(int i=0; i<grid_res[0]*grid_res[1]*grid_res[2]*3;i++)
		tmp_new_vectors[i]=new_vectors[i];

	//parameter setting:
	float mu=0.1;
	//iteration by iteration
	for(int i=0; i<iter; i++)
	{
		//start iterations
		//update the GVF
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

				}
			}
		}
		
		//update the GVF
		for(int z=0; z<grid_res[2];z++)
		{
			for(int y=0; y<grid_res[1];y++)
			{
				for(int x=0; x<grid_res[0];x++)
				{
					int idx=x+y*grid_res[0]+z*grid_res[0]*grid_res[1]; 
					new_vectors[idx*3+0]=tmp_new_vectors[idx*3+0];
					new_vectors[idx*3+1]=tmp_new_vectors[idx*3+1];
					new_vectors[idx*3+2]=tmp_new_vectors[idx*3+2];

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
	delete [] c3;

}
float log2(float ang)
{
	return log(ang)/log(2.0);
}
/*
//actually this computes the relative entropy
float calcRelativeEntropy6( float* vectors,float* new_vectors, int* grid_res, VECTOR3 startpt,VECTOR3 endpt)
{
	int binnum=60;
	int *histo_pxy=new int[binnum*binnum];
	int *histo_px=new int[binnum];
	int *histo_py=new int[binnum];
	memset(histo_pxy,0,sizeof(int)*binnum*binnum);
	memset(histo_py,0,sizeof(int)*binnum);
	memset(histo_px,0,sizeof(int)*binnum);

	int area=(endpt.y()-startpt.y())*(endpt.x()-startpt.x());

	float pi=3.1415926;
	
	int vac_count=0;//vacuum region
	for(int y=(int)startpt.y(); y<(int)endpt.y();y++)
	{
		for(int x=(int)startpt.x(); x<(int)endpt.x();x++)
		{
			//VECTOR3 p=point_cloud[i];
			VECTOR3 orif,newf;
			int idx=x+y*grid_res[0];
			orif.Set(vectors[idx*3+0],vectors[idx*3+1],0);
			newf.Set(new_vectors[idx*3+0],new_vectors[idx*3+1],0);

			float scalar[2];
			if((orif.x()==0) && (orif.y()==0))
				scalar[0]=0;
			else
				scalar[0]=pi+(atan2(orif.y(),orif.x()));

			if((newf.x()==0)&&(newf.y()==0))
			{
				scalar[1]=0;
				vac_count++;
			}
			else
				scalar[1]=pi+(atan2(newf.y(),newf.x()));

			int bin_no_ori=(binnum-1)*scalar[0]/(2*3.1415926);
			int bin_no_new=(binnum-1)*scalar[1]/(2*3.1415926);
			if(bin_no_ori<0 || bin_no_ori>=binnum||
				bin_no_new<0 || bin_no_new>=binnum)
			{
				printf("sth wrong, bin id=%d  %d\n", bin_no_ori,bin_no_new);
			}
			idx=bin_no_ori+binnum*bin_no_new;
			histo_pxy[idx]++;//p(x,y)
			//p(y)
			histo_px[bin_no_ori]++;
			histo_py[bin_no_new]++;
		}
	}

	//calc probs.
	float* pxy=new float[binnum*binnum];
	float* py=new float[binnum];
	float* px=new float[binnum];

	int total_num=0;
	for(int i=0; i<binnum*binnum; i++)
		total_num+=histo_pxy[i];

	//H(x|y=a)
	float* entropy_tmp=new float[binnum];

	for(int y=0; y<binnum; y++)
	{
		entropy_tmp[y]=0;
		float* p_tmp=new float[binnum];
		memset(p_tmp,0,sizeof(float)*binnum);
		for(int x=0; x<binnum; x++)
		{
			//p(x|y=a)
			float p_tmp=histo_pxy[x+binnum*y]/((float)(histo_py[y]));
			if(p_tmp>0)
			entropy_tmp[y]-=(p_tmp*log(p_tmp));
		}
		delete [] p_tmp;
	}
	
//	printf("bin num=%d\n",binnum);

	float entropy=0;
//float sf=0;
	for(int i=0; i<binnum; i++)
	{
		py[i]=(((float)histo_py[i])/((float)area));
		px[i]=(((float)histo_px[i])/((float)area));
	//	entropy=entropy+py[i]*entropy_tmp[i];
		if(px[i]!=0 && py[i]!=0 )
		entropy=entropy-py[i]*log2(py[i]/px[i]);
	//	if(px[i]==0)
	//		entropy+=9;//infinity
//	sf=sf+entropy_tmp[i];
	}
//getchar();

	
	float penalty=vac_count*0.001;
	//entropy=entropy+penalty;
	

	delete [] pxy;
	delete [] py;
	delete [] px;
	delete [] histo_pxy;
	delete [] histo_py;
	delete [] histo_px;
	delete [] entropy_tmp;
	return entropy;

}
*/

float calcEntropy( float* vectors, int* grid_res, VECTOR3 startpt,VECTOR3 endpt)
{
	int binnum=60;
	int *histo_py=new int[binnum];
	memset(histo_py,0,sizeof(int)*binnum);

	int area=(endpt.y()-startpt.y())*(endpt.x()-startpt.x());
	float pi=3.1415926;
	
	for(int y=(int)startpt.y(); y<(int)endpt.y();y++)
	{
		for(int x=(int)startpt.x(); x<(int)endpt.x();x++)
		{
			VECTOR3 orif;
			int idx=x+y*grid_res[0];
			
			orif.Set(vectors[idx*3+0],vectors[idx*3+1],0);
			float scalar[2];
			if((orif.x()==0) && (orif.y()==0))
				scalar[0]=0;
			else
				scalar[0]=pi+(atan2(orif.y(),orif.x()));

			int bin_no_ori=(binnum-1)*scalar[0]/(2*3.1415926);

			if(bin_no_ori<0 || bin_no_ori>=binnum)
				printf("sth wrong, bin id=%d  \n", bin_no_ori);
			histo_py[bin_no_ori]++;
		}
	}

	//calc probs.
	float* py=new float[binnum];
	float entropy=0;
	for(int i=0; i<binnum; i++)
	{
		py[i]=(((float)histo_py[i])/((float)area));
		if(py[i])
			entropy=entropy-py[i]*log2(py[i]);
	}
	

	delete [] py;
	delete [] histo_py;
	return entropy;

}



float calcRelativeEntropy6_load_bins(int* bin_vector, int* bin_newvectors,int* grid_res, VECTOR3 startpt,VECTOR3 endpt)
{
	int binnum=60;
	int *histo_pxy=new int[binnum*binnum];
	int *histo_py=new int[binnum];
	memset(histo_pxy,0,sizeof(int)*binnum*binnum);
	memset(histo_py,0,sizeof(int)*binnum);

	int area=(endpt.y()-startpt.y())*(endpt.x()-startpt.x());
	float pi=3.1415926;
	
	float total_err=0;
	int vac_count=0;//vacuum region
	
	for(int y=(int)startpt.y(); y<(int)endpt.y();y++)
	{
		for(int x=(int)startpt.x(); x<(int)endpt.x();x++)
		{
			
			//VECTOR3 p=point_cloud[i];
			
			int idx=x+y*grid_res[0];
			int bin_no_ori=bin_vector[idx];
			int bin_no_new=bin_newvectors[idx];

			//if the point is on boundary, do not consider
			if( (x<=0) || x>=grid_res[0]-1 ||
				 y<=0  || y>=grid_res[1]-1)
				 bin_no_ori=bin_no_new;

			if(bin_no_ori<0 || bin_no_ori>=binnum||
				bin_no_new<0 || bin_no_new>=binnum)
			{
				printf("sth wrong, bin id=%d  %d\n", bin_no_ori,bin_no_new);
			}
			idx=bin_no_ori+binnum*bin_no_new;
			histo_pxy[idx]++;//p(x,y)
			//p(y)
			histo_py[bin_no_new]++;
		}
	}

	//calc probs.
	float* pxy=new float[binnum*binnum];
	float* py=new float[binnum];

	int total_num=0;
	for(int i=0; i<binnum*binnum; i++)
		total_num+=histo_pxy[i];

	//H(x|y=a)
	float* entropy_tmp=new float[binnum];

	for(int y=0; y<binnum; y++)
	{
		entropy_tmp[y]=0;
		for(int x=0; x<binnum; x++)
		{
			//p(x|y=a)
			if(histo_pxy[x+binnum*y]==0)
				continue;
			float p_tmp=histo_pxy[x+binnum*y]/((float)(histo_py[y]));
			if(p_tmp>0)
			entropy_tmp[y]-=(p_tmp*log2(p_tmp));
		}
	}
	
	float entropy=0;
	for(int i=0; i<binnum; i++)
	{
		py[i]=(((float)histo_py[i])/((float)area));
		entropy=entropy+py[i]*entropy_tmp[i];
	}
	

	delete [] pxy;
	delete [] py;
	delete [] histo_pxy;
	delete [] histo_py;
	delete [] entropy_tmp;
	return entropy;

}

float calcRelativeEntropy6_load_histograms(int* bin_vector, int* bin_newvectors,int* grid_res, VECTOR3 startpt,VECTOR3 endpt,
										   int* histo_pxy,int *histo_py)
{
	int binnum=60;

	int area=(endpt.y()-startpt.y())*(endpt.x()-startpt.x());
	float pi=3.1415926;
	
	float total_err=0;
	int vac_count=0;//vacuum region
	

	//calc probs.
	float* pxy=new float[binnum*binnum];
	float* py=new float[binnum];

	int total_num=0;
	for(int i=0; i<binnum*binnum; i++)
		total_num+=histo_pxy[i];

	//H(x|y=a)
	float* entropy_tmp=new float[binnum];

	for(int y=0; y<binnum; y++)
	{
		entropy_tmp[y]=0;
		for(int x=0; x<binnum; x++)
		{
			//p(x|y=a)
			if(histo_pxy[x+binnum*y]==0)
				continue;
			float p_tmp=histo_pxy[x+binnum*y]/((float)(histo_py[y]));
			if(p_tmp>0)
			entropy_tmp[y]-=(p_tmp*log2(p_tmp));
		}
	}
	
	float entropy=0;
	for(int i=0; i<binnum; i++)
	{
		py[i]=(((float)histo_py[i])/((float)area));
		entropy=entropy+py[i]*entropy_tmp[i];
	}
	

	delete [] pxy;
	delete [] py;
	//delete [] histo_pxy;
	//delete [] histo_py;
	delete [] entropy_tmp;
	return entropy;

}





float calcRelativeEntropy6( float* vectors,float* new_vectors, int* grid_res, VECTOR3 startpt,VECTOR3 endpt,int* occupied)
{
	int binnum=60;
	int *histo_pxy=new int[binnum*binnum];
	int *histo_py=new int[binnum];
	memset(histo_pxy,0,sizeof(int)*binnum*binnum);
	memset(histo_py,0,sizeof(int)*binnum);

	int area=(endpt.y()-startpt.y())*(endpt.x()-startpt.x());
	float pi=3.1415926;
	
	float total_err=0;
	int vac_count=0;//vacuum region
	
	for(int y=(int)startpt.y(); y<(int)endpt.y();y++)
	{
		for(int x=(int)startpt.x(); x<(int)endpt.x();x++)
		{
			//VECTOR3 p=point_cloud[i];
			VECTOR3 orif,newf,tmp;
			int idx=x+y*grid_res[0];
			//if(occupied && occupied[idx]==1)
			{
				//area--;
			//	continue;
			}
			
			orif.Set(vectors[idx*3+0],vectors[idx*3+1],0);
			newf.Set(new_vectors[idx*3+0],new_vectors[idx*3+1],0);

			newf.Normalize();
			tmp=newf-orif;
			total_err+=tmp.GetMag();
			float scalar[2];
			if((orif.x()==0) && (orif.y()==0))
				scalar[0]=0;
			else
				scalar[0]=pi+(atan2(orif.y(),orif.x()));

			if((newf.x()==0)&&(newf.y()==0))
			{
				scalar[1]=0;
				vac_count++;
			}
			else
				scalar[1]=pi+(atan2(newf.y(),newf.x()));

			if(fabs(scalar[1]-scalar[0])<3.1415926/((float)binnum)*2)
				scalar[1]=scalar[0];

			int bin_no_ori=(binnum-1)*scalar[0]/(2*3.1415926);
			int bin_no_new=(binnum-1)*scalar[1]/(2*3.1415926);

			//if the point is on boundary, do not consider
			if( (x<=0) || x>=grid_res[0]-1 ||
				 y<=0  || y>=grid_res[1]-1)
				 bin_no_ori=bin_no_new;

			if(bin_no_ori<0 || bin_no_ori>=binnum||
				bin_no_new<0 || bin_no_new>=binnum)
			{
				printf("sth wrong, bin id=%d  %d\n", bin_no_ori,bin_no_new);
			}
			idx=bin_no_ori+binnum*bin_no_new;
			histo_pxy[idx]++;//p(x,y)
			//p(y)
			histo_py[bin_no_new]++;
		}
	}

	//calc probs.
	float* pxy=new float[binnum*binnum];
	float* py=new float[binnum];

	int total_num=0;
	for(int i=0; i<binnum*binnum; i++)
		total_num+=histo_pxy[i];

	//H(x|y=a)
	float* entropy_tmp=new float[binnum];

	for(int y=0; y<binnum; y++)
	{
		entropy_tmp[y]=0;
		float* p_tmp=new float[binnum];
		memset(p_tmp,0,sizeof(float)*binnum);
		for(int x=0; x<binnum; x++)
		{
			//p(x|y=a)
			float p_tmp=histo_pxy[x+binnum*y]/((float)(histo_py[y]));
			if(p_tmp>0)
			entropy_tmp[y]-=(p_tmp*log2(p_tmp));
		}
		delete [] p_tmp;
	}
	
	float entropy=0;
	for(int i=0; i<binnum; i++)
	{
		py[i]=(((float)histo_py[i])/((float)area));
		entropy=entropy+py[i]*entropy_tmp[i];
	}
	

	delete [] pxy;
	delete [] py;
	delete [] histo_pxy;
	delete [] histo_py;
	delete [] entropy_tmp;
	return entropy;

}




bool getnewseed(VECTOR3* newseed,float* new_vectors,float* vectors,int* grid_res, float thres)
{
	float max_dif=0;
	float sx,sy;
	int x,y;

	for(int y=0; y<grid_res[1];y++)
	{
			VECTOR3 orif,newf,difv;
		for(int x=0; x<grid_res[0];x++)
		{
			int idx=x+y*grid_res[0];
			orif.Set(vectors[idx*3+0],vectors[idx*3+1],0);
			newf.Set(new_vectors[idx*3+0],new_vectors[idx*3+1],0);
			
			//first normalize
			orif.Normalize();
			newf.Normalize();

			difv.Set(fabs(orif.x()-newf.x()),fabs(orif.y()-newf.y()),0);
			float scalar=difv.GetMag();
			if(scalar>max_dif)
			{
				max_dif=scalar;
				sx=x; sy=y;
			}
		}
	}

	if(max_dif>thres)
	{
		newseed->Set(sx,sy,0);
		return true;
	}
	else
	{
		printf("different too small\n");
		//getchar();

		return false;
	}
}

void QuadNode::CalcEntropy_FindMaxEntropyNode(float* vectors,float* new_vectors, 
											  int* grid_res, int& min_x, int& max_x, int& min_y, int& max_y,int* occupied,float hx)
{
	if(LeaveNode==1)
	{
		min_x=XMin; max_x=XMax; min_y=YMin; max_y=YMax;
		return;
	}
/*	m_entropy=calcRelativeEntropy6( vectors,new_vectors, grid_res, 
									VECTOR3(XMin,YMin,0), 
									VECTOR3(XMax,YMax,0));*/

	if(LeaveNode!=1)
	{
	//	for(int i=0; i<ChildrenNo;i++)
	//		Children[i]->CalcEntropy(vectors, new_vectors, grid_res);

		float maxentropy=0;
		int maxi=0;
		for(int i=0; i<ChildrenNo;i++)
		{
			//(Children[i]->m_entropy
			float entropy=calcRelativeEntropy6( vectors,new_vectors, grid_res, 
				VECTOR3(Children[i]->XMin, Children[i]->YMin,0),
				VECTOR3(Children[i]->XMax, Children[i]->YMax,0),occupied);

			float alpha=1;
	
			if(occupied)// && Children[i]->LeaveNode==1)//add density control
			{			
		
				bool found=false;
				for(int y=Children[i]->YMin;y<Children[i]->YMax;y++)
				{
					for(int x=Children[i]->XMin;x<Children[i]->XMax;x++)
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
				entropy=(entropy)*alpha+(1-alpha)*densityv;
			}	
				//printf("occupied=%f\n",1-pv);
		
			if(entropy > maxentropy)
			{	
				maxentropy=entropy;
				maxi=i;
			}

		}
		Children[maxi]->CalcEntropy_FindMaxEntropyNode( vectors, new_vectors,  grid_res,min_x,  max_x,  min_y,  max_y,occupied,hx);
	}
}

void QuadNode::CalcEntropy(float* vectors,float* new_vectors, int* grid_res)
{
	/*

	m_entropy=calcRelativeEntropy6( vectors,new_vectors, grid_res, 
									VECTOR3(XMin,YMin,0), 
									VECTOR3(XMax,YMax,0));

	if(LeaveNode!=1)
	{
		for(int i=0; i<ChildrenNo;i++)
			Children[i]->CalcEntropy(vectors, new_vectors, grid_res);

	}
	*/
}

void QuadNode::FindMaxEntropyNode(int& min_x, int& max_x, int& min_y, int& max_y)
{
	if(LeaveNode==1)
	{
		min_x=XMin; max_x=XMax; min_y=YMin; max_y=YMax;
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
	Children[maxi]->FindMaxEntropyNode( min_x,  max_x,  min_y,  max_y);

}
//Function for the BonoNode class
//Assign the children of the BonoNode
void QuadNode::getLeaves(std::vector<VECTOR3>& leaves)
{
	if(LeaveNode==1)
	{
		 VECTOR3 startpt, endpt;
		 startpt.Set(XMin,YMin,0);
		 endpt.Set(XMax, YMax,0);
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
void QuadNode::Build(int xdim, int ydim,int MinLen)
{
	int XLen = XMax-XMin+1;
	int YLen = YMax-YMin+1;
	
	//Already reach the finest level
	if (XLen <= MinLen && YLen <= MinLen)
		return;

	//The node is not a leave node
	LeaveNode = 0;

	// 3 2 
	// 0 1
	//Build the children of the node

	//Child 00
	Children[0] = new QuadNode (XMin, (XMin + XMax)/2,
								YMin, (YMin + YMax)/2);
	Children[0]->Build(xdim, ydim, MinLen);

	//Child 01
	Children[1] = new QuadNode ((XMin + XMax)/2, XMax,
								YMin, (YMin + YMax)/2);
	Children[1]->Build(xdim, ydim, MinLen);

	//Child 10
	Children[2] = new QuadNode ((XMin + XMax)/2, XMax,
								(YMin + YMax)/2, YMax);
	Children[2]->Build(xdim, ydim, MinLen);

	//Child 11
	Children[3] = new QuadNode (XMin, (XMin + XMax)/2,
								(YMin + YMax)/2, YMax);
	Children[3]->Build(xdim, ydim, MinLen);
}


//Functions for the BonoTree class
//Build the Bono tree
void QuadTree::Build(int MinLen)
{
	Root = new QuadNode(0, CellXDim-1, 0, CellYDim-1);
	Root->Build(CellXDim, CellYDim, MinLen);

}

void QuadTree::drawSelf()
{
	Root->drawself();
}

/*

$Log: not supported by cvs2svn $
Revision 1.1  2010/01/22 21:09:12  leeten

[01/22/2010]
1. [1ST] First time checkin.


*/
