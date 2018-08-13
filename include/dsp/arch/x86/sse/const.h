/*
 * const.h
 *
 *  Created on: 05 окт. 2015 г.
 *      Author: sadko
 */

#ifndef DSP_ARCH_X86_DSP_CONST_H_
#define DSP_ARCH_X86_DSP_CONST_H_

#ifndef DSP_ARCH_X86_SSE_IMPL
    #error "This header should not be included directly"
#endif /* DSP_ARCH_X86_SSE_IMPL */

// Parameters for SSE
#define SSE_MULTIPLE                4
#define SSE_ALIGN                   (SSE_MULTIPLE * sizeof(float))
#define SFENCE                      __asm__ __volatile__ ( __ASM_EMIT("sfence") )
#define EMMS                        __asm__ __volatile__ ( __ASM_EMIT("emms") )
#define MOVNTPS                     "movaps"
#define SSE_SVEC4(name, value)      static const float name[] __lsp_aligned16       = { value, value, value, value }
#define SSE_UVEC4(name, value)      static const uint32_t name[] __lsp_aligned16    = { value, value, value, value }
#define SSE_UVEC(name, a, b, c, d)  static const uint32_t name[] __lsp_aligned16    = { uint32_t(a), uint32_t(b), uint32_t(c), uint32_t(d) }
#define SSE_SVEC(name, a, b, c, d)  static const float name[] __lsp_aligned16       = { a, b, c, d }
#define SSE_X4VEC(x)                x, x, x, x

static const float XFFT_W_RE[] __lsp_aligned16 =
{
    SSE_X4VEC(0.0000000000000000f),
    SSE_X4VEC(0.0000000000000000f),
    SSE_X4VEC(0.7071067811865475f),
    SSE_X4VEC(0.9238795325112868f),
    SSE_X4VEC(0.9807852804032305f),
    SSE_X4VEC(0.9951847266721969f),
    SSE_X4VEC(0.9987954562051724f),
    SSE_X4VEC(0.9996988186962042f),
    SSE_X4VEC(0.9999247018391445f),
    SSE_X4VEC(0.9999811752826011f),
    SSE_X4VEC(0.9999952938095762f),
    SSE_X4VEC(0.9999988234517019f),
    SSE_X4VEC(0.9999997058628822f),
    SSE_X4VEC(0.9999999264657179f),
    SSE_X4VEC(0.9999999816164293f),
    SSE_X4VEC(0.9999999954041073f),
    SSE_X4VEC(0.9999999988510268f)
};

static const float XFFT_W_IM[] __lsp_aligned16 =
{
    SSE_X4VEC(1.0000000000000000f),
    SSE_X4VEC(1.0000000000000000f),
    SSE_X4VEC(0.7071067811865475f),
    SSE_X4VEC(0.3826834323650898f),
    SSE_X4VEC(0.1950903220161283f),
    SSE_X4VEC(0.0980171403295606f),
    SSE_X4VEC(0.0490676743274180f),
    SSE_X4VEC(0.0245412285229123f),
    SSE_X4VEC(0.0122715382857199f),
    SSE_X4VEC(0.0061358846491545f),
    SSE_X4VEC(0.0030679567629660f),
    SSE_X4VEC(0.0015339801862848f),
    SSE_X4VEC(0.0007669903187427f),
    SSE_X4VEC(0.0003834951875714f),
    SSE_X4VEC(0.0001917475973107f),
    SSE_X4VEC(0.0000958737990960f),
    SSE_X4VEC(0.0000479368996031f)
};

