/*
 * static.h
 *
 *  Created on: 27 янв. 2020 г.
 *      Author: sadko
 */

#ifndef DSP_ARCH_AARCH64_ASIMD_FILTERS_STATIC_H_
#define DSP_ARCH_AARCH64_ASIMD_FILTERS_STATIC_H_

#ifndef DSP_ARCH_AARCH64_ASIMD_IMPL
    #error "This header should not be included directly"
#endif /* DSP_ARCH_AARCH64_ASIMD_IMPL */

namespace asimd
{
    void biquad_process_x1(float *dst, const float *src, size_t count, biquad_t *f)
    {
        // s'    = a0*s + d0;
        // d0'   = d1 + a1*s + b1*s';
        // d1'   = a2*s + b2*s';
        ARCH_AARCH64_ASM(
            // Check count
            __ASM_EMIT("cbz             %[count], 6f")
            __ASM_EMIT("ldp             s16, s17, [%[FX1], #0x00]")             // v16  = a0, v17 = a0
            __ASM_EMIT("ldp             s18, s19, [%[FX1], #0x08]")             // v18  = a1, v19 = a2
            __ASM_EMIT("ldp             s20, s21, [%[FX1], #0x10]")             // v20  = b1, v21 = b2
            __ASM_EMIT("ldp             s22, s23, [%[FD]]")                     // v22  = d0, v23 = d1
            // x2 blocks
            __ASM_EMIT("subs            %[count], %[count], #2")
            __ASM_EMIT("b.lt            2f")
            __ASM_EMIT("1:")
            __ASM_EMIT("ldp             s0, s1, [%[src]]")                      // v0   = s0, v1 = s1
            __ASM_EMIT("fmadd           s2, s16, s0, s22")                      // v2   = s' = a0*s0+d0
            __ASM_EMIT("fmul            s4, s18, s0")                           // v4   = a1*s0
            __ASM_EMIT("fmadd           s6, s20, s2, s23")                      // v6   = b1*s' + d1
            __ASM_EMIT("fmul            s0, s19, s0")                           // v0   = a2*s0
            __ASM_EMIT("fadd            s22, s4, s6")                           // v22  = d0' = d1 + a1*s0 + b1*s'
            __ASM_EMIT("fmadd           s23, s21, s2, s0")                      // v23  = d1' = a2*s0 + b2*s'
            __ASM_EMIT("fmadd           s3, s16, s1, s22")                      // v3   = s' = a0*s1+d0
            __ASM_EMIT("fmul            s5, s18, s1")                           // v5   = a1*s1
            __ASM_EMIT("fmadd           s7, s20, s3, s23")                      // v7   = b1*s3 + d1
            __ASM_EMIT("fmul            s1, s19, s1")                           // v1   = a2*s1
            __ASM_EMIT("fadd            s22, s5, s7")                           // v22  = d0' = d1 + a1*s1 + b1*s3
            __ASM_EMIT("fmadd           s23, s21, s3, s1")                      // v23  = d1' = a2*s1 + b2*s0
            __ASM_EMIT("subs            %[count], %[count], #2")
            __ASM_EMIT("stp             s2, s3, [%[dst]]")
            __ASM_EMIT("add             %[src], %[src], #0x08")
            __ASM_EMIT("add             %[dst], %[dst], #0x08")
            __ASM_EMIT("b.ge            1b")
            __ASM_EMIT("2:")
            // X1 block:
            __ASM_EMIT("adds            %[count], %[count], #1")
            __ASM_EMIT("b.lt            4f")
            __ASM_EMIT("ldr             s0, [%[src]]")                          // v0   = s0
            __ASM_EMIT("fmadd           s2, s16, s0, s22")                      // v2   = s' = a0*s0+d0
            __ASM_EMIT("fmul            s4, s18, s0")                           // v4   = a1*s0
            __ASM_EMIT("fmadd           s6, s20, s2, s23")                      // v6   = b1*s' + d1
            __ASM_EMIT("fmul            s0, s19, s0")                           // v0   = a2*s0
            __ASM_EMIT("fadd            s22, s4, s6")                           // v22  = d0' = d1 + a1*s0 + b1*s'
            __ASM_EMIT("fmadd           s23, s21, s2, s0")                      // v23  = d1' = a2*s0 + b2*s'
            __ASM_EMIT("str             s2, [%[dst]]")
            __ASM_EMIT("4:")
            // Store the updated buffer state
            __ASM_EMIT("stp             s22, s23, [%[FD]]")
            __ASM_EMIT("6:")

            : [dst] "+r" (dst), [src] "+r" (src), [count] "+r" (count)
            : [FD] "r" (&f->d[0]), [FX1] "r" (&f->x1)
            : "cc", "memory",
              "q0", "q1", "q2", "q3",
              "q4", "q5", "q6", "q7",
              "q16", "q17", "q18", "q19",
              "q20", "q21", "q22", "q23"
        );
    }

