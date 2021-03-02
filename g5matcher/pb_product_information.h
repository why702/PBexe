/*
 * Copyright (c) 2008 - 2014, Precise Biometrics AB
 * All rights reserved.
 *
 */

#ifndef PB_PRODUCT_INFORMATION_H
#define PB_PRODUCT_INFORMATION_H

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Returns the product number. The product number
 * is a string of letters and numbers which uniquely identify
 * the product that build the library.
 */
const char*
pb_prod_number(void);

/**
 * Returns the product revision in the format
 * R1234_5 (actual numbers may vary).
 */
const char*
pb_prod_revision(void);

/**
 * Returns revison number in decimal notation.
 * 
 * Higher number means a later release. Revision R1234_5
 * would return as the integer 12345.
 */
unsigned int
pb_prod_rev(void);

/** Returns the source revision of the source that built the library. */
const char*
pb_prod_source_rev(void);

#ifdef __cplusplus
}
#endif

#endif /* PB_PRODUCT_INFORMATION_H */
