/**
 * Copyright (c) 2017, Precise Biometrics AB
 * All rights reserved.
 *
 */

#ifndef PB_PADI_H
#define PB_PADI_H

#include "pb_errors.h"
#include "pb_session.h"
#include "pb_pad.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Creates a specific presentation attack detector object.
 *
 * @param[in] session is the session object.
 *
 * @return the allocated pad object if successful, else 0. The
 *         caller is responsible for deleting the returned object by
 *         calling pb_pad_delete.
 */
typedef pb_pad_t* pb_padI_create_fn_t(pb_session_t* session);

/// The PAD object API
typedef struct pb_padI_s{
    /// See definition of pb_padI_create_fn_t()
    pb_padI_create_fn_t* create;
} pb_padI;

#ifdef __cplusplus
}
#endif

#endif /* PB_PADI_H */