    void biquad_process_x2(float *dst, const float *src, size_t count, biquad_t *f)
    {
        IF_ARCH_AARCH64(biquad_x2_t *x2 = &f->x2);

        // s'    = a0*s + d0;
        // d0'   = d1 + a1*s + b1*s';
        // d1'   = a2*s + b2*s';
        ARCH_AARCH64_ASM(
            // Check count
            __ASM_EMIT("cbz             %[count], 6f")
            __ASM_EMIT("ldp             q0, q1, [%[FD], #0x00]")                            // v0   = d0 d1 0 0, v1 = e0 e1 0 0
            __ASM_EMIT("ld4             {v16.2s, v17.2s, v18.2s, v19.2s}, [%[f]], #0x20")   // v16  = a0, v17 = a0, v18  = a1, v19 = a2
            __ASM_EMIT("ld4             {v20.2s, v21.2s, v22.2s, v23.2s}, [%[f]]")          // v20  = b1, v21 = b2, v22 = 0, v23 = 0
            __ASM_EMIT("trn1            v22.4s, v0.4s, v1.4s")                              // v22  = d0 e0 0 0
            __ASM_EMIT("trn2            v23.4s, v0.4s, v1.4s")                              // v23  = d1 e1 0 0
            // x1 head block
            __ASM_EMIT("ld1             {v0.s}[0], [%[src]]")                   // v0   = s0
            __ASM_EMIT("fmul            v1.2s, v16.2s, v0.2s")                  // v1   = a0*s0
            __ASM_EMIT("fadd            v4.2s, v22.2s, v1.2s")                  // v4   = s' = d0+a0*s0
            __ASM_EMIT("fmul            v2.2s, v18.2s, v0.2s")                  // v2   = a1*s0
            __ASM_EMIT("fadd            v5.2s, v23.2s, v2.2s")                  // v5   = d1+a1*s0
            __ASM_EMIT("fmul            v6.2s, v19.2s, v0.2s")                  // v6   = a2*s0
            __ASM_EMIT("fmla            v5.2s, v20.2s, v4.2s")                  // v5   = d0' = d1+a1*s0+b1*s'
            __ASM_EMIT("fmla            v6.2s, v21.2s, v4.2s")                  // v6   = d1' = a2*s0+b2*s'
            __ASM_EMIT("add             %[src], %[src], #0x04")
            __ASM_EMIT("mov             v0.s[1], v4.s[0]")                      // shift
            __ASM_EMIT("mov             v22.s[0], v5.s[0]")                     // update d0
            __ASM_EMIT("mov             v23.s[0], v6.s[0]")                     // update d1
            // x2 blocks
            __ASM_EMIT("subs            %[count], %[count], #1")
            __ASM_EMIT("b.ls            2f")
            __ASM_EMIT("1:")
            __ASM_EMIT("ld1             {v0.s}[0], [%[src]]")                   // v0   = s0 j0
            __ASM_EMIT("fmul            v1.2s, v16.2s, v0.2s")                  // v1   = a0*s0 a0*j0
            __ASM_EMIT("fadd            v4.2s, v22.2s, v1.2s")                  // v4   = s' j' = d0+a0*s0 e0+a0*j0
            __ASM_EMIT("fmul            v2.2s, v18.2s, v0.2s")                  // v2   = a1*s0 a1*j0
            __ASM_EMIT("fadd            v22.2s, v23.2s, v2.2s")                 // v22  = d1+a1*s0 e1+a1*j0
            __ASM_EMIT("st1             {v4.s}[1], [%[dst]]")
            __ASM_EMIT("fmul            v23.2s, v19.2s, v0.2s")                 // v23  = a2*s0 a2*j0
            __ASM_EMIT("fmla            v22.2s, v20.2s, v4.2s")                 // v22  = d0' e0' = d1+a1*s0+b1*s' e1+a1*j0+b1*j'
            __ASM_EMIT("fmla            v23.2s, v21.2s, v4.2s")                 // v23  = d1' e1' = a2*s0+b2*s' a2*j0 b2*j'
            __ASM_EMIT("mov             v0.s[1], v4.s[0]")                      // shift
            __ASM_EMIT("subs            %[count], %[count], #1")
            __ASM_EMIT("add             %[src], %[src], #0x04")
            __ASM_EMIT("add             %[dst], %[dst], #0x04")
            __ASM_EMIT("b.hi            1b")
            __ASM_EMIT("2:")
            // x1 tail block:
            __ASM_EMIT("trn1            v6.4s, v22.4s, v23.4s")                 // v22  = d0 d1 0 0
            __ASM_EMIT("fmul            v1.2s, v16.2s, v0.2s")                  // v1   = a0*j0
            __ASM_EMIT("str             d6, [%[FD], #0x00]")
            __ASM_EMIT("fadd            v4.2s, v22.2s, v1.2s")                  // v4   = s' = e0 + a0*j0
            __ASM_EMIT("fmul            v2.2s, v18.2s, v0.2s")                  // v2   = a1*j0
            __ASM_EMIT("fadd            v22.2s, v23.2s, v2.2s")                 // v22  = e1 + a1*j0
            __ASM_EMIT("st1             {v4.s}[1], [%[dst]]")
            __ASM_EMIT("fmul            v23.2s, v19.2s, v0.2s")                 // v23  = a2*j0
            __ASM_EMIT("fmla            v22.2s, v20.2s, v4.2s")                 // v22  = d0' = e1 + a1*j0 + b1*j'
            __ASM_EMIT("fmla            v23.2s, v21.2s, v4.2s")                 // v23  = d1' = a2*j0 + b2*j'
            __ASM_EMIT("trn2            v6.4s, v22.4s, v23.4s")                 // v22  = e0 e1 0 0
            __ASM_EMIT("str             d6, [%[FD], #0x10]")
            __ASM_EMIT("6:")

            : [dst] "+r" (dst), [src] "+r" (src), [count] "+r" (count),
              [f] "+r" (x2)
            : [FD] "r" (&f->d[0])
            : "cc", "memory",
              "q0", "q1", "q2", "q3",
              "q4", "q5", "q6", "q7",
              "q16", "q17", "q18", "q19",
              "q20", "q21", "q22", "q23"
        );
    }

