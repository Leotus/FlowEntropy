#if defined(__cplusplus)
extern "C" {
#endif
#include "FlowFusion.fatbin.c"
#include "crt/host_runtime.h"
#if defined(__device_emulation) && defined(__cplusplus)
namespace __cuda_emu {struct __T20 {float __par0;int __par1;int __par2;int __par3;int __par4;int __par5;int __par6;struct cudaPitchedPtr __par7;struct cudaPitchedPtr __par8;int __dummy_field;};
}
#endif
struct __T20 {float __par0;int __par1;int __par2;int __par3;int __par4;int __par5;int __par6;struct cudaPitchedPtr __par7;struct cudaPitchedPtr __par8;int __dummy_field;};
static void __sti____cudaRegisterAll_18_FlowFusion_cpp1_ii_abb6e4d2(void);
#pragma section(".CRT$XCU",read,write)
__declspec(allocate(".CRT$XCU"))static void (__cdecl *__dummy_static_init__sti____cudaRegisterAll_18_FlowFusion_cpp1_ii_abb6e4d2[])(void) = {__sti____cudaRegisterAll_18_FlowFusion_cpp1_ii_abb6e4d2};
static void __device_stub__Z18_FlowFusion_kernelfiiiiii14cudaPitchedPtrS_(float __par0, int __par1, int __par2, int __par3, int __par4, int __par5, int __par6, struct cudaPitchedPtr __cppref__ __par7, struct cudaPitchedPtr __cppref__ __par8){auto struct __T20 *__T21;
__cudaInitArgBlock(__T21);__cudaSetupArg(__par0, __T21);__cudaSetupArg(__par1, __T21);__cudaSetupArg(__par2, __T21);__cudaSetupArg(__par3, __T21);__cudaSetupArg(__par4, __T21);__cudaSetupArg(__par5, __T21);__cudaSetupArg(__par6, __T21);__cudaSetupArg(__par7, __T21);__cudaSetupArg(__par8, __T21);__cudaLaunch(((char *)__device_stub_name(__device_stub__Z18_FlowFusion_kernelfiiiiii14cudaPitchedPtrS_,((void ( *)(float, int, int, int, int, int, int, struct cudaPitchedPtr, struct cudaPitchedPtr))_FlowFusion_kernel__entry))));}
#if defined(__cplusplus)
}
static void _FlowFusion_kernel__entry( float __cuda_0,int __cuda_1,int __cuda_2,int __cuda_3,int __cuda_4,int __cuda_5,int __cuda_6,struct cudaPitchedPtr __cuda_7,struct cudaPitchedPtr __cuda_8)
{__device_stub__Z18_FlowFusion_kernelfiiiiii14cudaPitchedPtrS_( __cuda_0,__cuda_1,__cuda_2,__cuda_3,__cuda_4,__cuda_5,__cuda_6,__cuda_7,__cuda_8);}
extern "C"{
#endif
#if defined(__device_emulation)
#if defined(__cplusplus)
namespace __cuda_emu {
#endif
static void __device_wrapper__Z18_FlowFusion_kernelfiiiiii14cudaPitchedPtrS_(char *__T2129){_Z18_FlowFusion_kernelfiiiiii14cudaPitchedPtrS_((((*((struct __T20 *)__T2129)).__par0)), (((*((struct __T20 *)__T2129)).__par1)), (((*((struct __T20 *)__T2129)).__par2)), (((*((struct __T20 *)__T2129)).__par3)), (((*((struct __T20 *)__T2129)).__par4)), (((*((struct __T20 *)__T2129)).__par5)), (((*((struct __T20 *)__T2129)).__par6)), (((*((struct __T20 *)__T2129)).__par7)), (((*((struct __T20 *)__T2129)).__par8)));}
#if defined(__cplusplus)
}
#endif
#endif
static void __sti____cudaRegisterAll_18_FlowFusion_cpp1_ii_abb6e4d2(void){__cudaRegisterBinary();__cudaRegisterEntry(__device_stub_name(__device_stub__Z18_FlowFusion_kernelfiiiiii14cudaPitchedPtrS_,((void ( *)(float, int, int, int, int, int, int, struct cudaPitchedPtr, struct cudaPitchedPtr))_FlowFusion_kernel__entry)), _Z18_FlowFusion_kernelfiiiiii14cudaPitchedPtrS_, (-1));__cudaRegisterGlobalTexture(__text_var(t2dSrc,t2dSrc), 2, 0, 0);__cudaRegisterGlobalTexture(__text_var(t3dWeightOffset,t3dWeightOffset), 3, 0, 0);}
#if defined(__cplusplus)
}
#endif
