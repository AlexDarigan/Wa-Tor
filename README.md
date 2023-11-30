# Concurrent Wa-Tor Project

Author: David Darigan
LICENCE: GPLV3.0

## Purpose

Apply concurrency to the [Wa-Tor]() simulation and research speed up gains.

![](data/AverageTimePerThread.svg)

## Install

__Prerequisites__

- [GCC](https://gcc.gnu.org/)
- [make](https://www.gnu.org/software/make/)
- [SFML](https://www.sfml-dev.org/)
- [OpenMP](https://www.openmp.org/)


1. Clone this repository
2. Navigate to this repository
3. Run 'make' to compile
4. Run ./watorApp 

## Device

The following is information about the device this project was written, compiled and tested on.

### CPU Information

    Architecture:            x86_64
    CPU op-mode(s):        32-bit, 64-bit
    Address sizes:         39 bits physical, 48 bits virtual
    Byte Order:            Little Endian
    CPU(s):                  8
    On-line CPU(s) list:   0-7
    Vendor ID:               GenuineIntel
    Model name:            11th Gen Intel(R) Core(TM) i5-1155G7 @ 2.50GHz
        CPU family:          6
        Model:               140
        Thread(s) per core:  2
        Core(s) per socket:  4
        Socket(s):           1
        Stepping:            2
        CPU max MHz:         4500.0000
        CPU min MHz:         400.0000
        BogoMIPS:            4992.00
        Flags:               fpu vme de pse tsc msr pae mce cx8 apic sep mtrr pge mc
                            a cmov pat pse36 clflush dts acpi mmx fxsr sse sse2 ss 
                            ht tm pbe syscall nx pdpe1gb rdtscp lm constant_tsc art
                            arch_perfmon pebs bts rep_good nopl xtopology nonstop_
                            tsc cpuid aperfmperf tsc_known_freq pni pclmulqdq dtes6
                            4 monitor ds_cpl vmx est tm2 ssse3 sdbg fma cx16 xtpr p
                            dcm pcid sse4_1 sse4_2 x2apic movbe popcnt tsc_deadline
                            _timer aes xsave avx f16c rdrand lahf_lm abm 3dnowprefe
                            tch cpuid_fault epb cat_l2 invpcid_single cdp_l2 ssbd i
                            brs ibpb stibp ibrs_enhanced tpr_shadow vnmi flexpriori
                            ty ept vpid ept_ad fsgsbase tsc_adjust bmi1 avx2 smep b
                            mi2 erms invpcid rdt_a avx512f avx512dq rdseed adx smap
                            avx512ifma clflushopt clwb intel_pt avx512cd sha_ni av
                            x512bw avx512vl xsaveopt xsavec xgetbv1 xsaves split_lo
                            ck_detect dtherm ida arat pln pts hwp hwp_notify hwp_ac
                            t_window hwp_epp hwp_pkg_req avx512vbmi umip pku ospke 
                            avx512_vbmi2 gfni vaes vpclmulqdq avx512_vnni avx512_bi
                            talg avx512_vpopcntdq rdpid movdiri movdir64b fsrm avx5
                            12_vp2intersect md_clear ibt flush_l1d arch_capabilitie
                            s
    Virtualization features: 
    Virtualization:        VT-x
    Caches (sum of all):     
    L1d:                   192 KiB (4 instances)
    L1i:                   128 KiB (4 instances)
    L2:                    5 MiB (4 instances)
    L3:                    8 MiB (1 instance)
    NUMA:                    
    NUMA node(s):          1
    NUMA node0 CPU(s):     0-7
    Vulnerabilities:         
    Gather data sampling:  Mitigation; Microcode
    Itlb multihit:         Not affected
    L1tf:                  Not affected
    Mds:                   Not affected
    Meltdown:              Not affected
    Mmio stale data:       Not affected
    Retbleed:              Not affected
    Spec rstack overflow:  Not affected
    Spec store bypass:     Mitigation; Speculative Store Bypass disabled via prctl
    Spectre v1:            Mitigation; usercopy/swapgs barriers and __user pointer
                            sanitization
    Spectre v2:            Mitigation; Enhanced IBRS, IBPB conditional, RSB fillin
                            g, PBRSB-eIBRS SW sequence
    Srbds:                 Not affected
    Tsx async abort:       Not affected


