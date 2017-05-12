#include "SADFunctions_avx.h"
//#include "overlap.h"
#include <map>
#include <tuple>

#include <emmintrin.h>
#include "def.h"

MV_FORCEINLINE unsigned int SADABS(int x) {	return ( x < 0 ) ? -x : x; }

// simple AVX still does not support 256 bit integer SIMD versions of SSE2 functions
// Still the compiler can generate more optimized code
template<int nBlkWidth, int nBlkHeight, typename pixel_t>
static unsigned int Sad_AVX_C(const uint8_t *pSrc, int nSrcPitch,const uint8_t *pRef,
  int nRefPitch)
{
  _mm256_zeroupper();
  unsigned int sum = 0; // int is probably enough for 32x32
  for ( int y = 0; y < nBlkHeight; y++ )
  {
    for ( int x = 0; x < nBlkWidth; x++ )
      sum += SADABS(reinterpret_cast<const pixel_t *>(pSrc)[x] - reinterpret_cast<const pixel_t *>(pRef)[x]);
    pSrc += nSrcPitch;
    pRef += nRefPitch;
  }
  _mm256_zeroupper();
  return sum;
}



SADFunction* get_sad_avx_C_function(int BlockX, int BlockY, int pixelsize, arch_t arch)
{
    // BlkSizeX, BlkSizeY, pixelsize, arch_t
    std::map<std::tuple<int, int, int, arch_t>, SADFunction*> func_sad;
    using std::make_tuple;

    /*
    64x64, 64x48, 64x32, 64x16
    48x64
    32x64, 32x32, 32x24, 32x16, 32x8
    24x32
    16x64, 16x32, 16x16, 16x12, 16x8, 16x4, (16x2)
    12x16
    8x32, 8x16, 8x8, 8x4, (8x2, 8x1)
    4x8, 4x4, 4x2
    2x4, 2x2
    */
#define MAKE_SAD_FN(x, y) func_sad[make_tuple(x, y, 1, NO_SIMD)] = Sad_AVX_C<x, y, uint8_t>; \
func_sad[make_tuple(x, y, 2, NO_SIMD)] = Sad_AVX_C<x, y, uint16_t>;
    MAKE_SAD_FN(64, 64)
      MAKE_SAD_FN(64, 48)
      MAKE_SAD_FN(64, 32)
      MAKE_SAD_FN(64, 16)
      MAKE_SAD_FN(48, 64)
      MAKE_SAD_FN(48, 48)
      MAKE_SAD_FN(48, 24)
      MAKE_SAD_FN(48, 12)
      MAKE_SAD_FN(32, 64)
      MAKE_SAD_FN(32, 32)
      MAKE_SAD_FN(32, 24)
      MAKE_SAD_FN(32, 16)
      MAKE_SAD_FN(32, 8)
      MAKE_SAD_FN(24, 48)
      MAKE_SAD_FN(24, 32)
      MAKE_SAD_FN(24, 24)
      MAKE_SAD_FN(24, 12)
      MAKE_SAD_FN(24, 6)
      MAKE_SAD_FN(16, 64)
      MAKE_SAD_FN(16, 32)
      MAKE_SAD_FN(16, 16)
      MAKE_SAD_FN(16, 12)
      MAKE_SAD_FN(16, 8)
      MAKE_SAD_FN(16, 4)
      MAKE_SAD_FN(16, 2)
      MAKE_SAD_FN(16, 1)
      MAKE_SAD_FN(12, 48)
      MAKE_SAD_FN(12, 24)
      MAKE_SAD_FN(12, 16)
      MAKE_SAD_FN(12, 6)
      MAKE_SAD_FN(8, 32)
      MAKE_SAD_FN(8, 16)
      MAKE_SAD_FN(8, 8)
      MAKE_SAD_FN(8, 4)
      MAKE_SAD_FN(8, 2)
      MAKE_SAD_FN(8, 1)
      MAKE_SAD_FN(6, 12)
      MAKE_SAD_FN(6, 6)
      MAKE_SAD_FN(6, 3)
      MAKE_SAD_FN(4, 8)
      MAKE_SAD_FN(4, 4)
      MAKE_SAD_FN(4, 2)
      MAKE_SAD_FN(4, 1)
      MAKE_SAD_FN(3, 6)
      MAKE_SAD_FN(3, 3)
      MAKE_SAD_FN(2, 4)
      MAKE_SAD_FN(2, 2)
      MAKE_SAD_FN(2, 1)
#undef MAKE_SAD_FN

    SADFunction *result = func_sad[make_tuple(BlockX, BlockY, pixelsize, arch)];
    return result;
}

