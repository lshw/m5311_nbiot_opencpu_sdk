/* Copyright Statement:
 *
 * (C) 2005-2016  MediaTek Inc. All rights reserved.
 *
 * This software/firmware and related documentation ("MediaTek Software") are
 * protected under relevant copyright laws. The information contained herein
 * is confidential and proprietary to MediaTek Inc. ("MediaTek") and/or its licensors.
 * Without the prior written permission of MediaTek and/or its licensors,
 * any reproduction, modification, use or disclosure of MediaTek Software,
 * and information contained herein, in whole or in part, shall be strictly prohibited.
 * You may only use, reproduce, modify, or distribute (as applicable) MediaTek Software
 * if you have agreed to and been bound by the applicable license agreement with
 * MediaTek ("License Agreement") and been granted explicit permission to do so within
 * the License Agreement ("Permitted User").  If you are not a Permitted User,
 * please cease any access or use of MediaTek Software immediately.
 * BY OPENING THIS FILE, RECEIVER HEREBY UNEQUIVOCALLY ACKNOWLEDGES AND AGREES
 * THAT MEDIATEK SOFTWARE RECEIVED FROM MEDIATEK AND/OR ITS REPRESENTATIVES
 * ARE PROVIDED TO RECEIVER ON AN "AS-IS" BASIS ONLY. MEDIATEK EXPRESSLY DISCLAIMS ANY AND ALL
 * WARRANTIES, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE OR NONINFRINGEMENT.
 * NEITHER DOES MEDIATEK PROVIDE ANY WARRANTY WHATSOEVER WITH RESPECT TO THE
 * SOFTWARE OF ANY THIRD PARTY WHICH MAY BE USED BY, INCORPORATED IN, OR
 * SUPPLIED WITH MEDIATEK SOFTWARE, AND RECEIVER AGREES TO LOOK ONLY TO SUCH
 * THIRD PARTY FOR ANY WARRANTY CLAIM RELATING THERETO. RECEIVER EXPRESSLY ACKNOWLEDGES
 * THAT IT IS RECEIVER'S SOLE RESPONSIBILITY TO OBTAIN FROM ANY THIRD PARTY ALL PROPER LICENSES
 * CONTAINED IN MEDIATEK SOFTWARE. MEDIATEK SHALL ALSO NOT BE RESPONSIBLE FOR ANY MEDIATEK
 * SOFTWARE RELEASES MADE TO RECEIVER'S SPECIFICATION OR TO CONFORM TO A PARTICULAR
 * STANDARD OR OPEN FORUM. RECEIVER'S SOLE AND EXCLUSIVE REMEDY AND MEDIATEK'S ENTIRE AND
 * CUMULATIVE LIABILITY WITH RESPECT TO MEDIATEK SOFTWARE RELEASED HEREUNDER WILL BE,
 * AT MEDIATEK'S OPTION, TO REVISE OR REPLACE MEDIATEK SOFTWARE AT ISSUE,
 * OR REFUND ANY SOFTWARE LICENSE FEES OR SERVICE CHARGE PAID BY RECEIVER TO
 * MEDIATEK FOR SUCH MEDIATEK SOFTWARE AT ISSUE.
 */


#ifndef __HAL_GPT_H__
#define __HAL_GPT_H__





typedef enum {
    HAL_GPT_0 = 0,                          /**< GPT port 0. */
    HAL_GPT_1 = 1,                          /**< GPT port 1. */
    HAL_GPT_2 = 2,                          /**< GPT port 2. */
    HAL_GPT_3 = 3,                          /**< GPT port 3. */
    HAL_GPT_4 = 4,                          /**< GPT port 4. */
    HAL_GPT_5 = 5,                          /**< GPT port 5. */
    HAL_GPT_MAX_PORT = 6,                        /**< The total number of GPT ports (invalid GPT port). */
    HAL_GPT_MAX = 6
} hal_gpt_port_t;



/** @defgroup hal_gpt_typedef Typedef
  * @{
  */

/** @brief  Register a callback function when using GPT oneshot or repeat modes.
  *         The callback is called after the timer expires and triggers an interrupt in the GPT ISR routine.
  *         For more details about the callback function, please refer to #hal_gpt_register_callback().
  * @param[in] user_data is the pointer to the user defined data to pass to the callback function.
  */
