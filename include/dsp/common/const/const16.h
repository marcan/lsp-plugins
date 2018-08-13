/*
 * const16.h
 *
 *  Created on: 13 авг. 2018 г.
 *      Author: sadko
 */

#ifndef DSP_F32VEC4
    #error "This header should not be included directly"
#endif /* DSP_F32VEC4 */

namespace dsp
{
    /* Sine approximation coefficients */
    DSP_F32VEC4(SIN_C0,  1.00000000000000000000);
    DSP_F32VEC4(SIN_C1, -0.16666666666666665741);
    DSP_F32VEC4(SIN_C2,  0.00833333333333333322);
    DSP_F32VEC4(SIN_C3, -0.00019841269841269841);
    DSP_F32VEC4(SIN_C4,  0.00000275573192239859);
    DSP_F32VEC4(SIN_C5, -0.00000002505210838544);

    /* Cosine approximation coefficients */
    DSP_F32VEC4(COS_C0,  1.00000000000000000000);
    DSP_F32VEC4(COS_C1, -0.50000000000000000000);
    DSP_F32VEC4(COS_C2,  0.04166666666666666435);
    DSP_F32VEC4(COS_C3, -0.00138888888888888894);
    DSP_F32VEC4(COS_C4,  0.00002480158730158730);
    DSP_F32VEC4(COS_C5, -0.00000027557319223986);

    /* Logarithm approximation coefficients */
    DSP_F32VEC4(LOG_C0, 7.0376836292E-2);
    DSP_F32VEC4(LOG_C1, -1.1514610310E-1);
    DSP_F32VEC4(LOG_C2, 1.1676998740E-1);
    DSP_F32VEC4(LOG_C3, -1.2420140846E-1);
    DSP_F32VEC4(LOG_C4, +1.4249322787E-1);
    DSP_F32VEC4(LOG_C5, -1.6668057665E-1);
    DSP_F32VEC4(LOG_C6, +2.0000714765E-1);
    DSP_F32VEC4(LOG_C7, -2.4999993993E-1);
    DSP_F32VEC4(LOG_C8, +3.3333331174E-1);
    DSP_F32VEC4(LOG_C9, 0.5);
    DSP_F32VEC4(LOG_LXE, -2.12194440e-4);

    /* Math constants */
    DSP_F32VEC4(ZERO, 0.0f);
    DSP_F32VEC4(ONE, 1.0f);
    DSP_F32VEC4(PI,  M_PI);
    DSP_F32VEC4(PI_2, M_PI_2);
    DSP_F32VEC4(SQRT1_2, M_SQRT1_2);
    DSP_F32VEC4(LN2, M_LN2);

    /* Sign Mask */
    DSP_U32VEC4(X_SIGN,  0x7fffffff);
    DSP_U32VEC4(X_ISIGN, 0x80000000);
    DSP_F32VEC4(X_HALF,  0.5f);
    DSP_F32VEC4(X_MINUS_ONE,  -1.0f);
    DSP_U32VEC4(X_MANT,  0x007fffff);
    DSP_U32VEC4(X_MMASK, 0x0000007f);
    DSP_F32VEC4(X_AMP_THRESH, AMPLIFICATION_THRESH);
    DSP_U32VEC4(X_P_DENORM, 0x00800000);
    DSP_U32VEC4(X_N_DENORM, 0x80800000);

    /* Positive and negative infinities */
    DSP_U32VEC4(X_P_INF,  0x7f800000);
    DSP_U32VEC4(X_N_INF,  0xff800000);
    DSP_U32VEC4(X_P_INFM1,0x7f7fffff);
    DSP_U32VEC4(X_N_INFM1,0xff7fffff);
    DSP_U32VEC4(X_ZERO_M1,0xffffffff);
    DSP_U32VEC4(X_CMASK,  0x00ff00ff);

    /* Saturation replacement */
    DSP_F32VEC4(SX_P_INF, FLOAT_SAT_P_INF);
    DSP_F32VEC4(SX_N_INF, FLOAT_SAT_N_INF);
    DSP_F32VEC4(SX_P_NAN, FLOAT_SAT_P_NAN);
    DSP_F32VEC4(SX_N_NAN, FLOAT_SAT_N_NAN);

    /* Miscellaneous vectors */
    DSP_U32VECX4(X_MASK0001, -1, 0, 0, 0);
    DSP_U32VECX4(X_MASK0111, -1, -1, -1, 0);
    DSP_U32VECX4(X_SMASK0010, 0, 0x80000000, 0, 0);
    DSP_U32VECX4(X_SMASK0001, 0x80000000, 0, 0, 0);

    DSP_F32VECX4(X_3DPOINT, 0.0f, 0.0f, 0.0f, 1.0f);

    /* 3D */
    DSP_F32VEC4(X_3D_TOLERANCE, DSP_3D_TOLERANCE);
}
