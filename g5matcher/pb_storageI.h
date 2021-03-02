/*
 * Copyright (c) 2018 - 2018, Precise Biometrics AB
 * All rights reserved.
 *
 */

#ifndef PB_STORAGEI_H
#define PB_STORAGEI_H

#include "pb_errors.h"
#include "pb_types.h"

#ifdef __cplusplus
extern "C" {
#endif

/** The purpose of the storage interface is to make it possible to
  * avoid an intermediate copy on the heap of serialized template data
  * or other data objects. Through this interface, the data can be
  * written straight to permanent storage. The specific implementation
  * of the interface is written by the system integrator and passed as
  * a parameter to the functions that encode templates and other data
  * objects.
  *
  * Example:
  * 
  * @code
  *
  * typedef struct my_storage_state_s {
  *    uint8_t* storage_address;
  *    uint32_t storage_size;
  *    uint32_t offset;
  * } my_storage_state_t;
  *
  * static pb_rc_t my_reserve(void* storage_state,
  *                           uint32_t data_size,
  *                           const uint8_t** data,
  *                           uint32_t* mr_const,
  *                           pb_memref_release_fn_t** mr_release,
  *                           void** mr_release_obj)
  * {
  *    my_storage_state_t* my_storage_state = (my_storage_state_t*)storage_state;
  *    my_storage_state->offset = 0;
  *    *data                    = my_storage_state->storage_address;
  *    *mr_const                = PB_MR_CONST_NOT_CONSTANT;
  *    *mr_release              = 0;
  *    *mr_release_obj          = 0;
  *    if (data_size > my_storage_state->storage_size) {
  *       return PB_RC_MEMORY_ALLOCATION_FAILED;
  *    }
  *    return PB_RC_OK;
  * }
  *
  * static pb_rc_t my_write(void* storage_state,
  *                         const uint8_t* data,
  *                         uint32_t data_size)
  * {
  *    my_storage_state_t* my_storage_state = (my_storage_state_t*)storage_state;
  *    if (my_storage_state->offset + data_size > my_storage_state->storage_size) {
  *       return PB_RC_FATAL;
  *    }
  *    write_to_flash(my_storage_state->storage_address + my_storage_state->offset, data, data_size);
  *    my_storage_state->offset += data_size;
  *    return PB_RC_OK;
  * }
  *
  * pb_storageI my_storage = { my_reserve, my_write };
  *
  * #define MY_STORAGE_ADDRESS 0x800000
  * #define MY_STORAGE_SIZE    0x10000
  *
  * pb_algorithm_t* algorithm;
  * pb_multitemplate_enroll_t* mte;
  * pb_template_t* multitemplate;
  * pb_finger_t* finger;
  * uint8_t max_verifications = 16; // Example value
  * int rfu = 0; // 0 means incremental update
  * my_storage_state_t my_storage_state = {MY_STORAGE_ADDRESS, MY_STORAGE_SIZE, 0};
  *
  * mte = pb_multitemplate_enroll_mcu_create_algorithm_with_storage (algorithm, finger, gui, max_verifications,
  *                                                                  rfu, &my_storage, &my_storage_state);
  * if (mte == 0) {
  *   // Error has occured
  * }
  *
  * status = pb_multitemplate_enroll_finalize_template (algorithm, mte, &multitemplate);
  * if (status == PB_RC_OK) {
  *    pb_template_delete(multitemplate);
  * }
  * @endcode
  */

/** The serialization process begins with a request to store data_size
  * number of bytes. The storage returns PB_RC_OK if the required
  * space is available. The out parameter data points to an address in
  * memory where the data can be read when it has been serialized. If
  * set to 0, the data is not memory mapped after serialization. The
  * storage can also return a mr_release function that will be
  * called by the system with the mr_release_obj when it no longer
  * uses the serialized data.
  *
  * @param[in] storage_state Used by the storage to keep internal state.
  * @param[in] data_size     The number of bytes that will be serialized.
  * @param[out] data         Pointer to serialized data. 0 means the data
  *                          is not memory mapped after serialization.
  * @param[out] mr_const     Flag that indicates that it is ok to break
  *                          the const-ness of the pb_template_t object that
  *                          is created when the serialization is complete.
  *                          Setting to PB_MR_CONST_NOT_CONSTANT will make
  *                          it possible to save RAM, but the drawback is
  *                          that only a single thread can use the returned
  *                          template safely.
  * @param[out] mr_release   Optional relase callback, called when the
  *                          last reference to the serialized data is
  *                          deleted.
  * @param[out] mr_release_obj Value passed to the optional release
  *                            function.
  *
  * @return PB_RC_OK on success, other code on error.
  */
typedef pb_rc_t pb_storageI_reserve_fn_t(void* storage_state,
                                         uint32_t data_size,
                                         const uint8_t** data,
                                         uint32_t* mr_const,
                                         pb_memref_release_fn_t** mr_release,
                                         void** mr_release_obj);

/** The serialization process continues with a sequence of write
  * operations. The last write operation will have data_size = 0.
  *
  * @param[in] storage_state Used by the storage to store internal state.
  * @param[in] data Serialized data
  * @param[in] data_size Size in bytes of data.
  *
  * @return PB_RC_OK on success, other code on error.
  */
typedef pb_rc_t pb_storageI_write_fn_t(void* storage_state,
                                       const uint8_t* data,
                                       uint32_t data_size);

/// Data storage interface
typedef struct pb_storageI_s {
    /// See definition of pb_storageI_reserve_fn_t()
    pb_storageI_reserve_fn_t* reserve;
    /// See definition of pb_storageI_write_fn_t()
    pb_storageI_write_fn_t* write;
} pb_storageI;

#ifdef __cplusplus
}
#endif

#endif /* PB_STORAGEI_H */
