/*
* Copyright (c) 2019, Precise Biometrics AB
* All rights reserved.
*
*/

#ifndef PB_ERRORS_H
#define PB_ERRORS_H

#include <stdint.h>

/* The possible return codes from the PB interfaces. */
typedef uint32_t pb_rc_t;

#define ERROR_CODE(error_nbr) \
        ((pb_rc_t)error_nbr)

/**
 * Converts an error code into a string representation. May return null outside
 * of development builds.
 * 
 */
const char*
pb_error_to_string(pb_rc_t error_code);


/** The function returned without errors. */
#define PB_RC_OK ERROR_CODE(0)

/** The requested operation is not supported by the implementation. */
#define PB_RC_NOT_SUPPORTED ERROR_CODE(1)

/** At least one of the parameters is invalid. */
#define PB_RC_INVALID_PARAMETER ERROR_CODE(2)

/** The data passed to the function has the wrong format. */
#define PB_RC_WRONG_DATA_FORMAT ERROR_CODE(3)

/** At least one buffer has an incorrect size. */
#define PB_RC_WRONG_BUFFER_SIZE ERROR_CODE(4)

/** A function is called before the interface being initialized. */
#define PB_RC_NOT_INITIALIZED ERROR_CODE(5)

/** The requested item was not found. */
#define PB_RC_NOT_FOUND ERROR_CODE(6)

/** The function returned because the caller canceled it. */
#define PB_RC_CANCELLED ERROR_CODE(7)

/** The operation timed out before it could finish the operation. */
#define PB_RC_TIMED_OUT ERROR_CODE(8)

/** Cannot allocate enough memory. */
#define PB_RC_MEMORY_ALLOCATION_FAILED ERROR_CODE(9)

/** Unable to open a file. */
#define PB_RC_FILE_OPEN_FAILED ERROR_CODE(10)
/** Unable to read from a file. */
#define PB_RC_FILE_READ_FAILED ERROR_CODE(11)
/** Unable to write to a file. */
#define PB_RC_FILE_WRITE_FAILED ERROR_CODE(12)

/** Reader is not connected or not started. */
#define PB_RC_READER_NOT_AVAILABLE ERROR_CODE(13)

/** Reader has been locked by another user. */
#define PB_RC_READER_BUSY ERROR_CODE(14)

/**
 * The enrollment failed because none of the images matched each
 * other. This only applies if multiple images are required for
 * enrollment.
 */
#define PB_RC_ENROLLMENT_VERIFICATION_FAILED ERROR_CODE(15)

/**
 * The finger is temporarily blocked due to brute force attack attempt. 
 * As long as the finger is blocked, all verification attempts will fail.
 */
#define PB_RC_FINGER_BLOCKED ERROR_CODE(16)

/**
 * An undefined fatal error has occurred. This error code is used
 * for errors that "cannot happen" and isn't covered by any other
 * error code. */
#define PB_RC_FATAL ERROR_CODE(17)

/**
 * The selected sensor type and size is not supported by the
 * verifier to calculate score or make decisions. Choose one
 * of the supported algorithm combinations. */
#define PB_RC_SENSOR_NOT_SUPPORTED ERROR_CODE(18)

/**
 * The function has exceeded maximum capacity and cannot accept
 * more data.
 */
#define PB_RC_CAPACITY ERROR_CODE(19)

/**
 * No algorithm as been selected. This is a requirement for newer
 * versions of the API.
 */
#define PB_RC_NO_ALGORITHM ERROR_CODE(20)

/** The image does not contain enough fingerprint information. */
#define PB_RC_INSUFFICIENT_INFORMATION ERROR_CODE(21)

/** Software has been tampered with. Returned for some secure usecases, such as split TEE/SE verification. */
#define PB_RC_FLOW_CONTROL ERROR_CODE(22)

#endif // PB_ERRORS_H
