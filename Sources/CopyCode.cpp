// Author: Manao
// See legal notice in Copying.txt for more information
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA, or visit
// http://www.gnu.org/copyleft/gpl.html .

#include "CopyCode.h"
#include <map>
#include <tuple>
#include <stdint.h>

#if !defined(_M_X64)
#define rax	eax
#define rbx	ebx
#define rcx	ecx
#define rdx	edx
#define rsi	esi
#define rdi	edi
#define rbp	ebp
#define rsp	esp
#define movzx_int mov
#define movsx_int mov
#else
#define rax	rax
#define rbx	rbx
#define rcx	rcx
#define rdx	rdx
#define rsi	rsi
#define rdi	rdi
#define rbp	rbp
#define rsp	r15
#define movzx_int movzxd
#define movsx_int movsxd
#endif

// I use here the copy code from avisynth. I duplicated it, because I have to use it
// in a class which doesn't have to know what "env" is. Anyway, such static functions
// should not have been put into that class in the first place ( imho )

void BitBlt(unsigned char* dstp, int dst_pitch, const unsigned char* srcp, int src_pitch, int row_size, int height, bool isse) {
  if ( (!height)|| (!row_size)) return;
#if 0
#ifdef X86_32
  if (isse) {
    if (height == 1 || (src_pitch == dst_pitch && dst_pitch == row_size)) {
//      memcpy_amd(dstp, srcp, row_size*height);
		memcpy(dstp, srcp, row_size*height); // P.F. no memcpy_amd here
	} else {
      asm_BitBlt_ISSE(dstp,dst_pitch,srcp,src_pitch,row_size,height);
    }
    return;
  }
#endif
#endif
  if (height == 1 || (dst_pitch == src_pitch && src_pitch == row_size)) {
    memcpy(dstp, srcp, row_size*height); // Fizick: fixed bug
  } else {
    for (int y=height; y>0; --y) {
      memcpy(dstp, srcp, row_size);
      dstp += dst_pitch;
      srcp += src_pitch;
    }
  }
}

void MemZoneSet(unsigned char *ptr, unsigned char value, int width,
				int height, int offsetX, int offsetY, int pitch)
{
	ptr += offsetX + offsetY*pitch;
	for (int y=offsetY; y<height+offsetY; y++)
	{
		memset(ptr, value, width);
		ptr += pitch;
	}
}