static const float XFFT_W[] __lsp_aligned16 =
{
    SSE_X4VEC(0.0000000000000000f), SSE_X4VEC(1.0000000000000000f),
    SSE_X4VEC(0.0000000000000000f), SSE_X4VEC(1.0000000000000000f),
    SSE_X4VEC(0.7071067811865475f), SSE_X4VEC(0.7071067811865475f),
    SSE_X4VEC(0.9238795325112868f), SSE_X4VEC(0.3826834323650898f),
    SSE_X4VEC(0.9807852804032305f), SSE_X4VEC(0.1950903220161283f),
    SSE_X4VEC(0.9951847266721969f), SSE_X4VEC(0.0980171403295606f),
    SSE_X4VEC(0.9987954562051724f), SSE_X4VEC(0.0490676743274180f),
    SSE_X4VEC(0.9996988186962042f), SSE_X4VEC(0.0245412285229123f),
    SSE_X4VEC(0.9999247018391445f), SSE_X4VEC(0.0122715382857199f),
    SSE_X4VEC(0.9999811752826011f), SSE_X4VEC(0.0061358846491545f),
    SSE_X4VEC(0.9999952938095762f), SSE_X4VEC(0.0030679567629660f),
    SSE_X4VEC(0.9999988234517019f), SSE_X4VEC(0.0015339801862848f),
    SSE_X4VEC(0.9999997058628822f), SSE_X4VEC(0.0007669903187427f),
    SSE_X4VEC(0.9999999264657179f), SSE_X4VEC(0.0003834951875714f),
    SSE_X4VEC(0.9999999816164293f), SSE_X4VEC(0.0001917475973107f),
    SSE_X4VEC(0.9999999954041073f), SSE_X4VEC(0.0000958737990960f),
    SSE_X4VEC(0.9999999988510268f), SSE_X4VEC(0.0000479368996031f)
};

static const float XFFT_A_RE[] __lsp_aligned16 =
{
    1.0000000000000000f, 0.7071067811865475f, 0.0000000000000001f, -0.7071067811865475f,
    1.0000000000000000f, 0.9238795325112868f, 0.7071067811865475f, 0.3826834323650898f,
    1.0000000000000000f, 0.9807852804032305f, 0.9238795325112868f, 0.8314696123025452f,
    1.0000000000000000f, 0.9951847266721969f, 0.9807852804032305f, 0.9569403357322089f,
    1.0000000000000000f, 0.9987954562051724f, 0.9951847266721969f, 0.9891765099647810f,
    1.0000000000000000f, 0.9996988186962042f, 0.9987954562051724f, 0.9972904566786902f,
    1.0000000000000000f, 0.9999247018391445f, 0.9996988186962042f, 0.9993223845883495f,
    1.0000000000000000f, 0.9999811752826011f, 0.9999247018391445f, 0.9998305817958234f,
    1.0000000000000000f, 0.9999952938095762f, 0.9999811752826011f, 0.9999576445519639f,
    1.0000000000000000f, 0.9999988234517019f, 0.9999952938095762f, 0.9999894110819284f,
    1.0000000000000000f, 0.9999997058628822f, 0.9999988234517019f, 0.9999973527669782f,
    1.0000000000000000f, 0.9999999264657179f, 0.9999997058628822f, 0.9999993381915255f,
    1.0000000000000000f, 0.9999999816164293f, 0.9999999264657179f, 0.9999998345478677f,
    1.0000000000000000f, 0.9999999954041073f, 0.9999999816164293f, 0.9999999586369661f,
    1.0000000000000000f, 0.9999999988510268f, 0.9999999954041073f, 0.9999999896592415f
};

static const float XFFT_A_IM[] __lsp_aligned16 =
{
    0.0000000000000000f, 0.7071067811865475f, 1.0000000000000000f, 0.7071067811865476f,
    0.0000000000000000f, 0.3826834323650898f, 0.7071067811865475f, 0.9238795325112867f,
    0.0000000000000000f, 0.1950903220161283f, 0.3826834323650898f, 0.5555702330196022f,
    0.0000000000000000f, 0.0980171403295606f, 0.1950903220161283f, 0.2902846772544624f,
    0.0000000000000000f, 0.0490676743274180f, 0.0980171403295606f, 0.1467304744553617f,
    0.0000000000000000f, 0.0245412285229123f, 0.0490676743274180f, 0.0735645635996674f,
    0.0000000000000000f, 0.0122715382857199f, 0.0245412285229123f, 0.0368072229413588f,
    0.0000000000000000f, 0.0061358846491545f, 0.0122715382857199f, 0.0184067299058048f,
    0.0000000000000000f, 0.0030679567629660f, 0.0061358846491545f, 0.0092037547820598f,
    0.0000000000000000f, 0.0015339801862848f, 0.0030679567629660f, 0.0046019261204486f,
    0.0000000000000000f, 0.0007669903187427f, 0.0015339801862848f, 0.0023009691514258f,
    0.0000000000000000f, 0.0003834951875714f, 0.0007669903187427f, 0.0011504853371138f,
    0.0000000000000000f, 0.0001917475973107f, 0.0003834951875714f, 0.0005752427637321f,
    0.0000000000000000f, 0.0000958737990960f, 0.0001917475973107f, 0.0002876213937629f,
    0.0000000000000000f, 0.0000479368996031f, 0.0000958737990960f, 0.0001438106983686f
};

