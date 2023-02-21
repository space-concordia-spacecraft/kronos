#ifndef __NANOCAM_H__
#define __NANOCAM_H__
/* Copyright (c) 2013-2018 GomSpace A/S. All rights reserved. */
/**
   @file
   NanoCam Application Interface.
*/

#include <gs/cam/nanocam_types.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Image list callback.
 *
 * This callback is called once for each element in the image listing. If no
 * images are available, the function is called once with elm equal to NULL.
 *
 * @param seq Element sequence number, starting from 0.
 * @param elm Pointer to image list element structure
 */
typedef void (*nanocam_img_list_cb)(int seq, cam_list_element_t *elm);

/**
 * Set camera CSP address.
 *
 * This function sets the CSP address of the camera for all other commands.
 *
 * @param node CSP address of camera
 */
void nanocam_set_node(uint8_t node);

/**
 * Get camera CSP address.
 *
 * This function gets the CSP address of the camera for all other commands.
 *
 * @return node CSP address of camera
 */
uint8_t nanocam_get_node();

/**
 * Capture a picture.
 *
 * This function captures a picture with the camera.
 *
 * @param[in] snap snap
 * @param[in,out] reply reply
 * @param[in] timeout timeout.
 * @return 0 on success, error code on error
 */
int nanocam_snap(cam_snap_t *snap, cam_snap_reply_t *reply, unsigned int timeout);

/**
 * Store a captured image.
 *
 * This function stores captured image from the snap buffer to either flash or RAM.
 *
 * @param[in] store store.
 * @param[in,out] reply reply
 * @param[in] timeout timeout.
 * @return 0 on success, error code on error
 */
int nanocam_store(cam_store_t *store, cam_store_reply_t *reply, unsigned int timeout);

/**
 * Read sensor register.
 *
 * @param[in] reg register.
 * @param[out] value value.
 * @param[in] timeout timeout.
 * @return 0 on success, error code on error
 */
int nanocam_reg_read(uint8_t reg, uint16_t *value, unsigned int timeout);

/**
 * Write sensor register.
 *
 * @param[in] reg register.
 * @param[in] value value.
 * @param[in] timeout timeout.
 * @return 0 on success, error code on error
 */
int nanocam_reg_write(uint8_t reg, uint16_t value, unsigned int timeout);

/**
 * List RAM images.
 *
 * @param[in] cb cb.
 * @param[in] timeout timeout.
 * @return 0 on success, error code on error
 */
int nanocam_img_list(nanocam_img_list_cb cb, unsigned int timeout);

/**
 * Flush RAM images.
 *
 * @param[in] timeout timeout.
 * @return 0 on success, error code on error
 */
int nanocam_img_list_flush(unsigned int timeout);

/**
 * Focus assist routine.
 *
 * Run one iteration of focus assist routine.
 *
 * @param[in] algorithm algorithm.
 * @param[out] af af. 
 * @param[in] timeout timeout.
 * @return 0 on success, error code on error
 */
int nanocam_focus(uint8_t algorithm, uint32_t *af, unsigned int timeout);

/**
 * Recover data partition.
 *
 * @param[in] timeout timeout.
 * @return 0 on success, error code on error
 */
int nanocam_recoverfs(unsigned int timeout);

/**
 * Register command interface.
 *
 */
void nanocam_register_commands(void);

#ifdef __cplusplus
}
#endif
#endif