typedef void(* hal_gpt_callback_t)(void *user_data);

/**
  * @}
  */
/** @defgroup hal_gpt_enum Enum
  * @{
  */
/** @brief This enum defines the GPT timer type. */
typedef enum {
    HAL_GPT_TIMER_TYPE_ONE_SHOT = 0,                /**< Set the GPT oneshot mode.  */
    HAL_GPT_TIMER_TYPE_REPEAT   = 1                 /**< Set the GPT repeat  mode.  */
} hal_gpt_timer_type_t;


/** @brief This enum defines the GPT status. */
typedef enum {
    HAL_GPT_STATUS_ERROR_PORT_USED   = -4,         /**< The timer has beed used. */
    HAL_GPT_STATUS_ERROR             = -3,         /**< GPT function error occurred. */
    HAL_GPT_STATUS_ERROR_PORT        = -2,         /**< A wrong GPT port is set. */
    HAL_GPT_STATUS_INVALID_PARAMETER = -1,         /**< An invalid parameter. */
    HAL_GPT_STATUS_OK   = 0                        /**< No error occurred during the function call.*/
} hal_gpt_status_t;


/** @brief This enum defines the GPT running status. */
typedef enum {
    HAL_GPT_STOPPED = 0,                            /**< The GPT has stopped. */
    HAL_GPT_RUNNING = 1                             /**< The GPT is running. */
} hal_gpt_running_status_t;


/**
  * @}
  */

/**
 * @brief     This function initializes the GPT base enironment. Call this function if a timer is required.
 * @param[in] gpt_port is the port number.
 * @return    #HAL_GPT_STATUS_OK, if the operation is successful.\n
 *            #HAL_GPT_STATUS_ERROR_PORT, if the gpt_port value is wrong.
 * @sa        #hal_gpt_deinit()
 */
hal_gpt_status_t hal_gpt_init(hal_gpt_port_t gpt_port);


/**
 * @brief     This function de-initializes the GPT timer.
 *            After calling this function, the callback is cleared, the clock power is turned off, interrupts and GPT module are disabled.
 * @param[in] gpt_port is the port number.
 * @return    #HAL_GPT_STATUS_OK, if the operation is successful.\n
 *            #HAL_GPT_STATUS_ERROR_PORT, if the gpt_port value is wrong.
 * @sa        #hal_gpt_init()
 */
hal_gpt_status_t hal_gpt_deinit(hal_gpt_port_t gpt_port);



/**
 * @brief     This function registers a callback function with the timer specified at the port.
 *            The callback can only be registered when the timer, as specified by the port, is in #HAL_GPT_STOPPED state.
 *            If the timer is in #HAL_GPT_RUNNING state, the callback cannot be registered and this function returns #HAL_GPT_STATUS_ERROR.
 * @param[in] gpt_port is the port number.
 * @param[in] callback is the function pointer of the callback.
 *            This callback will be called when the timer expires.
 * @param[in] user_data is the pointer to the user defined data to pass to the callback function.
 * @return    #HAL_GPT_STATUS_OK, if the operation is successful.\n
 *            #HAL_GPT_STATUS_ERROR_PORT, if the gpt_port value is wrong.\n
 *            #HAL_GPT_STATUS_ERROR, if the callback registration failed.
 */
hal_gpt_status_t hal_gpt_register_callback(hal_gpt_port_t gpt_port, hal_gpt_callback_t callback, void *user_data);

/**
 * @brief     This function sets the expiration time in milliseconds and the timer mode, then starts the timer.
 *            The function should only be called while the timer is stopped. An error would be returned if this function is called when the timer is running.
 * @param[in] gpt_port is the port number.
 * @param[in] timeout_time_ms is the expiration time in milliseconds.
 * @param[in] timer_type is the timer mode, such as oneshot or repeat timer mode defined in #hal_gpt_timer_type_t.
 * @return    #HAL_GPT_STATUS_OK, if the operation is successful.\n
 *            #HAL_GPT_STATUS_ERROR_PORT, if the gpt_port value is wrong.
 */