static const float XFFT_A[] __lsp_aligned16 =
{
    1.0000000000000000f, 0.7071067811865475f, 0.0000000000000001f, -0.7071067811865475f,0.0000000000000000f, 0.7071067811865475f, 1.0000000000000000f, 0.7071067811865476f,
    1.0000000000000000f, 0.9238795325112868f, 0.7071067811865475f, 0.3826834323650898f, 0.0000000000000000f, 0.3826834323650898f, 0.7071067811865475f, 0.9238795325112867f,
    1.0000000000000000f, 0.9807852804032305f, 0.9238795325112868f, 0.8314696123025452f, 0.0000000000000000f, 0.1950903220161283f, 0.3826834323650898f, 0.5555702330196022f,
    1.0000000000000000f, 0.9951847266721969f, 0.9807852804032305f, 0.9569403357322089f, 0.0000000000000000f, 0.0980171403295606f, 0.1950903220161283f, 0.2902846772544624f,
    1.0000000000000000f, 0.9987954562051724f, 0.9951847266721969f, 0.9891765099647810f, 0.0000000000000000f, 0.0490676743274180f, 0.0980171403295606f, 0.1467304744553617f,
    1.0000000000000000f, 0.9996988186962042f, 0.9987954562051724f, 0.9972904566786902f, 0.0000000000000000f, 0.0245412285229123f, 0.0490676743274180f, 0.0735645635996674f,
    1.0000000000000000f, 0.9999247018391445f, 0.9996988186962042f, 0.9993223845883495f, 0.0000000000000000f, 0.0122715382857199f, 0.0245412285229123f, 0.0368072229413588f,
    1.0000000000000000f, 0.9999811752826011f, 0.9999247018391445f, 0.9998305817958234f, 0.0000000000000000f, 0.0061358846491545f, 0.0122715382857199f, 0.0184067299058048f,
    1.0000000000000000f, 0.9999952938095762f, 0.9999811752826011f, 0.9999576445519639f, 0.0000000000000000f, 0.0030679567629660f, 0.0061358846491545f, 0.0092037547820598f,
    1.0000000000000000f, 0.9999988234517019f, 0.9999952938095762f, 0.9999894110819284f, 0.0000000000000000f, 0.0015339801862848f, 0.0030679567629660f, 0.0046019261204486f,
    1.0000000000000000f, 0.9999997058628822f, 0.9999988234517019f, 0.9999973527669782f, 0.0000000000000000f, 0.0007669903187427f, 0.0015339801862848f, 0.0023009691514258f,
    1.0000000000000000f, 0.9999999264657179f, 0.9999997058628822f, 0.9999993381915255f, 0.0000000000000000f, 0.0003834951875714f, 0.0007669903187427f, 0.0011504853371138f,
    1.0000000000000000f, 0.9999999816164293f, 0.9999999264657179f, 0.9999998345478677f, 0.0000000000000000f, 0.0001917475973107f, 0.0003834951875714f, 0.0005752427637321f,
    1.0000000000000000f, 0.9999999954041073f, 0.9999999816164293f, 0.9999999586369661f, 0.0000000000000000f, 0.0000958737990960f, 0.0001917475973107f, 0.0002876213937629f,
    1.0000000000000000f, 0.9999999988510268f, 0.9999999954041073f, 0.9999999896592415f, 0.0000000000000000f, 0.0000479368996031f, 0.0000958737990960f, 0.0001438106983686f
};


static inline bool __lsp_forced_inline sse_aligned(const void *ptr)         { return !(ptrdiff_t(ptr) & (SSE_ALIGN - 1));  };
static inline ptrdiff_t __lsp_forced_inline sse_offset(const void *ptr)     { return (ptrdiff_t(ptr) & (SSE_ALIGN - 1));   };
static inline size_t __lsp_forced_inline sse_multiple(size_t count)         { return count & (SSE_MULTIPLE - 1);           };

#endif /* DSP_ARCH_X86_DSP_CONST_H_ */