#if 0
// Coded by Steady
void asm_BitBlt_ISSE(unsigned char* dstp, int dst_pitch, const unsigned char* srcp, int src_pitch, int row_size, int height) {

  if(row_size==0 || height==0) return; //abort on goofs
  //move backwards for easier looping and to disable hardware prefetch
  const unsigned char* srcStart=srcp+src_pitch*(height-1);
  unsigned char* dstStart=dstp+dst_pitch*(height-1);

  if(row_size < 64) {
    _asm {
#if !defined(_M_X64)
#define src_pitch_ src_pitch
#define dst_pitch_ dst_pitch
#else
#define src_pitch_ r8
#define dst_pitch_ r9
		movsx_int src_pitch_,src_pitch
		movsx_int dst_pitch_,dst_pitch
#endif
      mov   rsi,srcStart  //move rows from bottom up
      mov   rdi,dstStart
      mov   edx,row_size
      dec   edx
      mov   ebx,height
      align 16
memoptS_rowloop:
      mov   ecx,edx
//      rep movsb
memoptS_byteloop:
      mov   AL,[rsi+rcx]
      mov   [rdi+rcx],AL
      sub   ecx,1
      jnc   memoptS_byteloop
      sub   rsi,src_pitch_
      sub   rdi,dst_pitch_
      dec   ebx
      jne   memoptS_rowloop
#undef src_pitch_
#undef dst_pitch_
    };
    return;
  }//end small version

  else if( (intptr_t(dstp) | row_size | src_pitch | dst_pitch) & 7) {//not QW aligned
    //unaligned version makes no assumptions on alignment

    _asm {
//****** initialize
#if !defined(_M_X64)
#define src_pitch_ src_pitch
#define dst_pitch_ dst_pitch
#else
#define src_pitch_ r8
#define dst_pitch_ r9
		movsx_int src_pitch_,src_pitch
		movsx_int dst_pitch_,dst_pitch
#endif
      mov   rsi,srcStart  //bottom row
      mov   AL,[rsi]
      mov   rdi,dstStart
      mov   edx,row_size
      mov   ebx,height

//********** loop starts here ***********

      align 16
memoptU_rowloop:
      mov   ecx,edx     //row_size
      dec   ecx         //offset to last byte in row
      add   rcx,rsi     //ecx= ptr last byte in row
      and   rcx,~63     //align to first byte in cache line
memoptU_prefetchloop:
      mov   AX,[rcx]    //tried AL,AX,EAX, AX a tiny bit faster
      sub   rcx,64
      cmp   rcx,rsi
      jae   memoptU_prefetchloop

//************ write *************

      movq    mm6,[rsi]     //move the first unaligned bytes
      movntq  [rdi],mm6
//************************
      mov   rax,rdi
      neg   rax
      mov   rcx,rax
      and   eax,63      //eax=bytes from [edi] to start of next 64 byte cache line
      and   ecx,7       //ecx=bytes from [edi] to next QW
      align 16
memoptU_prewrite8loop:        //write out odd QW's so 64 bit write is cache line aligned
      cmp   ecx,eax           //start of cache line ?
      jz    memoptU_pre8done  //if not, write single QW
      movq    mm7,[rsi+rcx]
      movntq  [rdi+rcx],mm7
      add   ecx,8
      jmp   memoptU_prewrite8loop

      align 16
memoptU_write64loop:
      movntq  [rdi+rcx-64],mm0
      movntq  [rdi+rcx-56],mm1
      movntq  [rdi+rcx-48],mm2
      movntq  [rdi+rcx-40],mm3
      movntq  [rdi+rcx-32],mm4
      movntq  [rdi+rcx-24],mm5
      movntq  [rdi+rcx-16],mm6
      movntq  [rdi+rcx- 8],mm7
memoptU_pre8done:
      add   ecx,64
      cmp   ecx,edx         //while(offset <= row_size) do {...
      ja    memoptU_done64
      movq    mm0,[rsi+rcx-64]
      movq    mm1,[rsi+rcx-56]
      movq    mm2,[rsi+rcx-48]
      movq    mm3,[rsi+rcx-40]
      movq    mm4,[rsi+rcx-32]
      movq    mm5,[rsi+rcx-24]
      movq    mm6,[rsi+rcx-16]
      movq    mm7,[rsi+rcx- 8]
      jmp   memoptU_write64loop
memoptU_done64:

      sub     ecx,64    //went to far
      align 16
memoptU_write8loop:
      add     ecx,8           //next QW
      cmp     ecx,edx         //any QW's left in row ?
      ja      memoptU_done8
      movq    mm0,[rsi+rcx-8]
      movntq  [rdi+rcx-8],mm0
      jmp   memoptU_write8loop
memoptU_done8:

      movq    mm1,[rsi+rdx-8] //write the last unaligned bytes
      movntq  [rdi+rdx-8],mm1
      sub   rsi,src_pitch_
      sub   rdi,dst_pitch_
	  dec   ebx               //row counter (=height at start)
      jne   memoptU_rowloop

      sfence
      emms
#undef src_pitch_
#undef dst_pitch_
    };
    return;
  }//end unaligned version

  else {//QW aligned version (fastest)
  //else dstp and row_size QW aligned - hope for the best from srcp
  //QW aligned version should generally be true when copying full rows
    _asm {
#if !defined(_M_X64)
#define src_pitch_ src_pitch
#define dst_pitch_ dst_pitch
#else
#define src_pitch_ r8
#define dst_pitch_ r9
		movsx_int src_pitch_,src_pitch
		movsx_int dst_pitch_,dst_pitch
#endif
      mov   rsi,srcStart  //start of bottom row
      mov   rdi,dstStart
      mov   ebx,height
      mov   edx,row_size
     align 16
memoptA_rowloop:
      mov   ecx,edx //row_size
      dec   ecx     //offset to last byte in row

//********forward routine
      add   rcx,rsi
      and   rcx,~63   //align prefetch to first byte in cache line(~3-4% faster)
      align 16
memoptA_prefetchloop:
      mov   AX,[rcx]
      sub   rcx,64
      cmp   rcx,rsi
      jae   memoptA_prefetchloop

      mov   rax,rdi
      xor   ecx,ecx
      neg   rax
      and   eax,63            //eax=bytes from edi to start of cache line
      align 16
memoptA_prewrite8loop:        //write out odd QW's so 64bit write is cache line aligned
      cmp   ecx,eax           //start of cache line ?
      jz    memoptA_pre8done  //if not, write single QW
      movq    mm7,[rsi+rcx]
      movntq  [rdi+rcx],mm7
      add   ecx,8
      jmp   memoptA_prewrite8loop

      align 16
memoptA_write64loop:
      movntq  [rdi+rcx-64],mm0
      movntq  [rdi+rcx-56],mm1
      movntq  [rdi+rcx-48],mm2
      movntq  [rdi+rcx-40],mm3
      movntq  [rdi+rcx-32],mm4
      movntq  [rdi+rcx-24],mm5
      movntq  [rdi+rcx-16],mm6
      movntq  [rdi+rcx- 8],mm7
memoptA_pre8done:
      add   ecx,64
      cmp   ecx,edx
      ja    memoptA_done64    //less than 64 bytes left
      movq    mm0,[rsi+rcx-64]
      movq    mm1,[rsi+rcx-56]
      movq    mm2,[rsi+rcx-48]
      movq    mm3,[rsi+rcx-40]
      movq    mm4,[rsi+rcx-32]
      movq    mm5,[rsi+rcx-24]
      movq    mm6,[rsi+rcx-16]
      movq    mm7,[rsi+rcx- 8]
      jmp   memoptA_write64loop

memoptA_done64:
      sub   ecx,64

      align 16
memoptA_write8loop:           //less than 8 QW's left
      add   ecx,8
      cmp   ecx,edx
      ja    memoptA_done8     //no QW's left
      movq    mm7,[rsi+rcx-8]
      movntq  [rdi+rcx-8],mm7
      jmp   memoptA_write8loop

memoptA_done8:
      sub   rsi,src_pitch_
      sub   rdi,dst_pitch_
      dec   ebx               //row counter (height)
      jne   memoptA_rowloop

      sfence
      emms
#undef src_pitch_
#undef dst_pitch_
    };
    return;
  }//end aligned version
}//end BitBlt_memopt()
#endif