hal_gpt_status_t hal_gpt_start_timer_ms(hal_gpt_port_t gpt_port, uint32_t timeout_time_ms, hal_gpt_timer_type_t timer_type);


/**
 * @brief     This function sets the expiration time in microseconds and the timer mode, then starts the timer.
 *            The function should only be called while the timer is stopped. An error would be returned if this function is called when the timer is running.
 * @param[in] gpt_port is the port number.
 * @param[in] timeout_time_us is the expiration time in microseconds.
 * @param[in] timer_type is the timer mode, i.e. oneshot or repeat timer mode defined in #hal_gpt_timer_type_t.
 * @return    #HAL_GPT_STATUS_OK, if the operation is successful.\n
 *            #HAL_GPT_STATUS_ERROR_PORT, if the gpt_port value is wrong.
 */
hal_gpt_status_t hal_gpt_start_timer_us(hal_gpt_port_t gpt_port, uint32_t timeout_time_us, hal_gpt_timer_type_t timer_type);

/**
 * @brief     This function stops the timer only for oneshot mode and repeat mode.
 * @param[in] gpt_port is the port number.
 * @return    #HAL_GPT_STATUS_OK, if the timer is stopped successfully.\n
 *            #HAL_GPT_STATUS_ERROR_PORT, if the gpt_port value is wrong.
 */
hal_gpt_status_t hal_gpt_stop_timer(hal_gpt_port_t gpt_port);



/**
 * @brief      This function allocates timer handle.
 * @param[out] handle is an unsigned integer for accessing this timer. It's equivalent to an ID number of the timer.
 * @return     #HAL_GPT_STATUS_OK, if the operation is successful.\n
 *             #HAL_GPT_STATUS_ERROR, if a timer cannot be allocated.
 */
hal_gpt_status_t hal_gpt_sw_get_timer(uint32_t *handle);

/**
 * @brief      This function frees timer.
 * @param[in]  handle is the handle of the timer to be freed.
 * @return     #HAL_GPT_STATUS_OK, if the operation is successful.\n
 *             #HAL_GPT_STATUS_INVALID_PARAMETER, if the handle is invalid. \n
 *             #HAL_GPT_STATUS_ERROR, for all other errors.
 */
hal_gpt_status_t hal_gpt_sw_free_timer(uint32_t handle);

/**
 * @brief      This function starts the software timer.
 * @param[in]  handle is the handle of the timer to be started.
 * @param[in]  timeout_time_ms is the timeout value in milliseconds.
 * @param[in]  callback is the callback to be registered with this timer.
 *             This callback will be called when the timer times out.
 * @param[in]  user_data is the pointer to the user defined data to pass to the callback function.
 * @return     #HAL_GPT_STATUS_OK, if the operation is successful.\n
 *             #HAL_GPT_STATUS_INVALID_PARAMETER, if the parameter is invalid. \n
 *             #HAL_GPT_STATUS_ERROR, for all other errors.
 */
hal_gpt_status_t hal_gpt_sw_start_timer_ms(uint32_t handle, uint32_t timeout_time_ms, hal_gpt_callback_t callback, void *user_data);

/**
 * @brief      This function stops the specified software timer.
 * @param[in]  handle is the handle of the timer to be stopped.
 * @return     #HAL_GPT_STATUS_OK, if the operation is successful.\n
 *             #HAL_GPT_STATUS_INVALID_PARAMETER, if the handle is invalid. \n
 *             #HAL_GPT_STATUS_ERROR, for all other errors.
 */
hal_gpt_status_t hal_gpt_sw_stop_timer_ms(uint32_t handle);

/**
 * @brief      This function gets the remaining timeout value of the specified software timer.
 * @param[in]  handle is the handle of the timer to get the remaining time from.
 * @param[out] remaing_time is the pointer to the value of the remaining timeout, after the return of this function.
 * @return     #HAL_GPT_STATUS_OK, if the operation is successful.\n
 *             #HAL_GPT_STATUS_INVALID_PARAMETER, if the handle is invalid. \n
 *             #HAL_GPT_STATUS_ERROR, if the handle is not allocated.
 */
hal_gpt_status_t hal_gpt_sw_get_remaining_time_ms(uint32_t handle, uint32_t *remaing_time);



#endif /* __HAL_GPT_H__ */