// above width==4 for uint16_t and width==8 for uint8_t
template<int nBlkWidth, int nBlkHeight, typename pixel_t>
unsigned int Sad16_sse2_avx(const uint8_t *pSrc, int nSrcPitch, const uint8_t *pRef, int nRefPitch)
{
#if 0
  // check result against C
  unsigned int result2 = Sad16_C<nBlkWidth, nBlkHeight, pixel_t>(pSrc, nSrcPitch, pRef, nRefPitch);
#endif
  _mm256_zeroupper(); // diff from main sse2

  __m128i zero = _mm_setzero_si128();
  __m128i sum = _mm_setzero_si128(); // 2x or 4x int is probably enough for 32x32
                                     // 16 bit width=12: as 3x4, no mask needed
  __m128i mask12_8_bit = _mm_set_epi8(0, 0, 0, 0, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF);
  __m128i mask6_16bit = _mm_set_epi16(0, 0, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF);
  __m128i mask6_8bit = _mm_set_epi8(0, 0, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0, 0, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF);

  const bool two_8byte_rows = (sizeof(pixel_t) == 2 && nBlkWidth <= 4) || (sizeof(pixel_t) == 1 && nBlkWidth <= 8);
  const bool one_cycle = (sizeof(pixel_t) * nBlkWidth) > 8 && (sizeof(pixel_t) * nBlkWidth) <= 16; // 8 bit: 12,16 pixel, 16 bit: 6,8 pixel
  const bool unroll_by2 = !two_8byte_rows && nBlkHeight >= 2; // unroll by 4: slower

  for (int y = 0; y < nBlkHeight; y += (two_8byte_rows || unroll_by2) ? 2 : 1)
  {
    if (two_8byte_rows) { // no x cycle
                          // 8 bit: width<=8 (4,6,8)
                          // 16 bits: width <= 4
      __m128i src1, src2;
      // (8 bytes or 4 words) * 2 rows
      if (sizeof(pixel_t) == 1) {
        // blksize <= 8
        src1 = _mm_unpacklo_epi8(_mm_loadl_epi64((__m128i *) (pSrc)), _mm_loadl_epi64((__m128i *) (pSrc + nSrcPitch)));
        if (nBlkWidth == 6)
          src1 = _mm_and_si128(src1, mask6_8bit);
        src2 = _mm_unpacklo_epi8(_mm_loadl_epi64((__m128i *) (pRef)), _mm_loadl_epi64((__m128i *) (pRef + nRefPitch)));
      }
      else if (sizeof(pixel_t) == 2) {
        // blksize <= 4
        src1 = _mm_unpacklo_epi16(_mm_loadl_epi64((__m128i *) (pSrc)), _mm_loadl_epi64((__m128i *) (pSrc + nSrcPitch)));
        src2 = _mm_unpacklo_epi16(_mm_loadl_epi64((__m128i *) (pRef)), _mm_loadl_epi64((__m128i *) (pRef + nRefPitch)));
      }
      if (sizeof(pixel_t) == 1) {
        // this is uint_16 specific, but will test on uint8_t against external .asm SAD functions)
        sum = _mm_add_epi32(sum, _mm_sad_epu8(src1, src2)); // yihhaaa, existing SIMD   sum1_32, 0, sum2_32, 0
                                                            // result in two 32 bit areas at the upper and lower 64 bytes
      }
      else {
        __m128i greater_t = _mm_subs_epu16(src1, src2); // unsigned sub with saturation
        __m128i smaller_t = _mm_subs_epu16(src2, src1);
        __m128i absdiff = _mm_or_si128(greater_t, smaller_t); //abs(s1-s2)  == (satsub(s1,s2) | satsub(s2,s1))
                                                              // 8 x uint16 absolute differences
        sum = _mm_add_epi32(sum, _mm_unpacklo_epi16(absdiff, zero));
        sum = _mm_add_epi32(sum, _mm_unpackhi_epi16(absdiff, zero));
        // sum1_32, sum2_32, sum3_32, sum4_32
      }
    }
    else if (one_cycle)
    {
      // 8 bit: 8 < blksize <= 16
      // 16 bit: 4 < blksize <= 8
      __m128i src1, src2;
      src1 = _mm_load_si128((__m128i *) (pSrc)); // no x
      src2 = _mm_loadu_si128((__m128i *) (pRef));
      if (sizeof(pixel_t) == 1) {
        if (nBlkWidth == 12) {
          src1 = _mm_and_si128(src1, mask12_8_bit);
          src2 = _mm_and_si128(src2, mask12_8_bit);
        }
        // this is uint_16 specific, but will test on uint8_t against external .asm SAD functions)
        sum = _mm_add_epi32(sum, _mm_sad_epu8(src1, src2)); // yihhaaa, existing SIMD   sum1_32, 0, sum2_32, 0
                                                            // result in two 32 bit areas at the upper and lower 64 bytes
      }
      else {
        if (nBlkWidth == 6) {
          src1 = _mm_and_si128(src1, mask6_16bit);
          src2 = _mm_and_si128(src2, mask6_16bit);
        }
        __m128i greater_t = _mm_subs_epu16(src1, src2); // unsigned sub with saturation
        __m128i smaller_t = _mm_subs_epu16(src2, src1);
        __m128i absdiff = _mm_or_si128(greater_t, smaller_t); //abs(s1-s2)  == (satsub(s1,s2) | satsub(s2,s1))
                                                              // 8 x uint16 absolute differences
        sum = _mm_add_epi32(sum, _mm_unpacklo_epi16(absdiff, zero));
        sum = _mm_add_epi32(sum, _mm_unpackhi_epi16(absdiff, zero));
        // sum1_32, sum2_32, sum3_32, sum4_32
      }
      if (unroll_by2) {
        // unroll#2
        src1 = _mm_load_si128((__m128i *) (pSrc + nSrcPitch)); // no x
        src2 = _mm_loadu_si128((__m128i *) (pRef + nRefPitch));
        if (sizeof(pixel_t) == 1) {
          if (nBlkWidth == 12) {
            src1 = _mm_and_si128(src1, mask12_8_bit);
            src2 = _mm_and_si128(src2, mask12_8_bit);
          }
          // this is uint_16 specific, but will test on uint8_t against external .asm SAD functions)
          sum = _mm_add_epi32(sum, _mm_sad_epu8(src1, src2)); // yihhaaa, existing SIMD   sum1_32, 0, sum2_32, 0
                                                              // result in two 32 bit areas at the upper and lower 64 bytes
        }
        else {
          if (nBlkWidth == 6) {
            src1 = _mm_and_si128(src1, mask6_16bit);
            src2 = _mm_and_si128(src2, mask6_16bit);
          }
          __m128i greater_t = _mm_subs_epu16(src1, src2); // unsigned sub with saturation
          __m128i smaller_t = _mm_subs_epu16(src2, src1);
          __m128i absdiff = _mm_or_si128(greater_t, smaller_t); //abs(s1-s2)  == (satsub(s1,s2) | satsub(s2,s1))
                                                                // 8 x uint16 absolute differences
          sum = _mm_add_epi32(sum, _mm_unpacklo_epi16(absdiff, zero));
          sum = _mm_add_epi32(sum, _mm_unpackhi_epi16(absdiff, zero));
          // sum1_32, sum2_32, sum3_32, sum4_32
        }
      }
    }
    else if ((nBlkWidth * sizeof(pixel_t)) % 16 == 0) {
      for (int x = 0; x < nBlkWidth * sizeof(pixel_t); x += 16)
      {
        __m128i src1, src2;
        src1 = _mm_load_si128((__m128i *) (pSrc + x));
        src2 = _mm_loadu_si128((__m128i *) (pRef + x));
        if (sizeof(pixel_t) == 1) {
          // this is uint_16 specific, but will test on uint8_t against external .asm SAD functions)
          sum = _mm_add_epi32(sum, _mm_sad_epu8(src1, src2)); // yihhaaa, existing SIMD   sum1_32, 0, sum2_32, 0
                                                              // result in two 32 bit areas at the upper and lower 64 bytes
        }
        else {
          __m128i greater_t = _mm_subs_epu16(src1, src2); // unsigned sub with saturation
          __m128i smaller_t = _mm_subs_epu16(src2, src1);
          __m128i absdiff = _mm_or_si128(greater_t, smaller_t); //abs(s1-s2)  == (satsub(s1,s2) | satsub(s2,s1))
                                                                // 8 x uint16 absolute differences
          sum = _mm_add_epi32(sum, _mm_unpacklo_epi16(absdiff, zero));
          sum = _mm_add_epi32(sum, _mm_unpackhi_epi16(absdiff, zero));
          // sum1_32, sum2_32, sum3_32, sum4_32
        }
        // sum += SADABS(reinterpret_cast<const pixel_t *>(pSrc)[x] - reinterpret_cast<const pixel_t *>(pRef)[x]);
        if (unroll_by2)
        {
          // unroll#2
          src1 = _mm_load_si128((__m128i *) (pSrc + nSrcPitch + x));
          src2 = _mm_loadu_si128((__m128i *) (pRef + nRefPitch + x));
          if (sizeof(pixel_t) == 1) {
            // this is uint_16 specific, but will test on uint8_t against external .asm SAD functions)
            sum = _mm_add_epi32(sum, _mm_sad_epu8(src1, src2)); // yihhaaa, existing SIMD   sum1_32, 0, sum2_32, 0
                                                                // result in two 32 bit areas at the upper and lower 64 bytes
          }
          else {
            __m128i greater_t = _mm_subs_epu16(src1, src2); // unsigned sub with saturation
            __m128i smaller_t = _mm_subs_epu16(src2, src1);
            __m128i absdiff = _mm_or_si128(greater_t, smaller_t); //abs(s1-s2)  == (satsub(s1,s2) | satsub(s2,s1))
                                                                  // 8 x uint16 absolute differences
            sum = _mm_add_epi32(sum, _mm_unpacklo_epi16(absdiff, zero));
            sum = _mm_add_epi32(sum, _mm_unpackhi_epi16(absdiff, zero));
            // sum1_32, sum2_32, sum3_32, sum4_32
          }
        }
      }
    }
    else if ((nBlkWidth * sizeof(pixel_t)) % 8 == 0) {
      // e.g. 12x16 uint16 -> 3x4
      for (int x = 0; x < nBlkWidth * sizeof(pixel_t); x += 8)
      {
        __m128i src1, src2;
        src1 = _mm_loadl_epi64((__m128i *) (pSrc + x));
        src2 = _mm_loadl_epi64((__m128i *) (pRef + x));
        if (sizeof(pixel_t) == 1) {
          // this is uint_16 specific, but will test on uint8_t against external .asm SAD functions)
          sum = _mm_add_epi32(sum, _mm_sad_epu8(src1, src2)); // yihhaaa, existing SIMD   sum1_32, 0, sum2_32, 0
                                                              // result in two 32 bit areas at the upper and lower 64 bytes
        }
        else {
          __m128i greater_t = _mm_subs_epu16(src1, src2); // unsigned sub with saturation
          __m128i smaller_t = _mm_subs_epu16(src2, src1);
          __m128i absdiff = _mm_or_si128(greater_t, smaller_t); //abs(s1-s2)  == (satsub(s1,s2) | satsub(s2,s1))
                                                                // 8 x uint16 absolute differences
          sum = _mm_add_epi32(sum, _mm_unpacklo_epi16(absdiff, zero));
          // sum1_32, sum2_32, sum3_32, sum4_32
        }
        // sum += SADABS(reinterpret_cast<const pixel_t *>(pSrc)[x] - reinterpret_cast<const pixel_t *>(pRef)[x]);
        if (unroll_by2)
        {
          // unroll#2
          src1 = _mm_loadl_epi64((__m128i *) (pSrc + nSrcPitch + x));
          src2 = _mm_loadl_epi64((__m128i *) (pRef + nRefPitch + x));
          if (sizeof(pixel_t) == 1) {
            sum = _mm_add_epi32(sum, _mm_sad_epu8(src1, src2)); // yihhaaa, existing SIMD   sum1_32, 0, sum2_32, 0
                                                                // result in two 32 bit areas at the upper and lower 64 bytes
          }
          else {
            __m128i greater_t = _mm_subs_epu16(src1, src2); // unsigned sub with saturation
            __m128i smaller_t = _mm_subs_epu16(src2, src1);
            __m128i absdiff = _mm_or_si128(greater_t, smaller_t); //abs(s1-s2)  == (satsub(s1,s2) | satsub(s2,s1))
                                                                  // 8 x uint16 absolute differences
            sum = _mm_add_epi32(sum, _mm_unpacklo_epi16(absdiff, zero));
            // sum1_32, sum2_32, sum3_32, sum4_32
          }
        }
      }
    }
    else {
      assert(0);
    }
    if (two_8byte_rows || unroll_by2) {
      pSrc += nSrcPitch * 2;
      pRef += nRefPitch * 2;
    }
    else {
      pSrc += nSrcPitch;
      pRef += nRefPitch;
    }
  }
  /*
  [Low64, Hi64]
  _mm_unpacklo_epi64(_mm_setzero_si128(), x)  [0, x0]
  _mm_unpackhi_epi64(_mm_setzero_si128(), x)  [0, x1]
  _mm_move_epi64(x)                           [x0, 0]
  _mm_unpackhi_epi64(x, _mm_setzero_si128())  [x1, 0]
  */
  if (sizeof(pixel_t) == 2) {
    // at 16 bits: we have 4 integers for sum: a0 a1 a2 a3
    __m128i a0_a1 = _mm_unpacklo_epi32(sum, zero); // a0 0 a1 0
    __m128i a2_a3 = _mm_unpackhi_epi32(sum, zero); // a2 0 a3 0
    sum = _mm_add_epi32(a0_a1, a2_a3); // a0+a2, 0, a1+a3, 0
                                       // hadd: shower
  }
  // sum here: two 32 bit partial result: sum1 0 sum2 0
  __m128i sum_hi = _mm_unpackhi_epi64(sum, zero); // a1 + a3. 2 dwords right 
  sum = _mm_add_epi32(sum, sum_hi);  // a0 + a2 + a1 + a3

  unsigned int result = _mm_cvtsi128_si32(sum);

#if 0
  // check result against C
  if (result != result2) {
    result = result2;
  }
#endif

  _mm256_zeroupper(); // diff from main sse2, paranoia, bacause here we have no ymm regs

  return result;
}  // end of SSE2 with AVX commandset Sad16


