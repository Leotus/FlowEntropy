#line 1 "FlowFusion.cudafe1.cpp"
#line 1 "e:/project/FlowEntropy/FlowFusion/FlowFusion_test/FlowFusion.cu"
#line 105 "C:\\Program Files\\Microsoft Visual Studio 8\\VC\\INCLUDE\\crtdefs.h"
#pragma comment(linker,"/manifestdependency:\"type='win32' " "name='" "Microsoft.VC80" ".DebugCRT' " "version='" "8.0.50727.4053" "' " "processorArchitecture='x86' " "publicKeyToken='" "1fc8b3b9a1e18e3b" "'\"")
#line 164 "C:\\Program Files\\Microsoft Visual Studio 8\\VC\\INCLUDE\\crtdefs.h"
#pragma pack ( push, 8 )
#line 32 "C:\\Program Files\\Microsoft Visual Studio 8\\VC\\INCLUDE\\vadefs.h"
#pragma pack ( push, 8 )
#line 52 "C:\\Program Files\\Microsoft Visual Studio 8\\VC\\INCLUDE\\vadefs.h"
extern "C" { typedef unsigned __w64 uintptr_t; }
#line 61 "C:\\Program Files\\Microsoft Visual Studio 8\\VC\\INCLUDE\\vadefs.h"
extern "C" { typedef char *va_list; }
#line 151 "C:\\Program Files\\Microsoft Visual Studio 8\\VC\\INCLUDE\\vadefs.h"
#pragma pack ( pop )
#line 482 "C:\\Program Files\\Microsoft Visual Studio 8\\VC\\INCLUDE\\crtdefs.h"
typedef unsigned size_t; 
#line 1 "C:\\CUDA\\include\\crt/host_runtime.h"

























































#line 59 "C:\\CUDA\\include\\crt/host_runtime.h"






#line 66 "C:\\CUDA\\include\\crt/host_runtime.h"



#line 70 "C:\\CUDA\\include\\crt/host_runtime.h"
























#line 95 "C:\\CUDA\\include\\crt/host_runtime.h"










#line 106 "C:\\CUDA\\include\\crt/host_runtime.h"

#line 1 "C:\\CUDA\\include\\cuda_runtime_api.h"




















































#line 1 "c:\\cuda\\include\\host_defines.h"










































#line 44 "c:\\cuda\\include\\host_defines.h"

















#line 62 "c:\\cuda\\include\\host_defines.h"










#line 73 "c:\\cuda\\include\\host_defines.h"






















#line 96 "c:\\cuda\\include\\host_defines.h"






#line 103 "c:\\cuda\\include\\host_defines.h"



#line 107 "c:\\cuda\\include\\host_defines.h"






#line 115 "c:\\cuda\\include\\host_defines.h"



#line 119 "c:\\cuda\\include\\host_defines.h"














#line 134 "c:\\cuda\\include\\host_defines.h"
#line 54 "C:\\CUDA\\include\\cuda_runtime_api.h"
#line 1 "c:\\cuda\\include\\builtin_types.h"









































#line 1 "c:\\cuda\\include\\device_types.h"













































enum cudaRoundMode
{
  cudaRoundNearest,
  cudaRoundZero,
  cudaRoundPosInf,
  cudaRoundMinInf
};

#line 55 "c:\\cuda\\include\\device_types.h"
#line 43 "c:\\cuda\\include\\builtin_types.h"
#line 1 "c:\\cuda\\include\\driver_types.h"














































































#line 80 "c:\\cuda\\include\\driver_types.h"











enum cudaError
{
  cudaSuccess                           =      0,   
  cudaErrorMissingConfiguration         =      1,   
  cudaErrorMemoryAllocation             =      2,   
  cudaErrorInitializationError          =      3,   
  cudaErrorLaunchFailure                =      4,   
  cudaErrorPriorLaunchFailure           =      5,   
  cudaErrorLaunchTimeout                =      6,   
  cudaErrorLaunchOutOfResources         =      7,   
  cudaErrorInvalidDeviceFunction        =      8,   
  cudaErrorInvalidConfiguration         =      9,   
  cudaErrorInvalidDevice                =     10,   
  cudaErrorInvalidValue                 =     11,   
  cudaErrorInvalidPitchValue            =     12,   
  cudaErrorInvalidSymbol                =     13,   
  cudaErrorMapBufferObjectFailed        =     14,   
  cudaErrorUnmapBufferObjectFailed      =     15,   
  cudaErrorInvalidHostPointer           =     16,   
  cudaErrorInvalidDevicePointer         =     17,   
  cudaErrorInvalidTexture               =     18,   
  cudaErrorInvalidTextureBinding        =     19,   
  cudaErrorInvalidChannelDescriptor     =     20,   
  cudaErrorInvalidMemcpyDirection       =     21,   
  cudaErrorAddressOfConstant            =     22,   
  cudaErrorTextureFetchFailed           =     23,   
  cudaErrorTextureNotBound              =     24,   
  cudaErrorSynchronizationError         =     25,   
  cudaErrorInvalidFilterSetting         =     26,   
  cudaErrorInvalidNormSetting           =     27,   
  cudaErrorMixedDeviceExecution         =     28,   
  cudaErrorCudartUnloading              =     29,   
  cudaErrorUnknown                      =     30,   
  cudaErrorNotYetImplemented            =     31,   
  cudaErrorMemoryValueTooLarge          =     32,   
  cudaErrorInvalidResourceHandle        =     33,   
  cudaErrorNotReady                     =     34,   
  cudaErrorInsufficientDriver           =     35,   
  cudaErrorSetOnActiveProcess           =     36,   
  cudaErrorNoDevice                     =     38,   
  cudaErrorStartupFailure               =   0x7f,   
  cudaErrorApiFailureBase               =  10000    
};





enum cudaChannelFormatKind
{
  cudaChannelFormatKindSigned           =   0,      
  cudaChannelFormatKindUnsigned         =   1,      
  cudaChannelFormatKindFloat            =   2,      
  cudaChannelFormatKindNone             =   3       
};





struct cudaChannelFormatDesc
{
  int                        x; 
  int                        y; 
  int                        z; 
  int                        w; 
  enum cudaChannelFormatKind f; 
};





struct cudaArray;





enum cudaMemcpyKind
{
  cudaMemcpyHostToHost          =   0,      
  cudaMemcpyHostToDevice        =   1,      
  cudaMemcpyDeviceToHost        =   2,      
  cudaMemcpyDeviceToDevice      =   3       
};





struct cudaPitchedPtr
{
  void   *ptr;      
  size_t  pitch;    
  size_t  xsize;    
  size_t  ysize;    
};





struct cudaExtent
{
  size_t width;     
  size_t height;    
  size_t depth;     
};





struct cudaPos
{
  size_t x;     
  size_t y;     
  size_t z;     
};





struct cudaMemcpy3DParms
{
  struct cudaArray      *srcArray;  
  struct cudaPos         srcPos;    
  struct cudaPitchedPtr  srcPtr;    

  struct cudaArray      *dstArray;  
  struct cudaPos         dstPos;    
  struct cudaPitchedPtr  dstPtr;    

  struct cudaExtent      extent;    
  enum cudaMemcpyKind    kind;      
};





struct cudaFuncAttributes
{
   size_t sharedSizeBytes;  
   size_t constSizeBytes;   
   size_t localSizeBytes;   
   int maxThreadsPerBlock;  
   int numRegs;             
   int __cudaReserved[8];
};





enum cudaComputeMode
{
  cudaComputeModeDefault    =   0,  
  cudaComputeModeExclusive  =   1,  
  cudaComputeModeProhibited =   2   
};






struct cudaDeviceProp
{
  char   name[256];                 
  size_t totalGlobalMem;            
  size_t sharedMemPerBlock;         
  int    regsPerBlock;              
  int    warpSize;                  
  size_t memPitch;                  
  int    maxThreadsPerBlock;        
  int    maxThreadsDim[3];          
  int    maxGridSize[3];            
  int    clockRate;                 
  size_t totalConstMem;             
  int    major;                     
  int    minor;                     
  size_t textureAlignment;          
  int    deviceOverlap;             
  int    multiProcessorCount;       
  int    kernelExecTimeoutEnabled;  
  int    integrated;                
  int    canMapHostMemory;          
  int    computeMode;               
  int    __cudaReserved[36];
};



































typedef enum cudaError cudaError_t;





typedef int cudaStream_t;





typedef int cudaEvent_t;


 

#line 337 "c:\\cuda\\include\\driver_types.h"
#line 44 "c:\\cuda\\include\\builtin_types.h"
#line 1 "c:\\cuda\\include\\texture_types.h"












































#line 1 "c:\\cuda\\include\\driver_types.h"















































































































































































































































































































































#line 337 "c:\\cuda\\include\\driver_types.h"
#line 46 "c:\\cuda\\include\\texture_types.h"








enum cudaTextureAddressMode
{
  cudaAddressModeWrap,
  cudaAddressModeClamp
};


enum cudaTextureFilterMode
{
  cudaFilterModePoint,
  cudaFilterModeLinear
};


enum cudaTextureReadMode
{
  cudaReadModeElementType,
  cudaReadModeNormalizedFloat
};


struct textureReference
{
  int                          normalized;
  enum cudaTextureFilterMode   filterMode;
  enum cudaTextureAddressMode  addressMode[3];
  struct cudaChannelFormatDesc channelDesc;
  int                          __cudaReserved[16];
};

#line 85 "c:\\cuda\\include\\texture_types.h"
#line 45 "c:\\cuda\\include\\builtin_types.h"
#line 1 "c:\\cuda\\include\\vector_types.h"












































#line 1 "c:\\cuda\\include\\host_defines.h"




































































































































#line 134 "c:\\cuda\\include\\host_defines.h"
#line 46 "c:\\cuda\\include\\vector_types.h"











#line 58 "c:\\cuda\\include\\vector_types.h"




















#line 80 "c:\\cuda\\include\\vector_types.h"


struct char1
{
  signed char x;
  
};


struct uchar1 
{
  unsigned char x;
  
};


struct __declspec(align(2)) char2
{
  signed char x, y;
  
};


struct __declspec(align(2)) uchar2
{
  unsigned char x, y;
  
};


struct char3
{
  signed char x, y, z;
  
};


struct uchar3
{
  unsigned char x, y, z;
  
};


struct __declspec(align(4)) char4
{
  signed char x, y, z, w;
  
};


struct __declspec(align(4)) uchar4
{
  unsigned char x, y, z, w;
  
};


struct short1
{
  short x;
  
};


struct ushort1
{
  unsigned short x;
  
};


struct __declspec(align(4)) short2
{
  short x, y;
  
};


struct __declspec(align(4)) ushort2
{
  unsigned short x, y;
  
};


struct short3
{
  short x, y, z;
  
};


struct ushort3
{
  unsigned short x, y, z;
  
};


struct short4 { union { struct { short x, y, z, w; }; struct { long long int :1,:0; }; };  };


struct ushort4 { union { struct { unsigned short x, y, z, w; }; struct { long long int :1,:0; }; };  };


struct int1
{
  int x;
  
};


struct uint1
{
  unsigned int x;
  
};


struct int2 { union { struct { int x, y; }; struct { long long int :1,:0; }; };  };


struct uint2 { union { struct { unsigned int x, y; }; struct { long long int :1,:0; }; };  };


struct int3
{
  int x, y, z;
  
};


struct uint3
{
  unsigned int x, y, z;
  
};


struct  int4
{
  int x, y, z, w;
  
};


struct  uint4
{
  unsigned int x, y, z, w;
  
};


struct long1
{
  long int x;
  
};


struct ulong1
{
  unsigned long x;
  
};




struct long2 { union { struct { long int x, y; }; struct { long long int :1,:0; }; };  };


struct ulong2 { union { struct { unsigned long int x, y; }; struct { long long int :1,:0; }; };  };

















#line 272 "c:\\cuda\\include\\vector_types.h"




struct long3
{
  long int x, y, z;
  
};


struct ulong3
{
  unsigned long int x, y, z;
  
};


struct  long4
{
  long int x, y, z, w;
  
};


struct  ulong4
{
  unsigned long int x, y, z, w;
  
};

#line 304 "c:\\cuda\\include\\vector_types.h"


struct float1
{
  float x;
  
};


struct float2 { union { struct { float x, y; }; struct { long long int :1,:0; }; };  };


struct float3
{
  float x, y, z;
  
};


struct  float4
{
  float x, y, z, w;
  
};


struct longlong1
{
  long long int x;
  
};


struct ulonglong1
{
  unsigned long long int x;
  
};


struct  longlong2
{
  long long int x, y;
  
};


struct  ulonglong2
{
  unsigned long long int x, y;
  
};


struct double1
{
  double x;
  
};


struct  double2
{
  double x, y;
  
};








typedef struct char1 char1;

typedef struct uchar1 uchar1;

typedef struct char2 char2;

typedef struct uchar2 uchar2;

typedef struct char3 char3;

typedef struct uchar3 uchar3;

typedef struct char4 char4;

typedef struct uchar4 uchar4;

typedef struct short1 short1;

typedef struct ushort1 ushort1;

typedef struct short2 short2;

typedef struct ushort2 ushort2;

typedef struct short3 short3;

typedef struct ushort3 ushort3;

typedef struct short4 short4;

typedef struct ushort4 ushort4;

typedef struct int1 int1;

typedef struct uint1 uint1;

typedef struct int2 int2;

typedef struct uint2 uint2;

typedef struct int3 int3;

typedef struct uint3 uint3;

typedef struct int4 int4;

typedef struct uint4 uint4;

typedef struct long1 long1;

typedef struct ulong1 ulong1;

typedef struct long2 long2;

typedef struct ulong2 ulong2;

typedef struct long3 long3;

typedef struct ulong3 ulong3;

typedef struct long4 long4;

typedef struct ulong4 ulong4;

typedef struct float1 float1;

typedef struct float2 float2;

typedef struct float3 float3;

typedef struct float4 float4;

typedef struct longlong1 longlong1;

typedef struct ulonglong1 ulonglong1;

typedef struct longlong2 longlong2;

typedef struct ulonglong2 ulonglong2;

typedef struct double1 double1;

typedef struct double2 double2;








struct dim3
{
    unsigned int x, y, z;

      dim3(unsigned int x = 1, unsigned int y = 1, unsigned int z = 1) : x(x), y(y), z(z) {}
      dim3(uint3 v) : x(v.x), y(v.y), z(v.z) {}
      operator uint3(void) { uint3 t; t.x = x; t.y = y; t.z = z; return t; }
#line 477 "c:\\cuda\\include\\vector_types.h"
};


typedef struct dim3 dim3;




#line 486 "c:\\cuda\\include\\vector_types.h"
#line 46 "c:\\cuda\\include\\builtin_types.h"
#line 55 "C:\\CUDA\\include\\cuda_runtime_api.h"















#line 71 "C:\\CUDA\\include\\cuda_runtime_api.h"









extern "C" {
#line 82 "C:\\CUDA\\include\\cuda_runtime_api.h"







extern  cudaError_t __stdcall cudaMalloc3D(struct cudaPitchedPtr* pitchedDevPtr, struct cudaExtent extent);
extern  cudaError_t __stdcall cudaMalloc3DArray(struct cudaArray** arrayPtr, const struct cudaChannelFormatDesc* desc, struct cudaExtent extent);
extern  cudaError_t __stdcall cudaMemset3D(struct cudaPitchedPtr pitchedDevPtr, int value, struct cudaExtent extent);
extern  cudaError_t __stdcall cudaMemcpy3D(const struct cudaMemcpy3DParms *p);
extern  cudaError_t __stdcall cudaMemcpy3DAsync(const struct cudaMemcpy3DParms *p, cudaStream_t stream);








extern  cudaError_t __stdcall cudaMalloc(void **devPtr, size_t size);
extern  cudaError_t __stdcall cudaMallocHost(void **ptr, size_t size);
extern  cudaError_t __stdcall cudaMallocPitch(void **devPtr, size_t *pitch, size_t width, size_t height);
extern  cudaError_t __stdcall cudaMallocArray(struct cudaArray **array, const struct cudaChannelFormatDesc *desc, size_t width, size_t height );
extern  cudaError_t __stdcall cudaFree(void *devPtr);
extern  cudaError_t __stdcall cudaFreeHost(void *ptr);
extern  cudaError_t __stdcall cudaFreeArray(struct cudaArray *array);

extern  cudaError_t __stdcall cudaHostAlloc(void **pHost, size_t bytes, unsigned int flags);
extern  cudaError_t __stdcall cudaHostGetDevicePointer(void **pDevice, void *pHost, unsigned int flags);
extern  cudaError_t __stdcall cudaHostGetFlags(unsigned int *pFlags, void *pHost);








extern  cudaError_t __stdcall cudaMemcpy(void *dst, const void *src, size_t count, enum cudaMemcpyKind kind);
extern  cudaError_t __stdcall cudaMemcpyToArray(struct cudaArray *dst, size_t wOffset, size_t hOffset, const void *src, size_t count, enum cudaMemcpyKind kind);
extern  cudaError_t __stdcall cudaMemcpyFromArray(void *dst, const struct cudaArray *src, size_t wOffset, size_t hOffset, size_t count, enum cudaMemcpyKind kind);
extern  cudaError_t __stdcall cudaMemcpyArrayToArray(struct cudaArray *dst, size_t wOffsetDst, size_t hOffsetDst, const struct cudaArray *src, size_t wOffsetSrc, size_t hOffsetSrc, size_t count, enum cudaMemcpyKind kind );
extern  cudaError_t __stdcall cudaMemcpy2D(void *dst, size_t dpitch, const void *src, size_t spitch, size_t width, size_t height, enum cudaMemcpyKind kind);
extern  cudaError_t __stdcall cudaMemcpy2DToArray(struct cudaArray *dst, size_t wOffset, size_t hOffset, const void *src, size_t spitch, size_t width, size_t height, enum cudaMemcpyKind kind);
extern  cudaError_t __stdcall cudaMemcpy2DFromArray(void *dst, size_t dpitch, const struct cudaArray *src, size_t wOffset, size_t hOffset, size_t width, size_t height, enum cudaMemcpyKind kind);
extern  cudaError_t __stdcall cudaMemcpy2DArrayToArray(struct cudaArray *dst, size_t wOffsetDst, size_t hOffsetDst, const struct cudaArray *src, size_t wOffsetSrc, size_t hOffsetSrc, size_t width, size_t height, enum cudaMemcpyKind kind );
extern  cudaError_t __stdcall cudaMemcpyToSymbol(const char *symbol, const void *src, size_t count, size_t offset , enum cudaMemcpyKind kind );
extern  cudaError_t __stdcall cudaMemcpyFromSymbol(void *dst, const char *symbol, size_t count, size_t offset , enum cudaMemcpyKind kind );







extern  cudaError_t __stdcall cudaMemcpyAsync(void *dst, const void *src, size_t count, enum cudaMemcpyKind kind, cudaStream_t stream);
extern  cudaError_t __stdcall cudaMemcpyToArrayAsync(struct cudaArray *dst, size_t wOffset, size_t hOffset, const void *src, size_t count, enum cudaMemcpyKind kind, cudaStream_t stream);
extern  cudaError_t __stdcall cudaMemcpyFromArrayAsync(void *dst, const struct cudaArray *src, size_t wOffset, size_t hOffset, size_t count, enum cudaMemcpyKind kind, cudaStream_t stream);
extern  cudaError_t __stdcall cudaMemcpy2DAsync(void *dst, size_t dpitch, const void *src, size_t spitch, size_t width, size_t height, enum cudaMemcpyKind kind, cudaStream_t stream);
extern  cudaError_t __stdcall cudaMemcpy2DToArrayAsync(struct cudaArray *dst, size_t wOffset, size_t hOffset, const void *src, size_t spitch, size_t width, size_t height, enum cudaMemcpyKind kind, cudaStream_t stream);
extern  cudaError_t __stdcall cudaMemcpy2DFromArrayAsync(void *dst, size_t dpitch, const struct cudaArray *src, size_t wOffset, size_t hOffset, size_t width, size_t height, enum cudaMemcpyKind kind, cudaStream_t stream);
extern  cudaError_t __stdcall cudaMemcpyToSymbolAsync(const char *symbol, const void *src, size_t count, size_t offset, enum cudaMemcpyKind kind, cudaStream_t stream);
extern  cudaError_t __stdcall cudaMemcpyFromSymbolAsync(void *dst, const char *symbol, size_t count, size_t offset, enum cudaMemcpyKind kind, cudaStream_t stream);







extern  cudaError_t __stdcall cudaMemset(void *devPtr, int value, size_t count);
extern  cudaError_t __stdcall cudaMemset2D(void *devPtr, size_t pitch, int value, size_t width, size_t height);







extern  cudaError_t __stdcall cudaGetSymbolAddress(void **devPtr, const char *symbol);
extern  cudaError_t __stdcall cudaGetSymbolSize(size_t *size, const char *symbol);







extern  cudaError_t __stdcall cudaGetDeviceCount(int *count);
extern  cudaError_t __stdcall cudaGetDeviceProperties(struct cudaDeviceProp *prop, int device);
extern  cudaError_t __stdcall cudaChooseDevice(int *device, const struct cudaDeviceProp *prop);
extern  cudaError_t __stdcall cudaSetDevice(int device);
extern  cudaError_t __stdcall cudaGetDevice(int *device);
extern  cudaError_t __stdcall cudaSetValidDevices(int *device_arr, int len);
extern  cudaError_t __stdcall cudaSetDeviceFlags( int flags );







extern  cudaError_t __stdcall cudaBindTexture(size_t *offset, const struct textureReference *texref, const void *devPtr, const struct cudaChannelFormatDesc *desc, size_t size );
extern  cudaError_t __stdcall cudaBindTexture2D(size_t *offset,const struct textureReference *texref,const void *devPtr, const struct cudaChannelFormatDesc *desc,size_t width, size_t height, size_t pitch);
extern  cudaError_t __stdcall cudaBindTextureToArray(const struct textureReference *texref, const struct cudaArray *array, const struct cudaChannelFormatDesc *desc);
extern  cudaError_t __stdcall cudaUnbindTexture(const struct textureReference *texref);
extern  cudaError_t __stdcall cudaGetTextureAlignmentOffset(size_t *offset, const struct textureReference *texref);
extern  cudaError_t __stdcall cudaGetTextureReference(const struct textureReference **texref, const char *symbol);







extern  cudaError_t __stdcall cudaGetChannelDesc(struct cudaChannelFormatDesc *desc, const struct cudaArray *array);
extern  struct cudaChannelFormatDesc __stdcall cudaCreateChannelDesc(int x, int y, int z, int w, enum cudaChannelFormatKind f);







extern  cudaError_t __stdcall cudaGetLastError(void);
extern  const char* __stdcall cudaGetErrorString(cudaError_t error);







extern  cudaError_t __stdcall cudaConfigureCall(dim3 gridDim, dim3 blockDim, size_t sharedMem , cudaStream_t stream );
extern  cudaError_t __stdcall cudaSetupArgument(const void *arg, size_t size, size_t offset);
extern  cudaError_t __stdcall cudaLaunch(const char *entry);
extern  cudaError_t __stdcall cudaFuncGetAttributes(struct cudaFuncAttributes *attr, const char *func);







extern  cudaError_t __stdcall cudaStreamCreate(cudaStream_t *pStream);
extern  cudaError_t __stdcall cudaStreamDestroy(cudaStream_t stream);
extern  cudaError_t __stdcall cudaStreamSynchronize(cudaStream_t stream);
extern  cudaError_t __stdcall cudaStreamQuery(cudaStream_t stream);







extern  cudaError_t __stdcall cudaEventCreate(cudaEvent_t *event);
extern  cudaError_t __stdcall cudaEventCreateWithFlags(cudaEvent_t *event, int flags);
extern  cudaError_t __stdcall cudaEventRecord(cudaEvent_t event, cudaStream_t stream);
extern  cudaError_t __stdcall cudaEventQuery(cudaEvent_t event);
extern  cudaError_t __stdcall cudaEventSynchronize(cudaEvent_t event);
extern  cudaError_t __stdcall cudaEventDestroy(cudaEvent_t event);
extern  cudaError_t __stdcall cudaEventElapsedTime(float *ms, cudaEvent_t start, cudaEvent_t end);







extern  cudaError_t __stdcall cudaSetDoubleForDevice(double *d);
extern  cudaError_t __stdcall cudaSetDoubleForHost(double *d);







extern  cudaError_t __stdcall cudaThreadExit(void);
extern  cudaError_t __stdcall cudaThreadSynchronize(void);







extern  cudaError_t __stdcall cudaDriverGetVersion(int *driverVersion);
extern  cudaError_t __stdcall cudaRuntimeGetVersion(int *runtimeVersion);


}
#line 276 "C:\\CUDA\\include\\cuda_runtime_api.h"



#line 280 "C:\\CUDA\\include\\cuda_runtime_api.h"
#line 108 "C:\\CUDA\\include\\crt/host_runtime.h"
#line 1 "c:\\cuda\\include\\crt\\storage_class.h"










































#line 44 "c:\\cuda\\include\\crt\\storage_class.h"






#line 51 "c:\\cuda\\include\\crt\\storage_class.h"



#line 55 "c:\\cuda\\include\\crt\\storage_class.h"



#line 59 "c:\\cuda\\include\\crt\\storage_class.h"



#line 63 "c:\\cuda\\include\\crt\\storage_class.h"



#line 67 "c:\\cuda\\include\\crt\\storage_class.h"



#line 71 "c:\\cuda\\include\\crt\\storage_class.h"



#line 75 "c:\\cuda\\include\\crt\\storage_class.h"



#line 79 "c:\\cuda\\include\\crt\\storage_class.h"



#line 83 "c:\\cuda\\include\\crt\\storage_class.h"



#line 87 "c:\\cuda\\include\\crt\\storage_class.h"



#line 91 "c:\\cuda\\include\\crt\\storage_class.h"



#line 95 "c:\\cuda\\include\\crt\\storage_class.h"



#line 99 "c:\\cuda\\include\\crt\\storage_class.h"



#line 103 "c:\\cuda\\include\\crt\\storage_class.h"



#line 107 "c:\\cuda\\include\\crt\\storage_class.h"



#line 111 "c:\\cuda\\include\\crt\\storage_class.h"



#line 115 "c:\\cuda\\include\\crt\\storage_class.h"



#line 119 "c:\\cuda\\include\\crt\\storage_class.h"



#line 123 "c:\\cuda\\include\\crt\\storage_class.h"



#line 127 "c:\\cuda\\include\\crt\\storage_class.h"



#line 131 "c:\\cuda\\include\\crt\\storage_class.h"



#line 135 "c:\\cuda\\include\\crt\\storage_class.h"



#line 139 "c:\\cuda\\include\\crt\\storage_class.h"



#line 143 "c:\\cuda\\include\\crt\\storage_class.h"



#line 147 "c:\\cuda\\include\\crt\\storage_class.h"

#line 149 "c:\\cuda\\include\\crt\\storage_class.h"
#line 109 "C:\\CUDA\\include\\crt/host_runtime.h"































































































































































































#line 301 "C:\\CUDA\\include\\crt/host_runtime.h"
#line 484 "C:\\Program Files\\Microsoft Visual Studio 8\\VC\\INCLUDE\\crtdefs.h"
#line 489 "C:\\Program Files\\Microsoft Visual Studio 8\\VC\\INCLUDE\\crtdefs.h"
extern "C" { typedef size_t rsize_t; }
#line 498 "C:\\Program Files\\Microsoft Visual Studio 8\\VC\\INCLUDE\\crtdefs.h"
extern "C" { typedef int __w64 intptr_t; }
#line 516 "C:\\Program Files\\Microsoft Visual Studio 8\\VC\\INCLUDE\\crtdefs.h"
extern "C" { typedef int __w64 ptrdiff_t; }
#line 527 "C:\\Program Files\\Microsoft Visual Studio 8\\VC\\INCLUDE\\crtdefs.h"
extern "C" { typedef unsigned short wint_t; }
extern "C" { typedef unsigned short wctype_t; }
#line 556 "C:\\Program Files\\Microsoft Visual Studio 8\\VC\\INCLUDE\\crtdefs.h"
extern "C" { typedef int errcode; }
#line 561 "C:\\Program Files\\Microsoft Visual Studio 8\\VC\\INCLUDE\\crtdefs.h"
extern "C" { typedef int errno_t; }
#line 565 "C:\\Program Files\\Microsoft Visual Studio 8\\VC\\INCLUDE\\crtdefs.h"
extern "C" { typedef long __w64 __time32_t; }
#line 571 "C:\\Program Files\\Microsoft Visual Studio 8\\VC\\INCLUDE\\crtdefs.h"
extern "C" { typedef __int64 __time64_t; }
#line 580 "C:\\Program Files\\Microsoft Visual Studio 8\\VC\\INCLUDE\\crtdefs.h"
extern "C" { typedef __time64_t time_t; }
#line 2035 "C:\\Program Files\\Microsoft Visual Studio 8\\VC\\INCLUDE\\crtdefs.h"
struct threadlocaleinfostruct; 
struct threadmbcinfostruct; 
extern "C" { typedef threadlocaleinfostruct *pthreadlocinfo; }
extern "C" { typedef threadmbcinfostruct *pthreadmbcinfo; }
struct __lc_time_data; 
#line 2045 "C:\\Program Files\\Microsoft Visual Studio 8\\VC\\INCLUDE\\crtdefs.h"
extern "C" { typedef 
#line 2041 "C:\\Program Files\\Microsoft Visual Studio 8\\VC\\INCLUDE\\crtdefs.h"
struct localeinfo_struct { 

pthreadlocinfo locinfo; 
pthreadmbcinfo mbcinfo; 
} _locale_tstruct; }extern "C" { typedef localeinfo_struct *_locale_t; }
#line 2052 "C:\\Program Files\\Microsoft Visual Studio 8\\VC\\INCLUDE\\crtdefs.h"
extern "C" { typedef 
#line 2048 "C:\\Program Files\\Microsoft Visual Studio 8\\VC\\INCLUDE\\crtdefs.h"
struct tagLC_ID { 
unsigned short wLanguage; 
unsigned short wCountry; 
unsigned short wCodePage; 
} LC_ID; }extern "C" { typedef tagLC_ID *LPLC_ID; }
#line 2081 "C:\\Program Files\\Microsoft Visual Studio 8\\VC\\INCLUDE\\crtdefs.h"
extern "C" { typedef 
#line 2057 "C:\\Program Files\\Microsoft Visual Studio 8\\VC\\INCLUDE\\crtdefs.h"
struct threadlocaleinfostruct { 
int refcount; 
unsigned lc_codepage; 
unsigned lc_collate_cp; 
unsigned long lc_handle[6]; 
LC_ID lc_id[6]; 
struct { 
char *locale; 
__wchar_t *wlocale; 
int *refcount; 
int *wrefcount; 
} lc_category[6]; 
int lc_clike; 
int mb_cur_max; 
int *lconv_intl_refcount; 
int *lconv_num_refcount; 
int *lconv_mon_refcount; 
struct lconv *lconv; 
int *ctype1_refcount; 
unsigned short *ctype1; 
const unsigned short *pctype; 
const unsigned char *pclmap; 
const unsigned char *pcumap; 
__lc_time_data *lc_time_curr; 
} threadlocinfo; }
#line 2119 "C:\\Program Files\\Microsoft Visual Studio 8\\VC\\INCLUDE\\crtdefs.h"
#pragma pack ( pop )
#line 46 "c:\\cuda\\include\\device_types.h"









#line 56 "c:\\cuda\\include\\device_types.h"
#line 41 "C:\\Program Files\\Microsoft Visual Studio 8\\VC\\INCLUDE\\stddef.h"
extern "C" { extern __declspec( dllimport ) int *__cdecl _errno(); } 


extern "C" { extern errno_t __cdecl _set_errno(int); } 
extern "C" { extern errno_t __cdecl _get_errno(int *); } 
#line 68 "C:\\Program Files\\Microsoft Visual Studio 8\\VC\\INCLUDE\\stddef.h"
extern "C" { extern __declspec( dllimport ) unsigned long __cdecl __threadid(); } 

extern "C" { extern __declspec( dllimport ) uintptr_t __cdecl __threadhandle(); } 
#line 91 "c:\\cuda\\include\\driver_types.h"













































#line 137 "c:\\cuda\\include\\driver_types.h"
#line 139 "c:\\cuda\\include\\driver_types.h"









#line 149 "c:\\cuda\\include\\driver_types.h"
#line 151 "c:\\cuda\\include\\driver_types.h"










#line 162 "c:\\cuda\\include\\driver_types.h"
#line 164 "c:\\cuda\\include\\driver_types.h"


#line 167 "c:\\cuda\\include\\driver_types.h"
#line 170 "c:\\cuda\\include\\driver_types.h"









#line 180 "c:\\cuda\\include\\driver_types.h"
#line 182 "c:\\cuda\\include\\driver_types.h"









#line 192 "c:\\cuda\\include\\driver_types.h"
#line 194 "c:\\cuda\\include\\driver_types.h"








#line 203 "c:\\cuda\\include\\driver_types.h"
#line 205 "c:\\cuda\\include\\driver_types.h"








#line 214 "c:\\cuda\\include\\driver_types.h"
#line 216 "c:\\cuda\\include\\driver_types.h"















#line 232 "c:\\cuda\\include\\driver_types.h"
#line 234 "c:\\cuda\\include\\driver_types.h"











#line 246 "c:\\cuda\\include\\driver_types.h"
#line 248 "c:\\cuda\\include\\driver_types.h"








#line 257 "c:\\cuda\\include\\driver_types.h"
#line 260 "c:\\cuda\\include\\driver_types.h"


























#line 287 "c:\\cuda\\include\\driver_types.h"
#line 319 "c:\\cuda\\include\\driver_types.h"




#line 324 "c:\\cuda\\include\\driver_types.h"
#line 325 "c:\\cuda\\include\\driver_types.h"




#line 330 "c:\\cuda\\include\\driver_types.h"
#line 331 "c:\\cuda\\include\\driver_types.h"




#line 336 "c:\\cuda\\include\\driver_types.h"
#line 54 "c:\\cuda\\include\\texture_types.h"







#line 62 "c:\\cuda\\include\\texture_types.h"
#line 61 "c:\\cuda\\include\\texture_types.h"







#line 69 "c:\\cuda\\include\\texture_types.h"
#line 68 "c:\\cuda\\include\\texture_types.h"







#line 76 "c:\\cuda\\include\\texture_types.h"
#line 75 "c:\\cuda\\include\\texture_types.h"










#line 86 "c:\\cuda\\include\\texture_types.h"
#line 82 "c:\\cuda\\include\\vector_types.h"







#line 90 "c:\\cuda\\include\\vector_types.h"
#line 89 "c:\\cuda\\include\\vector_types.h"







#line 97 "c:\\cuda\\include\\vector_types.h"
#line 96 "c:\\cuda\\include\\vector_types.h"







#line 104 "c:\\cuda\\include\\vector_types.h"
#line 103 "c:\\cuda\\include\\vector_types.h"







#line 111 "c:\\cuda\\include\\vector_types.h"
#line 110 "c:\\cuda\\include\\vector_types.h"







#line 118 "c:\\cuda\\include\\vector_types.h"
#line 117 "c:\\cuda\\include\\vector_types.h"







#line 125 "c:\\cuda\\include\\vector_types.h"
#line 124 "c:\\cuda\\include\\vector_types.h"







#line 132 "c:\\cuda\\include\\vector_types.h"
#line 131 "c:\\cuda\\include\\vector_types.h"







#line 139 "c:\\cuda\\include\\vector_types.h"
#line 138 "c:\\cuda\\include\\vector_types.h"







#line 146 "c:\\cuda\\include\\vector_types.h"
#line 145 "c:\\cuda\\include\\vector_types.h"







#line 153 "c:\\cuda\\include\\vector_types.h"
#line 152 "c:\\cuda\\include\\vector_types.h"







#line 160 "c:\\cuda\\include\\vector_types.h"
#line 159 "c:\\cuda\\include\\vector_types.h"







#line 167 "c:\\cuda\\include\\vector_types.h"
#line 166 "c:\\cuda\\include\\vector_types.h"







#line 174 "c:\\cuda\\include\\vector_types.h"
#line 173 "c:\\cuda\\include\\vector_types.h"







#line 181 "c:\\cuda\\include\\vector_types.h"
#line 180 "c:\\cuda\\include\\vector_types.h"



#line 184 "c:\\cuda\\include\\vector_types.h"
#line 183 "c:\\cuda\\include\\vector_types.h"



#line 187 "c:\\cuda\\include\\vector_types.h"
#line 186 "c:\\cuda\\include\\vector_types.h"







#line 194 "c:\\cuda\\include\\vector_types.h"
#line 193 "c:\\cuda\\include\\vector_types.h"







#line 201 "c:\\cuda\\include\\vector_types.h"
#line 200 "c:\\cuda\\include\\vector_types.h"



#line 204 "c:\\cuda\\include\\vector_types.h"
#line 203 "c:\\cuda\\include\\vector_types.h"



#line 207 "c:\\cuda\\include\\vector_types.h"
#line 206 "c:\\cuda\\include\\vector_types.h"







#line 214 "c:\\cuda\\include\\vector_types.h"
#line 213 "c:\\cuda\\include\\vector_types.h"







#line 221 "c:\\cuda\\include\\vector_types.h"
#line 220 "c:\\cuda\\include\\vector_types.h"







#line 228 "c:\\cuda\\include\\vector_types.h"
#line 227 "c:\\cuda\\include\\vector_types.h"







#line 235 "c:\\cuda\\include\\vector_types.h"
#line 234 "c:\\cuda\\include\\vector_types.h"







#line 242 "c:\\cuda\\include\\vector_types.h"
#line 241 "c:\\cuda\\include\\vector_types.h"







#line 249 "c:\\cuda\\include\\vector_types.h"
#line 250 "c:\\cuda\\include\\vector_types.h"



#line 254 "c:\\cuda\\include\\vector_types.h"
#line 253 "c:\\cuda\\include\\vector_types.h"



#line 257 "c:\\cuda\\include\\vector_types.h"
#line 276 "c:\\cuda\\include\\vector_types.h"







#line 284 "c:\\cuda\\include\\vector_types.h"
#line 283 "c:\\cuda\\include\\vector_types.h"







#line 291 "c:\\cuda\\include\\vector_types.h"
#line 290 "c:\\cuda\\include\\vector_types.h"







#line 298 "c:\\cuda\\include\\vector_types.h"
#line 297 "c:\\cuda\\include\\vector_types.h"







#line 305 "c:\\cuda\\include\\vector_types.h"
#line 306 "c:\\cuda\\include\\vector_types.h"







#line 314 "c:\\cuda\\include\\vector_types.h"
#line 313 "c:\\cuda\\include\\vector_types.h"



#line 317 "c:\\cuda\\include\\vector_types.h"
#line 316 "c:\\cuda\\include\\vector_types.h"







#line 324 "c:\\cuda\\include\\vector_types.h"
#line 323 "c:\\cuda\\include\\vector_types.h"







#line 331 "c:\\cuda\\include\\vector_types.h"
#line 330 "c:\\cuda\\include\\vector_types.h"







#line 338 "c:\\cuda\\include\\vector_types.h"
#line 337 "c:\\cuda\\include\\vector_types.h"







#line 345 "c:\\cuda\\include\\vector_types.h"
#line 344 "c:\\cuda\\include\\vector_types.h"







#line 352 "c:\\cuda\\include\\vector_types.h"
#line 351 "c:\\cuda\\include\\vector_types.h"







#line 359 "c:\\cuda\\include\\vector_types.h"
#line 358 "c:\\cuda\\include\\vector_types.h"







#line 366 "c:\\cuda\\include\\vector_types.h"
#line 365 "c:\\cuda\\include\\vector_types.h"







#line 373 "c:\\cuda\\include\\vector_types.h"
#line 378 "c:\\cuda\\include\\vector_types.h"




#line 383 "c:\\cuda\\include\\vector_types.h"
#line 380 "c:\\cuda\\include\\vector_types.h"




#line 385 "c:\\cuda\\include\\vector_types.h"
#line 382 "c:\\cuda\\include\\vector_types.h"




#line 387 "c:\\cuda\\include\\vector_types.h"
#line 384 "c:\\cuda\\include\\vector_types.h"




#line 389 "c:\\cuda\\include\\vector_types.h"
#line 386 "c:\\cuda\\include\\vector_types.h"




#line 391 "c:\\cuda\\include\\vector_types.h"
#line 388 "c:\\cuda\\include\\vector_types.h"




#line 393 "c:\\cuda\\include\\vector_types.h"
#line 390 "c:\\cuda\\include\\vector_types.h"




#line 395 "c:\\cuda\\include\\vector_types.h"
#line 392 "c:\\cuda\\include\\vector_types.h"




#line 397 "c:\\cuda\\include\\vector_types.h"
#line 394 "c:\\cuda\\include\\vector_types.h"




#line 399 "c:\\cuda\\include\\vector_types.h"
#line 396 "c:\\cuda\\include\\vector_types.h"




#line 401 "c:\\cuda\\include\\vector_types.h"
#line 398 "c:\\cuda\\include\\vector_types.h"




#line 403 "c:\\cuda\\include\\vector_types.h"
#line 400 "c:\\cuda\\include\\vector_types.h"




#line 405 "c:\\cuda\\include\\vector_types.h"
#line 402 "c:\\cuda\\include\\vector_types.h"




#line 407 "c:\\cuda\\include\\vector_types.h"
#line 404 "c:\\cuda\\include\\vector_types.h"




#line 409 "c:\\cuda\\include\\vector_types.h"
#line 406 "c:\\cuda\\include\\vector_types.h"




#line 411 "c:\\cuda\\include\\vector_types.h"
#line 408 "c:\\cuda\\include\\vector_types.h"




#line 413 "c:\\cuda\\include\\vector_types.h"
#line 410 "c:\\cuda\\include\\vector_types.h"




#line 415 "c:\\cuda\\include\\vector_types.h"
#line 412 "c:\\cuda\\include\\vector_types.h"




#line 417 "c:\\cuda\\include\\vector_types.h"
#line 414 "c:\\cuda\\include\\vector_types.h"




#line 419 "c:\\cuda\\include\\vector_types.h"
#line 416 "c:\\cuda\\include\\vector_types.h"




#line 421 "c:\\cuda\\include\\vector_types.h"
#line 418 "c:\\cuda\\include\\vector_types.h"




#line 423 "c:\\cuda\\include\\vector_types.h"
#line 420 "c:\\cuda\\include\\vector_types.h"




#line 425 "c:\\cuda\\include\\vector_types.h"
#line 422 "c:\\cuda\\include\\vector_types.h"




#line 427 "c:\\cuda\\include\\vector_types.h"
#line 424 "c:\\cuda\\include\\vector_types.h"




#line 429 "c:\\cuda\\include\\vector_types.h"
#line 426 "c:\\cuda\\include\\vector_types.h"




#line 431 "c:\\cuda\\include\\vector_types.h"
#line 428 "c:\\cuda\\include\\vector_types.h"




#line 433 "c:\\cuda\\include\\vector_types.h"
#line 430 "c:\\cuda\\include\\vector_types.h"




#line 435 "c:\\cuda\\include\\vector_types.h"
#line 432 "c:\\cuda\\include\\vector_types.h"




#line 437 "c:\\cuda\\include\\vector_types.h"
#line 434 "c:\\cuda\\include\\vector_types.h"




#line 439 "c:\\cuda\\include\\vector_types.h"
#line 436 "c:\\cuda\\include\\vector_types.h"




#line 441 "c:\\cuda\\include\\vector_types.h"
#line 438 "c:\\cuda\\include\\vector_types.h"




#line 443 "c:\\cuda\\include\\vector_types.h"
#line 440 "c:\\cuda\\include\\vector_types.h"




#line 445 "c:\\cuda\\include\\vector_types.h"
#line 442 "c:\\cuda\\include\\vector_types.h"




#line 447 "c:\\cuda\\include\\vector_types.h"
#line 444 "c:\\cuda\\include\\vector_types.h"




#line 449 "c:\\cuda\\include\\vector_types.h"
#line 446 "c:\\cuda\\include\\vector_types.h"




#line 451 "c:\\cuda\\include\\vector_types.h"
#line 448 "c:\\cuda\\include\\vector_types.h"




#line 453 "c:\\cuda\\include\\vector_types.h"
#line 450 "c:\\cuda\\include\\vector_types.h"




#line 455 "c:\\cuda\\include\\vector_types.h"
#line 452 "c:\\cuda\\include\\vector_types.h"




#line 457 "c:\\cuda\\include\\vector_types.h"
#line 454 "c:\\cuda\\include\\vector_types.h"




#line 459 "c:\\cuda\\include\\vector_types.h"
#line 456 "c:\\cuda\\include\\vector_types.h"




#line 461 "c:\\cuda\\include\\vector_types.h"
#line 458 "c:\\cuda\\include\\vector_types.h"




#line 463 "c:\\cuda\\include\\vector_types.h"
#line 460 "c:\\cuda\\include\\vector_types.h"




#line 465 "c:\\cuda\\include\\vector_types.h"
#line 469 "c:\\cuda\\include\\vector_types.h"







#line 477 "c:\\cuda\\include\\vector_types.h"
#line 480 "c:\\cuda\\include\\vector_types.h"




#line 485 "c:\\cuda\\include\\vector_types.h"
#line 89 "C:\\CUDA\\include\\cuda_runtime_api.h"
extern "C" { extern cudaError_t __stdcall cudaMalloc3D(cudaPitchedPtr *, cudaExtent); } 
extern "C" { extern cudaError_t __stdcall cudaMalloc3DArray(cudaArray **, const cudaChannelFormatDesc *, cudaExtent); } 
extern "C" { extern cudaError_t __stdcall cudaMemset3D(cudaPitchedPtr, int, cudaExtent); } 
extern "C" { extern cudaError_t __stdcall cudaMemcpy3D(const cudaMemcpy3DParms *); } 
extern "C" { extern cudaError_t __stdcall cudaMemcpy3DAsync(const cudaMemcpy3DParms *, cudaStream_t); } 
#line 102 "C:\\CUDA\\include\\cuda_runtime_api.h"
extern "C" { extern cudaError_t __stdcall cudaMalloc(void **, size_t); } 
extern "C" { extern cudaError_t __stdcall cudaMallocHost(void **, size_t); } 
extern "C" { extern cudaError_t __stdcall cudaMallocPitch(void **, size_t *, size_t, size_t); } 
extern "C" { extern cudaError_t __stdcall cudaMallocArray(cudaArray **, const cudaChannelFormatDesc *, size_t, size_t = (1)); } 
extern "C" { extern cudaError_t __stdcall cudaFree(void *); } 
extern "C" { extern cudaError_t __stdcall cudaFreeHost(void *); } 
extern "C" { extern cudaError_t __stdcall cudaFreeArray(cudaArray *); } 

extern "C" { extern cudaError_t __stdcall cudaHostAlloc(void **, size_t, unsigned); } 
extern "C" { extern cudaError_t __stdcall cudaHostGetDevicePointer(void **, void *, unsigned); } 
extern "C" { extern cudaError_t __stdcall cudaHostGetFlags(unsigned *, void *); } 
#line 121 "C:\\CUDA\\include\\cuda_runtime_api.h"
extern "C" { extern cudaError_t __stdcall cudaMemcpy(void *, const void *, size_t, cudaMemcpyKind); } 
extern "C" { extern cudaError_t __stdcall cudaMemcpyToArray(cudaArray *, size_t, size_t, const void *, size_t, cudaMemcpyKind); } 
extern "C" { extern cudaError_t __stdcall cudaMemcpyFromArray(void *, const cudaArray *, size_t, size_t, size_t, cudaMemcpyKind); } 
extern "C" { extern cudaError_t __stdcall cudaMemcpyArrayToArray(cudaArray *, size_t, size_t, const cudaArray *, size_t, size_t, size_t, cudaMemcpyKind = cudaMemcpyDeviceToDevice); } 
extern "C" { extern cudaError_t __stdcall cudaMemcpy2D(void *, size_t, const void *, size_t, size_t, size_t, cudaMemcpyKind); } 
extern "C" { extern cudaError_t __stdcall cudaMemcpy2DToArray(cudaArray *, size_t, size_t, const void *, size_t, size_t, size_t, cudaMemcpyKind); } 
extern "C" { extern cudaError_t __stdcall cudaMemcpy2DFromArray(void *, size_t, const cudaArray *, size_t, size_t, size_t, size_t, cudaMemcpyKind); } 
extern "C" { extern cudaError_t __stdcall cudaMemcpy2DArrayToArray(cudaArray *, size_t, size_t, const cudaArray *, size_t, size_t, size_t, size_t, cudaMemcpyKind = cudaMemcpyDeviceToDevice); } 
extern "C" { extern cudaError_t __stdcall cudaMemcpyToSymbol(const char *, const void *, size_t, size_t = (0), cudaMemcpyKind = cudaMemcpyHostToDevice); } 
extern "C" { extern cudaError_t __stdcall cudaMemcpyFromSymbol(void *, const char *, size_t, size_t = (0), cudaMemcpyKind = cudaMemcpyDeviceToHost); } 
#line 138 "C:\\CUDA\\include\\cuda_runtime_api.h"
extern "C" { extern cudaError_t __stdcall cudaMemcpyAsync(void *, const void *, size_t, cudaMemcpyKind, cudaStream_t); } 
extern "C" { extern cudaError_t __stdcall cudaMemcpyToArrayAsync(cudaArray *, size_t, size_t, const void *, size_t, cudaMemcpyKind, cudaStream_t); } 
extern "C" { extern cudaError_t __stdcall cudaMemcpyFromArrayAsync(void *, const cudaArray *, size_t, size_t, size_t, cudaMemcpyKind, cudaStream_t); } 
extern "C" { extern cudaError_t __stdcall cudaMemcpy2DAsync(void *, size_t, const void *, size_t, size_t, size_t, cudaMemcpyKind, cudaStream_t); } 
extern "C" { extern cudaError_t __stdcall cudaMemcpy2DToArrayAsync(cudaArray *, size_t, size_t, const void *, size_t, size_t, size_t, cudaMemcpyKind, cudaStream_t); } 
extern "C" { extern cudaError_t __stdcall cudaMemcpy2DFromArrayAsync(void *, size_t, const cudaArray *, size_t, size_t, size_t, size_t, cudaMemcpyKind, cudaStream_t); } 
extern "C" { extern cudaError_t __stdcall cudaMemcpyToSymbolAsync(const char *, const void *, size_t, size_t, cudaMemcpyKind, cudaStream_t); } 
extern "C" { extern cudaError_t __stdcall cudaMemcpyFromSymbolAsync(void *, const char *, size_t, size_t, cudaMemcpyKind, cudaStream_t); } 
#line 153 "C:\\CUDA\\include\\cuda_runtime_api.h"
extern "C" { extern cudaError_t __stdcall cudaMemset(void *, int, size_t); } 
extern "C" { extern cudaError_t __stdcall cudaMemset2D(void *, size_t, int, size_t, size_t); } 
#line 162 "C:\\CUDA\\include\\cuda_runtime_api.h"
extern "C" { extern cudaError_t __stdcall cudaGetSymbolAddress(void **, const char *); } 
extern "C" { extern cudaError_t __stdcall cudaGetSymbolSize(size_t *, const char *); } 
#line 171 "C:\\CUDA\\include\\cuda_runtime_api.h"
extern "C" { extern cudaError_t __stdcall cudaGetDeviceCount(int *); } 
extern "C" { extern cudaError_t __stdcall cudaGetDeviceProperties(cudaDeviceProp *, int); } 
extern "C" { extern cudaError_t __stdcall cudaChooseDevice(int *, const cudaDeviceProp *); } 
extern "C" { extern cudaError_t __stdcall cudaSetDevice(int); } 
extern "C" { extern cudaError_t __stdcall cudaGetDevice(int *); } 
extern "C" { extern cudaError_t __stdcall cudaSetValidDevices(int *, int); } 
extern "C" { extern cudaError_t __stdcall cudaSetDeviceFlags(int); } 
#line 185 "C:\\CUDA\\include\\cuda_runtime_api.h"
extern "C" { extern cudaError_t __stdcall cudaBindTexture(size_t *, const textureReference *, const void *, const cudaChannelFormatDesc *, size_t = (4294967295U)); } 
extern "C" { extern cudaError_t __stdcall cudaBindTexture2D(size_t *, const textureReference *, const void *, const cudaChannelFormatDesc *, size_t, size_t, size_t); } 
extern "C" { extern cudaError_t __stdcall cudaBindTextureToArray(const textureReference *, const cudaArray *, const cudaChannelFormatDesc *); } 
extern "C" { extern cudaError_t __stdcall cudaUnbindTexture(const textureReference *); } 
extern "C" { extern cudaError_t __stdcall cudaGetTextureAlignmentOffset(size_t *, const textureReference *); } 
extern "C" { extern cudaError_t __stdcall cudaGetTextureReference(const textureReference **, const char *); } 
#line 198 "C:\\CUDA\\include\\cuda_runtime_api.h"
extern "C" { extern cudaError_t __stdcall cudaGetChannelDesc(cudaChannelFormatDesc *, const cudaArray *); } 
extern "C" { extern cudaChannelFormatDesc __stdcall cudaCreateChannelDesc(int, int, int, int, cudaChannelFormatKind); } 
#line 207 "C:\\CUDA\\include\\cuda_runtime_api.h"
extern "C" { extern cudaError_t __stdcall cudaGetLastError(); } 
extern "C" { extern const char *__stdcall cudaGetErrorString(cudaError_t); } 
#line 216 "C:\\CUDA\\include\\cuda_runtime_api.h"
extern "C" { extern cudaError_t __stdcall cudaConfigureCall(dim3, dim3, size_t = (0), cudaStream_t = (0)); } 
extern "C" { extern cudaError_t __stdcall cudaSetupArgument(const void *, size_t, size_t); } 
extern "C" { extern cudaError_t __stdcall cudaLaunch(const char *); } 
extern "C" { extern cudaError_t __stdcall cudaFuncGetAttributes(cudaFuncAttributes *, const char *); } 
#line 227 "C:\\CUDA\\include\\cuda_runtime_api.h"
extern "C" { extern cudaError_t __stdcall cudaStreamCreate(cudaStream_t *); } 
extern "C" { extern cudaError_t __stdcall cudaStreamDestroy(cudaStream_t); } 
extern "C" { extern cudaError_t __stdcall cudaStreamSynchronize(cudaStream_t); } 
extern "C" { extern cudaError_t __stdcall cudaStreamQuery(cudaStream_t); } 
#line 238 "C:\\CUDA\\include\\cuda_runtime_api.h"
extern "C" { extern cudaError_t __stdcall cudaEventCreate(cudaEvent_t *); } 
extern "C" { extern cudaError_t __stdcall cudaEventCreateWithFlags(cudaEvent_t *, int); } 
extern "C" { extern cudaError_t __stdcall cudaEventRecord(cudaEvent_t, cudaStream_t); } 
extern "C" { extern cudaError_t __stdcall cudaEventQuery(cudaEvent_t); } 
extern "C" { extern cudaError_t __stdcall cudaEventSynchronize(cudaEvent_t); } 
extern "C" { extern cudaError_t __stdcall cudaEventDestroy(cudaEvent_t); } 
extern "C" { extern cudaError_t __stdcall cudaEventElapsedTime(float *, cudaEvent_t, cudaEvent_t); } 
#line 252 "C:\\CUDA\\include\\cuda_runtime_api.h"
extern "C" { extern cudaError_t __stdcall cudaSetDoubleForDevice(double *); } 
extern "C" { extern cudaError_t __stdcall cudaSetDoubleForHost(double *); } 
#line 261 "C:\\CUDA\\include\\cuda_runtime_api.h"
extern "C" { extern cudaError_t __stdcall cudaThreadExit(); } 
extern "C" { extern cudaError_t __stdcall cudaThreadSynchronize(); } 
#line 270 "C:\\CUDA\\include\\cuda_runtime_api.h"
extern "C" { extern cudaError_t __stdcall cudaDriverGetVersion(int *); } 
extern "C" { extern cudaError_t __stdcall cudaRuntimeGetVersion(int *); } 
#line 93 "c:\\cuda\\include\\channel_descriptor.h"
template<class T> __inline cudaChannelFormatDesc cudaCreateChannelDesc() 
{ 
return cudaCreateChannelDesc(0, 0, 0, 0, cudaChannelFormatKindNone); 
} 

template<> __inline cudaChannelFormatDesc cudaCreateChannelDesc< char> () 
{ 
auto int e = (((int)sizeof(char)) * 8); 
#line 105 "c:\\cuda\\include\\channel_descriptor.h"
return cudaCreateChannelDesc(e, 0, 0, 0, cudaChannelFormatKindUnsigned); 
#line 107 "c:\\cuda\\include\\channel_descriptor.h"
} 

template<> __inline cudaChannelFormatDesc cudaCreateChannelDesc< signed char> () 
{ 
auto int e = (((int)sizeof(signed char)) * 8); 

return cudaCreateChannelDesc(e, 0, 0, 0, cudaChannelFormatKindSigned); 
} 

template<> __inline cudaChannelFormatDesc cudaCreateChannelDesc< unsigned char> () 
{ 
auto int e = (((int)sizeof(unsigned char)) * 8); 

return cudaCreateChannelDesc(e, 0, 0, 0, cudaChannelFormatKindUnsigned); 
} 

template<> __inline cudaChannelFormatDesc cudaCreateChannelDesc< char1> () 
{ 
auto int e = (((int)sizeof(signed char)) * 8); 

return cudaCreateChannelDesc(e, 0, 0, 0, cudaChannelFormatKindSigned); 
} 

template<> __inline cudaChannelFormatDesc cudaCreateChannelDesc< uchar1> () 
{ 
auto int e = (((int)sizeof(unsigned char)) * 8); 

return cudaCreateChannelDesc(e, 0, 0, 0, cudaChannelFormatKindUnsigned); 
} 

template<> __inline cudaChannelFormatDesc cudaCreateChannelDesc< char2> () 
{ 
auto int e = (((int)sizeof(signed char)) * 8); 

return cudaCreateChannelDesc(e, e, 0, 0, cudaChannelFormatKindSigned); 
} 

template<> __inline cudaChannelFormatDesc cudaCreateChannelDesc< uchar2> () 
{ 
auto int e = (((int)sizeof(unsigned char)) * 8); 

return cudaCreateChannelDesc(e, e, 0, 0, cudaChannelFormatKindUnsigned); 
} 

template<> __inline cudaChannelFormatDesc cudaCreateChannelDesc< char4> () 
{ 
auto int e = (((int)sizeof(signed char)) * 8); 

return cudaCreateChannelDesc(e, e, e, e, cudaChannelFormatKindSigned); 
} 

template<> __inline cudaChannelFormatDesc cudaCreateChannelDesc< uchar4> () 
{ 
auto int e = (((int)sizeof(unsigned char)) * 8); 

return cudaCreateChannelDesc(e, e, e, e, cudaChannelFormatKindUnsigned); 
} 

template<> __inline cudaChannelFormatDesc cudaCreateChannelDesc< short> () 
{ 
auto int e = (((int)sizeof(short)) * 8); 

return cudaCreateChannelDesc(e, 0, 0, 0, cudaChannelFormatKindSigned); 
} 

template<> __inline cudaChannelFormatDesc cudaCreateChannelDesc< unsigned short> () 
{ 
auto int e = (((int)sizeof(unsigned short)) * 8); 

return cudaCreateChannelDesc(e, 0, 0, 0, cudaChannelFormatKindUnsigned); 
} 

template<> __inline cudaChannelFormatDesc cudaCreateChannelDesc< short1> () 
{ 
auto int e = (((int)sizeof(short)) * 8); 

return cudaCreateChannelDesc(e, 0, 0, 0, cudaChannelFormatKindSigned); 
} 

template<> __inline cudaChannelFormatDesc cudaCreateChannelDesc< ushort1> () 
{ 
auto int e = (((int)sizeof(unsigned short)) * 8); 

return cudaCreateChannelDesc(e, 0, 0, 0, cudaChannelFormatKindUnsigned); 
} 

template<> __inline cudaChannelFormatDesc cudaCreateChannelDesc< short2> () 
{ 
auto int e = (((int)sizeof(short)) * 8); 

return cudaCreateChannelDesc(e, e, 0, 0, cudaChannelFormatKindSigned); 
} 

template<> __inline cudaChannelFormatDesc cudaCreateChannelDesc< ushort2> () 
{ 
auto int e = (((int)sizeof(unsigned short)) * 8); 

return cudaCreateChannelDesc(e, e, 0, 0, cudaChannelFormatKindUnsigned); 
} 

template<> __inline cudaChannelFormatDesc cudaCreateChannelDesc< short4> () 
{ 
auto int e = (((int)sizeof(short)) * 8); 

return cudaCreateChannelDesc(e, e, e, e, cudaChannelFormatKindSigned); 
} 

template<> __inline cudaChannelFormatDesc cudaCreateChannelDesc< ushort4> () 
{ 
auto int e = (((int)sizeof(unsigned short)) * 8); 

return cudaCreateChannelDesc(e, e, e, e, cudaChannelFormatKindUnsigned); 
} 

template<> __inline cudaChannelFormatDesc cudaCreateChannelDesc< int> () 
{ 
auto int e = (((int)sizeof(int)) * 8); 

return cudaCreateChannelDesc(e, 0, 0, 0, cudaChannelFormatKindSigned); 
} 

template<> __inline cudaChannelFormatDesc cudaCreateChannelDesc< unsigned> () 
{ 
auto int e = (((int)sizeof(unsigned)) * 8); 

return cudaCreateChannelDesc(e, 0, 0, 0, cudaChannelFormatKindUnsigned); 
} 

template<> __inline cudaChannelFormatDesc cudaCreateChannelDesc< int1> () 
{ 
auto int e = (((int)sizeof(int)) * 8); 

return cudaCreateChannelDesc(e, 0, 0, 0, cudaChannelFormatKindSigned); 
} 

template<> __inline cudaChannelFormatDesc cudaCreateChannelDesc< uint1> () 
{ 
auto int e = (((int)sizeof(unsigned)) * 8); 

return cudaCreateChannelDesc(e, 0, 0, 0, cudaChannelFormatKindUnsigned); 
} 

template<> __inline cudaChannelFormatDesc cudaCreateChannelDesc< int2> () 
{ 
auto int e = (((int)sizeof(int)) * 8); 

return cudaCreateChannelDesc(e, e, 0, 0, cudaChannelFormatKindSigned); 
} 

template<> __inline cudaChannelFormatDesc cudaCreateChannelDesc< uint2> () 
{ 
auto int e = (((int)sizeof(unsigned)) * 8); 

return cudaCreateChannelDesc(e, e, 0, 0, cudaChannelFormatKindUnsigned); 
} 

template<> __inline cudaChannelFormatDesc cudaCreateChannelDesc< int4> () 
{ 
auto int e = (((int)sizeof(int)) * 8); 

return cudaCreateChannelDesc(e, e, e, e, cudaChannelFormatKindSigned); 
} 

template<> __inline cudaChannelFormatDesc cudaCreateChannelDesc< uint4> () 
{ 
auto int e = (((int)sizeof(unsigned)) * 8); 

return cudaCreateChannelDesc(e, e, e, e, cudaChannelFormatKindUnsigned); 
} 



template<> __inline cudaChannelFormatDesc cudaCreateChannelDesc< long> () 
{ 
auto int e = (((int)sizeof(long)) * 8); 

return cudaCreateChannelDesc(e, 0, 0, 0, cudaChannelFormatKindSigned); 
} 

template<> __inline cudaChannelFormatDesc cudaCreateChannelDesc< unsigned long> () 
{ 
auto int e = (((int)sizeof(unsigned long)) * 8); 

return cudaCreateChannelDesc(e, 0, 0, 0, cudaChannelFormatKindUnsigned); 
} 

template<> __inline cudaChannelFormatDesc cudaCreateChannelDesc< long1> () 
{ 
auto int e = (((int)sizeof(long)) * 8); 

return cudaCreateChannelDesc(e, 0, 0, 0, cudaChannelFormatKindSigned); 
} 

template<> __inline cudaChannelFormatDesc cudaCreateChannelDesc< ulong1> () 
{ 
auto int e = (((int)sizeof(unsigned long)) * 8); 

return cudaCreateChannelDesc(e, 0, 0, 0, cudaChannelFormatKindUnsigned); 
} 

template<> __inline cudaChannelFormatDesc cudaCreateChannelDesc< long2> () 
{ 
auto int e = (((int)sizeof(long)) * 8); 

return cudaCreateChannelDesc(e, e, 0, 0, cudaChannelFormatKindSigned); 
} 

template<> __inline cudaChannelFormatDesc cudaCreateChannelDesc< ulong2> () 
{ 
auto int e = (((int)sizeof(unsigned long)) * 8); 

return cudaCreateChannelDesc(e, e, 0, 0, cudaChannelFormatKindUnsigned); 
} 

template<> __inline cudaChannelFormatDesc cudaCreateChannelDesc< long4> () 
{ 
auto int e = (((int)sizeof(long)) * 8); 

return cudaCreateChannelDesc(e, e, e, e, cudaChannelFormatKindSigned); 
} 

template<> __inline cudaChannelFormatDesc cudaCreateChannelDesc< ulong4> () 
{ 
auto int e = (((int)sizeof(unsigned long)) * 8); 

return cudaCreateChannelDesc(e, e, e, e, cudaChannelFormatKindUnsigned); 
} 
#line 337 "c:\\cuda\\include\\channel_descriptor.h"
template<> __inline cudaChannelFormatDesc cudaCreateChannelDesc< float> () 
{ 
auto int e = (((int)sizeof(float)) * 8); 

return cudaCreateChannelDesc(e, 0, 0, 0, cudaChannelFormatKindFloat); 
} 

template<> __inline cudaChannelFormatDesc cudaCreateChannelDesc< float1> () 
{ 
auto int e = (((int)sizeof(float)) * 8); 

return cudaCreateChannelDesc(e, 0, 0, 0, cudaChannelFormatKindFloat); 
} 

template<> __inline cudaChannelFormatDesc cudaCreateChannelDesc< float2> () 
{ 
auto int e = (((int)sizeof(float)) * 8); 

return cudaCreateChannelDesc(e, e, 0, 0, cudaChannelFormatKindFloat); 
} 

template<> __inline cudaChannelFormatDesc cudaCreateChannelDesc< float4> () 
{ 
auto int e = (((int)sizeof(float)) * 8); 

return cudaCreateChannelDesc(e, e, e, e, cudaChannelFormatKindFloat); 
} 
#line 54 "c:\\cuda\\include\\driver_functions.h"
static __inline cudaPitchedPtr make_cudaPitchedPtr(void *d, size_t p, size_t xsz, size_t ysz) 
{ 
auto cudaPitchedPtr s; 

(s.ptr) = d; 
(s.pitch) = p; 
(s.xsize) = xsz; 
(s.ysize) = ysz; 

return s; 
} 

static __inline cudaPos make_cudaPos(size_t x, size_t y, size_t z) 
{ 
auto cudaPos p; 

(p.x) = x; 
(p.y) = y; 
(p.z) = z; 

return p; 
} 

static __inline cudaExtent make_cudaExtent(size_t w, size_t h, size_t d) 
{ 
auto cudaExtent e; 

(e.width) = w; 
(e.height) = h; 
(e.depth) = d; 

return e; 
} 
#line 54 "c:\\cuda\\include\\vector_functions.h"
static __inline char1 make_char1(signed char x) 
{ 
auto char1 t; (t.x) = x; return t; 
} 

static __inline uchar1 make_uchar1(unsigned char x) 
{ 
auto uchar1 t; (t.x) = x; return t; 
} 

static __inline char2 make_char2(signed char x, signed char y) 
{ 
auto char2 t; (t.x) = x; (t.y) = y; return t; 
} 

static __inline uchar2 make_uchar2(unsigned char x, unsigned char y) 
{ 
auto uchar2 t; (t.x) = x; (t.y) = y; return t; 
} 

static __inline char3 make_char3(signed char x, signed char y, signed char z) 
{ 
auto char3 t; (t.x) = x; (t.y) = y; (t.z) = z; return t; 
} 

static __inline uchar3 make_uchar3(unsigned char x, unsigned char y, unsigned char z) 
{ 
auto uchar3 t; (t.x) = x; (t.y) = y; (t.z) = z; return t; 
} 

static __inline char4 make_char4(signed char x, signed char y, signed char z, signed char w) 
{ 
auto char4 t; (t.x) = x; (t.y) = y; (t.z) = z; (t.w) = w; return t; 
} 

static __inline uchar4 make_uchar4(unsigned char x, unsigned char y, unsigned char z, unsigned char w) 
{ 
auto uchar4 t; (t.x) = x; (t.y) = y; (t.z) = z; (t.w) = w; return t; 
} 

static __inline short1 make_short1(short x) 
{ 
auto short1 t; (t.x) = x; return t; 
} 

static __inline ushort1 make_ushort1(unsigned short x) 
{ 
auto ushort1 t; (t.x) = x; return t; 
} 

static __inline short2 make_short2(short x, short y) 
{ 
auto short2 t; (t.x) = x; (t.y) = y; return t; 
} 

static __inline ushort2 make_ushort2(unsigned short x, unsigned short y) 
{ 
auto ushort2 t; (t.x) = x; (t.y) = y; return t; 
} 

static __inline short3 make_short3(short x, short y, short z) 
{ 
auto short3 t; (t.x) = x; (t.y) = y; (t.z) = z; return t; 
} 

static __inline ushort3 make_ushort3(unsigned short x, unsigned short y, unsigned short z) 
{ 
auto ushort3 t; (t.x) = x; (t.y) = y; (t.z) = z; return t; 
} 

static __inline short4 make_short4(short x, short y, short z, short w) 
{ 
auto short4 t; (t.x) = x; (t.y) = y; (t.z) = z; (t.w) = w; return t; 
} 

static __inline ushort4 make_ushort4(unsigned short x, unsigned short y, unsigned short z, unsigned short w) 
{ 
auto ushort4 t; (t.x) = x; (t.y) = y; (t.z) = z; (t.w) = w; return t; 
} 

static __inline int1 make_int1(int x) 
{ 
auto int1 t; (t.x) = x; return t; 
} 

static __inline uint1 make_uint1(unsigned x) 
{ 
auto uint1 t; (t.x) = x; return t; 
} 

static __inline int2 make_int2(int x, int y) 
{ 
auto int2 t; (t.x) = x; (t.y) = y; return t; 
} 

static __inline uint2 make_uint2(unsigned x, unsigned y) 
{ 
auto uint2 t; (t.x) = x; (t.y) = y; return t; 
} 

static __inline int3 make_int3(int x, int y, int z) 
{ 
auto int3 t; (t.x) = x; (t.y) = y; (t.z) = z; return t; 
} 

static __inline uint3 make_uint3(unsigned x, unsigned y, unsigned z) 
{ 
auto uint3 t; (t.x) = x; (t.y) = y; (t.z) = z; return t; 
} 

static __inline int4 make_int4(int x, int y, int z, int w) 
{ 
auto int4 t; (t.x) = x; (t.y) = y; (t.z) = z; (t.w) = w; return t; 
} 

static __inline uint4 make_uint4(unsigned x, unsigned y, unsigned z, unsigned w) 
{ 
auto uint4 t; (t.x) = x; (t.y) = y; (t.z) = z; (t.w) = w; return t; 
} 

static __inline long1 make_long1(long x) 
{ 
auto long1 t; (t.x) = x; return t; 
} 

static __inline ulong1 make_ulong1(unsigned long x) 
{ 
auto ulong1 t; (t.x) = x; return t; 
} 

static __inline long2 make_long2(long x, long y) 
{ 
auto long2 t; (t.x) = x; (t.y) = y; return t; 
} 

static __inline ulong2 make_ulong2(unsigned long x, unsigned long y) 
{ 
auto ulong2 t; (t.x) = x; (t.y) = y; return t; 
} 



static __inline long3 make_long3(long x, long y, long z) 
{ 
auto long3 t; (t.x) = x; (t.y) = y; (t.z) = z; return t; 
} 

static __inline ulong3 make_ulong3(unsigned long x, unsigned long y, unsigned long z) 
{ 
auto ulong3 t; (t.x) = x; (t.y) = y; (t.z) = z; return t; 
} 

static __inline long4 make_long4(long x, long y, long z, long w) 
{ 
auto long4 t; (t.x) = x; (t.y) = y; (t.z) = z; (t.w) = w; return t; 
} 

static __inline ulong4 make_ulong4(unsigned long x, unsigned long y, unsigned long z, unsigned long w) 
{ 
auto ulong4 t; (t.x) = x; (t.y) = y; (t.z) = z; (t.w) = w; return t; 
} 
#line 218 "c:\\cuda\\include\\vector_functions.h"
static __inline float1 make_float1(float x) 
{ 
auto float1 t; (t.x) = x; return t; 
} 

static __inline float2 make_float2(float x, float y) 
{ 
auto float2 t; (t.x) = x; (t.y) = y; return t; 
} 

static __inline float3 make_float3(float x, float y, float z) 
{ 
auto float3 t; (t.x) = x; (t.y) = y; (t.z) = z; return t; 
} 

static __inline float4 make_float4(float x, float y, float z, float w) 
{ 
auto float4 t; (t.x) = x; (t.y) = y; (t.z) = z; (t.w) = w; return t; 
} 

static __inline longlong1 make_longlong1(__int64 x) 
{ 
auto longlong1 t; (t.x) = x; return t; 
} 

static __inline ulonglong1 make_ulonglong1(unsigned __int64 x) 
{ 
auto ulonglong1 t; (t.x) = x; return t; 
} 

static __inline longlong2 make_longlong2(__int64 x, __int64 y) 
{ 
auto longlong2 t; (t.x) = x; (t.y) = y; return t; 
} 

static __inline ulonglong2 make_ulonglong2(unsigned __int64 x, unsigned __int64 y) 
{ 
auto ulonglong2 t; (t.x) = x; (t.y) = y; return t; 
} 

static __inline double1 make_double1(double x) 
{ 
auto double1 t; (t.x) = x; return t; 
} 

static __inline double2 make_double2(double x, double y) 
{ 
auto double2 t; (t.x) = x; (t.y) = y; return t; 
} 
#line 35 "C:\\Program Files\\Microsoft Visual Studio 8\\VC\\INCLUDE\\time.h"
#pragma pack ( push, 8 )
#line 95 "C:\\Program Files\\Microsoft Visual Studio 8\\VC\\INCLUDE\\time.h"
extern "C" { typedef long clock_t; }
#line 119 "C:\\Program Files\\Microsoft Visual Studio 8\\VC\\INCLUDE\\time.h"
extern "C" { struct tm { 
int tm_sec; 
int tm_min; 
int tm_hour; 
int tm_mday; 
int tm_mon; 
int tm_year; 
int tm_wday; 
int tm_yday; 
int tm_isdst; 
}; }
#line 144 "C:\\Program Files\\Microsoft Visual Studio 8\\VC\\INCLUDE\\time.h"
extern "C" { extern __declspec( dllimport ) int *__cdecl __daylight(); } 



extern "C" { extern __declspec( dllimport ) long *__cdecl __dstbias(); } 



extern "C" { extern __declspec( dllimport ) long *__cdecl __timezone(); } 



extern "C" { extern __declspec( dllimport ) char **__cdecl __tzname(); } 


extern "C" { extern __declspec( dllimport ) errno_t __cdecl _get_daylight(int *); } 
extern "C" { extern __declspec( dllimport ) errno_t __cdecl _get_dstbias(long *); } 
extern "C" { extern __declspec( dllimport ) errno_t __cdecl _get_timezone(long *); } 
extern "C" { extern __declspec( dllimport ) errno_t __cdecl _get_tzname(size_t *, char *, size_t, int); } 



extern "C" { extern __declspec( dllimport ) char *__cdecl asctime(const tm *); } 

extern "C" { extern __declspec( dllimport ) errno_t __cdecl asctime_s(char *, size_t, const tm *); } 
#line 170 "C:\\Program Files\\Microsoft Visual Studio 8\\VC\\INCLUDE\\time.h"
template < size_t _Size > inline errno_t __cdecl asctime_s ( char ( & _Buffer ) [ _Size ], const struct tm * _Time ) { return asctime_s ( _Buffer, _Size, _Time ); }

extern "C" { extern __declspec( dllimport ) char *__cdecl _ctime32(const __time32_t *); } 
extern "C" { extern __declspec( dllimport ) errno_t __cdecl _ctime32_s(char *, size_t, const __time32_t *); } 
template < size_t _Size > inline errno_t __cdecl _ctime32_s ( char ( & _Buffer ) [ _Size ], const __time32_t * _Time ) { return _ctime32_s ( _Buffer, _Size, _Time ); }

extern "C" { extern __declspec( dllimport ) clock_t __cdecl clock(); } 
extern "C" { extern __declspec( dllimport ) double __cdecl _difftime32(__time32_t, __time32_t); } 

extern "C" { extern __declspec( dllimport ) tm *__cdecl _gmtime32(const __time32_t *); } 
extern "C" { extern __declspec( dllimport ) errno_t __cdecl _gmtime32_s(tm *, const __time32_t *); } 

extern "C" { extern __declspec( dllimport ) tm *__cdecl _localtime32(const __time32_t *); } 
extern "C" { extern __declspec( dllimport ) errno_t __cdecl _localtime32_s(tm *, const __time32_t *); } 

extern "C" { extern __declspec( dllimport ) size_t __cdecl strftime(char *, size_t, const char *, const tm *); } 
extern "C" { extern __declspec( dllimport ) size_t __cdecl _strftime_l(char *, size_t, const char *, const tm *, _locale_t); } 

extern "C" { extern __declspec( dllimport ) errno_t __cdecl _strdate_s(char *, size_t); } 
template < size_t _Size > inline errno_t __cdecl _strdate_s ( char ( & _Buffer ) [ _Size ] ) { return _strdate_s ( _Buffer, _Size ); }
extern "C" { extern __declspec( dllimport ) char *__cdecl _strdate(char *); } 

extern "C" { extern __declspec( dllimport ) errno_t __cdecl _strtime_s(char *, size_t); } 
template < size_t _Size > inline errno_t __cdecl _strtime_s ( char ( & _Buffer ) [ _Size ] ) { return _strtime_s ( _Buffer, _Size ); }
extern "C" { extern __declspec( dllimport ) char *__cdecl _strtime(char *); } 

extern "C" { extern __declspec( dllimport ) __time32_t __cdecl _time32(__time32_t *); } 
extern "C" { extern __declspec( dllimport ) __time32_t __cdecl _mktime32(tm *); } 
extern "C" { extern __declspec( dllimport ) __time32_t __cdecl _mkgmtime32(tm *); } 




extern "C" { extern __declspec( dllimport ) void __cdecl _tzset(); } 
#line 207 "C:\\Program Files\\Microsoft Visual Studio 8\\VC\\INCLUDE\\time.h"
extern "C" { extern __declspec( dllimport ) double __cdecl _difftime64(__time64_t, __time64_t); } 
extern "C" { extern __declspec( dllimport ) char *__cdecl _ctime64(const __time64_t *); } 
extern "C" { extern __declspec( dllimport ) errno_t __cdecl _ctime64_s(char *, size_t, const __time64_t *); } 
template < size_t _Size > inline errno_t __cdecl _ctime64_s ( char ( & _Buffer ) [ _Size ], const __time64_t * _Time ) { return _ctime64_s ( _Buffer, _Size, _Time ); }

extern "C" { extern __declspec( dllimport ) tm *__cdecl _gmtime64(const __time64_t *); } 
extern "C" { extern __declspec( dllimport ) errno_t __cdecl _gmtime64_s(tm *, const __time64_t *); } 

extern "C" { extern __declspec( dllimport ) tm *__cdecl _localtime64(const __time64_t *); } 
extern "C" { extern __declspec( dllimport ) errno_t __cdecl _localtime64_s(tm *, const __time64_t *); } 

extern "C" { extern __declspec( dllimport ) __time64_t __cdecl _mktime64(tm *); } 
extern "C" { extern __declspec( dllimport ) __time64_t __cdecl _mkgmtime64(tm *); } 
extern "C" { extern __declspec( dllimport ) __time64_t __cdecl _time64(__time64_t *); } 
#line 224 "C:\\Program Files\\Microsoft Visual Studio 8\\VC\\INCLUDE\\time.h"
extern "C" { extern __declspec(deprecated("This function or variable has been superceded by newer library or operating system functionality. Consider using GetLocalTime instead. See online help for details.")) unsigned __cdecl _getsystime(tm *); } 
extern "C" { extern __declspec(deprecated("This function or variable has been superceded by newer library or operating system functionality. Consider using SetLocalTime instead. See online help for details.")) unsigned __cdecl _setsystime(tm *, unsigned); } 
#line 237 "C:\\Program Files\\Microsoft Visual Studio 8\\VC\\INCLUDE\\time.h"
extern "C" { extern __declspec( dllimport ) __wchar_t *__cdecl _wasctime(const tm *); } 
extern "C" { extern __declspec( dllimport ) errno_t __cdecl _wasctime_s(__wchar_t *, size_t, const tm *); } 
template < size_t _Size > inline errno_t __cdecl _wasctime_s ( wchar_t ( & _Buffer ) [ _Size ], const struct tm * _Time ) { return _wasctime_s ( _Buffer, _Size, _Time ); }

extern "C" { extern __declspec( dllimport ) __wchar_t *__cdecl _wctime32(const __time32_t *); } 
extern "C" { extern __declspec( dllimport ) errno_t __cdecl _wctime32_s(__wchar_t *, size_t, const __time32_t *); } 
template < size_t _Size > inline errno_t __cdecl _wctime32_s ( wchar_t ( & _Buffer ) [ _Size ], const __time32_t * _Time ) { return _wctime32_s ( _Buffer, _Size, _Time ); }

extern "C" { extern __declspec( dllimport ) size_t __cdecl wcsftime(__wchar_t *, size_t, const __wchar_t *, const tm *); } 
extern "C" { extern __declspec( dllimport ) size_t __cdecl _wcsftime_l(__wchar_t *, size_t, const __wchar_t *, const tm *, _locale_t); } 

extern "C" { extern __declspec( dllimport ) errno_t __cdecl _wstrdate_s(__wchar_t *, size_t); } 
template < size_t _Size > inline errno_t __cdecl _wstrdate_s ( wchar_t ( & _Buffer ) [ _Size ] ) { return _wstrdate_s ( _Buffer, _Size ); }
extern "C" { extern __declspec( dllimport ) __wchar_t *__cdecl _wstrdate(__wchar_t *); } 

extern "C" { extern __declspec( dllimport ) errno_t __cdecl _wstrtime_s(__wchar_t *, size_t); } 
template < size_t _Size > inline errno_t __cdecl _wstrtime_s ( wchar_t ( & _Buffer ) [ _Size ] ) { return _wstrtime_s ( _Buffer, _Size ); }
extern "C" { extern __declspec( dllimport ) __wchar_t *__cdecl _wstrtime(__wchar_t *); } 


extern "C" { extern __declspec( dllimport ) __wchar_t *__cdecl _wctime64(const __time64_t *); } 
extern "C" { extern __declspec( dllimport ) errno_t __cdecl _wctime64_s(__wchar_t *, size_t, const __time64_t *); } 
template < size_t _Size > inline errno_t __cdecl _wctime64_s ( wchar_t ( & _Buffer ) [ _Size ], const __time64_t * _Time ) { return _wctime64_s ( _Buffer, _Size, _Time ); }
#line 29 "C:\\Program Files\\Microsoft Visual Studio 8\\VC\\INCLUDE\\wtime.inl"
#pragma warning(push)
#pragma warning(disable:4996)
#line 46 "C:\\Program Files\\Microsoft Visual Studio 8\\VC\\INCLUDE\\wtime.inl"
extern "C" { static __inline __wchar_t *__cdecl _wctime(const time_t *_Time) 
{ 
#pragma warning( push )
#pragma warning( disable : 4996 )
return _wctime64(_Time); 
#pragma warning( pop )
} } 

extern "C" { static __inline errno_t __cdecl _wctime_s(__wchar_t *_Buffer, size_t _SizeInWords, const time_t *_Time) 
{ 
return _wctime64_s(_Buffer, _SizeInWords, _Time); 
} } 
#line 60 "C:\\Program Files\\Microsoft Visual Studio 8\\VC\\INCLUDE\\wtime.inl"
#pragma warning(pop)
#line 84 "C:\\Program Files\\Microsoft Visual Studio 8\\VC\\INCLUDE\\time.inl"
extern "C" { static __inline double __cdecl difftime(time_t _Time1, time_t _Time2) 
{ 
return _difftime64(_Time1, _Time2); 
} } 
extern "C" { static __inline char *__cdecl ctime(const time_t *_Time) 
{ 
#pragma warning( push )
#pragma warning( disable : 4996 )
return _ctime64(_Time); 
#pragma warning( pop )
} } 

extern "C" { static __inline errno_t __cdecl ctime_s(char *_Buffer, size_t _SizeInBytes, const time_t *_Time) 
{ 
return _ctime64_s(_Buffer, _SizeInBytes, _Time); 
} } 
#line 101 "C:\\Program Files\\Microsoft Visual Studio 8\\VC\\INCLUDE\\time.inl"
extern "C" { static __inline tm *__cdecl gmtime(const time_t *_Time) 
{ 
#pragma warning( push )
#pragma warning( disable : 4996 )
return _gmtime64(_Time); 
#pragma warning( pop )
} } 

extern "C" { static __inline errno_t __cdecl gmtime_s(tm *_Tm, const time_t *_Time) 
{ 
return _gmtime64_s(_Tm, _Time); 
} } 
#line 114 "C:\\Program Files\\Microsoft Visual Studio 8\\VC\\INCLUDE\\time.inl"
extern "C" { static __inline tm *__cdecl localtime(const time_t *_Time) 
{ 
#pragma warning( push )
#pragma warning( disable : 4996 )
return _localtime64(_Time); 
#pragma warning( pop )
} } 
extern "C" { static __inline errno_t __cdecl localtime_s(tm *_Tm, const time_t *_Time) 
{ 
return _localtime64_s(_Tm, _Time); 
} } 
extern "C" { static __inline time_t __cdecl mktime(tm *_Tm) 
{ 
return _mktime64(_Tm); 
} } 
extern "C" { static __inline time_t __cdecl _mkgmtime(tm *_Tm) 
{ 
return _mkgmtime64(_Tm); 
} } 
extern "C" { static __inline time_t __cdecl time(time_t *_Time) 
{ 
return _time64(_Time); 
} } 
#line 285 "C:\\Program Files\\Microsoft Visual Studio 8\\VC\\INCLUDE\\time.h"
extern "C" __declspec( dllimport ) int daylight; 
extern "C" __declspec( dllimport ) long timezone; 
extern "C" __declspec( dllimport ) char *tzname[2]; 
#line 290 "C:\\Program Files\\Microsoft Visual Studio 8\\VC\\INCLUDE\\time.h"
extern "C" { extern __declspec( dllimport ) void __cdecl tzset(); } 
#line 300 "C:\\Program Files\\Microsoft Visual Studio 8\\VC\\INCLUDE\\time.h"
#pragma pack ( pop )
#line 48 "C:\\Program Files\\Microsoft Visual Studio 8\\VC\\INCLUDE\\string.h"
extern "C" { extern __declspec( dllimport ) void *__cdecl _memccpy(void *, const void *, int, size_t); } 
extern "C" { extern __declspec( dllimport ) const void *__cdecl memchr(const void *, int, size_t); } 
extern "C" { extern __declspec( dllimport ) int __cdecl _memicmp(const void *, const void *, size_t); } 
extern "C" { extern __declspec( dllimport ) int __cdecl _memicmp_l(const void *, const void *, size_t, _locale_t); } 
extern "C" { extern int __cdecl memcmp(const void *, const void *, size_t); } 
extern "C" { extern void *__cdecl memcpy(void *, const void *, size_t); } 

extern "C" { extern __declspec( dllimport ) errno_t __cdecl memcpy_s(void *, rsize_t, const void *, rsize_t); } 
#line 57 "C:\\Program Files\\Microsoft Visual Studio 8\\VC\\INCLUDE\\string.h"
extern "C" { extern void *__cdecl memset(void *, int, size_t); } 



extern "C" { extern __declspec( dllimport ) void *__cdecl memccpy(void *, const void *, int, size_t); } 
extern "C" { extern __declspec( dllimport ) int __cdecl memicmp(const void *, const void *, size_t); } 
#line 67 "C:\\Program Files\\Microsoft Visual Studio 8\\VC\\INCLUDE\\string.h"
extern "C" { extern __declspec( dllimport ) errno_t __cdecl _strset_s(char *, size_t, int); } 
template < size_t _Size > inline errno_t __cdecl _strset_s ( char ( & _Dest ) [ _Size ], int _Value ) { return _strset_s ( _Dest, _Size, _Value ); }
extern "C" { extern char *__cdecl _strset(char *, int); } 

extern "C" { extern __declspec( dllimport ) errno_t __cdecl strcpy_s(char *, rsize_t, const char *); } 
#line 73 "C:\\Program Files\\Microsoft Visual Studio 8\\VC\\INCLUDE\\string.h"
template < size_t _Size > inline errno_t __cdecl strcpy_s ( char ( & _Dest ) [ _Size ], const char * _Source ) { return strcpy_s ( _Dest, _Size, _Source ); }
extern "C" { extern char *__cdecl strcpy(char *, const char *); } 

extern "C" { extern __declspec( dllimport ) errno_t __cdecl strcat_s(char *, rsize_t, const char *); } 
#line 78 "C:\\Program Files\\Microsoft Visual Studio 8\\VC\\INCLUDE\\string.h"
template < size_t _Size > inline errno_t __cdecl strcat_s ( char ( & _Dest ) [ _Size ], const char * _Source ) { return strcat_s ( _Dest, _Size, _Source ); }
extern "C" { extern char *__cdecl strcat(char *, const char *); } 
extern "C" { extern int __cdecl strcmp(const char *, const char *); } 
extern "C" { extern size_t __cdecl strlen(const char *); } 
extern "C" { extern __declspec( dllimport ) size_t __cdecl strnlen(const char *, size_t); } 

extern "C" { static __inline size_t __cdecl strnlen_s(const char *_Str, size_t _MaxCount) 
{ 
return strnlen(_Str, _MaxCount); 
} } 
#line 90 "C:\\Program Files\\Microsoft Visual Studio 8\\VC\\INCLUDE\\string.h"
extern "C" { extern __declspec( dllimport ) errno_t __cdecl memmove_s(void *, rsize_t, const void *, rsize_t); } 
#line 96 "C:\\Program Files\\Microsoft Visual Studio 8\\VC\\INCLUDE\\string.h"
extern "C" { extern __declspec( dllimport ) void *__cdecl memmove(void *, const void *, size_t); } 
#line 104 "C:\\Program Files\\Microsoft Visual Studio 8\\VC\\INCLUDE\\string.h"
extern "C" { extern __declspec( dllimport ) char *__cdecl _strdup(const char *); } 
#line 110 "C:\\Program Files\\Microsoft Visual Studio 8\\VC\\INCLUDE\\string.h"
extern "C" { extern __declspec( dllimport ) const char *__cdecl strchr(const char *, int); } 
extern "C" { extern __declspec( dllimport ) int __cdecl _stricmp(const char *, const char *); } 
extern "C" { extern __declspec( dllimport ) int __cdecl _strcmpi(const char *, const char *); } 
extern "C" { extern __declspec( dllimport ) int __cdecl _stricmp_l(const char *, const char *, _locale_t); } 
extern "C" { extern __declspec( dllimport ) int __cdecl strcoll(const char *, const char *); } 
extern "C" { extern __declspec( dllimport ) int __cdecl _strcoll_l(const char *, const char *, _locale_t); } 
extern "C" { extern __declspec( dllimport ) int __cdecl _stricoll(const char *, const char *); } 
extern "C" { extern __declspec( dllimport ) int __cdecl _stricoll_l(const char *, const char *, _locale_t); } 
extern "C" { extern __declspec( dllimport ) int __cdecl _strncoll(const char *, const char *, size_t); } 
extern "C" { extern __declspec( dllimport ) int __cdecl _strncoll_l(const char *, const char *, size_t, _locale_t); } 
extern "C" { extern __declspec( dllimport ) int __cdecl _strnicoll(const char *, const char *, size_t); } 
extern "C" { extern __declspec( dllimport ) int __cdecl _strnicoll_l(const char *, const char *, size_t, _locale_t); } 
extern "C" { extern __declspec( dllimport ) size_t __cdecl strcspn(const char *, const char *); } 
extern "C" { extern __declspec( dllimport ) char *__cdecl _strerror(const char *); } 
extern "C" { extern __declspec( dllimport ) errno_t __cdecl _strerror_s(char *, size_t, const char *); } 
template < size_t _Size > inline errno_t __cdecl _strerror_s ( char ( & _Buffer ) [ _Size ], const char * _ErrorMessage ) { return _strerror_s ( _Buffer, _Size, _ErrorMessage ); }
extern "C" { extern __declspec( dllimport ) char *__cdecl strerror(int); } 

extern "C" { extern __declspec( dllimport ) errno_t __cdecl strerror_s(char *, size_t, int); } 
#line 130 "C:\\Program Files\\Microsoft Visual Studio 8\\VC\\INCLUDE\\string.h"
template < size_t _Size > inline errno_t __cdecl strerror_s ( char ( & _Buffer ) [ _Size ], int _ErrorMessage ) { return strerror_s ( _Buffer, _Size, _ErrorMessage ); }
extern "C" { extern __declspec( dllimport ) errno_t __cdecl _strlwr_s(char *, size_t); } 
template < size_t _Size > inline errno_t __cdecl _strlwr_s ( char ( & _String ) [ _Size ] ) { return _strlwr_s ( _String, _Size ); }
extern "C" { extern __declspec( dllimport ) char *__cdecl _strlwr(char *); } 
extern "C" { extern __declspec( dllimport ) errno_t __cdecl _strlwr_s_l(char *, size_t, _locale_t); } 
template < size_t _Size > inline errno_t __cdecl _strlwr_s_l ( char ( & _String ) [ _Size ], _locale_t _Locale ) { return _strlwr_s_l ( _String, _Size, _Locale ); }
extern "C" { extern __declspec( dllimport ) char *__cdecl _strlwr_l(char *, _locale_t); } 

extern "C" { extern __declspec( dllimport ) errno_t __cdecl strncat_s(char *, rsize_t, const char *, rsize_t); } 
#line 140 "C:\\Program Files\\Microsoft Visual Studio 8\\VC\\INCLUDE\\string.h"
template < size_t _Size > inline errno_t __cdecl strncat_s ( char ( & _Dest ) [ _Size ], const char * _Source, size_t _Count ) { return strncat_s ( _Dest, _Size, _Source, _Count ); }
#pragma warning(push)
#pragma warning(disable:6059)

extern "C" { extern __declspec( dllimport ) char *__cdecl strncat(char *, const char *, size_t); } 
#pragma warning(pop)
#line 149 "C:\\Program Files\\Microsoft Visual Studio 8\\VC\\INCLUDE\\string.h"
extern "C" { extern __declspec( dllimport ) int __cdecl strncmp(const char *, const char *, size_t); } 
#line 151 "C:\\Program Files\\Microsoft Visual Studio 8\\VC\\INCLUDE\\string.h"
extern "C" { extern __declspec( dllimport ) int __cdecl _strnicmp(const char *, const char *, size_t); } 
extern "C" { extern __declspec( dllimport ) int __cdecl _strnicmp_l(const char *, const char *, size_t, _locale_t); } 

extern "C" { extern __declspec( dllimport ) errno_t __cdecl strncpy_s(char *, rsize_t, const char *, rsize_t); } 
#line 156 "C:\\Program Files\\Microsoft Visual Studio 8\\VC\\INCLUDE\\string.h"
template < size_t _Size > inline errno_t __cdecl strncpy_s ( char ( & _Dest ) [ _Size ], const char * _Source, size_t _Count ) { return strncpy_s ( _Dest, _Size, _Source, _Count ); }
extern "C" { extern __declspec( dllimport ) char *__cdecl strncpy(char *, const char *, size_t); } 
extern "C" { extern __declspec( dllimport ) errno_t __cdecl _strnset_s(char *, size_t, int, size_t); } 
template < size_t _Size > inline errno_t __cdecl _strnset_s ( char ( & _Dest ) [ _Size ], int _Val, size_t _Count ) { return _strnset_s ( _Dest, _Size, _Val, _Count ); }
extern "C" { extern __declspec( dllimport ) char *__cdecl _strnset(char *, int, size_t); } 
extern "C" { extern __declspec( dllimport ) const char *__cdecl strpbrk(const char *, const char *); } 
extern "C" { extern __declspec( dllimport ) const char *__cdecl strrchr(const char *, int); } 
extern "C" { extern __declspec( dllimport ) char *__cdecl _strrev(char *); } 
extern "C" { extern __declspec( dllimport ) size_t __cdecl strspn(const char *, const char *); } 
extern "C" { extern __declspec( dllimport ) const char *__cdecl strstr(const char *, const char *); } 
extern "C" { extern __declspec( dllimport ) char *__cdecl strtok(char *, const char *); } 

extern "C" { extern __declspec( dllimport ) char *__cdecl strtok_s(char *, const char *, char **); } 
#line 170 "C:\\Program Files\\Microsoft Visual Studio 8\\VC\\INCLUDE\\string.h"
extern "C" { extern __declspec( dllimport ) errno_t __cdecl _strupr_s(char *, size_t); } 
template < size_t _Size > inline errno_t __cdecl _strupr_s ( char ( & _String ) [ _Size ] ) { return _strupr_s ( _String, _Size ); }
extern "C" { extern __declspec( dllimport ) char *__cdecl _strupr(char *); } 
extern "C" { extern __declspec( dllimport ) errno_t __cdecl _strupr_s_l(char *, size_t, _locale_t); } 
template < size_t _Size > inline errno_t __cdecl _strupr_s_l ( char ( & _String ) [ _Size ], _locale_t _Locale ) { return _strupr_s_l ( _String, _Size, _Locale ); }
extern "C" { extern __declspec( dllimport ) char *__cdecl _strupr_l(char *, _locale_t); } 
extern "C" { extern __declspec( dllimport ) size_t __cdecl strxfrm(char *, const char *, size_t); } 
extern "C" { extern __declspec( dllimport ) size_t __cdecl _strxfrm_l(char *, const char *, size_t, _locale_t); } 
#line 183 "C:\\Program Files\\Microsoft Visual Studio 8\\VC\\INCLUDE\\string.h"
inline char *__cdecl strchr(char *_Str, int _Ch) 
{ return (char *)strchr((const char *)_Str, _Ch); } 
inline char *__cdecl strpbrk(char *_Str, const char *_Control) 
{ return (char *)strpbrk((const char *)_Str, _Control); } 
inline char *__cdecl strrchr(char *_Str, int _Ch) 
{ return (char *)strrchr((const char *)_Str, _Ch); } 
inline char *__cdecl strstr(char *_Str, const char *_SubStr) 
{ return (char *)strstr((const char *)_Str, _SubStr); } 
#line 194 "C:\\Program Files\\Microsoft Visual Studio 8\\VC\\INCLUDE\\string.h"
inline void *__cdecl memchr(void *_Pv, int _C, size_t _N) 
{ return (void *)memchr((const void *)_Pv, _C, _N); } 
#line 207 "C:\\Program Files\\Microsoft Visual Studio 8\\VC\\INCLUDE\\string.h"
extern "C" { extern __declspec( dllimport ) char *__cdecl strdup(const char *); } 
#line 214 "C:\\Program Files\\Microsoft Visual Studio 8\\VC\\INCLUDE\\string.h"
extern "C" { extern __declspec( dllimport ) int __cdecl strcmpi(const char *, const char *); } 
extern "C" { extern __declspec( dllimport ) int __cdecl stricmp(const char *, const char *); } 
extern "C" { extern __declspec( dllimport ) char *__cdecl strlwr(char *); } 
extern "C" { extern __declspec( dllimport ) int __cdecl strnicmp(const char *, const char *, size_t); } 
extern "C" { extern __declspec( dllimport ) char *__cdecl strnset(char *, int, size_t); } 
extern "C" { extern __declspec( dllimport ) char *__cdecl strrev(char *); } 
extern "C" { extern char *__cdecl strset(char *, int); } 
extern "C" { extern __declspec( dllimport ) char *__cdecl strupr(char *); } 
#line 235 "C:\\Program Files\\Microsoft Visual Studio 8\\VC\\INCLUDE\\string.h"
extern "C" { extern __declspec( dllimport ) __wchar_t *__cdecl _wcsdup(const __wchar_t *); } 
#line 242 "C:\\Program Files\\Microsoft Visual Studio 8\\VC\\INCLUDE\\string.h"
extern "C" { extern __declspec( dllimport ) errno_t __cdecl wcscat_s(__wchar_t *, rsize_t, const __wchar_t *); } 
#line 244 "C:\\Program Files\\Microsoft Visual Studio 8\\VC\\INCLUDE\\string.h"
template < size_t _Size > inline errno_t __cdecl wcscat_s ( wchar_t ( & _Dest ) [ _Size ], const wchar_t * _Source ) { return wcscat_s ( _Dest, _Size, _Source ); }
extern "C" { extern __declspec( dllimport ) __wchar_t *__cdecl wcscat(__wchar_t *, const __wchar_t *); } 
extern "C" { extern __declspec( dllimport ) const __wchar_t *__cdecl wcschr(const __wchar_t *, __wchar_t); } 
extern "C" { extern __declspec( dllimport ) int __cdecl wcscmp(const __wchar_t *, const __wchar_t *); } 

extern "C" { extern __declspec( dllimport ) errno_t __cdecl wcscpy_s(__wchar_t *, rsize_t, const __wchar_t *); } 
#line 251 "C:\\Program Files\\Microsoft Visual Studio 8\\VC\\INCLUDE\\string.h"
template < size_t _Size > inline errno_t __cdecl wcscpy_s ( wchar_t ( & _Dest ) [ _Size ], const wchar_t * _Source ) { return wcscpy_s ( _Dest, _Size, _Source ); }
extern "C" { extern __declspec( dllimport ) __wchar_t *__cdecl wcscpy(__wchar_t *, const __wchar_t *); } 
extern "C" { extern __declspec( dllimport ) size_t __cdecl wcscspn(const __wchar_t *, const __wchar_t *); } 
extern "C" { extern __declspec( dllimport ) size_t __cdecl wcslen(const __wchar_t *); } 
extern "C" { extern __declspec( dllimport ) size_t __cdecl wcsnlen(const __wchar_t *, size_t); } 

extern "C" { static __inline size_t __cdecl wcsnlen_s(const __wchar_t *_Src, size_t _MaxCount) 
{ 
return wcsnlen(_Src, _MaxCount); 
} } 
#line 263 "C:\\Program Files\\Microsoft Visual Studio 8\\VC\\INCLUDE\\string.h"
extern "C" { extern __declspec( dllimport ) errno_t __cdecl wcsncat_s(__wchar_t *, rsize_t, const __wchar_t *, rsize_t); } 
#line 265 "C:\\Program Files\\Microsoft Visual Studio 8\\VC\\INCLUDE\\string.h"
template < size_t _Size > inline errno_t __cdecl wcsncat_s ( wchar_t ( & _Dest ) [ _Size ], const wchar_t * _Source, size_t _Count ) { return wcsncat_s ( _Dest, _Size, _Source, _Count ); }
extern "C" { extern __declspec( dllimport ) __wchar_t *__cdecl wcsncat(__wchar_t *, const __wchar_t *, size_t); } 
extern "C" { extern __declspec( dllimport ) int __cdecl wcsncmp(const __wchar_t *, const __wchar_t *, size_t); } 

extern "C" { extern __declspec( dllimport ) errno_t __cdecl wcsncpy_s(__wchar_t *, rsize_t, const __wchar_t *, rsize_t); } 
#line 271 "C:\\Program Files\\Microsoft Visual Studio 8\\VC\\INCLUDE\\string.h"
template < size_t _Size > inline errno_t __cdecl wcsncpy_s ( wchar_t ( & _Dest ) [ _Size ], const wchar_t * _Source, size_t _Count ) { return wcsncpy_s ( _Dest, _Size, _Source, _Count ); }
extern "C" { extern __declspec( dllimport ) __wchar_t *__cdecl wcsncpy(__wchar_t *, const __wchar_t *, size_t); } 
extern "C" { extern __declspec( dllimport ) const __wchar_t *__cdecl wcspbrk(const __wchar_t *, const __wchar_t *); } 
extern "C" { extern __declspec( dllimport ) const __wchar_t *__cdecl wcsrchr(const __wchar_t *, __wchar_t); } 
extern "C" { extern __declspec( dllimport ) size_t __cdecl wcsspn(const __wchar_t *, const __wchar_t *); } 
extern "C" { extern __declspec( dllimport ) const __wchar_t *__cdecl wcsstr(const __wchar_t *, const __wchar_t *); } 
extern "C" { extern __declspec( dllimport ) __wchar_t *__cdecl wcstok(__wchar_t *, const __wchar_t *); } 
extern "C" { extern __declspec( dllimport ) __wchar_t *__cdecl wcstok_s(__wchar_t *, const __wchar_t *, __wchar_t **); } 
extern "C" { extern __declspec( dllimport ) __wchar_t *__cdecl _wcserror(int); } 
extern "C" { extern __declspec( dllimport ) errno_t __cdecl _wcserror_s(__wchar_t *, size_t, int); } 
template < size_t _Size > inline errno_t __cdecl _wcserror_s ( wchar_t ( & _Buffer ) [ _Size ], int _Error ) { return _wcserror_s ( _Buffer, _Size, _Error ); }
extern "C" { extern __declspec( dllimport ) __wchar_t *__cdecl __wcserror(const __wchar_t *); } 
extern "C" { extern __declspec( dllimport ) errno_t __cdecl __wcserror_s(__wchar_t *, size_t, const __wchar_t *); } 
template < size_t _Size > inline errno_t __cdecl __wcserror_s ( wchar_t ( & _Buffer ) [ _Size ], const wchar_t * _ErrorMessage ) { return __wcserror_s ( _Buffer, _Size, _ErrorMessage ); }

extern "C" { extern __declspec( dllimport ) int __cdecl _wcsicmp(const __wchar_t *, const __wchar_t *); } 
extern "C" { extern __declspec( dllimport ) int __cdecl _wcsicmp_l(const __wchar_t *, const __wchar_t *, _locale_t); } 
extern "C" { extern __declspec( dllimport ) int __cdecl _wcsnicmp(const __wchar_t *, const __wchar_t *, size_t); } 
extern "C" { extern __declspec( dllimport ) int __cdecl _wcsnicmp_l(const __wchar_t *, const __wchar_t *, size_t, _locale_t); } 
extern "C" { extern __declspec( dllimport ) errno_t __cdecl _wcsnset_s(__wchar_t *, size_t, __wchar_t, size_t); } 
template < size_t _Size > inline errno_t __cdecl _wcsnset_s ( wchar_t ( & _Dst ) [ _Size ], wchar_t _Val, size_t _MaxCount ) { return _wcsnset_s ( _Dst, _Size, _Val, _MaxCount ); }
extern "C" { extern __declspec( dllimport ) __wchar_t *__cdecl _wcsnset(__wchar_t *, __wchar_t, size_t); } 
extern "C" { extern __declspec( dllimport ) __wchar_t *__cdecl _wcsrev(__wchar_t *); } 
extern "C" { extern __declspec( dllimport ) errno_t __cdecl _wcsset_s(__wchar_t *, size_t, __wchar_t); } 
template < size_t _Size > inline errno_t __cdecl _wcsset_s ( wchar_t ( & _Str ) [ _Size ], wchar_t _Val ) { return _wcsset_s ( _Str, _Size, _Val ); }
extern "C" { extern __declspec( dllimport ) __wchar_t *__cdecl _wcsset(__wchar_t *, __wchar_t); } 

extern "C" { extern __declspec( dllimport ) errno_t __cdecl _wcslwr_s(__wchar_t *, size_t); } 
template < size_t _Size > inline errno_t __cdecl _wcslwr_s ( wchar_t ( & _String ) [ _Size ] ) { return _wcslwr_s ( _String, _Size ); }
extern "C" { extern __declspec( dllimport ) __wchar_t *__cdecl _wcslwr(__wchar_t *); } 
extern "C" { extern __declspec( dllimport ) errno_t __cdecl _wcslwr_s_l(__wchar_t *, size_t, _locale_t); } 
template < size_t _Size > inline errno_t __cdecl _wcslwr_s_l ( wchar_t ( & _String ) [ _Size ], _locale_t _Locale ) { return _wcslwr_s_l ( _String, _Size, _Locale ); }
extern "C" { extern __declspec( dllimport ) __wchar_t *__cdecl _wcslwr_l(__wchar_t *, _locale_t); } 
extern "C" { extern __declspec( dllimport ) errno_t __cdecl _wcsupr_s(__wchar_t *, size_t); } 
template < size_t _Size > inline errno_t __cdecl _wcsupr_s ( wchar_t ( & _String ) [ _Size ] ) { return _wcsupr_s ( _String, _Size ); }
extern "C" { extern __declspec( dllimport ) __wchar_t *__cdecl _wcsupr(__wchar_t *); } 
extern "C" { extern __declspec( dllimport ) errno_t __cdecl _wcsupr_s_l(__wchar_t *, size_t, _locale_t); } 
template < size_t _Size > inline errno_t __cdecl _wcsupr_s_l ( wchar_t ( & _String ) [ _Size ], _locale_t _Locale ) { return _wcsupr_s_l ( _String, _Size, _Locale ); }
extern "C" { extern __declspec( dllimport ) __wchar_t *__cdecl _wcsupr_l(__wchar_t *, _locale_t); } 
extern "C" { extern __declspec( dllimport ) size_t __cdecl wcsxfrm(__wchar_t *, const __wchar_t *, size_t); } 
extern "C" { extern __declspec( dllimport ) size_t __cdecl _wcsxfrm_l(__wchar_t *, const __wchar_t *, size_t, _locale_t); } 
extern "C" { extern __declspec( dllimport ) int __cdecl wcscoll(const __wchar_t *, const __wchar_t *); } 
extern "C" { extern __declspec( dllimport ) int __cdecl _wcscoll_l(const __wchar_t *, const __wchar_t *, _locale_t); } 
extern "C" { extern __declspec( dllimport ) int __cdecl _wcsicoll(const __wchar_t *, const __wchar_t *); } 
extern "C" { extern __declspec( dllimport ) int __cdecl _wcsicoll_l(const __wchar_t *, const __wchar_t *, _locale_t); } 
extern "C" { extern __declspec( dllimport ) int __cdecl _wcsncoll(const __wchar_t *, const __wchar_t *, size_t); } 
extern "C" { extern __declspec( dllimport ) int __cdecl _wcsncoll_l(const __wchar_t *, const __wchar_t *, size_t, _locale_t); } 
extern "C" { extern __declspec( dllimport ) int __cdecl _wcsnicoll(const __wchar_t *, const __wchar_t *, size_t); } 
extern "C" { extern __declspec( dllimport ) int __cdecl _wcsnicoll_l(const __wchar_t *, const __wchar_t *, size_t, _locale_t); } 
#line 325 "C:\\Program Files\\Microsoft Visual Studio 8\\VC\\INCLUDE\\string.h"
inline __wchar_t *__cdecl wcschr(__wchar_t *_Str, __wchar_t _Ch) 
{ return (__wchar_t *)wcschr((const __wchar_t *)_Str, _Ch); } 
inline __wchar_t *__cdecl wcspbrk(__wchar_t *_Str, const __wchar_t *_Control) 
{ return (__wchar_t *)wcspbrk((const __wchar_t *)_Str, _Control); } 
inline __wchar_t *__cdecl wcsrchr(__wchar_t *_Str, __wchar_t _Ch) 
{ return (__wchar_t *)wcsrchr((const __wchar_t *)_Str, _Ch); } 
inline __wchar_t *__cdecl wcsstr(__wchar_t *_Str, const __wchar_t *_SubStr) 
{ return (__wchar_t *)wcsstr((const __wchar_t *)_Str, _SubStr); } 
#line 344 "C:\\Program Files\\Microsoft Visual Studio 8\\VC\\INCLUDE\\string.h"
extern "C" { extern __declspec( dllimport ) __wchar_t *__cdecl wcsdup(const __wchar_t *); } 
#line 354 "C:\\Program Files\\Microsoft Visual Studio 8\\VC\\INCLUDE\\string.h"
extern "C" { extern __declspec( dllimport ) int __cdecl wcsicmp(const __wchar_t *, const __wchar_t *); } 
extern "C" { extern __declspec( dllimport ) int __cdecl wcsnicmp(const __wchar_t *, const __wchar_t *, size_t); } 
extern "C" { extern __declspec( dllimport ) __wchar_t *__cdecl wcsnset(__wchar_t *, __wchar_t, size_t); } 
extern "C" { extern __declspec( dllimport ) __wchar_t *__cdecl wcsrev(__wchar_t *); } 
extern "C" { extern __declspec( dllimport ) __wchar_t *__cdecl wcsset(__wchar_t *, __wchar_t); } 
extern "C" { extern __declspec( dllimport ) __wchar_t *__cdecl wcslwr(__wchar_t *); } 
extern "C" { extern __declspec( dllimport ) __wchar_t *__cdecl wcsupr(__wchar_t *); } 
extern "C" { extern __declspec( dllimport ) int __cdecl wcsicoll(const __wchar_t *, const __wchar_t *); } 
#line 56 "c:\\cuda\\include\\common_functions.h"
extern "C" { extern __declspec( dllimport ) clock_t clock(); } 


extern "C" { extern void *memset(void *, int, size_t); } 


extern "C" { extern void *memcpy(void *, const void *, size_t); } 
#line 65 "c:\\cuda\\include\\math_functions.h"
extern "C" { extern int abs(int); } 

extern "C" { extern long labs(long); } 

extern "C" { extern __int64 llabs(__int64); } 

extern "C" { extern double fabs(double); } 

extern "C" { inline float fabsf(float); } 


extern "C" { extern int min(int, int); } 

extern "C" { extern unsigned umin(unsigned, unsigned); } 

extern "C" { extern __int64 llmin(__int64, __int64); } 

extern "C" { extern unsigned __int64 ullmin(unsigned __int64, unsigned __int64); } 

extern "C" { extern float fminf(float, float); } 

extern "C" { extern double fmin(double, double); } 


extern "C" { extern int max(int, int); } 

extern "C" { extern unsigned umax(unsigned, unsigned); } 

extern "C" { extern __int64 llmax(__int64, __int64); } 

extern "C" { extern unsigned __int64 ullmax(unsigned __int64, unsigned __int64); } 

extern "C" { extern float fmaxf(float, float); } 

extern "C" { extern double fmax(double, double); } 


extern "C" { extern double sin(double); } 

extern "C" { inline float sinf(float); } 


extern "C" { extern double cos(double); } 

extern "C" { inline float cosf(float); } 


extern "C" { extern void sincos(double, double *, double *); } 

extern "C" { extern void sincosf(float, float *, float *); } 


extern "C" { extern double tan(double); } 

extern "C" { inline float tanf(float); } 


extern "C" { extern double sqrt(double); } 

extern "C" { inline float sqrtf(float); } 


extern "C" { extern double rsqrt(double); } 

extern "C" { extern float rsqrtf(float); } 


extern "C" { extern double exp2(double); } 

extern "C" { extern float exp2f(float); } 


extern "C" { extern double exp10(double); } 

extern "C" { extern float exp10f(float); } 


extern "C" { extern double expm1(double); } 

extern "C" { extern float expm1f(float); } 


extern "C" { extern double log2(double); } 

extern "C" { extern float log2f(float); } 


extern "C" { extern double log10(double); } 

extern "C" { inline float log10f(float); } 


extern "C" { extern double log(double); } 

extern "C" { inline float logf(float); } 


extern "C" { extern double log1p(double); } 

extern "C" { extern float log1pf(float); } 


extern "C" { extern __declspec( dllimport ) double floor(double); } 

extern "C" { inline float floorf(float); } 


extern "C" { extern double exp(double); } 

extern "C" { inline float expf(float); } 


extern "C" { extern double cosh(double); } 

extern "C" { inline float coshf(float); } 


extern "C" { extern double sinh(double); } 

extern "C" { inline float sinhf(float); } 


extern "C" { extern double tanh(double); } 

extern "C" { inline float tanhf(float); } 


extern "C" { extern double acosh(double); } 

extern "C" { extern float acoshf(float); } 


extern "C" { extern double asinh(double); } 

extern "C" { extern float asinhf(float); } 


extern "C" { extern double atanh(double); } 

extern "C" { extern float atanhf(float); } 


extern "C" { extern __declspec( dllimport ) double ldexp(double, int); } 

extern "C" { inline float ldexpf(float, int); } 


extern "C" { extern double logb(double); } 

extern "C" { extern float logbf(float); } 


extern "C" { extern int ilogb(double); } 

extern "C" { extern int ilogbf(float); } 


extern "C" { extern double scalbn(double, int); } 

extern "C" { extern float scalbnf(float, int); } 


extern "C" { extern double scalbln(double, long); } 

extern "C" { extern float scalblnf(float, long); } 


extern "C" { extern __declspec( dllimport ) double frexp(double, int *); } 

extern "C" { inline float frexpf(float, int *); } 


extern "C" { extern double round(double); } 

extern "C" { extern float roundf(float); } 


extern "C" { extern long lround(double); } 

extern "C" { extern long lroundf(float); } 


extern "C" { extern __int64 llround(double); } 

extern "C" { extern __int64 llroundf(float); } 


extern "C" { extern double rint(double); } 

extern "C" { extern float rintf(float); } 


extern "C" { extern long lrint(double); } 

extern "C" { extern long lrintf(float); } 


extern "C" { extern __int64 llrint(double); } 

extern "C" { extern __int64 llrintf(float); } 


extern "C" { extern double nearbyint(double); } 

extern "C" { extern float nearbyintf(float); } 


extern "C" { extern __declspec( dllimport ) double ceil(double); } 

extern "C" { inline float ceilf(float); } 


extern "C" { extern double trunc(double); } 

extern "C" { extern float truncf(float); } 


extern "C" { extern double fdim(double, double); } 

extern "C" { extern float fdimf(float, float); } 


extern "C" { extern double atan2(double, double); } 

extern "C" { inline float atan2f(float, float); } 


extern "C" { extern double atan(double); } 

extern "C" { inline float atanf(float); } 


extern "C" { extern double asin(double); } 

extern "C" { inline float asinf(float); } 


extern "C" { extern double acos(double); } 

extern "C" { inline float acosf(float); } 


extern "C" { extern __declspec( dllimport ) double hypot(double, double); } 

extern "C" { extern float hypotf(float, float); } 


extern "C" { extern double cbrt(double); } 

extern "C" { extern float cbrtf(float); } 


extern "C" { extern double pow(double, double); } 

extern "C" { inline float powf(float, float); } 


extern "C" { extern __declspec( dllimport ) double modf(double, double *); } 

extern "C" { inline float modff(float, float *); } 


extern "C" { extern double fmod(double, double); } 

extern "C" { inline float fmodf(float, float); } 


extern "C" { extern double remainder(double, double); } 

extern "C" { extern float remainderf(float, float); } 


extern "C" { extern double remquo(double, double, int *); } 

extern "C" { extern float remquof(float, float, int *); } 


extern "C" { extern double erf(double); } 

extern "C" { extern float erff(float); } 


extern "C" { extern double erfinv(double); } 

extern "C" { extern float erfinvf(float); } 


extern "C" { extern double erfc(double); } 

extern "C" { extern float erfcf(float); } 


extern "C" { extern double erfcinv(double); } 

extern "C" { extern float erfcinvf(float); } 


extern "C" { extern double lgamma(double); } 

extern "C" { extern float lgammaf(float); } 


extern "C" { extern double tgamma(double); } 

extern "C" { extern float tgammaf(float); } 


extern "C" { extern double copysign(double, double); } 

extern "C" { extern float copysignf(float, float); } 


extern "C" { extern double nextafter(double, double); } 

extern "C" { extern float nextafterf(float, float); } 


extern "C" { extern double nan(const char *); } 

extern "C" { extern float nanf(const char *); } 


extern "C" { extern int __isinf(double); } 

extern "C" { extern int __isinff(float); } 


extern "C" { extern int __isnan(double); } 

extern "C" { extern int __isnanf(float); } 
#line 408 "c:\\cuda\\include\\math_functions.h"
extern "C" { extern int __finite(double); } 

extern "C" { extern int __finitef(float); } 

extern "C" { extern int __signbit(double); } 
#line 417 "c:\\cuda\\include\\math_functions.h"
extern "C" { extern int __signbitf(float); } 


extern "C" { extern double fma(double, double, double); } 

extern "C" { extern float fmaf(float, float, float); } 
#line 25 "C:\\Program Files\\Microsoft Visual Studio 8\\VC\\INCLUDE\\math.h"
#pragma pack ( push, 8 )
#line 39 "C:\\Program Files\\Microsoft Visual Studio 8\\VC\\INCLUDE\\math.h"
extern "C" { struct _exception { 
int type; 
char *name; 
double arg1; 
double arg2; 
double retval; 
}; }
#line 56 "C:\\Program Files\\Microsoft Visual Studio 8\\VC\\INCLUDE\\math.h"
extern "C" { struct _complex { 
double x; double y; 
}; }
#line 90 "C:\\Program Files\\Microsoft Visual Studio 8\\VC\\INCLUDE\\math.h"
extern "C" __declspec( dllimport ) double _HUGE; 
#line 103 "C:\\Program Files\\Microsoft Visual Studio 8\\VC\\INCLUDE\\math.h"
extern "C" { extern int __cdecl abs(int); } 
extern "C" { extern long __cdecl labs(long); } 
#line 107 "C:\\Program Files\\Microsoft Visual Studio 8\\VC\\INCLUDE\\math.h"
extern "C" { extern double __cdecl acos(double); } 
extern "C" { extern double __cdecl asin(double); } 
extern "C" { extern double __cdecl atan(double); } 
extern "C" { extern double __cdecl atan2(double, double); } 

extern "C" { extern __declspec( dllimport ) double __cdecl _copysign(double, double); } 
extern "C" { extern __declspec( dllimport ) double __cdecl _chgsign(double); } 
#line 116 "C:\\Program Files\\Microsoft Visual Studio 8\\VC\\INCLUDE\\math.h"
extern "C" { extern double __cdecl cos(double); } 
extern "C" { extern double __cdecl cosh(double); } 
extern "C" { extern double __cdecl exp(double); } 
extern "C" { extern double __cdecl fabs(double); } 
extern "C" { extern double __cdecl fmod(double, double); } 
extern "C" { extern double __cdecl log(double); } 
extern "C" { extern double __cdecl log10(double); } 
extern "C" { extern double __cdecl pow(double, double); } 
extern "C" { extern double __cdecl sin(double); } 
extern "C" { extern double __cdecl sinh(double); } 
extern "C" { extern double __cdecl tan(double); } 
extern "C" { extern double __cdecl tanh(double); } 
extern "C" { extern double __cdecl sqrt(double); } 


extern "C" { extern __declspec( dllimport ) double __cdecl atof(const char *); } 
extern "C" { extern __declspec( dllimport ) double __cdecl _atof_l(const char *, _locale_t); } 
#line 135 "C:\\Program Files\\Microsoft Visual Studio 8\\VC\\INCLUDE\\math.h"
extern "C" { extern __declspec( dllimport ) double __cdecl _cabs(_complex); } 
extern "C" { extern __declspec( dllimport ) double __cdecl ceil(double); } 
extern "C" { extern __declspec( dllimport ) double __cdecl floor(double); } 
extern "C" { extern __declspec( dllimport ) double __cdecl frexp(double, int *); } 
extern "C" { extern __declspec( dllimport ) double __cdecl _hypot(double, double); } 
extern "C" { extern __declspec( dllimport ) double __cdecl _j0(double); } 
extern "C" { extern __declspec( dllimport ) double __cdecl _j1(double); } 
extern "C" { extern __declspec( dllimport ) double __cdecl _jn(int, double); } 
extern "C" { extern __declspec( dllimport ) double __cdecl ldexp(double, int); } 
#line 149 "C:\\Program Files\\Microsoft Visual Studio 8\\VC\\INCLUDE\\math.h"
extern "C" { extern int __cdecl _matherr(_exception *); } 
#line 152 "C:\\Program Files\\Microsoft Visual Studio 8\\VC\\INCLUDE\\math.h"
extern "C" { extern __declspec( dllimport ) double __cdecl modf(double, double *); } 

extern "C" { extern __declspec( dllimport ) double __cdecl _y0(double); } 
extern "C" { extern __declspec( dllimport ) double __cdecl _y1(double); } 
extern "C" { extern __declspec( dllimport ) double __cdecl _yn(int, double); } 




extern "C" { extern __declspec( dllimport ) int __cdecl _set_SSE2_enable(int); } 
extern "C" { extern __declspec( dllimport ) float __cdecl _hypotf(float, float); } 
#line 317 "C:\\Program Files\\Microsoft Visual Studio 8\\VC\\INCLUDE\\math.h"
extern "C" { inline long double acosl(long double _X) 
{ return acos((double)_X); } } 
extern "C" { inline long double asinl(long double _X) 
{ return asin((double)_X); } } 
extern "C" { inline long double atanl(long double _X) 
{ return atan((double)_X); } } 
extern "C" { inline long double atan2l(long double _X, long double _Y) 
{ return atan2((double)_X, (double)_Y); } } 
extern "C" { inline long double ceill(long double _X) 
{ return ceil((double)_X); } } 
extern "C" { inline long double cosl(long double _X) 
{ return cos((double)_X); } } 
extern "C" { inline long double coshl(long double _X) 
{ return cosh((double)_X); } } 
extern "C" { inline long double expl(long double _X) 
{ return exp((double)_X); } } 
extern "C" { inline long double fabsl(long double _X) 
{ return fabs((double)_X); } } 
extern "C" { inline long double floorl(long double _X) 
{ return floor((double)_X); } } 
extern "C" { inline long double fmodl(long double _X, long double _Y) 
{ return fmod((double)_X, (double)_Y); } } 
extern "C" { inline long double frexpl(long double _X, int *_Y) 
{ return frexp((double)_X, _Y); } } 
extern "C" { inline long double ldexpl(long double _X, int _Y) 
{ return ldexp((double)_X, _Y); } } 
extern "C" { inline long double logl(long double _X) 
{ return log((double)_X); } } 
extern "C" { inline long double log10l(long double _X) 
{ return log10((double)_X); } } 
extern "C" { inline long double modfl(long double _X, long double *_Y) 
{ auto double _Di; auto double _Df = modf((double)_X, &_Di); 
(*_Y) = (long double)_Di; 
return _Df; } } 
extern "C" { inline long double powl(long double _X, long double _Y) 
{ return pow((double)_X, (double)_Y); } } 
extern "C" { inline long double sinl(long double _X) 
{ return sin((double)_X); } } 
extern "C" { inline long double sinhl(long double _X) 
{ return sinh((double)_X); } } 
extern "C" { inline long double sqrtl(long double _X) 
{ return sqrt((double)_X); } } 

extern "C" { inline long double tanl(long double _X) 
{ return tan((double)_X); } } 
#line 366 "C:\\Program Files\\Microsoft Visual Studio 8\\VC\\INCLUDE\\math.h"
extern "C" { inline long double tanhl(long double _X) 
{ return tanh((double)_X); } } 

extern "C" { inline long double _chgsignl(long double _Number) 
{ 
return _chgsign(static_cast< double>(_Number)); 
} } 

extern "C" { inline long double _copysignl(long double _Number, long double _Sign) 
{ 
return _copysign(static_cast< double>(_Number), static_cast< double>(_Sign)); 
} } 

extern "C" { inline float frexpf(float _X, int *_Y) 
{ return (float)frexp((double)_X, _Y); } } 


extern "C" { inline float fabsf(float _X) 
{ return (float)fabs((double)_X); } } 
extern "C" { inline float ldexpf(float _X, int _Y) 
{ return (float)ldexp((double)_X, _Y); } } 

extern "C" { inline float acosf(float _X) 
{ return (float)acos((double)_X); } } 
extern "C" { inline float asinf(float _X) 
{ return (float)asin((double)_X); } } 
extern "C" { inline float atanf(float _X) 
{ return (float)atan((double)_X); } } 
extern "C" { inline float atan2f(float _X, float _Y) 
{ return (float)atan2((double)_X, (double)_Y); } } 
extern "C" { inline float ceilf(float _X) 
{ return (float)ceil((double)_X); } } 
extern "C" { inline float cosf(float _X) 
{ return (float)cos((double)_X); } } 
extern "C" { inline float coshf(float _X) 
{ return (float)cosh((double)_X); } } 
extern "C" { inline float expf(float _X) 
{ return (float)exp((double)_X); } } 
extern "C" { inline float floorf(float _X) 
{ return (float)floor((double)_X); } } 
extern "C" { inline float fmodf(float _X, float _Y) 
{ return (float)fmod((double)_X, (double)_Y); } } 
extern "C" { inline float logf(float _X) 
{ return (float)log((double)_X); } } 
extern "C" { inline float log10f(float _X) 
{ return (float)log10((double)_X); } } 
extern "C" { inline float modff(float _X, float *_Y) 
{ auto double _Di; auto double _Df = modf((double)_X, &_Di); 
(*_Y) = (float)_Di; 
return (float)_Df; } } 
extern "C" { inline float powf(float _X, float _Y) 
{ return (float)pow((double)_X, (double)_Y); } } 
extern "C" { inline float sinf(float _X) 
{ return (float)sin((double)_X); } } 
extern "C" { inline float sinhf(float _X) 
{ return (float)sinh((double)_X); } } 
extern "C" { inline float sqrtf(float _X) 
{ return (float)sqrt((double)_X); } } 
extern "C" { inline float tanf(float _X) 
{ return (float)tan((double)_X); } } 
extern "C" { inline float tanhf(float _X) 
{ return (float)tanh((double)_X); } } 
#line 449 "C:\\Program Files\\Microsoft Visual Studio 8\\VC\\INCLUDE\\math.h"
extern "C" __declspec( dllimport ) double HUGE; 
#line 454 "C:\\Program Files\\Microsoft Visual Studio 8\\VC\\INCLUDE\\math.h"
extern "C" { extern __declspec( dllimport ) double __cdecl cabs(_complex); } 
extern "C" { extern __declspec( dllimport ) double __cdecl hypot(double, double); } 
extern "C" { extern __declspec( dllimport ) double __cdecl j0(double); } 
extern "C" { extern __declspec( dllimport ) double __cdecl j1(double); } 
extern "C" { extern __declspec( dllimport ) double __cdecl jn(int, double); } 
extern "C" { extern __declspec( dllimport ) double __cdecl y0(double); } 
extern "C" { extern __declspec( dllimport ) double __cdecl y1(double); } 
extern "C" { extern __declspec( dllimport ) double __cdecl yn(int, double); } 
#line 472 "C:\\Program Files\\Microsoft Visual Studio 8\\VC\\INCLUDE\\math.h"
template < class _Ty > inline
        _Ty _Pow_int ( _Ty _X, int _Y )
        { unsigned int _N;
        if ( _Y >= 0 )
                _N = ( unsigned int ) _Y;
        else
                _N = ( unsigned int ) ( - _Y );
        for ( _Ty _Z = _Ty ( 1 );; _X *= _X )
                { if ( ( _N & 1 ) != 0 )
                        _Z *= _X;
                if ( ( _N >>= 1 ) == 0 )
                        return ( _Y < 0 ? _Ty ( 1 ) / _Z : _Z ); } }

inline long __cdecl abs(long _X) 
{ return labs(_X); } 
inline double __cdecl abs(double _X) 
{ return fabs(_X); } 
inline double __cdecl pow(double _X, int _Y) 
{ return _Pow_int(_X, _Y); } 
inline float __cdecl abs(float _X) 
{ return fabsf(_X); } 
inline float __cdecl acos(float _X) 
{ return acosf(_X); } 
inline float __cdecl asin(float _X) 
{ return asinf(_X); } 
inline float __cdecl atan(float _X) 
{ return atanf(_X); } 
inline float __cdecl atan2(float _Y, float _X) 
{ return atan2f(_Y, _X); } 
inline float __cdecl ceil(float _X) 
{ return ceilf(_X); } 
inline float __cdecl cos(float _X) 
{ return cosf(_X); } 
inline float __cdecl cosh(float _X) 
{ return coshf(_X); } 
inline float __cdecl exp(float _X) 
{ return expf(_X); } 
inline float __cdecl fabs(float _X) 
{ return fabsf(_X); } 
inline float __cdecl floor(float _X) 
{ return floorf(_X); } 
inline float __cdecl fmod(float _X, float _Y) 
{ return fmodf(_X, _Y); } 
inline float __cdecl frexp(float _X, int *_Y) 
{ return frexpf(_X, _Y); } 
inline float __cdecl ldexp(float _X, int _Y) 
{ return ldexpf(_X, _Y); } 
inline float __cdecl log(float _X) 
{ return logf(_X); } 
inline float __cdecl log10(float _X) 
{ return log10f(_X); } 
inline float __cdecl modf(float _X, float *_Y) 
{ return modff(_X, _Y); } 
inline float __cdecl pow(float _X, float _Y) 
{ return powf(_X, _Y); } 
inline float __cdecl pow(float _X, int _Y) 
{ return _Pow_int(_X, _Y); } 
inline float __cdecl sin(float _X) 
{ return sinf(_X); } 
inline float __cdecl sinh(float _X) 
{ return sinhf(_X); } 
inline float __cdecl sqrt(float _X) 
{ return sqrtf(_X); } 
inline float __cdecl tan(float _X) 
{ return tanf(_X); } 
inline float __cdecl tanh(float _X) 
{ return tanhf(_X); } 
inline long double __cdecl abs(long double _X) 
{ return fabsl(_X); } 
inline long double __cdecl acos(long double _X) 
{ return acosl(_X); } 
inline long double __cdecl asin(long double _X) 
{ return asinl(_X); } 
inline long double __cdecl atan(long double _X) 
{ return atanl(_X); } 
inline long double __cdecl atan2(long double _Y, long double _X) 
{ return atan2l(_Y, _X); } 
inline long double __cdecl ceil(long double _X) 
{ return ceill(_X); } 
inline long double __cdecl cos(long double _X) 
{ return cosl(_X); } 
inline long double __cdecl cosh(long double _X) 
{ return coshl(_X); } 
inline long double __cdecl exp(long double _X) 
{ return expl(_X); } 
inline long double __cdecl fabs(long double _X) 
{ return fabsl(_X); } 
inline long double __cdecl floor(long double _X) 
{ return floorl(_X); } 
inline long double __cdecl fmod(long double _X, long double _Y) 
{ return fmodl(_X, _Y); } 
inline long double __cdecl frexp(long double _X, int *_Y) 
{ return frexpl(_X, _Y); } 
inline long double __cdecl ldexp(long double _X, int _Y) 
{ return ldexpl(_X, _Y); } 
inline long double __cdecl log(long double _X) 
{ return logl(_X); } 
inline long double __cdecl log10(long double _X) 
{ return log10l(_X); } 
inline long double __cdecl modf(long double _X, long double *_Y) 
{ return modfl(_X, _Y); } 
inline long double __cdecl pow(long double _X, long double _Y) 
{ return powl(_X, _Y); } 
inline long double __cdecl pow(long double _X, int _Y) 
{ return _Pow_int(_X, _Y); } 
inline long double __cdecl sin(long double _X) 
{ return sinl(_X); } 
inline long double __cdecl sinh(long double _X) 
{ return sinhl(_X); } 
inline long double __cdecl sqrt(long double _X) 
{ return sqrtl(_X); } 
inline long double __cdecl tan(long double _X) 
{ return tanl(_X); } 
inline long double __cdecl tanh(long double _X) 
{ return tanhl(_X); } 
#line 592 "C:\\Program Files\\Microsoft Visual Studio 8\\VC\\INCLUDE\\math.h"
#pragma pack ( pop )
#line 31 "C:\\Program Files\\Microsoft Visual Studio 8\\VC\\INCLUDE\\stdlib.h"
#pragma pack ( push, 8 )
#line 56 "C:\\Program Files\\Microsoft Visual Studio 8\\VC\\INCLUDE\\stdlib.h"
extern "C" { typedef int (__cdecl *_onexit_t)(void); }
#line 82 "C:\\Program Files\\Microsoft Visual Studio 8\\VC\\INCLUDE\\stdlib.h"
extern "C" { typedef 
#line 79 "C:\\Program Files\\Microsoft Visual Studio 8\\VC\\INCLUDE\\stdlib.h"
struct _div_t { 
int quot; 
int rem; 
} div_t; }




extern "C" { typedef 
#line 84 "C:\\Program Files\\Microsoft Visual Studio 8\\VC\\INCLUDE\\stdlib.h"
struct _ldiv_t { 
long quot; 
long rem; 
} ldiv_t; }
#line 101 "C:\\Program Files\\Microsoft Visual Studio 8\\VC\\INCLUDE\\stdlib.h"
#pragma pack ( 4 )


extern "C" { typedef 
#line 102 "C:\\Program Files\\Microsoft Visual Studio 8\\VC\\INCLUDE\\stdlib.h"
struct _LDOUBLE { 
unsigned char ld[10]; 
} _LDOUBLE; }
#pragma pack ( )
#line 123 "C:\\Program Files\\Microsoft Visual Studio 8\\VC\\INCLUDE\\stdlib.h"
extern "C" { typedef 
#line 121 "C:\\Program Files\\Microsoft Visual Studio 8\\VC\\INCLUDE\\stdlib.h"
struct _CRT_DOUBLE { 
double x; 
} _CRT_DOUBLE; }



extern "C" { typedef 
#line 125 "C:\\Program Files\\Microsoft Visual Studio 8\\VC\\INCLUDE\\stdlib.h"
struct _CRT_FLOAT { 
float f; 
} _CRT_FLOAT; }
#line 138 "C:\\Program Files\\Microsoft Visual Studio 8\\VC\\INCLUDE\\stdlib.h"
extern "C" { typedef 
#line 133 "C:\\Program Files\\Microsoft Visual Studio 8\\VC\\INCLUDE\\stdlib.h"
struct _LONGDOUBLE { 



long double x; 
} _LONGDOUBLE; }



#pragma pack ( 4 )


extern "C" { typedef 
#line 143 "C:\\Program Files\\Microsoft Visual Studio 8\\VC\\INCLUDE\\stdlib.h"
struct _LDBL12 { 
unsigned char ld12[12]; 
} _LDBL12; }
#pragma pack ( )
#line 166 "C:\\Program Files\\Microsoft Visual Studio 8\\VC\\INCLUDE\\stdlib.h"
extern "C" __declspec( dllimport ) int __mb_cur_max; 
#line 171 "C:\\Program Files\\Microsoft Visual Studio 8\\VC\\INCLUDE\\stdlib.h"
extern "C" { extern __declspec( dllimport ) int __cdecl ___mb_cur_max_func(); } 
extern "C" { extern __declspec( dllimport ) int __cdecl ___mb_cur_max_l_func(_locale_t); } 
#line 211 "C:\\Program Files\\Microsoft Visual Studio 8\\VC\\INCLUDE\\stdlib.h"
extern "C" { typedef void (__cdecl *_purecall_handler)(void); }


extern "C" { extern __declspec( dllimport ) _purecall_handler __cdecl _set_purecall_handler(_purecall_handler); } 
extern "C" { extern __declspec( dllimport ) _purecall_handler __cdecl _get_purecall_handler(); } 
#line 239 "C:\\Program Files\\Microsoft Visual Studio 8\\VC\\INCLUDE\\stdlib.h"
extern "C" { typedef void (__cdecl *_invalid_parameter_handler)(const __wchar_t *, const __wchar_t *, const __wchar_t *, unsigned, uintptr_t); }


extern "C" { extern __declspec( dllimport ) _invalid_parameter_handler __cdecl _set_invalid_parameter_handler(_invalid_parameter_handler); } 
extern "C" { extern __declspec( dllimport ) _invalid_parameter_handler __cdecl _get_invalid_parameter_handler(); } 
#line 274 "C:\\Program Files\\Microsoft Visual Studio 8\\VC\\INCLUDE\\stdlib.h"
extern "C" { extern __declspec( dllimport ) unsigned long *__cdecl __doserrno(); } 


extern "C" { extern errno_t __cdecl _set_doserrno(unsigned long); } 
extern "C" { extern errno_t __cdecl _get_doserrno(unsigned long *); } 


extern "C" { extern __declspec( dllimport ) char **__cdecl __sys_errlist(); } 


extern "C" { extern __declspec( dllimport ) int *__cdecl __sys_nerr(); } 




extern "C" { extern __declspec( dllimport ) int *__cdecl __p___argc(); } 
extern "C" { extern __declspec( dllimport ) char ***__cdecl __p___argv(); } 
extern "C" { extern __declspec( dllimport ) __wchar_t ***__cdecl __p___wargv(); } 
extern "C" { extern __declspec( dllimport ) char ***__cdecl __p__environ(); } 
extern "C" { extern __declspec( dllimport ) __wchar_t ***__cdecl __p__wenviron(); } 
extern "C" { extern __declspec( dllimport ) char **__cdecl __p__pgmptr(); } 
extern "C" { extern __declspec( dllimport ) __wchar_t **__cdecl __p__wpgmptr(); } 
#line 301 "C:\\Program Files\\Microsoft Visual Studio 8\\VC\\INCLUDE\\stdlib.h"
extern "C" __declspec( dllimport ) int __argc; 
extern "C" __declspec( dllimport ) char **__argv; 
extern "C" __declspec( dllimport ) __wchar_t **__wargv; 
#line 317 "C:\\Program Files\\Microsoft Visual Studio 8\\VC\\INCLUDE\\stdlib.h"
extern "C" __declspec( dllimport ) char **_environ; 
extern "C" __declspec( dllimport ) __wchar_t **_wenviron; 
#line 321 "C:\\Program Files\\Microsoft Visual Studio 8\\VC\\INCLUDE\\stdlib.h"
extern "C" __declspec( dllimport ) char *_pgmptr; 
extern "C" __declspec( dllimport ) __wchar_t *_wpgmptr; 
#line 339 "C:\\Program Files\\Microsoft Visual Studio 8\\VC\\INCLUDE\\stdlib.h"
extern "C" { extern errno_t __cdecl _get_pgmptr(char **); } 
extern "C" { extern errno_t __cdecl _get_wpgmptr(__wchar_t **); } 



extern "C" __declspec( dllimport ) int _fmode; 
#line 350 "C:\\Program Files\\Microsoft Visual Studio 8\\VC\\INCLUDE\\stdlib.h"
extern "C" { extern __declspec( dllimport ) errno_t __cdecl _set_fmode(int); } 
extern "C" { extern __declspec( dllimport ) errno_t __cdecl _get_fmode(int *); } 



#pragma warning(push)
#pragma warning(disable:4141)


extern "C" __declspec( dllimport ) __declspec(deprecated("This function or variable has been superceded by newer library or operating system functionality. Consider using GetVersionEx instead. See online help for details.")) unsigned _osplatform; 
extern "C" __declspec( dllimport ) __declspec(deprecated("This function or variable has been superceded by newer library or operating system functionality. Consider using GetVersionEx instead. See online help for details.")) unsigned _osver; 
extern "C" __declspec( dllimport ) __declspec(deprecated("This function or variable has been superceded by newer library or operating system functionality. Consider using GetVersionEx instead. See online help for details.")) unsigned _winver; 
extern "C" __declspec( dllimport ) __declspec(deprecated("This function or variable has been superceded by newer library or operating system functionality. Consider using GetVersionEx instead. See online help for details.")) unsigned _winmajor; 
extern "C" __declspec( dllimport ) __declspec(deprecated("This function or variable has been superceded by newer library or operating system functionality. Consider using GetVersionEx instead. See online help for details.")) unsigned _winminor; 
#line 380 "C:\\Program Files\\Microsoft Visual Studio 8\\VC\\INCLUDE\\stdlib.h"
#pragma warning(pop)

extern "C" { extern __declspec(deprecated("This function or variable has been superceded by newer library or operating system functionality. Consider using GetVersionEx instead. See online help for details.")) errno_t __cdecl _get_osplatform(unsigned *); } 
extern "C" { extern __declspec(deprecated("This function or variable has been superceded by newer library or operating system functionality. Consider using GetVersionEx instead. See online help for details.")) errno_t __cdecl _get_osver(unsigned *); } 
extern "C" { extern __declspec(deprecated("This function or variable has been superceded by newer library or operating system functionality. Consider using GetVersionEx instead. See online help for details.")) errno_t __cdecl _get_winver(unsigned *); } 
extern "C" { extern __declspec(deprecated("This function or variable has been superceded by newer library or operating system functionality. Consider using GetVersionEx instead. See online help for details.")) errno_t __cdecl _get_winmajor(unsigned *); } 
extern "C" { extern __declspec(deprecated("This function or variable has been superceded by newer library or operating system functionality. Consider using GetVersionEx instead. See online help for details.")) errno_t __cdecl _get_winminor(unsigned *); } 
#line 395 "C:\\Program Files\\Microsoft Visual Studio 8\\VC\\INCLUDE\\stdlib.h"
template<class _CountofType, size_t _SizeOfArray> extern char (*__countof_helper(_CountofType (&)[_SizeOfArray]))[_SizeOfArray]; 
#line 406 "C:\\Program Files\\Microsoft Visual Studio 8\\VC\\INCLUDE\\stdlib.h"
extern "C" { extern __declspec( dllimport noreturn ) void __cdecl exit(int); } 
extern "C" { extern __declspec( dllimport noreturn ) void __cdecl _exit(int); } 
extern "C" { extern __declspec( dllimport ) void __cdecl abort(); } 
#line 411 "C:\\Program Files\\Microsoft Visual Studio 8\\VC\\INCLUDE\\stdlib.h"
extern "C" { extern __declspec( dllimport ) unsigned __cdecl _set_abort_behavior(unsigned, unsigned); } 
#line 420 "C:\\Program Files\\Microsoft Visual Studio 8\\VC\\INCLUDE\\stdlib.h"
extern "C" { extern __int64 __cdecl _abs64(__int64); } 
#line 446 "C:\\Program Files\\Microsoft Visual Studio 8\\VC\\INCLUDE\\stdlib.h"
extern "C" { extern int __cdecl atexit(void (__cdecl *)(void)); } 
#line 453 "C:\\Program Files\\Microsoft Visual Studio 8\\VC\\INCLUDE\\stdlib.h"
extern "C" { extern __declspec( dllimport ) int __cdecl atoi(const char *); } 
extern "C" { extern __declspec( dllimport ) int __cdecl _atoi_l(const char *, _locale_t); } 
extern "C" { extern __declspec( dllimport ) long __cdecl atol(const char *); } 
extern "C" { extern __declspec( dllimport ) long __cdecl _atol_l(const char *, _locale_t); } 



extern "C" { extern __declspec( dllimport ) void *__cdecl bsearch_s(const void *, const void *, rsize_t, rsize_t, int (__cdecl *)(void *, const void *, const void *), void *); } 
#line 464 "C:\\Program Files\\Microsoft Visual Studio 8\\VC\\INCLUDE\\stdlib.h"
extern "C" { extern __declspec( dllimport ) void *__cdecl bsearch(const void *, const void *, size_t, size_t, int (__cdecl *)(const void *, const void *)); } 




extern "C" { extern __declspec( dllimport ) void __cdecl qsort_s(void *, rsize_t, rsize_t, int (__cdecl *)(void *, const void *, const void *), void *); } 
#line 473 "C:\\Program Files\\Microsoft Visual Studio 8\\VC\\INCLUDE\\stdlib.h"
extern "C" { extern __declspec( dllimport ) void __cdecl qsort(void *, size_t, size_t, int (__cdecl *)(const void *, const void *)); } 
#line 477 "C:\\Program Files\\Microsoft Visual Studio 8\\VC\\INCLUDE\\stdlib.h"
extern "C" { extern unsigned short __cdecl _byteswap_ushort(unsigned short); } 
extern "C" { extern unsigned long __cdecl _byteswap_ulong(unsigned long); } 

extern "C" { extern unsigned __int64 __cdecl _byteswap_uint64(unsigned __int64); } 
#line 482 "C:\\Program Files\\Microsoft Visual Studio 8\\VC\\INCLUDE\\stdlib.h"
extern "C" { extern __declspec( dllimport ) div_t __cdecl div(int, int); } 
extern "C" { extern __declspec( dllimport ) char *__cdecl getenv(const char *); } 

extern "C" { extern __declspec( dllimport ) errno_t __cdecl getenv_s(size_t *, char *, rsize_t, const char *); } 
#line 487 "C:\\Program Files\\Microsoft Visual Studio 8\\VC\\INCLUDE\\stdlib.h"
template < size_t _Size > inline errno_t __cdecl getenv_s ( size_t * _ReturnSize, char ( & _Dest ) [ _Size ], const char * _VarName ) { return getenv_s ( _ReturnSize, _Dest, _Size, _VarName ); }
#line 493 "C:\\Program Files\\Microsoft Visual Studio 8\\VC\\INCLUDE\\stdlib.h"
extern "C" { extern __declspec( dllimport ) errno_t __cdecl _dupenv_s(char **, size_t *, const char *); } 
#line 499 "C:\\Program Files\\Microsoft Visual Studio 8\\VC\\INCLUDE\\stdlib.h"
extern "C" { extern __declspec( dllimport ) errno_t __cdecl _itoa_s(int, char *, size_t, int); } 
template < size_t _Size > inline errno_t __cdecl _itoa_s ( int _Value, char ( & _Dest ) [ _Size ], int _Radix ) { return _itoa_s ( _Value, _Dest, _Size, _Radix ); }
extern "C" { extern __declspec( dllimport ) char *__cdecl _itoa(int, char *, int); } 

extern "C" { extern __declspec( dllimport ) errno_t __cdecl _i64toa_s(__int64, char *, size_t, int); } 
extern "C" { extern __declspec( dllimport ) char *__cdecl _i64toa(__int64, char *, int); } 
extern "C" { extern __declspec( dllimport ) errno_t __cdecl _ui64toa_s(unsigned __int64, char *, size_t, int); } 
extern "C" { extern __declspec( dllimport ) char *__cdecl _ui64toa(unsigned __int64, char *, int); } 
extern "C" { extern __declspec( dllimport ) __int64 __cdecl _atoi64(const char *); } 
extern "C" { extern __declspec( dllimport ) __int64 __cdecl _atoi64_l(const char *, _locale_t); } 
extern "C" { extern __declspec( dllimport ) __int64 __cdecl _strtoi64(const char *, char **, int); } 
extern "C" { extern __declspec( dllimport ) __int64 __cdecl _strtoi64_l(const char *, char **, int, _locale_t); } 
extern "C" { extern __declspec( dllimport ) unsigned __int64 __cdecl _strtoui64(const char *, char **, int); } 
extern "C" { extern __declspec( dllimport ) unsigned __int64 __cdecl _strtoui64_l(const char *, char **, int, _locale_t); } 
#line 514 "C:\\Program Files\\Microsoft Visual Studio 8\\VC\\INCLUDE\\stdlib.h"
extern "C" { extern __declspec( dllimport ) ldiv_t __cdecl ldiv(long, long); } 



inline ldiv_t div(long _A1, long _A2) 
{ 
return ldiv(_A1, _A2); 
} 
#line 524 "C:\\Program Files\\Microsoft Visual Studio 8\\VC\\INCLUDE\\stdlib.h"
extern "C" { extern __declspec( dllimport ) errno_t __cdecl _ltoa_s(long, char *, size_t, int); } 
template < size_t _Size > inline errno_t __cdecl _ltoa_s ( long _Value, char ( & _Dest ) [ _Size ], int _Radix ) { return _ltoa_s ( _Value, _Dest, _Size, _Radix ); }
extern "C" { extern __declspec( dllimport ) char *__cdecl _ltoa(long, char *, int); } 
extern "C" { extern __declspec( dllimport ) int __cdecl mblen(const char *, size_t); } 
extern "C" { extern __declspec( dllimport ) int __cdecl _mblen_l(const char *, size_t, _locale_t); } 
extern "C" { extern __declspec( dllimport ) size_t __cdecl _mbstrlen(const char *); } 
extern "C" { extern __declspec( dllimport ) size_t __cdecl _mbstrlen_l(const char *, _locale_t); } 
extern "C" { extern __declspec( dllimport ) size_t __cdecl _mbstrnlen(const char *, size_t); } 
extern "C" { extern __declspec( dllimport ) size_t __cdecl _mbstrnlen_l(const char *, size_t, _locale_t); } 
extern "C" { extern __declspec( dllimport ) int __cdecl mbtowc(__wchar_t *, const char *, size_t); } 
extern "C" { extern __declspec( dllimport ) int __cdecl _mbtowc_l(__wchar_t *, const char *, size_t, _locale_t); } 
extern "C" { extern __declspec( dllimport ) errno_t __cdecl mbstowcs_s(size_t *, __wchar_t *, size_t, const char *, size_t); } 
template < size_t _Size > inline errno_t __cdecl mbstowcs_s ( size_t * _PtNumOfCharConverted, wchar_t ( & _Dest ) [ _Size ], const char * _Source, size_t _MaxCount ) { return mbstowcs_s ( _PtNumOfCharConverted, _Dest, _Size, _Source, _MaxCount ); }
extern "C" { extern __declspec( dllimport ) size_t __cdecl mbstowcs(__wchar_t *, const char *, size_t); } 

extern "C" { extern __declspec( dllimport ) errno_t __cdecl _mbstowcs_s_l(size_t *, __wchar_t *, size_t, const char *, size_t, _locale_t); } 
template < size_t _Size > inline errno_t __cdecl _mbstowcs_s_l ( size_t * _PtNumOfCharConverted, wchar_t ( & _Dest ) [ _Size ], const char * _Source, size_t _MaxCount, _locale_t _Locale ) { return _mbstowcs_s_l ( _PtNumOfCharConverted, _Dest, _Size, _Source, _MaxCount, _Locale ); }
extern "C" { extern __declspec( dllimport ) size_t __cdecl _mbstowcs_l(__wchar_t *, const char *, size_t, _locale_t); } 

extern "C" { extern __declspec( dllimport ) int __cdecl rand(); } 
#line 548 "C:\\Program Files\\Microsoft Visual Studio 8\\VC\\INCLUDE\\stdlib.h"
extern "C" { extern __declspec( dllimport ) int __cdecl _set_error_mode(int); } 

extern "C" { extern __declspec( dllimport ) void __cdecl srand(unsigned); } 
extern "C" { extern __declspec( dllimport ) double __cdecl strtod(const char *, char **); } 
extern "C" { extern __declspec( dllimport ) double __cdecl _strtod_l(const char *, char **, _locale_t); } 
extern "C" { extern __declspec( dllimport ) long __cdecl strtol(const char *, char **, int); } 
extern "C" { extern __declspec( dllimport ) long __cdecl _strtol_l(const char *, char **, int, _locale_t); } 
extern "C" { extern __declspec( dllimport ) unsigned long __cdecl strtoul(const char *, char **, int); } 
extern "C" { extern __declspec( dllimport ) unsigned long __cdecl _strtoul_l(const char *, char **, int, _locale_t); } 


extern "C" { extern __declspec( dllimport ) int __cdecl system(const char *); } 
#line 561 "C:\\Program Files\\Microsoft Visual Studio 8\\VC\\INCLUDE\\stdlib.h"
extern "C" { extern __declspec( dllimport ) errno_t __cdecl _ultoa_s(unsigned long, char *, size_t, int); } 
template < size_t _Size > inline errno_t __cdecl _ultoa_s ( unsigned long _Value, char ( & _Dest ) [ _Size ], int _Radix ) { return _ultoa_s ( _Value, _Dest, _Size, _Radix ); }
extern "C" { extern __declspec( dllimport ) char *__cdecl _ultoa(unsigned long, char *, int); } 
extern "C" { extern __declspec( dllimport ) int __cdecl wctomb(char *, __wchar_t); } 
extern "C" { extern __declspec( dllimport ) int __cdecl _wctomb_l(char *, __wchar_t, _locale_t); } 

extern "C" { extern __declspec( dllimport ) errno_t __cdecl wctomb_s(int *, char *, rsize_t, __wchar_t); } 
#line 569 "C:\\Program Files\\Microsoft Visual Studio 8\\VC\\INCLUDE\\stdlib.h"
extern "C" { extern __declspec( dllimport ) errno_t __cdecl _wctomb_s_l(int *, char *, size_t, __wchar_t, _locale_t); } 
extern "C" { extern __declspec( dllimport ) errno_t __cdecl wcstombs_s(size_t *, char *, size_t, const __wchar_t *, size_t); } 
template < size_t _Size > inline errno_t __cdecl wcstombs_s ( size_t * _PtNumOfCharConverted, char ( & _Dest ) [ _Size ], const wchar_t * _Source, size_t _MaxCount ) { return wcstombs_s ( _PtNumOfCharConverted, _Dest, _Size, _Source, _MaxCount ); }
extern "C" { extern __declspec( dllimport ) size_t __cdecl wcstombs(char *, const __wchar_t *, size_t); } 
extern "C" { extern __declspec( dllimport ) errno_t __cdecl _wcstombs_s_l(size_t *, char *, size_t, const __wchar_t *, size_t, _locale_t); } 
template < size_t _Size > inline errno_t __cdecl _wcstombs_s_l ( size_t * _PtNumOfCharConverted, char ( & _Dest ) [ _Size ], const wchar_t * _Source, size_t _MaxCount, _locale_t _Locale ) { return _wcstombs_s_l ( _PtNumOfCharConverted, _Dest, _Size, _Source, _MaxCount, _Locale ); }
extern "C" { extern __declspec( dllimport ) size_t __cdecl _wcstombs_l(char *, const __wchar_t *, size_t, _locale_t); } 
#line 634 "C:\\Program Files\\Microsoft Visual Studio 8\\VC\\INCLUDE\\stdlib.h"
extern "C" { extern __declspec( dllimport ) void *__cdecl calloc(size_t, size_t); } 
extern "C" { extern __declspec( dllimport ) void __cdecl free(void *); } 
extern "C" { extern __declspec( dllimport ) void *__cdecl malloc(size_t); } 
extern "C" { extern __declspec( dllimport ) void *__cdecl realloc(void *, size_t); } 
extern "C" { extern __declspec( dllimport ) void *__cdecl _recalloc(void *, size_t, size_t); } 
extern "C" { extern __declspec( dllimport ) void __cdecl _aligned_free(void *); } 
extern "C" { extern __declspec( dllimport ) void *__cdecl _aligned_malloc(size_t, size_t); } 
extern "C" { extern __declspec( dllimport ) void *__cdecl _aligned_offset_malloc(size_t, size_t, size_t); } 
extern "C" { extern __declspec( dllimport ) void *__cdecl _aligned_realloc(void *, size_t, size_t); } 
extern "C" { extern __declspec( dllimport ) void *__cdecl _aligned_recalloc(void *, size_t, size_t, size_t); } 
extern "C" { extern __declspec( dllimport ) void *__cdecl _aligned_offset_realloc(void *, size_t, size_t, size_t); } 
extern "C" { extern __declspec( dllimport ) void *__cdecl _aligned_offset_recalloc(void *, size_t, size_t, size_t, size_t); } 
extern "C" { extern __declspec( dllimport ) size_t __cdecl _aligned_msize(void *, size_t, size_t); } 
#line 673 "C:\\Program Files\\Microsoft Visual Studio 8\\VC\\INCLUDE\\stdlib.h"
extern "C" { extern __declspec( dllimport ) errno_t __cdecl _itow_s(int, __wchar_t *, size_t, int); } 
template < size_t _Size > inline errno_t __cdecl _itow_s ( int _Value, wchar_t ( & _Dest ) [ _Size ], int _Radix ) { return _itow_s ( _Value, _Dest, _Size, _Radix ); }
extern "C" { extern __declspec( dllimport ) __wchar_t *__cdecl _itow(int, __wchar_t *, int); } 
extern "C" { extern __declspec( dllimport ) errno_t __cdecl _ltow_s(long, __wchar_t *, size_t, int); } 
template < size_t _Size > inline errno_t __cdecl _ltow_s ( long _Value, wchar_t ( & _Dest ) [ _Size ], int _Radix ) { return _ltow_s ( _Value, _Dest, _Size, _Radix ); }
extern "C" { extern __declspec( dllimport ) __wchar_t *__cdecl _ltow(long, __wchar_t *, int); } 
extern "C" { extern __declspec( dllimport ) errno_t __cdecl _ultow_s(unsigned long, __wchar_t *, size_t, int); } 
template < size_t _Size > inline errno_t __cdecl _ultow_s ( unsigned long _Value, wchar_t ( & _Dest ) [ _Size ], int _Radix ) { return _ultow_s ( _Value, _Dest, _Size, _Radix ); }
extern "C" { extern __declspec( dllimport ) __wchar_t *__cdecl _ultow(unsigned long, __wchar_t *, int); } 
extern "C" { extern __declspec( dllimport ) double __cdecl wcstod(const __wchar_t *, __wchar_t **); } 
extern "C" { extern __declspec( dllimport ) double __cdecl _wcstod_l(const __wchar_t *, __wchar_t **, _locale_t); } 
extern "C" { extern __declspec( dllimport ) long __cdecl wcstol(const __wchar_t *, __wchar_t **, int); } 
extern "C" { extern __declspec( dllimport ) long __cdecl _wcstol_l(const __wchar_t *, __wchar_t **, int, _locale_t); } 
extern "C" { extern __declspec( dllimport ) unsigned long __cdecl wcstoul(const __wchar_t *, __wchar_t **, int); } 
extern "C" { extern __declspec( dllimport ) unsigned long __cdecl _wcstoul_l(const __wchar_t *, __wchar_t **, int, _locale_t); } 
extern "C" { extern __declspec( dllimport ) __wchar_t *__cdecl _wgetenv(const __wchar_t *); } 
extern "C" { extern __declspec( dllimport ) errno_t __cdecl _wgetenv_s(size_t *, __wchar_t *, size_t, const __wchar_t *); } 
template < size_t _Size > inline errno_t __cdecl _wgetenv_s ( size_t * _ReturnSize, wchar_t ( & _Dest ) [ _Size ], const wchar_t * _VarName ) { return _wgetenv_s ( _ReturnSize, _Dest, _Size, _VarName ); }
#line 697 "C:\\Program Files\\Microsoft Visual Studio 8\\VC\\INCLUDE\\stdlib.h"
extern "C" { extern __declspec( dllimport ) errno_t __cdecl _wdupenv_s(__wchar_t **, size_t *, const __wchar_t *); } 
#line 705 "C:\\Program Files\\Microsoft Visual Studio 8\\VC\\INCLUDE\\stdlib.h"
extern "C" { extern __declspec( dllimport ) int __cdecl _wsystem(const __wchar_t *); } 
#line 707 "C:\\Program Files\\Microsoft Visual Studio 8\\VC\\INCLUDE\\stdlib.h"
extern "C" { extern __declspec( dllimport ) double __cdecl _wtof(const __wchar_t *); } 
extern "C" { extern __declspec( dllimport ) double __cdecl _wtof_l(const __wchar_t *, _locale_t); } 
extern "C" { extern __declspec( dllimport ) int __cdecl _wtoi(const __wchar_t *); } 
extern "C" { extern __declspec( dllimport ) int __cdecl _wtoi_l(const __wchar_t *, _locale_t); } 
extern "C" { extern __declspec( dllimport ) long __cdecl _wtol(const __wchar_t *); } 
extern "C" { extern __declspec( dllimport ) long __cdecl _wtol_l(const __wchar_t *, _locale_t); } 


extern "C" { extern __declspec( dllimport ) errno_t __cdecl _i64tow_s(__int64, __wchar_t *, size_t, int); } 
extern "C" { extern __declspec( dllimport ) __wchar_t *__cdecl _i64tow(__int64, __wchar_t *, int); } 
extern "C" { extern __declspec( dllimport ) errno_t __cdecl _ui64tow_s(unsigned __int64, __wchar_t *, size_t, int); } 
extern "C" { extern __declspec( dllimport ) __wchar_t *__cdecl _ui64tow(unsigned __int64, __wchar_t *, int); } 
extern "C" { extern __declspec( dllimport ) __int64 __cdecl _wtoi64(const __wchar_t *); } 
extern "C" { extern __declspec( dllimport ) __int64 __cdecl _wtoi64_l(const __wchar_t *, _locale_t); } 
extern "C" { extern __declspec( dllimport ) __int64 __cdecl _wcstoi64(const __wchar_t *, __wchar_t **, int); } 
extern "C" { extern __declspec( dllimport ) __int64 __cdecl _wcstoi64_l(const __wchar_t *, __wchar_t **, int, _locale_t); } 
extern "C" { extern __declspec( dllimport ) unsigned __int64 __cdecl _wcstoui64(const __wchar_t *, __wchar_t **, int); } 
extern "C" { extern __declspec( dllimport ) unsigned __int64 __cdecl _wcstoui64_l(const __wchar_t *, __wchar_t **, int, _locale_t); } 
#line 745 "C:\\Program Files\\Microsoft Visual Studio 8\\VC\\INCLUDE\\stdlib.h"
extern "C" { extern __declspec( dllimport ) char *__cdecl _fullpath(char *, const char *, size_t); } 
#line 753 "C:\\Program Files\\Microsoft Visual Studio 8\\VC\\INCLUDE\\stdlib.h"
extern "C" { extern __declspec( dllimport ) errno_t __cdecl _ecvt_s(char *, size_t, double, int, int *, int *); } 
template < size_t _Size > inline errno_t __cdecl _ecvt_s ( char ( & _Dest ) [ _Size ], double _Value, int _NumOfDigits, int * _PtDec, int * _PtSign ) { return _ecvt_s ( _Dest, _Size, _Value, _NumOfDigits, _PtDec, _PtSign ); }
extern "C" { extern __declspec( dllimport ) char *__cdecl _ecvt(double, int, int *, int *); } 
extern "C" { extern __declspec( dllimport ) errno_t __cdecl _fcvt_s(char *, size_t, double, int, int *, int *); } 
template < size_t _Size > inline errno_t __cdecl _fcvt_s ( char ( & _Dest ) [ _Size ], double _Value, int _NumOfDigits, int * _PtDec, int * _PtSign ) { return _fcvt_s ( _Dest, _Size, _Value, _NumOfDigits, _PtDec, _PtSign ); }
extern "C" { extern __declspec( dllimport ) char *__cdecl _fcvt(double, int, int *, int *); } 
extern "C" { extern __declspec( dllimport ) errno_t __cdecl _gcvt_s(char *, size_t, double, int); } 
template < size_t _Size > inline errno_t __cdecl _gcvt_s ( char ( & _Dest ) [ _Size ], double _Value, int _NumOfDigits ) { return _gcvt_s ( _Dest, _Size, _Value, _NumOfDigits ); }
extern "C" { extern __declspec( dllimport ) char *__cdecl _gcvt(double, int, char *); } 

extern "C" { extern __declspec( dllimport ) int __cdecl _atodbl(_CRT_DOUBLE *, char *); } 
extern "C" { extern __declspec( dllimport ) int __cdecl _atoldbl(_LDOUBLE *, char *); } 
extern "C" { extern __declspec( dllimport ) int __cdecl _atoflt(_CRT_FLOAT *, char *); } 
extern "C" { extern __declspec( dllimport ) int __cdecl _atodbl_l(_CRT_DOUBLE *, char *, _locale_t); } 
extern "C" { extern __declspec( dllimport ) int __cdecl _atoldbl_l(_LDOUBLE *, char *, _locale_t); } 
extern "C" { extern __declspec( dllimport ) int __cdecl _atoflt_l(_CRT_FLOAT *, char *, _locale_t); } 
extern "C" { extern unsigned long __cdecl _lrotl(unsigned long, int); } 
extern "C" { extern unsigned long __cdecl _lrotr(unsigned long, int); } 
extern "C" { extern __declspec( dllimport ) errno_t __cdecl _makepath_s(char *, size_t, const char *, const char *, const char *, const char *); } 

template < size_t _Size > inline errno_t __cdecl _makepath_s ( char ( & _Path ) [ _Size ], const char * _Drive, const char * _Dir, const char * _Filename, const char * _Ext ) { return _makepath_s ( _Path, _Size, _Drive, _Dir, _Filename, _Ext ); }
extern "C" { extern __declspec( dllimport ) void __cdecl _makepath(char *, const char *, const char *, const char *, const char *); } 
#line 801 "C:\\Program Files\\Microsoft Visual Studio 8\\VC\\INCLUDE\\stdlib.h"
extern "C" { extern _onexit_t __cdecl _onexit(_onexit_t); } 
#line 806 "C:\\Program Files\\Microsoft Visual Studio 8\\VC\\INCLUDE\\stdlib.h"
extern "C" { extern __declspec( dllimport ) void __cdecl perror(const char *); } 
#line 808 "C:\\Program Files\\Microsoft Visual Studio 8\\VC\\INCLUDE\\stdlib.h"
extern "C" { extern __declspec( dllimport ) int __cdecl _putenv(const char *); } 
extern "C" { extern __declspec( dllimport ) errno_t __cdecl _putenv_s(const char *, const char *); } 
extern "C" { extern unsigned __cdecl _rotl(unsigned, int); } 

extern "C" { extern unsigned __int64 __cdecl _rotl64(unsigned __int64, int); } 
#line 814 "C:\\Program Files\\Microsoft Visual Studio 8\\VC\\INCLUDE\\stdlib.h"
extern "C" { extern unsigned __cdecl _rotr(unsigned, int); } 

extern "C" { extern unsigned __int64 __cdecl _rotr64(unsigned __int64, int); } 
#line 818 "C:\\Program Files\\Microsoft Visual Studio 8\\VC\\INCLUDE\\stdlib.h"
extern "C" { extern __declspec( dllimport ) errno_t __cdecl _searchenv_s(const char *, const char *, char *, size_t); } 
template < size_t _Size > inline errno_t __cdecl _searchenv_s ( const char * _Filename, const char * _EnvVar, char ( & _ResultPath ) [ _Size ] ) { return _searchenv_s ( _Filename, _EnvVar, _ResultPath, _Size ); }
extern "C" { extern __declspec( dllimport ) void __cdecl _searchenv(const char *, const char *, char *); } 

extern "C" { extern __declspec( dllimport ) void __cdecl _splitpath(const char *, char *, char *, char *, char *); } 
extern "C" { extern __declspec( dllimport ) errno_t __cdecl _splitpath_s(const char *, char *, size_t, char *, size_t, char *, size_t, char *, size_t); } 




template < size_t _DriveSize, size_t _DirSize, size_t _NameSize, size_t _ExtSize > inline errno_t __cdecl _splitpath_s ( const char * _Dest, char ( & _Drive ) [ _DriveSize ], char ( & _Dir ) [ _DirSize ], char ( & _Name ) [ _NameSize ], char ( & _Ext ) [ _ExtSize ] ) { return _splitpath_s ( _Dest, _Drive, _DriveSize, _Dir, _DirSize, _Name, _NameSize, _Ext, _ExtSize ); }

extern "C" { extern __declspec( dllimport ) void __cdecl _swab(char *, char *, int); } 
#line 841 "C:\\Program Files\\Microsoft Visual Studio 8\\VC\\INCLUDE\\stdlib.h"
extern "C" { extern __declspec( dllimport ) __wchar_t *__cdecl _wfullpath(__wchar_t *, const __wchar_t *, size_t); } 
#line 847 "C:\\Program Files\\Microsoft Visual Studio 8\\VC\\INCLUDE\\stdlib.h"
extern "C" { extern __declspec( dllimport ) errno_t __cdecl _wmakepath_s(__wchar_t *, size_t, const __wchar_t *, const __wchar_t *, const __wchar_t *, const __wchar_t *); } 

template < size_t _Size > inline errno_t __cdecl _wmakepath_s ( wchar_t ( & _ResultPath ) [ _Size ], const wchar_t * _Drive, const wchar_t * _Dir, const wchar_t * _Filename, const wchar_t * _Ext ) { return _wmakepath_s ( _ResultPath, _Size, _Drive, _Dir, _Filename, _Ext ); }
extern "C" { extern __declspec( dllimport ) void __cdecl _wmakepath(__wchar_t *, const __wchar_t *, const __wchar_t *, const __wchar_t *, const __wchar_t *); } 


extern "C" { extern __declspec( dllimport ) void __cdecl _wperror(const __wchar_t *); } 
#line 855 "C:\\Program Files\\Microsoft Visual Studio 8\\VC\\INCLUDE\\stdlib.h"
extern "C" { extern __declspec( dllimport ) int __cdecl _wputenv(const __wchar_t *); } 
extern "C" { extern __declspec( dllimport ) errno_t __cdecl _wputenv_s(const __wchar_t *, const __wchar_t *); } 
extern "C" { extern __declspec( dllimport ) errno_t __cdecl _wsearchenv_s(const __wchar_t *, const __wchar_t *, __wchar_t *, size_t); } 
template < size_t _Size > inline errno_t __cdecl _wsearchenv_s ( const wchar_t * _Filename, const wchar_t * _EnvVar, wchar_t ( & _ResultPath ) [ _Size ] ) { return _wsearchenv_s ( _Filename, _EnvVar, _ResultPath, _Size ); }
extern "C" { extern __declspec( dllimport ) void __cdecl _wsearchenv(const __wchar_t *, const __wchar_t *, __wchar_t *); } 
extern "C" { extern __declspec( dllimport ) void __cdecl _wsplitpath(const __wchar_t *, __wchar_t *, __wchar_t *, __wchar_t *, __wchar_t *); } 
extern "C" { extern __declspec( dllimport ) errno_t __cdecl _wsplitpath_s(const __wchar_t *, __wchar_t *, size_t, __wchar_t *, size_t, __wchar_t *, size_t, __wchar_t *, size_t); } 




template < size_t _DriveSize, size_t _DirSize, size_t _NameSize, size_t _ExtSize > inline errno_t __cdecl _wsplitpath_s ( const wchar_t * _Path, wchar_t ( & _Drive ) [ _DriveSize ], wchar_t ( & _Dir ) [ _DirSize ], wchar_t ( & _Name ) [ _NameSize ], wchar_t ( & _Ext ) [ _ExtSize ] ) { return _wsplitpath_s ( _Path, _Drive, _DriveSize, _Dir, _DirSize, _Name, _NameSize, _Ext, _ExtSize ); }
#line 872 "C:\\Program Files\\Microsoft Visual Studio 8\\VC\\INCLUDE\\stdlib.h"
extern "C" { extern __declspec( dllimport ) __declspec(deprecated("This function or variable has been superceded by newer library or operating system functionality. Consider using SetErrorMode instead. See online help for details.")) void __cdecl _seterrormode(int); } 
extern "C" { extern __declspec( dllimport ) __declspec(deprecated("This function or variable has been superceded by newer library or operating system functionality. Consider using Beep instead. See online help for details.")) void __cdecl _beep(unsigned, unsigned); } 
extern "C" { extern __declspec( dllimport ) __declspec(deprecated("This function or variable has been superceded by newer library or operating system functionality. Consider using Sleep instead. See online help for details.")) void __cdecl _sleep(unsigned long); } 
#line 893 "C:\\Program Files\\Microsoft Visual Studio 8\\VC\\INCLUDE\\stdlib.h"
#pragma warning(push)
#pragma warning(disable: 4141)
extern "C" { extern __declspec( dllimport ) char *__cdecl ecvt(double, int, int *, int *); } 
extern "C" { extern __declspec( dllimport ) char *__cdecl fcvt(double, int, int *, int *); } 
extern "C" { extern __declspec( dllimport ) char *__cdecl gcvt(double, int, char *); } 
extern "C" { extern __declspec( dllimport ) char *__cdecl itoa(int, char *, int); } 
extern "C" { extern __declspec( dllimport ) char *__cdecl ltoa(long, char *, int); } 
extern "C" { extern __declspec( dllimport ) int __cdecl putenv(const char *); } 
extern "C" { extern __declspec( dllimport ) void __cdecl swab(char *, char *, int); } 
extern "C" { extern __declspec( dllimport ) char *__cdecl ultoa(unsigned long, char *, int); } 
#pragma warning(pop)
extern "C" { extern _onexit_t __cdecl onexit(_onexit_t); } 
#line 27 "C:\\Program Files\\Microsoft Visual Studio 8\\VC\\INCLUDE\\use_ansi.h"
#pragma comment(lib,"msvcprtd")
#line 57 "C:\\Program Files\\Microsoft Visual Studio 8\\VC\\INCLUDE\\use_ansi.h"
#pragma comment(linker,"/manifestdependency:\"type='win32' " "name='" "Microsoft.VC80" ".DebugCRT' " "version='" "8.0.50727.4053" "' " "processorArchitecture='x86' " "publicKeyToken='" "1fc8b3b9a1e18e3b" "'\"")
#line 916 "C:\\Program Files\\Microsoft Visual Studio 8\\VC\\INCLUDE\\stdlib.h"
#pragma pack ( pop )
#line 9 "C:\\Program Files\\Microsoft Visual Studio 8\\VC\\INCLUDE\\yvals.h"
#pragma pack ( push, 8 )
#line 513 "C:\\Program Files\\Microsoft Visual Studio 8\\VC\\INCLUDE\\yvals.h"
namespace std { 
typedef bool _Bool; 
}
#line 531 "C:\\Program Files\\Microsoft Visual Studio 8\\VC\\INCLUDE\\yvals.h"
typedef __int64 _Longlong; 
typedef unsigned __int64 _ULonglong; 
#line 554 "C:\\Program Files\\Microsoft Visual Studio 8\\VC\\INCLUDE\\yvals.h"
namespace std { 



#pragma warning(push)
#pragma warning(disable:4412)
class __declspec( dllimport ) _Lockit { 
#line 580 "C:\\Program Files\\Microsoft Visual Studio 8\\VC\\INCLUDE\\yvals.h"
public: explicit __thiscall _Lockit(); 
explicit __thiscall _Lockit(int); 
__thiscall ~_Lockit(); 
#line 585 "C:\\Program Files\\Microsoft Visual Studio 8\\VC\\INCLUDE\\yvals.h"
static void __cdecl _Lockit_ctor(int); 
static void __cdecl _Lockit_dtor(int); 


private: static void __cdecl _Lockit_ctor(_Lockit *); 
static void __cdecl _Lockit_ctor(_Lockit *, int); 
static void __cdecl _Lockit_dtor(_Lockit *); 

_Lockit(const _Lockit &); 
_Lockit &operator=(const _Lockit &); 

int _Locktype; 
#line 613 "C:\\Program Files\\Microsoft Visual Studio 8\\VC\\INCLUDE\\yvals.h"
}; 
#line 707 "C:\\Program Files\\Microsoft Visual Studio 8\\VC\\INCLUDE\\yvals.h"
class __declspec( dllimport ) _Mutex { 
#line 731 "C:\\Program Files\\Microsoft Visual Studio 8\\VC\\INCLUDE\\yvals.h"
public: __thiscall _Mutex(); 
__thiscall ~_Mutex(); 
void __thiscall _Lock(); 
void __thiscall _Unlock(); 
#line 738 "C:\\Program Files\\Microsoft Visual Studio 8\\VC\\INCLUDE\\yvals.h"
private: static void __cdecl _Mutex_ctor(_Mutex *); 
static void __cdecl _Mutex_dtor(_Mutex *); 
static void __cdecl _Mutex_Lock(_Mutex *); 
static void __cdecl _Mutex_Unlock(_Mutex *); 

_Mutex(const _Mutex &); 
_Mutex &operator=(const _Mutex &); 
void *_Mtx; 
#line 757 "C:\\Program Files\\Microsoft Visual Studio 8\\VC\\INCLUDE\\yvals.h"
}; 

class __declspec( dllimport ) _Init_locks { 
#line 775 "C:\\Program Files\\Microsoft Visual Studio 8\\VC\\INCLUDE\\yvals.h"
public: __thiscall _Init_locks(); 
__thiscall ~_Init_locks(); 
#line 780 "C:\\Program Files\\Microsoft Visual Studio 8\\VC\\INCLUDE\\yvals.h"
private: static void __cdecl _Init_locks_ctor(_Init_locks *); 
static void __cdecl _Init_locks_dtor(_Init_locks *); 
#line 793 "C:\\Program Files\\Microsoft Visual Studio 8\\VC\\INCLUDE\\yvals.h"
}; 
#pragma warning(pop)
}
#line 804 "C:\\Program Files\\Microsoft Visual Studio 8\\VC\\INCLUDE\\yvals.h"
extern __declspec( dllimport ) void __cdecl _Atexit(void (__cdecl *)(void)); 

typedef int _Mbstatet; 
#line 816 "C:\\Program Files\\Microsoft Visual Studio 8\\VC\\INCLUDE\\yvals.h"
#pragma pack ( pop )
#line 17 "C:\\Program Files\\Microsoft Visual Studio 8\\VC\\INCLUDE\\cmath"
namespace std { 
using ::acosf;using ::asinf;
using ::atanf;using ::atan2f;using ::ceilf;
using ::cosf;using ::coshf;using ::expf;
using ::fabsf;using ::floorf;using ::fmodf;
using ::frexpf;using ::ldexpf;using ::logf;
using ::log10f;using ::modff;using ::powf;
using ::sinf;using ::sinhf;using ::sqrtf;
using ::tanf;using ::tanhf;

using ::acosl;using ::asinl;
using ::atanl;using ::atan2l;using ::ceill;
using ::cosl;using ::coshl;using ::expl;
using ::fabsl;using ::floorl;using ::fmodl;
using ::frexpl;using ::ldexpl;using ::logl;
using ::log10l;using ::modfl;using ::powl;
using ::sinl;using ::sinhl;using ::sqrtl;
using ::tanl;using ::tanhl;

using ::abs;
using ::acos;using ::asin;
using ::atan;using ::atan2;using ::ceil;
using ::cos;using ::cosh;using ::exp;
using ::fabs;using ::floor;using ::fmod;
using ::frexp;using ::ldexp;using ::log;
using ::log10;using ::modf;using ::pow;
using ::sin;using ::sinh;using ::sqrt;
using ::tan;using ::tanh;

}
#line 17 "C:\\Program Files\\Microsoft Visual Studio 8\\VC\\INCLUDE\\cstdlib"
namespace std { 
using ::size_t;using ::div_t;using ::ldiv_t;

using ::abort;using ::atexit;
using ::atof;using ::atoi;using ::atol;
using ::bsearch;using ::calloc;using ::div;
using ::exit;using ::free;using ::getenv;
using ::labs;using ::ldiv;using ::malloc;
using ::mblen;using ::mbstowcs;using ::mbtowc;
using ::qsort;using ::rand;using ::realloc;
using ::srand;using ::strtod;using ::strtol;
using ::strtoul;using ::system;
using ::wcstombs;using ::wctomb;
}
#line 518 "c:\\cuda\\include\\math_functions.h"
static __inline __int64 abs(__int64 a) 
{ 
return llabs(a); 
} 

static __inline int signbit(double a) 
{ 
return __signbit(a); 
} 

static __inline int signbit(float a) 
{ 
return __signbitf(a); 
} 

static __inline int isinf(double a) 
{ 
return __isinf(a); 
} 

static __inline int isinf(float a) 
{ 
return __isinff(a); 
} 

static __inline int isnan(double a) 
{ 
return __isnan(a); 
} 

static __inline int isnan(float a) 
{ 
return __isnanf(a); 
} 

static __inline int isfinite(double a) 
{ 
return __finite(a); 
} 

static __inline int isfinite(float a) 
{ 
return __finitef(a); 
} 

template<class T> inline T _Pow_int(T, int); 
#line 571 "c:\\cuda\\include\\math_functions.h"
inline long abs(long); 
inline float abs(float); 
inline double abs(double); 
inline float fabs(float); 
inline float ceil(float); 
inline float floor(float); 
inline float sqrt(float); 
inline float pow(float, float); 
inline float pow(float, int); 
inline double pow(double, int); 
inline float log(float); 
inline float log10(float); 
inline float fmod(float, float); 
inline float modf(float, float *); 
inline float exp(float); 
inline float frexp(float, int *); 
inline float ldexp(float, int); 
inline float asin(float); 
inline float sin(float); 
inline float sinh(float); 
inline float acos(float); 
inline float cos(float); 
inline float cosh(float); 
inline float atan(float); 
inline float atan2(float, float); 
inline float tan(float); 
inline float tanh(float); 
#line 603 "c:\\cuda\\include\\math_functions.h"
static __inline float logb(float a) 
{ 
return logbf(a); 
} 

static __inline int ilogb(float a) 
{ 
return ilogbf(a); 
} 

static __inline float scalbn(float a, int b) 
{ 
return scalbnf(a, b); 
} 

static __inline float scalbln(float a, long b) 
{ 
return scalblnf(a, b); 
} 

static __inline float exp2(float a) 
{ 
return exp2f(a); 
} 

static __inline float exp10(float a) 
{ 
return exp10f(a); 
} 

static __inline float expm1(float a) 
{ 
return expm1f(a); 
} 

static __inline float log2(float a) 
{ 
return log2f(a); 
} 

static __inline float log1p(float a) 
{ 
return log1pf(a); 
} 

static __inline float rsqrt(float a) 
{ 
return rsqrtf(a); 
} 

static __inline float acosh(float a) 
{ 
return acoshf(a); 
} 

static __inline float asinh(float a) 
{ 
return asinhf(a); 
} 

static __inline float atanh(float a) 
{ 
return atanhf(a); 
} 

static __inline float hypot(float a, float b) 
{ 
return hypotf(a, b); 
} 

static __inline float cbrt(float a) 
{ 
return cbrtf(a); 
} 

static __inline void sincos(float a, float *sptr, float *cptr) 
{ 
sincosf(a, sptr, cptr); 
} 

static __inline float erf(float a) 
{ 
return erff(a); 
} 

static __inline float erfinv(float a) 
{ 
return erfinvf(a); 
} 

static __inline float erfc(float a) 
{ 
return erfcf(a); 
} 

static __inline float erfcinv(float a) 
{ 
return erfcinvf(a); 
} 

static __inline float lgamma(float a) 
{ 
return lgammaf(a); 
} 

static __inline float tgamma(float a) 
{ 
return tgammaf(a); 
} 

static __inline float copysign(float a, float b) 
{ 
return copysignf(a, b); 
} 

static __inline double copysign(double a, float b) 
{ 
return copysign(a, (double)b); 
} 

static __inline float copysign(float a, double b) 
{ 
return copysignf(a, (float)b); 
} 

static __inline float nextafter(float a, float b) 
{ 
return nextafterf(a, b); 
} 

static __inline float remainder(float a, float b) 
{ 
return remainderf(a, b); 
} 

static __inline float remquo(float a, float b, int *quo) 
{ 
return remquof(a, b, quo); 
} 

static __inline float round(float a) 
{ 
return roundf(a); 
} 

static __inline long lround(float a) 
{ 
return lroundf(a); 
} 

static __inline __int64 llround(float a) 
{ 
return llroundf(a); 
} 

static __inline float trunc(float a) 
{ 
return truncf(a); 
} 

static __inline float rint(float a) 
{ 
return rintf(a); 
} 

static __inline long lrint(float a) 
{ 
return lrintf(a); 
} 

static __inline __int64 llrint(float a) 
{ 
return llrintf(a); 
} 

static __inline float nearbyint(float a) 
{ 
return nearbyintf(a); 
} 

static __inline float fdim(float a, float b) 
{ 
return fdimf(a, b); 
} 

static __inline float fma(float a, float b, float c) 
{ 
return fmaf(a, b, c); 
} 

static __inline unsigned min(unsigned a, unsigned b) 
{ 
return umin(a, b); 
} 

static __inline unsigned min(int a, unsigned b) 
{ 
return umin((unsigned)a, b); 
} 

static __inline unsigned min(unsigned a, int b) 
{ 
return umin(a, (unsigned)b); 
} 

static __inline __int64 min(__int64 a, __int64 b) 
{ 
return llmin(a, b); 
} 

static __inline unsigned __int64 min(unsigned __int64 a, unsigned __int64 b) 
{ 
return ullmin(a, b); 
} 

static __inline unsigned __int64 min(__int64 a, unsigned __int64 b) 
{ 
return ullmin((unsigned __int64)a, b); 
} 

static __inline unsigned __int64 min(unsigned __int64 a, __int64 b) 
{ 
return ullmin(a, (unsigned __int64)b); 
} 

static __inline float min(float a, float b) 
{ 
return fminf(a, b); 
} 

static __inline double min(double a, double b) 
{ 
return fmin(a, b); 
} 

static __inline double min(float a, double b) 
{ 
return fmin((double)a, b); 
} 

static __inline double min(double a, float b) 
{ 
return fmin(a, (double)b); 
} 

static __inline unsigned max(unsigned a, unsigned b) 
{ 
return umax(a, b); 
} 

static __inline unsigned max(int a, unsigned b) 
{ 
return umax((unsigned)a, b); 
} 

static __inline unsigned max(unsigned a, int b) 
{ 
return umax(a, (unsigned)b); 
} 

static __inline __int64 max(__int64 a, __int64 b) 
{ 
return llmax(a, b); 
} 

static __inline unsigned __int64 max(unsigned __int64 a, unsigned __int64 b) 
{ 
return ullmax(a, b); 
} 

static __inline unsigned __int64 max(__int64 a, unsigned __int64 b) 
{ 
return ullmax((unsigned __int64)a, b); 
} 

static __inline unsigned __int64 max(unsigned __int64 a, __int64 b) 
{ 
return ullmax(a, (unsigned __int64)b); 
} 

static __inline float max(float a, float b) 
{ 
return fmaxf(a, b); 
} 

static __inline double max(double a, double b) 
{ 
return fmax(a, b); 
} 

static __inline double max(float a, double b) 
{ 
return fmax((double)a, b); 
} 

static __inline double max(double a, float b) 
{ 
return fmax(a, (double)b); 
} 
#line 59 "c:\\cuda\\include\\cuda_texture_types.h"
template<class T, int dim = 1, cudaTextureReadMode mode = cudaReadModeElementType> 
struct texture : public textureReference { 

texture(int norm = 0, cudaTextureFilterMode 
fMode = cudaFilterModePoint, cudaTextureAddressMode 
aMode = cudaAddressModeClamp) 
{ 
(this->normalized) = norm; 
(this->filterMode) = fMode; 
((this->addressMode)[0]) = aMode; 
((this->addressMode)[1]) = aMode; 
((this->addressMode)[2]) = aMode; 
(this->channelDesc) = cudaCreateChannelDesc< T> (); 
} 

texture(int norm, cudaTextureFilterMode 
fMode, cudaTextureAddressMode 
aMode, cudaChannelFormatDesc 
desc) 
{ 
(this->normalized) = norm; 
(this->filterMode) = fMode; 
((this->addressMode)[0]) = aMode; 
((this->addressMode)[1]) = aMode; 
((this->addressMode)[2]) = aMode; 
(this->channelDesc) = desc; 
} 
}; 

#line 89 "c:\\cuda\\include\\cuda_texture_types.h"

#line 91 "c:\\cuda\\include\\cuda_texture_types.h"

#line 93 "c:\\cuda\\include\\cuda_texture_types.h"

#line 95 "c:\\cuda\\include\\cuda_texture_types.h"

#line 97 "c:\\cuda\\include\\cuda_texture_types.h"

#line 99 "c:\\cuda\\include\\cuda_texture_types.h"
#line 53 "c:\\cuda\\include\\device_launch_parameters.h"




#line 58 "c:\\cuda\\include\\device_launch_parameters.h"
#line 55 "c:\\cuda\\include\\device_launch_parameters.h"




#line 60 "c:\\cuda\\include\\device_launch_parameters.h"
#line 57 "c:\\cuda\\include\\device_launch_parameters.h"




#line 62 "c:\\cuda\\include\\device_launch_parameters.h"
#line 59 "c:\\cuda\\include\\device_launch_parameters.h"




#line 64 "c:\\cuda\\include\\device_launch_parameters.h"
#line 61 "c:\\cuda\\include\\device_launch_parameters.h"




#line 66 "c:\\cuda\\include\\device_launch_parameters.h"
#line 101 "C:\\CUDA\\include\\cuda_runtime.h"
template<class T> __inline cudaError_t 
cudaSetupArgument(T 
arg, size_t 
offset) 

{ 
return cudaSetupArgument((const void *)(&arg), sizeof(T), offset); 
} 

template<class T> __inline cudaError_t 
cudaHostAlloc(T **
ptr, size_t 
size, unsigned 
flags) 

{ 
return cudaHostAlloc((void **)((void *)ptr), size, flags); 
} 

template<class T> __inline cudaError_t 
cudaHostGetDevicePointer(T **
pDevice, void *
pHost, unsigned 
flags) 

{ 
return cudaHostGetDevicePointer((void **)((void *)pDevice), pHost, flags); 
} 

template<class T> __inline cudaError_t 
cudaMalloc(T **
devPtr, size_t 
size) 

{ 
return cudaMalloc((void **)((void *)devPtr), size); 
} 

template<class T> __inline cudaError_t 
cudaMallocHost(T **
ptr, size_t 
size) 

{ 
return cudaMallocHost((void **)((void *)ptr), size); 
} 

template<class T> __inline cudaError_t 
cudaMallocPitch(T **
devPtr, size_t *
pitch, size_t 
width, size_t 
height) 

{ 
return cudaMallocPitch((void **)((void *)devPtr), pitch, width, height); 
} 
#line 172 "C:\\CUDA\\include\\cuda_runtime.h"
static __inline cudaError_t cudaMemcpyToSymbol(char *
symbol, const void *
src, size_t 
count, size_t 
offset = (0), cudaMemcpyKind 
kind = cudaMemcpyHostToDevice) 

{ 
return cudaMemcpyToSymbol((const char *)symbol, src, count, offset, kind); 
} 

template<class T> __inline cudaError_t 
cudaMemcpyToSymbol(const T &
symbol, const void *
src, size_t 
count, size_t 
offset = (0), cudaMemcpyKind 
kind = cudaMemcpyHostToDevice) 

{ 
return cudaMemcpyToSymbol((const char *)(&symbol), src, count, offset, kind); 
} 

static __inline cudaError_t cudaMemcpyToSymbolAsync(char *
symbol, const void *
src, size_t 
count, size_t 
offset, cudaMemcpyKind 
kind, cudaStream_t 
stream) 

{ 
return cudaMemcpyToSymbolAsync((const char *)symbol, src, count, offset, kind, stream); 
} 

template<class T> __inline cudaError_t 
cudaMemcpyToSymbolAsync(const T &
symbol, const void *
src, size_t 
count, size_t 
offset, cudaMemcpyKind 
kind, cudaStream_t 
stream) 

{ 
return cudaMemcpyToSymbolAsync((const char *)(&symbol), src, count, offset, kind, stream); 
} 
#line 226 "C:\\CUDA\\include\\cuda_runtime.h"
static __inline cudaError_t cudaMemcpyFromSymbol(void *
dst, char *
symbol, size_t 
count, size_t 
offset = (0), cudaMemcpyKind 
kind = cudaMemcpyDeviceToHost) 

{ 
return cudaMemcpyFromSymbol(dst, (const char *)symbol, count, offset, kind); 
} 

template<class T> __inline cudaError_t 
cudaMemcpyFromSymbol(void *
dst, const T &
symbol, size_t 
count, size_t 
offset = (0), cudaMemcpyKind 
kind = cudaMemcpyDeviceToHost) 

{ 
return cudaMemcpyFromSymbol(dst, (const char *)(&symbol), count, offset, kind); 
} 

static __inline cudaError_t cudaMemcpyFromSymbolAsync(void *
dst, char *
symbol, size_t 
count, size_t 
offset, cudaMemcpyKind 
kind, cudaStream_t 
stream) 

{ 
return cudaMemcpyFromSymbolAsync(dst, (const char *)symbol, count, offset, kind, stream); 
} 

template<class T> __inline cudaError_t 
cudaMemcpyFromSymbolAsync(void *
dst, const T &
symbol, size_t 
count, size_t 
offset, cudaMemcpyKind 
kind, cudaStream_t 
stream) 

{ 
return cudaMemcpyFromSymbolAsync(dst, (const char *)(&symbol), count, offset, kind, stream); 
} 

static __inline cudaError_t cudaGetSymbolAddress(void **
devPtr, char *
symbol) 

{ 
return cudaGetSymbolAddress(devPtr, (const char *)symbol); 
} 
#line 304 "C:\\CUDA\\include\\cuda_runtime.h"
template<class T> __inline cudaError_t 
cudaGetSymbolAddress(void **
devPtr, const T &
symbol) 

{ 
return cudaGetSymbolAddress(devPtr, (const char *)(&symbol)); 
} 
#line 319 "C:\\CUDA\\include\\cuda_runtime.h"
static __inline cudaError_t cudaGetSymbolSize(size_t *
size, char *
symbol) 

{ 
return cudaGetSymbolSize(size, (const char *)symbol); 
} 
#line 348 "C:\\CUDA\\include\\cuda_runtime.h"
template<class T> __inline cudaError_t 
cudaGetSymbolSize(size_t *
size, const T &
symbol) 

{ 
return cudaGetSymbolSize(size, (const char *)(&symbol)); 
} 
#line 404 "C:\\CUDA\\include\\cuda_runtime.h"
template<class T, int dim, cudaTextureReadMode readMode> __inline cudaError_t 
cudaBindTexture(size_t *
offset, const texture< T, dim, readMode>  &
tex, const void *
devPtr, const cudaChannelFormatDesc &
desc, size_t 
size = (4294967295U)) 

{ 
return cudaBindTexture(offset, &tex, devPtr, (&desc), size); 
} 
#line 448 "C:\\CUDA\\include\\cuda_runtime.h"
template<class T, int dim, cudaTextureReadMode readMode> __inline cudaError_t 
cudaBindTexture(size_t *
offset, const texture< T, dim, readMode>  &
tex, const void *
devPtr, size_t 
size = (4294967295U)) 

{ 
return cudaBindTexture(offset, tex, devPtr, (tex.channelDesc), size); 
} 
#line 503 "C:\\CUDA\\include\\cuda_runtime.h"
template<class T, int dim, cudaTextureReadMode readMode> __inline cudaError_t 
cudaBindTexture2D(size_t *
offset, const texture< T, dim, readMode>  &
tex, const void *
devPtr, const cudaChannelFormatDesc &
desc, size_t 
width, size_t 
height, size_t 
pitch) 

{ 
return cudaBindTexture2D(offset, &tex, devPtr, (&desc), width, height, pitch); 
} 
#line 545 "C:\\CUDA\\include\\cuda_runtime.h"
template<class T, int dim, cudaTextureReadMode readMode> __inline cudaError_t 
cudaBindTextureToArray(const texture< T, dim, readMode>  &
tex, const cudaArray *
array, const cudaChannelFormatDesc &
desc) 

{ 
return cudaBindTextureToArray(&tex, array, (&desc)); 
} 
#line 582 "C:\\CUDA\\include\\cuda_runtime.h"
template<class T, int dim, cudaTextureReadMode readMode> __inline cudaError_t 
cudaBindTextureToArray(const texture< T, dim, readMode>  &
tex, const cudaArray *
array) 

{ 
auto cudaChannelFormatDesc desc; 
auto cudaError_t err = cudaGetChannelDesc(&desc, array); 

return (err == (cudaSuccess)) ? (cudaBindTextureToArray(tex, array, desc)) : err; 
} 
#line 620 "C:\\CUDA\\include\\cuda_runtime.h"
template<class T, int dim, cudaTextureReadMode readMode> __inline cudaError_t 
cudaUnbindTexture(const texture< T, dim, readMode>  &
tex) 

{ 
return cudaUnbindTexture(&tex); 
} 
#line 659 "C:\\CUDA\\include\\cuda_runtime.h"
template<class T, int dim, cudaTextureReadMode readMode> __inline cudaError_t 
cudaGetTextureAlignmentOffset(size_t *
offset, const texture< T, dim, readMode>  &
tex) 

{ 
return cudaGetTextureAlignmentOffset(offset, &tex); 
} 
#line 708 "C:\\CUDA\\include\\cuda_runtime.h"
template<class T> __inline cudaError_t 
cudaLaunch(T *
entry) 

{ 
return cudaLaunch((const char *)entry); 
} 
#line 744 "C:\\CUDA\\include\\cuda_runtime.h"
template<class T> __inline cudaError_t 
cudaFuncGetAttributes(cudaFuncAttributes *
attr, T *
entry) 

{ 
return cudaFuncGetAttributes(attr, (const char *)entry); 
} 
#line 28 "C:\\Program Files\\Microsoft Visual Studio 8\\VC\\INCLUDE\\stdio.h"
#pragma pack ( push, 8 )
#line 59 "C:\\Program Files\\Microsoft Visual Studio 8\\VC\\INCLUDE\\stdio.h"
extern "C" { struct _iobuf { 
char *_ptr; 
int _cnt; 
char *_base; 
int _flag; 
int _file; 
int _charbuf; 
int _bufsiz; 
char *_tmpfname; 
}; }
extern "C" { typedef _iobuf FILE; }
#line 131 "C:\\Program Files\\Microsoft Visual Studio 8\\VC\\INCLUDE\\stdio.h"
extern "C" { extern __declspec( dllimport ) FILE *__cdecl __iob_func(); } 
#line 147 "C:\\Program Files\\Microsoft Visual Studio 8\\VC\\INCLUDE\\stdio.h"
extern "C" { typedef __int64 fpos_t; }
#line 188 "C:\\Program Files\\Microsoft Visual Studio 8\\VC\\INCLUDE\\stdio.h"
extern "C" { extern __declspec( dllimport ) int __cdecl _filbuf(FILE *); } 
extern "C" { extern __declspec( dllimport ) int __cdecl _flsbuf(int, FILE *); } 




extern "C" { extern __declspec( dllimport ) FILE *__cdecl _fsopen(const char *, const char *, int); } 
#line 197 "C:\\Program Files\\Microsoft Visual Studio 8\\VC\\INCLUDE\\stdio.h"
extern "C" { extern __declspec( dllimport ) void __cdecl clearerr(FILE *); } 
extern "C" { extern __declspec( dllimport ) errno_t __cdecl clearerr_s(FILE *); } 
extern "C" { extern __declspec( dllimport ) int __cdecl fclose(FILE *); } 
extern "C" { extern __declspec( dllimport ) int __cdecl _fcloseall(); } 




extern "C" { extern __declspec( dllimport ) FILE *__cdecl _fdopen(int, const char *); } 
#line 208 "C:\\Program Files\\Microsoft Visual Studio 8\\VC\\INCLUDE\\stdio.h"
extern "C" { extern __declspec( dllimport ) int __cdecl feof(FILE *); } 
extern "C" { extern __declspec( dllimport ) int __cdecl ferror(FILE *); } 
extern "C" { extern __declspec( dllimport ) int __cdecl fflush(FILE *); } 
extern "C" { extern __declspec( dllimport ) int __cdecl fgetc(FILE *); } 
extern "C" { extern __declspec( dllimport ) int __cdecl _fgetchar(); } 
extern "C" { extern __declspec( dllimport ) int __cdecl fgetpos(FILE *, fpos_t *); } 
extern "C" { extern __declspec( dllimport ) char *__cdecl fgets(char *, int, FILE *); } 




extern "C" { extern __declspec( dllimport ) int __cdecl _fileno(FILE *); } 
#line 227 "C:\\Program Files\\Microsoft Visual Studio 8\\VC\\INCLUDE\\stdio.h"
extern "C" { extern __declspec( dllimport ) char *__cdecl _tempnam(const char *, const char *); } 
#line 233 "C:\\Program Files\\Microsoft Visual Studio 8\\VC\\INCLUDE\\stdio.h"
extern "C" { extern __declspec( dllimport ) int __cdecl _flushall(); } 
extern "C" { extern __declspec( dllimport ) FILE *__cdecl fopen(const char *, const char *); } 

extern "C" { extern __declspec( dllimport ) errno_t __cdecl fopen_s(FILE **, const char *, const char *); } 
#line 238 "C:\\Program Files\\Microsoft Visual Studio 8\\VC\\INCLUDE\\stdio.h"
extern "C" { extern __declspec( dllimport ) int __cdecl fprintf(FILE *, const char *, ...); } 
extern "C" { extern __declspec( dllimport ) int __cdecl fprintf_s(FILE *, const char *, ...); } 
extern "C" { extern __declspec( dllimport ) int __cdecl fputc(int, FILE *); } 
extern "C" { extern __declspec( dllimport ) int __cdecl _fputchar(int); } 
extern "C" { extern __declspec( dllimport ) int __cdecl fputs(const char *, FILE *); } 
extern "C" { extern __declspec( dllimport ) size_t __cdecl fread(void *, size_t, size_t, FILE *); } 
extern "C" { extern __declspec( dllimport ) size_t __cdecl fread_s(void *, size_t, size_t, size_t, FILE *); } 
extern "C" { extern __declspec( dllimport ) FILE *__cdecl freopen(const char *, const char *, FILE *); } 

extern "C" { extern __declspec( dllimport ) errno_t __cdecl freopen_s(FILE **, const char *, const char *, FILE *); } 
#line 249 "C:\\Program Files\\Microsoft Visual Studio 8\\VC\\INCLUDE\\stdio.h"
extern "C" { extern __declspec( dllimport ) int __cdecl fscanf(FILE *, const char *, ...); } 
extern "C" { extern __declspec( dllimport ) int __cdecl _fscanf_l(FILE *, const char *, _locale_t, ...); } 

extern "C" { extern __declspec( dllimport ) int __cdecl fscanf_s(FILE *, const char *, ...); } 
#line 254 "C:\\Program Files\\Microsoft Visual Studio 8\\VC\\INCLUDE\\stdio.h"
extern "C" { extern __declspec( dllimport ) int __cdecl _fscanf_s_l(FILE *, const char *, _locale_t, ...); } 
extern "C" { extern __declspec( dllimport ) int __cdecl fsetpos(FILE *, const fpos_t *); } 
extern "C" { extern __declspec( dllimport ) int __cdecl fseek(FILE *, long, int); } 
extern "C" { extern __declspec( dllimport ) long __cdecl ftell(FILE *); } 

extern "C" { extern __declspec( dllimport ) int __cdecl _fseeki64(FILE *, __int64, int); } 
extern "C" { extern __declspec( dllimport ) __int64 __cdecl _ftelli64(FILE *); } 

extern "C" { extern __declspec( dllimport ) size_t __cdecl fwrite(const void *, size_t, size_t, FILE *); } 
extern "C" { extern __declspec( dllimport ) int __cdecl getc(FILE *); } 
extern "C" { extern __declspec( dllimport ) int __cdecl getchar(); } 
extern "C" { extern __declspec( dllimport ) int __cdecl _getmaxstdio(); } 

extern "C" { extern __declspec( dllimport ) char *__cdecl gets_s(char *, rsize_t); } 
#line 269 "C:\\Program Files\\Microsoft Visual Studio 8\\VC\\INCLUDE\\stdio.h"
template < size_t _Size > inline char * __cdecl gets_s ( char ( & _Buffer ) [ _Size ] ) { return gets_s ( _Buffer, _Size ); }
extern "C" { extern __declspec( dllimport ) char *__cdecl gets(char *); } 
extern "C" { extern int __cdecl _getw(FILE *); } 




extern "C" { extern __declspec( dllimport ) int __cdecl _pclose(FILE *); } 
extern "C" { extern __declspec( dllimport ) FILE *__cdecl _popen(const char *, const char *); } 
extern "C" { extern __declspec( dllimport ) int __cdecl printf(const char *, ...); } 
extern "C" { extern __declspec( dllimport ) int __cdecl printf_s(const char *, ...); } 
extern "C" { extern __declspec( dllimport ) int __cdecl putc(int, FILE *); } 
extern "C" { extern __declspec( dllimport ) int __cdecl putchar(int); } 
extern "C" { extern __declspec( dllimport ) int __cdecl puts(const char *); } 
extern "C" { extern __declspec( dllimport ) int __cdecl _putw(int, FILE *); } 


extern "C" { extern __declspec( dllimport ) int __cdecl remove(const char *); } 
extern "C" { extern __declspec( dllimport ) int __cdecl rename(const char *, const char *); } 
extern "C" { extern __declspec( dllimport ) int __cdecl _unlink(const char *); } 

extern "C" { extern __declspec( dllimport ) int __cdecl unlink(const char *); } 
#line 293 "C:\\Program Files\\Microsoft Visual Studio 8\\VC\\INCLUDE\\stdio.h"
extern "C" { extern __declspec( dllimport ) void __cdecl rewind(FILE *); } 
extern "C" { extern __declspec( dllimport ) int __cdecl _rmtmp(); } 
extern "C" { extern __declspec( dllimport ) int __cdecl scanf(const char *, ...); } 
extern "C" { extern __declspec( dllimport ) int __cdecl _scanf_l(const char *, _locale_t, ...); } 

extern "C" { extern __declspec( dllimport ) int __cdecl scanf_s(const char *, ...); } 
#line 300 "C:\\Program Files\\Microsoft Visual Studio 8\\VC\\INCLUDE\\stdio.h"
extern "C" { extern __declspec( dllimport ) int __cdecl _scanf_s_l(const char *, _locale_t, ...); } 
extern "C" { extern __declspec( dllimport ) void __cdecl setbuf(FILE *, char *); } 
extern "C" { extern __declspec( dllimport ) int __cdecl _setmaxstdio(int); } 
extern "C" { extern __declspec( dllimport ) unsigned __cdecl _set_output_format(unsigned); } 
extern "C" { extern __declspec( dllimport ) unsigned __cdecl _get_output_format(); } 
extern "C" { extern __declspec( dllimport ) int __cdecl setvbuf(FILE *, char *, int, size_t); } 
extern "C" { extern __declspec( dllimport ) int __cdecl _snprintf_s(char *, size_t, size_t, const char *, ...); } 
__pragma(warning(push)) __pragma(warning(disable: 4793)) template < size_t _Size > inline int __cdecl _snprintf_s ( char ( & _Dest ) [ _Size ], size_t _Size, const char * _Format, ... ) { va_list _ArgList; ( _ArgList = ( va_list ) ( & reinterpret_cast < const char & > ( _Format ) ) + ( ( sizeof ( _Format ) + sizeof ( int ) - 1 ) & ~ ( sizeof ( int ) - 1 ) ) ); return _vsnprintf_s ( _Dest, _Size, _Size, _Format, _ArgList ); }__pragma(warning(pop)) 
extern "C" { extern __declspec( dllimport ) int __cdecl sprintf_s(char *, size_t, const char *, ...); } 
__pragma(warning(push)) __pragma(warning(disable: 4793)) template < size_t _Size > inline int __cdecl sprintf_s ( char ( & _Dest ) [ _Size ], const char * _Format, ... ) { va_list _ArgList; ( _ArgList = ( va_list ) ( & reinterpret_cast < const char & > ( _Format ) ) + ( ( sizeof ( _Format ) + sizeof ( int ) - 1 ) & ~ ( sizeof ( int ) - 1 ) ) ); return vsprintf_s ( _Dest, _Size, _Format, _ArgList ); }__pragma(warning(pop)) 
extern "C" { extern __declspec( dllimport ) int __cdecl _scprintf(const char *, ...); } 
extern "C" { extern __declspec( dllimport ) int __cdecl sscanf(const char *, const char *, ...); } 
extern "C" { extern __declspec( dllimport ) int __cdecl _sscanf_l(const char *, const char *, _locale_t, ...); } 

extern "C" { extern __declspec( dllimport ) int __cdecl sscanf_s(const char *, const char *, ...); } 
#line 316 "C:\\Program Files\\Microsoft Visual Studio 8\\VC\\INCLUDE\\stdio.h"
extern "C" { extern __declspec( dllimport ) int __cdecl _sscanf_s_l(const char *, const char *, _locale_t, ...); } 
extern "C" { extern __declspec( dllimport ) int __cdecl _snscanf(const char *, size_t, const char *, ...); } 
extern "C" { extern __declspec( dllimport ) int __cdecl _snscanf_l(const char *, size_t, const char *, _locale_t, ...); } 
extern "C" { extern __declspec( dllimport ) int __cdecl _snscanf_s(const char *, size_t, const char *, ...); } 
extern "C" { extern __declspec( dllimport ) int __cdecl _snscanf_s_l(const char *, size_t, const char *, _locale_t, ...); } 
extern "C" { extern __declspec( dllimport ) FILE *__cdecl tmpfile(); } 

extern "C" { extern __declspec( dllimport ) errno_t __cdecl tmpfile_s(FILE **); } 
extern "C" { extern __declspec( dllimport ) errno_t __cdecl tmpnam_s(char *, rsize_t); } 
#line 326 "C:\\Program Files\\Microsoft Visual Studio 8\\VC\\INCLUDE\\stdio.h"
template < size_t _Size > inline errno_t __cdecl tmpnam_s ( char ( & _Buf ) [ _Size ] ) { return tmpnam_s ( _Buf, _Size ); }
extern "C" { extern __declspec( dllimport ) char *__cdecl tmpnam(char *); } 
extern "C" { extern __declspec( dllimport ) int __cdecl ungetc(int, FILE *); } 
extern "C" { extern __declspec( dllimport ) int __cdecl vfprintf(FILE *, const char *, va_list); } 
extern "C" { extern __declspec( dllimport ) int __cdecl vfprintf_s(FILE *, const char *, va_list); } 
extern "C" { extern __declspec( dllimport ) int __cdecl vprintf(const char *, va_list); } 
extern "C" { extern __declspec( dllimport ) int __cdecl vprintf_s(const char *, va_list); } 
extern "C" { extern __declspec( dllimport ) int __cdecl vsnprintf(char *, size_t, const char *, va_list); } 
extern "C" { extern __declspec( dllimport ) int __cdecl vsnprintf_s(char *, size_t, size_t, const char *, va_list); } 
extern "C" { extern __declspec( dllimport ) int __cdecl _vsnprintf_s(char *, size_t, size_t, const char *, va_list); } 
template < size_t _Size > inline int __cdecl _vsnprintf_s ( char ( & _Dest ) [ _Size ], size_t _Size, const char * _Format, va_list _Args ) { return _vsnprintf_s ( _Dest, _Size, _Size, _Format, _Args ); }
#pragma warning(push)
#pragma warning(disable:4793)
extern "C" { extern __declspec( dllimport ) int __cdecl _snprintf(char *, size_t, const char *, ...); } extern "C" { extern __declspec( dllimport ) int __cdecl _vsnprintf(char *, size_t, const char *, va_list); } 
#pragma warning(pop)
extern "C" { extern __declspec( dllimport ) int __cdecl vsprintf_s(char *, size_t, const char *, va_list); } 
template < size_t _Size > inline int __cdecl vsprintf_s ( char ( & _Dest ) [ _Size ], const char * _Format, va_list _Args ) { return vsprintf_s ( _Dest, _Size, _Format, _Args ); }
#pragma warning(push)
#pragma warning(disable:4793)
extern "C" { extern __declspec( dllimport ) int __cdecl sprintf(char *, const char *, ...); } extern "C" { extern __declspec( dllimport ) int __cdecl vsprintf(char *, const char *, va_list); } 
#pragma warning(pop)
extern "C" { extern __declspec( dllimport ) int __cdecl _vscprintf(const char *, va_list); } 
extern "C" { extern __declspec( dllimport ) int __cdecl _snprintf_c(char *, size_t, const char *, ...); } 
extern "C" { extern __declspec( dllimport ) int __cdecl _vsnprintf_c(char *, size_t, const char *, va_list); } 

extern "C" { extern __declspec( dllimport ) int __cdecl _fprintf_p(FILE *, const char *, ...); } 
extern "C" { extern __declspec( dllimport ) int __cdecl _printf_p(const char *, ...); } 
extern "C" { extern __declspec( dllimport ) int __cdecl _sprintf_p(char *, size_t, const char *, ...); } 
extern "C" { extern __declspec( dllimport ) int __cdecl _vfprintf_p(FILE *, const char *, va_list); } 
extern "C" { extern __declspec( dllimport ) int __cdecl _vprintf_p(const char *, va_list); } 
extern "C" { extern __declspec( dllimport ) int __cdecl _vsprintf_p(char *, size_t, const char *, va_list); } 
extern "C" { extern __declspec( dllimport ) int __cdecl _scprintf_p(const char *, ...); } 
extern "C" { extern __declspec( dllimport ) int __cdecl _vscprintf_p(const char *, va_list); } 
extern "C" { extern __declspec( dllimport ) int __cdecl _set_printf_count_output(int); } 
extern "C" { extern __declspec( dllimport ) int __cdecl _get_printf_count_output(); } 

extern "C" { extern __declspec( dllimport ) int __cdecl _printf_l(const char *, _locale_t, ...); } 
extern "C" { extern __declspec( dllimport ) int __cdecl _printf_p_l(const char *, _locale_t, ...); } 
extern "C" { extern __declspec( dllimport ) int __cdecl _printf_s_l(const char *, _locale_t, ...); } 
extern "C" { extern __declspec( dllimport ) int __cdecl _vprintf_l(const char *, _locale_t, va_list); } 
extern "C" { extern __declspec( dllimport ) int __cdecl _vprintf_p_l(const char *, _locale_t, va_list); } 
extern "C" { extern __declspec( dllimport ) int __cdecl _vprintf_s_l(const char *, _locale_t, va_list); } 

extern "C" { extern __declspec( dllimport ) int __cdecl _fprintf_l(FILE *, const char *, _locale_t, ...); } 
extern "C" { extern __declspec( dllimport ) int __cdecl _fprintf_p_l(FILE *, const char *, _locale_t, ...); } 
extern "C" { extern __declspec( dllimport ) int __cdecl _fprintf_s_l(FILE *, const char *, _locale_t, ...); } 
extern "C" { extern __declspec( dllimport ) int __cdecl _vfprintf_l(FILE *, const char *, _locale_t, va_list); } 
extern "C" { extern __declspec( dllimport ) int __cdecl _vfprintf_p_l(FILE *, const char *, _locale_t, va_list); } 
extern "C" { extern __declspec( dllimport ) int __cdecl _vfprintf_s_l(FILE *, const char *, _locale_t, va_list); } 

extern "C" { extern __declspec( dllimport ) int __cdecl _sprintf_l(char *, const char *, _locale_t, ...); } 
extern "C" { extern __declspec( dllimport ) int __cdecl _sprintf_p_l(char *, size_t, const char *, _locale_t, ...); } 
extern "C" { extern __declspec( dllimport ) int __cdecl _sprintf_s_l(char *, size_t, const char *, _locale_t, ...); } 
extern "C" { extern __declspec( dllimport ) int __cdecl _vsprintf_l(char *, const char *, _locale_t, va_list); } 
extern "C" { extern __declspec( dllimport ) int __cdecl _vsprintf_p_l(char *, size_t, const char *, _locale_t, va_list); } 
extern "C" { extern __declspec( dllimport ) int __cdecl _vsprintf_s_l(char *, size_t, const char *, _locale_t, va_list); } 

extern "C" { extern __declspec( dllimport ) int __cdecl _scprintf_l(const char *, _locale_t, ...); } 
extern "C" { extern __declspec( dllimport ) int __cdecl _scprintf_p_l(const char *, _locale_t, ...); } 
extern "C" { extern __declspec( dllimport ) int __cdecl _vscprintf_l(const char *, _locale_t, va_list); } 
extern "C" { extern __declspec( dllimport ) int __cdecl _vscprintf_p_l(const char *, _locale_t, va_list); } 

extern "C" { extern __declspec( dllimport ) int __cdecl _snprintf_l(char *, size_t, const char *, _locale_t, ...); } 
extern "C" { extern __declspec( dllimport ) int __cdecl _snprintf_c_l(char *, size_t, const char *, _locale_t, ...); } 
extern "C" { extern __declspec( dllimport ) int __cdecl _snprintf_s_l(char *, size_t, size_t, const char *, _locale_t, ...); } 
extern "C" { extern __declspec( dllimport ) int __cdecl _vsnprintf_l(char *, size_t, const char *, _locale_t, va_list); } 
extern "C" { extern __declspec( dllimport ) int __cdecl _vsnprintf_c_l(char *, size_t, const char *, _locale_t, va_list); } 
extern "C" { extern __declspec( dllimport ) int __cdecl _vsnprintf_s_l(char *, size_t, size_t, const char *, _locale_t, va_list); } 
#line 406 "C:\\Program Files\\Microsoft Visual Studio 8\\VC\\INCLUDE\\stdio.h"
extern "C" { extern __declspec( dllimport ) FILE *__cdecl _wfsopen(const __wchar_t *, const __wchar_t *, int); } 
#line 409 "C:\\Program Files\\Microsoft Visual Studio 8\\VC\\INCLUDE\\stdio.h"
extern "C" { extern __declspec( dllimport ) wint_t __cdecl fgetwc(FILE *); } 
extern "C" { extern __declspec( dllimport ) wint_t __cdecl _fgetwchar(); } 
extern "C" { extern __declspec( dllimport ) wint_t __cdecl fputwc(__wchar_t, FILE *); } 
extern "C" { extern __declspec( dllimport ) wint_t __cdecl _fputwchar(__wchar_t); } 
extern "C" { extern __declspec( dllimport ) wint_t __cdecl getwc(FILE *); } 
extern "C" { inline __declspec( dllimport ) wint_t __cdecl getwchar(); } 
extern "C" { extern __declspec( dllimport ) wint_t __cdecl putwc(__wchar_t, FILE *); } 
extern "C" { inline __declspec( dllimport ) wint_t __cdecl putwchar(__wchar_t); } 
extern "C" { extern __declspec( dllimport ) wint_t __cdecl ungetwc(wint_t, FILE *); } 

extern "C" { extern __declspec( dllimport ) __wchar_t *__cdecl fgetws(__wchar_t *, int, FILE *); } 
extern "C" { extern __declspec( dllimport ) int __cdecl fputws(const __wchar_t *, FILE *); } 
extern "C" { extern __declspec( dllimport ) __wchar_t *__cdecl _getws_s(__wchar_t *, size_t); } 
template < size_t _Size > inline wchar_t * __cdecl _getws_s ( wchar_t ( & _String ) [ _Size ] ) { return _getws_s ( _String, _Size ); }
extern "C" { extern __declspec( dllimport ) __wchar_t *__cdecl _getws(__wchar_t *); } 
extern "C" { extern __declspec( dllimport ) int __cdecl _putws(const __wchar_t *); } 

extern "C" { extern __declspec( dllimport ) int __cdecl fwprintf(FILE *, const __wchar_t *, ...); } 
extern "C" { extern __declspec( dllimport ) int __cdecl fwprintf_s(FILE *, const __wchar_t *, ...); } 
extern "C" { extern __declspec( dllimport ) int __cdecl wprintf(const __wchar_t *, ...); } 
extern "C" { extern __declspec( dllimport ) int __cdecl wprintf_s(const __wchar_t *, ...); } 
extern "C" { extern __declspec( dllimport ) int __cdecl _scwprintf(const __wchar_t *, ...); } 
extern "C" { extern __declspec( dllimport ) int __cdecl vfwprintf(FILE *, const __wchar_t *, va_list); } 
extern "C" { extern __declspec( dllimport ) int __cdecl vfwprintf_s(FILE *, const __wchar_t *, va_list); } 
extern "C" { extern __declspec( dllimport ) int __cdecl vwprintf(const __wchar_t *, va_list); } 
extern "C" { extern __declspec( dllimport ) int __cdecl vwprintf_s(const __wchar_t *, va_list); } 

extern "C" { extern __declspec( dllimport ) int __cdecl swprintf_s(__wchar_t *, size_t, const __wchar_t *, ...); } 
__pragma(warning(push)) __pragma(warning(disable: 4793)) template < size_t _Size > inline int __cdecl swprintf_s ( wchar_t ( & _Dest ) [ _Size ], const wchar_t * _Format, ... ) { va_list _ArgList; ( _ArgList = ( va_list ) ( & reinterpret_cast < const char & > ( _Format ) ) + ( ( sizeof ( _Format ) + sizeof ( int ) - 1 ) & ~ ( sizeof ( int ) - 1 ) ) ); return vswprintf_s ( _Dest, _Size, _Format, _ArgList ); }__pragma(warning(pop)) 
extern "C" { extern __declspec( dllimport ) int __cdecl vswprintf_s(__wchar_t *, size_t, const __wchar_t *, va_list); } 
template < size_t _Size > inline int __cdecl vswprintf_s ( wchar_t ( & _Dest ) [ _Size ], const wchar_t * _Format, va_list _Args ) { return vswprintf_s ( _Dest, _Size, _Format, _Args ); }

extern "C" { extern __declspec( dllimport ) int __cdecl _swprintf_c(__wchar_t *, size_t, const __wchar_t *, ...); } 
extern "C" { extern __declspec( dllimport ) int __cdecl _vswprintf_c(__wchar_t *, size_t, const __wchar_t *, va_list); } 

extern "C" { extern __declspec( dllimport ) int __cdecl _snwprintf_s(__wchar_t *, size_t, size_t, const __wchar_t *, ...); } 
__pragma(warning(push)) __pragma(warning(disable: 4793)) template < size_t _Size > inline int __cdecl _snwprintf_s ( wchar_t ( & _Dest ) [ _Size ], size_t _Count, const wchar_t * _Format, ... ) { va_list _ArgList; ( _ArgList = ( va_list ) ( & reinterpret_cast < const char & > ( _Format ) ) + ( ( sizeof ( _Format ) + sizeof ( int ) - 1 ) & ~ ( sizeof ( int ) - 1 ) ) ); return _vsnwprintf_s ( _Dest, _Size, _Count, _Format, _ArgList ); }__pragma(warning(pop)) 
extern "C" { extern __declspec( dllimport ) int __cdecl _vsnwprintf_s(__wchar_t *, size_t, size_t, const __wchar_t *, va_list); } 
template < size_t _Size > inline int __cdecl _vsnwprintf_s ( wchar_t ( & _Dest ) [ _Size ], size_t _Count, const wchar_t * _Format, va_list _Args ) { return _vsnwprintf_s ( _Dest, _Size, _Count, _Format, _Args ); }
#pragma warning(push)
#pragma warning(disable:4793)
extern "C" { extern __declspec( dllimport ) int __cdecl _snwprintf(__wchar_t *, size_t, const __wchar_t *, ...); } extern "C" { extern __declspec( dllimport ) int __cdecl _vsnwprintf(__wchar_t *, size_t, const __wchar_t *, va_list); } 
#pragma warning(pop)

extern "C" { extern __declspec( dllimport ) int __cdecl _fwprintf_p(FILE *, const __wchar_t *, ...); } 
extern "C" { extern __declspec( dllimport ) int __cdecl _wprintf_p(const __wchar_t *, ...); } 
extern "C" { extern __declspec( dllimport ) int __cdecl _vfwprintf_p(FILE *, const __wchar_t *, va_list); } 
extern "C" { extern __declspec( dllimport ) int __cdecl _vwprintf_p(const __wchar_t *, va_list); } 
extern "C" { extern __declspec( dllimport ) int __cdecl _swprintf_p(__wchar_t *, size_t, const __wchar_t *, ...); } 
extern "C" { extern __declspec( dllimport ) int __cdecl _vswprintf_p(__wchar_t *, size_t, const __wchar_t *, va_list); } 
extern "C" { extern __declspec( dllimport ) int __cdecl _scwprintf_p(const __wchar_t *, ...); } 
extern "C" { extern __declspec( dllimport ) int __cdecl _vscwprintf_p(const __wchar_t *, va_list); } 

extern "C" { extern __declspec( dllimport ) int __cdecl _wprintf_l(const __wchar_t *, _locale_t, ...); } 
extern "C" { extern __declspec( dllimport ) int __cdecl _wprintf_p_l(const __wchar_t *, _locale_t, ...); } 
extern "C" { extern __declspec( dllimport ) int __cdecl _wprintf_s_l(const __wchar_t *, _locale_t, ...); } 
extern "C" { extern __declspec( dllimport ) int __cdecl _vwprintf_l(const __wchar_t *, _locale_t, va_list); } 
extern "C" { extern __declspec( dllimport ) int __cdecl _vwprintf_p_l(const __wchar_t *, _locale_t, va_list); } 
extern "C" { extern __declspec( dllimport ) int __cdecl _vwprintf_s_l(const __wchar_t *, _locale_t, va_list); } 

extern "C" { extern __declspec( dllimport ) int __cdecl _fwprintf_l(FILE *, const __wchar_t *, _locale_t, ...); } 
extern "C" { extern __declspec( dllimport ) int __cdecl _fwprintf_p_l(FILE *, const __wchar_t *, _locale_t, ...); } 
extern "C" { extern __declspec( dllimport ) int __cdecl _fwprintf_s_l(FILE *, const __wchar_t *, _locale_t, ...); } 
extern "C" { extern __declspec( dllimport ) int __cdecl _vfwprintf_l(FILE *, const __wchar_t *, _locale_t, va_list); } 
extern "C" { extern __declspec( dllimport ) int __cdecl _vfwprintf_p_l(FILE *, const __wchar_t *, _locale_t, va_list); } 
extern "C" { extern __declspec( dllimport ) int __cdecl _vfwprintf_s_l(FILE *, const __wchar_t *, _locale_t, va_list); } 

extern "C" { extern __declspec( dllimport ) int __cdecl _swprintf_c_l(__wchar_t *, size_t, const __wchar_t *, _locale_t, ...); } 
extern "C" { extern __declspec( dllimport ) int __cdecl _swprintf_p_l(__wchar_t *, size_t, const __wchar_t *, _locale_t, ...); } 
extern "C" { extern __declspec( dllimport ) int __cdecl _swprintf_s_l(__wchar_t *, size_t, const __wchar_t *, _locale_t, ...); } 
extern "C" { extern __declspec( dllimport ) int __cdecl _vswprintf_c_l(__wchar_t *, size_t, const __wchar_t *, _locale_t, va_list); } 
extern "C" { extern __declspec( dllimport ) int __cdecl _vswprintf_p_l(__wchar_t *, size_t, const __wchar_t *, _locale_t, va_list); } 
extern "C" { extern __declspec( dllimport ) int __cdecl _vswprintf_s_l(__wchar_t *, size_t, const __wchar_t *, _locale_t, va_list); } 

extern "C" { extern __declspec( dllimport ) int __cdecl _scwprintf_l(const __wchar_t *, _locale_t, ...); } 
extern "C" { extern __declspec( dllimport ) int __cdecl _scwprintf_p_l(const __wchar_t *, _locale_t, ...); } 
extern "C" { extern __declspec( dllimport ) int __cdecl _vscwprintf_p_l(const __wchar_t *, _locale_t, va_list); } 

extern "C" { extern __declspec( dllimport ) int __cdecl _snwprintf_l(__wchar_t *, size_t, const __wchar_t *, _locale_t, ...); } 
extern "C" { extern __declspec( dllimport ) int __cdecl _snwprintf_s_l(__wchar_t *, size_t, size_t, const __wchar_t *, _locale_t, ...); } 
extern "C" { extern __declspec( dllimport ) int __cdecl _vsnwprintf_l(__wchar_t *, size_t, const __wchar_t *, _locale_t, va_list); } 
extern "C" { extern __declspec( dllimport ) int __cdecl _vsnwprintf_s_l(__wchar_t *, size_t, size_t, const __wchar_t *, _locale_t, va_list); } 
#line 504 "C:\\Program Files\\Microsoft Visual Studio 8\\VC\\INCLUDE\\stdio.h"
#pragma warning(push)
#pragma warning(disable:4141 4996 4793)
extern "C" { extern __declspec( dllimport ) __declspec(deprecated("swprintf has been changed to conform with the ISO C standard, adding an extra character count parameter. To use traditional Microsoft swprintf, set _CRT_NON_CONFORMING_SWPRINTFS.")) int __cdecl _swprintf(__wchar_t *, const __wchar_t *, ...); } extern "C" { extern __declspec( dllimport ) __declspec(deprecated("swprintf has been changed to conform with the ISO C standard, adding an extra character count parameter. To use traditional Microsoft swprintf, set _CRT_NON_CONFORMING_SWPRINTFS.")) int __cdecl _vswprintf(__wchar_t *, const __wchar_t *, va_list); } 
extern "C" { extern __declspec( dllimport ) __declspec(deprecated("swprintf has been changed to conform with the ISO C standard, adding an extra character count parameter. To use traditional Microsoft swprintf, set _CRT_NON_CONFORMING_SWPRINTFS.")) int __cdecl __swprintf_l(__wchar_t *, const __wchar_t *, _locale_t, ...); } extern "C" { extern __declspec( dllimport ) __declspec(deprecated("swprintf has been changed to conform with the ISO C standard, adding an extra character count parameter. To use traditional Microsoft swprintf, set _CRT_NON_CONFORMING_SWPRINTFS.")) int __cdecl __vswprintf_l(__wchar_t *, const __wchar_t *, _locale_t, va_list); } 
#pragma warning(pop)
#line 34 "C:\\Program Files\\Microsoft Visual Studio 8\\VC\\INCLUDE\\swprintf.inl"
#pragma warning( push )
#pragma warning( disable : 4793 4412 )
extern "C" { static __inline int swprintf(__wchar_t *_String, size_t _Count, const __wchar_t *_Format, ...) 
{ 
auto va_list _Arglist; 
auto int _Ret; 
_Arglist = (va_list)(&(reinterpret_cast< const char &>(_Format))) + (((sizeof(_Format) + sizeof(int)) - (1)) & (~(sizeof(int) - (1)))); 
_Ret = _vswprintf_c_l(_String, _Count, _Format, 0, _Arglist); 
_Arglist = ((va_list)0); 
return _Ret; 
} } 
#pragma warning( pop )

#pragma warning( push )
#pragma warning( disable : 4412 )
extern "C" { static __inline int __cdecl vswprintf(__wchar_t *_String, size_t _Count, const __wchar_t *_Format, va_list _Ap) 
{ 
return _vswprintf_c_l(_String, _Count, _Format, 0, _Ap); 
} } 
#pragma warning( pop )
#line 58 "C:\\Program Files\\Microsoft Visual Studio 8\\VC\\INCLUDE\\swprintf.inl"
#pragma warning( push )
#pragma warning( disable : 4793 4412 )
extern "C" { static __inline int _swprintf_l(__wchar_t *_String, size_t _Count, const __wchar_t *_Format, _locale_t _Plocinfo, ...) 
{ 
auto va_list _Arglist; 
auto int _Ret; 
_Arglist = (va_list)(&(reinterpret_cast< const char &>(_Plocinfo))) + (((sizeof(_Plocinfo) + sizeof(int)) - (1)) & (~(sizeof(int) - (1)))); 
_Ret = _vswprintf_c_l(_String, _Count, _Format, _Plocinfo, _Arglist); 
_Arglist = ((va_list)0); 
return _Ret; 
} } 
#pragma warning( pop )

#pragma warning( push )
#pragma warning( disable : 4412 )
extern "C" { static __inline int __cdecl _vswprintf_l(__wchar_t *_String, size_t _Count, const __wchar_t *_Format, _locale_t _Plocinfo, va_list _Ap) 
{ 
return _vswprintf_c_l(_String, _Count, _Format, _Plocinfo, _Ap); 
} } 
#pragma warning( pop )


#pragma warning( push )
#pragma warning( disable : 4996 )

#pragma warning( push )
#pragma warning( disable : 4793 4141 )
__inline __declspec(deprecated("swprintf has been changed to conform with the ISO C standard, adding an extra character count parameter. To use traditional Microsoft swprintf, set _CRT_NON_CONFORMING_SWPRINTFS.")) int swprintf(__wchar_t *_String, const __wchar_t *_Format, ...) 
{ 
auto va_list _Arglist; 
_Arglist = (va_list)(&(reinterpret_cast< const char &>(_Format))) + (((sizeof(_Format) + sizeof(int)) - (1)) & (~(sizeof(int) - (1)))); 
auto int _Ret = _vswprintf(_String, _Format, _Arglist); 
_Arglist = ((va_list)0); 
return _Ret; 
} 
#pragma warning( pop )

#pragma warning( push )
#pragma warning( disable : 4141 )
__inline __declspec(deprecated("swprintf has been changed to conform with the ISO C standard, adding an extra character count parameter. To use traditional Microsoft swprintf, set _CRT_NON_CONFORMING_SWPRINTFS.")) int __cdecl vswprintf(__wchar_t *_String, const __wchar_t *_Format, va_list _Ap) 
{ 
return _vswprintf(_String, _Format, _Ap); 
} 
#pragma warning( pop )

#pragma warning( push )
#pragma warning( disable : 4793 4141 )
__inline __declspec(deprecated("swprintf has been changed to conform with the ISO C standard, adding an extra character count parameter. To use traditional Microsoft swprintf, set _CRT_NON_CONFORMING_SWPRINTFS.")) int _swprintf_l(__wchar_t *_String, const __wchar_t *_Format, _locale_t _Plocinfo, ...) 
{ 
auto va_list _Arglist; 
_Arglist = (va_list)(&(reinterpret_cast< const char &>(_Plocinfo))) + (((sizeof(_Plocinfo) + sizeof(int)) - (1)) & (~(sizeof(int) - (1)))); 
auto int _Ret = __vswprintf_l(_String, _Format, _Plocinfo, _Arglist); 
_Arglist = ((va_list)0); 
return _Ret; 
} 
#pragma warning( pop )

#pragma warning( push )
#pragma warning( disable : 4141 )
__inline __declspec(deprecated("swprintf has been changed to conform with the ISO C standard, adding an extra character count parameter. To use traditional Microsoft swprintf, set _CRT_NON_CONFORMING_SWPRINTFS.")) int __cdecl _vswprintf_l(__wchar_t *_String, const __wchar_t *_Format, _locale_t _Plocinfo, va_list _Ap) 
{ 
return __vswprintf_l(_String, _Format, _Plocinfo, _Ap); 
} 
#pragma warning( pop )

#pragma warning( pop )
#line 528 "C:\\Program Files\\Microsoft Visual Studio 8\\VC\\INCLUDE\\stdio.h"
extern "C" { extern __declspec( dllimport ) __wchar_t *__cdecl _wtempnam(const __wchar_t *, const __wchar_t *); } 
#line 534 "C:\\Program Files\\Microsoft Visual Studio 8\\VC\\INCLUDE\\stdio.h"
extern "C" { extern __declspec( dllimport ) int __cdecl _vscwprintf(const __wchar_t *, va_list); } 
extern "C" { extern __declspec( dllimport ) int __cdecl _vscwprintf_l(const __wchar_t *, _locale_t, va_list); } 
extern "C" { extern __declspec( dllimport ) int __cdecl fwscanf(FILE *, const __wchar_t *, ...); } 
extern "C" { extern __declspec( dllimport ) int __cdecl _fwscanf_l(FILE *, const __wchar_t *, _locale_t, ...); } 

extern "C" { extern __declspec( dllimport ) int __cdecl fwscanf_s(FILE *, const __wchar_t *, ...); } 
#line 541 "C:\\Program Files\\Microsoft Visual Studio 8\\VC\\INCLUDE\\stdio.h"
extern "C" { extern __declspec( dllimport ) int __cdecl _fwscanf_s_l(FILE *, const __wchar_t *, _locale_t, ...); } 
extern "C" { extern __declspec( dllimport ) int __cdecl swscanf(const __wchar_t *, const __wchar_t *, ...); } 
extern "C" { extern __declspec( dllimport ) int __cdecl _swscanf_l(const __wchar_t *, const __wchar_t *, _locale_t, ...); } 

extern "C" { extern __declspec( dllimport ) int __cdecl swscanf_s(const __wchar_t *, const __wchar_t *, ...); } 
#line 547 "C:\\Program Files\\Microsoft Visual Studio 8\\VC\\INCLUDE\\stdio.h"
extern "C" { extern __declspec( dllimport ) int __cdecl _swscanf_s_l(const __wchar_t *, const __wchar_t *, _locale_t, ...); } 
extern "C" { extern __declspec( dllimport ) int __cdecl _snwscanf(const __wchar_t *, size_t, const __wchar_t *, ...); } 
extern "C" { extern __declspec( dllimport ) int __cdecl _snwscanf_l(const __wchar_t *, size_t, const __wchar_t *, _locale_t, ...); } 
extern "C" { extern __declspec( dllimport ) int __cdecl _snwscanf_s(const __wchar_t *, size_t, const __wchar_t *, ...); } 
extern "C" { extern __declspec( dllimport ) int __cdecl _snwscanf_s_l(const __wchar_t *, size_t, const __wchar_t *, _locale_t, ...); } 
extern "C" { extern __declspec( dllimport ) int __cdecl wscanf(const __wchar_t *, ...); } 
extern "C" { extern __declspec( dllimport ) int __cdecl _wscanf_l(const __wchar_t *, _locale_t, ...); } 

extern "C" { extern __declspec( dllimport ) int __cdecl wscanf_s(const __wchar_t *, ...); } 
#line 557 "C:\\Program Files\\Microsoft Visual Studio 8\\VC\\INCLUDE\\stdio.h"
extern "C" { extern __declspec( dllimport ) int __cdecl _wscanf_s_l(const __wchar_t *, _locale_t, ...); } 

extern "C" { extern __declspec( dllimport ) FILE *__cdecl _wfdopen(int, const __wchar_t *); } 
extern "C" { extern __declspec( dllimport ) FILE *__cdecl _wfopen(const __wchar_t *, const __wchar_t *); } 
extern "C" { extern __declspec( dllimport ) errno_t __cdecl _wfopen_s(FILE **, const __wchar_t *, const __wchar_t *); } 
extern "C" { extern __declspec( dllimport ) FILE *__cdecl _wfreopen(const __wchar_t *, const __wchar_t *, FILE *); } 
extern "C" { extern __declspec( dllimport ) errno_t __cdecl _wfreopen_s(FILE **, const __wchar_t *, const __wchar_t *, FILE *); } 
#line 569 "C:\\Program Files\\Microsoft Visual Studio 8\\VC\\INCLUDE\\stdio.h"
extern "C" { extern __declspec( dllimport ) FILE *__cdecl _wpopen(const __wchar_t *, const __wchar_t *); } 
extern "C" { extern __declspec( dllimport ) int __cdecl _wremove(const __wchar_t *); } 
extern "C" { extern __declspec( dllimport ) errno_t __cdecl _wtmpnam_s(__wchar_t *, size_t); } 
template < size_t _Size > inline errno_t __cdecl _wtmpnam_s ( wchar_t ( & _Buffer ) [ _Size ] ) { return _wtmpnam_s ( _Buffer, _Size ); }
extern "C" { extern __declspec( dllimport ) __wchar_t *__cdecl _wtmpnam(__wchar_t *); } 

extern "C" { extern __declspec( dllimport ) wint_t __cdecl _fgetwc_nolock(FILE *); } 
extern "C" { extern __declspec( dllimport ) wint_t __cdecl _fputwc_nolock(__wchar_t, FILE *); } 
extern "C" { extern __declspec( dllimport ) wint_t __cdecl _ungetwc_nolock(wint_t, FILE *); } 
#line 585 "C:\\Program Files\\Microsoft Visual Studio 8\\VC\\INCLUDE\\stdio.h"
extern "C" { inline __declspec( dllimport ) wint_t __cdecl getwchar() 
{ return fgetwc(__iob_func() + 0); } } 
extern "C" { inline __declspec( dllimport ) wint_t __cdecl putwchar(__wchar_t _C) 
{ return fputwc(_C, __iob_func() + 1); } } 
#line 635 "C:\\Program Files\\Microsoft Visual Studio 8\\VC\\INCLUDE\\stdio.h"
extern "C" { extern __declspec( dllimport ) void __cdecl _lock_file(FILE *); } 
extern "C" { extern __declspec( dllimport ) void __cdecl _unlock_file(FILE *); } 
#line 643 "C:\\Program Files\\Microsoft Visual Studio 8\\VC\\INCLUDE\\stdio.h"
extern "C" { extern __declspec( dllimport ) int __cdecl _fclose_nolock(FILE *); } 
extern "C" { extern __declspec( dllimport ) int __cdecl _fflush_nolock(FILE *); } 
extern "C" { extern __declspec( dllimport ) size_t __cdecl _fread_nolock(void *, size_t, size_t, FILE *); } 
extern "C" { extern __declspec( dllimport ) size_t __cdecl _fread_nolock_s(void *, size_t, size_t, size_t, FILE *); } 
extern "C" { extern __declspec( dllimport ) int __cdecl _fseek_nolock(FILE *, long, int); } 
extern "C" { extern __declspec( dllimport ) long __cdecl _ftell_nolock(FILE *); } 
extern "C" { extern __declspec( dllimport ) int __cdecl _fseeki64_nolock(FILE *, __int64, int); } 
extern "C" { extern __declspec( dllimport ) __int64 __cdecl _ftelli64_nolock(FILE *); } 
extern "C" { extern __declspec( dllimport ) size_t __cdecl _fwrite_nolock(const void *, size_t, size_t, FILE *); } 
extern "C" { extern __declspec( dllimport ) int __cdecl _ungetc_nolock(int, FILE *); } 
#line 679 "C:\\Program Files\\Microsoft Visual Studio 8\\VC\\INCLUDE\\stdio.h"
extern "C" { extern __declspec( dllimport ) char *__cdecl tempnam(const char *, const char *); } 
#line 685 "C:\\Program Files\\Microsoft Visual Studio 8\\VC\\INCLUDE\\stdio.h"
extern "C" { extern __declspec( dllimport ) int __cdecl fcloseall(); } 
extern "C" { extern __declspec( dllimport ) FILE *__cdecl fdopen(int, const char *); } 
extern "C" { extern __declspec( dllimport ) int __cdecl fgetchar(); } 
extern "C" { extern __declspec( dllimport ) int __cdecl fileno(FILE *); } 
extern "C" { extern __declspec( dllimport ) int __cdecl flushall(); } 
extern "C" { extern __declspec( dllimport ) int __cdecl fputchar(int); } 
extern "C" { extern __declspec( dllimport ) int __cdecl getw(FILE *); } 
extern "C" { extern __declspec( dllimport ) int __cdecl putw(int, FILE *); } 
extern "C" { extern __declspec( dllimport ) int __cdecl rmtmp(); } 
#line 19 "C:\\Documents and Settings\\All Users\\Application Data\\NVIDIA Corporation\\NVIDIA GPU Computing SDK\\C\\common\\inc\\cutil.h"
#pragma warning( disable : 4996 )
#line 702 "C:\\Program Files\\Microsoft Visual Studio 8\\VC\\INCLUDE\\stdio.h"
#pragma pack ( pop )
#line 49 "C:\\Documents and Settings\\All Users\\Application Data\\NVIDIA Corporation\\NVIDIA GPU Computing SDK\\C\\common\\inc\\cutil.h"
enum CUTBoolean { 

CUTFalse, 
CUTTrue
}; 
#line 61 "C:\\Documents and Settings\\All Users\\Application Data\\NVIDIA Corporation\\NVIDIA GPU Computing SDK\\C\\common\\inc\\cutil.h"
extern "C" { extern __declspec( dllimport ) void __stdcall cutFree(void *); } 
#line 79 "C:\\Documents and Settings\\All Users\\Application Data\\NVIDIA Corporation\\NVIDIA GPU Computing SDK\\C\\common\\inc\\cutil.h"
extern "C" { extern __declspec( dllimport ) void __stdcall cutCheckBankAccess(unsigned, unsigned, unsigned, unsigned, unsigned, unsigned, const char *, const int, const char *, const int); } 
#line 92 "C:\\Documents and Settings\\All Users\\Application Data\\NVIDIA Corporation\\NVIDIA GPU Computing SDK\\C\\common\\inc\\cutil.h"
extern "C" { extern __declspec( dllimport ) char *__stdcall cutFindFilePath(const char *, const char *); } 
#line 107 "C:\\Documents and Settings\\All Users\\Application Data\\NVIDIA Corporation\\NVIDIA GPU Computing SDK\\C\\common\\inc\\cutil.h"
extern "C" { extern __declspec( dllimport ) CUTBoolean __stdcall cutReadFilef(const char *, float **, unsigned *, bool = false); } 
#line 123 "C:\\Documents and Settings\\All Users\\Application Data\\NVIDIA Corporation\\NVIDIA GPU Computing SDK\\C\\common\\inc\\cutil.h"
extern "C" { extern __declspec( dllimport ) CUTBoolean __stdcall cutReadFiled(const char *, double **, unsigned *, bool = false); } 
#line 139 "C:\\Documents and Settings\\All Users\\Application Data\\NVIDIA Corporation\\NVIDIA GPU Computing SDK\\C\\common\\inc\\cutil.h"
extern "C" { extern __declspec( dllimport ) CUTBoolean __stdcall cutReadFilei(const char *, int **, unsigned *, bool = false); } 
#line 154 "C:\\Documents and Settings\\All Users\\Application Data\\NVIDIA Corporation\\NVIDIA GPU Computing SDK\\C\\common\\inc\\cutil.h"
extern "C" { extern __declspec( dllimport ) CUTBoolean __stdcall cutReadFileui(const char *, unsigned **, unsigned *, bool = false); } 
#line 170 "C:\\Documents and Settings\\All Users\\Application Data\\NVIDIA Corporation\\NVIDIA GPU Computing SDK\\C\\common\\inc\\cutil.h"
extern "C" { extern __declspec( dllimport ) CUTBoolean __stdcall cutReadFileb(const char *, char **, unsigned *, bool = false); } 
#line 186 "C:\\Documents and Settings\\All Users\\Application Data\\NVIDIA Corporation\\NVIDIA GPU Computing SDK\\C\\common\\inc\\cutil.h"
extern "C" { extern __declspec( dllimport ) CUTBoolean __stdcall cutReadFileub(const char *, unsigned char **, unsigned *, bool = false); } 
#line 200 "C:\\Documents and Settings\\All Users\\Application Data\\NVIDIA Corporation\\NVIDIA GPU Computing SDK\\C\\common\\inc\\cutil.h"
extern "C" { extern __declspec( dllimport ) CUTBoolean __stdcall cutWriteFilef(const char *, const float *, unsigned, const float, bool = false); } 
#line 214 "C:\\Documents and Settings\\All Users\\Application Data\\NVIDIA Corporation\\NVIDIA GPU Computing SDK\\C\\common\\inc\\cutil.h"
extern "C" { extern __declspec( dllimport ) CUTBoolean __stdcall cutWriteFiled(const char *, const float *, unsigned, const double, bool = false); } 
#line 226 "C:\\Documents and Settings\\All Users\\Application Data\\NVIDIA Corporation\\NVIDIA GPU Computing SDK\\C\\common\\inc\\cutil.h"
extern "C" { extern __declspec( dllimport ) CUTBoolean __stdcall cutWriteFilei(const char *, const int *, unsigned, bool = false); } 
#line 238 "C:\\Documents and Settings\\All Users\\Application Data\\NVIDIA Corporation\\NVIDIA GPU Computing SDK\\C\\common\\inc\\cutil.h"
extern "C" { extern __declspec( dllimport ) CUTBoolean __stdcall cutWriteFileui(const char *, const unsigned *, unsigned, bool = false); } 
#line 250 "C:\\Documents and Settings\\All Users\\Application Data\\NVIDIA Corporation\\NVIDIA GPU Computing SDK\\C\\common\\inc\\cutil.h"
extern "C" { extern __declspec( dllimport ) CUTBoolean __stdcall cutWriteFileb(const char *, const char *, unsigned, bool = false); } 
#line 262 "C:\\Documents and Settings\\All Users\\Application Data\\NVIDIA Corporation\\NVIDIA GPU Computing SDK\\C\\common\\inc\\cutil.h"
extern "C" { extern __declspec( dllimport ) CUTBoolean __stdcall cutWriteFileub(const char *, const unsigned char *, unsigned, bool = false); } 
#line 278 "C:\\Documents and Settings\\All Users\\Application Data\\NVIDIA Corporation\\NVIDIA GPU Computing SDK\\C\\common\\inc\\cutil.h"
extern "C" { extern __declspec( dllimport ) CUTBoolean __stdcall cutLoadPGMub(const char *, unsigned char **, unsigned *, unsigned *); } 
#line 291 "C:\\Documents and Settings\\All Users\\Application Data\\NVIDIA Corporation\\NVIDIA GPU Computing SDK\\C\\common\\inc\\cutil.h"
extern "C" { extern __declspec( dllimport ) CUTBoolean __stdcall cutLoadPPMub(const char *, unsigned char **, unsigned *, unsigned *); } 
#line 305 "C:\\Documents and Settings\\All Users\\Application Data\\NVIDIA Corporation\\NVIDIA GPU Computing SDK\\C\\common\\inc\\cutil.h"
extern "C" { extern __declspec( dllimport ) CUTBoolean __stdcall cutLoadPPM4ub(const char *, unsigned char **, unsigned *, unsigned *); } 
#line 321 "C:\\Documents and Settings\\All Users\\Application Data\\NVIDIA Corporation\\NVIDIA GPU Computing SDK\\C\\common\\inc\\cutil.h"
extern "C" { extern __declspec( dllimport ) CUTBoolean __stdcall cutLoadPGMi(const char *, unsigned **, unsigned *, unsigned *); } 
#line 337 "C:\\Documents and Settings\\All Users\\Application Data\\NVIDIA Corporation\\NVIDIA GPU Computing SDK\\C\\common\\inc\\cutil.h"
extern "C" { extern __declspec( dllimport ) CUTBoolean __stdcall cutLoadPGMs(const char *, unsigned short **, unsigned *, unsigned *); } 
#line 352 "C:\\Documents and Settings\\All Users\\Application Data\\NVIDIA Corporation\\NVIDIA GPU Computing SDK\\C\\common\\inc\\cutil.h"
extern "C" { extern __declspec( dllimport ) CUTBoolean __stdcall cutLoadPGMf(const char *, float **, unsigned *, unsigned *); } 
#line 364 "C:\\Documents and Settings\\All Users\\Application Data\\NVIDIA Corporation\\NVIDIA GPU Computing SDK\\C\\common\\inc\\cutil.h"
extern "C" { extern __declspec( dllimport ) CUTBoolean __stdcall cutSavePGMub(const char *, unsigned char *, unsigned, unsigned); } 
#line 376 "C:\\Documents and Settings\\All Users\\Application Data\\NVIDIA Corporation\\NVIDIA GPU Computing SDK\\C\\common\\inc\\cutil.h"
extern "C" { extern __declspec( dllimport ) CUTBoolean __stdcall cutSavePPMub(const char *, unsigned char *, unsigned, unsigned); } 
#line 389 "C:\\Documents and Settings\\All Users\\Application Data\\NVIDIA Corporation\\NVIDIA GPU Computing SDK\\C\\common\\inc\\cutil.h"
extern "C" { extern __declspec( dllimport ) CUTBoolean __stdcall cutSavePPM4ub(const char *, unsigned char *, unsigned, unsigned); } 
#line 401 "C:\\Documents and Settings\\All Users\\Application Data\\NVIDIA Corporation\\NVIDIA GPU Computing SDK\\C\\common\\inc\\cutil.h"
extern "C" { extern __declspec( dllimport ) CUTBoolean __stdcall cutSavePGMi(const char *, unsigned *, unsigned, unsigned); } 
#line 413 "C:\\Documents and Settings\\All Users\\Application Data\\NVIDIA Corporation\\NVIDIA GPU Computing SDK\\C\\common\\inc\\cutil.h"
extern "C" { extern __declspec( dllimport ) CUTBoolean __stdcall cutSavePGMs(const char *, unsigned short *, unsigned, unsigned); } 
#line 425 "C:\\Documents and Settings\\All Users\\Application Data\\NVIDIA Corporation\\NVIDIA GPU Computing SDK\\C\\common\\inc\\cutil.h"
extern "C" { extern __declspec( dllimport ) CUTBoolean __stdcall cutSavePGMf(const char *, float *, unsigned, unsigned); } 
#line 446 "C:\\Documents and Settings\\All Users\\Application Data\\NVIDIA Corporation\\NVIDIA GPU Computing SDK\\C\\common\\inc\\cutil.h"
extern "C" { extern __declspec( dllimport ) CUTBoolean __stdcall cutCheckCmdLineFlag(const int, const char **, const char *); } 
#line 460 "C:\\Documents and Settings\\All Users\\Application Data\\NVIDIA Corporation\\NVIDIA GPU Computing SDK\\C\\common\\inc\\cutil.h"
extern "C" { extern __declspec( dllimport ) CUTBoolean __stdcall cutGetCmdLineArgumenti(const int, const char **, const char *, int *); } 
#line 474 "C:\\Documents and Settings\\All Users\\Application Data\\NVIDIA Corporation\\NVIDIA GPU Computing SDK\\C\\common\\inc\\cutil.h"
extern "C" { extern __declspec( dllimport ) CUTBoolean __stdcall cutGetCmdLineArgumentf(const int, const char **, const char *, float *); } 
#line 488 "C:\\Documents and Settings\\All Users\\Application Data\\NVIDIA Corporation\\NVIDIA GPU Computing SDK\\C\\common\\inc\\cutil.h"
extern "C" { extern __declspec( dllimport ) CUTBoolean __stdcall cutGetCmdLineArgumentstr(const int, const char **, const char *, char **); } 
#line 503 "C:\\Documents and Settings\\All Users\\Application Data\\NVIDIA Corporation\\NVIDIA GPU Computing SDK\\C\\common\\inc\\cutil.h"
extern "C" { extern __declspec( dllimport ) CUTBoolean __stdcall cutGetCmdLineArgumentListstr(const int, const char **, const char *, char **, unsigned *); } 
#line 517 "C:\\Documents and Settings\\All Users\\Application Data\\NVIDIA Corporation\\NVIDIA GPU Computing SDK\\C\\common\\inc\\cutil.h"
extern "C" { extern __declspec( dllimport ) CUTBoolean __stdcall cutCheckCondition(int, const char *, const int); } 
#line 529 "C:\\Documents and Settings\\All Users\\Application Data\\NVIDIA Corporation\\NVIDIA GPU Computing SDK\\C\\common\\inc\\cutil.h"
extern "C" { extern __declspec( dllimport ) CUTBoolean __stdcall cutComparef(const float *, const float *, const unsigned); } 
#line 542 "C:\\Documents and Settings\\All Users\\Application Data\\NVIDIA Corporation\\NVIDIA GPU Computing SDK\\C\\common\\inc\\cutil.h"
extern "C" { extern __declspec( dllimport ) CUTBoolean __stdcall cutComparei(const int *, const int *, const unsigned); } 
#line 556 "C:\\Documents and Settings\\All Users\\Application Data\\NVIDIA Corporation\\NVIDIA GPU Computing SDK\\C\\common\\inc\\cutil.h"
extern "C" { extern __declspec( dllimport ) CUTBoolean __stdcall cutCompareuit(const unsigned *, const unsigned *, const unsigned, const float, const float); } 
#line 569 "C:\\Documents and Settings\\All Users\\Application Data\\NVIDIA Corporation\\NVIDIA GPU Computing SDK\\C\\common\\inc\\cutil.h"
extern "C" { extern __declspec( dllimport ) CUTBoolean __stdcall cutCompareub(const unsigned char *, const unsigned char *, const unsigned); } 
#line 584 "C:\\Documents and Settings\\All Users\\Application Data\\NVIDIA Corporation\\NVIDIA GPU Computing SDK\\C\\common\\inc\\cutil.h"
extern "C" { extern __declspec( dllimport ) CUTBoolean __stdcall cutCompareubt(const unsigned char *, const unsigned char *, const unsigned, const float, const float); } 
#line 598 "C:\\Documents and Settings\\All Users\\Application Data\\NVIDIA Corporation\\NVIDIA GPU Computing SDK\\C\\common\\inc\\cutil.h"
extern "C" { extern __declspec( dllimport ) CUTBoolean __stdcall cutCompareube(const unsigned char *, const unsigned char *, const unsigned, const float); } 
#line 612 "C:\\Documents and Settings\\All Users\\Application Data\\NVIDIA Corporation\\NVIDIA GPU Computing SDK\\C\\common\\inc\\cutil.h"
extern "C" { extern __declspec( dllimport ) CUTBoolean __stdcall cutComparefe(const float *, const float *, const unsigned, const float); } 
#line 627 "C:\\Documents and Settings\\All Users\\Application Data\\NVIDIA Corporation\\NVIDIA GPU Computing SDK\\C\\common\\inc\\cutil.h"
extern "C" { extern __declspec( dllimport ) CUTBoolean __stdcall cutComparefet(const float *, const float *, const unsigned, const float, const float); } 
#line 642 "C:\\Documents and Settings\\All Users\\Application Data\\NVIDIA Corporation\\NVIDIA GPU Computing SDK\\C\\common\\inc\\cutil.h"
extern "C" { extern __declspec( dllimport ) CUTBoolean __stdcall cutCompareL2fe(const float *, const float *, const unsigned, const float); } 
#line 657 "C:\\Documents and Settings\\All Users\\Application Data\\NVIDIA Corporation\\NVIDIA GPU Computing SDK\\C\\common\\inc\\cutil.h"
extern "C" { extern __declspec( dllimport ) CUTBoolean __stdcall cutComparePPM(const char *, const char *, const float, const float, bool = false); } 
#line 670 "C:\\Documents and Settings\\All Users\\Application Data\\NVIDIA Corporation\\NVIDIA GPU Computing SDK\\C\\common\\inc\\cutil.h"
extern "C" { extern __declspec( dllimport ) CUTBoolean __stdcall cutCreateTimer(unsigned *); } 
#line 679 "C:\\Documents and Settings\\All Users\\Application Data\\NVIDIA Corporation\\NVIDIA GPU Computing SDK\\C\\common\\inc\\cutil.h"
extern "C" { extern __declspec( dllimport ) CUTBoolean __stdcall cutDeleteTimer(unsigned); } 
#line 687 "C:\\Documents and Settings\\All Users\\Application Data\\NVIDIA Corporation\\NVIDIA GPU Computing SDK\\C\\common\\inc\\cutil.h"
extern "C" { extern __declspec( dllimport ) CUTBoolean __stdcall cutStartTimer(const unsigned); } 
#line 695 "C:\\Documents and Settings\\All Users\\Application Data\\NVIDIA Corporation\\NVIDIA GPU Computing SDK\\C\\common\\inc\\cutil.h"
extern "C" { extern __declspec( dllimport ) CUTBoolean __stdcall cutStopTimer(const unsigned); } 
#line 703 "C:\\Documents and Settings\\All Users\\Application Data\\NVIDIA Corporation\\NVIDIA GPU Computing SDK\\C\\common\\inc\\cutil.h"
extern "C" { extern __declspec( dllimport ) CUTBoolean __stdcall cutResetTimer(const unsigned); } 
#line 712 "C:\\Documents and Settings\\All Users\\Application Data\\NVIDIA Corporation\\NVIDIA GPU Computing SDK\\C\\common\\inc\\cutil.h"
extern "C" { extern __declspec( dllimport ) float __stdcall cutGetTimerValue(const unsigned); } 
#line 723 "C:\\Documents and Settings\\All Users\\Application Data\\NVIDIA Corporation\\NVIDIA GPU Computing SDK\\C\\common\\inc\\cutil.h"
extern "C" { extern __declspec( dllimport ) float __stdcall cutGetAverageTimerValue(const unsigned); } 
#line 146 "E:\\mylib\\include\\cuda_macro.h"
template<class T> 
struct CBuffer2D { 

size_t uWidth; 
size_t uHeight; 

size_t uWidthInBytes; 
size_t uPitch; 
size_t uPitchInWords; 

size_t uSize; 

T *pDataOnDevice; 

CBuffer2D() 
{ 
(this->uWidth) = ((this->uHeight) = ((this->uWidthInBytes) = ((this->uPitch) = ((this->uPitchInWords) = (0))))); 
(this->pDataOnDevice) = 0; 
} 

void *PAlloc(size_t w, size_t h) 
{ 
(this->uWidth) = w; 
(this->uHeight) = h; 
(this->uWidthInBytes) = this->uWidth * sizeof((*(this->pDataOnDevice))); 
{ auto cudaError err = cudaMallocPitch((void **)(&(this->pDataOnDevice)), &(this->uPitch), this->uWidthInBytes, this->uHeight); if ((cudaSuccess) != err) { fprintf(__iob_func() + 2, "Cuda error in file \'%s\' in line %i : %s.\n", "E:\\mylib\\include\\cuda_macro.h", 174, cudaGetErrorString(err)); exit(1); }  } ; 
#line 175 "E:\\mylib\\include\\cuda_macro.h"
(this->uPitchInWords) = this->uPitch / sizeof((*(this->pDataOnDevice))); 
(this->uSize) = this->uPitch * this->uHeight; 

return this->pDataOnDevice; 
} 
#line 187 "E:\\mylib\\include\\cuda_macro.h"
}; 
#line 28 "C:\\Program Files\\Microsoft Visual Studio 8\\VC\\INCLUDE\\assert.h"
extern "C" { extern __declspec( dllimport ) void __cdecl _wassert(const __wchar_t *, const __wchar_t *, unsigned); } 
#line 18 "E:\\mylib\\include\\libbuf.h"
template<class t> struct TBuffer { 

int num; 
t *data; 

TBuffer() 
{ 
(this->num) = 0; 
(this->data) = 0; 
} 


bool BIsAllocated() { return (0 != (this->data)) ? true : false; } 
unsigned USize() { return (unsigned)(this->num); } 



void free() 
{ 
if (this->data) { 
delete [] (this->data); }  
(this->data) = 0; 
} 


~TBuffer() 
{ 
#line 52 "E:\\mylib\\include\\libbuf.h"
this->free(); 
#line 55 "E:\\mylib\\include\\libbuf.h"
} 

t *alloc(int n) 
{ 
(void)((!(!(n > 0))) || (_wassert(L"\x6e\x20\x3e\x20\x30", L"\x45\x3a\x5c\x6d\x79\x6c\x69\x62\x5c\x69\x6e\x63\x6c\x75\x64\x65\x5c\x6c\x69\x62\x62\x75\x66\x2e\x68", 59), 0)); 
(this->num) = n; 


this->free(); 



(this->data) = (new (t [this->num])); 
(void)((!(!(this->data))) || (_wassert(L"\x64\x61\x74\x61", L"\x45\x3a\x5c\x6d\x79\x6c\x69\x62\x5c\x69\x6e\x63\x6c\x75\x64\x65\x5c\x6c\x69\x62\x62\x75\x66\x2e\x68", 68), 0)); 
memset(this->data, 0, sizeof(t) * this->num); 
return this->data; 
} 

t &operator[](int index) 
{ 
(void)((!(!(index < (this->num)))) || (_wassert(L"\x69\x6e\x64\x65\x78\x20\x3c\x20\x6e\x75\x6d", L"\x45\x3a\x5c\x6d\x79\x6c\x69\x62\x5c\x69\x6e\x63\x6c\x75\x64\x65\x5c\x6c\x69\x62\x62\x75\x66\x2e\x68", 75), 0)); 
return (this->data)[index]; 
} 

const t &operator[](int index) const 
{ 
(void)((!(!(index < (this->num)))) || (_wassert(L"\x69\x6e\x64\x65\x78\x20\x3c\x20\x6e\x75\x6d", L"\x45\x3a\x5c\x6d\x79\x6c\x69\x62\x5c\x69\x6e\x63\x6c\x75\x64\x65\x5c\x6c\x69\x62\x62\x75\x66\x2e\x68", 81), 0)); 
return (this->data)[index]; 
} 
}; 
#line 32 "e:/project/FlowEntropy/FlowFusion/FlowFusion_test/FlowFusion.cu"
static cudaExtent cVolumeExtent; 
static float4 *pf4Volume_host; 
static cudaPitchedPtr pcVolumePtrs_global[2]; 
static   texture< float4, 2, cudaReadModeElementType>  t2dSrc; 


static cudaExtent cVolumeExtent_array; 
static float4 *pf4WeightOffsetVolume_host; 
static cudaArray *cWeightOffsetVolume_array = (0); 
static   texture< float4, 3, cudaReadModeElementType>  t3dWeightOffset; 
static cudaChannelFormatDesc cWeightOffsetChannelDesc = cudaCreateChannelDesc< float4> (); 
#line 51 "e:/project/FlowEntropy/FlowFusion/FlowFusion_test/FlowFusion.cu"
static cudaExtent cErrorVolumeExtent; 
static cudaPitchedPtr cErrorVolume_device; 
static cudaPitchedPtr cErrorSum_device; 
#line 60 "e:/project/FlowEntropy/FlowFusion/FlowFusion_test/FlowFusion.cu"
static void _FlowFusion2D_kernel__entry(float 


fAttenuation, int 

iVolumeWidth, int 
iVolumeHeight, cudaPitchedPtr 

cDstPitchedPtr, cudaPitchedPtr 


cErrorPitchedPtr);
































#line 105 "e:/project/FlowEntropy/FlowFusion/FlowFusion_test/FlowFusion.cu"
#line 120 "e:/project/FlowEntropy/FlowFusion/FlowFusion_test/FlowFusion.cu"
static void _FlowFusion_kernel__entry(float 


fAttenuation, int 

iVolumeWidth, int 
iVolumeHeight, int 
iVolumeDepth, int 
iZ, int 
#line 134 "e:/project/FlowEntropy/FlowFusion/FlowFusion_test/FlowFusion.cu"
iNrOfYBlocks, int 
iBlockZSize, cudaPitchedPtr 
#line 142 "e:/project/FlowEntropy/FlowFusion/FlowFusion_test/FlowFusion.cu"
cDstPitchedPtr, cudaPitchedPtr 


cErrorPitchedPtr);





































































#line 216 "e:/project/FlowEntropy/FlowFusion/FlowFusion_test/FlowFusion.cu"
#line 359 "e:/project/FlowEntropy/FlowFusion/FlowFusion_test/FlowFusion.cu"
extern "C" { void _FlowFusionFree() 
{ 
{ if (pf4Volume_host) { cudaFreeHost(pf4Volume_host); pf4Volume_host = (0); }  } ; 
for (int i = 0; i < 2; i++) 
{ if (((pcVolumePtrs_global)[i]).ptr) { cudaFree(((pcVolumePtrs_global)[i]).ptr); (((pcVolumePtrs_global)[i]).ptr) = (0); }  }  ; 
{ if (cWeightOffsetVolume_array) { cudaFreeArray(cWeightOffsetVolume_array); cWeightOffsetVolume_array = (0); }  } ; 
{ if (pf4WeightOffsetVolume_host) { cudaFreeHost(pf4WeightOffsetVolume_host); pf4WeightOffsetVolume_host = (0); }  } ; 

{ if (cErrorVolume_device.ptr) { cudaFree(cErrorVolume_device.ptr); (cErrorVolume_device.ptr) = (0); }  } ; 
{ if (cErrorSum_device.ptr) { cudaFree(cErrorSum_device.ptr); (cErrorSum_device.ptr) = (0); }  } ; 
#line 374 "e:/project/FlowEntropy/FlowFusion/FlowFusion_test/FlowFusion.cu"
} } 


extern "C" { void _FlowFusionInit(int 
iVolumeWidth, int 
iVolumeHeight, int 
iVolumeDepth) 

{ 
{ static unsigned _uTimer = (0); static float _pfTimers[64]; auto size_t _t = (0); if (1) { if (!(_uTimer)) { if ((CUTTrue) != (cutCreateTimer(&_uTimer))) { fprintf(__iob_func() + 2, "Cut error in file \'%s\' in line %i.\n", "e:/project/FlowEntropy/FlowFusion/FlowFusion_test/FlowFusion.cu", 383); exit(1); }  }  ; printf("%s", "_FlowFusionInit(): "); }  ; 

auto int iNrOfVoxels = ((iVolumeWidth * iVolumeHeight) * iVolumeDepth); 
cVolumeExtent = make_cudaExtent(iVolumeWidth * sizeof(float4), iVolumeHeight, iVolumeDepth); 




if (1) { if ((CUTTrue) != (cutStartTimer(_uTimer))) { fprintf(__iob_func() + 2, "Cut error in file \'%s\' in line %i.\n", "e:/project/FlowEntropy/FlowFusion/FlowFusion_test/FlowFusion.cu", 391); exit(1); }  ; if ((CUTTrue) != (cutResetTimer(_uTimer))) { fprintf(__iob_func() + 2, "Cut error in file \'%s\' in line %i.\n", "e:/project/FlowEntropy/FlowFusion/FlowFusion_test/FlowFusion.cu", 391); exit(1); }  ; }  ; 
{ auto cudaError err = cudaMallocHost((void **)(&pf4Volume_host), sizeof(pf4Volume_host[0]) * iNrOfVoxels); if ((cudaSuccess) != err) { fprintf(__iob_func() + 2, "Cuda error in file \'%s\' in line %i : %s.\n", "e:/project/FlowEntropy/FlowFusion/FlowFusion_test/FlowFusion.cu", 395, cudaGetErrorString(err)); exit(1); }  } ; ; 
#line 396 "e:/project/FlowEntropy/FlowFusion/FlowFusion_test/FlowFusion.cu"
if (1) { if (true) { auto cudaError err = cudaThreadSynchronize(); if ((cudaSuccess) != err) { fprintf(__iob_func() + 2, "Cuda error in file \'%s\' in line %i : %s.\n", "e:/project/FlowEntropy/FlowFusion/FlowFusion_test/FlowFusion.cu", 396, cudaGetErrorString(err)); exit(1); }  }  ; ; if ((CUTTrue) != (cutStopTimer(_uTimer))) { fprintf(__iob_func() + 2, "Cut error in file \'%s\' in line %i.\n", "e:/project/FlowEntropy/FlowFusion/FlowFusion_test/FlowFusion.cu", 396); exit(1); }  ; ((_pfTimers)[_t++]) = cutGetTimerValue(_uTimer); }  ; 




if (1) { if ((CUTTrue) != (cutStartTimer(_uTimer))) { fprintf(__iob_func() + 2, "Cut error in file \'%s\' in line %i.\n", "e:/project/FlowEntropy/FlowFusion/FlowFusion_test/FlowFusion.cu", 401); exit(1); }  ; if ((CUTTrue) != (cutResetTimer(_uTimer))) { fprintf(__iob_func() + 2, "Cut error in file \'%s\' in line %i.\n", "e:/project/FlowEntropy/FlowFusion/FlowFusion_test/FlowFusion.cu", 401); exit(1); }  ; }  ; 

for (int i = 0; i < 2; i++) 
{ 
{ auto cudaError err = cudaMalloc3D((pcVolumePtrs_global) + i, cVolumeExtent); if ((cudaSuccess) != err) { fprintf(__iob_func() + 2, "Cuda error in file \'%s\' in line %i : %s.\n", "e:/project/FlowEntropy/FlowFusion/FlowFusion_test/FlowFusion.cu", 406, cudaGetErrorString(err)); exit(1); }  } ; ; 
#line 407 "e:/project/FlowEntropy/FlowFusion/FlowFusion_test/FlowFusion.cu"
}  


cErrorVolumeExtent = make_cudaExtent(iVolumeWidth, iVolumeHeight, iVolumeDepth); 



{ auto cudaError err = cudaMalloc3D(&cErrorVolume_device, cErrorVolumeExtent); if ((cudaSuccess) != err) { fprintf(__iob_func() + 2, "Cuda error in file \'%s\' in line %i : %s.\n", "e:/project/FlowEntropy/FlowFusion/FlowFusion_test/FlowFusion.cu", 415, cudaGetErrorString(err)); exit(1); }  } ; ; 
#line 416 "e:/project/FlowEntropy/FlowFusion/FlowFusion_test/FlowFusion.cu"
{ auto cudaError err = cudaMalloc3D(&cErrorSum_device, cErrorVolumeExtent); if ((cudaSuccess) != err) { fprintf(__iob_func() + 2, "Cuda error in file \'%s\' in line %i : %s.\n", "e:/project/FlowEntropy/FlowFusion/FlowFusion_test/FlowFusion.cu", 417, cudaGetErrorString(err)); exit(1); }  } ; ; 
#line 431 "e:/project/FlowEntropy/FlowFusion/FlowFusion_test/FlowFusion.cu"
cVolumeExtent_array = make_cudaExtent(iVolumeWidth, iVolumeHeight, iVolumeDepth); 




{ auto cudaError err = cudaMalloc3DArray(&cWeightOffsetVolume_array, &cWeightOffsetChannelDesc, cVolumeExtent_array); if ((cudaSuccess) != err) { fprintf(__iob_func() + 2, "Cuda error in file \'%s\' in line %i : %s.\n", "e:/project/FlowEntropy/FlowFusion/FlowFusion_test/FlowFusion.cu", 437, cudaGetErrorString(err)); exit(1); }  } ; ; 
#line 439 "e:/project/FlowEntropy/FlowFusion/FlowFusion_test/FlowFusion.cu"
{ auto cudaError err = cudaMallocHost((void **)(&pf4WeightOffsetVolume_host), sizeof(pf4WeightOffsetVolume_host[0]) * iNrOfVoxels); if ((cudaSuccess) != err) { fprintf(__iob_func() + 2, "Cuda error in file \'%s\' in line %i : %s.\n", "e:/project/FlowEntropy/FlowFusion/FlowFusion_test/FlowFusion.cu", 442, cudaGetErrorString(err)); exit(1); }  } ; ; 
#line 445 "e:/project/FlowEntropy/FlowFusion/FlowFusion_test/FlowFusion.cu"
if (1) { if (true) { auto cudaError err = cudaThreadSynchronize(); if ((cudaSuccess) != err) { fprintf(__iob_func() + 2, "Cuda error in file \'%s\' in line %i : %s.\n", "e:/project/FlowEntropy/FlowFusion/FlowFusion_test/FlowFusion.cu", 445, cudaGetErrorString(err)); exit(1); }  }  ; ; if ((CUTTrue) != (cutStopTimer(_uTimer))) { fprintf(__iob_func() + 2, "Cut error in file \'%s\' in line %i.\n", "e:/project/FlowEntropy/FlowFusion/FlowFusion_test/FlowFusion.cu", 445); exit(1); }  ; ((_pfTimers)[_t++]) = cutGetTimerValue(_uTimer); }  ; 

if (1) { auto float fTotalTime = (0); for (size_t i = (0); i < _t; i++) { fTotalTime += (_pfTimers)[i]; printf("%.2f,", (_pfTimers)[i]); }  printf("%.2f\n", fTotalTime); }  } ; 
} } 


extern "C" { void _FlowFusion(float 
fAttenuation, int 
iNrOfIterations, int 
iVolumeWidth, int 
iVolumeHeight, int 
iVolumeDepth, int 

iBlockZSize, float *
pfWeightVolume, float *



pfXOffsetVolume, float *
pfYOffsetVolume, float *
pfZOffsetVolume, float *


pfSrcVolume, float *
pfDstVolume, int *

piFlagVolume) 


{ 
{ static unsigned _uTimer = (0); static float _pfTimers[64]; auto size_t _t = (0); if (1) { if (!(_uTimer)) { if ((CUTTrue) != (cutCreateTimer(&_uTimer))) { fprintf(__iob_func() + 2, "Cut error in file \'%s\' in line %i.\n", "e:/project/FlowEntropy/FlowFusion/FlowFusion_test/FlowFusion.cu", 475); exit(1); }  }  ; printf("%s", "_FlowFusion(): "); }  ; 
#line 486 "e:/project/FlowEntropy/FlowFusion/FlowFusion_test/FlowFusion.cu"
auto dim3 v3Blk = dim3(16, 12); 

auto dim3 v3Grid = dim3((unsigned)ceilf((float)iVolumeWidth / (float)(v3Blk.x)), (unsigned)ceilf((float)iVolumeHeight / (float)(v3Blk.y)) * (unsigned)ceilf((float)iVolumeDepth / (float)iBlockZSize)); 
#line 504 "e:/project/FlowEntropy/FlowFusion/FlowFusion_test/FlowFusion.cu"
if (1) { if ((CUTTrue) != (cutStartTimer(_uTimer))) { fprintf(__iob_func() + 2, "Cut error in file \'%s\' in line %i.\n", "e:/project/FlowEntropy/FlowFusion/FlowFusion_test/FlowFusion.cu", 504); exit(1); }  ; if ((CUTTrue) != (cutResetTimer(_uTimer))) { fprintf(__iob_func() + 2, "Cut error in file \'%s\' in line %i.\n", "e:/project/FlowEntropy/FlowFusion/FlowFusion_test/FlowFusion.cu", 504); exit(1); }  ; }  ; 
for (int v = 0, d = 0; d < iVolumeDepth; d++) { 
for (int h = 0; h < iVolumeHeight; h++) { 
for (int w = 0; w < iVolumeWidth; (w++), (v++)) 
{ 
((pf4Volume_host[v]).x) = pfSrcVolume[v * 3 + 0]; 
((pf4Volume_host[v]).y) = pfSrcVolume[v * 3 + 1]; 
((pf4Volume_host[v]).z) = pfSrcVolume[v * 3 + 2]; 


if (piFlagVolume) { 
((pf4Volume_host[v]).w) = (float)(piFlagVolume[v]); } else { 


((pf4Volume_host[v]).w) = (0.0F); }  
}  }  }  


for (int v = 0, d = 0; d < iVolumeDepth; d++) { 
for (int h = 0; h < iVolumeHeight; h++) { 
for (int w = 0; w < iVolumeWidth; (w++), (v++)) 
{ 
#line 529 "e:/project/FlowEntropy/FlowFusion/FlowFusion_test/FlowFusion.cu"
if (pfXOffsetVolume) { 
#line 531 "e:/project/FlowEntropy/FlowFusion/FlowFusion_test/FlowFusion.cu"
((pf4WeightOffsetVolume_host[v]).x) = pfXOffsetVolume[v]; }  

if (pfYOffsetVolume) { 
#line 535 "e:/project/FlowEntropy/FlowFusion/FlowFusion_test/FlowFusion.cu"
((pf4WeightOffsetVolume_host[v]).y) = pfYOffsetVolume[v]; }  

if (pfZOffsetVolume) { 
#line 539 "e:/project/FlowEntropy/FlowFusion/FlowFusion_test/FlowFusion.cu"
((pf4WeightOffsetVolume_host[v]).z) = pfZOffsetVolume[v]; }  
((pf4WeightOffsetVolume_host[v]).w) = (1.0F) - pfWeightVolume[v]; 
#line 542 "e:/project/FlowEntropy/FlowFusion/FlowFusion_test/FlowFusion.cu"
}  }  }  


if (1) { if (true) { auto cudaError err = cudaThreadSynchronize(); if ((cudaSuccess) != err) { fprintf(__iob_func() + 2, "Cuda error in file \'%s\' in line %i : %s.\n", "e:/project/FlowEntropy/FlowFusion/FlowFusion_test/FlowFusion.cu", 545, cudaGetErrorString(err)); exit(1); }  }  ; ; if ((CUTTrue) != (cutStopTimer(_uTimer))) { fprintf(__iob_func() + 2, "Cut error in file \'%s\' in line %i.\n", "e:/project/FlowEntropy/FlowFusion/FlowFusion_test/FlowFusion.cu", 545); exit(1); }  ; ((_pfTimers)[_t++]) = cutGetTimerValue(_uTimer); }  ; 


if (1) { if ((CUTTrue) != (cutStartTimer(_uTimer))) { fprintf(__iob_func() + 2, "Cut error in file \'%s\' in line %i.\n", "e:/project/FlowEntropy/FlowFusion/FlowFusion_test/FlowFusion.cu", 548); exit(1); }  ; if ((CUTTrue) != (cutResetTimer(_uTimer))) { fprintf(__iob_func() + 2, "Cut error in file \'%s\' in line %i.\n", "e:/project/FlowEntropy/FlowFusion/FlowFusion_test/FlowFusion.cu", 548); exit(1); }  ; }  ; 

auto cudaMemcpy3DParms cCopyParamsHostToDevice = {(0)}; 

(cCopyParamsHostToDevice.srcPtr) = make_cudaPitchedPtr((void *)pf4Volume_host, cVolumeExtent.width, iVolumeWidth * sizeof(float4), iVolumeHeight); 




(cCopyParamsHostToDevice.dstPtr) = (pcVolumePtrs_global)[0]; 
(cCopyParamsHostToDevice.extent) = cVolumeExtent; 
(cCopyParamsHostToDevice.kind) = cudaMemcpyHostToDevice; 
{ auto cudaError err = cudaMemcpy3D(&cCopyParamsHostToDevice); if ((cudaSuccess) != err) { fprintf(__iob_func() + 2, "Cuda error in file \'%s\' in line %i : %s.\n", "e:/project/FlowEntropy/FlowFusion/FlowFusion_test/FlowFusion.cu", 560, cudaGetErrorString(err)); exit(1); }  } ; ; 


auto cudaMemcpy3DParms cCopyParamsHostToArray = {(0)}; 
(cCopyParamsHostToArray.srcPtr) = make_cudaPitchedPtr((void *)pf4WeightOffsetVolume_host, iVolumeWidth * sizeof(float4), iVolumeWidth, iVolumeHeight); 




(cCopyParamsHostToArray.dstArray) = cWeightOffsetVolume_array; 
(cCopyParamsHostToArray.extent) = cVolumeExtent_array; 
(cCopyParamsHostToArray.kind) = cudaMemcpyHostToDevice; 
{ auto cudaError err = cudaMemcpy3D(&cCopyParamsHostToArray); if ((cudaSuccess) != err) { fprintf(__iob_func() + 2, "Cuda error in file \'%s\' in line %i : %s.\n", "e:/project/FlowEntropy/FlowFusion/FlowFusion_test/FlowFusion.cu", 573, cudaGetErrorString(err)); exit(1); }  } ; ; 
#line 575 "e:/project/FlowEntropy/FlowFusion/FlowFusion_test/FlowFusion.cu"
((t3dWeightOffset.addressMode)[0]) = cudaAddressModeClamp; 
((t3dWeightOffset.addressMode)[1]) = cudaAddressModeClamp; 
((t3dWeightOffset.addressMode)[2]) = cudaAddressModeClamp; 
(t3dWeightOffset.filterMode) = cudaFilterModePoint; 
(t3dWeightOffset.normalized) = (false); 


{ auto cudaError err = cudaBindTextureToArray(t3dWeightOffset, cWeightOffsetVolume_array, cWeightOffsetChannelDesc); if ((cudaSuccess) != err) { fprintf(__iob_func() + 2, "Cuda error in file \'%s\' in line %i : %s.\n", "e:/project/FlowEntropy/FlowFusion/FlowFusion_test/FlowFusion.cu", 583, cudaGetErrorString(err)); exit(1); }  } ; ; 
#line 586 "e:/project/FlowEntropy/FlowFusion/FlowFusion_test/FlowFusion.cu"
if (1) { if (true) { auto cudaError err = cudaThreadSynchronize(); if ((cudaSuccess) != err) { fprintf(__iob_func() + 2, "Cuda error in file \'%s\' in line %i : %s.\n", "e:/project/FlowEntropy/FlowFusion/FlowFusion_test/FlowFusion.cu", 586, cudaGetErrorString(err)); exit(1); }  }  ; ; if ((CUTTrue) != (cutStopTimer(_uTimer))) { fprintf(__iob_func() + 2, "Cut error in file \'%s\' in line %i.\n", "e:/project/FlowEntropy/FlowFusion/FlowFusion_test/FlowFusion.cu", 586); exit(1); }  ; ((_pfTimers)[_t++]) = cutGetTimerValue(_uTimer); }  ; 


if (1) { if ((CUTTrue) != (cutStartTimer(_uTimer))) { fprintf(__iob_func() + 2, "Cut error in file \'%s\' in line %i.\n", "e:/project/FlowEntropy/FlowFusion/FlowFusion_test/FlowFusion.cu", 589); exit(1); }  ; if ((CUTTrue) != (cutResetTimer(_uTimer))) { fprintf(__iob_func() + 2, "Cut error in file \'%s\' in line %i.\n", "e:/project/FlowEntropy/FlowFusion/FlowFusion_test/FlowFusion.cu", 589); exit(1); }  ; }  ; 

((t2dSrc.addressMode)[0]) = cudaAddressModeClamp; 
((t2dSrc.addressMode)[1]) = cudaAddressModeClamp; 
(t2dSrc.filterMode) = cudaFilterModePoint; 
(t2dSrc.normalized) = (false); 

auto int iSrc = 0; 
for (int i = 0; i < iNrOfIterations; (i++), (iSrc = 1 - iSrc)) 


{ 


auto cudaChannelFormatDesc cChannelDesc = cudaCreateChannelDesc< float4> (); 


{ auto cudaError err = cudaBindTexture2D(0, t2dSrc, ((pcVolumePtrs_global)[iSrc]).ptr, cChannelDesc, iVolumeWidth, iVolumeHeight * iVolumeDepth, ((pcVolumePtrs_global)[iSrc]).pitch); if ((cudaSuccess) != err) { fprintf(__iob_func() + 2, "Cuda error in file \'%s\' in line %i : %s.\n", "e:/project/FlowEntropy/FlowFusion/FlowFusion_test/FlowFusion.cu", 614, cudaGetErrorString(err)); exit(1); }  } ; 
#line 617 "e:/project/FlowEntropy/FlowFusion/FlowFusion_test/FlowFusion.cu"
if (iVolumeDepth <= 2) { 
cudaConfigureCall(v3Grid, v3Blk, 0) ? ((void)0) : _FlowFusion2D_kernel__entry(fAttenuation, iVolumeWidth, iVolumeHeight, (pcVolumePtrs_global)[1 - iSrc], cErrorVolume_device); } else 
#line 627 "e:/project/FlowEntropy/FlowFusion/FlowFusion_test/FlowFusion.cu"
{ 
#line 659 "e:/project/FlowEntropy/FlowFusion/FlowFusion_test/FlowFusion.cu"
cudaConfigureCall(v3Grid, v3Blk, 0) ? ((void)0) : _FlowFusion_kernel__entry(fAttenuation, iVolumeWidth, iVolumeHeight, iVolumeDepth, 0, (int)ceilf((float)iVolumeHeight / ((float)12)), iBlockZSize, (pcVolumePtrs_global)[1 - iSrc], cErrorVolume_device); 
#line 678 "e:/project/FlowEntropy/FlowFusion/FlowFusion_test/FlowFusion.cu"
}  
#line 680 "e:/project/FlowEntropy/FlowFusion/FlowFusion_test/FlowFusion.cu"
{ auto cudaError_t err = cudaGetLastError(); if ((cudaSuccess) != err) { fprintf(__iob_func() + 2, "Cuda error: %s in file \'%s\' in line %i : %s.\n", "_FlowFusion_kernel() failed", "e:/project/FlowEntropy/FlowFusion/FlowFusion_test/FlowFusion.cu", 680, cudaGetErrorString(err)); exit(1); }  err = cudaThreadSynchronize(); if ((cudaSuccess) != err) { fprintf(__iob_func() + 2, "Cuda error: %s in file \'%s\' in line %i : %s.\n", "_FlowFusion_kernel() failed", "e:/project/FlowEntropy/FlowFusion/FlowFusion_test/FlowFusion.cu", 680, cudaGetErrorString(err)); exit(1); }  } ; 
#line 708 "e:/project/FlowEntropy/FlowFusion/FlowFusion_test/FlowFusion.cu"
}  
if (1) { if (true) { auto cudaError err = cudaThreadSynchronize(); if ((cudaSuccess) != err) { fprintf(__iob_func() + 2, "Cuda error in file \'%s\' in line %i : %s.\n", "e:/project/FlowEntropy/FlowFusion/FlowFusion_test/FlowFusion.cu", 709, cudaGetErrorString(err)); exit(1); }  }  ; ; if ((CUTTrue) != (cutStopTimer(_uTimer))) { fprintf(__iob_func() + 2, "Cut error in file \'%s\' in line %i.\n", "e:/project/FlowEntropy/FlowFusion/FlowFusion_test/FlowFusion.cu", 709); exit(1); }  ; ((_pfTimers)[_t++]) = cutGetTimerValue(_uTimer); }  ; 

auto int iDst = iSrc; 


if (1) { if ((CUTTrue) != (cutStartTimer(_uTimer))) { fprintf(__iob_func() + 2, "Cut error in file \'%s\' in line %i.\n", "e:/project/FlowEntropy/FlowFusion/FlowFusion_test/FlowFusion.cu", 714); exit(1); }  ; if ((CUTTrue) != (cutResetTimer(_uTimer))) { fprintf(__iob_func() + 2, "Cut error in file \'%s\' in line %i.\n", "e:/project/FlowEntropy/FlowFusion/FlowFusion_test/FlowFusion.cu", 714); exit(1); }  ; }  ; 

auto cudaMemcpy3DParms cCopyParamsDeviceToHost = {(0)}; 
(cCopyParamsDeviceToHost.srcPtr) = (pcVolumePtrs_global)[iDst]; 
(cCopyParamsDeviceToHost.dstPtr) = make_cudaPitchedPtr((void *)pf4Volume_host, cVolumeExtent.width, iVolumeWidth * sizeof(float4), iVolumeHeight); 




(cCopyParamsDeviceToHost.extent) = cVolumeExtent; 
(cCopyParamsDeviceToHost.kind) = cudaMemcpyDeviceToHost; 

{ auto cudaError err = cudaMemcpy3D(&cCopyParamsDeviceToHost); if ((cudaSuccess) != err) { fprintf(__iob_func() + 2, "Cuda error in file \'%s\' in line %i : %s.\n", "e:/project/FlowEntropy/FlowFusion/FlowFusion_test/FlowFusion.cu", 726, cudaGetErrorString(err)); exit(1); }  } ; ; 

if (1) { if (true) { auto cudaError err = cudaThreadSynchronize(); if ((cudaSuccess) != err) { fprintf(__iob_func() + 2, "Cuda error in file \'%s\' in line %i : %s.\n", "e:/project/FlowEntropy/FlowFusion/FlowFusion_test/FlowFusion.cu", 728, cudaGetErrorString(err)); exit(1); }  }  ; ; if ((CUTTrue) != (cutStopTimer(_uTimer))) { fprintf(__iob_func() + 2, "Cut error in file \'%s\' in line %i.\n", "e:/project/FlowEntropy/FlowFusion/FlowFusion_test/FlowFusion.cu", 728); exit(1); }  ; ((_pfTimers)[_t++]) = cutGetTimerValue(_uTimer); }  ; 


if (1) { if ((CUTTrue) != (cutStartTimer(_uTimer))) { fprintf(__iob_func() + 2, "Cut error in file \'%s\' in line %i.\n", "e:/project/FlowEntropy/FlowFusion/FlowFusion_test/FlowFusion.cu", 731); exit(1); }  ; if ((CUTTrue) != (cutResetTimer(_uTimer))) { fprintf(__iob_func() + 2, "Cut error in file \'%s\' in line %i.\n", "e:/project/FlowEntropy/FlowFusion/FlowFusion_test/FlowFusion.cu", 731); exit(1); }  ; }  ; 
for (int v = 0, d = 0; d < iVolumeDepth; d++) { 
for (int h = 0; h < iVolumeHeight; h++) { 
for (int w = 0; w < iVolumeWidth; (w++), (v++)) 
{ 
(pfDstVolume[v * 3 + 0]) = (pf4Volume_host[v]).x; 
(pfDstVolume[v * 3 + 1]) = (pf4Volume_host[v]).y; 
(pfDstVolume[v * 3 + 2]) = (pf4Volume_host[v]).z; 
}  }  }  
if (1) { if (true) { auto cudaError err = cudaThreadSynchronize(); if ((cudaSuccess) != err) { fprintf(__iob_func() + 2, "Cuda error in file \'%s\' in line %i : %s.\n", "e:/project/FlowEntropy/FlowFusion/FlowFusion_test/FlowFusion.cu", 740, cudaGetErrorString(err)); exit(1); }  }  ; ; if ((CUTTrue) != (cutStopTimer(_uTimer))) { fprintf(__iob_func() + 2, "Cut error in file \'%s\' in line %i.\n", "e:/project/FlowEntropy/FlowFusion/FlowFusion_test/FlowFusion.cu", 740); exit(1); }  ; ((_pfTimers)[_t++]) = cutGetTimerValue(_uTimer); }  ; 

if (1) { auto float fTotalTime = (0); for (size_t i = (0); i < _t; i++) { fTotalTime += (_pfTimers)[i]; printf("%.2f,", (_pfTimers)[i]); }  printf("%.2f\n", fTotalTime); }  } ; 
} } 


extern "C" { void _FlowFusionCPU(float 
fAttenuation, int 
iNrOfIterations, int 
iVolumeWidth, int 
iVolumeHeight, int 
iVolumeDepth, float *

pfWeightVolume, float *
pfOffsetVolume, float *

pfSrcVolume, float *
pfDstVolume) 

{ 
{ static unsigned _uTimer = (0); static float _pfTimers[64]; auto size_t _t = (0); if (1) { if (!(_uTimer)) { if ((CUTTrue) != (cutCreateTimer(&_uTimer))) { fprintf(__iob_func() + 2, "Cut error in file \'%s\' in line %i.\n", "e:/project/FlowEntropy/FlowFusion/FlowFusion_test/FlowFusion.cu", 760); exit(1); }  }  ; printf("%s", "_FlowFusionCPU(): "); }  ; 

if (1) { if ((CUTTrue) != (cutStartTimer(_uTimer))) { fprintf(__iob_func() + 2, "Cut error in file \'%s\' in line %i.\n", "e:/project/FlowEntropy/FlowFusion/FlowFusion_test/FlowFusion.cu", 762); exit(1); }  ; if ((CUTTrue) != (cutResetTimer(_uTimer))) { fprintf(__iob_func() + 2, "Cut error in file \'%s\' in line %i.\n", "e:/project/FlowEntropy/FlowFusion/FlowFusion_test/FlowFusion.cu", 762); exit(1); }  ; }  ; 
auto int iNrOfVoxels = ((iVolumeWidth * iVolumeHeight) * iVolumeDepth); 

auto TBuffer< float>  ppfTempVolume[2]; 
for (int i = 0; i < 2; i++) { 
((ppfTempVolume)[i]).alloc(3 * iNrOfVoxels); }  
memcpy((&((ppfTempVolume)[0])[0]), pfSrcVolume + 0, sizeof(((ppfTempVolume)[0])[0]) * ((ppfTempVolume)[0]).USize()); 



if (1) { if (true) { auto cudaError err = cudaThreadSynchronize(); if ((cudaSuccess) != err) { fprintf(__iob_func() + 2, "Cuda error in file \'%s\' in line %i : %s.\n", "e:/project/FlowEntropy/FlowFusion/FlowFusion_test/FlowFusion.cu", 772, cudaGetErrorString(err)); exit(1); }  }  ; ; if ((CUTTrue) != (cutStopTimer(_uTimer))) { fprintf(__iob_func() + 2, "Cut error in file \'%s\' in line %i.\n", "e:/project/FlowEntropy/FlowFusion/FlowFusion_test/FlowFusion.cu", 772); exit(1); }  ; ((_pfTimers)[_t++]) = cutGetTimerValue(_uTimer); }  ; 


if (1) { if ((CUTTrue) != (cutStartTimer(_uTimer))) { fprintf(__iob_func() + 2, "Cut error in file \'%s\' in line %i.\n", "e:/project/FlowEntropy/FlowFusion/FlowFusion_test/FlowFusion.cu", 775); exit(1); }  ; if ((CUTTrue) != (cutResetTimer(_uTimer))) { fprintf(__iob_func() + 2, "Cut error in file \'%s\' in line %i.\n", "e:/project/FlowEntropy/FlowFusion/FlowFusion_test/FlowFusion.cu", 775); exit(1); }  ; }  ; 
auto int iSrc = 0; 
for (int i = 0; i < iNrOfIterations; (i++), (iSrc = 1 - iSrc)) 


{ 
auto int iDst = (1 - iSrc); 
for (int v = 0, d = 0; d < iVolumeDepth; d++) { 
for (int h = 0; h < iVolumeHeight; h++) { 
for (int w = 0; w < iVolumeWidth; (w++), (v++)) 
{ 
for (int c = 0; c < 3; c++) 
{ 
auto float fV = (((ppfTempVolume)[iSrc])[(c + 3 * v)]); 
#line 797 "e:/project/FlowEntropy/FlowFusion/FlowFusion_test/FlowFusion.cu"
auto float fPX = (((ppfTempVolume)[iSrc])[(c + 3 * (v + (((w + 1) < iVolumeWidth) ? 1 : 0)))]); 
auto float fNX = (((ppfTempVolume)[iSrc])[(c + 3 * (v - (((w - 1) >= 0) ? 1 : 0)))]); 
auto float fPY = (((ppfTempVolume)[iSrc])[(c + 3 * (v + (((h + 1) < iVolumeHeight) ? iVolumeWidth : 0)))]); 
auto float fNY = (((ppfTempVolume)[iSrc])[(c + 3 * (v - (((h - 1) >= 0) ? iVolumeWidth : 0)))]); 
auto float fPZ = (((ppfTempVolume)[iSrc])[(c + 3 * (v + (((d + 1) < iVolumeDepth) ? (iVolumeWidth * iVolumeHeight) : 0)))]); 
auto float fNZ = (((ppfTempVolume)[iSrc])[(c + 3 * (v - (((d - 1) >= 0) ? (iVolumeWidth * iVolumeHeight) : 0)))]); 
#line 804 "e:/project/FlowEntropy/FlowFusion/FlowFusion_test/FlowFusion.cu"
((ppfTempVolume)[iDst])[(c + 3 * v)] = (pfWeightVolume[v] * fV + (((((((fPX + fNX) + fPY) + fNY) + fPZ) + fNZ) - (6.0F) * fV) * fAttenuation) / (6.0F)) + pfOffsetVolume[c + 3 * v]; 
#line 810 "e:/project/FlowEntropy/FlowFusion/FlowFusion_test/FlowFusion.cu"
}  
}  }  }  
}  
if (1) { if (true) { auto cudaError err = cudaThreadSynchronize(); if ((cudaSuccess) != err) { fprintf(__iob_func() + 2, "Cuda error in file \'%s\' in line %i : %s.\n", "e:/project/FlowEntropy/FlowFusion/FlowFusion_test/FlowFusion.cu", 813, cudaGetErrorString(err)); exit(1); }  }  ; ; if ((CUTTrue) != (cutStopTimer(_uTimer))) { fprintf(__iob_func() + 2, "Cut error in file \'%s\' in line %i.\n", "e:/project/FlowEntropy/FlowFusion/FlowFusion_test/FlowFusion.cu", 813); exit(1); }  ; ((_pfTimers)[_t++]) = cutGetTimerValue(_uTimer); }  ; 

if (1) { if ((CUTTrue) != (cutStartTimer(_uTimer))) { fprintf(__iob_func() + 2, "Cut error in file \'%s\' in line %i.\n", "e:/project/FlowEntropy/FlowFusion/FlowFusion_test/FlowFusion.cu", 815); exit(1); }  ; if ((CUTTrue) != (cutResetTimer(_uTimer))) { fprintf(__iob_func() + 2, "Cut error in file \'%s\' in line %i.\n", "e:/project/FlowEntropy/FlowFusion/FlowFusion_test/FlowFusion.cu", 815); exit(1); }  ; }  ; 
auto int iDst = iSrc; 
memcpy(pfDstVolume + 0, (&((ppfTempVolume)[iDst])[0]), sizeof(((ppfTempVolume)[iDst])[0]) * ((ppfTempVolume)[iDst]).USize()); 



if (1) { if (true) { auto cudaError err = cudaThreadSynchronize(); if ((cudaSuccess) != err) { fprintf(__iob_func() + 2, "Cuda error in file \'%s\' in line %i : %s.\n", "e:/project/FlowEntropy/FlowFusion/FlowFusion_test/FlowFusion.cu", 821, cudaGetErrorString(err)); exit(1); }  }  ; ; if ((CUTTrue) != (cutStopTimer(_uTimer))) { fprintf(__iob_func() + 2, "Cut error in file \'%s\' in line %i.\n", "e:/project/FlowEntropy/FlowFusion/FlowFusion_test/FlowFusion.cu", 821); exit(1); }  ; ((_pfTimers)[_t++]) = cutGetTimerValue(_uTimer); }  ; 

if (1) { auto float fTotalTime = (0); for (size_t i = (0); i < _t; i++) { fTotalTime += (_pfTimers)[i]; printf("%.2f,", (_pfTimers)[i]); }  printf("%.2f\n", fTotalTime); }  } ; 
} } 

#line 1 "e:\\project\\flowentropy\\entropy_final_2d\\myinterface\\FlowFusion.cudafe1.stub.c"

extern "C" {
#line 4 "e:\\project\\flowentropy\\entropy_final_2d\\myinterface\\FlowFusion.cudafe1.stub.c"
#line 1 "e:\\project\\flowentropy\\entropy_final_2d\\myinterface\\FlowFusion.fatbin.c"
#line 1 "C:\\CUDA\\include\\__cudaFatFormat.h"


















































































extern "C" {
#line 85 "C:\\CUDA\\include\\__cudaFatFormat.h"












typedef struct {
    char*            gpuProfileName;
    char*            cubin;
} __cudaFatCubinEntry;












typedef struct {
    char*            gpuProfileName;            
    char*            ptx;
} __cudaFatPtxEntry;








typedef struct __cudaFatDebugEntryRec {
    char*                   gpuProfileName;            
    char*                   debug;
    struct __cudaFatDebugEntryRec *next;
    unsigned int                   size;
} __cudaFatDebugEntry;


typedef enum {
      __cudaFatDontSearchFlag = (1 << 0),
      __cudaFatDontCacheFlag  = (1 << 1),
      __cudaFatSassDebugFlag  = (1 << 2)
} __cudaFatCudaBinaryFlag;








typedef struct {
    char* name;
} __cudaFatSymbol;










 
typedef struct __cudaFatCudaBinaryRec {
    unsigned long            magic;
    unsigned long            version;
    unsigned long            gpuInfoVersion;
    char*                   key;
    char*                   ident;
    char*                   usageMode;
    __cudaFatPtxEntry             *ptx;
    __cudaFatCubinEntry           *cubin;
    __cudaFatDebugEntry           *debug;
    void*                  debugInfo;
    unsigned int                   flags;
    __cudaFatSymbol               *exported;
    __cudaFatSymbol               *imported;
    struct __cudaFatCudaBinaryRec *dependends;
    unsigned int                   characteristic;
} __cudaFatCudaBinary;



















    typedef enum {
        __cudaFatAvoidPTX,
        __cudaFatPreferBestCode
    } __cudaFatCompilationPolicy;



















void fatGetCubinForGpuWithPolicy( __cudaFatCudaBinary *binary, __cudaFatCompilationPolicy policy, char* gpuName, char* *cubin, char* *dbgInfoFile );










void fatFreeCubin( char* cubin, char* dbgInfoFile );


}
#line 235 "C:\\CUDA\\include\\__cudaFatFormat.h"

#line 237 "C:\\CUDA\\include\\__cudaFatFormat.h"
#line 2 "e:\\project\\flowentropy\\entropy_final_2d\\myinterface\\FlowFusion.fatbin.c"




extern "C" {
#line 8 "e:\\project\\flowentropy\\entropy_final_2d\\myinterface\\FlowFusion.fatbin.c"

static const unsigned long long __deviceText_$sm_10$[] = {
0x6365746968637261,0x6d737b2065727574,0x62610a0d7d30315f,0x6e6f697372657669,
0x0a0d7d317b202020,0x20656d616e646f6d,0x75637b2020202020,0x61730a0d7d6e6962,
0x0d7b2072656c706d,0x2020656d616e090a,0x57643374203d2020,0x66664f7468676965,
0x6574090a0d746573,0x203d2074696e7578,0x61730a0d7d0a0d31,0x0d7b2072656c706d,
0x2020656d616e090a,0x53643274203d2020,0x786574090a0d6372,0x30203d2074696e75,
0x646f630a0d7d0a0d,0x616e090a0d7b2065,0x315a5f203d20656d,0x7546776f6c465f38,
0x72656b5f6e6f6973,0x69696969666c656e,0x6164756334316969,0x5064656863746950,
0x6c090a0d5f537274,0x0d30203d206d656d,0x3d206d656d73090a,0x6572090a0d363720,
0x0d3234203d202067,0x3d2020726162090a,0x6e6f63090a0d3020,0x09090a0d7b207473,
0x656d616e67657309,0x74736e6f63203d20,0x6765730909090a0d,0x31203d20206d756e,
0x66666f0909090a0d,0x30203d2020746573,0x7479620909090a0d,0x31203d2020207365,
0x6d656d09090a0d32,0x300909090a0d7b20,0x6633303030303078,0x3030303078302066,
0x3478302031303030,0x2030303030306330,0x090a0d7d09090a0d,0x636e6962090a0d7d,
0x090a0d7b2065646f,0x3430303061783009,0x3430783020393065,0x3020303837303032,
0x3032663030313178,0x3030303178302034,0x090a0d2030313438,0x6639303032783009,
0x3030783020333030,0x3020303837303030,0x3038383030303178,0x3030313178302063,
0x090a0d2034313466,0x6430303031783009,0x3430783020353032,0x3020303837633332,
0x3134303030303178,0x3330343078302031,0x090a0d2030383763,0x3962303032783009,
0x3030783020333030,0x3020303837303030,0x3063306230303478,0x3030303078302035,
0x090a0d2030383730,0x3061303036783009,0x3030783020353065,0x3020303837343030,
0x3034343930303478,0x3032303078302039,0x090a0d2030383730,0x3030313033783009,
0x3463783020353032,0x3020303837303031,0x3034303031303378,0x3031346378302039,
0x090a0d2030383730,0x3061303036783009,0x3030783020356163,0x3020303837343030,
0x3134343830303678,0x3032303078302031,0x090a0d2030383738,0x6466333032783009,
0x6630783020353032,0x3020666666666666,0x3063633030303178,0x3332343078302039,
0x090a0d2030383763,0x3063373033783009,0x6338783020393132,0x3020303837303030,
0x3038303335303478,0x3030303078302064,0x090a0d2030383730,0x3230303131783009,
0x3034783020383032,0x3020343138306430,0x3032303038306478,0x3034303078302064,
0x090a0d2030383730,0x3032353036783009,0x3030783020643061,0x3020303837633030,
0x3030303030306178,0x3030343078302031,0x090a0d2030383730,0x3063303036783009,
0x3030783020643161,0x3020303837343130,0x3136303030306178,0x3030343078302035,
0x090a0d2030383730,0x3030313033783009,0x3463783020643036,0x3020303837303031,
0x3663343230303678,0x3032303078302064,0x090a0d2030383730,0x3030313033783009,
0x3463783020313065,0x3020303837303031,0x3738303030303278,0x3130343078302064,
0x090a0d2030383734,0x3032353036783009,0x3030783020353138,0x3020303837633030,
0x3436333030306178,0x3130343478302064,0x090a0d2030383734,0x3063303036783009,
0x3030783020353038,0x3020303837303030,0x3061386631303278,0x3030303178302063,
0x090a0d2030303661,0x6231303032783009,0x3031783020383065,0x3020303336613030,
0x3036303030306178,0x3130343478302035,0x090a0d2030383734,0x3530303031783009,
0x3430783020643232,0x3020303837633330,0x3334303030306178,0x3130343478302035,
0x090a0d2030383734,0x3030343666783009,0x3030783020313030,0x3020343837343030,
0x3330303034366678,0x3030303078302031,0x090a0d2034383734,0x6430303032783009,
0x3430783020313134,0x3020303837346132,0x3565633430303378,0x3032636178302064,
0x090a0d2030383730,0x3239323033783009,0x6336783020646666,0x3020386337633030,
0x3961393030303178,0x3030303178302038,0x090a0d2063396339,0x3030303033783009,
0x3030783020333030,0x3020303832303030,0x3163656633313278,0x6666663078302031,
0x090a0d2066666666,0x6231303032783009,0x3030783020643165,0x3020333030303030,
0x3265626633303278,0x6666663078302031,0x090a0d2066666666,0x6362313033783009,
0x6336783020393161,0x3020303837303132,0x3263636631303378,0x3132633678302035,
0x090a0d2030383730,0x3037303033783009,0x6361783020313138,0x3020303837303030,
0x3130316337303378,0x3030633878302064,0x090a0d2030383730,0x3031383064783009,
0x3430783020393163,0x3020303837303034,0x3232313138306478,0x3034343078302031,
0x090a0d2030383730,0x3835303032783009,0x3032783020303838,0x3020343861383730,
0x3963303830306478,0x3030343078302035,0x090a0d2030383730,0x6566333132783009,
0x6630783020643865,0x3020666666666666,0x3136623130303278,0x3030303078302031,
0x090a0d2033303030,0x6266333032783009,0x6630783020353136,0x3020666666666666,
0x3965333030306178,0x3130343478302031,0x090a0d2030383734,0x3030303031783009,
0x3430783020643336,0x3020303837633330,0x3338303030306178,0x3130343478302035,
0x090a0d2030383734,0x3030303061783009,0x3434783020393361,0x3020303837343130,
0x3832643130303278,0x3030303078302039,0x090a0d2033303030,0x6330303031783009,
0x3430783020353163,0x3020303837633332,0x3134343332303378,0x3030636178302031,
0x090a0d2030383730,0x3139303034783009,0x3030783020393134,0x3020303837303030,
0x3136313830303678,0x3130303078302039,0x090a0d2030383738,0x3030313033783009,
0x3463783020393163,0x3020303837303031,0x3234313830303678,0x3130303078302035,
0x090a0d2030383738,0x3463373033783009,0x6336783020646662,0x3020386337383030,
0x3030363930306178,0x3030303078302033,0x090a0d2030303030,0x3530303061783009,
0x3434783020393132,0x3020303837343130,0x3136323030303178,0x3330343078302031,
0x090a0d2030383763,0x3130303061783009,0x3434783020643165,0x3020303837343130,
0x3138633030303178,0x3030303178302034,0x090a0d2030376139,0x3930303031783009,
0x3031783020303663,0x3020343765383030,0x3665383030303178,0x3238306678302034,
0x090a0d2030313330,0x3030343666783009,0x3030783020313630,0x3020343837343030,
0x3730303034366678,0x3030303078302031,0x090a0d2034383734,0x3430303061783009,
0x3434783020353530,0x3020303837343130,0x3536323030303178,0x3330343078302031,
0x090a0d2030383763,0x3430303061783009,0x3434783020353432,0x3020303837343130,
0x3436613030303178,0x3030303178302030,0x090a0d2030323661,0x3830303031783009,
0x3032783020306165,0x3020343232396631,0x3134333030303178,0x3330343078302064,
0x090a0d2030383763,0x3130303061783009,0x3434783020353232,0x3020303837343130,
0x3530303034366678,0x3030303078302031,0x090a0d2034383734,0x3030343666783009,
0x3030783020313430,0x3020343837343030,0x3230303034366678,0x3030303078302031,
0x090a0d2034383734,0x3639303031783009,0x3030783020333030,0x3020303832303030,
0x3638333831306278,0x3431306278302030,0x090a0d2030363033,0x3339313062783009,
0x3062783020303561,0x3020633163333730,0x3538323531306278,0x3631306278302034,
0x090a0d2038356530,0x6530303131783009,0x3131783020633163,0x3020303538663030,
0x3561323131306278,0x3231306278302034,0x090a0d2038356332,0x3330313062783009,
0x3034783020343430,0x3020303463313932,0x3461323930306278,0x6130306278302038,
0x090a0d2034356332,0x3230303062783009,0x3030783020353432,0x3020303837303230,
0x3465313832303678,0x3430303078302031,0x090a0d2030383730,0x3236323062783009,
0x3062783020383434,0x3020343561323732,0x3532323030306278,0x3330303078302039,
0x090a0d2030383730,0x3230313033783009,0x3463783020313430,0x3020303837303031,
0x3332353034303278,0x3230343078302031,0x090a0d2030383763,0x3038323063783009,
0x3030783020313630,0x3020303837303030,0x3463313832303678,0x3430303078302035,
0x090a0d2030383730,0x6530303031783009,0x3430783020313430,0x3020303837633332,
0x3632353630303478,0x3030303078302035,0x090a0d2030383730,0x3234303033783009,
0x3465783020353432,0x3020303837303031,0x3630353730303678,0x3630303078302035,
0x090a0d2030383734,0x3830303065783009,0x3430783020393530,0x3020333030306330,
0x3630333332303478,0x3132303478302038,0x090a0d2030376331,0x3330313033783009,
0x3463783020353632,0x3020303837303031,0x3638633631306578,0x3632303078302031,
0x090a0d2030383730,0x3332323036783009,0x3030783020393632,0x3020303837383630,
0x3765313032303678,0x3730303078302031,0x090a0d2030383730,0x3536303036783009,
0x3030783020393530,0x3020303837343630,0x3536333430303378,0x3031346378302031,
0x090a0d2030383730,0x3330313033783009,0x3463783020353634,0x3020303837303031,
0x3638333031303378,0x3031346378302039,0x090a0d2030383730,0x3334303062783009,
0x3032783020303130,0x3020303563613431,0x3430333232303678,0x3630303078302035,
0x090a0d2030383734,0x3130323036783009,0x3030783020643163,0x3020303837383630,
0x3530353130306378,0x3230306378302038,0x090a0d2030363035,0x3031323034783009,
0x3030783020353663,0x3020303837303030,0x3432383030306578,0x6330343078302039,
0x090a0d2033303030,0x3830303065783009,0x3430783020353534,0x3020333030306330,
0x3165303230303378,0x3031346578302064,0x090a0d2030383730,0x3030323036783009,
0x3030783020353665,0x3020303837343630,0x3438633231306578,0x3532303078302039,
0x090a0d2030383738,0x6335313065783009,0x3030783020353538,0x3020303837303632,
0x3530336630303478,0x3030303078302039,0x090a0d2030383730,0x3330313033783009,
0x3463783020313632,0x3020303837303031,0x3134323530306278,0x3630306278302034,
0x090a0d2038316132,0x3365303036783009,0x3030783020353532,0x3020303837383530,
0x3663303032303678,0x3630303078302031,0x090a0d2030383730,0x6634313132783009,
0x3062783020303436,0x3020383461383130,0x3432323430303378,0x3031346378302035,
0x090a0d2030383730,0x3230313033783009,0x3463783020353561,0x3020303837303031,
0x3563303030306278,0x3030343078302031,0x090a0d2030383738,0x3332303033783009,
0x3463783020393536,0x3020303837303031,0x3432323030303278,0x3430343078302031,
0x090a0d2030383730,0x3365303036783009,0x3030783020313330,0x3020303837343530,
0x3434323431306278,0x3831303878302035,0x090a0d2030383730,0x3330303032783009,
0x3430783020313530,0x3020303837383530,0x3138663030303178,0x3330343078302064,
0x090a0d2030383763,0x3132303033783009,0x3463783020393438,0x3020303837303031,
0x3338383030306278,0x3231313278302030,0x090a0d2038346566,0x3131313062783009,
0x3038783020313338,0x3020303837303031,0x3130326530306478,0x3061306178302031,
0x090a0d2030383730,0x3230303032783009,0x3430783020313134,0x3020303837303530,
0x3338306530306478,0x3063306178302031,0x090a0d2030383730,0x6330303032783009,
0x3430783020353863,0x3020323837343832,0x3863653032313278,0x6630313278302030,
0x090a0d2063336365,0x3232323033783009,0x6336783020646666,0x3020386337343130,
0x3330383030303178,0x3030303178302030,0x090a0d2038393238,0x3830303031783009,
0x3031783020633934,0x3020303030393030,0x6134633030303178,0x3030303178302034,
0x090a0d2034303239,0x3130303031783009,0x3430783020393034,0x3020303837633330,
0x3030643330303178,0x3030303078302033,0x090a0d2030383230,0x3030303033783009,
0x3030783020333030,0x3020303837303030,0x3032303030306178,0x3030343478302064,
0x090a0d2030383734,0x3034303039783009,0x3030783020353136,0x3020303837303030,
0x3038303030306178,0x3630343478302064,0x090a0d2030383734,0x3865333032783009,
0x6630783020353161,0x3020666666666666,0x3036303530306378,0x3030303078302064,
0x090a0d2030633763,0x3030303061783009,0x3438783020643036,0x3020303837343630,
0x3163303330303478,0x3030303078302039,0x090a0d2030383730,0x3032303036783009,
0x3030783020393165,0x3020303837383130,0x3163303031303378,0x3031346378302039,
0x090a0d2030383730,0x3032303036783009,0x3030783020393163,0x3020303837383130,
0x3138303034303278,0x3130343078302039,0x090a0d2030383738,0x3030303061783009,
0x3434783020393163,0x3020303837343630,0x3163303530306378,0x3030303078302035,
0x090a0d2030633763,0x3030303061783009,0x3438783020353161,0x3020303837343630,
0x3036303030303278,0x3130343078302064,0x090a0d2030383734,0x3033303034783009,
0x3030783020353163,0x3020303837303030,0x3165303230303678,0x3130303078302035,
0x090a0d2030383734,0x3030313033783009,0x3463783020353161,0x3020303837303031,
0x3163303230303678,0x3130303078302035,0x090a0d2030383734,0x3030303033783009,
0x3430783020313161,0x3020303837303130,0x3132303430303378,0x3030343678302031,
0x090a0d2030383763,0x3030303033783009,0x3430783020313138,0x3020303837633030,
0x6633306337303378,0x3130343678302064,0x090a0d2038633734,0x3031303064783009,
0x3430783020313138,0x3020303035633230,0x3030303030303378,0x3030303078302033,
0x090a0d2030383730,0x3030303066783009,0x3065783020313030,0x3020303030303030,
0x3032303030306178,0x3030343478302039,0x090a0d2030383734,0x3034303039783009,
0x3030783020393134,0x3020303837303030,0x3038303030306178,0x3630343478302039,
0x090a0d2030383734,0x3865333032783009,0x6630783020393163,0x3020666666666666,
0x3034303630306378,0x3030303078302039,0x090a0d2030633763,0x3030303061783009,
0x3438783020393034,0x3020303837343630,0x3138303330303478,0x3030303078302064,
0x090a0d2030383730,0x3032303036783009,0x3030783020643161,0x3020303837633130,
0x3165303031303378,0x3031346378302064,0x090a0d2030383730,0x3032303036783009,
0x3030783020643138,0x3020303837633130,0x3138303034303278,0x3130343078302064,
0x090a0d2030383763,0x3030303061783009,0x3434783020643165,0x3020303837343630,
0x3165303630306378,0x3030303078302039,0x090a0d2030633763,0x3030303061783009,
0x3438783020393163,0x3020303837343630,0x3034303030303278,0x3130343078302039,
0x090a0d2030383738,0x3033303034783009,0x3030783020393138,0x3020303837303030,
0x3161303230303678,0x3130303078302039,0x090a0d2030383738,0x3030313033783009,
0x3463783020393163,0x3020303837303031,0x3138303230303678,0x3130303078302039,
0x090a0d2030383738,0x3030303033783009,0x3430783020393163,0x3020303837303130,
0x3132303630303378,0x3030343678302039,0x090a0d2030383763,0x3030303033783009,
0x3430783020393063,0x3020303837383030,0x3138303330303478,0x3030303078302039,
0x090a0d2030383730,0x3032303036783009,0x3030783020393161,0x3020303837383130,
0x3163303031303378,0x3031346378302039,0x090a0d2030383730,0x3032303036783009,
0x3030783020393038,0x3020303837383130,0x3134303030303378,0x3130343078302031,
0x090a0d2030383730,0x3063373033783009,0x3436783020646633,0x3020386337343130,
0x3138303130306478,0x3230343078302031,0x090a0d2030303563,0x3030303033783009,
0x3030783020333030,0x3020303837303030,0x3030303030306678,0x3030306578302031,
0x090a0d2031303030,0x6f630a0d7d0a0d7d,0x6e090a0d7b206564,0x5a5f203d20656d61,
0x46776f6c465f3032,0x5f44326e6f697375,0x69666c656e72656b,0x5061647563343169,
0x7450646568637469,0x6d6c090a0d5f5372,0x0a0d30203d206d65,0x203d206d656d7309,
0x676572090a0d3036,0x0a0d3432203d2020,0x203d202072616209,0x736e6f63090a0d30,
0x0909090a0d7b2074,0x20656d616e676573,0x0d74736e6f63203d,0x6e6765730909090a,
0x0d31203d20206d75,0x7366666f0909090a,0x0d30203d20207465,0x657479620909090a,
0x0d38203d20202073,0x7b206d656d09090a,0x3078300909090a0d,0x2066663330303030,
0x3030303830347830,0x7d09090a0d203030,0x62090a0d7d090a0d,0x7b2065646f636e69,
0x3031783009090a0d,0x3020353030303030,0x3837633330343078,0x3038306478302030,
0x3078302064303630,0x2030383730303430,0x3131783009090a0d,0x3020343032323030,
0x3034323030313178,0x3030306178302030,0x3078302039303430,0x2030383730303034,
0x3061783009090a0d,0x3020353036303030,0x3837303030343078,0x3130303678302030,
0x3078302064316334,0x2030383738303230,0x3036783009090a0d,0x3020643265343030,
0x3837343032303078,0x6633313278302030,0x3078302064306165,0x2066666666666666,
0x3132783009090a0d,0x3020353063656633,0x6666666666663078,0x3130303278302066,
0x3078302039306538,0x2033303030303030,0x3032783009090a0d,0x3020313036393130,
0x3030303030303078,0x3230303378302033,0x6178302039303630,0x2030383730303063,
0x3033783009090a0d,0x3020643032303030,0x3837303030636178,0x3030306178302030,
0x3478302031336530,0x2030383734313034,0x3032783009090a0d,0x3020313065386633,
0x6666666666663078,0x6230303378302066,0x3678302064666463,0x2038633730313263,
0x3061783009090a0d,0x3020353036313030,0x3837343130343478,0x3030303178302030,
0x3078302031313831,0x2030383763333034,0x3033783009090a0d,0x3020313030306337,
0x3837303030633878,0x3030306178302030,0x3478302031353430,0x2030383734313034,
0x3033783009090a0d,0x3020646662633730,0x6332303132633678,0x3030303178302038,
0x3178302034313238,0x2034353238303030,0x3061783009090a0d,0x3020313430303030,
0x3837343130343478,0x3030303178302030,0x3078302035343230,0x2030383763333034,
0x3666783009090a0d,0x3020313130303034,0x3837343030303078,0x3034366678302034,
0x3078302031353030,0x2034383734303030,0x3666783009090a0d,0x3020313430303034,
0x3837343030303078,0x6633303278302034,0x3078302031303639,0x2066666666666666,
0x3031783009090a0d,0x3020393038663030,0x3837633330343078,0x3030303178302030,
0x3078302031323831,0x2030383763333034,0x3033783009090a0d,0x3020393230306337,
0x3837303030633878,0x3030303178302030,0x3078302031303831,0x2030383763333034,
0x3061783009090a0d,0x3020353236303030,0x3837343130343478,0x3030306178302030,
0x3478302035333431,0x2030383734313034,0x3031783009090a0d,0x3020633361383030,
0x3163383030303178,0x3030303178302034,0x3178302038316161,0x2063346361303030,
0x3031783009090a0d,0x3020343532613030,0x3434613030303178,0x3238366678302034,
0x3078302031303230,0x2034383763303030,0x3666783009090a0d,0x3020313230303034,
0x3837343030303078,0x3034366678302034,0x3078302031333030,0x2034383734303030,
0x3031783009090a0d,0x3020383432393030,0x3234393030303178,0x3030303178302034,
0x3178302038326139,0x2034336339303030,0x3033783009090a0d,0x3020333030303030,
0x3031303030303078,0x3031306278302030,0x6278302038333832,0x2030326331383030,
0x3062783009090a0d,0x3020383363303531,0x3136323131306278,0x3231306278302038,
0x6278302038336331,0x2030323031633030,0x3062783009090a0d,0x3020313363303030,
0x3837343230303078,0x3330306378302030,0x3078302031343830,0x2030383730303030,
0x3065783009090a0d,0x3020313238383030,0x3030303830343078,0x3030303178302033,
0x3078302039313064,0x2030383763333234,0x3062783009090a0d,0x3020353263313030,
0x3837383230303078,0x3830306578302030,0x3078302039333863,0x2030383730343230,
0x3063783009090a0d,0x3020383236306630,0x3438313731303478,0x3030306578302030,
0x3078302035326539,0x2033303030383034,0x3062783009090a0d,0x3020303238316430,
0x3063313030306278,0x3631303678302030,0x3078302031336131,0x2030383730343030,
0x3065783009090a0d,0x3020353238633930,0x3837383232303078,0x3530306378302030,
0x3178302038323630,0x2063303866303031,0x3033783009090a0d,0x3020313338313031,
0x3837303031346378,0x3030306578302030,0x3078302031326138,0x2033303030383034,
0x3062783009090a0d,0x3020343032313130,0x3265323630303478,0x3631303678302034,
0x3078302031333831,0x2030383730333030,0x3065783009090a0d,0x3020393238633830,
0x3837383232303078,0x3030306278302030,0x3078302031323230,0x2030383763333034,
0x3036783009090a0d,0x3020393163323730,0x3837343230303078,0x3430303378302030,
0x6378302035336530,0x2030383730303134,0x3062783009090a0d,0x3020343230383430,
0x3230313830306378,0x3031303378302030,0x6378302031316330,0x2030383730303134,
0x3032783009090a0d,0x3020303338396430,0x3034313230306278,0x3930306578302038,
0x3078302031323231,0x2030383730323030,0x3036783009090a0d,0x3020313163323630,
0x3837303130303078,0x3230303378302030,0x6378302039316530,0x2030383730303134,
0x3031783009090a0d,0x3020643038663030,0x3837633330343078,0x3530306278302030,
0x3278302034313438,0x2063316565633031,0x3032783009090a0d,0x3020313138303030,
0x3837383130343078,0x3530306578302030,0x3078302035316130,0x2030383730323030,
0x3064783009090a0d,0x3020313065306530,0x3837303061306178,0x3030303278302030,
0x3078302031303664,0x2030383730313234,0x3064783009090a0d,0x3020353130306530,
0x3837303063306178,0x0a0d7d090a0d2031,0x00000000000a0d7d
};


}
#line 258 "e:\\project\\flowentropy\\entropy_final_2d\\myinterface\\FlowFusion.fatbin.c"


extern "C" {
#line 262 "e:\\project\\flowentropy\\entropy_final_2d\\myinterface\\FlowFusion.fatbin.c"

static const unsigned long long __deviceText_$compute_10$[] = {
0x6f69737265762e09,0x090a0d342e31206e,0x207465677261742e,0x6d202c30315f6d73,
0x745f3436665f7061,0x090a0d3233665f6f,0x69706d6f63202f2f,0x687469772064656c,
0x414455435c3a4320,0x2f2e2e2f6e69625c,0x6c2f34366e65706f,0x652e65622f2f6269,
0x202f2f090a0d6578,0x63636e65706f766e,0x69756220332e3220,0x3032206e6f20746c,
0x33302d38302d3930,0x2d2f2f090a0d0a0d,0x2d2d2d2d2d2d2d2d,0x2d2d2d2d2d2d2d2d,
0x2d2d2d2d2d2d2d2d,0x2d2d2d2d2d2d2d2d,0x2d2d2d2d2d2d2d2d,0x2d2d2d2d2d2d2d2d,
0x2d2d2d2d2d2d2d2d,0x202f2f090a0d2d2d,0x6e696c69706d6f43,0x7546776f6c462067,
0x7070632e6e6f6973,0x2f3a432820692e33,0x317e454d55434f44,0x2f6e657465656c2f,
0x317e534c41434f4c,0x63632f706d65542f,0x323130612e234942,0x2f2f090a0d293639,
0x2d2d2d2d2d2d2d2d,0x2d2d2d2d2d2d2d2d,0x2d2d2d2d2d2d2d2d,0x2d2d2d2d2d2d2d2d,
0x2d2d2d2d2d2d2d2d,0x2d2d2d2d2d2d2d2d,0x2d2d2d2d2d2d2d2d,0x090a0d0a0d2d2d2d,
0x2d2d2d2d2d2d2f2f,0x2d2d2d2d2d2d2d2d,0x2d2d2d2d2d2d2d2d,0x2d2d2d2d2d2d2d2d,
0x2d2d2d2d2d2d2d2d,0x2d2d2d2d2d2d2d2d,0x2d2d2d2d2d2d2d2d,0x090a0d2d2d2d2d2d,
0x6f6974704f202f2f,0x2f2f090a0d3a736e,0x2d2d2d2d2d2d2d2d,0x2d2d2d2d2d2d2d2d,
0x2d2d2d2d2d2d2d2d,0x2d2d2d2d2d2d2d2d,0x2d2d2d2d2d2d2d2d,0x2d2d2d2d2d2d2d2d,
0x2d2d2d2d2d2d2d2d,0x2f2f090a0d2d2d2d,0x7465677261542020,0x5349202c7874703a,
0x2c30315f6d733a41,0x3a6e6169646e4520,0x202c656c7474696c,0x207265746e696f50,
0x0d32333a657a6953,0x4f2d20202f2f090a,0x6d6974704f280933,0x206e6f6974617a69,
0x0a0d296c6576656c,0x30672d20202f2f09,0x2067756265442809,0x0a0d296c6576656c,
0x326d2d20202f2f09,0x74726f7065522809,0x726f736976646120,0x2f090a0d29736569,
0x2d2d2d2d2d2d2d2f,0x2d2d2d2d2d2d2d2d,0x2d2d2d2d2d2d2d2d,0x2d2d2d2d2d2d2d2d,
0x2d2d2d2d2d2d2d2d,0x2d2d2d2d2d2d2d2d,0x2d2d2d2d2d2d2d2d,0x0a0d0a0d2d2d2d2d,
0x3109656c69662e09,0x7546776f6c462209,0x6475632e6e6f6973,0x7570672e32656661,
0x6c69662e090a0d22,0x5c3a432209320965,0x206d6172676f7250,0x694d5c73656c6946,
0x2074666f736f7263,0x53206c6175736956,0x5c38206f69647574,0x554c434e495c4356,
0x65647472635c4544,0x090a0d22682e7366,0x093309656c69662e,0x414455435c3a4322,
0x6564756c636e695c,0x7665642f7472635c,0x746e75725f656369,0x0a0d22682e656d69,
0x3409656c69662e09,0x4455435c3a432209,0x64756c636e695c41,0x645f74736f685c65,
0x682e73656e696665,0x6c69662e090a0d22,0x5c3a432209350965,0x636e695c41445543,
0x6975625c6564756c,0x7079745f6e69746c,0x090a0d22682e7365,0x093609656c69662e,
0x616475635c3a6322,0x6564756c636e695c,0x5f6563697665645c,0x22682e7365707974,
0x656c69662e090a0d,0x635c3a6322093709,0x6c636e695c616475,0x766972645c656475,
0x73657079745f7265,0x662e090a0d22682e,0x6322093809656c69,0x695c616475635c3a,
0x745c6564756c636e,0x745f657275747865,0x0d22682e73657079,0x09656c69662e090a,
0x75635c3a63220939,0x756c636e695c6164,0x6f746365765c6564,0x2e73657079745f72,
0x69662e090a0d2268,0x632209303109656c,0x695c616475635c3a,0x685c6564756c636e,
0x696665645f74736f,0x0a0d22682e73656e,0x3109656c69662e09,0x55435c3a43220931,
0x756c636e695c4144,0x63697665645c6564,0x68636e75616c5f65,0x74656d617261705f,
0x0a0d22682e737265,0x3109656c69662e09,0x75635c3a63220932,0x756c636e695c6164,
0x735c7472635c6564,0x635f656761726f74,0x0d22682e7373616c,0x09656c69662e090a,
0x505c3a4322093331,0x46206d6172676f72,0x63694d5c73656c69,0x562074666f736f72,
0x7453206c61757369,0x565c38206f696475,0x44554c434e495c43,0x682e656d69745c45,
0x6c69662e090a0d22,0x3a65220934310965,0x7463656a6f72702f,0x746e45776f6c462f,
0x6f6c462f79706f72,0x2f6e6f6973754677,0x69737546776f6c46,0x2f747365745f6e6f,
0x69737546776f6c46,0x0a0d2275632e6e6f,0x3109656c69662e09,0x55435c3a43220935,
0x756c636e695c4144,0x6f6d6d6f635c6564,0x6974636e75665f6e,0x0a0d22682e736e6f,
0x3109656c69662e09,0x75635c3a63220936,0x756c636e695c6164,0x662f7472635c6564,
0x7263616d5f636e75,0x2e090a0d22682e6f,0x09373109656c6966,0x616475635c3a6322,
0x6564756c636e695c,0x75665f6874616d5c,0x2e736e6f6974636e,0x69662e090a0d2268,
0x632209383109656c,0x695c616475635c3a,0x645c6564756c636e,0x75665f6563697665,
0x2e736e6f6974636e,0x69662e090a0d2268,0x632209393109656c,0x695c616475635c3a,
0x6d5c6564756c636e,0x736e6f635f687461,0x22682e73746e6174,0x656c69662e090a0d,
0x5c3a632209303209,0x636e695c61647563,0x5f6d735c6564756c,0x696d6f74615f3131,
0x6974636e75665f63,0x0a0d22682e736e6f,0x3209656c69662e09,0x75635c3a63220931,
0x756c636e695c6164,0x32315f6d735c6564,0x5f63696d6f74615f,0x6e6f6974636e7566,
0x2e090a0d22682e73,0x09323209656c6966,0x616475635c3a6322,0x6564756c636e695c,
0x645f33315f6d735c,0x75665f656c62756f,0x2e736e6f6974636e,0x69662e090a0d2268,
0x632209333209656c,0x695c616475635c3a,0x635c6564756c636e,0x79745f6e6f6d6d6f,
0x0a0d22682e736570,0x3209656c69662e09,0x75635c3a63220934,0x756c636e695c6164,
0x75747865745c6564,0x68637465665f6572,0x6f6974636e75665f,0x090a0d22682e736e,
0x353209656c69662e,0x6475635c3a632209,0x64756c636e695c61,0x665f6874616d5c65,
0x736e6f6974636e75,0x7874705f6c62645f,0x0a0d0a0d22682e31,0x752e207865742e09,
0x7253643274203233,0x65742e090a0d3b63,0x74203233752e2078,0x7468676965576433,
0x0d3b74657366664f,0x746e652e090a0d0a,0x5f30325a5f207972,0x69737546776f6c46,
0x72656b5f44326e6f,0x34316969666c656e,0x6374695061647563,0x5f53727450646568,
0x702e09090a0d2820,0x33662e206d617261,0x616475635f5f2032,0x325a5f5f6d726170,
0x7546776f6c465f30,0x6b5f44326e6f6973,0x6969666c656e7265,0x6950616475633431,
0x7274506465686374,0x65747441665f5f53,0x2c6e6f697461756e,0x7261702e09090a0d,
0x203233732e206d61,0x6170616475635f5f,0x5f30325a5f5f6d72,0x69737546776f6c46,
0x72656b5f44326e6f,0x34316969666c656e,0x6374695061647563,0x5f53727450646568,
0x656d756c6f56695f,0x0a0d2c6874646957,0x6d617261702e0909,0x5f5f203233732e20,
0x6d72617061647563,0x6c465f30325a5f5f,0x6e6f69737546776f,0x656e72656b5f4432,
0x756334316969666c,0x6568637469506164,0x695f5f5372745064,0x6548656d756c6f56,
0x090a0d2c74686769,0x206d617261702e09,0x34206e67696c612e,0x635f5f2038622e20,
0x5f6d726170616475,0x6f6c465f30325a5f,0x326e6f6973754677,0x6c656e72656b5f44,
0x6475633431696966,0x6465686374695061,0x44635f5f53727450,0x6568637469507473,
0x5d36315b72745064,0x61702e09090a0d2c,0x696c612e206d6172,0x38622e2034206e67,
0x70616475635f5f20,0x30325a5f5f6d7261,0x737546776f6c465f,0x656b5f44326e6f69,
0x316969666c656e72,0x7469506164756334,0x5372745064656863,0x726f727245635f5f,
0x5064656863746950,0x0d295d36315b7274,0x722e090a0d7b090a,0x203631752e206765,
0x0d3b3e363c687225,0x2e206765722e090a,0x343c722520323375,0x722e090a0d3b3e30,
0x203233662e206765,0x3b3e3630313c6625,0x206765722e090a0d,0x702520646572702e,
0x2e090a0d3b3e333c,0x3709343109636f6c,0x424c240a0d300931,0x5f30325a5f5f3142,
0x69737546776f6c46,0x72656b5f44326e6f,0x34316969666c656e,0x6374695061647563,
0x5f53727450646568,0x2e766f6d090a0d3a,0x6872250920363175,0x6961746325202c31,
0x6d090a0d3b782e64,0x09203631752e766f,0x6e25202c32687225,0x0a0d3b782e646974,
0x6469772e6c756d09,0x2509203631752e65,0x31687225202c3172,0x0d3b32687225202c,
0x31752e766f6d090a,0x2c33687225092036,0x2e64696174632520,0x766f6d090a0d3b79,
0x722509203631752e,0x69746e25202c3468,0x6d090a0d3b792e64,0x2e656469772e6c75,
0x3272250920363175,0x202c33687225202c,0x090a0d3b34687225,0x2e3233752e747663,
0x3372250920363175,0x782e64697425202c,0x2e646461090a0d3b,0x3472250920323375,
0x25202c337225202c,0x7663090a0d3b3172,0x31752e3233752e74,0x202c357225092036,
0x0d3b792e64697425,0x33752e646461090a,0x202c367225092032,0x327225202c357225,
0x2e747663090a0d3b,0x732e3233662e6e72,0x2c31662509203233,0x090a0d3b34722520,
0x662e6e722e747663,0x09203233732e3233,0x367225202c326625,0x2e766f6d090a0d3b,
0x3366250920323366,0x0a0d3b316625202c,0x3233662e766f6d09,0x25202c3466250920,
0x6f6d090a0d3b3266,0x2509203233662e76,0x30306630202c3566,0x203b303030303030,
0x202f2f0920202020,0x2e766f6d090a0d30,0x3666250920323366,0x303030306630202c,
0x2020203b30303030,0x0d30202f2f092020,0x64322e786574090a,0x2e3233662e34762e,
0x3766257b20323366,0x3966252c3866252c,0x5b2c7d303166252c,0x7b2c637253643274,
0x2c3466252c336625,0x7d3666252c356625,0x6f6c2e090a0d3b5d,0x0939370934310963,
0x2e766f6d090a0d30,0x3166250920323366,0x0d3b376625202c31,0x33662e766f6d090a,
0x2c32316625092032,0x090a0d3b38662520,0x203233662e766f6d,0x25202c3331662509,
0x6c2e090a0d3b3966,0x303809343109636f,0x2e646c090a0d3009,0x33732e6d61726170,
0x202c377225092032,0x70616475635f5f5b,0x30325a5f5f6d7261,0x737546776f6c465f,
0x656b5f44326e6f69,0x316969666c656e72,0x7469506164756334,0x5372745064656863,
0x6d756c6f56695f5f,0x3b5d687464695765,0x732e627573090a0d,0x2c38722509203233,
0x3b31202c37722520,0x732e646461090a0d,0x2c39722509203233,0x3b31202c34722520,
0x732e6e696d090a0d,0x3031722509203233,0x25202c387225202c,0x7663090a0d3b3972,
0x3233662e6e722e74,0x662509203233732e,0x30317225202c3431,0x2e766f6d090a0d3b,
0x3166250920323366,0x0d3b326625202c35,0x33662e766f6d090a,0x2c36316625092032,
0x3030303030663020,0x202020203b303030,0x090a0d30202f2f09,0x203233662e766f6d,
0x30202c3731662509,0x3030303030303066,0x2f09202020203b30,0x6574090a0d30202f,
0x2e34762e64322e78,0x203233662e323366,0x66252c383166257b,0x2c303266252c3931,
0x745b2c7d31326625,0x257b2c6372536432,0x353166252c343166,0x66252c363166252c,
0x090a0d3b5d7d3731,0x203233662e766f6d,0x25202c3232662509,0x6d090a0d3b383166,
0x09203233662e766f,0x6625202c33326625,0x6f6d090a0d3b3931,0x2509203233662e76,
0x326625202c343266,0x6f6c2e090a0d3b30,0x0931380934310963,0x2e627573090a0d30,
0x3172250920323373,0x202c347225202c31,0x766f6d090a0d3b31,0x722509203233732e,
0x0a0d3b30202c3231,0x3233732e78616d09,0x202c333172250920,0x7225202c31317225,
0x7663090a0d3b3231,0x3233662e6e722e74,0x662509203233732e,0x33317225202c3532,
0x2e766f6d090a0d3b,0x3266250920323366,0x0d3b326625202c36,0x33662e766f6d090a,
0x2c37326625092032,0x3030303030663020,0x202020203b303030,0x090a0d30202f2f09,
0x203233662e766f6d,0x30202c3832662509,0x3030303030303066,0x2f09202020203b30,
0x6574090a0d30202f,0x2e34762e64322e78,0x203233662e323366,0x66252c393266257b,
0x2c313366252c3033,0x745b2c7d32336625,0x257b2c6372536432,0x363266252c353266,
0x66252c373266252c,0x090a0d3b5d7d3832,0x203233662e766f6d,0x25202c3333662509,
0x6d090a0d3b393266,0x09203233662e766f,0x6625202c34336625,0x6f6d090a0d3b3033,
0x2509203233662e76,0x336625202c353366,0x6f6c2e090a0d3b31,0x0932380934310963,
0x702e646c090a0d30,0x3233732e6d617261,0x202c343172250920,0x70616475635f5f5b,
0x30325a5f5f6d7261,0x737546776f6c465f,0x656b5f44326e6f69,0x316969666c656e72,
0x7469506164756334,0x5372745064656863,0x6d756c6f56695f5f,0x5d74686769654865,
0x2e766f6d090a0d3b,0x3366250920323366,0x0d3b316625202c36,0x33732e627573090a,
0x2c35317225092032,0x31202c3431722520,0x2e646461090a0d3b,0x3172250920323373,
0x202c367225202c36,0x6e696d090a0d3b31,0x722509203233732e,0x35317225202c3731,
0x0d3b36317225202c,0x6e722e747663090a,0x3233732e3233662e,0x202c373366250920,
0x090a0d3b37317225,0x203233662e766f6d,0x30202c3833662509,0x3030303030303066,
0x2f09202020203b30,0x6f6d090a0d30202f,0x2509203233662e76,0x306630202c393366,
0x3b30303030303030,0x202f2f0920202020,0x2e786574090a0d30,0x33662e34762e6432,
0x257b203233662e32,0x313466252c303466,0x66252c323466252c,0x6432745b2c7d3334,
0x3366257b2c637253,0x252c373366252c36,0x393366252c383366,0x6f6d090a0d3b5d7d,
0x2509203233662e76,0x346625202c343466,0x766f6d090a0d3b30,0x662509203233662e,
0x31346625202c3534,0x2e766f6d090a0d3b,0x3466250920323366,0x3b32346625202c36,
0x09636f6c2e090a0d,0x0d30093338093431,0x33662e766f6d090a,0x2c37346625092032,
0x090a0d3b31662520,0x203233732e627573,0x25202c3831722509,0x0a0d3b31202c3672,
0x3233732e766f6d09,0x202c393172250920,0x78616d090a0d3b30,0x722509203233732e,
0x38317225202c3032,0x0d3b39317225202c,0x6e722e747663090a,0x3233732e3233662e,
0x202c383466250920,0x090a0d3b30327225,0x203233662e766f6d,0x30202c3934662509,
0x3030303030303066,0x2f09202020203b30,0x6f6d090a0d30202f,0x2509203233662e76,
0x306630202c303566,0x3b30303030303030,0x202f2f0920202020,0x2e786574090a0d30,
0x33662e34762e6432,0x257b203233662e32,0x323566252c313566,0x66252c333566252c,
0x6432745b2c7d3435,0x3466257b2c637253,0x252c383466252c37,0x303566252c393466,
0x6f6d090a0d3b5d7d,0x2509203233662e76,0x356625202c353566,0x766f6d090a0d3b31,
0x662509203233662e,0x32356625202c3635,0x2e766f6d090a0d3b,0x3566250920323366,
0x3b33356625202c37,0x662e766f6d090a0d,0x3835662509203233,0x0a0d3b316625202c,
0x3233662e766f6d09,0x202c393566250920,0x6d090a0d3b326625,0x09203233662e766f,
0x6630202c30366625,0x3030303030303030,0x2f2f09202020203b,0x766f6d090a0d3020,
0x662509203233662e,0x30306630202c3136,0x203b303030303030,0x30202f2f09202020,
0x332e786574090a0d,0x3233662e34762e64,0x66257b203233662e,0x2c333666252c3236,
0x3666252c34366625,0x576433745b2c7d35,0x66664f7468676965,0x3566257b2c746573,
0x252c393566252c38,0x313666252c303666,0x6c2e090a0d3b5d7d,0x353809343109636f,
0x766f6d090a0d3009,0x662509203233662e,0x32366625202c3636,0x2e766f6d090a0d3b,
0x3666250920323366,0x3b33366625202c37,0x662e766f6d090a0d,0x3836662509203233,
0x0d3b34366625202c,0x33662e766f6d090a,0x2c39366625092032,0x0a0d3b3536662520,
0x2e74672e74657309,0x203233732e323375,0x25202c3132722509,0x367225202c343172,
0x2e67656e090a0d3b,0x3272250920323373,0x3b31327225202c32,0x672e746573090a0d,
0x33732e3233752e74,0x2c33327225092032,0x7225202c37722520,0x67656e090a0d3b34,
0x722509203233732e,0x33327225202c3432,0x2e646e61090a0d3b,0x3272250920323362,
0x2c32327225202c35,0x0a0d3b3432722520,0x3233752e766f6d09,0x202c363272250920,
0x746573090a0d3b30,0x3233732e71652e70,0x25202c3170250920,0x327225202c353272,
0x702540090a0d3b36,0x2409206172622031,0x3832315f305f744c,0x6f6c2e090a0d3b32,
0x0937390934310963,0x2e6c756d090a0d30,0x3766250920323366,0x2c31316625202c30,
0x0a0d3b3936662520,0x3233662e64646109,0x202c313766250920,0x6625202c32326625,
0x6461090a0d3b3333,0x2509203233662e64,0x376625202c323766,0x3b34346625202c31,
0x6c2e6c756d090a0d,0x2509203233752e6f,0x347225202c373272,0x090a0d3b3631202c,
0x6d617261702e646c,0x722509203233752e,0x635f5f5b202c3832,0x5f6d726170616475,
0x6f6c465f30325a5f,0x326e6f6973754677,0x6c656e72656b5f44,0x6475633431696966,
0x6465686374695061,0x44635f5f53727450,0x6568637469507473,0x3b5d342b72745064,
0x6c2e6c756d090a0d,0x2509203233752e6f,0x327225202c393272,0x0d3b367225202c38,
0x33662e646461090a,0x2c33376625092032,0x25202c3237662520,0x61090a0d3b353566,
0x09203233752e6464,0x7225202c30337225,0x39327225202c3732,0x2e766f6d090a0d3b,
0x3766250920323366,0x3830346630202c34,0x20203b3030303030,0x0d34202f2f092020,
0x33662e6c756d090a,0x2c35376625092032,0x25202c3437662520,0x73090a0d3b313166,
0x09203233662e6275,0x6625202c36376625,0x35376625202c3337,0x702e646c090a0d3b,
0x3233752e6d617261,0x202c313372250920,0x70616475635f5f5b,0x30325a5f5f6d7261,
0x737546776f6c465f,0x656b5f44326e6f69,0x316969666c656e72,0x7469506164756334,
0x5372745064656863,0x6950747344635f5f,0x7274506465686374,0x61090a0d3b5d302b,
0x09203233752e6464,0x7225202c32337225,0x30337225202c3133,0x702e646c090a0d3b,
0x3233662e6d617261,0x202c373766250920,0x70616475635f5f5b,0x30325a5f5f6d7261,
0x737546776f6c465f,0x656b5f44326e6f69,0x316969666c656e72,0x7469506164756334,
0x5372745064656863,0x6e65747441665f5f,0x3b5d6e6f69746175,0x662e64616d090a0d,
0x3837662509203233,0x202c37376625202c,0x6625202c36376625,0x6461090a0d3b3037,
0x2509203233662e64,0x376625202c393766,0x3b36366625202c38,0x662e6c756d090a0d,
0x3038662509203233,0x202c32316625202c,0x090a0d3b39366625,0x203233662e646461,
0x25202c3138662509,0x336625202c333266,0x646461090a0d3b34,0x662509203233662e,
0x31386625202c3238,0x0d3b35346625202c,0x33662e646461090a,0x2c33386625092032,
0x25202c3238662520,0x6d090a0d3b363566,0x09203233662e766f,0x6630202c34386625,
0x3030303030383034,0x2f2f09202020203b,0x6c756d090a0d3420,0x662509203233662e,
0x34386625202c3538,0x0d3b32316625202c,0x33662e627573090a,0x2c36386625092032,
0x25202c3338662520,0x6d090a0d3b353866,0x09203233662e6461,0x6625202c37386625,
0x36386625202c3737,0x0d3b30386625202c,0x33662e646461090a,0x2c38386625092032,
0x25202c3738662520,0x6d090a0d3b373666,0x09203233662e6c75,0x6625202c39386625,
0x39366625202c3331,0x2e646461090a0d3b,0x3966250920323366,0x2c34326625202c30,
0x0a0d3b3533662520,0x3233662e64646109,0x202c313966250920,0x6625202c30396625,
0x6461090a0d3b3634,0x2509203233662e64,0x396625202c323966,0x3b37356625202c31,
0x662e766f6d090a0d,0x3339662509203233,0x303830346630202c,0x2020203b30303030,
0x0a0d34202f2f0920,0x3233662e6c756d09,0x202c343966250920,0x6625202c33396625,
0x7573090a0d3b3331,0x2509203233662e62,0x396625202c353966,0x3b34396625202c32,
0x662e64616d090a0d,0x3639662509203233,0x202c37376625202c,0x6625202c35396625,
0x6461090a0d3b3938,0x2509203233662e64,0x396625202c373966,0x3b38366625202c36,
0x662e766f6d090a0d,0x3839662509203233,0x303030306630202c,0x2020203b30303030,
0x0a0d30202f2f0920,0x626f6c672e747309,0x33662e34762e6c61,0x323372255b092032,
0x66257b202c5d302b,0x2c383866252c3937,0x3966252c37396625,0x6c2e090a0d3b7d38,
0x303109343109636f,0x7573090a0d300937,0x2509203233662e62,0x316625202c393966,
0x3b39376625202c31,0x662e627573090a0d,0x3031662509203233,0x2c32316625202c30,
0x0a0d3b3838662520,0x3233662e62757309,0x2c31303166250920,0x25202c3331662520,
0x6d090a0d3b373966,0x09203233662e6c75,0x25202c3230316625,0x6625202c30303166,
0x6d090a0d3b303031,0x09203233662e6461,0x25202c3330316625,0x396625202c393966,
0x3230316625202c39,0x2e64616d090a0d3b,0x3166250920323366,0x30316625202c3430,
0x3130316625202c31,0x3b3330316625202c,0x61702e646c090a0d,0x203233752e6d6172,
0x5b202c3333722509,0x6170616475635f5f,0x5f30325a5f5f6d72,0x69737546776f6c46,
0x72656b5f44326e6f,0x34316969666c656e,0x6374695061647563,0x5f53727450646568,
0x50726f727245635f,0x7450646568637469,0x090a0d3b5d302b72,0x6d617261702e646c,
0x722509203233752e,0x635f5f5b202c3433,0x5f6d726170616475,0x6f6c465f30325a5f,
0x326e6f6973754677,0x6c656e72656b5f44,0x6475633431696966,0x6465686374695061,
0x45635f5f53727450,0x63746950726f7272,0x342b727450646568,0x6c756d090a0d3b5d,
0x203233752e6f6c2e,0x25202c3533722509,0x367225202c343372,0x2e6c756d090a0d3b,
0x09203233752e6f6c,0x7225202c36337225,0x090a0d3b34202c34,0x203233752e646461,
0x25202c3733722509,0x337225202c353372,0x646461090a0d3b36,0x722509203233752e,
0x33337225202c3833,0x0d3b37337225202c,0x6f6c672e7473090a,0x203233662e6c6162,
0x302b383372255b09,0x3430316625202c5d,0x305f744c240a0d3b,0x0a0d3a323832315f,
0x343109636f6c2e09,0x0a0d300932313109,0x0a0d3b7469786509,0x5f646e6557444c24,
0x6f6c465f30325a5f,0x326e6f6973754677,0x6c656e72656b5f44,0x6475633431696966,
0x6465686374695061,0x0a0d3a5f53727450,0x5a5f202f2f207d09,0x46776f6c465f3032,
0x5f44326e6f697375,0x69666c656e72656b,0x5061647563343169,0x7450646568637469,
0x090a0d0a0d5f5372,0x5f207972746e652e,0x776f6c465f38315a,0x6b5f6e6f69737546,
0x6969666c656e7265,0x7563343169696969,0x6568637469506164,0x28205f5372745064,
0x7261702e09090a0d,0x203233662e206d61,0x6170616475635f5f,0x5f38315a5f5f6d72,
0x69737546776f6c46,0x656e72656b5f6e6f,0x696969696969666c,0x6950616475633431,
0x7274506465686374,0x65747441665f5f53,0x2c6e6f697461756e,0x7261702e09090a0d,
0x203233732e206d61,0x6170616475635f5f,0x5f38315a5f5f6d72,0x69737546776f6c46,
0x656e72656b5f6e6f,0x696969696969666c,0x6950616475633431,0x7274506465686374,
0x756c6f56695f5f53,0x2c6874646957656d,0x7261702e09090a0d,0x203233732e206d61,
0x6170616475635f5f,0x5f38315a5f5f6d72,0x69737546776f6c46,0x656e72656b5f6e6f,
0x696969696969666c,0x6950616475633431,0x7274506465686374,0x756c6f56695f5f53,
0x746867696548656d,0x61702e09090a0d2c,0x3233732e206d6172,0x70616475635f5f20,
0x38315a5f5f6d7261,0x737546776f6c465f,0x6e72656b5f6e6f69,0x6969696969666c65,
0x5061647563343169,0x7450646568637469,0x6c6f56695f5f5372,0x6874706544656d75,
0x61702e09090a0d2c,0x3233732e206d6172,0x70616475635f5f20,0x38315a5f5f6d7261,
0x737546776f6c465f,0x6e72656b5f6e6f69,0x6969696969666c65,0x5061647563343169,
0x7450646568637469,0x0d2c5a695f5f5372,0x617261702e09090a,0x5f203233732e206d,
0x726170616475635f,0x465f38315a5f5f6d,0x6f69737546776f6c,0x6c656e72656b5f6e,
0x3169696969696966,0x7469506164756334,0x5372745064656863,0x59664f724e695f5f,
0x0d2c736b636f6c42,0x617261702e09090a,0x5f203233732e206d,0x726170616475635f,
0x465f38315a5f5f6d,0x6f69737546776f6c,0x6c656e72656b5f6e,0x3169696969696966,
0x7469506164756334,0x5372745064656863,0x6b636f6c42695f5f,0x0a0d2c657a69535a,
0x6d617261702e0909,0x206e67696c612e20,0x5f5f2038622e2034,0x6d72617061647563,
0x6c465f38315a5f5f,0x6e6f69737546776f,0x666c656e72656b5f,0x3431696969696969,
0x6374695061647563,0x5f53727450646568,0x746950747344635f,0x5b72745064656863,
0x09090a0d2c5d3631,0x2e206d617261702e,0x2034206e67696c61,0x75635f5f2038622e,
0x5f5f6d7261706164,0x776f6c465f38315a,0x6b5f6e6f69737546,0x6969666c656e7265,
0x7563343169696969,0x6568637469506164,0x635f5f5372745064,0x746950726f727245,
0x5b72745064656863,0x7b090a0d295d3631,0x206765722e090a0d,0x687225203631752e,
0x2e090a0d3b3e343c,0x3233752e20676572,0x3b3e33373c722520,0x206765722e090a0d,
0x3c6625203233662e,0x090a0d3b3e353431,0x72702e206765722e,0x3e373c7025206465,
0x636f6c2e090a0d3b,0x0935343109343109,0x3142424c240a0d30,0x6c465f38315a5f5f,
0x6e6f69737546776f,0x666c656e72656b5f,0x3431696969696969,0x6374695061647563,
0x5f53727450646568,0x636f6c2e090a0d3a,0x0930363109343109,0x2e766f6d090a0d30,
0x6872250920363175,0x6961746325202c31,0x6d090a0d3b782e64,0x09203631752e766f,
0x6e25202c32687225,0x0a0d3b782e646974,0x6469772e6c756d09,0x2509203631752e65,
0x31687225202c3172,0x0d3b32687225202c,0x7261702e646c090a,0x09203233752e6d61,
0x5f5f5b202c327225,0x6d72617061647563,0x6c465f38315a5f5f,0x6e6f69737546776f,
0x666c656e72656b5f,0x3431696969696969,0x6374695061647563,0x5f53727450646568,
0x4259664f724e695f,0x0d3b5d736b636f6c,0x33752e747663090a,0x2509203631752e32,
0x61746325202c3372,0x090a0d3b792e6469,0x203233752e766964,0x7225202c34722509,
0x0d3b327225202c33,0x33752e6d6572090a,0x202c357225092032,0x327225202c337225,
0x2e747663090a0d3b,0x203631752e323375,0x7425202c36722509,0x090a0d3b782e6469,
0x203233752e646461,0x7225202c37722509,0x0d3b317225202c36,0x7261702e646c090a,
0x09203233752e6d61,0x5f5f5b202c387225,0x6d72617061647563,0x6c465f38315a5f5f,
0x6e6f69737546776f,0x666c656e72656b5f,0x3431696969696969,0x6374695061647563,
0x5f53727450646568,0x5a6b636f6c42695f,0x0a0d3b5d657a6953,0x2e6f6c2e6c756d09,
0x3972250920323375,0x25202c347225202c,0x7663090a0d3b3872,0x31752e3233752e74,
0x2c30317225092036,0x792e6469746e2520,0x2e6c756d090a0d3b,0x09203233752e6f6c,
0x7225202c31317225,0x3b357225202c3031,0x722e747663090a0d,0x33732e3233662e6e,
0x202c316625092032,0x6c090a0d3b377225,0x2e6d617261702e64,0x3172250920323373,
0x75635f5f5b202c32,0x5f5f6d7261706164,0x776f6c465f38315a,0x6b5f6e6f69737546,
0x6969666c656e7265,0x7563343169696969,0x6568637469506164,0x695f5f5372745064,
0x6548656d756c6f56,0x0a0d3b5d74686769,0x2e6f6c2e6c756d09,0x3172250920323373,
0x2c32317225202c33,0x090a0d3b39722520,0x2e3233752e747663,0x3172250920363175,
0x2e64697425202c34,0x646461090a0d3b79,0x722509203233752e,0x34317225202c3531,
0x0d3b31317225202c,0x33732e646461090a,0x2c36317225092032,0x25202c3331722520,
0x6d090a0d3b353172,0x09203233662e766f,0x316625202c326625,0x2e747663090a0d3b,
0x732e3233662e6e72,0x2c33662509203233,0x0a0d3b3631722520,0x3233662e766f6d09,
0x30202c3466250920,0x3030303030303066,0x0920202020203b30,0x6d090a0d30202f2f,
0x09203233662e766f,0x306630202c356625,0x3b30303030303030,0x2f2f092020202020,
0x786574090a0d3020,0x662e34762e64322e,0x7b203233662e3233,0x2c3766252c366625,
0x7d3966252c386625,0x6372536432745b2c,0x66252c3266257b2c,0x66252c3466252c33,
0x2e090a0d3b5d7d35,0x3209343109636f6c,0x6d090a0d30093037,0x09203233662e766f,
0x6625202c30316625,0x766f6d090a0d3b36,0x662509203233662e,0x3b376625202c3131,
0x662e766f6d090a0d,0x3231662509203233,0x0a0d3b386625202c,0x3233662e766f6d09,
0x202c333166250920,0x090a0d3b30316625,0x203233662e766f6d,0x25202c3431662509,
0x6d090a0d3b313166,0x09203233662e766f,0x6625202c35316625,0x6c2e090a0d3b3231,
0x373209343109636f,0x6f6d090a0d300931,0x2509203233662e76,0x316625202c363166,
0x2e627573090a0d3b,0x3172250920323373,0x202c397225202c37,0x766f6d090a0d3b31,
0x722509203233732e,0x0a0d3b30202c3831,0x3233732e78616d09,0x202c393172250920,
0x7225202c37317225,0x756d090a0d3b3831,0x3233732e6f6c2e6c,0x202c303272250920,
0x7225202c32317225,0x6461090a0d3b3931,0x2509203233732e64,0x317225202c313272,
0x3b30327225202c35,0x722e747663090a0d,0x33732e3233662e6e,0x2c37316625092032,
0x0a0d3b3132722520,0x3233662e766f6d09,0x202c383166250920,0x3030303030306630,
0x09202020203b3030,0x6d090a0d30202f2f,0x09203233662e766f,0x6630202c39316625,
0x3030303030303030,0x2f2f09202020203b,0x786574090a0d3020,0x662e34762e64322e,
0x7b203233662e3233,0x3266252c30326625,0x252c323266252c31,0x32745b2c7d333266,
0x66257b2c63725364,0x2c373166252c3631,0x3166252c38316625,0x6d090a0d3b5d7d39,
0x09203233662e766f,0x6625202c34326625,0x6f6d090a0d3b3032,0x2509203233662e76,
0x326625202c353266,0x766f6d090a0d3b31,0x662509203233662e,0x32326625202c3632,
0x2e766f6d090a0d3b,0x3266250920323366,0x3b34326625202c37,0x662e766f6d090a0d,
0x3832662509203233,0x0d3b35326625202c,0x33662e766f6d090a,0x2c39326625092032,
0x0a0d3b3632662520,0x343109636f6c2e09,0x0a0d300934373209,0x3233732e766f6d09,
0x202c323272250920,0x61090a0d3b397225,0x09203233732e6464,0x7225202c33327225,
0x0d3b387225202c39,0x7261702e646c090a,0x09203233732e6d61,0x5f5b202c34327225,
0x726170616475635f,0x465f38315a5f5f6d,0x6f69737546776f6c,0x6c656e72656b5f6e,
0x3169696969696966,0x7469506164756334,0x5372745064656863,0x6d756c6f56695f5f,
0x3b5d687470654465,0x732e6e696d090a0d,0x3532722509203233,0x202c33327225202c,
0x090a0d3b34327225,0x2e656c2e70746573,0x3170250920323373,0x202c35327225202c,
0x40090a0d3b397225,0x2061726220317025,0x325f315f744c2409,0x73090a0d3b363033,
0x09203233732e6275,0x7225202c36327225,0x3b397225202c3532,0x732e627573090a0d,
0x3732722509203233,0x202c32317225202c,0x627573090a0d3b31,0x722509203233732e,
0x34327225202c3832,0x6c090a0d3b31202c,0x2e6d617261702e64,0x3272250920323373,
0x75635f5f5b202c39,0x5f5f6d7261706164,0x776f6c465f38315a,0x6b5f6e6f69737546,
0x6969666c656e7265,0x7563343169696969,0x6568637469506164,0x695f5f5372745064,
0x6957656d756c6f56,0x090a0d3b5d687464,0x2e74672e70746573,0x3270250920323373,
0x202c39327225202c,0x61090a0d3b377225,0x09203233732e6464,0x7225202c30337225,
0x090a0d3b31202c37,0x662e6e722e747663,0x09203233732e3233,0x7225202c30336625,
0x7573090a0d3b3033,0x2509203233732e62,0x377225202c313372,0x63090a0d3b31202c,
0x33662e6e722e7476,0x2509203233732e32,0x337225202c313366,0x6c6573090a0d3b31,
0x2509203233732e70,0x202c31202c323372,0x0d3b327025202c30,0x6e722e747663090a,
0x3233732e3233662e,0x202c323366250920,0x090a0d3b35317225,0x203233732e766f6d,
0x25202c3333722509,0x61090a0d3b363172,0x09203233732e6464,0x7225202c34337225,
0x0a0d3b31202c3531,0x3233732e62757309,0x202c353372250920,0x3b31202c35317225,
0x2e70746573090a0d,0x09203233732e7467,0x317225202c337025,0x3b35317225202c32,
0x732e6e696d090a0d,0x3633722509203233,0x202c37327225202c,0x090a0d3b34337225,
0x203233732e766f6d,0x30202c3733722509,0x2e78616d090a0d3b,0x3372250920323373,
0x2c35337225202c38,0x0a0d3b3733722520,0x33732e706c657309,0x2c39337225092032,
0x25202c30202c3120,0x6461090a0d3b3370,0x2509203233732e64,0x337225202c303472,
0x3b33317225202c36,0x732e646461090a0d,0x3134722509203233,0x202c38337225202c,
0x090a0d3b33317225,0x203233622e646e61,0x25202c3234722509,0x337225202c323372,
0x766f6d090a0d3b39,0x722509203233732e,0x36327225202c3334,0x315f744c240a0d3b,
0x0a0d3a383138325f,0x706f6f6c3c2f2f20,0x6220706f6f4c203e,0x656e696c2079646f,
0x656e202c34373220,0x656420676e697473,0x202c31203a687470,0x6574616d69747365,
0x7461726574692064,0x6e75203a736e6f69,0x090a0d6e776f6e6b,0x203233662e766f6d,
0x25202c3333662509,0x6f6d090a0d3b3166,0x2509203233662e76,0x336625202c343366,
0x747663090a0d3b32,0x2e3233662e6e722e,0x3366250920323373,0x3b32327225202c35,
0x662e766f6d090a0d,0x3633662509203233,0x303030306630202c,0x2020203b30303030,
0x0a0d30202f2f0920,0x2e64332e78657409,0x662e3233662e3476,0x373366257b203233,
0x66252c383366252c,0x7d303466252c3933,0x6965576433745b2c,0x657366664f746867,
0x2c333366257b2c74,0x3366252c34336625,0x5d7d363366252c35,0x6c3c2f2f200a0d3b,
0x726150203e706f6f,0x6f6f6c20666f2074,0x6c2079646f622070,0x2c34373220656e69,
0x616c206461656820,0x4c242064656c6562,0x383138325f315f74,0x09636f6c2e090a0d,
0x3009373732093431,0x662e766f6d090a0d,0x3134662509203233,0x0d3b37336625202c,
0x33662e766f6d090a,0x2c32346625092032,0x0a0d3b3833662520,0x3233662e766f6d09,
0x202c333466250920,0x090a0d3b39336625,0x203233662e766f6d,0x25202c3434662509,
0x63090a0d3b303466,0x33662e6e722e7476,0x2509203233732e32,0x337225202c353466,
0x766f6d090a0d3b33,0x662509203233662e,0x30336625202c3634,0x2e766f6d090a0d3b,
0x3466250920323366,0x3b35346625202c37,0x662e766f6d090a0d,0x3834662509203233,
0x303030306630202c,0x2020203b30303030,0x0a0d30202f2f0920,0x3233662e766f6d09,
0x202c393466250920,0x3030303030306630,0x09202020203b3030,0x74090a0d30202f2f,
0x34762e64322e7865,0x3233662e3233662e,0x252c303566257b20,0x323566252c313566,
0x5b2c7d333566252c,0x7b2c637253643274,0x3466252c36346625,0x252c383466252c37,
0x0a0d3b5d7d393466,0x706f6f6c3c2f2f20,0x6f2074726150203e,0x6220706f6f6c2066,
0x656e696c2079646f,0x6568202c34373220,0x6c6562616c206461,0x315f744c24206465,
0x090a0d383138325f,0x09343109636f6c2e,0x090a0d3009323832,0x203233662e766f6d,
0x25202c3435662509,0x6d090a0d3b303566,0x09203233662e766f,0x6625202c35356625,
0x6f6d090a0d3b3135,0x2509203233662e76,0x356625202c363566,0x766f6d090a0d3b32,
0x662509203233662e,0x31336625202c3735,0x2e766f6d090a0d3b,0x3566250920323366,
0x3b35346625202c38,0x662e766f6d090a0d,0x3935662509203233,0x303030306630202c,
0x2020203b30303030,0x0a0d30202f2f0920,0x3233662e766f6d09,0x202c303666250920,
0x3030303030306630,0x09202020203b3030,0x74090a0d30202f2f,0x34762e64322e7865,
0x3233662e3233662e,0x252c313666257b20,0x333666252c323666,0x5b2c7d343666252c,
0x7b2c637253643274,0x3566252c37356625,0x252c393566252c38,0x0a0d3b5d7d303666,
0x706f6f6c3c2f2f20,0x6f2074726150203e,0x6220706f6f6c2066,0x656e696c2079646f,
0x6568202c34373220,0x6c6562616c206461,0x315f744c24206465,0x090a0d383138325f,
0x09343109636f6c2e,0x090a0d3009333832,0x203233662e766f6d,0x25202c3536662509,
0x6d090a0d3b313666,0x09203233662e766f,0x6625202c36366625,0x6f6d090a0d3b3236,
0x2509203233662e76,0x366625202c373666,0x6f6c2e090a0d3b33,0x3838320934310963,
0x766f6d090a0d3009,0x662509203233662e,0x3b316625202c3836,0x722e747663090a0d,
0x33732e3233662e6e,0x2c39366625092032,0x0a0d3b3034722520,0x3233662e766f6d09,
0x202c303766250920,0x3030303030306630,0x09202020203b3030,0x6d090a0d30202f2f,
0x09203233662e766f,0x6630202c31376625,0x3030303030303030,0x2f2f09202020203b,
0x786574090a0d3020,0x662e34762e64322e,0x7b203233662e3233,0x3766252c32376625,
0x252c343766252c33,0x32745b2c7d353766,0x66257b2c63725364,0x2c393666252c3836,
0x3766252c30376625,0x2f200a0d3b5d7d31,0x203e706f6f6c3c2f,0x20666f2074726150,
0x646f6220706f6f6c,0x3220656e696c2079,0x64616568202c3437,0x64656c6562616c20,
0x325f315f744c2420,0x6f6d090a0d383138,0x2509203233662e76,0x376625202c363766,
0x766f6d090a0d3b32,0x662509203233662e,0x33376625202c3737,0x2e766f6d090a0d3b,
0x3766250920323366,0x3b34376625202c38,0x09636f6c2e090a0d,0x3009393832093431,
0x662e766f6d090a0d,0x3937662509203233,0x0a0d3b316625202c,0x2e6e722e74766309,
0x203233732e323366,0x25202c3038662509,0x6d090a0d3b313472,0x09203233662e766f,
0x6630202c31386625,0x3030303030303030,0x2f2f09202020203b,0x766f6d090a0d3020,
0x662509203233662e,0x30306630202c3238,0x203b303030303030,0x30202f2f09202020,
0x322e786574090a0d,0x3233662e34762e64,0x66257b203233662e,0x2c343866252c3338,
0x3866252c35386625,0x536432745b2c7d36,0x393766257b2c6372,0x66252c303866252c,
0x7d323866252c3138,0x3c2f2f200a0d3b5d,0x6150203e706f6f6c,0x6f6c20666f207472,
0x2079646f6220706f,0x34373220656e696c,0x6c2064616568202c,0x242064656c656261,
0x3138325f315f744c,0x2e766f6d090a0d38,0x3866250920323366,0x3b33386625202c37,
0x662e766f6d090a0d,0x3838662509203233,0x0d3b34386625202c,0x33662e766f6d090a,
0x2c39386625092032,0x0a0d3b3538662520,0x343109636f6c2e09,0x0a0d300933393209,
0x3233732e64646109,0x202c343472250920,0x3b31202c32327225,0x662e766f6d090a0d,
0x3039662509203233,0x0a0d3b316625202c,0x3233732e6e696d09,0x202c353472250920,
0x7225202c38327225,0x756d090a0d3b3434,0x3233732e6f6c2e6c,0x202c363472250920,
0x7225202c32317225,0x6461090a0d3b3534,0x2509203233732e64,0x317225202c373472,
0x3b36347225202c35,0x722e747663090a0d,0x33732e3233662e6e,0x2c31396625092032,
0x0a0d3b3734722520,0x3233662e766f6d09,0x202c323966250920,0x3030303030306630,
0x09202020203b3030,0x6d090a0d30202f2f,0x09203233662e766f,0x6630202c33396625,
0x3030303030303030,0x2f2f09202020203b,0x786574090a0d3020,0x662e34762e64322e,
0x7b203233662e3233,0x3966252c34396625,0x252c363966252c35,0x32745b2c7d373966,
0x66257b2c63725364,0x2c313966252c3039,0x3966252c32396625,0x2f200a0d3b5d7d33,
0x203e706f6f6c3c2f,0x20666f2074726150,0x646f6220706f6f6c,0x3220656e696c2079,
0x64616568202c3437,0x64656c6562616c20,0x325f315f744c2420,0x6f6d090a0d383138,
0x2509203233662e76,0x396625202c383966,0x766f6d090a0d3b34,0x662509203233662e,
0x35396625202c3939,0x2e766f6d090a0d3b,0x3166250920323366,0x36396625202c3030,
0x2e766f6d090a0d3b,0x3472250920323375,0x090a0d3b30202c38,0x2e71652e70746573,
0x3470250920323373,0x202c32347225202c,0x090a0d3b38347225,0x6172622034702540,
0x5f315f744c240920,0x200a0d3b34373033,0x3e706f6f6c3c2f2f,0x666f207472615020,
0x6f6220706f6f6c20,0x20656e696c207964,0x616568202c343732,0x656c6562616c2064,
0x5f315f744c242064,0x2e090a0d38313832,0x3309343109636f6c,0x6d090a0d30093932,
0x09203233662e6c75,0x25202c3130316625,0x346625202c333166,0x646461090a0d3b34,
0x662509203233662e,0x356625202c323031,0x3b35366625202c34,0x61702e646c090a0d,
0x203233752e6d6172,0x5b202c3934722509,0x6170616475635f5f,0x5f38315a5f5f6d72,
0x69737546776f6c46,0x656e72656b5f6e6f,0x696969696969666c,0x6950616475633431,
0x7274506465686374,0x50747344635f5f53,0x7450646568637469,0x090a0d3b5d342b72,
0x752e6f6c2e6c756d,0x3035722509203233,0x202c32317225202c,0x090a0d3b39347225,
0x203233662e646461,0x202c333031662509,0x25202c3230316625,0x73090a0d3b363766,
0x09203233752e7268,0x7225202c31357225,0x0a0d3b34202c3035,0x2e6f6c2e6c756d09,
0x3572250920323375,0x202c377225202c32,0x7573090a0d3b3631,0x2509203233732e62,
0x327225202c333572,0x0d3b397225202c32,0x33662e646461090a,0x3430316625092032,
0x2c3330316625202c,0x0a0d3b3738662520,0x2e6f6c2e6c756d09,0x3572250920323375,
0x2c33357225202c34,0x0a0d3b3135722520,0x3233662e64646109,0x2c35303166250920,
0x202c343031662520,0x090a0d3b38396625,0x752e6f6c2e6c756d,0x3535722509203233,
0x202c34357225202c,0x756d090a0d3b3631,0x3233752e6f6c2e6c,0x202c363572250920,
0x7225202c36317225,0x6461090a0d3b3934,0x2509203233662e64,0x6625202c36303166,
0x326625202c353031,0x646461090a0d3b37,0x722509203233752e,0x32357225202c3735,
0x0d3b36357225202c,0x33662e766f6d090a,0x3730316625092032,0x306330346630202c,
0x2020203b30303030,0x090a0d36202f2f09,0x203233662e6c756d,0x202c383031662509,
0x25202c3730316625,0x73090a0d3b333166,0x09203233662e6275,0x25202c3930316625,
0x6625202c36303166,0x6c090a0d3b383031,0x2e6d617261702e64,0x3572250920323375,
0x75635f5f5b202c38,0x5f5f6d7261706164,0x776f6c465f38315a,0x6b5f6e6f69737546,
0x6969666c656e7265,0x7563343169696969,0x6568637469506164,0x635f5f5372745064,
0x6863746950747344,0x5d302b7274506465,0x2e646461090a0d3b,0x3572250920323375,
0x2c38357225202c39,0x0a0d3b3735722520,0x617261702e646c09,0x2509203233662e6d,
0x5f5b202c30313166,0x726170616475635f,0x465f38315a5f5f6d,0x6f69737546776f6c,
0x6c656e72656b5f6e,0x3169696969696966,0x7469506164756334,0x5372745064656863,
0x6e65747441665f5f,0x3b5d6e6f69746175,0x662e64616d090a0d,0x3131662509203233,
0x3031316625202c31,0x2c3930316625202c,0x0d3b313031662520,0x33752e646461090a,
0x2c30367225092032,0x25202c3535722520,0x61090a0d3b393572,0x09203233662e6464,
0x25202c3231316625,0x6625202c31313166,0x756d090a0d3b3134,0x2509203233662e6c,
0x6625202c33313166,0x34346625202c3431,0x2e646461090a0d3b,0x3166250920323366,
0x35356625202c3431,0x0d3b36366625202c,0x33662e646461090a,0x3531316625092032,
0x2c3431316625202c,0x0a0d3b3737662520,0x3233662e64646109,0x2c36313166250920,
0x202c353131662520,0x090a0d3b38386625,0x203233662e646461,0x202c373131662509,
0x25202c3631316625,0x61090a0d3b393966,0x09203233662e6464,0x25202c3831316625,
0x6625202c37313166,0x6f6d090a0d3b3832,0x2509203233662e76,0x6630202c39313166,
0x3030303030633034,0x202f2f092020203b,0x2e6c756d090a0d36,0x3166250920323366,
0x31316625202c3032,0x3b34316625202c39,0x662e627573090a0d,0x3231662509203233,
0x3831316625202c31,0x3b3032316625202c,0x662e64616d090a0d,0x3231662509203233,
0x3031316625202c32,0x2c3132316625202c,0x0d3b333131662520,0x33662e646461090a,
0x3332316625092032,0x2c3232316625202c,0x0a0d3b3234662520,0x3233662e6c756d09,
0x2c34323166250920,0x25202c3531662520,0x61090a0d3b343466,0x09203233662e6464,
0x25202c3532316625,0x366625202c363566,0x646461090a0d3b37,0x662509203233662e,
0x316625202c363231,0x38376625202c3532,0x2e646461090a0d3b,0x3166250920323366,
0x32316625202c3732,0x3b39386625202c36,0x662e646461090a0d,0x3231662509203233,
0x3732316625202c38,0x3b3030316625202c,0x662e646461090a0d,0x3231662509203233,
0x3832316625202c39,0x0d3b39326625202c,0x33662e766f6d090a,0x3033316625092032,
0x306330346630202c,0x2020203b30303030,0x090a0d36202f2f09,0x203233662e6c756d,
0x202c313331662509,0x25202c3033316625,0x73090a0d3b353166,0x09203233662e6275,
0x25202c3233316625,0x6625202c39323166,0x6d090a0d3b313331,0x09203233662e6461,
0x25202c3333316625,0x6625202c30313166,0x316625202c323331,0x6461090a0d3b3432,
0x2509203233662e64,0x6625202c34333166,0x346625202c333331,0x766f6d090a0d3b33,
0x662509203233662e,0x306630202c353331,0x3b30303030303030,0x30202f2f09202020,
0x6c672e7473090a0d,0x2e34762e6c61626f,0x72255b0920323366,0x7b202c5d302b3036,
0x66252c3231316625,0x333166252c333231,0x7d35333166252c34,0x636f6c2e090a0d3b,
0x0938333309343109,0x2e627573090a0d30,0x3166250920323366,0x33316625202c3633,
0x3b3231316625202c,0x662e736261090a0d,0x3331662509203233,0x3633316625202c37,
0x2e627573090a0d3b,0x3166250920323366,0x34316625202c3833,0x3b3332316625202c,
0x662e736261090a0d,0x3331662509203233,0x3833316625202c39,0x2e627573090a0d3b,
0x3166250920323366,0x35316625202c3034,0x3b3433316625202c,0x662e736261090a0d,
0x3431662509203233,0x3034316625202c31,0x2e78616d090a0d3b,0x3166250920323366,
0x33316625202c3234,0x3134316625202c39,0x2e78616d090a0d3b,0x3166250920323366,
0x33316625202c3334,0x3234316625202c37,0x636f6c2e090a0d3b,0x0939333309343109,
0x702e646c090a0d30,0x3233752e6d617261,0x202c313672250920,0x70616475635f5f5b,
0x38315a5f5f6d7261,0x737546776f6c465f,0x6e72656b5f6e6f69,0x6969696969666c65,
0x5061647563343169,0x7450646568637469,0x727245635f5f5372,0x656863746950726f,
0x3b5d342b72745064,0x61702e646c090a0d,0x203233752e6d6172,0x5b202c3236722509,
0x6170616475635f5f,0x5f38315a5f5f6d72,0x69737546776f6c46,0x656e72656b5f6e6f,
0x696969696969666c,0x6950616475633431,0x7274506465686374,0x6f727245635f5f53,
0x6465686374695072,0x0d3b5d302b727450,0x6f6c2e6c756d090a,0x722509203233752e,
0x32317225202c3336,0x0d3b31367225202c,0x33752e726873090a,0x2c34367225092032,
0x32202c3336722520,0x2e6c756d090a0d3b,0x09203233752e6f6c,0x7225202c35367225,
0x34367225202c3335,0x2e6c756d090a0d3b,0x09203233752e6f6c,0x7225202c36367225,
0x0a0d3b34202c3536,0x3233752e64646109,0x202c373672250920,0x7225202c32367225,
0x756d090a0d3b3636,0x3233752e6f6c2e6c,0x202c383672250920,0x7225202c36317225,
0x756d090a0d3b3136,0x3233752e6f6c2e6c,0x202c393672250920,0x0d3b34202c377225,
0x33752e646461090a,0x2c30377225092032,0x25202c3836722520,0x61090a0d3b393672,
0x09203233752e6464,0x7225202c31377225,0x30377225202c3736,0x672e7473090a0d3b,
0x33662e6c61626f6c,0x313772255b092032,0x316625202c5d302b,0x744c240a0d3b3334,
0x3a343730335f315f,0x6f6c3c2f2f200a0d,0x74726150203e706f,0x706f6f6c20666f20,
0x696c2079646f6220,0x202c34373220656e,0x62616c2064616568,0x744c242064656c65,
0x0d383138325f315f,0x3109636f6c2e090a,0x0d30093437320934,0x33662e766f6d090a,
0x2c37326625092032,0x0a0d3b3331662520,0x3233662e766f6d09,0x202c383266250920,
0x090a0d3b34316625,0x203233662e766f6d,0x25202c3932662509,0x6d090a0d3b353166,
0x09203233662e766f,0x6625202c33316625,0x6f6d090a0d3b3839,0x2509203233662e76,
0x396625202c343166,0x766f6d090a0d3b39,0x662509203233662e,0x30316625202c3531,
0x766f6d090a0d3b30,0x722509203233732e,0x34347225202c3232,0x2e646461090a0d3b,
0x3472250920323373,0x2c31347225202c31,0x0a0d3b3231722520,0x3233732e64646109,
0x202c303472250920,0x7225202c30347225,0x6461090a0d3b3231,0x2509203233732e64,
0x337225202c333372,0x3b32317225202c33,0x2e70746573090a0d,0x09203233732e656e,
0x327225202c357025,0x3b34347225202c35,0x2035702540090a0d,0x744c240920617262,
0x3b383138325f315f,0x5f315f744c240a0d,0x090a0d3a36303332,0x09343109636f6c2e,
0x090a0d3009343533,0x240a0d3b74697865,0x5f5f646e6557444c,0x776f6c465f38315a,
0x6b5f6e6f69737546,0x6969666c656e7265,0x7563343169696969,0x6568637469506164,
0x0d3a5f5372745064,0x5f202f2f207d090a,0x776f6c465f38315a,0x6b5f6e6f69737546,
0x6969666c656e7265,0x7563343169696969,0x6568637469506164,0x0a0d5f5372745064,
0x0000000000000a0d
};


}
#line 791 "e:\\project\\flowentropy\\entropy_final_2d\\myinterface\\FlowFusion.fatbin.c"

static __cudaFatPtxEntry   __ptxEntries  [] = {{(char*)"compute_10",(char*)__deviceText_$compute_10$},{0,0}};
static __cudaFatCubinEntry __cubinEntries[] = {{(char*)"sm_10",(char*)__deviceText_$sm_10$},{0,0}};
static __cudaFatDebugEntry __debugEntries0 = {0, 0, 0, 0} ;


#pragma data_seg(".nvFatBinSegment")
#pragma data_seg()

__declspec(allocate(".nvFatBinSegment")) static __cudaFatCudaBinary __fatDeviceText= {0x1ee55a01,0x00000003,0x8ecc680c,(char*)"4a2b5f85e8cd53b9",(char*)"e:/project/FlowEntropy/FlowFusion/FlowFusion_test/FlowFusion.cu",(char*)"",__ptxEntries,__cubinEntries,&__debugEntries0,0,0,0,0,0,0x9bb844a7};

#line 5 "e:\\project\\flowentropy\\entropy_final_2d\\myinterface\\FlowFusion.cudafe1.stub.c"
#line 1 "C:\\CUDA\\include\\crt/host_runtime.h"













































































































#line 111 "C:\\CUDA\\include\\crt/host_runtime.h"

#line 1 "C:\\CUDA\\include\\host_defines.h"




































































































































#line 134 "C:\\CUDA\\include\\host_defines.h"
#line 113 "C:\\CUDA\\include\\crt/host_runtime.h"


























extern "C" {
#line 141 "C:\\CUDA\\include\\crt/host_runtime.h"

extern void** __stdcall __cudaRegisterFatBinary(
  void *fatCubin
);

extern void __stdcall __cudaUnregisterFatBinary(
  void **fatCubinHandle
);

extern void __stdcall __cudaRegisterVar(
        void **fatCubinHandle,
        char  *hostVar,
        char  *deviceAddress,
  const char  *deviceName,
        int    ext,
        int    size,
        int    constant,
        int    global
);

extern void __stdcall __cudaRegisterTexture(
        void                    **fatCubinHandle,
  const struct textureReference  *hostVar,
  const void                    **deviceAddress,
  const char                     *deviceName,
        int                       dim,       
        int                       norm,      
        int                       ext        
);

extern void __stdcall __cudaRegisterSurface(
        void                    **fatCubinHandle,
  const struct surfaceReference  *hostVar,
  const void                    **deviceAddress,
  const char                     *deviceName,
        int                       dim,       
        int                       ext        
);

extern void __stdcall __cudaRegisterShared(
  void **fatCubinHandle,
  void **devicePtr
);

extern void __stdcall __cudaRegisterSharedVar(
  void   **fatCubinHandle,
  void   **devicePtr,
  size_t   size,
  size_t   alignment,
  int      storage
);

extern void __stdcall __cudaRegisterFunction(
        void   **fatCubinHandle,
  const char    *hostFun,
        char    *deviceFun,
  const char    *deviceName,
        int      thread_limit,
        uint3   *tid,
        uint3   *bid,
        dim3    *bDim,
        dim3    *gDim,
        int     *wSize
);


}
#line 209 "C:\\CUDA\\include\\crt/host_runtime.h"





#line 215 "C:\\CUDA\\include\\crt/host_runtime.h"

extern int atexit(void(*)(void));

#line 219 "C:\\CUDA\\include\\crt/host_runtime.h"

static void **__cudaFatCubinHandle;

static void __cudaUnregisterBinaryUtil(void)
{
  __cudaUnregisterFatBinary(__cudaFatCubinHandle);
}



































#line 262 "C:\\CUDA\\include\\crt/host_runtime.h"












#line 1 "C:\\CUDA\\include\\common_functions.h"

































































#line 67 "C:\\CUDA\\include\\common_functions.h"

#line 1 "c:\\cuda\\include\\crt/func_macro.h"













































#line 47 "c:\\cuda\\include\\crt/func_macro.h"





#line 53 "c:\\cuda\\include\\crt/func_macro.h"










#line 64 "c:\\cuda\\include\\crt/func_macro.h"






#line 71 "c:\\cuda\\include\\crt/func_macro.h"








#line 80 "c:\\cuda\\include\\crt/func_macro.h"






#line 87 "c:\\cuda\\include\\crt/func_macro.h"






#line 94 "c:\\cuda\\include\\crt/func_macro.h"

#line 96 "c:\\cuda\\include\\crt/func_macro.h"

#line 98 "c:\\cuda\\include\\crt/func_macro.h"
#line 69 "C:\\CUDA\\include\\common_functions.h"

static clock_t __cuda_clock(void)
{
  return clock();
}

static void *__cuda_memset(void *s, int c, size_t n)
{
  return memset(s, c, n);
}

static void *__cuda_memcpy(void *d, const void *s, size_t n)
{
  return memcpy(d, s, n);
}

#line 86 "C:\\CUDA\\include\\common_functions.h"







#line 1 "c:\\cuda\\include\\math_functions.h"






































































































































































































































































































































































































































































































































































































































































































































































































































































































































#line 904 "c:\\cuda\\include\\math_functions.h"

#line 1 "c:\\cuda\\include\\crt/func_macro.h"
































































































#line 98 "c:\\cuda\\include\\crt/func_macro.h"
#line 906 "c:\\cuda\\include\\math_functions.h"



























#line 934 "c:\\cuda\\include\\math_functions.h"







static double log2(double a)
{
  return log(a) / log(2.0);
}

static float log2f(float a)
{
  return (float)log2((double)a);
}

static double exp2(double a)
{
  return pow(2.0, a);
}

static float exp2f(float a)
{
  return (float)exp2((double)a);
}

static long long int llabs(long long int a)
{
  return a < 0ll ? -a : a;
}



#line 969 "c:\\cuda\\include\\math_functions.h"

#line 971 "c:\\cuda\\include\\math_functions.h"

static int __cuda_abs(int a)
{
  return abs(a);
}

static float __cuda_fabsf(float a)
{
  return fabsf(a);
}

static long long int __cuda_llabs(long long int a)
{

  return ::llabs(a);


#line 989 "c:\\cuda\\include\\math_functions.h"
}

static float __cuda_exp2f(float a)
{
  return exp2f(a);
}

#line 1 "c:\\cuda\\include\\device_functions.h"




















































































































































































































































































































































































































































#line 438 "c:\\cuda\\include\\device_functions.h"

#line 1 "c:\\cuda\\include\\crt/func_macro.h"
































































































#line 98 "c:\\cuda\\include\\crt/func_macro.h"
#line 440 "c:\\cuda\\include\\device_functions.h"

#line 1 "c:\\cuda\\include\\host_defines.h"




































































































































#line 134 "c:\\cuda\\include\\host_defines.h"
#line 442 "c:\\cuda\\include\\device_functions.h"
#line 1 "c:\\cuda\\include\\math_constants.h"
















































































#line 82 "c:\\cuda\\include\\math_constants.h"






#line 89 "c:\\cuda\\include\\math_constants.h"



















































#line 141 "c:\\cuda\\include\\math_constants.h"
#line 443 "c:\\cuda\\include\\device_functions.h"



















































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































#line 2383 "c:\\cuda\\include\\device_functions.h"

#line 1 "c:\\cuda\\include\\common_types.h"




































union __cudart_FloatUintCvt {
    float f;
    unsigned int i;
};
 
union __cudart_FloatIntCvt {
    float f;
    int i;
};

union __cudart_DoubleUlonglongCvt {
    double d;
    unsigned long long int i;
};

union __cudart_DoubleLonglongCvt {
    double d;
    long long int i;
};

union __cudart_DoubleInthiloCvt {
    double     d;
    signed int i[2];
};

struct __cudart_UintUint {
    unsigned int lo;
    unsigned int hi;
};

#line 68 "c:\\cuda\\include\\common_types.h"
#line 2385 "c:\\cuda\\include\\device_functions.h"

static  const unsigned char __internal_rcpTab[128] = 
{
  0xff, 0xfd, 0xfb, 0xf9, 0xf7, 0xf5, 0xf4, 0xf2, 
  0xf0, 0xee, 0xed, 0xeb, 0xe9, 0xe8, 0xe6, 0xe4,
  0xe3, 0xe1, 0xe0, 0xde, 0xdd, 0xdb, 0xda, 0xd8, 
  0xd7, 0xd5, 0xd4, 0xd3, 0xd1, 0xd0, 0xcf, 0xcd,
  0xcc, 0xcb, 0xca, 0xc8, 0xc7, 0xc6, 0xc5, 0xc4, 
  0xc2, 0xc1, 0xc0, 0xbf, 0xbe, 0xbd, 0xbc, 0xbb,
  0xba, 0xb9, 0xb8, 0xb7, 0xb6, 0xb5, 0xb4, 0xb3, 
  0xb2, 0xb1, 0xb0, 0xaf, 0xae, 0xad, 0xac, 0xab,
  0xaa, 0xa9, 0xa8, 0xa8, 0xa7, 0xa6, 0xa5, 0xa4, 
  0xa3, 0xa3, 0xa2, 0xa1, 0xa0, 0x9f, 0x9f, 0x9e,
  0x9d, 0x9c, 0x9c, 0x9b, 0x9a, 0x99, 0x99, 0x98, 
  0x97, 0x97, 0x96, 0x95, 0x95, 0x94, 0x93, 0x93,
  0x92, 0x91, 0x91, 0x90, 0x8f, 0x8f, 0x8e, 0x8e, 
  0x8d, 0x8c, 0x8c, 0x8b, 0x8b, 0x8a, 0x89, 0x89,
  0x88, 0x88, 0x87, 0x87, 0x86, 0x85, 0x85, 0x84, 
  0x84, 0x83, 0x83, 0x82, 0x82, 0x81, 0x81, 0x80
};

static  const unsigned int __internal_invSqrtCubeTab[96] =
{ 
  0xfa0bf8fe, 0xee6b28fa, 0xe5f024f7, 0xdaf268f3,
  0xd2f000f0, 0xc890c0ec, 0xc10378e9, 0xb9a758e6,
  0xb4da40e4, 0xadcea0e1, 0xa6f278de, 0xa279c0dc,
  0x9beb48d9, 0x97a5c4d7, 0x916340d4, 0x8d4fc8d2,
  0x895000d0, 0x8563b8ce, 0x818ac0cc, 0x7dc4e8ca,
  0x7a1200c8, 0x7671d8c6, 0x72e440c4, 0x6f6908c2,
  0x6db240c1, 0x6a523cbf, 0x670424bd, 0x6563c0bc,
  0x623028ba, 0x609ce8b9, 0x5d8364b7, 0x5bfd18b6,
  0x58fd40b4, 0x5783a8b3, 0x560e48b2, 0x533000b0,
  0x51c70caf, 0x506238ae, 0x4da4c0ac, 0x4c4c10ab,
  0x4af768aa, 0x49a6b8a9, 0x485a00a8, 0x471134a7,
  0x45cc58a6, 0x434e40a4, 0x4214f8a3, 0x40df88a2,
  0x3fade0a1, 0x3e8000a0, 0x3d55dc9f, 0x3c2f789e,
  0x3c2f789e, 0x3b0cc49d, 0x39edc09c, 0x38d2609b,
  0x37baa89a, 0x36a68899, 0x35960098, 0x34890497,
  0x34890497, 0x337f9896, 0x3279ac95, 0x31774094,
  0x30784893, 0x30784893, 0x2f7cc892, 0x2e84b091,
  0x2d900090, 0x2d900090, 0x2c9eac8f, 0x2bb0b88e,
  0x2bb0b88e, 0x2ac6148d, 0x29dec08c, 0x29dec08c,
  0x28fab08b, 0x2819e88a, 0x2819e88a, 0x273c5889,
  0x273c5889, 0x26620088, 0x258ad487, 0x258ad487,
  0x24b6d886, 0x24b6d886, 0x23e5fc85, 0x23184084,
  0x23184084, 0x224d9883, 0x224d9883, 0x21860882,
  0x21860882, 0x20c18081, 0x20c18081, 0x20000080
};

static float __internal_frcp_kernel (float x,enum cudaRoundMode mode)
{
  unsigned long long prod;
  volatile union __cudart_FloatUintCvt arg;
  unsigned int expo;
  unsigned int sign;
  unsigned f, y;
    
  arg.f = x;
  sign = arg.i & 0x80000000;
  expo = (arg.i >> 23);
  expo = expo & 0xff;
  f = expo - 1;
    
  if (f <= 0xFD) {
    y = (arg.i << 8);
    y = y | 0x80000000;
    
    arg.i = __internal_rcpTab[(y >> 24) - 128];
    
    f = arg.i * arg.i;
    f = f << 16;
    prod = ((unsigned long long)y) * f;
    arg.i = (arg.i << 24) - (unsigned)(prod >> 32);
      
    f = arg.i + arg.i;
    prod = ((unsigned long long)y) * f;
    f = (unsigned)(-(int)(prod >> 32));
    prod = ((unsigned long long)arg.i) * f;
    y = y >> 8;
    
    expo = (2 * 127) - expo - 2;
    arg.i = (unsigned)(prod >> 32);
    if (mode == cudaRoundNearest) {
      arg.i = arg.i >> 6;
    } else {
      arg.i = (arg.i + 32) >> 6;
    }
    if ((int)expo >= 0) {
      f = y * arg.i;
      arg.i = ((expo << 23) + arg.i) | sign;
    } else {
      
      expo = -(int)expo;
      arg.i = arg.i >> expo;
      f = y * arg.i;
      arg.i = arg.i | sign;
    }
    if (mode == cudaRoundNearest) {
      expo = f + y;
      if ((int)f < 0) f = (unsigned)(-(int)f);
      if ((int)expo < 0) expo = (unsigned)(-(int)expo);
      if (expo < f) arg.i++;
    } else if (mode == cudaRoundZero) {
      if ((int)f > 0) arg.i = arg.i - 1;
    } else if (mode == cudaRoundPosInf) {
      if (((int)f > 0) &&  sign) arg.i = arg.i - 1;
      if (((int)f < 0) && !sign) arg.i = arg.i + 1;
    } else { 
      if (((int)f > 0) && !sign) arg.i = arg.i - 1;
      if (((int)f < 0) &&  sign) arg.i = arg.i + 1;
    }
    return arg.f;
  } else {
    
    if (!(arg.i << 1)) {
      arg.i = 0x7F800000 | arg.i;
      return arg.f;
    } 
    
    if ((arg.i << 1) == 0xff000000) {
      arg.i &= 0x80000000;
      return arg.f;
    }
    
    if ((arg.i << 1) > 0xff000000) {
      arg.i |= 0x00400000;
      return arg.f;
    }
    
    f = 0;
    arg.i <<= 8;
    do {
      f++;
      arg.i <<= 1;
    } while ((int)arg.i > 0);
    arg.i >>= 8;
    arg.i |= sign;
    arg.f = __internal_frcp_kernel (arg.f, mode);
    expo = ((arg.i << 1) >> 24);
    if ((expo + f) < 255) {
      arg.i = (arg.i + (f << 23));
      return arg.f;
    } 
    if (mode == cudaRoundNearest) {
      arg.i = (arg.i & 0x80000000) | 0x7f800000;
    } else if (mode == cudaRoundZero) {
      arg.i = (arg.i & 0x80000000) | 0x7f7fffff;
    } else if (mode == cudaRoundPosInf) {
      arg.i = (arg.i & 0x80000000) | ((sign) ? 0x7f7fffff : 0x7f800000);
    } else { 
      arg.i = (arg.i & 0x80000000) | ((sign) ? 0x7f800000 : 0x7f7fffff);
    }
    return arg.f;
  }
}

static float __internal_fsqrt_kernel (float radicand, enum cudaRoundMode mode)
#line 2543 "c:\\cuda\\include\\device_functions.h"
{
  unsigned long long prod;
  volatile union __cudart_FloatUintCvt arg;
  unsigned int expo;
  unsigned int s, f, x;

  arg.f = radicand;
  expo = arg.i >> 23;
  expo = expo & 0xff;
  f = expo - 1;

  if ((arg.i <= 0x80000000) && (f <= 0xFD)) {
    
    x = (arg.i << 8) | 0x80000000;
    x = x >> (expo & 1);
    
    arg.i = f = __internal_invSqrtCubeTab[((unsigned)x >> 25) - 32];
    
    prod = ((unsigned long long)x) * f;
    arg.i = ((arg.i * 3) << 22) - (unsigned)(prod >> 32);
    
    prod = ((unsigned long long)arg.i) * arg.i;
    s = (unsigned)(prod >> 32);
    prod = ((unsigned long long)x) * s;
    f = 0x30000000 - (unsigned)(prod >> 32);
    prod = ((unsigned long long)f) * arg.i; 
    arg.i = (unsigned)(prod >> 32);
    
    prod = ((unsigned long long)x) * arg.i;
    arg.i = (unsigned)(prod >> 32);
    if (mode == cudaRoundNearest) {
      arg.i = arg.i >> 3;
    } else {
      arg.i = (arg.i + 4) >> 3;
    }
    x = (x << 16) - (arg.i * arg.i);
    
    if (mode == cudaRoundNearest) {
      f = x - (2 * arg.i + 1);
      if ((int)f < 0) f = (unsigned)(-(int)f);
      if ((int)x < 0) x = (unsigned)(-(int)x);
      if (f < x) arg.i ++;
    } else if ((mode == cudaRoundZero) || (mode == cudaRoundMinInf)) {
      if ((int)x < 0) arg.i--;
    } else if (mode == cudaRoundPosInf) {
      if ((int)x > 0) arg.i++;
    }
    arg.i = arg.i + (((expo + 125) & ~0x1) << 22);
    return arg.f;
  } else {
    
    if (!(arg.i << 1) || (arg.i == 0x7F800000)) {
      return arg.f;
    } 
    
    if ((arg.i << 1) > 0xFF000000) {
      arg.i |= 0x00400000;
      return arg.f;
    }
    
    if (arg.i & 0x80000000) {
      arg.i = 0xFFC00000;
      return arg.f;
    } 
    
    x = 0;
    arg.i <<= 8;
    do {
      x++;
      arg.i <<= 1;
    } while ((int)arg.i > 0);
    arg.i >>= 8;
    arg.i += (x & 1) << 23;
    x     += (x & 1);
    arg.f = __internal_fsqrt_kernel (arg.f, mode);
    arg.i -= ((x >> 1) << 23);
    return arg.f;
  }
}

static float __internal_fdiv_kernel (float dividend, float divisor, enum cudaRoundMode mode)
#line 2625 "c:\\cuda\\include\\device_functions.h"
{
  unsigned long long prod;
  unsigned r, f, x, y, expox, expoy, sign;
  volatile union __cudart_FloatUintCvt cvtx, cvty, res;

  cvtx.f = dividend;
  cvty.f = divisor;
  expox = ((cvtx.i >> 23) & 0xff) - 1;
  expoy = ((cvty.i >> 23) & 0xff) - 1;
  sign  = ((cvtx.i ^ cvty.i) & 0x80000000);

  if ((expox <= 0xFD) && (expoy <= 0xFD)) {
divide:
    expox = expox - expoy + 127 - 1;
    expoy = expox;
    
    y = (cvty.i << 8) | 0x80000000;
    x = (cvtx.i & 0x00ffffff) | 0x00800000;
    
    r = __internal_rcpTab[(y >> 24) - 128];
    
    f = r * r;
    prod = ((unsigned long long)y) * (f << 16);
    r = (r << 24) - (unsigned)(prod >> 32);
    
    prod = ((unsigned long long)y) * (r << 1);
    f = (unsigned)-(int)(prod >> 32);
    prod = ((unsigned long long)f) * (r << 1);
    r = (unsigned)(prod >> 32);
    
    prod = ((unsigned long long)x) * (r << 1);
    
    if (((int)((prod >> 32) << 8)) > 0) {
      expox--;
      prod = prod + prod;
    }
    if (mode == cudaRoundNearest) {
      
      r = (unsigned)(prod >> 32);
      y = y >> 8;
      
      if (expox <= 0xFD) {
        int rem0, rem1, inc;
        
        prod = ((unsigned long long)y) * r;
        x = x << (23 + ((prod >> 32) >> 15));
        rem1 = x - (unsigned)(prod & 0xffffffff);
        rem0 = rem1 - y;
        inc = abs(rem0) < abs(rem1);
        
        res.i = sign | ((expox << 23) + r + inc);
        return res.f;
      } else if ((int)expox >= 254) {
        
        res.i = sign | 0x7f800000;
        return res.f;
      } else {
        
        int shift = -(int)expox;
        if (shift > 23) {
          
          r = (shift < 25) && ((x != y) || (r > 0x00ff0000));
          res.i = sign | r;
          return res.f;
        } 
        if (x == y) {
          
          shift = -(int)expoy;
          r = 0x00800000 >> shift;
          res.i = sign | r;
          return res.f;
        }
        {
          unsigned long long tempx;
          long long remlo, remhi;
          
          r = r >> shift;
          prod  = ((unsigned long long)y) * r;
          tempx = ((unsigned long long)x) << (23 - shift);
          remlo = 2 * tempx - 2 * prod - y;
          remhi = remlo + 2 * tempx;
          if (remlo < 0) remlo = -remlo;
          if (remhi < 0) remhi = -remhi;
          if (remhi < remlo) tempx = 2 * tempx;
          remlo = tempx - prod;
          remhi = remlo - y;
          if (remlo < 0) remlo = -remlo;
          if (remhi < 0) remhi = -remhi;
          if ((remhi < remlo) || ((remhi == remlo) && (r & 1))) r++;
          res.i = sign | r;
          return res.f;
        }
      }
    } else if (mode == cudaRoundZero) {
      
      prod += 0x0000000080000000ULL;
      r = (unsigned)(prod >> 32);
      y = y >> 8;
      
      if (expox <= 0xFD) {
        int rem1;
        prod = ((unsigned long long)y) * r;
        x = x << (23 + ((prod >> 32) >> 15));
        rem1 = x - (unsigned)(prod & 0xffffffff);
        if (rem1 < 0) r--;
        r = (expox << 23) + r;
        if (r == 0x7f800000) r = 0x7f7fffff;
        res.i = sign | r;
        return res.f;
      } else if ((int)expox >= 254) {
        
        res.i = sign | 0x7f7fffff;
        return res.f;
      } else {
        
        int shift = -(int)expox;
        if ((x == y) && (shift < 31)) {
          shift = -(int)expoy;
          r = 0x00800000 >> shift;
          res.i = sign | r;
          return res.f;
        }
        if (shift > 23) {
          r = 0;
          res.i = sign | r;
          return res.f;
        } 
        {
          unsigned long long tempx;
          long long remlo, remhi;
          
          r = r >> shift;
          prod  = ((unsigned long long)y) * r;
          tempx = ((unsigned long long)x) << (23 - shift);
          remlo = 2 * tempx - 2 * prod - y;
          remhi = remlo + 2 * tempx;
          if (remlo < 0) remlo = -remlo;
          if (remhi < 0) remhi = -remhi;
          if (remhi < remlo) tempx = 2 * tempx;
          remlo = tempx - prod;
          if ((remlo < 0) & (r != 0)) r--;
          res.i = sign | r;
          return res.f;
        }
      }
    } else if (mode == cudaRoundPosInf) {
      
      prod += 0x0000000080000000ULL;
      r = (unsigned)(prod >> 32);
      y = y >> 8;
      
      if (expox <= 0xFD) {
        int rem1;
        prod = ((unsigned long long)y) * r;
        x = x << (23 + ((prod >> 32) >> 15));
        rem1 = x - (unsigned)(prod & 0xffffffff);
        if ((rem1 < 0) &&  (sign)) r--;
        if ((rem1 > 0) && (!sign)) r++;
        r = (expox << 23) + r;
        if ((r == 0x7f800000) && (sign)) r = 0x7f7fffff;
        res.i = sign | r;
        return res.f;
      } else if ((int)expox >= 254) {
        
        r = sign ? 0x7f7fffff : 0x7f800000;
        res.i = sign | r;
        return res.f;
      } else {
        
        int shift = -(int)expox;
        if ((x == y) && (shift <= 24)) {
          shift = -(int)expoy;
          r = 0x00800000 >> shift;
          if (r == 0) r = !sign; 
          res.i = sign | r;
          return res.f;
        }
        if (shift > 23) {
          r = !sign; 
          res.i = sign | r;
          return res.f;
        } 
        {
          unsigned long long tempx;
          long long remlo, remhi;
          
          r = r >> shift;
          prod  = ((unsigned long long)y) * r;
          tempx = ((unsigned long long)x) << (23 - shift);
          remlo = 2 * tempx - 2 * prod - y;
          remhi = remlo + 2 * tempx;
          if (remlo < 0) remlo = -remlo;
          if (remhi < 0) remhi = -remhi;
          if (remhi < remlo) tempx = 2 * tempx;
          remlo = tempx - prod;
          if ((remlo < 0) && (r != 0) &&  (sign)) r--;
          if ((remlo > 0) &&             (!sign)) r++;
          res.i = sign | r;
          return res.f;
        }
      }
    } else if (mode == cudaRoundMinInf) {
      
      prod += 0x0000000080000000ULL;
      r = (unsigned)(prod >> 32);
      y = y >> 8;
      
      if (expox <= 0xFD) {
        int rem1;
        prod = ((unsigned long long)y) * r;
        x = x << (23 + ((prod >> 32) >> 15));
        rem1 = x - (unsigned)(prod & 0xffffffff);
        if ((rem1 < 0) && (!sign)) r--;
        if ((rem1 > 0) &&  (sign)) r++;
        r = (expox << 23) + r;
        if ((r == 0x7f800000) && (!sign)) r = 0x7f7fffff;
        res.i = sign | r;
        return res.f;
      } else if ((int)expox >= 254) {
        
        r = sign ? 0x7f800000 : 0x7f7fffff;
        res.i = sign | r;
        return res.f;
      } else {
        
        int shift = -(int)expox;
        if ((x == y) && (shift <= 24)) {
          shift = -(int)expoy;
          r = 0x00800000 >> shift;
          if (r == 0) r = !!sign; 
          res.i = sign | r;
          return res.f;
        }
        if (shift > 23) {
          r = !!sign; 
          res.i = sign | r;
          return res.f;
        } 
        {
          unsigned long long tempx;
          long long remlo, remhi;
          
          r = r >> shift;
          prod  = ((unsigned long long)y) * r;
          tempx = ((unsigned long long)x) << (23 - shift);
          remlo = 2 * tempx - 2 * prod - y;
          remhi = remlo + 2 * tempx;
          if (remlo < 0) remlo = -remlo;
          if (remhi < 0) remhi = -remhi;
          if (remhi < remlo) tempx = 2 * tempx;
          remlo = tempx - prod;
          if ((remlo < 0) && (r != 0) && (!sign)) r--;
          if ((remlo > 0) &&              (sign)) r++;
          res.i = sign | r;
          return res.f;
        }
      }
    }
  }
  {
    int xzero, yzero, xinf, yinf, xnan, ynan;
    
    xnan = (cvtx.i << 1) > 0xff000000;
    ynan = (cvty.i << 1) > 0xff000000;
    
    if (xnan) {
      res.i = cvtx.i | 0x00400000;
      return res.f;
    }
    if (ynan) {
      res.i = cvty.i | 0x00400000;
      return res.f;
    }
    xzero = (cvtx.i << 1) == 0x00000000;
    yzero = (cvty.i << 1) == 0x00000000;
    xinf  = (cvtx.i << 1) == 0xff000000;
    yinf  = (cvty.i << 1) == 0xff000000;
    
    if ((xzero & yzero) | (xinf & yinf)) {
      res.i = 0xffc00000;
      return res.f;
    }
    
    if (xzero | yinf) {
      res.i = sign;
      return res.f;
    }
    
    if (yzero | xinf) {
      res.i = sign | 0x7f800000;
      return res.f;
    }
    
    if ((int)expox < 0) {
      cvtx.i = cvtx.i << 9;
      while ((int)cvtx.i >= 0) {
        expox--;
        cvtx.i = cvtx.i + cvtx.i;
      }
      cvtx.i = cvtx.i >> 8;
    }
    if ((int)expoy < 0) {
      cvty.i = cvty.i << 9;
      while ((int)cvty.i >= 0) {
        expoy--;
        cvty.i = cvty.i + cvty.i;
      }
      cvty.i = cvty.i >> 8;
    }
    goto divide;
  }
}

static float __internal_fmul_kernel (float a, float b, enum cudaRoundMode mode)
#line 2940 "c:\\cuda\\include\\device_functions.h"
{
  unsigned long long product;
  volatile union __cudart_FloatUintCvt xx, yy;
  unsigned expo_x, expo_y;
    
  xx.f = a;
  yy.f = b;

  expo_y = 0xFF;
  expo_x = expo_y & (xx.i >> 23);
  expo_x = expo_x - 1;
  expo_y = expo_y & (yy.i >> 23);
  expo_y = expo_y - 1;
    
  if ((expo_x <= 0xFD) && 
      (expo_y <= 0xFD)) {
multiply:
    expo_x = expo_x + expo_y;
    expo_y = xx.i ^ yy.i;
    xx.i = xx.i & 0x00ffffff;
    yy.i = yy.i << 8;
    xx.i = xx.i | 0x00800000;
    yy.i = yy.i | 0x80000000;
    
    product = ((unsigned long long)xx.i) * yy.i;
    expo_x = expo_x - 127 + 2;
    expo_y = expo_y & 0x80000000;
    xx.i = (unsigned int)(product >> 32);
    yy.i = (unsigned int)(product & 0xffffffff);
    
    if (xx.i < 0x00800000) {
      xx.i = (xx.i << 1) | (yy.i >> 31);
      yy.i = (yy.i << 1);
      expo_x--;
    }
    if (expo_x <= 0xFD) {
      xx.i = xx.i | expo_y;          
      xx.i = xx.i + (expo_x << 23);  
      
      if (mode == cudaRoundNearest) {
        if (yy.i < 0x80000000) return xx.f;
        xx.i += ((yy.i == 0x80000000) ? (xx.i & 1) : (yy.i >> 31));
      } else if (mode == cudaRoundZero) {          
      } else if (mode == cudaRoundPosInf) {
        xx.i += (yy.i && !expo_y);
      } else if (mode == cudaRoundMinInf) {
        xx.i += (yy.i && expo_y);
      }
      return xx.f;    
    } else if ((int)expo_x >= 254) {
      
      if (mode == cudaRoundNearest) {
         xx.i = expo_y | 0x7F800000;
      } else if (mode == cudaRoundZero) {
         xx.i = expo_y | 0x7F7FFFFF;
      } else if (mode == cudaRoundPosInf) {
         xx.i = (expo_y ? 0xff7fffff : 0x7F800000);
      } else { 
         xx.i = (expo_y ? 0xFF800000 : 0x7f7fffff);
      }
      return xx.f;
    } else {
      
      expo_x = ((unsigned int)-((int)expo_x));
      if (mode == cudaRoundNearest) {
        if (expo_x > 25) {
          
          xx.i = expo_y;
          return xx.f;
        } else {
          yy.i = (xx.i << (32 - expo_x)) | ((yy.i) ? 1 : 0);
          xx.i = expo_y + (xx.i >> expo_x);
          xx.i += ((yy.i == 0x80000000) ? (xx.i & 1) : (yy.i >> 31));
          return xx.f;
        }
      } else if (mode == cudaRoundZero) {
        if (expo_x > 25) expo_x = 25; 
        xx.i = expo_y + (xx.i >> expo_x);
        return xx.f;
      } else if (mode == cudaRoundPosInf) {
        if (expo_x > 25) expo_x = 25;
        yy.i = (xx.i << (32 - expo_x)) | ((yy.i) ? 1 : 0);
        xx.i = expo_y + (xx.i >> expo_x);
        xx.i += (yy.i && !expo_y);
        return xx.f;
      } else { 
        if (expo_x > 25) expo_x = 25;
        yy.i = (xx.i << (32 - expo_x)) | ((yy.i) ? 1 : 0);
        xx.i = expo_y + (xx.i >> expo_x);
        xx.i += (yy.i && expo_y);
        return xx.f;
      } 
    }
  } else {
    product = xx.i ^ yy.i;
    product = product & 0x80000000;
    if (!(xx.i & 0x7fffffff)) {
      if (expo_y != 254) {
        xx.i = (unsigned int)product;
        return xx.f;
      }
      expo_y = yy.i << 1;
      if (expo_y == 0xFF000000) {
        xx.i = expo_y | 0x00C00000;
      } else {
        xx.i = yy.i | 0x00400000;
      }
      return xx.f;
    }
    if (!(yy.i & 0x7fffffff)) {
      if (expo_x != 254) {
        xx.i = (unsigned int)product;
        return xx.f;
      }
      expo_x = xx.i << 1;
      if (expo_x == 0xFF000000) {
        xx.i = expo_x | 0x00C00000;
      } else {
        xx.i = xx.i | 0x00400000;
      }
      return xx.f;
    }
    if ((expo_y != 254) && (expo_x != 254)) {
      expo_y++;
      expo_x++;
      if (expo_x == 0) {
        expo_y |= xx.i & 0x80000000;
        



        xx.i = xx.i << 8;
        while (!(xx.i & 0x80000000)) {
          xx.i <<= 1;
          expo_x--;
        }
        xx.i = (xx.i >> 8) | (expo_y & 0x80000000);
        expo_y &= ~0x80000000;
        expo_y--;
        goto multiply;
      }
      if (expo_y == 0) {
        expo_x |= yy.i & 0x80000000;
        yy.i = yy.i << 8;
        while (!(yy.i & 0x80000000)) {
          yy.i <<= 1;
          expo_y--;
        }
        yy.i = (yy.i >> 8) | (expo_x & 0x80000000);
        expo_x &= ~0x80000000;
        expo_x--;
        goto multiply;
      }
    }
    expo_x = xx.i << 1;
    expo_y = yy.i << 1;
    
    if (expo_x > 0xFF000000) {
      
      xx.i = xx.i | 0x00400000;
      return xx.f;
    }
    
    if (expo_y > 0xFF000000) {
      
      xx.i = yy.i | 0x00400000;
      return xx.f;
    } 
    xx.i = (unsigned int)product | 0x7f800000;
    return xx.f;
  }
}

static float __internal_fmaf_kernel (float a, float b, float c, enum cudaRoundMode mode)
#line 3115 "c:\\cuda\\include\\device_functions.h"
{
  unsigned long long product;
  unsigned int xx, yy, zz, ww;
  unsigned int temp, s, u;
  unsigned int expo_x, expo_y, expo_z;
  volatile union __cudart_FloatUintCvt cvt;
  
  cvt.f = a;
  xx = cvt.i;
  cvt.f = b;
  yy = cvt.i;
  cvt.f = c;
  zz = cvt.i;

  temp = 0xff;
  expo_x = temp & (xx >> 23);
  expo_x = expo_x - 1;
  expo_y = temp & (yy >> 23);
  expo_y = expo_y - 1;
  expo_z = temp & (zz >> 23);
  expo_z = expo_z - 1;

  if (!((expo_x <= 0xFD) && 
        (expo_y <= 0xFD) &&
        (expo_z <= 0xFD))) {
    



    if ((yy << 1) > 0xff000000) {
      return b + b;
    }
    if ((zz << 1) > 0xff000000) {
      return c + c;
    }
    if ((xx << 1) > 0xff000000) {
      return a + a;
    }
    










    if ((((xx << 1) == 0) && ((yy << 1) == 0xff000000)) ||
        (((yy << 1) == 0) && ((xx << 1) == 0xff000000))) {
      cvt.i = 0xffc00000;
      return cvt.f;
    }
    if ((zz << 1) == 0xff000000) {
      if (((yy << 1) == 0xff000000) || ((xx << 1) == 0xff000000)) {
        if ((int)(xx ^ yy ^ zz) < 0) {
          cvt.i = 0xffc00000;
          return cvt.f;
        }
      }
    }
    



    if ((xx << 1) == 0xff000000) {
      xx = xx ^ (yy & 0x80000000);
      cvt.i = xx;
      return cvt.f;
    }
    if ((yy << 1) == 0xff000000) {
      yy = yy ^ (xx & 0x80000000);
      cvt.i = yy;
      return cvt.f;
    }
    if ((zz << 1) == 0xff000000) {
      cvt.i = zz;
      return cvt.f;
    }
    




    if (zz == 0x80000000) {
      if (((xx << 1) == 0) || ((yy << 1) == 0)) {
        if ((int)(xx ^ yy) < 0) {
          cvt.i = zz;
          return cvt.f;
        }
      }
    }
    


    if (((zz << 1) == 0) && 
        (((xx << 1) == 0) || ((yy << 1) == 0))) {
      if (mode == cudaRoundMinInf) {
        zz = 0x80000000 & (xx ^ yy ^ zz);
      } else {
        zz &= 0x7fffffff;
      }
      cvt.i = zz;
      return cvt.f;
    }
    


    if (((xx << 1) == 0) || ((yy << 1) == 0)) {
      cvt.i = zz;
      return cvt.f;
    }
    
    if (expo_x == (unsigned)-1) {
      temp = xx & 0x80000000;
      xx = xx << 8;
      while (!(xx & 0x80000000)) {
        xx <<= 1;
        expo_x--;
      }
      expo_x++;
      xx = (xx >> 8) | temp;
    }
    
    if (expo_y == (unsigned)-1) {
      temp = yy & 0x80000000;
      yy = yy << 8;
      while (!(yy & 0x80000000)) {
        yy <<= 1;
        expo_y--;
      }
      expo_y++;
      yy = (yy >> 8) | temp;
    }
    
    if ((expo_z == (unsigned)-1) && ((zz << 1) != 0)) {
      temp = zz & 0x80000000;
      zz = zz << 8;
      while (!(zz & 0x80000000)) {
        zz <<= 1;
        expo_z--;
      }
      expo_z++;
      zz = (zz >> 8) | temp;
    }
  }
    
  expo_x = expo_x + expo_y;
  expo_y = xx ^ yy;
  xx = xx & 0x00ffffff;
  yy = yy << 8;
  xx = xx | 0x00800000;
  yy = yy | 0x80000000;

  product = ((unsigned long long)xx) * yy;
  xx = (unsigned)(product >> 32);
  yy = (unsigned)(product & 0xffffffff);

  expo_x = expo_x - 127 + 2;
  expo_y = expo_y & 0x80000000;
  
  if (xx < 0x00800000) {
    xx = (xx << 1) | (yy >> 31);
    yy = (yy << 1);
    expo_x--;
  }
  temp = 0;

  if ((zz << 1) != 0) { 
    s = zz & 0x80000000;
    zz &= 0x00ffffff;
    zz |= 0x00800000;
    ww = 0;
    
    if ((int)expo_z > (int)expo_x) {
      temp = expo_z;
      expo_z = expo_x;
      expo_x = temp;
      temp = zz;
      zz = xx;
      xx = temp;
      temp = ww;
      ww = yy;
      yy = temp;
      temp = expo_y;
      expo_y = s;
      s = temp;
    }
    
    
    expo_z = expo_x - expo_z;
    u = expo_y ^ s;
    if (expo_z <= 49) {
      
      temp = 0;
      while (expo_z >= 32) {
        temp = ww | (temp != 0);
        ww = zz;
        zz = 0;
        expo_z -= 32;
      }
      if (expo_z) {
        temp = ((temp >> expo_z) | (ww << (32 - expo_z)) | 
                ((temp << (32 - expo_z)) != 0));
        ww = (ww >> expo_z) | (zz << (32 - expo_z));
        zz = (zz >> expo_z);
      }
      
    } else {
      temp = 1;
      ww = 0;
      zz = 0;
    }            
    if ((int)u < 0) {
      
      temp = (unsigned)(-(int)temp);
      s = (temp != 0);
      u = yy - s;
      s = u > yy;
      yy = u - ww;
      s += yy > u;
      xx = (xx - zz) - s;
      if (!(xx | yy | temp)) {
        
        if (mode == cudaRoundMinInf) {
          xx = 0x80000000;
        }
        cvt.i = xx;
        return cvt.f;
      }
      if ((int)xx < 0) {
        

        temp = ~temp;
        yy = ~yy;
        xx = ~xx;
        if (++temp == 0) {
          if (++yy == 0) {
            ++xx;
          }
        }
        expo_y ^= 0x80000000;
      }
      
      while (!(xx & 0x00800000)) {
        xx = (xx << 1) | (yy >> 31);
        yy = (yy << 1);
        expo_x--;
      }
    } else {
      
      yy = yy + ww;
      s =  yy < ww;
      xx = xx + zz + s;
      if (xx & 0x01000000) {
        temp = temp | (yy << 31);
        yy = (yy >> 1) | (xx << 31);
        xx = ((xx & 0x80000000) | (xx >> 1)) & ~0x40000000;
        expo_x++;
      }
    }
  }
  temp = yy | (temp != 0);
  if (expo_x <= 0xFD) {
    
    xx |= expo_y; 
    if (mode == cudaRoundNearest) {
      s = xx & 1; 
      xx += (temp == 0x80000000) ? s : (temp >> 31);
    } else if (mode == cudaRoundPosInf) {
      xx += temp && !expo_y;
    } else if (mode == cudaRoundMinInf) {
      xx += temp && expo_y;
    }
    xx = xx + (expo_x << 23); 
    cvt.i = xx;
    return cvt.f;
  } else if ((int)expo_x >= 126) {
    
    if (mode == cudaRoundNearest) {  
      xx = expo_y | 0x7f800000;
    } else if (mode == cudaRoundZero) {
      xx = expo_y | 0x7F7FFFFF;
    } else if (mode == cudaRoundPosInf) {
      xx = expo_y ? 0xFF7FFFFF : 0x7f800000;
    } else if (mode == cudaRoundMinInf) {
      xx = expo_y ? 0xff800000 : 0x7f7fffff;
    }
    cvt.i = xx;
    return cvt.f;
  }
  
  expo_x = (unsigned int)(-(int)expo_x);
  if (expo_x > 25) {
    
    xx = 0;
    if (mode == cudaRoundPosInf) {
      xx += !expo_y;
    } else if (mode == cudaRoundMinInf) {
      xx += !!expo_y;
    }
    cvt.i = expo_y | xx;
    return cvt.f;
  }
  temp = (xx << (32 - expo_x)) | ((temp) ? 1 : 0);
  xx = xx >> expo_x;
  if (mode == cudaRoundNearest) {
    xx = xx + ((temp == 0x80000000) ? (xx & 1) : (temp >> 31));
  } else if (mode == cudaRoundPosInf) {
    xx = xx + (!expo_y && temp);
  } else if (mode == cudaRoundMinInf) {
    xx = xx + (expo_y && temp);
  }
  xx = expo_y + xx; 
  cvt.i = xx;
  return cvt.f;
}

static float __internal_fadd_kernel (float a, float b, enum cudaRoundMode mode)
#line 3436 "c:\\cuda\\include\\device_functions.h"
{
  volatile union __cudart_FloatUintCvt xx, yy;
  unsigned int expo_x;
  unsigned int expo_y;
  unsigned int temp;

  xx.f = a;
  yy.f = b;

  
  expo_y = yy.i << 1;
  if (expo_y > (xx.i << 1)) {
    expo_y = xx.i;
    xx.i   = yy.i;
    yy.i   = expo_y;
  }
    
  temp = 0xff;
  expo_x = temp & (xx.i >> 23);
  expo_x = expo_x - 1;
  expo_y = temp & (yy.i >> 23);
  expo_y = expo_y - 1;

  if ((expo_x <= 0xFD) && 
      (expo_y <= 0xFD)) {
add:
    expo_y = expo_x - expo_y;
    if (expo_y > 25) {
      expo_y = 31;
    }
    temp = xx.i ^ yy.i;
    xx.i = xx.i & ~0x7f000000;
    xx.i = xx.i |  0x00800000;
    yy.i = yy.i & ~0xff000000;
    yy.i = yy.i |  0x00800000;

    if ((int)temp < 0) {
      
      temp = 32 - expo_y;
      temp = (expo_y) ? (yy.i << temp) : 0;
      temp = (unsigned)(-((int)temp));
      xx.i = xx.i - (yy.i >> expo_y) - (temp ? 1 : 0);
      if (xx.i & 0x00800000) {
        if (expo_x <= 0xFD) {
          xx.i = xx.i + (expo_x << 23);
          if (mode == cudaRoundNearest) {
            if (temp < 0x80000000) return xx.f;
            xx.i += ((temp == 0x80000000) ? (xx.i & 1) : (temp >> 31));
          } else if (mode == cudaRoundZero) {              
          } else if (mode == cudaRoundPosInf) {
            xx.i += (temp && !(xx.i & 0x80000000));
          } else if (mode == cudaRoundMinInf) {
            xx.i += (temp && (xx.i & 0x80000000));
          }
          return xx.f;
        }
      } else {
        if ((temp | (xx.i << 1)) == 0) {
          
          if (mode == cudaRoundMinInf) {
            xx.i = 0x80000000;
          } else {
            xx.i = 0;
          }
          return xx.f;
        }
        
        yy.i = xx.i & 0x80000000;
        do {
          xx.i = (xx.i << 1) | (temp >> 31);
          temp <<= 1;
          expo_x--;
        } while (!(xx.i & 0x00800000));
        xx.i = xx.i | yy.i;
      }
    } else {
      
      temp = 32 - expo_y;
      temp = (expo_y) ? (yy.i << temp) : 0;
      xx.i = xx.i + (yy.i >> expo_y);
      if (!(xx.i & 0x01000000)) {
        if (expo_x <= 0xFD) {
          xx.i = xx.i + (expo_x << 23);
          if (mode == cudaRoundNearest) {
            if (temp < 0x80000000) return xx.f;
            xx.i += ((temp == 0x80000000) ? (xx.i & 1) : (temp >> 31));
          } else if (mode == cudaRoundZero) {
          } else if (mode == cudaRoundPosInf) {
            xx.i += (temp && !(xx.i & 0x80000000));
          } else if (mode == cudaRoundMinInf) {
            xx.i += (temp && (xx.i & 0x80000000));
          }
          return xx.f;
        }
      } else {
        
        temp = (xx.i << 31) | (temp >> 1);
        xx.i = ((xx.i & 0x80000000) | (xx.i >> 1)) & ~0x40000000;
        expo_x++;
      }
    }
    if (expo_x <= 0xFD) {
      xx.i = xx.i + (expo_x << 23);
      if (mode == cudaRoundNearest) {
        if (temp < 0x80000000) return xx.f;
        xx.i += ((temp == 0x80000000) ? (xx.i & 1) : (temp >> 31));
      } else if (mode == cudaRoundZero) {
      } else if (mode == cudaRoundPosInf) {
        xx.i += (temp && !(xx.i & 0x80000000));
      } else if (mode == cudaRoundMinInf) {
        xx.i += (temp && (xx.i & 0x80000000));
      }
      return xx.f;
    }
    if ((int)expo_x >= 254) {
      
      temp = xx.i & 0x80000000;
      if (mode == cudaRoundNearest) {
        xx.i = (temp) | 0x7f800000;
      } else if (mode == cudaRoundZero) {
        xx.i = (temp) | 0x7f7fffff;
      } else if (mode == cudaRoundMinInf) {
        xx.i = (temp ? 0xFF800000 : 0x7f7fffff);
      } else if (mode == cudaRoundPosInf) {
        xx.i = (temp ? 0xff7fffff : 0x7F800000);
      }
      return xx.f;
    }
    
    expo_y = expo_x + 32;
    yy.i = xx.i &  0x80000000;
    xx.i = xx.i & ~0xff000000;
    expo_x = (unsigned)(-((int)expo_x));
    temp = xx.i << expo_y | ((temp) ? 1 : 0);
    xx.i = yy.i | (xx.i >> expo_x);
    if (mode == cudaRoundNearest) {
      xx.i += (temp == 0x80000000) ? (xx.i & 1) : (temp >> 31);
    } else if (mode == cudaRoundZero) {
    } else if (mode == cudaRoundPosInf) {
      xx.i += (temp && !yy.i);
    } else if (mode == cudaRoundMinInf) {
      xx.i += (temp && yy.i);
    }
    return xx.f;
  } else {
    
    if (!(yy.i << 1)) {
      if (mode == cudaRoundMinInf) {
        if (!(xx.i << 1)) {
          xx.i = xx.i | yy.i;
        }
      } else {
        if (xx.i == 0x80000000) {
          xx.i = yy.i;
        }
      }
      if ((xx.i << 1) > 0xff000000) {
        xx.i |= 0x00400000;
      }
      return xx.f;
    }
    if ((expo_y != 254) && (expo_x != 254)) {
      
      if (expo_x == (unsigned int) -1) {
        temp = xx.i & 0x80000000;
        xx.i = xx.i << 8;
        while (!(xx.i & 0x80000000)) {
          xx.i <<= 1;
          expo_x--;
        }
        expo_x++;
        xx.i = (xx.i >> 8) | temp;
      }
      if (expo_y == (unsigned int) -1) {
        temp = yy.i & 0x80000000;
        yy.i = yy.i << 8;
        while (!(yy.i & 0x80000000)) {
          yy.i <<= 1;
          expo_y--;
        }
        expo_y++;
        yy.i = (yy.i >> 8) | temp;
      }
      goto add;
    }
    expo_x = xx.i << 1;
    expo_y = yy.i << 1;
    
    if (expo_x > 0xff000000) {
      
      xx.i = xx.i | 0x00400000;
      return xx.f;
    }
    
    if (expo_y > 0xff000000) {
      
      xx.i = yy.i | 0x00400000;
      return xx.f;
    }
    if ((expo_x == 0xff000000) && (expo_y == 0xff000000)) {
      



      expo_x = xx.i ^ yy.i;
      xx.i = xx.i | ((expo_x) ? 0xffc00000 : 0);
      return xx.f;
    }
    
    if (expo_y == 0xff000000) {
      xx.i = yy.i;
    }
    return xx.f;
  }
}

static float __frcp_rn (float a)
{
  return __internal_frcp_kernel (a, cudaRoundNearest);
}

static float __frcp_rz (float a)
{
  return __internal_frcp_kernel (a, cudaRoundZero);
}

static float __frcp_rd (float a)
{
  return __internal_frcp_kernel (a, cudaRoundMinInf);
}

static float __frcp_ru (float a)
{
  return __internal_frcp_kernel (a, cudaRoundPosInf);
}

static float __fsqrt_rn (float a)
{
  return __internal_fsqrt_kernel (a, cudaRoundNearest);
}

static float __fsqrt_rz (float a)
{
  return __internal_fsqrt_kernel (a, cudaRoundZero);
}

static float __fsqrt_rd (float a)
{
  return __internal_fsqrt_kernel (a, cudaRoundMinInf);
}

static float __fsqrt_ru (float a)
{
  return __internal_fsqrt_kernel (a, cudaRoundPosInf);
}

static float __fdiv_rn (float a, float b)
{
  return __internal_fdiv_kernel (a, b, cudaRoundNearest);
}

static float __fdiv_rz (float a, float b)
{
  return __internal_fdiv_kernel (a, b, cudaRoundZero);
}

static float __fdiv_rd (float a, float b)
{
  return __internal_fdiv_kernel (a, b, cudaRoundMinInf);
}

static float __fdiv_ru (float a, float b)
{
  return __internal_fdiv_kernel (a, b, cudaRoundPosInf);
}

static float __fadd_rd (float a, float b)
{
  return __internal_fadd_kernel (a, b, cudaRoundMinInf);
}

static float __fadd_ru (float a, float b)
{
  return __internal_fadd_kernel (a, b, cudaRoundPosInf);
}

static float __fmul_rd (float a, float b)
{
  return __internal_fmul_kernel (a, b, cudaRoundMinInf);
}

static float __fmul_ru (float a, float b)
{
  return __internal_fmul_kernel (a, b, cudaRoundPosInf);
}

static float __fmaf_rn (float a, float b, float c)
{
  return __internal_fmaf_kernel (a, b, c, cudaRoundNearest);
}

static float __fmaf_rz (float a, float b, float c)
{
  return __internal_fmaf_kernel (a, b, c, cudaRoundZero);
}

static float __fmaf_ru (float a, float b, float c)
{
  return __internal_fmaf_kernel (a, b, c, cudaRoundPosInf);
}

static float __fmaf_rd (float a, float b, float c)
{
  return __internal_fmaf_kernel (a, b, c, cudaRoundMinInf);
}

static int __cuda___isnan(double a);
static int __cuda___isnanf(float a);
static int __double2int_rz(double);
static unsigned int __double2uint_rz(double);
static long long int __double2ll_rz(double);
static unsigned long long int __double2ull_rz(double);













static int __mulhi(int a, int b)
{
  long long int c = (long long int)a * (long long int)b;

  return (int)(c >> 32);
}

static unsigned int __umulhi(unsigned int a, unsigned int b)
{
  unsigned long long int c = (unsigned long long int)a * (unsigned long long int)b;

  return (unsigned int)(c >> 32);
}

static unsigned long long int __umul64hi(unsigned long long int a, unsigned long long int b)
{
  unsigned int           a_lo = (unsigned int)a;
  unsigned long long int a_hi = a >> 32;
  unsigned int           b_lo = (unsigned int)b;
  unsigned long long int b_hi = b >> 32;
  unsigned long long int m1 = a_lo * b_hi;
  unsigned long long int m2 = a_hi * b_lo;
  unsigned int           carry;

  carry = (0ULL + __umulhi(a_lo, b_lo) + (unsigned int)m1 + (unsigned int)m2) >> 32;

  return a_hi * b_hi + (m1 >> 32) + (m2 >> 32) + carry;
}

static long long int __mul64hi(long long int a, long long int b)
{
  long long int res;
  res = __umul64hi(a, b);
  if (a < 0LL) res = res - b;
  if (b < 0LL) res = res - a;
  return res;
}

static float __saturatef(float a)
{
  if (__cuda___isnanf(a)) return 0.0f; 
  return a >= 1.0f ? 1.0f : a <= 0.0f ? 0.0f : a;
}

static unsigned int __sad(int a, int b, unsigned int c)
{
  long long int diff = (long long int)a - (long long int)b;

  return (unsigned int)(__cuda_llabs(diff) + (long long int)c);
}

static unsigned int __usad(unsigned int a, unsigned int b, unsigned int c)
{
  long long int diff = (long long int)a - (long long int)b;

  return (unsigned int)(__cuda_llabs(diff) + (long long int)c);
}

static int __mul24(int a, int b)
{

  a &= 0xffffff;
  a = (a & 0x800000) != 0 ? a | ~0xffffff : a;
  b &= 0xffffff;
  b = (b & 0x800000) != 0 ? b | ~0xffffff : b;
#line 3837 "c:\\cuda\\include\\device_functions.h"

  return a * b;
}

static unsigned int __umul24(unsigned int a, unsigned int b)
{

  a &= 0xffffff;
  b &= 0xffffff;
#line 3847 "c:\\cuda\\include\\device_functions.h"

  return a * b;
}

static float __int_as_float(int a)
{
  volatile union __cudart_FloatIntCvt u;

  u.i = a;
  return u.f;
}

static int __float_as_int(float a)
{
  volatile union __cudart_FloatIntCvt u;

  u.f = a;
  return u.i;
}

static long long int __internal_float2ll_kernel(float a, long long int max, long long int min, long long int nan, enum cudaRoundMode rndMode)
{
  unsigned long long int res, t = 0ULL;
  int shift;
  unsigned int ia;

  if (sizeof(a) == sizeof(double) && __cuda___isnan((double)a)) return nan; if (sizeof(a) == sizeof(float) && __cuda___isnanf((float)a)) return nan; if (a >= max) return max; if (a <= min) return min;
  ia = __float_as_int(a);
  shift = 189 - ((ia >> 23) & 0xff);
  res = (unsigned long long int)(((ia << 8) | 0x80000000) >> 1) << 32;
  if (shift >= 64) {
    t = res;
    res = 0;
  } else if (shift) {
    t = res << (64 - shift);
    res = res >> shift;
  }
  if (rndMode == cudaRoundNearest && (long long int)t < 0LL) {
    res += t == 0x8000000000000000ULL ? res & 1ULL : 1ULL;
  }
  else if (rndMode == cudaRoundMinInf && t != 0ULL && ia > 0x80000000) {
    res++;
  }
  else if (rndMode == cudaRoundPosInf && t != 0ULL && (int)ia > 0) {
    res++;
  }
  if ((int)ia < 0) res = (unsigned long long int)-(long long int)res;
  return (long long int)res;
}

static int __internal_float2int(float a, enum cudaRoundMode rndMode)
{
  return (int)__internal_float2ll_kernel(a, 2147483647LL, -2147483648LL, 0LL, rndMode);
}

static int __float2int_rz(float a)
{


#line 3907 "c:\\cuda\\include\\device_functions.h"
  return __internal_float2int(a, cudaRoundZero);
#line 3909 "c:\\cuda\\include\\device_functions.h"
}

static int __float2int_ru(float a)
{
  return __internal_float2int(a, cudaRoundPosInf);
}

static int __float2int_rd(float a)
{
  return __internal_float2int(a, cudaRoundMinInf);
}

static int __float2int_rn(float a)
{
  return __internal_float2int(a, cudaRoundNearest);
}

static long long int __internal_float2ll(float a, enum cudaRoundMode rndMode)
{
  return __internal_float2ll_kernel(a, 9223372036854775807LL, -9223372036854775807LL -1LL, -9223372036854775807LL -1LL, rndMode);
}

static long long int __float2ll_rz(float a)
{


#line 3936 "c:\\cuda\\include\\device_functions.h"
  return __internal_float2ll(a, cudaRoundZero);
#line 3938 "c:\\cuda\\include\\device_functions.h"
}

static long long int __float2ll_ru(float a)
{
  return __internal_float2ll(a, cudaRoundPosInf);
}

static long long int __float2ll_rd(float a)
{
  return __internal_float2ll(a, cudaRoundMinInf);
}

static long long int __float2ll_rn(float a)
{
  return __internal_float2ll(a, cudaRoundNearest);
}

static unsigned long long int __internal_float2ull_kernel(float a, unsigned long long int max, unsigned long long int nan, enum cudaRoundMode rndMode)
{
  unsigned long long int res, t = 0ULL;
  int shift;
  unsigned int ia;

  if (sizeof(a) == sizeof(double) && __cuda___isnan((double)a)) return nan; if (sizeof(a) == sizeof(float) && __cuda___isnanf((float)a)) return nan; if (a >= max) return max; if (a <= 0LL) return 0LL;
  ia = __float_as_int(a);
  shift = 190 - ((ia >> 23) & 0xff);
  res = (unsigned long long int)((ia << 8) | 0x80000000) << 32;
  if (shift >= 64) {
    t = res >> (int)(shift > 64);
    res = 0;
  } else if (shift) {
    t = res << (64 - shift);
    res = res >> shift;
  }
  if (rndMode == cudaRoundNearest && (long long int)t < 0LL) {
    res += t == 0x8000000000000000ULL ? res & 1ULL : 1ULL;
  }
  else if (rndMode == cudaRoundPosInf && t != 0ULL) {
    res++;
  }
  return res;
}

static unsigned int __internal_float2uint(float a, enum cudaRoundMode rndMode)
{
  return (unsigned int)__internal_float2ull_kernel(a, 4294967295U, 0U, rndMode);
}

static unsigned int __float2uint_rz(float a)
{


#line 3991 "c:\\cuda\\include\\device_functions.h"
  return __internal_float2uint(a, cudaRoundZero);
#line 3993 "c:\\cuda\\include\\device_functions.h"
}

static unsigned int __float2uint_ru(float a)
{
  return __internal_float2uint(a, cudaRoundPosInf);
}

static unsigned int __float2uint_rd(float a)
{
  return __internal_float2uint(a, cudaRoundMinInf);
}

static unsigned int __float2uint_rn(float a)
{
  return __internal_float2uint(a, cudaRoundNearest);
}

static unsigned long long int __internal_float2ull(float a, enum cudaRoundMode rndMode)
{
  return __internal_float2ull_kernel(a, 18446744073709551615ULL, 9223372036854775808ULL, rndMode);
}

static unsigned long long int __float2ull_rz(float a)
{


#line 4020 "c:\\cuda\\include\\device_functions.h"
  return __internal_float2ull(a, cudaRoundZero);
#line 4022 "c:\\cuda\\include\\device_functions.h"
}

static unsigned long long int __float2ull_ru(float a)
{
  return __internal_float2ull(a, cudaRoundPosInf);
}

static unsigned long long int __float2ull_rd(float a)
{
  return __internal_float2ull(a, cudaRoundMinInf);
}

static unsigned long long int __float2ull_rn(float a)
{
  return __internal_float2ull(a, cudaRoundNearest);
}

static int __internal_normalize64(unsigned long long int *a)
{
  int lz = 0;

  if ((*a & 0xffffffff00000000ULL) == 0ULL) {
    *a <<= 32;
    lz += 32;
  }
  if ((*a & 0xffff000000000000ULL) == 0ULL) {
    *a <<= 16;
    lz += 16;
  }
  if ((*a & 0xff00000000000000ULL) == 0ULL) {
    *a <<= 8;
    lz += 8;
  }
  if ((*a & 0xf000000000000000ULL) == 0ULL) {
    *a <<= 4;
    lz += 4;
  }
  if ((*a & 0xC000000000000000ULL) == 0ULL) {
    *a <<= 2;
    lz += 2;
  }
  if ((*a & 0x8000000000000000ULL) == 0ULL) {
    *a <<= 1;
    lz += 1;
  }
  return lz;
}

static int __internal_normalize(unsigned int *a)
{
  unsigned long long int t = (unsigned long long int)*a;
  int lz = __internal_normalize64(&t);
  
  *a = (unsigned int)(t >> 32);

  return lz - 32;
}

static float __internal_int2float_kernel(int a, enum cudaRoundMode rndMode)
{
  volatile union __cudart_FloatUintCvt res;
  int shift;
  unsigned int t;
  res.i = a;
  if (a == 0) return res.f;
  if (a < 0) res.i = (unsigned int)-a;
  shift = __internal_normalize((unsigned int*)&res.i);
  t = res.i << 24;
  res.i = (res.i >> 8);
  res.i += (127 + 30 - shift) << 23;
  if (a < 0) res.i |= 0x80000000;
  if ((rndMode == cudaRoundNearest) && (t >= 0x80000000)) {
    res.i += (t == 0x80000000) ? (res.i & 1) : (t >> 31);
  }
  else if ((rndMode == cudaRoundMinInf) && t && (a < 0)) {
    res.i++;
  }
  else if ((rndMode == cudaRoundPosInf) && t && (a > 0)) {
    res.i++;
  }
  return res.f;
}

static float __int2float_rz(int a)
{
  return __internal_int2float_kernel(a, cudaRoundZero);
}

static float __int2float_ru(int a)
{
  return __internal_int2float_kernel(a, cudaRoundPosInf);
}

static float __int2float_rd(int a)
{
  return __internal_int2float_kernel(a, cudaRoundMinInf);
}

static float __int2float_rn(int a)
{


#line 4125 "c:\\cuda\\include\\device_functions.h"
  return __internal_int2float_kernel(a, cudaRoundNearest);
#line 4127 "c:\\cuda\\include\\device_functions.h"
}

static float __internal_uint2float_kernel(unsigned int a, enum cudaRoundMode rndMode)
{
  volatile union __cudart_FloatUintCvt res;
  int shift;
  unsigned int t;
  res.i = a;
  if (a == 0) return res.f;
  shift = __internal_normalize((unsigned int*)&res.i);
  t = res.i << 24;
  res.i = (res.i >> 8);
  res.i += (127 + 30 - shift) << 23;
  if (rndMode == cudaRoundNearest) {
    res.i += (t == 0x80000000) ? (res.i & 1) : (t >> 31);
  }
  else if ((rndMode == cudaRoundPosInf) && t) {
    res.i++;
  }
  return res.f;
}

static float __uint2float_rz(unsigned int a)
{
  return __internal_uint2float_kernel(a, cudaRoundZero);
}

static float __uint2float_ru(unsigned int a)
{
  return __internal_uint2float_kernel(a, cudaRoundPosInf);
}

static float __uint2float_rd(unsigned int a)
{
  return __internal_uint2float_kernel(a, cudaRoundMinInf);
}

static float __uint2float_rn(unsigned int a)
{


#line 4169 "c:\\cuda\\include\\device_functions.h"
  return __internal_uint2float_kernel(a, cudaRoundNearest);
#line 4171 "c:\\cuda\\include\\device_functions.h"
}

static float __ll2float_rn(long long int a)
{
  return (float)a;
}      

static float __internal_ull2float_kernel(unsigned long long int a, enum cudaRoundMode rndMode)
{
  unsigned long long int temp;
  unsigned int res, t;
  int shift;
  if (a == 0ULL) return 0.0f;
  temp = a;
  shift = __internal_normalize64(&temp);
  temp = (temp >> 8) | ((temp & 0xffULL) ? 1ULL : 0ULL);
  res = (unsigned int)(temp >> 32);
  t = (unsigned int)temp;
  res += (127 + 62 - shift) << 23; 
  if (rndMode == cudaRoundNearest) {
    res += (t == 0x80000000) ? (res & 1) : (t >> 31);
  } else if (rndMode == cudaRoundPosInf) {
    res += (t != 0);
  }
  return __int_as_float(res);
}

static float __ull2float_rn(unsigned long long int a)
{


#line 4203 "c:\\cuda\\include\\device_functions.h"
  return __internal_ull2float_kernel(a, cudaRoundNearest);
#line 4205 "c:\\cuda\\include\\device_functions.h"
}   

static unsigned short __float2half_rn(float f)
{
  unsigned int x = __float_as_int (f);
  unsigned int u = (x & 0x7fffffff), remainder, shift, lsb, lsb_s1, lsb_m1;
  unsigned int sign, exponent, mantissa;

  
  if (u > 0x7f800000) {
    return 0x7fff;
  }
  
  sign = ((x >> 16) & 0x8000);
  
  
  if (u > 0x477fefff) {
    return sign | 0x7c00;
  }
  if (u < 0x33000001) {
    return sign | 0x0000;
  }

  exponent = ((u >> 23) & 0xff);
  mantissa = (u & 0x7fffff);

  if (exponent > 0x70) {
    shift = 13;
    exponent -= 0x70;
  } else {
    shift = 0x7e - exponent;
    exponent = 0;
    mantissa |= 0x800000;
  }
  lsb = (1 << shift);
  lsb_s1 = (lsb >> 1);
  lsb_m1 = (lsb - 1);
  
  
  remainder = (mantissa & lsb_m1);
  mantissa >>= shift;
  if (remainder > lsb_s1 || (remainder == lsb_s1 && (mantissa & 0x1))) {
    ++mantissa;
    if (!(mantissa & 0x3ff)) {
      ++exponent;
      mantissa = 0;
    }
  }  

  return sign | (exponent << 10) | mantissa;  
}

static float __half2float(unsigned short h)
{
  unsigned int sign = ((h >> 15) & 1);
  unsigned int exponent = ((h >> 10) & 0x1f);
  unsigned int mantissa = ((h & 0x3ff) << 13);

  if (exponent == 0x1f) {  
    mantissa = (mantissa
                ? (sign = 0, 0x7fffff)
                : 0);
    exponent = 0xff;
  } else if (!exponent) {  
    if (mantissa) {
      unsigned int msb;
      exponent = 0x71;
      do {
        msb = (mantissa & 0x400000);
        mantissa <<= 1;  
        --exponent;
      } while (!msb);
      mantissa &= 0x7fffff;  
    }
  } else {
    exponent += 0x70;
  }

  return __int_as_float ((sign << 31) | (exponent << 23) | mantissa);
}

static float __fadd_rz(float a, float b)
{
  return __internal_fadd_kernel(a, b, cudaRoundZero);
}

static float __fmul_rz(float a, float b)
{
  return __internal_fmul_kernel(a, b, cudaRoundZero);
}

static float __fadd_rn(float a, float b)
{
  return __internal_fadd_kernel(a, b, cudaRoundNearest);
}

static float __fmul_rn(float a, float b)
{
  return __internal_fmul_kernel(a, b, cudaRoundNearest);
}

static void __brkpt(int c)
{
  
}






#line 4317 "c:\\cuda\\include\\device_functions.h"

extern int __stdcall __cudaSynchronizeThreads(void**, void*);









#line 4329 "c:\\cuda\\include\\device_functions.h"




#line 4334 "c:\\cuda\\include\\device_functions.h"

#line 4336 "c:\\cuda\\include\\device_functions.h"

static void __prof_trigger(int a)
{
}

static void __threadfence(void)
{
  (void)__cudaSynchronizeThreads((void**)0, (void*)0);
}

static void __threadfence_block(void)
{
  (void)__cudaSynchronizeThreads((void**)0, (void*)0);
}








#line 4359 "c:\\cuda\\include\\device_functions.h"

static void __trap(void)
{
  __debugbreak();
}

#line 4366 "c:\\cuda\\include\\device_functions.h"

#line 4368 "c:\\cuda\\include\\device_functions.h"







static float __fdividef(float a, float b)
{


#line 4380 "c:\\cuda\\include\\device_functions.h"
  volatile float aa = a;
  volatile float bb = b;
  
  if (__cuda_fabsf(bb) > 8.507059173e37f) {
    if (__cuda_fabsf(aa) <= 3.402823466e38f) {
      return ((aa / bb) / 3.402823466e38f) / 3.402823466e38f;
    } else {
      bb = 1.0f / bb;
      bb = bb / 3.402823466e38f;
      return aa * bb;
    }
  } else {
    return aa / bb;
  }
#line 4395 "c:\\cuda\\include\\device_functions.h"
}
#line 4397 "c:\\cuda\\include\\device_functions.h"

static float __sinf(float a)
{

  if ((__float_as_int(a) << 1) == 0xff000000) {
    return __fadd_rn (a, -a); 
  }
#line 4405 "c:\\cuda\\include\\device_functions.h"
  return sinf(a);
}

static float __cosf(float a)
{

  if ((__float_as_int(a) << 1) == 0xff000000) {
    return __fadd_rn (a, -a); 
  }
#line 4415 "c:\\cuda\\include\\device_functions.h"
  return cosf(a);
}

static float __log2f(float a)
{
  return log2f(a);
}







static float __internal_accurate_fdividef(float a, float b)
{
  return a / b;
}

static float __tanf(float a)
{


#line 4439 "c:\\cuda\\include\\device_functions.h"
  return __fdividef (__sinf(a), __cosf(a));
#line 4441 "c:\\cuda\\include\\device_functions.h"
}

static void __sincosf(float a, float *sptr, float *cptr)
{


#line 4448 "c:\\cuda\\include\\device_functions.h"
  *sptr = __sinf(a);
  *cptr = __cosf(a);
#line 4451 "c:\\cuda\\include\\device_functions.h"
}

static float __expf(float a)
{


#line 4458 "c:\\cuda\\include\\device_functions.h"
  return __cuda_exp2f(a * 1.442695041f);
#line 4460 "c:\\cuda\\include\\device_functions.h"
}

static float __exp10f(float a)
{


#line 4467 "c:\\cuda\\include\\device_functions.h"
  return __cuda_exp2f(a * 3.321928094f);
#line 4469 "c:\\cuda\\include\\device_functions.h"
}

static float __log10f(float a)
{


#line 4476 "c:\\cuda\\include\\device_functions.h"
  return 0.301029996f * __log2f(a);
#line 4478 "c:\\cuda\\include\\device_functions.h"
}

static float __logf(float a)
{


#line 4485 "c:\\cuda\\include\\device_functions.h"
  return 0.693147181f * __log2f(a);
#line 4487 "c:\\cuda\\include\\device_functions.h"
}

static float __powf(float a, float b)
{


#line 4494 "c:\\cuda\\include\\device_functions.h"
  return __cuda_exp2f(b * __log2f(a));
#line 4496 "c:\\cuda\\include\\device_functions.h"
}

static float fdividef(float a, float b)
{


#line 4503 "c:\\cuda\\include\\device_functions.h"

#line 4505 "c:\\cuda\\include\\device_functions.h"
  return __internal_accurate_fdividef(a, b);
#line 4507 "c:\\cuda\\include\\device_functions.h"
}

static int __clz(int a)
{
  return (a)?(158-(__float_as_int(__uint2float_rz((unsigned int)a))>>23)):32;
}

static int __clzll(long long int a)
{
  int ahi = ((int)((unsigned long long)a >> 32));
  int alo = ((int)((unsigned long long)a & 0xffffffffULL));
  int res;
  if (ahi) {
    res = 0;
  } else {
    res = 32;
    ahi = alo;
  }
  res = res + __clz(ahi);
  return res;
}

static int __popc(unsigned int a)
{
  a = a - ((a >> 1) & 0x55555555);
  a = (a & 0x33333333) + ((a >> 2) & 0x33333333);
  a = (a + (a >> 4)) & 0x0f0f0f0f;
  a = ((__umul24(a, 0x808080) << 1) + a) >> 24;
  return a;
}

static int __popcll(unsigned long long int a)
{
  unsigned int ahi = ((unsigned int)(a >> 32));
  unsigned int alo = ((unsigned int)(a & 0xffffffffULL));
  alo = alo - ((alo >> 1) & 0x55555555);
  alo = (alo & 0x33333333) + ((alo >> 2) & 0x33333333);
  ahi = ahi - ((ahi >> 1) & 0x55555555);
  ahi = (ahi & 0x33333333) + ((ahi >> 2) & 0x33333333);
  alo = alo + ahi;
  alo = (alo & 0x0f0f0f0f) + ((alo >> 4) & 0x0f0f0f0f);
  alo = ((__umul24(alo, 0x808080) << 1) + alo) >> 24;
  return alo;
}

static unsigned int __brev(unsigned int a)
{
  a = ((a >>  1) & 0x55555555) + ((a & 0x55555555) <<  1);
  a = ((a >>  2) & 0x33333333) + ((a & 0x33333333) <<  2);
  a = ((a >>  4) & 0x0F0F0F0F) + ((a & 0x0F0F0F0F) <<  4);
  a = ((a >>  8) & 0x00FF00FF) + ((a & 0x00FF00FF) <<  8);
  a = ( a >> 16              ) + ( a               << 16);
  return a;
}

static unsigned long long int __brevll(unsigned long long int a)
{
  unsigned int hi = (unsigned int)(a >> 32);
  unsigned int lo = (unsigned int)(a & 0xffffffffULL);
  unsigned int t;
  t  = __brev(lo);
  lo = __brev(hi);
  return ((unsigned long long int)t << 32) + (unsigned long long int)lo;
}

static int __ffs(int a)
{
  return 32 - __clz (a & -a);
}

static int __ffsll(long long int a)
{
  return 64 - __clzll (a & -a);
}





#line 4587 "c:\\cuda\\include\\device_functions.h"



static double fdivide(double a, double b)
{
  return (double)fdividef((float)a, (float)b);
}



static int __double2int_rz(double a)
{
  return __float2int_rz((float)a);
}

static unsigned int __double2uint_rz(double a)
{
  return __float2uint_rz((float)a);
}

static long long int __double2ll_rz(double a)
{
  return __float2ll_rz((float)a);
}

static unsigned long long int __double2ull_rz(double a)
{
  return __float2ull_rz((float)a);
}

#line 4618 "c:\\cuda\\include\\device_functions.h"

#line 4620 "c:\\cuda\\include\\device_functions.h"





































#line 4658 "c:\\cuda\\include\\device_functions.h"

#line 4660 "c:\\cuda\\include\\device_functions.h"







#line 1 "c:\\cuda\\include\\sm_11_atomic_functions.h"















































































































































































































#line 209 "c:\\cuda\\include\\sm_11_atomic_functions.h"

#line 1 "c:\\cuda\\include\\crt/func_macro.h"
































































































#line 98 "c:\\cuda\\include\\crt/func_macro.h"
#line 211 "c:\\cuda\\include\\sm_11_atomic_functions.h"












































#line 256 "c:\\cuda\\include\\sm_11_atomic_functions.h"

extern void __stdcall __cudaMutexOperation(int lock);






static int __iAtomicAdd(int *address, int val)
{
  int old;

  __cudaMutexOperation(1); old = *address; *address = old + val; __cudaMutexOperation(0);
#line 272 "c:\\cuda\\include\\sm_11_atomic_functions.h"
  return old;
}

static unsigned int __uAtomicAdd(unsigned int *address, unsigned int val)
{
  unsigned int old;

  __cudaMutexOperation(1); old = *address; *address = old + val; __cudaMutexOperation(0);
#line 283 "c:\\cuda\\include\\sm_11_atomic_functions.h"

  return old;
}

static int __iAtomicExch(int *address, int val)
{
  int old;

  __cudaMutexOperation(1); old = *address; *address = val; __cudaMutexOperation(0);
#line 295 "c:\\cuda\\include\\sm_11_atomic_functions.h"

  return old;
}

static unsigned int __uAtomicExch(unsigned int *address, unsigned int val)
{
  unsigned int old;

  __cudaMutexOperation(1); old = *address; *address = val; __cudaMutexOperation(0);
#line 307 "c:\\cuda\\include\\sm_11_atomic_functions.h"

  return old;
}

static float __fAtomicExch(float *address, float val)
{
  float old;

  __cudaMutexOperation(1); old = *address; *address = val; __cudaMutexOperation(0);
#line 319 "c:\\cuda\\include\\sm_11_atomic_functions.h"

  return old;
}

static int __iAtomicMin(int *address, int val)
{
  int old;

  __cudaMutexOperation(1); old = *address; *address = old < val ? old : val; __cudaMutexOperation(0);
#line 331 "c:\\cuda\\include\\sm_11_atomic_functions.h"

  return old;
}

static unsigned int __uAtomicMin(unsigned int *address, unsigned int val)
{
  unsigned int old;

  __cudaMutexOperation(1); old = *address; *address = old < val ? old : val; __cudaMutexOperation(0);
#line 343 "c:\\cuda\\include\\sm_11_atomic_functions.h"

  return old;
}

static int __iAtomicMax(int *address, int val)
{
  int old;

  __cudaMutexOperation(1); old = *address; *address = old > val ? old : val; __cudaMutexOperation(0);
#line 355 "c:\\cuda\\include\\sm_11_atomic_functions.h"

  return old;
}

static unsigned int __uAtomicMax(unsigned int *address, unsigned int val)
{
  unsigned int old;

  __cudaMutexOperation(1); old = *address; *address = old > val ? old : val; __cudaMutexOperation(0);
#line 367 "c:\\cuda\\include\\sm_11_atomic_functions.h"

  return old;
}

static unsigned int __uAtomicInc(unsigned int *address, unsigned int val)
{
  unsigned int old;

  __cudaMutexOperation(1); old = *address; *address = (old >= val) ? 0 : old + 1; __cudaMutexOperation(0);
#line 379 "c:\\cuda\\include\\sm_11_atomic_functions.h"

  return old;
}

static unsigned int __uAtomicDec(unsigned int *address, unsigned int val)
{
  unsigned int old;

  __cudaMutexOperation(1); old = *address; *address = ((old == 0) | (old > val)) ? val : (old - 1); __cudaMutexOperation(0);
#line 391 "c:\\cuda\\include\\sm_11_atomic_functions.h"

  return old;
}

static int __iAtomicAnd(int *address, int val)
{
  int old;

  __cudaMutexOperation(1); old = *address; *address = old & val; __cudaMutexOperation(0);
#line 403 "c:\\cuda\\include\\sm_11_atomic_functions.h"

  return old;
}

static unsigned int __uAtomicAnd(unsigned int *address, unsigned int val)
{
  unsigned int old;

  __cudaMutexOperation(1); old = *address; *address = old & val; __cudaMutexOperation(0);
#line 415 "c:\\cuda\\include\\sm_11_atomic_functions.h"

  return old;
}

static int __iAtomicOr(int *address, int val)
{
  int old;

  __cudaMutexOperation(1); old = *address; *address = old | val; __cudaMutexOperation(0);
#line 427 "c:\\cuda\\include\\sm_11_atomic_functions.h"

  return old;
}

static unsigned int __uAtomicOr(unsigned int *address, unsigned int val)
{
  unsigned int old;

  __cudaMutexOperation(1); old = *address; *address = old | val; __cudaMutexOperation(0);
#line 439 "c:\\cuda\\include\\sm_11_atomic_functions.h"

  return old;
}

static int __iAtomicXor(int *address, int val)
{
  int old;

  __cudaMutexOperation(1); old = *address; *address = old ^ val; __cudaMutexOperation(0);
#line 451 "c:\\cuda\\include\\sm_11_atomic_functions.h"

  return old;
}

static unsigned int __uAtomicXor(unsigned int *address, unsigned int val)
{
  unsigned int old;

  __cudaMutexOperation(1); old = *address; *address = old ^ val; __cudaMutexOperation(0);
#line 463 "c:\\cuda\\include\\sm_11_atomic_functions.h"

  return old;
}

static int __iAtomicCAS(int *address, int compare, int val)
{
  int old;

  __cudaMutexOperation(1); old = *address; *address = old == compare ? val : old; __cudaMutexOperation(0);
#line 475 "c:\\cuda\\include\\sm_11_atomic_functions.h"

  return old;
}

static unsigned int __uAtomicCAS(unsigned int *address, unsigned int compare, unsigned int val)
{
  unsigned int old;

  __cudaMutexOperation(1); old = *address; *address = old == compare ? val : old; __cudaMutexOperation(0);
#line 487 "c:\\cuda\\include\\sm_11_atomic_functions.h"

  return old;
}



#line 494 "c:\\cuda\\include\\sm_11_atomic_functions.h"

#line 496 "c:\\cuda\\include\\sm_11_atomic_functions.h"

#line 498 "c:\\cuda\\include\\sm_11_atomic_functions.h"

#line 500 "c:\\cuda\\include\\sm_11_atomic_functions.h"
#line 4668 "c:\\cuda\\include\\device_functions.h"
#line 1 "c:\\cuda\\include\\sm_12_atomic_functions.h"




































































































#line 102 "c:\\cuda\\include\\sm_12_atomic_functions.h"

#line 1 "c:\\cuda\\include\\crt/func_macro.h"
































































































#line 98 "c:\\cuda\\include\\crt/func_macro.h"
#line 104 "c:\\cuda\\include\\sm_12_atomic_functions.h"












#line 117 "c:\\cuda\\include\\sm_12_atomic_functions.h"

extern void __stdcall __cudaMutexOperation(int lock);






static unsigned long long int __ullAtomicAdd(unsigned long long int *address, unsigned long long int val)
{
  unsigned long long int old;

  __cudaMutexOperation(1); old = *address; *address = old + val; __cudaMutexOperation(0);
#line 133 "c:\\cuda\\include\\sm_12_atomic_functions.h"
  return old;
}

static unsigned long long int __ullAtomicExch(unsigned long long int *address, unsigned long long int val)
{
  unsigned long long int old;

  __cudaMutexOperation(1); old = *address; *address = val; __cudaMutexOperation(0);
#line 144 "c:\\cuda\\include\\sm_12_atomic_functions.h"
  return old;
}

static unsigned long long int __ullAtomicCAS(unsigned long long int *address, unsigned long long int compare, unsigned long long int val)
{
  unsigned long long int old;

  __cudaMutexOperation(1); old = *address; *address = old == compare ? val : old; __cudaMutexOperation(0);
#line 155 "c:\\cuda\\include\\sm_12_atomic_functions.h"
  return old;
}



#line 161 "c:\\cuda\\include\\sm_12_atomic_functions.h"

static int __any(int cond)
{
  return cond;
}

static int __all(int cond)
{
  return cond;
}

#line 173 "c:\\cuda\\include\\sm_12_atomic_functions.h"

#line 175 "c:\\cuda\\include\\sm_12_atomic_functions.h"

#line 177 "c:\\cuda\\include\\sm_12_atomic_functions.h"
#line 4669 "c:\\cuda\\include\\device_functions.h"
#line 1 "c:\\cuda\\include\\sm_13_double_functions.h"





























































































































































































































































#line 255 "c:\\cuda\\include\\sm_13_double_functions.h"

#line 1 "c:\\cuda\\include\\crt/func_macro.h"
































































































#line 98 "c:\\cuda\\include\\crt/func_macro.h"
#line 257 "c:\\cuda\\include\\sm_13_double_functions.h"








#line 1 "c:\\cuda\\include\\common_types.h"


































































#line 68 "c:\\cuda\\include\\common_types.h"
#line 266 "c:\\cuda\\include\\sm_13_double_functions.h"

static double __longlong_as_double(long long int a)
{
  volatile union __cudart_DoubleLonglongCvt u;
  u.i = a;
  return u.d;
}

static long long int __double_as_longlong(double a)
{
  volatile union __cudart_DoubleLonglongCvt u;
  u.d = a;
  return u.i;
}

static float __internal_double2float_kernel(double a)
{
  volatile union __cudart_DoubleUlonglongCvt xx;
  volatile union __cudart_FloatUintCvt res;
  
  int shift;
  xx.d = a;
  if (xx.i == 0) return 0.0f;
  res.i = (((unsigned int) (xx.i >> 32)) & 0x80000000);
  if ((xx.i & 0x7ff0000000000000ULL) == 0x7ff0000000000000ULL) {
    if ((xx.i & 0x7fffffffffffffffULL) > 0x7ff0000000000000ULL) {
      
      res.i = ((unsigned int)((xx.i >> 32) & 0x80000000) |
               (255U << 23) | 0x00400000 |
               (unsigned int)((xx.i >> (53 - 24)) & 0x007fffff));
    } else {
      
      res.i |= 0x7f800000;
    }
    return res.f;
  }
  shift = ((int) ((xx.i >> 52) & 0x7ff)) - 1023;
  
  xx.i = (xx.i & 0x000fffffffffffffULL);
  if (shift >= 128) {
    res.i |= 0x7f7fffff;
    return res.f;
  }
  if (shift <= -127) {
    if (shift < -180) {
      
      xx.i = 0;
    } else {
      xx.i |= 0x0010000000000000ULL;
      xx.i >>= -126 - shift;
    }
  } else {
    res.i |= (unsigned int) (127 + shift) << 23;
  }
  res.i |= ((unsigned int) (xx.i >> 29)) & 0x007fffff;
  xx.i &= 0x1fffffff;
  return res.f;
}

static double __internal_ll2double_kernel(long long int a, enum cudaRoundMode rndMode)
{
  volatile union __cudart_DoubleUlonglongCvt res;
  int shift;
  unsigned int t;
  res.i = a;
  if (a == 0) return res.d;
  if (a < 0) res.i = (unsigned long long int)-a;
  shift = __internal_normalize64((unsigned long long int*)&res.i);
  t = ((unsigned int) res.i) << 21;
  res.i >>= 11;
  res.i += ((unsigned long long int)(1023 + 62 - shift)) << 52;
  if (a < 0) res.i |= 0x8000000000000000ULL;
  if ((rndMode == cudaRoundNearest) && (t >= 0x80000000)) {
    res.i += (t == 0x80000000) ? (res.i & 1) : 1;
  }
  else if ((rndMode == cudaRoundMinInf) && t && (a < 0)) {
    res.i++;
  }
  else if ((rndMode == cudaRoundPosInf) && t && (a > 0)) {
    res.i++;
  }
  return res.d;
}

static double __internal_ull2double_kernel(unsigned long long int a, enum cudaRoundMode rndMode)
{
  volatile union __cudart_DoubleUlonglongCvt res;
  int shift;
  unsigned int t;
  res.i = a;
  if (a == 0) return res.d;
  shift = __internal_normalize64((unsigned long long int *)&res.i);
  t = ((unsigned int) res.i) << 21;
  res.i >>= 11;
  res.i += ((unsigned long long int)(1023 + 62 - shift)) << 52;
  if ((rndMode == cudaRoundNearest) && (t >= 0x80000000)) {
    res.i += (t == 0x80000000) ? (res.i & 1) : 1;
  }
  else if ((rndMode == cudaRoundPosInf) && t) {
    res.i++;
  }
  return res.d;
}

static long long int __internal_double2ll_kernel(double a, long long int max, long long int min, long long int nan, enum cudaRoundMode rndMode)
{
  volatile union __cudart_DoubleUlonglongCvt xx, res;
  unsigned long long int t = 0;
  int shift;

  xx.d = a;
  if (sizeof(a) == sizeof(double) && __cuda___isnan((double)a)) return nan; if (sizeof(a) == sizeof(float) && __cuda___isnanf((float)a)) return nan; if (a >= max) return max; if (a <= min) return min;
  shift = (int) (1023 + 62 - ((xx.i >> 52) & 0x7ff));
  res.i = ((xx.i << 11) | 0x8000000000000000ULL) >> 1;
  if (shift >= 64) { 
    t = res.i;
    res.i = 0;
  } else if (shift) {
    t = res.i << (64 - shift);
    res.i = res.i >> shift;
  }
  if ((rndMode == cudaRoundNearest) && (t >= 0x8000000000000000ULL)) {
    res.i += (t == 0x8000000000000000ULL) ? (res.i & 1) : 1;
  }
  else if ((rndMode ==  cudaRoundMinInf) && t && 
          (xx.i > 0x8000000000000000ULL)) {
    res.i++;
  }
  else if ((rndMode == cudaRoundPosInf) && t && ((long long int)xx.i > 0)) {
    res.i++;
  }
  if ((long long int)xx.i < 0) {
    res.i = (unsigned long long int)(-(long long int)res.i);
  }
  return res.i;
}

static unsigned long long int __internal_double2ull_kernel(double a, unsigned long long int max, unsigned long long int nan, enum cudaRoundMode rndMode)
{
  volatile union __cudart_DoubleUlonglongCvt xx, res;
  unsigned long long int t = 0;
  int shift;

  xx.d = a;
  if (sizeof(a) == sizeof(double) && __cuda___isnan((double)a)) return nan; if (sizeof(a) == sizeof(float) && __cuda___isnanf((float)a)) return nan; if (a >= max) return max; if (a <= 0LL) return 0LL;

  if (a == 0.0) return 0LL;
  shift = (int) (1023 + 63 - ((xx.i >> 52) & 0x7ff));
  res.i = ((xx.i << 11) | 0x8000000000000000ULL);
  if (shift >= 64) { 
    t = res.i >> (int)(shift > 64); 
    res.i = 0; 
  } else if (shift) {
    t = res.i << (64 - shift);
    res.i = res.i >> shift;
  }
  if ((rndMode == cudaRoundNearest) && (t >= 0x8000000000000000ULL)) {
    res.i += (t == 0x8000000000000000ULL) ? (res.i & 1) : 1;
  }
  else if ((rndMode == cudaRoundPosInf) && t) {
    res.i++;
  }
  return res.i;
}

static int __double2hiint(double a)
{
  volatile union __cudart_DoubleInthiloCvt cvt;
  
  cvt.d = a;
  return cvt.i[1];
}

static int __double2loint(double a)
{
  volatile union __cudart_DoubleInthiloCvt cvt;
  
  cvt.d = a;
  return cvt.i[0];
}

static double __hiloint2double(int a, int b)
{
  volatile union __cudart_DoubleInthiloCvt cvt;
  
  cvt.i[0] = b;
  cvt.i[1] = a;
  return cvt.d;
}

static float __double2float_rn(double a)
{
  return (float)a;
}

static float __double2float_rz(double a)
{
  return __internal_double2float_kernel(a);
}

static int __internal_double2int(double a, enum cudaRoundMode rndMode)
{
  return (int)__internal_double2ll_kernel(a, 2147483647LL, -2147483648LL, -2147483648LL, rndMode);
}

static int __double2int_rn(double a)
{
  return __internal_double2int(a, cudaRoundNearest);
}

static int __double2int_ru(double a)
{
  return __internal_double2int(a, cudaRoundPosInf);
}

static int __double2int_rd(double a)
{
  return __internal_double2int(a, cudaRoundMinInf);
}

static unsigned int __internal_double2uint(double a, enum cudaRoundMode rndMode)
{
  return (unsigned int)__internal_double2ull_kernel(a, 4294967295ULL, 2147483648ULL, rndMode);
}

static unsigned int __double2uint_rn(double a)
{
  return __internal_double2uint(a, cudaRoundNearest);
}

static unsigned int __double2uint_ru(double a)
{
  return __internal_double2uint(a, cudaRoundPosInf);
}

static unsigned int __double2uint_rd(double a)
{
  return __internal_double2uint(a, cudaRoundMinInf);
}

static long long int __internal_double2ll(double a, enum cudaRoundMode rndMode)
{
  return __internal_double2ll_kernel(a, 9223372036854775807LL, -9223372036854775807LL -1LL, -9223372036854775807LL -1LL, rndMode);
}

static long long int __double2ll_rn(double a)
{
  return __internal_double2ll(a, cudaRoundNearest);
}

static long long int __double2ll_ru(double a)
{
  return __internal_double2ll(a, cudaRoundPosInf);
}

static long long int __double2ll_rd(double a)
{
  return __internal_double2ll(a, cudaRoundMinInf);
}

static unsigned long long int __internal_double2ull(double a, enum cudaRoundMode rndMode)
{
  return __internal_double2ull_kernel(a, 18446744073709551615ULL, 9223372036854775808ULL, rndMode);
}

static unsigned long long int __double2ull_rn(double a)
{
  return __internal_double2ull(a, cudaRoundNearest);
}

static unsigned long long int __double2ull_ru(double a)
{
  return __internal_double2ull(a, cudaRoundPosInf);
}

static unsigned long long int __double2ull_rd(double a)
{
  return __internal_double2ull(a, cudaRoundMinInf);
}

static double __int2double_rn(int a)
{
  return (double)a;
}

static double __uint2double_rn(unsigned int a)
{
  return (double)a;
}

static double __ll2double_rn(long long int a)
{
  return (double)a;
}

static double __ll2double_rz(long long int a)
{
  return __internal_ll2double_kernel(a, cudaRoundZero);
}

static double __ll2double_rd(long long int a)
{
  return __internal_ll2double_kernel(a, cudaRoundMinInf);
}

static double __ll2double_ru(long long int a)
{
  return __internal_ll2double_kernel(a, cudaRoundPosInf);
}

static double __ull2double_rn(unsigned long long int a)
{
  return __internal_ull2double_kernel(a, cudaRoundNearest);
}

static double __ull2double_rz(unsigned long long int a)
{
  return __internal_ull2double_kernel(a, cudaRoundZero);
}

static double __ull2double_rd(unsigned long long int a)
{
  return __internal_ull2double_kernel(a, cudaRoundMinInf);
}

static double __ull2double_ru(unsigned long long int a)
{
  return __internal_ull2double_kernel(a, cudaRoundPosInf);
}

#line 597 "c:\\cuda\\include\\sm_13_double_functions.h"



#line 1 "c:\\cuda\\include\\common_types.h"


































































#line 68 "c:\\cuda\\include\\common_types.h"
#line 601 "c:\\cuda\\include\\sm_13_double_functions.h"

static double __internal_fma_kernel(double x, double y, double z, enum cudaRoundMode rndMode)
{



  struct __cudart_UintUint xx, yy, zz, ww;
  unsigned int s, t, u, prod0, prod1, prod2, prod3, expo_x, expo_y, expo_z;
  
  xx.hi = __double2hiint(x);
  xx.lo = __double2loint(x);
  yy.hi = __double2hiint(y);
  yy.lo = __double2loint(y);
  zz.hi = __double2hiint(z);
  zz.lo = __double2loint(z);

  expo_z = 0x7FF;
  t =  xx.hi >> 20;
  expo_x = expo_z & t;
  expo_x = expo_x - 1;    
  t =  yy.hi >> 20;
  expo_y = expo_z & t;
  expo_y = expo_y - 1;    
  t =  zz.hi >> 20;
  expo_z = expo_z & t;
  expo_z = expo_z - 1;    

  if (!((expo_x <= 0x7FD) &&
        (expo_y <= 0x7FD) &&
        (expo_z <= 0x7FD))) {
    
    



    if (((yy.hi << 1) | (yy.lo != 0)) > 0xffe00000) {
      yy.hi |= 0x00080000;
      return __hiloint2double(yy.hi, yy.lo);
    }
    if (((zz.hi << 1) | (zz.lo != 0)) > 0xffe00000) {
      zz.hi |= 0x00080000;
      return __hiloint2double(zz.hi, zz.lo);
    }
    if (((xx.hi << 1) | (xx.lo != 0)) > 0xffe00000) {
      xx.hi |= 0x00080000;
      return __hiloint2double(xx.hi, xx.lo);
    }
    
    










    if (((((xx.hi << 1) | xx.lo) == 0) && 
         (((yy.hi << 1) | (yy.lo != 0)) == 0xffe00000)) ||
        ((((yy.hi << 1) | yy.lo) == 0) && 
         (((xx.hi << 1) | (xx.lo != 0)) == 0xffe00000))) {
      xx.hi = 0xfff80000;
      xx.lo = 0x00000000;
      return __hiloint2double(xx.hi, xx.lo);
    }
    if (((zz.hi << 1) | (zz.lo != 0)) == 0xffe00000) {
      if ((((yy.hi << 1) | (yy.lo != 0)) == 0xffe00000) ||
          (((xx.hi << 1) | (xx.lo != 0)) == 0xffe00000)) {
        if ((int)(xx.hi ^ yy.hi ^ zz.hi) < 0) {
          xx.hi = 0xfff80000;
          xx.lo = 0x00000000;
          return __hiloint2double(xx.hi, xx.lo);
        }
      }
    }
    



    if (((xx.hi << 1) | (xx.lo != 0)) == 0xffe00000) {
      xx.hi = xx.hi ^ (yy.hi & 0x80000000);
      return __hiloint2double(xx.hi, xx.lo);
    }
    if (((yy.hi << 1) | (yy.lo != 0)) == 0xffe00000) {
      yy.hi = yy.hi ^ (xx.hi & 0x80000000);
      return __hiloint2double(yy.hi, yy.lo);
    }
    if (((zz.hi << 1) | (zz.lo != 0)) == 0xffe00000) {
      return __hiloint2double(zz.hi, zz.lo);
    }
    




    if ((zz.hi == 0x80000000) && (zz.lo == 0)) {
      if ((((xx.hi << 1) | xx.lo) == 0) ||
          (((yy.hi << 1) | yy.lo) == 0)) {
        if ((int)(xx.hi ^ yy.hi) < 0) {
          return __hiloint2double(zz.hi, zz.lo);
        }
      }
    }
    


    if ((((zz.hi << 1) | zz.lo) == 0) &&
        ((((xx.hi << 1) | xx.lo) == 0) ||
         (((yy.hi << 1) | yy.lo) == 0))) {
      if (rndMode == cudaRoundMinInf) {
        return __hiloint2double((xx.hi ^ yy.hi ^ zz.hi) & 0x80000000, zz.lo);
      } else {
        zz.hi &= 0x7fffffff;
        return __hiloint2double(zz.hi, zz.lo);
      }
    }
    
    


    if ((((xx.hi << 1) | xx.lo) == 0) ||
        (((yy.hi << 1) | yy.lo) == 0)) {
      return __hiloint2double(zz.hi, zz.lo);
    }
    
    if (expo_x == 0xffffffff) {
      expo_x++;
      t = xx.hi & 0x80000000;
      s = xx.lo >> 21;
      xx.lo = xx.lo << 11;
      xx.hi = xx.hi << 11;
      xx.hi = xx.hi | s;
      if (!xx.hi) {
        xx.hi = xx.lo;
        xx.lo = 0;
        expo_x -= 32;
      }
      while ((int)xx.hi > 0) {
        s = xx.lo >> 31;
        xx.lo = xx.lo + xx.lo;
        xx.hi = xx.hi + xx.hi;
        xx.hi = xx.hi | s;
        expo_x--;
      }
      xx.lo = (xx.lo >> 11);
      xx.lo |= (xx.hi << 21);
      xx.hi = (xx.hi >> 11) | t;
    }
    if (expo_y == 0xffffffff) {
      expo_y++;
      t = yy.hi & 0x80000000;
      s = yy.lo >> 21;
      yy.lo = yy.lo << 11;
      yy.hi = yy.hi << 11;
      yy.hi = yy.hi | s;
      if (!yy.hi) {
        yy.hi = yy.lo;
        yy.lo = 0;
        expo_y -= 32;
      }
      while ((int)yy.hi > 0) {
        s = yy.lo >> 31;
        yy.lo = yy.lo + yy.lo;
        yy.hi = yy.hi + yy.hi;
        yy.hi = yy.hi | s;
        expo_y--;
      }
      yy.lo = (yy.lo >> 11);
      yy.lo |= (yy.hi << 21);
      yy.hi = (yy.hi >> 11) | t;
    }
    if (expo_z == 0xffffffff) {
      expo_z++;
      t = zz.hi & 0x80000000;
      s = zz.lo >> 21;
      zz.lo = zz.lo << 11;
      zz.hi = zz.hi << 11;
      zz.hi = zz.hi | s;
      if (!zz.hi) {
        zz.hi = zz.lo;
        zz.lo = 0;
        expo_z -= 32;
      }
      while ((int)zz.hi > 0) {
        s = zz.lo >> 31;
        zz.lo = zz.lo + zz.lo;
        zz.hi = zz.hi + zz.hi;
        zz.hi = zz.hi | s;
        expo_z--;
      }
      zz.lo = (zz.lo >> 11);
      zz.lo |= (zz.hi << 21);
      zz.hi = (zz.hi >> 11) | t;
    }
  }
  
  expo_x = expo_x + expo_y;
  expo_y = xx.hi ^ yy.hi;
  t = xx.lo >> 21;
  xx.lo = xx.lo << 11;
  xx.hi = xx.hi << 11;
  xx.hi = xx.hi | t;
  yy.hi = yy.hi & 0x000fffff;
  xx.hi = xx.hi | 0x80000000; 
  yy.hi = yy.hi | 0x00100000; 

  prod0 = xx.lo * yy.lo;
  prod1 = __umulhi (xx.lo, yy.lo);
  prod2 = xx.hi * yy.lo;
  prod3 = xx.lo * yy.hi;
  prod1 += prod2;
  t = prod1 < prod2;
  prod1 += prod3;
  t += prod1 < prod3;
  prod2 = __umulhi (xx.hi, yy.lo);
  prod3 = __umulhi (xx.lo, yy.hi);
  prod2 += prod3;
  s = prod2 < prod3;
  prod3 = xx.hi * yy.hi;
  prod2 += prod3;
  s += prod2 < prod3;
  prod2 += t;
  s += prod2 < t;
  prod3 = __umulhi (xx.hi, yy.hi) + s;
  
  yy.lo = prod0;                 
  yy.hi = prod1;                 
  xx.lo = prod2;                 
  xx.hi = prod3;                 
  expo_x = expo_x - (1023 - 2);  
  expo_y = expo_y & 0x80000000;  

  if (xx.hi < 0x00100000) {
    s = xx.lo >> 31;
    s = (xx.hi << 1) + s;
    xx.hi = s;
    s = yy.hi >> 31;
    s = (xx.lo << 1) + s;
    xx.lo = s;
    s = yy.lo >> 31;
    s = (yy.hi << 1) + s;
    yy.hi = s;
    s = yy.lo << 1;
    yy.lo = s;
    expo_x--;
  }

  t = 0;
  if (((zz.hi << 1) | zz.lo) != 0) { 
    
    s = zz.hi & 0x80000000;
    
    zz.hi &= 0x000fffff;
    zz.hi |= 0x00100000;
    ww.hi = 0;
    ww.lo = 0;
    
    
    if ((int)expo_z > (int)expo_x) {
      t = expo_z;
      expo_z = expo_x;
      expo_x = t;
      t = zz.hi;
      zz.hi = xx.hi;
      xx.hi = t;
      t = zz.lo;
      zz.lo = xx.lo;
      xx.lo = t;
      t = ww.hi;
      ww.hi = yy.hi;
      yy.hi = t;
      t = ww.lo;
      ww.lo = yy.lo;
      yy.lo = t;
      t = expo_y;
      expo_y = s;
      s = t;
    }
    
    
    
    expo_z = expo_x - expo_z;
    u = expo_y ^ s;
    if (expo_z <= 107) {
      
      t = 0;
      while (expo_z >= 32) {
        t     = ww.lo | (t != 0);
        ww.lo = ww.hi;
        ww.hi = zz.lo;
        zz.lo = zz.hi;
        zz.hi = 0;
        expo_z -= 32;
      }
      if (expo_z) {
        t     = (t     >> expo_z) | (ww.lo << (32 - expo_z)) | 
                ((t << (32 - expo_z)) != 0);
        ww.lo = (ww.lo >> expo_z) | (ww.hi << (32 - expo_z));
        ww.hi = (ww.hi >> expo_z) | (zz.lo << (32 - expo_z));
        zz.lo = (zz.lo >> expo_z) | (zz.hi << (32 - expo_z));
        zz.hi = (zz.hi >> expo_z);
      }
    } else {
      t = 1;
      ww.lo = 0;
      ww.hi = 0;
      zz.lo = 0;
      zz.hi = 0;
    }
    if ((int)u < 0) {
      
      t = (unsigned)(-(int)t);
      s = (t != 0);
      u = yy.lo - s;
      s = u > yy.lo;
      yy.lo = u - ww.lo;
      s += yy.lo > u;
      u = yy.hi - s;
      s = u > yy.hi;
      yy.hi = u - ww.hi;
      s += yy.hi > u;
      u = xx.lo - s;
      s = u > xx.lo;
      xx.lo = u - zz.lo;
      s += xx.lo > u;
      xx.hi = (xx.hi - zz.hi) - s;
      if (!(xx.hi | xx.lo | yy.hi | yy.lo | t)) {
        
        if (rndMode == cudaRoundMinInf) {
          return __hiloint2double(0x80000000, xx.lo);
        } else {
          return __hiloint2double(xx.hi, xx.lo);
        }
      }
      if ((int)xx.hi < 0) {
        


        t = ~t;
        yy.lo = ~yy.lo;
        yy.hi = ~yy.hi;
        xx.lo = ~xx.lo;
        xx.hi = ~xx.hi;
        if (++t == 0) {
          if (++yy.lo == 0) {
            if (++yy.hi == 0) {
              if (++xx.lo == 0) {
              ++xx.hi;
              }
            }
          }
        }
        expo_y ^= 0x80000000;
      }
        
      
      while (!(xx.hi & 0x00100000)) {
        xx.hi = (xx.hi << 1) | (xx.lo >> 31);
        xx.lo = (xx.lo << 1) | (yy.hi >> 31);
        yy.hi = (yy.hi << 1) | (yy.lo >> 31);
        yy.lo = (yy.lo << 1);
        expo_x--;
      }
    } else {
      
      yy.lo = yy.lo + ww.lo;
      s = yy.lo < ww.lo;
      yy.hi = yy.hi + s;
      u = yy.hi < s;
      yy.hi = yy.hi + ww.hi;
      u += yy.hi < ww.hi;
      xx.lo = xx.lo + u;
      s = xx.lo < u;
      xx.lo = xx.lo + zz.lo;
      s += xx.lo < zz.lo;
      xx.hi = xx.hi + zz.hi + s;
      if (xx.hi & 0x00200000) {
        t = t | (yy.lo << 31);
        yy.lo = (yy.lo >> 1) | (yy.hi << 31);
        yy.hi = (yy.hi >> 1) | (xx.lo << 31);
        xx.lo = (xx.lo >> 1) | (xx.hi << 31);
        xx.hi = ((xx.hi & 0x80000000) | (xx.hi >> 1)) & ~0x40000000;
        expo_x++;
      }
    }
  }
  t = yy.lo | (t != 0);
  t = yy.hi | (t != 0);
        
  xx.hi |= expo_y; 
  if (expo_x <= 0x7FD) {
    
    xx.hi = xx.hi & ~0x00100000; 
    s = xx.lo & 1; 
    u = xx.lo;
    if (rndMode == cudaRoundNearest) {
      xx.lo += (t == 0x80000000) ? s : (t >> 31);
    } else if (((rndMode == cudaRoundPosInf) && t && (!expo_y)) ||
               ((rndMode == cudaRoundMinInf) && t && expo_y)) {
      xx.lo += 1;
    }
    xx.hi += (u > xx.lo);
    xx.hi += ((expo_x + 1) << 20);
    return __hiloint2double(xx.hi, xx.lo);
  } else if ((int)expo_x >= 2046) {      
    
    if ((rndMode == cudaRoundNearest) || 
        ((rndMode == cudaRoundPosInf) && (!expo_y)) ||
        ((rndMode == cudaRoundMinInf) && expo_y)) {
      xx.hi = (xx.hi & 0x80000000) | 0x7ff00000;
      xx.lo = 0;
    } else {
      xx.hi = (xx.hi & 0x80000000) | 0x7fefffff;
      xx.lo = 0xffffffff;
    }
    return __hiloint2double(xx.hi, xx.lo);
  }
  
  expo_x = (unsigned)(-(int)expo_x);
  if (expo_x > 54) {
    
    if (((rndMode == cudaRoundPosInf) && (!expo_y)) ||
        ((rndMode == cudaRoundMinInf) && expo_y)) {
      return __hiloint2double(xx.hi & 0x80000000, 1);
    } else {
      return __hiloint2double(xx.hi & 0x80000000, 0);
    }
  }  
  yy.hi = xx.hi &  0x80000000;   
  xx.hi = xx.hi & ~0xffe00000;
  if (expo_x >= 32) {
    t = xx.lo | (t != 0);
    xx.lo = xx.hi;
    xx.hi = 0;
    expo_x -= 32;
  }
  if (expo_x) {
    t     = (t     >> expo_x) | (xx.lo << (32 - expo_x)) | (t != 0);
    xx.lo = (xx.lo >> expo_x) | (xx.hi << (32 - expo_x));
    xx.hi = (xx.hi >> expo_x);
  }
  expo_x = xx.lo & 1; 
  u = xx.lo;
  if (rndMode == cudaRoundNearest) {
    xx.lo += (t == 0x80000000) ? expo_x : (t >> 31);
  } else if (((rndMode == cudaRoundPosInf) && t && (!expo_y)) ||
             ((rndMode == cudaRoundMinInf) && t && expo_y)) {
    xx.lo += 1;
  }
  xx.hi += (u > xx.lo);
  xx.hi |= yy.hi;
  return __hiloint2double(xx.hi, xx.lo);
}

static double __fma_rn(double x, double y, double z)
{
  return __internal_fma_kernel(x, y, z, cudaRoundNearest);
}

static double __fma_rd(double x, double y, double z)
{
  return __internal_fma_kernel(x, y, z, cudaRoundMinInf);
}

static double __fma_ru(double x, double y, double z)
{
  return __internal_fma_kernel(x, y, z, cudaRoundPosInf);
}

static double __fma_rz(double x, double y, double z)
{
  return __internal_fma_kernel(x, y, z, cudaRoundZero);
}

static double __dadd_rz(double a, double b)
{
  return __fma_rz(a, 1.0, b); 
}

static double __dadd_ru(double a, double b)
{
  return __fma_ru(a, 1.0, b); 
}

static double __dadd_rd(double a, double b)
{
  return __fma_rd(a, 1.0, b); 
}

static double __dmul_rz(double a, double b)
{
  return __fma_rz(a, b, __hiloint2double(0x80000000, 0x00000000)); 
}

static double __dmul_ru(double a, double b)
{
  return __fma_ru(a, b, __hiloint2double(0x80000000, 0x00000000)); 
}

static double __dmul_rd(double a, double b)
{
  return __fma_rd(a, b, 0.0); 
}

static double __dadd_rn(double a, double b)
{
  return __fma_rn(a, 1.0, b); 
}

static double __dmul_rn(double a, double b)
{
  return __fma_rn(a, b, __hiloint2double(0x80000000, 0x00000000)); 
}

#line 1118 "c:\\cuda\\include\\sm_13_double_functions.h"







#line 1126 "c:\\cuda\\include\\sm_13_double_functions.h"

#line 1128 "c:\\cuda\\include\\sm_13_double_functions.h"

#line 4670 "c:\\cuda\\include\\device_functions.h"
#line 1 "c:\\cuda\\include\\texture_fetch_functions.h"


















































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































#line 1876 "c:\\cuda\\include\\texture_fetch_functions.h"

#line 1 "c:\\cuda\\include\\host_defines.h"




































































































































#line 134 "c:\\cuda\\include\\host_defines.h"
#line 1878 "c:\\cuda\\include\\texture_fetch_functions.h"
#line 1 "c:\\cuda\\include\\crt/func_macro.h"
































































































#line 98 "c:\\cuda\\include\\crt/func_macro.h"
#line 1879 "c:\\cuda\\include\\texture_fetch_functions.h"





























#line 1909 "c:\\cuda\\include\\texture_fetch_functions.h"

extern void __stdcall __cudaTextureFetch(const void *tex, void *index, int integer, void *val);

static int4 __itexfetchi(const void *tex, int4 index)
{
  int4 val;

  __cudaTextureFetch(tex, (void*)&index, 1, (void*)&val);

  return val;
}

static uint4 __utexfetchi(const void *tex, int4 index)
{
  uint4 val;

  __cudaTextureFetch(tex, (void*)&index, 1, (void*)&val);

  return val;
}

static float4 __ftexfetchi(const void *tex, int4 index)
{
  float4 val;

  __cudaTextureFetch(tex, (void*)&index, 1, (void*)&val);

  return val;
}

static int4 __itexfetch(const void *tex, float4 index, int dim)
{
  int4 val;

  __cudaTextureFetch(tex, (void*)&index, 0, (void*)&val);

  return val;
}

static uint4 __utexfetch(const void *tex, float4 index, int dim)
{
  uint4 val;

  __cudaTextureFetch(tex, (void*)&index, 0, (void*)&val);

  return val;
}

static float4 __ftexfetch(const void *tex, float4 index, int dim)
{
  float4 val;

  __cudaTextureFetch(tex, (void*)&index, 0, (void*)&val);

  return val;
}

#line 1967 "c:\\cuda\\include\\texture_fetch_functions.h"

#line 1969 "c:\\cuda\\include\\texture_fetch_functions.h"

#line 1971 "c:\\cuda\\include\\texture_fetch_functions.h"

#line 4671 "c:\\cuda\\include\\device_functions.h"

#line 4673 "c:\\cuda\\include\\device_functions.h"

#line 997 "c:\\cuda\\include\\math_functions.h"
#line 1 "c:\\cuda\\include\\math_constants.h"











































































































































#line 141 "c:\\cuda\\include\\math_constants.h"
#line 998 "c:\\cuda\\include\\math_functions.h"

static int __cuda___signbitf(float a)
{
  return (int)((unsigned int)__float_as_int(a) >> 31);
}




static float __cuda_copysignf(float a, float b)
{
  return __int_as_float((__float_as_int(b) &  0x80000000) | 
                        (__float_as_int(a) & ~0x80000000));
}











static int min(int a, int b)
{
  return a < b ? a : b;
}

static unsigned int umin(unsigned int a, unsigned int b)
{
  return a < b ? a : b;
}

static long long int llmin(long long int a, long long int b)
{
  return a < b ? a : b;
}

static unsigned long long int ullmin(unsigned long long int a, unsigned long long int b)
{
  return a < b ? a : b;
}

static int max(int a, int b)
{
  return a > b ? a : b;
}

static unsigned int umax(unsigned int a, unsigned int b)
{
  return a > b ? a : b;
}

static long long int llmax(long long int a, long long int b)
{
  return a > b ? a : b;
}

static unsigned long long int ullmax(unsigned long long int a, unsigned long long int b)
{
  return a > b ? a : b;
}







static double fmax(double a, double b)
{
  return a > b ? a : b;
}

static double fmin(double a, double b)
{
  return a < b ? a : b;
}

static float fmaxf(float a, float b)
{
  return (float)fmax((double)a, (double)b);
}

static float fminf(float a, float b)
{
  return (float)fmin((double)a, (double)b);
}

static int __signbit(double a)
{
  volatile union {
    double               d;
    signed long long int l;
  } cvt;

  cvt.d = a;

  return cvt.l < 0ll;
}

static double copysign(double a, double b)
{
  volatile union {
    double                 d;
    unsigned long long int l;
  } cvta, cvtb;

  cvta.d = a;
  cvtb.d = b;
  cvta.l = (cvta.l & 0x7fffffffffffffffULL) | (cvtb.l & 0x8000000000000000ULL);

  return cvta.d;
}

static int __signbitf(float a)
{
  return __cuda___signbitf(a);
}

static float copysignf(float a, float b)
{
  return __cuda_copysignf(a, b);
}



#line 1128 "c:\\cuda\\include\\math_functions.h"







static float __internal_nearbyintf(float a)
{
  float fa = fabsf(a);

  if (fa >= 8388608.0f) {
    return a;
  } else {
    volatile float u = 8388608.0f + fa;

    u = u - 8388608.0f;
    return copysignf(u, a);
  }
}

static float __internal_fminf(float a, float b)
{
  volatile union {
    float        f;
    unsigned int i;
  } cvta, cvtb;
  int nana, nanb;

  cvta.f = a;
  cvtb.f = b;
  nana = ((cvta.i << 1) > 0xff000000);
  nanb = ((cvtb.i << 1) > 0xff000000);
  if (nana && nanb) return a + b;
  if (nana) return b;
  if (nanb) return a;
  if ((cvta.i | cvtb.i) == 0x80000000) {
    return __int_as_float(0x80000000);
  }
  return a < b ? a : b;
}

static float __internal_fmaxf(float a, float b)
{
  volatile union {
    float        f;
    unsigned int i;
  } cvta, cvtb;
  int nana, nanb;

  cvta.f = a;
  cvtb.f = b;
  nana = ((cvta.i << 1) > 0xff000000);
  nanb = ((cvtb.i << 1) > 0xff000000);
  if (nana && nanb) return a + b;
  if (nana) return b;
  if (nanb) return a;
  if ((cvta.f == 0.0f) && (cvtb.f == 0.0f)) {
    cvta.i &= cvtb.i;
    return cvta.f;
  }
  return a > b ? a : b;
}





static double trunc(double a)
{
  return a < 0.0 ? ceil(a) : floor(a);
}

static double nearbyint(double a)
{
  double fa = fabs(a);
  if (fa >= 4503599627370496.0) {
    return a;
  } else {
    double u = 4503599627370496.0 + fa;
    u = u - 4503599627370496.0;
    return copysign(u, a);
  }
}

static float truncf(float a)
{
  return (float)trunc((double)a);
}

static float nearbyintf(float a)
{
  return __internal_nearbyintf(a);
}



#line 1226 "c:\\cuda\\include\\math_functions.h"

#line 1228 "c:\\cuda\\include\\math_functions.h"







static long int __cuda_labs(long int a)
{
  return labs(a);
}

static float __cuda_ceilf(float a)
{
  return ceilf(a);
}

static float __cuda_floorf(float a)
{
  return floorf(a);
}

static float __cuda_sqrtf(float a)
{
  return sqrtf(a);
}

static float __cuda_rsqrtf(float a)
{
  return rsqrtf(a);
}

static float __cuda_truncf(float a)
{
  return truncf(a);
}

static int __cuda_max(int a, int b)
{
  return max(a, b);
}

static int __cuda_min(int a, int b)
{
  return min(a, b);
}

static unsigned int __cuda_umax(unsigned int a, unsigned int b)
{
  return umax(a, b);
}

static unsigned int __cuda_umin(unsigned int a, unsigned int b)
{
  return umin(a, b);
}

static long long int __cuda_llmax(long long int a, long long int b)
{
  return llmax(a, b);
}

static long long int __cuda_llmin(long long int a, long long int b)
{
  return llmin(a, b);
}

static unsigned long long int __cuda_ullmax(unsigned long long int a, unsigned long long int b)
{
  return ullmax(a, b);
}

static unsigned long long int __cuda_ullmin(unsigned long long int a, unsigned long long int b)
{
  return ullmin(a, b);
}

static long long int __cuda_llrintf(float a)
{


#line 1310 "c:\\cuda\\include\\math_functions.h"
  return __float2ll_rn(a);
#line 1312 "c:\\cuda\\include\\math_functions.h"
}

static long int __cuda_lrintf(float a)
{


#line 1319 "c:\\cuda\\include\\math_functions.h"


#line 1322 "c:\\cuda\\include\\math_functions.h"
  return (long int)__float2int_rn(a);
#line 1324 "c:\\cuda\\include\\math_functions.h"
#line 1325 "c:\\cuda\\include\\math_functions.h"
}

static float __cuda_nearbyintf(float a)
{


#line 1332 "c:\\cuda\\include\\math_functions.h"

#line 1334 "c:\\cuda\\include\\math_functions.h"
  return __internal_nearbyintf(a);
#line 1336 "c:\\cuda\\include\\math_functions.h"
}

static float __cuda_fmaxf(float a, float b)
{


#line 1343 "c:\\cuda\\include\\math_functions.h"

#line 1345 "c:\\cuda\\include\\math_functions.h"
  return __internal_fmaxf(a, b);
#line 1347 "c:\\cuda\\include\\math_functions.h"
}

static float __cuda_fminf(float a, float b)
{


#line 1354 "c:\\cuda\\include\\math_functions.h"

#line 1356 "c:\\cuda\\include\\math_functions.h"
  return __internal_fminf(a, b);
#line 1358 "c:\\cuda\\include\\math_functions.h"
}






static int __cuda___finitef(float a)
{
  return __cuda_fabsf(a) < __int_as_float(0x7f800000);
}








#line 1378 "c:\\cuda\\include\\math_functions.h"

static int __cuda___isinff(float a)
{
  return __cuda_fabsf(a) == __int_as_float(0x7f800000);
}

static int __cuda___isnanf(float a)
{
  return !(__cuda_fabsf(a) <= __int_as_float(0x7f800000));
}

static float __cuda_nextafterf(float a, float b)
{
  unsigned int ia;
  unsigned int ib;
  ia = __float_as_int(a);
  ib = __float_as_int(b);



#line 1399 "c:\\cuda\\include\\math_functions.h"
  if (__cuda___isnanf(a) || __cuda___isnanf(b)) return a + b; 
  if (__int_as_float (ia | ib) == 0.0f) return __int_as_float(ib);




#line 1406 "c:\\cuda\\include\\math_functions.h"
  if (__int_as_float(ia) == 0.0f) {
    return __cuda_copysignf(__int_as_float(0x00000001), b); 
  }
#line 1410 "c:\\cuda\\include\\math_functions.h"
  if ((a < b) && (a < 0.0f)) ia--;
  if ((a < b) && (a > 0.0f)) ia++;
  if ((a > b) && (a < 0.0f)) ia++;
  if ((a > b) && (a > 0.0f)) ia--;
  a = __int_as_float(ia);




#line 1420 "c:\\cuda\\include\\math_functions.h"
  return a;
}

static float __cuda_nanf(const char *tagp)
{
  
  return __int_as_float(0x7fffffff);
}

static float __internal_fmad(float a, float b, float c)
{

  return __fmaf_rn (a, b, c);


#line 1436 "c:\\cuda\\include\\math_functions.h"
}


static float __internal_atanhf_kernel(float a_1, float a_2)
{
  float a, a2, t;

  a = a_1 + a_2;
  a2 = a * a;    
  t =                         1.566305595598990E-001f/64.0f;
  t = __internal_fmad (t, a2, 1.995081856004762E-001f/16.0f);
  t = __internal_fmad (t, a2, 3.333382699617026E-001f/4.0f);
  t = t * a2;
  t = __internal_fmad (t, a, a_2);
  t = t + a_1;
  return t;
}  




static float __internal_atanf_kernel(float a)
{
  float t4, t0, t1;

  t4 = a * a;
  t0 = -5.674867153f;
  t0 = __internal_fmad (t4, -0.823362947f, t0);
  t0 = __internal_fmad (t0, t4, -6.565555096f);
  t0 = t0 * t4;
  t0 = t0 * a;
  t1 = t4 + 11.33538818f;
  t1 = __internal_fmad (t1, t4, 28.84246826f);
  t1 = __internal_fmad (t1, t4, 19.69667053f);
  t1 = 1.0f / t1;
  a = __internal_fmad (t0, t1, a);
  return a;
}


static float __internal_tan_kernel(float a)
{
  float a2, s, t;

  a2 = a * a;
  t  = __internal_fmad (4.114678393115178E-003f, a2, -8.231194034909670E-001f);
  s  = a2 - 2.469348886157666E+000f;
  s  = 1.0f / s;
  t  = t * s;
  t  = t * a2;
  t  = __internal_fmad (t, a, a);
  return t;
}

static float __internal_accurate_logf(float a)
{
  float t;
  float z;
  float m;
  int ia;
  ia = __float_as_int(a);

  if ((a > 0.0f) && (a < __int_as_float(0x7f800000))) {
    int e = -127;

    
    if ((unsigned)ia < (unsigned)0x00800000) {
      a = a * 16777216.0f;
      e -= 24;
      ia = __float_as_int(a);
    }
#line 1508 "c:\\cuda\\include\\math_functions.h"
    
    m = __int_as_float((ia & 0x807fffff) | 0x3f800000);
    e += ((unsigned)ia >> 23);
    if (m > 1.414213562f) {
      m = m * 0.5f;
      e = e + 1;
    }      
    t = m - 1.0f;
    z = m + 1.0f;
    z = __fdividef (t, z);
    z = -t * z;
    z = __internal_atanhf_kernel(t, z);
    z = __internal_fmad ((float)e, 0.693147181f, z);
    return z;
  } else {
    
    return __logf(a);
  }
}  

static float2 __internal_log_ep(float a)
{
  float2 res;
  int expo;
  float m;
  float log_hi, log_lo;
  float t_hi, t_lo;
  float f, g, u, v, q;


#line 1539 "c:\\cuda\\include\\math_functions.h"
  float r, s, e;
#line 1541 "c:\\cuda\\include\\math_functions.h"
  expo = (__float_as_int(a) >> 23) & 0xff;

  
  if (expo == 0) {
    a *= 16777216.0f;
    expo = (__float_as_int(a) >> 23) & 0xff;
    expo -= 24;
  }  
#line 1550 "c:\\cuda\\include\\math_functions.h"
  expo -= 127;
  m = __int_as_float((__float_as_int(a) & 0x807fffff) | 0x3f800000);
  if (m > 1.414213562f) {
    m = m * 0.5f;
    expo = expo + 1;
  }

  




  f = m - 1.0f;
  g = m + 1.0f;
  g = 1.0f / g;
  u = 2.0f * f * g;
  v = u * u;
  q = 1.49356810919559350E-001f/64.0f;
  q = __internal_fmad (q, v, 1.99887797540072460E-001f/16.0f);
  q = __internal_fmad (q, v, 3.33333880955515580E-001f/4.0f);
  q = q * v;
  q = q * u;
  log_hi = __int_as_float(__float_as_int(u) & 0xfffff000);
  v = __int_as_float(__float_as_int(f) & 0xfffff000);
  u = 2.0f * (f - log_hi);
  f = f - v;
  u = __internal_fmad (-log_hi, v, u);
  u = __internal_fmad (-log_hi, f, u);
  u = g * u;
  

  
  r = log_hi + u;
  s = u - (r - log_hi);
  log_hi = r;
  log_lo = s;
  
  r = log_hi + q;
  s = ((log_hi - r) + q) + log_lo;
  log_hi = e = r + s;
  log_lo = (r - e) + s;

  
  t_hi = expo * 0.6931457519f;    
  t_lo = expo * 1.4286067653e-6f;

  
  r = t_hi + log_hi;
  s = (((t_hi - r) + log_hi) + log_lo) + t_lo;
  res.y = e = r + s;
  res.x = (r - e) + s;
  return res;
}

static float __internal_accurate_log2f(float a)
{
  return 1.442695041f * __internal_accurate_logf(a);
}




static float2 __internal_dsmul (float2 x, float2 y)
{
    float2 z;

    volatile float up, vp, u1, u2, v1, v2, mh, ml;


#line 1620 "c:\\cuda\\include\\math_functions.h"
    up  = x.y * 4097.0f;
    u1  = (x.y - up) + up;
    u2  = x.y - u1;
    vp  = y.y * 4097.0f;
    v1  = (y.y - vp) + vp;
    v2  = y.y - v1;
    mh  = __fmul_rn(x.y,y.y);
    ml  = (((u1 * v1 - mh) + u1 * v2) + u2 * v1) + u2 * v2;
    ml  = (__fmul_rn(x.y,y.x) + __fmul_rn(x.x,y.y)) + ml;
    z.y = up = mh + ml;
    z.x = (mh - up) + ml;
    return z;
}


static  unsigned int __cudart_i2opi_f [] = {
  0x3c439041,
  0xdb629599,
  0xf534ddc0,
  0xfc2757d1,
  0x4e441529,
  0xa2f9836e,
};


static float __internal_trig_reduction_kernel(float a, int *quadrant)
{
  float j;
  int q;
  if (__cuda_fabsf(a) > 48039.0f) {
    
    unsigned int ia = __float_as_int(a);
    unsigned int s = ia & 0x80000000;
    unsigned int result[7];
    unsigned int phi, plo;
    unsigned int hi, lo;
    unsigned int e;
    int idx;
    e = ((ia >> 23) & 0xff) - 128;
    ia = (ia << 8) | 0x80000000;
    
    idx = 4 - (e >> 5);
    hi = 0;


#line 1666 "c:\\cuda\\include\\math_functions.h"
    for (q = 0; q < 6; q++) {
      plo = __cudart_i2opi_f[q] * ia;
      phi = __umulhi (__cudart_i2opi_f[q], ia);
      lo = hi + plo;
      hi = phi + (lo < plo);
      result[q] = lo;
    }
    result[q] = hi;
    e = e & 31;
    


    hi = result[idx+2];
    lo = result[idx+1];
    if (e) {
      q = 32 - e;
      hi = (hi << e) + (lo >> q);
      lo = (lo << e) + (result[idx] >> q);
    }
    q = hi >> 30;
    
    hi = (hi << 2) + (lo >> 30);
    lo = (lo << 2);
    e = (hi + (lo > 0)) > 0x80000000; 
    q += e;
    if (s) q = -q;
    if (e) {
      unsigned int t;
      hi = ~hi;
      lo = -(int)lo;
      t = (lo == 0);
      hi += t;
      s = s ^ 0x80000000;
    }
    *quadrant = q;
    
    e = 0;
    while ((int)hi > 0) {
      hi = (hi << 1) + (lo >> 31);
      lo = (lo << 1);
      e--;
    }
    lo = hi * 0xc90fdaa2;
    hi = __umulhi(hi, 0xc90fdaa2);
    if ((int)hi > 0) {
      hi = (hi << 1) + (lo >> 31);
      lo = (lo << 1);
      e--;
    }
    hi = hi + (lo > 0);
    ia = s | (((e + 126) << 23) + (hi >> 8) + ((hi << 24) >= 0x80000000));
    return __int_as_float(ia);
  }
  q = __float2int_rn (a * 0.636619772f);
  j = (float)q;
  a = __internal_fmad (-j, 1.5703125000000000e+000f, a);

  
  a = __fmaf_rn (-j, 4.8382673412561417e-004f, a);



#line 1729 "c:\\cuda\\include\\math_functions.h"
  a = __internal_fmad (-j, 6.0771005065061922e-011f, a);
  *quadrant = q;
  return a;
}










static float __internal_expf_kernel(float a, float scale)
{
  float j, z;

  j = __cuda_truncf(a * 1.442695041f);
  z = __internal_fmad (j, -0.6931457519f,    a);
  z = __internal_fmad (j, -1.4286067653e-6f, z);
  z = z * 1.442695041f;
  z = __cuda_exp2f(z) * __cuda_exp2f(j + scale);
  return z;
}

static float __internal_accurate_expf(float a)
{
  float z;

  z = __internal_expf_kernel(a, 0.0f);
  if (a < -105.0f) z = 0.0f;
  if (a >  105.0f) z = __int_as_float(0x7f800000);
  return z;
}

static float __internal_accurate_exp10f(float a)
{
  float j, z;

  j = __cuda_truncf(a * 3.321928094f);
  z = __internal_fmad (j, -3.0102920532226563e-001f, a);
  z = __internal_fmad (j, -7.9034171557301747e-007f, z);
  z = z * 3.321928094f;
  z = __cuda_exp2f(z) * __cuda_exp2f(j);
  if (a < -46.0f) z = 0.0f;
  if (a >  46.0f) z = __int_as_float(0x7f800000);
  return z;
}

static float __internal_lgammaf_pos(float a)
{
  float sum;
  float s, t;

  if (a == __int_as_float(0x7f800000)) {
    return a;
  }
  if (a >= 3.0f) {
    if (a >= 7.8f) {
      


      s = 1.0f / a;
      t = s * s;
      sum =                           0.77783067e-3f;
      sum = __internal_fmad (sum, t, -0.2777655457e-2f);
      sum = __internal_fmad (sum, t,  0.83333273853e-1f);
      sum = __internal_fmad (sum, s,  0.918938533204672f);
      s = 0.5f * __internal_accurate_logf(a);
      t = a - 0.5f;
      s = s * t;
      t = s - a;
      s = __fadd_rn(s, sum); 
      t = t + s;
      return t;
    } else {
      a = a - 3.0f;
      s =                        -7.488903254816711E+002f;
      s = __internal_fmad (s, a, -1.234974215949363E+004f);
      s = __internal_fmad (s, a, -4.106137688064877E+004f);
      s = __internal_fmad (s, a, -4.831066242492429E+004f);
      s = __internal_fmad (s, a, -1.430333998207429E+005f);
      t =                     a - 2.592509840117874E+002f;
      t = __internal_fmad (t, a, -1.077717972228532E+004f);
      t = __internal_fmad (t, a, -9.268505031444956E+004f);
      t = __internal_fmad (t, a, -2.063535768623558E+005f);
      t = __fdividef (s, t);
      t = t + a;
      return t;
    }
  } else if (a >= 1.5f) {
    a = a - 2.0f;
    t =                         4.959849168282574E-005f;
    t = __internal_fmad (t, a, -2.208948403848352E-004f);
    t = __internal_fmad (t, a,  5.413142447864599E-004f);
    t = __internal_fmad (t, a, -1.204516976842832E-003f);
    t = __internal_fmad (t, a,  2.884251838546602E-003f);
    t = __internal_fmad (t, a, -7.382757963931180E-003f);
    t = __internal_fmad (t, a,  2.058131963026755E-002f);
    t = __internal_fmad (t, a, -6.735248600734503E-002f);
    t = __internal_fmad (t, a,  3.224670187176319E-001f);
    t = __internal_fmad (t, a,  4.227843368636472E-001f);
    t = t * a;
    return t;
  } else if (a >= 0.7f) {
    a = 1.0f - a;
    t =                        4.588266515364258E-002f;
    t = __internal_fmad (t, a, 1.037396712740616E-001f);
    t = __internal_fmad (t, a, 1.228036339653591E-001f);
    t = __internal_fmad (t, a, 1.275242157462838E-001f);
    t = __internal_fmad (t, a, 1.432166835245778E-001f);
    t = __internal_fmad (t, a, 1.693435824224152E-001f);
    t = __internal_fmad (t, a, 2.074079329483975E-001f);
    t = __internal_fmad (t, a, 2.705875136435339E-001f);
    t = __internal_fmad (t, a, 4.006854436743395E-001f);
    t = __internal_fmad (t, a, 8.224669796332661E-001f);
    t = __internal_fmad (t, a, 5.772156651487230E-001f);
    t = t * a;
    return t;
  } else {
    t =                         3.587515669447039E-003f;
    t = __internal_fmad (t, a, -5.471285428060787E-003f);
    t = __internal_fmad (t, a, -4.462712795343244E-002f);
    t = __internal_fmad (t, a,  1.673177015593242E-001f);
    t = __internal_fmad (t, a, -4.213597883575600E-002f);
    t = __internal_fmad (t, a, -6.558672843439567E-001f);
    t = __internal_fmad (t, a,  5.772153712885004E-001f);
    t = t * a;
    t = __internal_fmad (t, a, a);
    return -__internal_accurate_logf(t);
  }
}


static float __internal_sin_kernel(float x)
{
  float x2, z;

  x2 = x * x;
  z  =                         -1.95152959e-4f;
  z  = __internal_fmad (z, x2,  8.33216087e-3f);
  z  = __internal_fmad (z, x2, -1.66666546e-1f);
  z  = z * x2;
  z  = __internal_fmad (z, x, x);
  return z;
}


static float __internal_cos_kernel(float x)
{
  float x2, z;

  x2 = x * x;
  z  =                          2.44331571e-5f;
  z  = __internal_fmad (z, x2, -1.38873163e-3f);
  z  = __internal_fmad (z, x2,  4.16666457e-2f);
  z  = __internal_fmad (z, x2, -5.00000000e-1f);
  z  = __internal_fmad (z, x2,  1.00000000e+0f);
  return z;
}

static float __internal_accurate_sinf(float a)
{
  float z;
  int   i;

  if ((__cuda___isinff(a)) || (a == 0.0f)) {
    return __fmul_rn (a, 0.0f);
  }
  z = __internal_trig_reduction_kernel(a, &i);
  
  if (i & 1) {
    z = __internal_cos_kernel(z);
  } else {
    z = __internal_sin_kernel(z);
  }
  if (i & 2) {
    z = -z;
  }
  return z;
}







static float __cuda_rintf(float a)
{


#line 1923 "c:\\cuda\\include\\math_functions.h"
  return __cuda_nearbyintf(a);
#line 1925 "c:\\cuda\\include\\math_functions.h"
}

static float __cuda_sinf(float a)
{


#line 1932 "c:\\cuda\\include\\math_functions.h"

#line 1934 "c:\\cuda\\include\\math_functions.h"
  return __internal_accurate_sinf(a);
#line 1936 "c:\\cuda\\include\\math_functions.h"
}

static float __cuda_cosf(float a)
{


#line 1943 "c:\\cuda\\include\\math_functions.h"

#line 1945 "c:\\cuda\\include\\math_functions.h"
  float z;
  int i;

  if (__cuda___isinff(a)) {
    return __fadd_rn (a, -a); 
  }
  z = __internal_trig_reduction_kernel(a, &i);
  
  i++;
  if (i & 1) {
    z = __internal_cos_kernel(z);
  } else {
    z = __internal_sin_kernel(z);
  }
  if (i & 2) {
    z = -z;
  }
  return z;
#line 1964 "c:\\cuda\\include\\math_functions.h"
}

static float __cuda_tanf(float a)
{


#line 1971 "c:\\cuda\\include\\math_functions.h"

#line 1973 "c:\\cuda\\include\\math_functions.h"
  float z;
  int   i;

  if (__cuda___isinff(a)) {
    return __fadd_rn (a, -a); 
  }
  z = __internal_trig_reduction_kernel(a, &i);
  
  z = __internal_tan_kernel(z);
  if (i & 1) {
    z = - (1.0f / z);
  }
  return z;
#line 1987 "c:\\cuda\\include\\math_functions.h"
}

static float __cuda_log2f(float a)
{


#line 1994 "c:\\cuda\\include\\math_functions.h"

#line 1996 "c:\\cuda\\include\\math_functions.h"
  return __internal_accurate_log2f(a);
#line 1998 "c:\\cuda\\include\\math_functions.h"
}

static float __cuda_expf(float a)
{


#line 2005 "c:\\cuda\\include\\math_functions.h"

#line 2007 "c:\\cuda\\include\\math_functions.h"
  return __internal_accurate_expf(a);
#line 2009 "c:\\cuda\\include\\math_functions.h"
}

static float __cuda_exp10f(float a)
{


#line 2016 "c:\\cuda\\include\\math_functions.h"

#line 2018 "c:\\cuda\\include\\math_functions.h"
  return __internal_accurate_exp10f(a);
#line 2020 "c:\\cuda\\include\\math_functions.h"
}

static float __cuda_coshf(float a)
{
  float z;

  a = __cuda_fabsf(a);
  z = __internal_expf_kernel(a, -2.0f);
  z = __internal_fmad (2.0f, z,  __fdividef (0.125f, z));
  if (a >= 90.0f) {
    z = __int_as_float(0x7f800000);     
  }
  return z;
}

static float __cuda_sinhf(float a)
{
  float s, z;

  s = a;
  a = __cuda_fabsf(a);
  if (a < 1.0f) {         
    float a2 = a * a;
    
    z =                         2.816951222e-6f;
    z = __internal_fmad (z, a2, 1.983615978e-4f);
    z = __internal_fmad (z, a2, 8.333350058e-3f);
    z = __internal_fmad (z, a2, 1.666666650e-1f);
    z = z * a2;
    z = __internal_fmad (z, a, a);
  } else {
    z = __internal_expf_kernel(a, -2.0f);
    z = __internal_fmad (2.0f, z, -__fdividef (0.125f, z));
    if (a >= 90.0f) {
      z = __int_as_float(0x7f800000);     
    }
  }
  return __cuda_copysignf(z, s);
}

static float __cuda_tanhf(float a)
{
  float s, t;

  t = __cuda_fabsf(a);
  if (t < 0.55f) {
    float z, z2;
    z = t;
    z2 = z * z;
    t =                          1.643758066599993e-2f;
    t = __internal_fmad (t, z2, -5.267181327760551e-2f);
    t = __internal_fmad (t, z2,  1.332072505223051e-1f);
    t = __internal_fmad (t, z2, -3.333294663641083e-1f);
    t = t * z2;
    s = __internal_fmad (t, z, z);
  } else {
    s = 1.0f - __fdividef(2.0f,(__internal_expf_kernel(2.0f * t, 0.0f)+1.0f));
    if (t >= 88.0f) {
      s = 1.0f;
    }
  }
  return __cuda_copysignf(s, a);
}

static float __cuda_atan2f(float a, float b)
{


#line 2089 "c:\\cuda\\include\\math_functions.h"
  float t0, t1, t3, fa, fb;

  
  
  fb = __cuda_fabsf(b);
  fa = __cuda_fabsf(a);

  if (fa == 0.0f && fb == 0.0f) {
    t3 = __cuda___signbitf(b) ? 3.141592654f : 0;
  } else if ((fa == __int_as_float(0x7f800000)) && (fb == __int_as_float(0x7f800000))) {
    t3 = __cuda___signbitf(b) ? 2.356194490f : 0.785398163f;
  } else {
    
    if (fb < fa) {
      t0 = fa;
      t1 = fb;
    } else {
      t0 = fb;
      t1 = fa;
    }
    t3 = __internal_accurate_fdividef(t1, t0);
    t3 = __internal_atanf_kernel(t3);
    
    if (fa > fb)  t3 = 1.570796327f - t3;
    if (b < 0.0f) t3 = 3.141592654f - t3;
  }
  t3 = __cuda_copysignf(t3, a);

  return t3;
#line 2119 "c:\\cuda\\include\\math_functions.h"
}

static float __cuda_atanf(float a)
{
  float t0, t1;

  
  t0 = __cuda_fabsf(a);
  t1 = t0;
  if (t0 > 1.0f) {
    t1 = 1.0f / t1;
  }
  
  t1 = __internal_atanf_kernel(t1);
  
  if (t0 > 1.0f) {
    t1 = 1.570796327f - t1;
  }
  return __cuda_copysignf(t1, a);
}


static float __internal_asinf_kernel(float a)
{
  float t2, t3, t4;

  t2 = a * a;
  t3 = -0.501162291f;
  t3 = __internal_fmad (t3, t2, 0.915201485f);
  t3 = t3 * t2;
  t3 = t3 * a;
  t4 = t2 - 5.478654385f;
  t4 = __internal_fmad (t4, t2, 5.491230488f);
  t4 = 1.0f / t4;
  a = __internal_fmad (t3, t4, a);
  return a;
}

static float __cuda_asinf(float a)
{
  float t0, t1, t2;

  t0 = __cuda_fabsf(a);
  t2 = 1.0f - t0;
  t2 = 0.5f * t2;
  t2 = __cuda_sqrtf(t2);
  t1 = t0 > 0.575f ? t2 : t0;
  t1 = __internal_asinf_kernel(t1);
  t2 = __internal_fmad (-2.0f, t1, 1.570796327f);
  if (t0 > 0.575f) {
    t1 = t2;
  }
  return __cuda_copysignf(t1, a);
}

static float __cuda_acosf(float a)
{
  float t0, t1, t2;

  t0 = __cuda_fabsf(a);
  t2 = 1.0f - t0;
  t2 = 0.5f * t2;
  t2 = __cuda_sqrtf(t2);
  t1 = t0 > 0.575f ? t2 : t0;
  t1 = __internal_asinf_kernel(t1);
  t1 = t0 > 0.575f ? 2.0f * t1 : 1.570796327f - t1;
  if (__cuda___signbitf(a)) {
    t1 = 3.141592654f - t1;
  }
  return t1;
}

static float __cuda_logf(float a)
{


#line 2196 "c:\\cuda\\include\\math_functions.h"

#line 2198 "c:\\cuda\\include\\math_functions.h"
  return __internal_accurate_logf(a);
#line 2200 "c:\\cuda\\include\\math_functions.h"
}

static float __cuda_log10f(float a)
{


#line 2207 "c:\\cuda\\include\\math_functions.h"

#line 2209 "c:\\cuda\\include\\math_functions.h"
  return 0.434294482f * __internal_accurate_logf(a);
#line 2211 "c:\\cuda\\include\\math_functions.h"
}

static float __cuda_log1pf(float a)
{


#line 2218 "c:\\cuda\\include\\math_functions.h"
  float t;

  
  if (a == 0.0f) return a;
#line 2223 "c:\\cuda\\include\\math_functions.h"
  if (a >= -0.394f && a <= 0.65f) {
    
    t = a + 2.0f;
    t = __fdividef (a, t);
    t = -a * t;
    t = __internal_atanhf_kernel (a, t);
  } else {
    t = __internal_accurate_logf (1.0f + a);
  }
  return t;
#line 2234 "c:\\cuda\\include\\math_functions.h"
}

static float __cuda_acoshf(float a)
{


#line 2241 "c:\\cuda\\include\\math_functions.h"
  float t;

  t = a - 1.0f;
  if (__cuda_fabsf(t) > 8388608.0f) {
    
    return 0.693147181f + __internal_accurate_logf(a);
  } else {
    t = t + __cuda_sqrtf(__internal_fmad (a, t, t));
    return __cuda_log1pf(t);
  }
#line 2252 "c:\\cuda\\include\\math_functions.h"
}

static float __cuda_asinhf(float a)
{


#line 2259 "c:\\cuda\\include\\math_functions.h"
  float fa, oofa, t;

  fa = __cuda_fabsf(a);
  if (fa > 8.507059173e37f) {   
    t = 0.693147181f + __logf(fa);  
  } else {
    oofa = 1.0f / fa;
    t =fa+__fdividef (fa,(oofa+__cuda_sqrtf(__internal_fmad(oofa,oofa,1.0f))));
    t = __cuda_log1pf(t);
  }
  return __cuda_copysignf(t, a);
#line 2271 "c:\\cuda\\include\\math_functions.h"
}

static float __cuda_atanhf(float a)
{


#line 2278 "c:\\cuda\\include\\math_functions.h"
  float fa, t;

  fa = __cuda_fabsf(a);
  t = __fdividef ((2.0f * fa), (1.0f - fa));
  t = 0.5f * __cuda_log1pf(t);
  return __cuda_copysignf(t, a);
#line 2285 "c:\\cuda\\include\\math_functions.h"
}

static float __cuda_expm1f(float a)
{
  float t, z, j, u;
  
  t = __cuda_rintf (a * 1.442695041f);
  z = __internal_fmad (-t, 0.6931457519f, a);
  z = __internal_fmad (-t, 1.4286067653e-6f, z);
  
  if (__cuda_fabsf(a) < 0.41f) {
    z = a;
    t = 0.0f;
  }
  
  j = t;
  if (t == 128.0f) j = j - 1.0f; 
  
  u =                        1.38795078474044430E-003f;
  u = __internal_fmad (u, z, 8.38241261853264930E-003f);
  u = __internal_fmad (u, z, 4.16678317762833940E-002f);
  u = __internal_fmad (u, z, 1.66663978874356580E-001f);
  u = __internal_fmad (u, z, 4.99999940395997040E-001f);
  u = u * z;
  u = __internal_fmad (u, z, z);
  if (a == 0.0f) u = a; 
  
  z = __cuda_exp2f (j);
  a = z - 1.0f;
  if (a != 0.0f) u = __internal_fmad (u, z, a); 
  if (t == 128.0f) u = u + u; 
  
  if (j >  128.0f) u = __int_as_float(0x7f800000);
  if (j <  -25.0f) u = -1.0f;
  return u;
}

static float __cuda_hypotf(float a, float b)
{


#line 2327 "c:\\cuda\\include\\math_functions.h"
  float v, w, t;

  a = __cuda_fabsf(a);
  b = __cuda_fabsf(b);
  
  if (a > b) {
    v = a;
    w = b; 
  } else {
    v = b;
    w = a;
  }
  t = __internal_accurate_fdividef(w, v);
  t = __internal_fmad (t, t, 1.0f);
  t = v * __cuda_sqrtf(t);
  if (v == 0.0f) {
    t = v + w;
  }
  if ((v == __int_as_float(0x7f800000)) || (w == __int_as_float(0x7f800000))) {
    t = __int_as_float(0x7f800000);
  }
  return t;
#line 2350 "c:\\cuda\\include\\math_functions.h"
}

static float __cuda_cbrtf(float a)
{


#line 2357 "c:\\cuda\\include\\math_functions.h"
  float s, t;

  s = __cuda_fabsf(a);
  if ((a == 0.0f) || (s == __int_as_float(0x7f800000))) {
    return a + a;
  } 
  t = __cuda_exp2f(0.333333333f * __log2f(s));   
  t = t-(t-(__fdividef(s,(t*t))))*0.333333333f;  




#line 2370 "c:\\cuda\\include\\math_functions.h"
  t = __cuda_copysignf(t, a);
#line 2372 "c:\\cuda\\include\\math_functions.h"
  return t;
#line 2374 "c:\\cuda\\include\\math_functions.h"
}

static float __cuda_erff(float a)
{
  float t, r, q;

  t = __cuda_fabsf(a);
  if (t < 1.0f) {
    t = t * t;
    r =                        -5.58510127926029810E-004f;
    r = __internal_fmad (r, t,  4.90688891415893070E-003f);
    r = __internal_fmad (r, t, -2.67027980930150640E-002f);
    r = __internal_fmad (r, t,  1.12799056505903940E-001f);
    r = __internal_fmad (r, t, -3.76122956138427440E-001f);
    r = __internal_fmad (r, t,  1.12837911712623450E+000f);
    a = a * r;
  } else if (t <= __int_as_float(0x7f800000)) { 
    


    q = __internal_fmad (t, 0.3275911f, 1.0f);
    q = 1.0f / q;
    r =                         1.061405429f;
    r = __internal_fmad (r, q, -1.453152027f);
    r = __internal_fmad (r, q,  1.421413741f);
    r = __internal_fmad (r, q, -0.284496736f);
    r = __internal_fmad (r, q,  0.254829592f);
    r = r * q;
    q = __internal_expf_kernel(-a * a, 0.0f);
    r = __internal_fmad (-q, r, 1.0f);
    if (t >= 5.5f) {
      r = 1.0f;
    }
    a = __int_as_float (__float_as_int(r) | (__float_as_int(a) & 0x80000000));
  }
  return a;
}

static float __cuda_erfinvf (float a)
{
  float fa, t;
  
  fa = fabsf(a);
  if (fa >= 1.0f) {
    t = __cuda_rsqrtf (__int_as_float (0xffc00000));  
    if (fa == 1.0f) {
      t = a * __int_as_float(0x7f800000);   
    }
  } else if (fa > 0.9375f) {
    



    float p, q;

    t = __cuda_log1pf(-fa);
    t = __cuda_rsqrtf(-t);
    p =                        -1.64441567910e-1f;
    p = __internal_fmad (p, t,  6.80544246825e-1f);
    p = __internal_fmad (p, t, -1.12808139162e+0f);
    p = __internal_fmad (p, t,  6.90969348887e-1f);
    p = __internal_fmad (p, t,  1.38271964963e+0f);
    p = __internal_fmad (p, t,  1.55047000312e-1f);
    q =                     t + 1.38522814199e+0f;
    q = __internal_fmad (q, t,  1.55024849822e-1f);
    q = q * t;
    t = __fdividef (p, q);
    if (a < 0.0f) t = -t;
  } else if (fa > 0.75f) {
    



    float p, q;

    t = __internal_fmad (a, a, -0.87890625f);
    p =                        -7.1986748896e+0f;
    p = __internal_fmad (p, t, +1.3411974175e+1f);
    p = __internal_fmad (p, t, -5.1381573203e+0f);
    p = __internal_fmad (p, t,  4.9633374831e-1f);
    q =                     t  -1.1436535838e+1f;
    q = __internal_fmad (q, t,  1.3568885572e+1f);
    q = __internal_fmad (q, t, -4.1747509256e+0f);
    q = __internal_fmad (q, t,  3.5327242323e-1f);
    p = __fdividef (p, q);
    t = a * p;
  } else {  
    float a2;

    a2 = a * a;
    t =                          6.1046168794766742E-001f;
    t = __internal_fmad (t, a2, -8.9504882462753121E-001f);
    t = __internal_fmad (t, a2,  7.0224162369928511E-001f);
    t = __internal_fmad (t, a2, -1.9993784895823222E-001f);
    t = __internal_fmad (t, a2,  1.1920613463949599E-001f);
    t = __internal_fmad (t, a2,  8.0131492246997685E-002f);
    t = __internal_fmad (t, a2,  1.2793154958377403E-001f);
    t = __internal_fmad (t, a2,  2.3200529172828793E-001f);
    t = __internal_fmad (t, a2,  8.8622695604694379E-001f);
    t = t * a;
  }
  return t;
}

static float __cuda_erfcf(float a)
{

    if (__cuda___isnanf(a)) return a + a;
#line 2483 "c:\\cuda\\include\\math_functions.h"
  if (a <= 0.813f) {
    return 1.0f - __cuda_erff(a);
  } else if (a > 10.055f) {
    return 0.0f;
  } else {
    float p;
    float q;
    float h;
    float l;
    



    p =                        1.9997798317768276E-006f;
    p = __internal_fmad (p, a, 5.6410849770777538E-001f);
    p = __internal_fmad (p, a, 2.4579440583231387E+000f);
    p = __internal_fmad (p, a, 4.9763988947448308E+000f);
    p = __internal_fmad (p, a, 4.5767235962336885E+000f);
    q =                    a + 4.3540863476555538E+000f;
    q = __internal_fmad (q, a, 9.3453217404616584E+000f);
    q = __internal_fmad (q, a, 1.0132282183545772E+001f);
    q = __internal_fmad (q, a, 4.5781789912165518E+000f);   
    
    h = 1.0f / q;
    q = __internal_fmad (-q * h, h, 2.0f * h);
    p = p * q;
    
    h = __int_as_float(__float_as_int(a) & 0xfffff000);  
    l = __fadd_rn (a, -h);  
    q = __fmul_rn (-h, h);  
    q = __internal_expf_kernel(q, 0.0f);
    a = a + h;
    l = l * a;
    h = __internal_expf_kernel(-l, 0.0f);
    q = q * h;
    p = p * q;
    return p;
  }
}

static float __cuda_erfcinvf (float a)
{
  float t;
  if (a <= 0.0f) {
    t = __int_as_float(0x7fffffff);
    if (a == 0.0f) {
      t = (1.0f - a) * __int_as_float(0x7f800000);
    }
  } 
  else if (a >= 0.0625f) {
    t = __cuda_erfinvf (1.0f - a);
  }
  else {
    float p, q;
    t = __cuda_logf(a);
    t = __cuda_rsqrtf(-t);
    p =                        -1.64441567910e-1f;
    p = __internal_fmad (p, t,  6.80544246825e-1f);
    p = __internal_fmad (p, t, -1.12808139162e+0f);
    p = __internal_fmad (p, t,  6.90969348887e-1f);
    p = __internal_fmad (p, t,  1.38271964963e+0f);
    p = __internal_fmad (p, t,  1.55047000312e-1f);
    q =                     t + 1.38522814199e+0f;
    q = __internal_fmad (q, t,  1.55024849822e-1f);
    q = q * t;
    t = __fdividef (p, q);
  }
  return t;
}

static float __cuda_lgammaf(float a)
{
  float t;
  float i;
  int quot;
  t = __internal_lgammaf_pos(__cuda_fabsf(a));
  if (a >= 0.0f) return t;
  a = __cuda_fabsf(a);
  i = __cuda_floorf(a);                   
  if (a == i) return __int_as_float(0x7f800000); 
  if (a < 1e-19f) return -__internal_accurate_logf(a);
  i = __cuda_rintf (2.0f * a);
  quot = (int)i;
  i = __internal_fmad (-i, 0.5f, a);
  i = i * 3.141592654f;
  if (quot & 1) {
    i = __internal_cos_kernel(i);
  } else {
    i = __internal_sin_kernel(i);
  }
  i = __cuda_fabsf(i);
  t = 1.144729886f - __internal_accurate_logf(i * a) - t;
  return t;
}

static float __cuda_ldexpf(float a, int b)
{


#line 2583 "c:\\cuda\\include\\math_functions.h"
  float fa = __cuda_fabsf(a);

  if ((fa == 0.0f) || (fa == __int_as_float(0x7f800000)) || (b == 0)) {
    if (!(fa > 0.0f)) a = a + a;
    return a;
  } else if (__cuda_abs(b) < 126) {
    return a * __cuda_exp2f((float)b);
  } else if (__cuda_abs(b) < 252) {
    int bhalf = b / 2;
    return a * __cuda_exp2f((float)bhalf) * __cuda_exp2f((float)(b - bhalf));
  } else {
    int bquarter = b / 4;
    float t = __cuda_exp2f((float)bquarter);
    return a * t * t * t * __cuda_exp2f((float)(b - 3 * bquarter));
  }
#line 2599 "c:\\cuda\\include\\math_functions.h"
}

static float __cuda_scalbnf(float a, int b)
{


#line 2606 "c:\\cuda\\include\\math_functions.h"
  
  return __cuda_ldexpf(a, b);
#line 2609 "c:\\cuda\\include\\math_functions.h"
}

static float __cuda_scalblnf(float a, long int b)
{


#line 2616 "c:\\cuda\\include\\math_functions.h"
  int t;
  if (b > 2147483647L) {
    t = 2147483647;
  } else if (b < (-2147483647 - 1)) {
    t = (-2147483647 - 1);
  } else {
    t = (int)b;
  }
  return __cuda_scalbnf(a, t);
#line 2626 "c:\\cuda\\include\\math_functions.h"
}

static float __cuda_frexpf(float a, int *b)
{
  float fa = __cuda_fabsf(a);
  unsigned int expo;
  unsigned int denorm;

  if (fa < 1.175494351e-38f) {
    a *= 16777216.0f;
    denorm = 24;
  } else {
    denorm = 0;
  }
  expo = ((__float_as_int(a) >> 23) & 0xff);
  if ((fa == 0.0f) || (expo == 0xff)) {
    expo = 0;
    a = a + a;
  } else {  
    expo = expo - denorm - 126;
    a = __int_as_float(((__float_as_int(a) & 0x807fffff) | 0x3f000000));
  }
  *b = expo;
  return a;
}

static float __cuda_modff(float a, float *b)
{


#line 2657 "c:\\cuda\\include\\math_functions.h"
  float t;
  if (__cuda___finitef(a)) {
    t = __cuda_truncf(a);
    *b = t;
    t = a - t;
    return __cuda_copysignf(t, a);
  } else if (__cuda___isinff(a)) {
    t = 0.0f;
    *b = a;
    return __cuda_copysignf(t, a);
  } else {
    *b = a; 
    return a;
  }
#line 2672 "c:\\cuda\\include\\math_functions.h"
}

static float __cuda_fmodf(float a, float b)
{


#line 2679 "c:\\cuda\\include\\math_functions.h"
  float orig_a = a;
  float orig_b = b;
  a = __cuda_fabsf(a);
  b = __cuda_fabsf(b);
  if (!((a <= __int_as_float(0x7f800000)) && (b <= __int_as_float(0x7f800000)))) {
    return orig_a + orig_b;
  }
  if ((a == __int_as_float(0x7f800000)) || (b == 0.0f)) {
    return __cuda_rsqrtf (__int_as_float (0xffc00000));
  } else if (a >= b) {

    
    int expoa = (a < 1.175494351e-38f) ? 
        ((int)__log2f(a)) : (((__float_as_int(a) >> 23) & 0xff) - 127);
    int expob = (b < 1.175494351e-38f) ? 
        ((int)__log2f(b)) : (((__float_as_int(b) >> 23) & 0xff) - 127);
    int scale = expoa - expob;
    float scaled_b = __cuda_ldexpf(b, scale);
    if (scaled_b <= 0.5f * a) {
      scaled_b *= 2.0f;
    }






#line 2707 "c:\\cuda\\include\\math_functions.h"
    while (scaled_b >= b) {
      if (a >= scaled_b) {
        a -= scaled_b;
      }
      scaled_b *= 0.5f;
    }
    return __cuda_copysignf(a, orig_a);
  } else {
    if (!(a > 0.0f)) orig_a = orig_a + orig_a;
    return orig_a;
  }
#line 2719 "c:\\cuda\\include\\math_functions.h"
}

static float __cuda_remainderf(float a, float b)
{

  float twoa = 0.0f;
  unsigned int quot0 = 0;  
  float orig_a = a;
  float orig_b = b;

  a = __cuda_fabsf(a);
  b = __cuda_fabsf(b);
  if (!((a <= __int_as_float(0x7f800000)) && (b <= __int_as_float(0x7f800000)))) {
    return orig_a + orig_b;
  }
  if ((a == __int_as_float(0x7f800000)) || (b == 0.0f)) {
    return __cuda_rsqrtf (__int_as_float (0xffc00000));
  } else if (a >= b) {

    int expoa = (a < 1.175494351e-38f) ? 
        ((int)__log2f(a)) : (((__float_as_int(a) >> 23) & 0xff) - 127);
    int expob = (b < 1.175494351e-38f) ? 
        ((int)__log2f(b)) : (((__float_as_int(b) >> 23) & 0xff) - 127);
    int scale = expoa - expob;
    float scaled_b = __cuda_ldexpf(b, scale);
    if (scaled_b <= 0.5f * a) {
      scaled_b *= 2.0f;
    }










#line 2758 "c:\\cuda\\include\\math_functions.h"
    while (scaled_b >= b) {
      quot0 = 0;
      if (a >= scaled_b) {
        twoa = __internal_fmad (a, 2.0f, -scaled_b) - scaled_b;
        a = a - scaled_b;
        quot0 = 1;
      }
      scaled_b *= 0.5f;
    }
  }
  

  twoa = 2.0f * a;
  if ((twoa > b) || ((twoa == b) && quot0)) {
    a -= b;
    a = __int_as_float(__float_as_int(a) | 0x80000000);
  }














#line 2790 "c:\\cuda\\include\\math_functions.h"
  a = __int_as_float((__float_as_int(orig_a) & 0x80000000)^
                     __float_as_int(a));
  return a;
}

static float __cuda_remquof(float a, float b, int* quo)
{
  float twoa = 0.0f;
  unsigned int quot = 0;  
  unsigned int sign;
  float orig_a = a;
  float orig_b = b;

  
  sign = 0 - (__cuda___signbitf(a) != __cuda___signbitf(b));
  a = __cuda_fabsf(a);
  b = __cuda_fabsf(b);
  if (!((a <= __int_as_float(0x7f800000)) && (b <= __int_as_float(0x7f800000)))) {
    *quo = quot;
    return orig_a + orig_b;
  }
  if ((a == __int_as_float(0x7f800000)) || (b == 0.0f)) {
    *quo = quot;
    return __cuda_rsqrtf (__int_as_float (0xffc00000));
  } else if (a >= b) {

    
    int expoa = (a < 1.175494351e-38f) ? 
        ((int)__log2f(a)) : (((__float_as_int(a) >> 23) & 0xff) - 127);
    int expob = (b < 1.175494351e-38f) ? 
        ((int)__log2f(b)) : (((__float_as_int(b) >> 23) & 0xff) - 127);
    int scale = expoa - expob;
    float scaled_b = __cuda_ldexpf(b, scale);
    if (scaled_b <= 0.5f * a) {
      scaled_b *= 2.0f;
    }



















#line 2846 "c:\\cuda\\include\\math_functions.h"
    while (scaled_b >= b) {
      quot <<= 1;
      if (a >= scaled_b) {
        twoa = (2.0f * a - scaled_b) - scaled_b;
        a -= scaled_b;
        quot += 1;
      }
      scaled_b *= 0.5f;
    }
  }
  

  twoa = 2.0f * a;
  if ((twoa > b) || ((twoa == b) && (quot & 1))) {
    quot++;
    a -= b;
    a = __cuda_copysignf (a, -1.0f);
  }
















#line 2881 "c:\\cuda\\include\\math_functions.h"
  a = __int_as_float((__float_as_int(orig_a) & 0x80000000)^
                     __float_as_int(a));
  quot = quot & (~((~0)<<3));
  quot = quot ^ sign;
  quot = quot - sign;
  *quo = quot;
  return a;
}

static float __cuda_fmaf(float a, float b, float c)
{
  return __fmaf_rn(a, b, c);
}

static float __internal_accurate_powf(float a, float b)
{
  float2 loga, prod;

  volatile float t;


#line 2903 "c:\\cuda\\include\\math_functions.h"

  
  loga = __internal_log_ep(a);

  
  if (__cuda_fabsf(b) > 1.0e34f) b *= 1.220703125e-4f;
  prod.y = b;
  prod.x = 0.0f;
  prod = __internal_dsmul (prod, loga);

  
  if (__float_as_int(prod.y) == 0x42b17218) {
    prod.y = __int_as_float(__float_as_int(prod.y) - 1);
    prod.x = prod.x + __int_as_float(0x37000000);
  }

  
  t = __cuda_expf(prod.y);
  
  if (t != __int_as_float(0x7f800000)) {
    


    t = __internal_fmad (t, prod.x, t);
  }
  return t;
}

static float __cuda_powif(float a, int b)
{
  unsigned int e = __cuda_abs(b);
  float        r = 1.0f;

  while (1) {
    if ((e & 1) != 0) {
      r = r * a;
    }
    e = e >> 1;
    if (e == 0) {
      return b < 0 ? 1.0f / r : r;
    }
    a = a * a;
  }
}

static double __cuda_powi(double a, int b)
{
  unsigned int e = __cuda_abs(b);
  double       r = 1.0;

  while (1) {
    if ((e & 1) != 0) {
      r = r * a;
    }
    e = e >> 1;
    if (e == 0) {
      return b < 0 ? 1.0 / r : r;
    }
    a = a * a;
  }
}

static float __cuda_powf(float a, float b)
{


#line 2970 "c:\\cuda\\include\\math_functions.h"

#line 2972 "c:\\cuda\\include\\math_functions.h"
  int bIsOddInteger;
  float t;
  if (a == 1.0f || b == 0.0f) {
    return 1.0f;
  } 
  if (__cuda___isnanf(a) || __cuda___isnanf(b)) {
    return a + b;
  }
  if (a == __int_as_float(0x7f800000)) {
    return __cuda___signbitf(b) ? 0.0f : __int_as_float(0x7f800000);
  }
  if (__cuda___isinff(b)) {
    if (a == -1.0f) {
      return 1.0f;
    }
    t = (__cuda_fabsf(a) > 1.0f) ? __int_as_float(0x7f800000) : 0.0f;
    if (b < 0.0f) {
      t = 1.0f / t;
    }
    return t;
  }
  bIsOddInteger = (b - (2.0f * floorf(0.5f * b))) == 1.0f;
  if (a == 0.0f) {
    t = bIsOddInteger ? a : 0.0f;
    if (b < 0.0f) {
      t = 1.0f / t;
    }
    return t;
  } 
  if (a == -__int_as_float(0x7f800000)) {
    t = - ((b < 0.0f) ? (1.0f / a) : a);
    if (bIsOddInteger) {
      t = __int_as_float(__float_as_int(t) ^ 0x80000000);
    }
    return t;
  } 
  if ((a < 0.0f) && (b != __cuda_truncf(b))) {
    return __cuda_rsqrtf(__int_as_float(0xffc00000));
  }
  t = __cuda_fabsf(a);
  t = __internal_accurate_powf(t, b);
  if ((a < 0.0f) && bIsOddInteger) {
    t = __int_as_float(__float_as_int(t) ^ 0x80000000);
  }
  return t;
#line 3018 "c:\\cuda\\include\\math_functions.h"
}


static float __internal_tgammaf_kernel(float a)
{
  float t;
  t =                        -1.05767296987211380E-003f;
  t = __internal_fmad (t, a,  7.09279059435508670E-003f);
  t = __internal_fmad (t, a, -9.65347121958557050E-003f);
  t = __internal_fmad (t, a, -4.21736613253687960E-002f);
  t = __internal_fmad (t, a,  1.66542401247154280E-001f);
  t = __internal_fmad (t, a, -4.20043267827838460E-002f);
  t = __internal_fmad (t, a, -6.55878234051332940E-001f);
  t = __internal_fmad (t, a,  5.77215696929794240E-001f);
  t = __internal_fmad (t, a,  1.00000000000000000E+000f);
  return t;
}





static float __cuda_tgammaf(float a)
{
  float s, xx, x=a;
  if (x >= 0.0f) {
    if (x > 36.0f) x = 36.0f; 
    s = 1.0f;
    xx = x;
    if (x > 34.03f) { 
      xx -= 1.0f;
    }
    while (xx > 1.5f) {
      xx = xx - 1.0f;
      s = s * xx;
    }
    if (x >= 0.5f) {
      xx = xx - 1.0f;
    }
    xx = __internal_tgammaf_kernel(xx);
    if (x < 0.5f) {
      xx = xx * x;
    }
    s = __fdividef(s, xx);
    if (x > 34.03f) {
      
      xx = x - 1.0f;
      s = s * xx;
    }
    return s;
  } else {
    if (x == __cuda_floorf(x)) {  
      x = __int_as_float(0x7fffffff);  

      return x;
#line 3074 "c:\\cuda\\include\\math_functions.h"
    } 
    if (x < -41.1f) x = -41.1f; 
    xx = x;
    if (x < -34.03f) {           
      xx += 6.0f;
    } 
    s = xx;
    while (xx < -0.5f) {
      xx = xx + 1.0f;
      s = s * xx;
    }
    xx = __internal_tgammaf_kernel(xx);
    s = s * xx;
    s = 1.0f / s;
    if (x < -34.03f) {
      xx = x;
      xx *= (x + 1.0f);
      xx *= (x + 2.0f);
      xx *= (x + 3.0f);
      xx *= (x + 4.0f);
      xx *= (x + 5.0f);
      xx = 1.0f / xx;
      s = s * xx;
      if ((a < -42.0f) && !(((int)a)&1)) {
        s = __int_as_float(0x80000000);
      }
    }    
    return s;
  }
}

static float __cuda_roundf(float a)
{


#line 3110 "c:\\cuda\\include\\math_functions.h"
  float fa = __cuda_fabsf(a);
  float u = __cuda_copysignf (0.5f, a);
  u = __cuda_truncf (a + u);
  if (fa > 8388608.0f) u = a;
  if (fa < 0.5f) u = __cuda_truncf (a);
  return u;
#line 3117 "c:\\cuda\\include\\math_functions.h"
}

static long long int __internal_llroundf_kernel(float a)
{
  unsigned long long int res, t = 0LL;
  int shift;
  unsigned int ia = __float_as_int(a);
  
  if ((ia << 1) > 0xff000000) return 0x8000000000000000LL;
  if ((int)ia >= 0x5f000000) return 0x7fffffffffffffffLL;
  if (ia >= 0xdf000000) return 0x8000000000000000LL;
  shift = 189 - ((ia >> 23) & 0xff);
  res = ((long long int)(((ia << 8) | 0x80000000) >> 1)) << 32;
  if (shift >= 64) {
    t = res;
    res = 0;
  } else if (shift) {
    t = res << (64 - shift);
    res = res >> shift;
  }
  if (t >= 0x8000000000000000LL) {
    res++;
  }
  if ((int)ia < 0) res = (unsigned long long int)(-(long long int)res);
  return (long long int)res;
}

static long long int __cuda_llroundf(float a)
{


#line 3149 "c:\\cuda\\include\\math_functions.h"
  return __internal_llroundf_kernel(a);
#line 3151 "c:\\cuda\\include\\math_functions.h"
}

static long int __cuda_lroundf(float a)
{


#line 3158 "c:\\cuda\\include\\math_functions.h"


#line 3161 "c:\\cuda\\include\\math_functions.h"

  if (__cuda___isnanf(a)) return 0L;
  if (a >=  2147483648.0f) return 2147483647L;
  if (a <= -2147483648.0f) return (-2147483647L - 1L);
#line 3166 "c:\\cuda\\include\\math_functions.h"
  return (long int)(__cuda_roundf(a));
#line 3168 "c:\\cuda\\include\\math_functions.h"
#line 3169 "c:\\cuda\\include\\math_functions.h"
}

static float __cuda_fdimf(float a, float b)
{
  float t;
  t = a - b;    
  if (a <= b) {
    t = 0.0f;
  }
  return t;
}

static int __cuda_ilogbf(float a)
{
  unsigned int i;
  int expo;
  a = __cuda_fabsf(a);
  if (a <= 1.175494351e-38f) {
    
    if (a == 0.0f) {
      expo = -((int)((unsigned int)-1 >> 1))-1;
    } else {
      expo = -126;
      i = __float_as_int(a);
      i = i << 8;
      while ((int)i >= 0) {
        expo--;
        i = i + i;
      }
    }
  } else {
    i = __float_as_int(a);
    expo = ((int)((i >> 23) & 0xff)) - 127;
    if ((i == 0x7f800000)) {
      expo = ((int)((unsigned int)-1 >> 1));
    }
    if ((i > 0x7f800000)) {
      expo = -((int)((unsigned int)-1 >> 1))-1;
    }
  } 
  return expo;
}

static float __cuda_logbf(float a)
{


#line 3217 "c:\\cuda\\include\\math_functions.h"
  unsigned int i;
  int expo;
  float res;

  if (__cuda___isnanf(a)) return a + a;
#line 3223 "c:\\cuda\\include\\math_functions.h"
  a = __cuda_fabsf(a);
  if (a <= 1.175494351e-38f) {
    
    if (a == 0.0f) {
      res = -__int_as_float(0x7f800000);
    } else {
      expo = -126;
      i = __float_as_int(a);
      i = i << 8;
      while ((int)i >= 0) {
        expo--;
        i = i + i;
      }
      res = (float)expo;
    }
  } else {
    i = __float_as_int(a);
    expo = ((int)((i >> 23) & 0xff)) - 127;
    res = (float)expo;
    if ((i >= 0x7f800000)) {  
      
      res = a + a;
    }
  } 
  return res;
#line 3249 "c:\\cuda\\include\\math_functions.h"
}

static void __cuda_sincosf(float a, float *sptr, float *cptr)
{


#line 3256 "c:\\cuda\\include\\math_functions.h"

#line 3258 "c:\\cuda\\include\\math_functions.h"
  float t, u, s, c;
  int quadrant;
  t = __cuda_fabsf(a);
  if ((t == __int_as_float(0x7f800000)) || (t == 0.0f)) {
    s = __fmul_rn (a, 0.0f); 
    c = 1.0f + s;                     
    *sptr = s;
    *cptr = c;
    return;
  }
  t = __internal_trig_reduction_kernel(a, &quadrant);
  u = __internal_cos_kernel(t);
  t = __internal_sin_kernel(t);
  if (quadrant & 1) {
    s = u;
    c = t;
  } else {
    s = t;
    c = u;
  }
  if (quadrant & 2) {
    s = -s;
  }
  quadrant++;
  if (quadrant & 2) {
    c = -c;
  }
  *sptr = s;
  *cptr = c;
#line 3288 "c:\\cuda\\include\\math_functions.h"
}











static double rsqrt(double a)
{
  return 1.0 / sqrt(a);
}

static float rsqrtf(float a)
{
  return (float)rsqrt((double)a);
}













static double exp10(double a)
{
  return pow(10.0, a);
}



#line 3330 "c:\\cuda\\include\\math_functions.h"





static int __finite(double a)
{
  volatile union {
    double                 d;
    unsigned long long int l;
  } cvt;

  cvt.d = a;

  return cvt.l << 1 < 0xffe0000000000000ull;
}

static int __isnan(double a)
{
  volatile union {
    double                 d;
    unsigned long long int l;
  } cvt;

  cvt.d = a;

  return cvt.l << 1 > 0xffe0000000000000ull;
}

static int __isinf(double a)
{
  volatile union {
    double                 d;
    unsigned long long int l;
  } cvt;

  cvt.d = a;

  return cvt.l << 1 == 0xffe0000000000000ull;
}

static double round(double a)
{
  double fa = fabs(a);

  if (fa > 4503599627370496.0) {
    return a;
  } else {
    double u = floor(fa + 0.5);

    if (__signbit(a)) {
      u = -u;
    }
    return u;
  }
}

static long int lround(double a)
{
  return (long int)round(a);
}

static long long int llround(double a)
{
  return (long long int)round(a);
}

static double rint(double a)
{
  double fa = fabs(a);
  double u = 4503599627370496.0 + fa;
  if (fa >= 4503599627370496.0) {
    u = a;
  } else {
    u = u - 4503599627370496.0;
    if (__signbit(a)) {
      u = -u;
    }
  }
  return u;  
}

static long int lrint(double a)
{
  return (long int)rint(a);
}

static long long int llrint(double a)
{
  return (long long int)rint(a);
}

static double fdim(double a, double b)
{
  if (a > b) {
    return (a - b);
  } else if (a <= b) {
    return 0.0;
  } else if (__isnan(a)) {
    return a;
  } else {
    return b;
  }
}

static double scalbn(double a, int b)
{
  return ldexp(a, b);
}

static double scalbln(double a, long int b)
{
  int t;
  if (b > 2147483647L) {
    t = 2147483647;
  } else if (b < (-2147483647 - 1)) {
    t = (-2147483647 - 1);
  } else {
    t = (int)b;
  }
  return scalbn(a, t);
}






static double log1p(double a)
{
  volatile double u, m;

  u = 1.0 + a;
  if (u == 1.0) {
    
    u = a;
  } else {
    m = u - 1.0;
    u = log(u);
    if (a < 1.0) {
      
      u = a * u;
      u = u / m;
    }
  }
  return u;
}




static double expm1(double a)
{
  volatile double u, m;

  u = exp(a);
  m = u - 1.0;
  if (m == 0.0) {
    
    m = a;
  } 
  else if (fabs(a) < 1.0) {
    
    u = log(u);
    m = m * a;
    m = m / u;
  }
  return m;
}

static double cbrt(double a)
{
  double s, t;
  if (a == 0.0 || __isinf(a)) {
    return a;
  } 
  s = fabs(a);
  t = exp2(3.3333333333333333e-1 * log2(s));           
  t = t - (t - (s / (t * t))) * 3.3333333333333333e-1; 
  if (__signbit(a)) {
    t = -t;
  }
  return t;
}

static double acosh(double a)
{
  double s, t;

  t = a - 1.0;
  if (t == a) {
    return log(2.0) + log(a);
  } else {
    s = a + 1.0;
    t = t + sqrt(s * t);
    return log1p(t);
  }
}

static double asinh(double a)
{
  double fa, oofa, t;

  fa = fabs(a);
  oofa = 1.0 / fa;
  t = fa + fa / (oofa + sqrt(1.0 + oofa * oofa));
  t = log1p(t);
  if (__signbit(a)) {
    t = -t;
  }
  return t;
}

static double atanh(double a)
{
  double fa, t;

  fa = fabs(a);
  t = (2.0 * fa) / (1.0 - fa);
  t = 0.5 * log1p(t);
  if (__signbit(a)) {
    t = -t;
  }
  return t;
}

static int ilogb(double a)
{
  volatile union {
    double                 d;
    unsigned long long int l;
  } x;
  unsigned long long int i;
  int expo = -1022;

  if (__isnan(a)) return -((int)((unsigned int)-1 >> 1))-1;
  if (__isinf(a)) return ((int)((unsigned int)-1 >> 1));
  x.d = a;
  i = x.l & 0x7fffffffffffffffull;
  if (i == 0) return -((int)((unsigned int)-1 >> 1))-1;
  if (i >= 0x0010000000000000ull) {
    return (int)(((i >> 52) & 0x7ff) - 1023);
  }
  while (i < 0x0010000000000000ull) {
    expo--;
    i <<= 1;
  }
  return expo;
}

static double logb(double a)
{
  volatile union {
    double                 d;
    unsigned long long int l;
  } x;
  unsigned long long int i;
  int expo = -1022;

  if (__isnan(a)) return a;
  if (__isinf(a)) return fabs(a);
  x.d = a;
  i = x.l & 0x7fffffffffffffffull;
  if (i == 0) return -1.0/a;
  if (i >= 0x0010000000000000ull) {
    return (double)((int)((i >> 52) & 0x7ff) - 1023);
  }
  while (i < 0x0010000000000000ull) {
    expo--;
    i <<= 1;
  }
  return (double)expo;
}

static double fma(double a, double b, double c)
{
  return __fma_rn(a, b, c);
}

static void sincos(double a, double *sptr, double *cptr)
{
  *sptr = sin(a);
  *cptr = cos(a);
}







static float roundf(float a)
{
  return (float)round((double)a);
}

static long int lroundf(float a)
{
  return lround((double)a);
}

static long long int llroundf(float a)
{
  return llround((double)a);
}

static float rintf(float a)
{
  return (float)rint((double)a);
}

static long int lrintf(float a)
{
  return lrint((double)a);
}

static long long int llrintf(float a)
{
  return llrint((double)a);
}

static float logbf(float a)
{
  return (float)logb((double)a);
}

static float scalblnf(float a, long int b)
{
  return (float)scalbln((double)a, b);
}

static float acoshf(float a)
{
  return (float)acosh((double)a);
}

static float asinhf(float a)
{
  return (float)asinh((double)a);
}

static float atanhf(float a)
{
  return (float)atanh((double)a);
}

static float cbrtf(float a)
{
  return (float)cbrt((double)a);
}

static float expm1f(float a)
{
  return (float)expm1((double)a);
}

static float exp10f(float a)
{
  return (float)exp10((double)a);
}

static float fdimf(float a, float b)
{
  return (float)fdim((double)a, (double)b);
}

static float hypotf(float a, float b)
{
  return (float)hypot((double)a, (double)b);
}

static float log1pf(float a)
{
  return (float)log1p((double)a);
}

static float scalbnf(float a, int b)
{
  return (float)scalbn((double)a, b);
}

static float fmaf(float a, float b, float c)
{
  return (float)fma((double)a, (double)b, (double)c);
}

static void sincosf(float a, float *sptr, float *cptr)
{
  double s, c;

  sincos((double)a, &s, &c);
  *sptr = (float)s;
  *cptr = (float)c;
}

static int ilogbf(float a)
{
  return ilogb((double)a);
}

static float erff(float a)
{
  return (float)erf((double)a);
}

static float erfinvf(float a)
{
  return (float)erfinv((double)a);
}

static float erfcf(float a)
{
  return (float)erfc((double)a);
}

static float erfcinvf(float a)
{
  return (float)erfcinv((double)a);
}

static float lgammaf(float a)
{
  return (float)lgamma((double)a);
}

static float tgammaf(float a)
{
  return (float)tgamma((double)a);
}

static float remquof(float a, float b, int *quo)
{
  return (float)remquo((double)a, (double)b, quo);
}

static float remainderf(float a, float b)
{
  return (float)remainder((double)a, (double)b);
}

static float nextafterf(float a, float b)
{
  return (float)nextafter((double)a, (double)b);
}







static int __finitef(float a)
{
  return __cuda___finitef(a);
}

static int __isinff(float a)
{
  return __cuda___isinff(a);
}

static int __isnanf(float a)
{
  return __cuda___isnanf(a);
}







static double lgamma(double a)
{
  return (double)__cuda_lgammaf((float)a);
}

static double tgamma(double a)
{
  return (double)__cuda_tgammaf((float)a);
}

static double erf(double a)
{
  return (double)__cuda_erff((float)a);
}

static double erfinv(double a)
{
  return (double)__cuda_erfinvf((float)a);
}

static double erfc(double a)
{
  return (double)__cuda_erfcf((float)a);
}

static double erfcinv(double a)
{
  return (double)__cuda_erfcinvf((float)a);
}

static double remquo(double a, double b, int *quo)
{
  return (double)__cuda_remquof((float)a, (float)b, quo);
}

static double remainder(double a, double b)
{
  return (double)__cuda_remainderf((float)a, (float)b);
}

static double nextafter(double a, double b)
{
  return (double)__cuda_nextafterf((float)a, (float)b);
}



#line 3850 "c:\\cuda\\include\\math_functions.h"

#line 3852 "c:\\cuda\\include\\math_functions.h"

#line 3854 "c:\\cuda\\include\\math_functions.h"





#line 3860 "c:\\cuda\\include\\math_functions.h"



#line 1 "c:\\cuda\\include\\math_functions_dbl_ptx1.h"








































#line 42 "c:\\cuda\\include\\math_functions_dbl_ptx1.h"

#line 1 "c:\\cuda\\include\\crt/func_macro.h"
































































































#line 98 "c:\\cuda\\include\\crt/func_macro.h"
#line 44 "c:\\cuda\\include\\math_functions_dbl_ptx1.h"

static double __cuda_fabs(double a)
{
  return (float)__cuda_fabsf((float)a);
}

static double __cuda_fmax(double a, double b)
{
  return (float)__cuda_fmaxf((float)a, (float)b);
}

static double __cuda_fmin(double a, double b)
{
  return (float)__cuda_fminf((float)a, (float)b);
}

static int __cuda___finite(double a)
{
  return __cuda___finitef((float)a);
}

static int __cuda___isinf(double a)
{
  return __cuda___isinff((float)a);
}

static int __cuda___isnan(double a)
{
  return __cuda___isnanf((float)a);
}

static int __cuda___signbit(double a)
{
  return __cuda___signbitf((float)a);
}

static double __cuda_sqrt(double a)
{
  return (double)__cuda_sqrtf((float)a);
}

static double __cuda_rsqrt(double a)
{
  return (double)__cuda_rsqrtf((float)a);
}

static double __cuda_ceil(double a)
{
  return (double)__cuda_ceilf((float)a);
}

static double __cuda_trunc(double a)
{
  return (double)__cuda_truncf((float)a);
}

static double __cuda_floor(double a)
{
  return (double)__cuda_floorf((float)a);
}

static double __cuda_copysign(double a, double b)
{
  return (double)__cuda_copysignf((float)a, (float)b);
}

static double __cuda_sin(double a)
{
  return (double)__cuda_sinf((float)a);
}

static double __cuda_cos(double a)
{
  return (double)__cuda_cosf((float)a);
}

static void __cuda_sincos(double a, double *sptr, double *cptr)
{
  float fs, fc;

  __cuda_sincosf((float)a, &fs, &fc);

  *sptr = (double)fs;
  *cptr = (double)fc;
}

static double __cuda_tan(double a)
{
  return (double)__cuda_tanf((float)a);
}

static double __cuda_exp(double a)
{
  return (double)__cuda_expf((float)a);
}

static double __cuda_exp2(double a)
{
  return (double)__cuda_exp2f((float)a);
}

static double __cuda_exp10(double a)
{
  return (double)__cuda_exp10f((float)a);
}

static double __cuda_expm1(double a)
{
  return (double)__cuda_expm1f((float)a);
}

static double __cuda_cosh(double a)
{
  return (double)__cuda_coshf((float)a);
}

static double __cuda_sinh(double a)
{
  return (double)__cuda_sinhf((float)a);
}

static double __cuda_tanh(double a)
{
  return (double)__cuda_tanhf((float)a);
}

static double __cuda_asin(double a)
{
  return (double)__cuda_asinf((float)a);
}

static double __cuda_acos(double a)
{
  return (double)__cuda_acosf((float)a);
}

static double __cuda_atan(double a)
{
  return (double)__cuda_atanf((float)a);
}

static double __cuda_atan2(double a, double b)
{
  return (double)__cuda_atan2f((float)a, (float)b);
}

static double __cuda_log(double a)
{
  return (double)__cuda_logf((float)a);
}

static double __cuda_log2(double a)
{
  return (double)__cuda_log2f((float)a);
}

static double __cuda_log10(double a)
{
  return (double)__cuda_log10f((float)a);
}

static double __cuda_log1p(double a)
{
  return (double)__cuda_log1pf((float)a);
}

static double __cuda_acosh(double a)
{
  return (double)__cuda_acoshf((float)a);
}

static double __cuda_asinh(double a)
{
  return (double)__cuda_asinhf((float)a);
}

static double __cuda_atanh(double a)
{
  return (double)__cuda_atanhf((float)a);
}

static double __cuda_hypot(double a, double b)
{
  return (double)__cuda_hypotf((float)a, (float)b);
}

static double __cuda_cbrt(double a)
{
  return (double)__cuda_cbrtf((float)a);
}

static double __cuda_erf(double a)
{
  return (double)__cuda_erff((float)a);
}

static double __cuda_erfinv(double a)
{
  return (double)__cuda_erfinvf((float)a);
}

static double __cuda_erfc(double a)
{
  return (double)__cuda_erfcf((float)a);
}

static double __cuda_erfcinv(double a)
{
  return (double)__cuda_erfcinvf((float)a);
}

static double __cuda_lgamma(double a)
{
  return (double)__cuda_lgammaf((float)a);
}

static double __cuda_tgamma(double a)
{
  return (double)__cuda_tgammaf((float)a);
}

static double __cuda_ldexp(double a, int b)
{
  return (double)__cuda_ldexpf((float)a, b);
}

static double __cuda_scalbn(double a, int b)
{
  return (double)__cuda_scalbnf((float)a, b);
}

static double __cuda_scalbln(double a, long b)
{
  return (double)__cuda_scalblnf((float)a, b);
}

static double __cuda_frexp(double a, int *b)
{
  return (double)__cuda_frexpf((float)a, b);
}

static double __cuda_modf(double a, double *b)
{
  float fb;
  float fa = __cuda_modff((float)a, &fb);

  *b = (double)fb;

  return (double)fa;  
}

static double __cuda_fmod(double a, double b)
{
  return (double)__cuda_fmodf((float)a, (float)b);
}

static double __cuda_remainder(double a, double b)
{
  return (double)__cuda_remainderf((float)a, (float)b);
}

static double __cuda_remquo(double a, double b, int *c)
{
  return (double)__cuda_remquof((float)a, (float)b, c);
}

static double __cuda_nextafter(double a, double b)
{
  return (double)__cuda_nextafterf((float)a, (float)b);
}

static double __cuda_nan(const char *tagp)
{
  return (double)__cuda_nanf(tagp);
}

static double __cuda_pow(double a, double b)
{
  return (double)__cuda_powf((float)a, (float)b);
}

static double __cuda_round(double a)
{
  return (double)__cuda_roundf((float)a);
}

static long __cuda_lround(double a)
{
  return __cuda_lroundf((float)a);
}

static long long __cuda_llround(double a)
{
  return __cuda_llroundf((float)a);
}

static double __cuda_rint(double a)
{
  return (double)__cuda_rintf((float)a);
}

static long __cuda_lrint(double a)
{
  return __cuda_lrintf((float)a);
}

static long long __cuda_llrint(double a)
{
  return __cuda_llrintf((float)a);
}

static double __cuda_nearbyint(double a)
{
  return (double)__cuda_nearbyintf((float)a);
}

static double __cuda_fdim(double a, double b)
{
  return (double)__cuda_fdimf((float)a, (float)b);
}

static int __cuda_ilogb(double a)
{
  return __cuda_ilogbf((float)a);
}

static double __cuda_logb(double a)
{
  return (double)__cuda_logbf((float)a);
}

static double __cuda_fma(double a, double b, double c)
{
  return (double)__cuda_fmaf((float)a, (float)b, (float)c);
}











#line 391 "c:\\cuda\\include\\math_functions_dbl_ptx1.h"

#line 393 "c:\\cuda\\include\\math_functions_dbl_ptx1.h"

#line 395 "c:\\cuda\\include\\math_functions_dbl_ptx1.h"
#line 3864 "c:\\cuda\\include\\math_functions.h"

#line 3866 "c:\\cuda\\include\\math_functions.h"





#line 3872 "c:\\cuda\\include\\math_functions.h"

#line 3874 "c:\\cuda\\include\\math_functions.h"

#line 94 "C:\\CUDA\\include\\common_functions.h"

#line 96 "C:\\CUDA\\include\\common_functions.h"

#line 275 "C:\\CUDA\\include\\crt/host_runtime.h"

#line 277 "C:\\CUDA\\include\\crt/host_runtime.h"



















#pragma warning(disable: 4099)

#line 299 "C:\\CUDA\\include\\crt/host_runtime.h"

#line 301 "C:\\CUDA\\include\\crt/host_runtime.h"
#line 6 "e:\\project\\flowentropy\\entropy_final_2d\\myinterface\\FlowFusion.cudafe1.stub.c"



#line 10 "e:\\project\\flowentropy\\entropy_final_2d\\myinterface\\FlowFusion.cudafe1.stub.c"
struct __T20 {float __par0;int __par1;int __par2;struct cudaPitchedPtr __par3;struct cudaPitchedPtr __par4;int __dummy_field;};



#line 15 "e:\\project\\flowentropy\\entropy_final_2d\\myinterface\\FlowFusion.cudafe1.stub.c"
struct __T21 {float __par0;int __par1;int __par2;int __par3;int __par4;int __par5;int __par6;struct cudaPitchedPtr __par7;struct cudaPitchedPtr __par8;int __dummy_field;};
static void __sti____cudaRegisterAll_18_FlowFusion_cpp1_ii_abb6e4d2(void);
#pragma section(".CRT$XCU",read,write)
__declspec(allocate(".CRT$XCU"))static void (__cdecl *__dummy_static_init__sti____cudaRegisterAll_18_FlowFusion_cpp1_ii_abb6e4d2[])(void) = {__sti____cudaRegisterAll_18_FlowFusion_cpp1_ii_abb6e4d2};
static void __device_stub__Z20_FlowFusion2D_kernelfii14cudaPitchedPtrS_(float __par0, int __par1, int __par2, struct cudaPitchedPtr & __par3, struct cudaPitchedPtr & __par4){auto struct __T20 *__T22;
*(void**)(void*)&__T22 = (void*)0;if (cudaSetupArgument((void*)(char*)&__par0, sizeof(__par0), (size_t)&__T22->__par0) != cudaSuccess) return;if (cudaSetupArgument((void*)(char*)&__par1, sizeof(__par1), (size_t)&__T22->__par1) != cudaSuccess) return;if (cudaSetupArgument((void*)(char*)&__par2, sizeof(__par2), (size_t)&__T22->__par2) != cudaSuccess) return;if (cudaSetupArgument((void*)(char*)&__par3, sizeof(__par3), (size_t)&__T22->__par3) != cudaSuccess) return;if (cudaSetupArgument((void*)(char*)&__par4, sizeof(__par4), (size_t)&__T22->__par4) != cudaSuccess) return;{ volatile static char *__f; __f = ((char *)((void ( *)(float, int, int, struct cudaPitchedPtr, struct cudaPitchedPtr))_FlowFusion2D_kernel__entry)); (void)cudaLaunch(((char *)((void ( *)(float, int, int, struct cudaPitchedPtr, struct cudaPitchedPtr))_FlowFusion2D_kernel__entry))); };}

}
static void _FlowFusion2D_kernel__entry( float __cuda_0,int __cuda_1,int __cuda_2,struct cudaPitchedPtr __cuda_3,struct cudaPitchedPtr __cuda_4)
{__device_stub__Z20_FlowFusion2D_kernelfii14cudaPitchedPtrS_( __cuda_0,__cuda_1,__cuda_2,__cuda_3,__cuda_4);}
extern "C"{
#line 27 "e:\\project\\flowentropy\\entropy_final_2d\\myinterface\\FlowFusion.cudafe1.stub.c"
static void __device_stub__Z18_FlowFusion_kernelfiiiiii14cudaPitchedPtrS_(float __par0, int __par1, int __par2, int __par3, int __par4, int __par5, int __par6, struct cudaPitchedPtr & __par7, struct cudaPitchedPtr & __par8){auto struct __T21 *__T23;
*(void**)(void*)&__T23 = (void*)0;if (cudaSetupArgument((void*)(char*)&__par0, sizeof(__par0), (size_t)&__T23->__par0) != cudaSuccess) return;if (cudaSetupArgument((void*)(char*)&__par1, sizeof(__par1), (size_t)&__T23->__par1) != cudaSuccess) return;if (cudaSetupArgument((void*)(char*)&__par2, sizeof(__par2), (size_t)&__T23->__par2) != cudaSuccess) return;if (cudaSetupArgument((void*)(char*)&__par3, sizeof(__par3), (size_t)&__T23->__par3) != cudaSuccess) return;if (cudaSetupArgument((void*)(char*)&__par4, sizeof(__par4), (size_t)&__T23->__par4) != cudaSuccess) return;if (cudaSetupArgument((void*)(char*)&__par5, sizeof(__par5), (size_t)&__T23->__par5) != cudaSuccess) return;if (cudaSetupArgument((void*)(char*)&__par6, sizeof(__par6), (size_t)&__T23->__par6) != cudaSuccess) return;if (cudaSetupArgument((void*)(char*)&__par7, sizeof(__par7), (size_t)&__T23->__par7) != cudaSuccess) return;if (cudaSetupArgument((void*)(char*)&__par8, sizeof(__par8), (size_t)&__T23->__par8) != cudaSuccess) return;{ volatile static char *__f; __f = ((char *)((void ( *)(float, int, int, int, int, int, int, struct cudaPitchedPtr, struct cudaPitchedPtr))_FlowFusion_kernel__entry)); (void)cudaLaunch(((char *)((void ( *)(float, int, int, int, int, int, int, struct cudaPitchedPtr, struct cudaPitchedPtr))_FlowFusion_kernel__entry))); };}

}
static void _FlowFusion_kernel__entry( float __cuda_0,int __cuda_1,int __cuda_2,int __cuda_3,int __cuda_4,int __cuda_5,int __cuda_6,struct cudaPitchedPtr __cuda_7,struct cudaPitchedPtr __cuda_8)
{__device_stub__Z18_FlowFusion_kernelfiiiiii14cudaPitchedPtrS_( __cuda_0,__cuda_1,__cuda_2,__cuda_3,__cuda_4,__cuda_5,__cuda_6,__cuda_7,__cuda_8);}
extern "C"{
#line 35 "e:\\project\\flowentropy\\entropy_final_2d\\myinterface\\FlowFusion.cudafe1.stub.c"








#line 44 "e:\\project\\flowentropy\\entropy_final_2d\\myinterface\\FlowFusion.cudafe1.stub.c"








#line 53 "e:\\project\\flowentropy\\entropy_final_2d\\myinterface\\FlowFusion.cudafe1.stub.c"
static void __sti____cudaRegisterAll_18_FlowFusion_cpp1_ii_abb6e4d2(void){__cudaFatCubinHandle = __cudaRegisterFatBinary((void*)(&__fatDeviceText)); atexit(__cudaUnregisterBinaryUtil);__cudaRegisterFunction(__cudaFatCubinHandle, (const char*)((void ( *)(float, int, int, int, int, int, int, struct cudaPitchedPtr, struct cudaPitchedPtr))_FlowFusion_kernel__entry), (char*)"_Z18_FlowFusion_kernelfiiiiii14cudaPitchedPtrS_", "_Z18_FlowFusion_kernelfiiiiii14cudaPitchedPtrS_", (-1), (uint3*)0, (uint3*)0, (dim3*)0, (dim3*)0, (int*)0);__cudaRegisterFunction(__cudaFatCubinHandle, (const char*)((void ( *)(float, int, int, struct cudaPitchedPtr, struct cudaPitchedPtr))_FlowFusion2D_kernel__entry), (char*)"_Z20_FlowFusion2D_kernelfii14cudaPitchedPtrS_", "_Z20_FlowFusion2D_kernelfii14cudaPitchedPtrS_", (-1), (uint3*)0, (uint3*)0, (dim3*)0, (dim3*)0, (int*)0);__cudaRegisterTexture(__cudaFatCubinHandle, (const struct textureReference*)&t2dSrc, 0, "t2dSrc", 2, 0, 0);__cudaRegisterTexture(__cudaFatCubinHandle, (const struct textureReference*)&t3dWeightOffset, 0, "t3dWeightOffset", 3, 0, 0);}

}
#line 57 "e:\\project\\flowentropy\\entropy_final_2d\\myinterface\\FlowFusion.cudafe1.stub.c"
#line 827 "e:/project/FlowEntropy/FlowFusion/FlowFusion_test/FlowFusion.cu"

