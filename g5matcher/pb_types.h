/*
 * Copyright (c) 2008 - 2015, Precise Biometrics AB
 * All rights reserved.
 *
 */

/*****************************************************************************
 *
 * pbtypes.h
 *
 * Description:
 *  This file contains definitions of types that should be used in order to
 *  get the software as portable as possible. It does also contain specific
 *  "Precise Biometrics" types that are common for multiple modules.
 *  The specification of this file is in MAG/SPC/0019/PB.
 *
 ****************************************************************************/
#ifndef PB_TYPES_H
#define PB_TYPES_H

/* Definition of size_t. */
#include <stddef.h>

/* Definition of malloc */
#include <stdlib.h>

/* C99 integer types */

#if defined(_MSC_VER) && _MSC_VER < 1600
# include "win32/stdint.h"
#else
# include <stdint.h>
#endif

#ifdef __cplusplus
extern "C" {
#endif

/*
 * Macro to be used in functions that have unused arguments.
 * This is used to supress compiler warnings and makes it clear that the
 * argument was left unused on purpose.
 */
#ifndef PB_UNUSED_ARGUMENT
#define PB_UNUSED_ARGUMENT(x) (void)x
#endif /* PB_UNUSED_ARGUMENT */

#ifndef PB_CONTAINER_ATTRIBUTE
    #define PB_CONTAINER_ATTRIBUTE // In general no special container attribute is required
#endif

/*
 * Macro to be used to mark prototypes of functions whos return value
 * must be checked. Only works on gcc. Use in .h file like this:
 *
 *  pb_rc_t function_can_fail(arg_t) PB_MUST_CHECK_RESULT;
 *
 * Will expand to nothing on unsupported compilers.
 */
#ifdef __GNUC__
# define PB_MUST_CHECK_RESULT __attribute__((__warn_unused_result__))
#else
# define PB_MUST_CHECK_RESULT
#endif

/*
 * Macro used to conditionally decorate module interfaces as const.
 *
 * The default is to use const declaration but a special case where
 * ARM compiler option APCS variant ropi is used to create position
 * independent code will not accept such code but will accept a
 * non-const declaration (this could also be used to add decorator for
 * Windows DLLs).
 */
#ifdef __APCS_ROPI
# define pbif_const
#else
# define pbif_const const
#endif

/*
 * Definition of mr_const and mr_rw values
 * Beware: Do not change these values
 */
#define PB_MR_CONST_CONSTANT        1
#define PB_MR_CONST_NOT_CONSTANT    0
#define PB_MR_RW_READ_ONLY          0
#define PB_MR_RW_READ_WRITE         1

/*
 * Macros to be used for converting back and forth between
 * mr_const and mr_rw flags used by for example pb_image_t
 * and pb_template_t respectively.
 */
#define PB_MR_CONST_TO_MR_RW(x)     ((x) == PB_MR_CONST_NOT_CONSTANT ? PB_MR_RW_READ_WRITE : PB_MR_RW_READ_ONLY)
#define PB_MR_RW_TO_MR_CONST(x)     ((x) == PB_MR_RW_READ_ONLY ? PB_MR_CONST_CONSTANT : PB_MR_CONST_NOT_CONSTANT)

/** Sensor size. */
typedef enum {
    /** Unknown sensor size. */
    PB_SENSOR_SIZE_UNKNOWN = 0,

    /** A full size sensor, covering an entire finger. */
    PB_SENSOR_SIZE_FULL,

    /** Square sensors. */
    /** Extra Large (XL) square sensor, approximately 10x10 mm (200x200 pixels @ 500 dpi). */
    PB_SENSOR_SIZE_SQUARE_XL,
    /** Large (L) square sensor, approximately 8x8 mm (160x160 pixels @ 500 dpi). */
    PB_SENSOR_SIZE_SQUARE_L,
    /** Medium (M) square sensor, approximately 6.5x6.5 mm (130x130 pixels @ 500 dpi). */
    PB_SENSOR_SIZE_SQUARE_M,
    /** Small (S) square sensor, approximately 5.8x5.8 mm (115x115 pixels @ 500 dpi). */
    PB_SENSOR_SIZE_SQUARE_S,
    /** Extra Small (XS) square sensor, approximately 5x5 mm (100x100 pixels @ 500 dpi). */
    PB_SENSOR_SIZE_SQUARE_XS,
    /** Extra Extra Small (XXS) square sensor, approximately 4x3 mm (80x64 pixels @ 500 dpi). */
    PB_SENSOR_SIZE_SQUARE_XXS,

    /** Medium (M) rectangular sensor, approximately 10x4 mm (200x80 pixels @ 500 dpi). */
    PB_SENSOR_SIZE_RECTANGULAR_M,
    /** Small (S) rectangular sensor, approximately 10x3 mm (200x60 pixels @ 500 dpi). */
    PB_SENSOR_SIZE_RECTANGULAR_S,
} pb_sensor_size_t;

/** Sensor type. */
typedef enum {
    /** Unknown sensor type. */
    PB_SENSOR_TYPE_UNKNOWN = 0,
    /** Touch (flatbed) sensor. */
    PB_SENSOR_TYPE_TOUCH,
    /** Swipe sensor. */
    PB_SENSOR_TYPE_SWIPE,
} pb_sensor_type_t;

/** Flip type codes. */
typedef enum pb_flip_e {
    PB_FLIP_NONE = 0,
    PB_FLIP_HORIZONTALLY = 1,
    PB_FLIP_VERTICALLY = 2,
    PB_FLIP_ON_BOTH_SIDES = (PB_FLIP_HORIZONTALLY | PB_FLIP_VERTICALLY),
    PB_FLIP_90_CLOCKWISE = 4,
    PB_FLIP_90_COUNTERCLOCKWISE = 5
} pb_flip_t;

#ifdef __cplusplus
}
#endif

/** Memory-reference callback type is used by a few selected
 * objects to support implementation  allocation free versions
 * of objects such as image and template. In principle
 * any BMF objects delete function should be possible to typecast
 * to this type, else a veener function can be created.
 */
typedef void pb_memref_release_fn_t(void* object);

#endif  /* PB_TYPES_H */
