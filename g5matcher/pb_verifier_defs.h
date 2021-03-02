/**
 * Copyright (c) 2019, Precise Biometrics AB
 * All rights reserved.
 *
 */

#ifndef PB_VERIFIER_DEFS_H
#define PB_VERIFIER_DEFS_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef uint32_t pb_far_t;

/* False Accept Rates, (1/X). */
#define PB_FAR_1                0
#define PB_FAR_2                1
#define PB_FAR_5                2
#define PB_FAR_10               3
#define PB_FAR_20               4
#define PB_FAR_50               5
#define PB_FAR_100              6
#define PB_FAR_200              7
#define PB_FAR_500              8
#define PB_FAR_1000             9
#define PB_FAR_2000             10
#define PB_FAR_5000             11
#define PB_FAR_10000            12
#define PB_FAR_20000            13
#define PB_FAR_50000            14
#define PB_FAR_100000           15
#define PB_FAR_200000           16
#define PB_FAR_500000           17
#define PB_FAR_1000000          18
#define PB_FAR_2M               19
#define PB_FAR_5M               20
#define PB_FAR_10M              21
#define PB_FAR_20M              22
#define PB_FAR_50M              23
#define PB_FAR_100M             24
#define PB_FAR_200M             25
#define PB_FAR_500M             26
#define PB_FAR_1000M            27
#define PB_FAR_Inf              28

#define PB_FAR_1K   PB_FAR_1000
#define PB_FAR_2K   PB_FAR_2000
#define PB_FAR_5K   PB_FAR_5000
#define PB_FAR_10K  PB_FAR_10000   
#define PB_FAR_20K  PB_FAR_20000   
#define PB_FAR_50K  PB_FAR_50000
#define PB_FAR_100K PB_FAR_100000
#define PB_FAR_200K PB_FAR_200000
#define PB_FAR_500K PB_FAR_500000
#define PB_FAR_1M   PB_FAR_1000000

#define PB_FAR_MAX 27
#define PB_FAR_INVALID 0x7fffffff

/** Decisions. */
#define PB_DECISION_MATCH               (1)
#define PB_DECISION_NON_MATCH           (0)
#define PB_DECISION_TAMPERED            (0)
#define PB_DECISION_REAL_FINGER         (PB_DECISION_MATCH)
#define PB_DECISION_NOT_REAL_FINGER     (PB_DECISION_NON_MATCH)

#define IS_MATCH(value)                 ((value) == PB_DECISION_MATCH)
#define IS_NON_MATCH(value)             ((value) != PB_DECISION_MATCH)
#define IS_REAL_FINGER(value)           ((value) == PB_DECISION_REAL_FINGER)
#define IS_NOT_REAL_FINGER(value)       (!IS_REAL_FINGER(value))

/** The number of FAR values of the threshold table. The following values are
 * supported: PB_FAR_1, _2, _5, _10, _20, _50, _100 ... PB_FAR_1000M. */
#define PB_VERIFIER_NBR_OF_FAR_LEVELS           (PB_FAR_MAX+1)

/**
 * Get index of far enum. When using enums that are not matched to log values these needs
 * to be translated.
 */
#ifdef USE_HIGH_ENTROPY_ENUMS
    #define PB_FAR_TO_INDEX(x) pb_far_to_index(x)
    #define PB_INDEX_TO_FAR(x) pb_index_to_far(x)
    int32_t pb_far_to_index(pb_far_t far);
    pb_far_t pb_index_to_far(int32_t far);

#else
    #define PB_FAR_TO_INDEX(x) ((int32_t)(x))
    #define PB_INDEX_TO_FAR(x) ((pb_far_t)(x))

#endif

/* False Reject Rates for PAD and finger detect. */
#define    PB_FRR_0             (0)  /*   0.0% FRR */
#define    PB_FRR_0_2       (28013)  /*   0.2% FRR */
#define    PB_FRR_0_5       (30720)  /*   0.5% FRR */
#define    PB_FRR_1         (32768)  /*   1.0% FRR */
#define    PB_FRR_1_5       (33966)  /*   1.5% FRR */
#define    PB_FRR_2         (34816)  /*   2.0% FRR */
#define    PB_FRR_3         (36014)  /*   3.0% FRR */
#define    PB_FRR_4         (36864)  /*   4.0% FRR */
#define    PB_FRR_5         (37523)  /*   5.0% FRR */
#define    PB_FRR_10        (39571)  /*  10.0% FRR */
#define    PB_FRR_100       (46375)  /* 100.0% FRR */

typedef int32_t pb_frr_t;

#define PB_FRR_MIN PB_FRR_0
#define PB_FRR_MAX PB_FRR_100

#ifdef __cplusplus
}
#endif

#endif /* PB_VERIFIER_DEFS_H */
