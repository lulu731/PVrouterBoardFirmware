#ifndef CALIBRATION_CONSTANTS_H
#define CALIBRATION_CONSTANTS_H

/**
 * @file calibration_constants.h
 * @brief Constants used in calibration calculations
 *
 * This file contains magic numbers used throughout the calibration
 * code, replaced with well-named constants for better readability
 * and maintainability.
 */

/**
 * @brief Maximum PL constant value
 *
 * This is the maximum value that can be written to the PL_CONST
 * register (21H-22H). It represents 2^23 (838860800), the maximum
 * unsigned 24-bit value.
 */
#define PL_CONST_MAX 838860800u

/**
 * @brief Divider for RMS voltage readings
 *
 * The ADC returns raw values that need to be divided by this factor
 * to get the actual voltage in Volts.
 */
#define VOLTAGE_RMS_DIVIDER 100

/**
 * @brief Divider for RMS current readings
 *
 * The ADC returns raw values that need to be divided by this factor
 * to get the actual current in Amperes.
 */
#define CURRENT_RMS_DIVIDER 1000

/**
 * @brief Number of samples for averaging power offset
 *
 * Number of samples to take when calculating the average power offset.
 */
#define POWER_OFFSET_SAMPLES 5

/**
 * @brief Broadcast interval in microseconds
 *
 * Interval between broadcasts to WebSocket clients (2 seconds = 2000000 microseconds)
 */
#define BROADCAST_INTERVAL_US 2000000u

/**
 * @brief Broadcast interval in milliseconds
 */
#define BROADCAST_INTERVAL_MS 2000

#endif // CALIBRATION_CONSTANTS_H