    static const uint32_t biquad_x4_mask[8] __lsp_aligned16 =
    {
        0x00000000, 0x00000000, 0x00000000, 0x00000000,
        0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff
    };

    void biquad_process_x4(float *dst, const float *src, size_t count, biquad_t *f)
    {
        IF_ARCH_AARCH64(
            biquad_x4_t *fx4 = &f->x4;
            size_t mask;
        );

        ARCH_AARCH64_ASM
        (
            __ASM_EMIT("cbz         %[count], 10f")
            // Prepare
            __ASM_EMIT("ldp         q16, q17, [%[FD]]")                     // q16  = d0, q17 = d1
            __ASM_EMIT("ldp         q18, q19, [%[FX4], #0x00]")             // q18  = a0, q19 = a1
            __ASM_EMIT("ldp         q20, q21, [%[FX4], #0x20]")             // q20  = a1, q21 = b1
            __ASM_EMIT("ldr         q22, [%[FX4], #0x40]")                  // q22  = b2
            __ASM_EMIT("ldp         q24, q25, [%[X_MASK]]")                 // q24  = vmask, q25 = 1
            __ASM_EMIT("mov         %[mask], #1")                           // mask = 0

            // Do pre-loop
            __ASM_EMIT("1:")
            __ASM_EMIT("orr         %[mask], %[mask], %[mask], LSL #1")     // mask = (mask << 1) | 1
            __ASM_EMIT("ld1         {v0.s}[0], [%[src]]")                   // v0   = s
            __ASM_EMIT("ext         v24.16b, v25.16b, v24.16b, #12")        // v24  = (vmask << 1) | 1
            __ASM_EMIT("fmul        v1.4s, v18.4s, v0.4s")                  // v1   = a0*s
            __ASM_EMIT("fmul        v2.4s, v19.4s, v0.4s")                  // v2   = a1*s
            __ASM_EMIT("fmul        v3.4s, v20.4s, v0.4s")                  // v3   = a2*s
            __ASM_EMIT("fadd        v0.4s, v1.4s, v16.4s")                  // v0   = a0*s + d0 = s'
            __ASM_EMIT("fmla        v2.4s, v21.4s, v0.4s")                  // v2   = a1*s + b1*s'
            __ASM_EMIT("fmla        v3.4s, v22.4s, v0.4s")                  // v3   = d1' = a2*s + b2*s'
            __ASM_EMIT("fadd        v2.4s, v2.4s, v17.4s")                  // v2   = d0' = d1 + a1*s + b1*s'
            __ASM_EMIT("bit         v17.16b, v3.16b, v24.16b")              // q9   = (d1 & ~vmask) | (d1' & vmask)
            __ASM_EMIT("bit         v16.16b, v2.16b, v24.16b")              // q8   = (d0 & ~vmask) | (d0' & vmask)
            __ASM_EMIT("ext         v0.16b, v0.16b, v0.16b, #12")           // v0   = s' = s[3] s[0] s[1] s[2]
            __ASM_EMIT("subs        %[count], %[count], #1")
            __ASM_EMIT("add         %[src], %[src], #0x04")
            __ASM_EMIT("b.eq        6f")
            __ASM_EMIT("cmp         %[mask], #0x0f")
            __ASM_EMIT("b.ne        1b")

            // Do main loop
            __ASM_EMIT("5:")
            __ASM_EMIT("ld1         {v0.s}[0], [%[src]]")
            __ASM_EMIT("fmul        v1.4s, v18.4s, v0.4s")                  // v1   = a0*s
            __ASM_EMIT("fmul        v2.4s, v19.4s, v0.4s")                  // v2   = a1*s
            __ASM_EMIT("fmul        v3.4s, v20.4s, v0.4s")                  // v3   = a2*s
            __ASM_EMIT("fadd        v0.4s, v1.4s, v16.4s")                  // v0   = a0*s + d0 = s'
            __ASM_EMIT("fmla        v2.4s, v21.4s, v0.4s")                  // v2   = a1*s + b1*s'
            __ASM_EMIT("fmla        v3.4s, v22.4s, v0.4s")                  // v3   = d1' = a2*s + b2*s'
            __ASM_EMIT("ext         v0.16b, v0.16b, v0.16b, #12")           // v0   = s' = s[3] s[0] s[1] s[2]
            __ASM_EMIT("fadd        v16.4s, v2.4s, v17.4s")                 // v16  = d0' = d1 + a1*s + b1*s'
            __ASM_EMIT("mov         v17.16b, v3.16b")                       // v17  = d1'
            __ASM_EMIT("st1         {v0.s}[0], [%[dst]]")
            __ASM_EMIT("subs        %[count], %[count], #1")
            __ASM_EMIT("add         %[src], %[src], #0x04")
            __ASM_EMIT("add         %[dst], %[dst], #0x04")
            __ASM_EMIT("b.ne        5b")

            // Do post-loop
            __ASM_EMIT("6:")
            __ASM_EMIT("eor         %[mask], %[mask], #1")                  // reset bit
            __ASM_EMIT("eor         v25.16b, v25.16b, v25.16b")             // v25  = 0
            __ASM_EMIT("7:")
            __ASM_EMIT("lsl         %[mask], %[mask], #1")                  // mask  = mask << 1
            __ASM_EMIT("ext         v24.16b, v25.16b, v24.16b, #12")        // v24  = (vmask << 1) | 0
            __ASM_EMIT("fmul        v1.4s, v18.4s, v0.4s")                  // v1   = a0*s
            __ASM_EMIT("fmul        v2.4s, v19.4s, v0.4s")                  // v2   = a1*s
            __ASM_EMIT("fmul        v3.4s, v20.4s, v0.4s")                  // v3   = a2*s
            __ASM_EMIT("fadd        v0.4s, v1.4s, v16.4s")                  // v0   = a0*s + d0 = s'
            __ASM_EMIT("fmla        v2.4s, v21.4s, v0.4s")                  // v2   = a1*s + b1*s'
            __ASM_EMIT("fmla        v3.4s, v22.4s, v0.4s")                  // v3   = d1' = a2*s + b2*s'
            __ASM_EMIT("fadd        v2.4s, v2.4s, v17.4s")                  // v2   = d0' = d1 + a1*s + b1*s'
            __ASM_EMIT("bit         v17.16b, v3.16b, v24.16b")              // q9   = (d1 & ~vmask) | (d1' & vmask)
            __ASM_EMIT("bit         v16.16b, v2.16b, v24.16b")              // q8   = (d0 & ~vmask) | (d0' & vmask)
            __ASM_EMIT("ext         v0.16b, v0.16b, v0.16b, #12")           // v0   = s' = s[3] s[0] s[1] s[2]
            __ASM_EMIT("tst         %[mask], #0x10")                        // Need to emit?
            __ASM_EMIT("b.eq        8f")
            __ASM_EMIT("st1         {v0.s}[0], [%[dst]]")
            __ASM_EMIT("add         %[dst], %[dst], #0x04")
            __ASM_EMIT("8:")
            __ASM_EMIT("tst         %[mask], #0x0e")
            __ASM_EMIT("b.ne        7b")

            // Store memory
            __ASM_EMIT("stp         q16, q17, [%[FD]]")
            __ASM_EMIT("10:")

            : [dst] "+r" (dst), [src] "+r" (src), [count] "+r" (count),
              [mask] "=&r" (mask)
            : [FD] "r" (&f->d[0]), [FX4] "r" (fx4),
              [X_MASK] "r" (&biquad_x4_mask[0])
            : "cc", "memory",
              "q0", "q1", "q2", "q3",
              "q4", "q5", "q6", "q7",
              "q16", "q17", "q18", "q19",
              "q20", "q21", "q22", "q23",
              "q24", "q25"
        );
    }
}

#endif /* DSP_ARCH_AARCH64_ASIMD_FILTERS_STATIC_H_ */