template<int nBlkWidth, int nBlkHeight, typename pixel_t>
void Copy_C (uint8_t *pDst, int nDstPitch, const uint8_t *pSrc, int nSrcPitch)
{
    for (int j = 0; j < nBlkHeight; j++ )
    {
        //      for ( int i = 0; i < nBlkWidth; i++ )  //  waste cycles removed by Fizick in v1.2
      memcpy(pDst, pSrc, nBlkWidth * sizeof(pixel_t));
      pDst += nDstPitch;
      pSrc += nSrcPitch;
    }
}

template<int nBlkSize, typename pixel_t>
void Copy_C(uint8_t *pDst, int nDstPitch, const uint8_t *pSrc, int nSrcPitch)
{
    Copy_C<nBlkSize, nBlkSize, pixel_t>(pDst, nDstPitch, pSrc, nSrcPitch);
}


COPYFunction* get_copy_function(int BlockX, int BlockY, int pixelsize, arch_t arch)
{
    // 8 bit only (pixelsize==1)
    //---------- DENOISE/DEGRAIN
    // BlkSizeX, BlkSizeY, pixelsize, arch_t
    std::map<std::tuple<int, int, int, arch_t>, COPYFunction*> func_copy;
    using std::make_tuple;

    func_copy[make_tuple(32, 32, 1, NO_SIMD)] = Copy_C<32, 32,uint8_t>;
    func_copy[make_tuple(32, 16, 1, NO_SIMD)] = Copy_C<32, 16,uint8_t>;
    func_copy[make_tuple(32, 8 , 1, NO_SIMD)] = Copy_C<32, 8,uint8_t>;
    func_copy[make_tuple(16, 32, 1, NO_SIMD)] = Copy_C<16, 32,uint8_t>;
    func_copy[make_tuple(16, 16, 1, NO_SIMD)] = Copy_C<16, 16,uint8_t>;
    func_copy[make_tuple(16, 8 , 1, NO_SIMD)] = Copy_C<16, 8,uint8_t>;
    func_copy[make_tuple(16, 4 , 1, NO_SIMD)] = Copy_C<16, 4,uint8_t>;
    func_copy[make_tuple(16, 2 , 1, NO_SIMD)] = Copy_C<16, 2,uint8_t>;
    func_copy[make_tuple(8 , 16, 1, NO_SIMD)] = Copy_C<8 , 16,uint8_t>;
    func_copy[make_tuple(8 , 8 , 1, NO_SIMD)] = Copy_C<8 , 8,uint8_t>;
    func_copy[make_tuple(8 , 4 , 1, NO_SIMD)] = Copy_C<8 , 4,uint8_t>;
    func_copy[make_tuple(8 , 2 , 1, NO_SIMD)] = Copy_C<8 , 2,uint8_t>;
    func_copy[make_tuple(8 , 1 , 1, NO_SIMD)] = Copy_C<8 , 1,uint8_t>;
    func_copy[make_tuple(4 , 8 , 1, NO_SIMD)] = Copy_C<4 , 8,uint8_t>;
    func_copy[make_tuple(4 , 4 , 1, NO_SIMD)] = Copy_C<4 , 4,uint8_t>;
    func_copy[make_tuple(4 , 2 , 1, NO_SIMD)] = Copy_C<4 , 2,uint8_t>;
    func_copy[make_tuple(2 , 4 , 1, NO_SIMD)] = Copy_C<2 , 4,uint8_t>;
    func_copy[make_tuple(2 , 2 , 1, NO_SIMD)] = Copy_C<2 , 2,uint8_t>;
    func_copy[make_tuple(2 , 1 , 1, NO_SIMD)] = Copy_C<2 , 1,uint8_t>;

    func_copy[make_tuple(32, 32, 2, NO_SIMD)] = Copy_C<32, 32,uint16_t>;
    func_copy[make_tuple(32, 16, 2, NO_SIMD)] = Copy_C<32, 16,uint16_t>;
    func_copy[make_tuple(32, 8 , 2, NO_SIMD)] = Copy_C<32, 8,uint16_t>;
    func_copy[make_tuple(16, 32, 2, NO_SIMD)] = Copy_C<16, 32,uint16_t>;
    func_copy[make_tuple(16, 16, 2, NO_SIMD)] = Copy_C<16, 16,uint16_t>;
    func_copy[make_tuple(16, 8 , 2, NO_SIMD)] = Copy_C<16, 8,uint16_t>;
    func_copy[make_tuple(16, 4 , 2, NO_SIMD)] = Copy_C<16, 4,uint16_t>;
    func_copy[make_tuple(16, 2 , 2, NO_SIMD)] = Copy_C<16, 2,uint16_t>;
    func_copy[make_tuple(8 , 16, 2, NO_SIMD)] = Copy_C<8 , 16,uint16_t>;
    func_copy[make_tuple(8 , 8 , 2, NO_SIMD)] = Copy_C<8 , 8,uint16_t>;
    func_copy[make_tuple(8 , 4 , 2, NO_SIMD)] = Copy_C<8 , 4,uint16_t>;
    func_copy[make_tuple(8 , 2 , 2, NO_SIMD)] = Copy_C<8 , 2,uint16_t>;
    func_copy[make_tuple(8 , 1 , 2, NO_SIMD)] = Copy_C<8 , 1,uint16_t>;
    func_copy[make_tuple(4 , 8 , 2, NO_SIMD)] = Copy_C<4 , 8,uint16_t>;
    func_copy[make_tuple(4 , 4 , 2, NO_SIMD)] = Copy_C<4 , 4,uint16_t>;
    func_copy[make_tuple(4 , 2 , 2, NO_SIMD)] = Copy_C<4 , 2,uint16_t>;
    func_copy[make_tuple(2 , 4 , 2, NO_SIMD)] = Copy_C<2 , 4,uint16_t>;
    func_copy[make_tuple(2 , 2 , 2, NO_SIMD)] = Copy_C<2 , 2,uint16_t>;
    func_copy[make_tuple(2 , 1 , 2, NO_SIMD)] = Copy_C<2 , 1,uint16_t>;
    
    // we could even ignore copy sse2 assemblers, compilers are smart nowadays
    func_copy[make_tuple(32, 32, 1, USE_SSE2)] = Copy32x32_sse2;
    func_copy[make_tuple(32, 16, 1, USE_SSE2)] = Copy32x16_sse2;
    func_copy[make_tuple(32, 8 , 1, USE_SSE2)] = Copy32x8_sse2;
    func_copy[make_tuple(16, 32, 1, USE_SSE2)] = Copy16x32_sse2;
    func_copy[make_tuple(16, 16, 1, USE_SSE2)] = Copy16x16_sse2;
    func_copy[make_tuple(16, 8 , 1, USE_SSE2)] = Copy16x8_sse2;
    func_copy[make_tuple(16, 4 , 1, USE_SSE2)] = Copy16x4_sse2;
    func_copy[make_tuple(16, 2 , 1, USE_SSE2)] = Copy16x2_sse2;
    func_copy[make_tuple(8 , 16, 1, USE_SSE2)] = Copy8x16_sse2;
    func_copy[make_tuple(8 , 8 , 1, USE_SSE2)] = Copy8x8_sse2;
    func_copy[make_tuple(8 , 4 , 1, USE_SSE2)] = Copy8x4_sse2;
    func_copy[make_tuple(8 , 2 , 1, USE_SSE2)] = Copy8x2_sse2;
    func_copy[make_tuple(8 , 1 , 1, USE_SSE2)] = Copy8x1_sse2;
    func_copy[make_tuple(4 , 8 , 1, USE_SSE2)] = Copy4x8_sse2;
    func_copy[make_tuple(4 , 4 , 1, USE_SSE2)] = Copy4x4_sse2;
    func_copy[make_tuple(4 , 2 , 1, USE_SSE2)] = Copy4x2_sse2;
    func_copy[make_tuple(2 , 4 , 1, USE_SSE2)] = Copy2x4_sse2;
    func_copy[make_tuple(2 , 2 , 1, USE_SSE2)] = Copy2x2_sse2;
    //func_copy[make_tuple(2 , 1 , 1, USE_SSE2)] = Copy2x1_sse2; no such

    COPYFunction *result = nullptr;
    arch_t archlist[] = { USE_AVX2, USE_AVX, USE_SSE41, USE_SSE2, NO_SIMD };
    int index = 0;
    while (result == nullptr) {
      arch_t current_arch_try = archlist[index++];
      if (current_arch_try > arch) continue;
      result = func_copy[make_tuple(BlockX, BlockY, pixelsize, current_arch_try)];
      if (result == nullptr && current_arch_try == NO_SIMD)
        break;
    }
#if 0
    COPYFunction *result = func_copy[make_tuple(BlockX, BlockY, pixelsize, arch)];

    arch_t arch_orig = arch;



    // no AVX2 -> try AVX
    if (result == nullptr && (arch==USE_AVX2 || arch_orig==USE_AVX)) {
      arch = USE_AVX;
      result = func_copy[make_tuple(BlockX, BlockY, pixelsize, USE_AVX)];
    }
    // no AVX -> try SSE41
    if (result == nullptr && (arch==USE_AVX || arch_orig==USE_SSE41)) {
      arch = USE_SSE41;
      result = func_copy[make_tuple(BlockX, BlockY, pixelsize, USE_SSE41)];
    }
    // no SSE41 -> try SSE2
    if (result == nullptr && (arch==USE_SSE41 || arch_orig==USE_SSE2)) {
      arch = USE_SSE2;
      result = func_copy[make_tuple(BlockX, BlockY, pixelsize, USE_SSE2)];
    }
    // no SSE2 -> try C
    if (result == nullptr && (arch==USE_SSE2 || arch_orig==NO_SIMD)) {
      arch = NO_SIMD;
      /* C version variations are only working in SAD
      // priority: C version compiled to avx2, avx
      if(arch_orig==USE_AVX2)
        result = get_copy_avx2_C_function(BlockX, BlockY, pixelsize, NO_SIMD);
      else if(arch_orig==USE_AVX)
        result = get_copy_avx_C_function(BlockX, BlockY, pixelsize, NO_SIMD);
      */
      if(result == nullptr)
        result = func_copy[make_tuple(BlockX, BlockY, pixelsize, NO_SIMD)]; // fallback to C
    }
#endif

    return result;
}


