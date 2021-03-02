/*
 * Copyright (c) 2017, Precise Biometrics AB
 * All rights reserved.
 *
 */

#ifndef PB_CRC32_H
#define PB_CRC32_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

/// State structure for CRC-32
typedef struct pb_crc32_state_st {
    /// The 32-bit checksum
    uint32_t crc;
} pb_crc32_state_t;

/** Convenience function to compute crc32 on the buffer directly. */
uint32_t
pb_crc32(const uint8_t* data, uint32_t len);

/// Initializes the crc32 state
/**
 * @param[in,out] state is the crc32 state to initialize.
 */
void
pb_crc32_init_state(pb_crc32_state_t* state);

/// Computes the crc32 for the buffer
/** Accumulates the result in the provided structure.
 *
 * @param[in] data is the data to process
 * @param[in] data_size is the amount of data to process
 * @param[in,out] state is the crc32 state to update.
 */
void
pb_crc32_compute(const uint8_t* data,
                 uint32_t data_size,
                 pb_crc32_state_t* state);

/// Retrieves the crc32 from the state object.
/**
 * @param[in] state is the crc32 state to get a crc from.
 * 
 * @return the current crc32 checksum
 */
uint32_t
pb_crc32_get(pb_crc32_state_t* state);

#ifdef __cplusplus
}
#endif

#endif
