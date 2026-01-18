#include "calibration_helpers.h"

#include <stdint.h>
#include <math.h>

/**
 * @brief Sets the tx_data array with the given adc_data.
 *
 * This function takes an adc_data value and sets the first two bytes of the tx_data array to the given adc_data value.
 * The adc_data value is split into two bytes, where the first byte is the lower 8 bits and the second byte is the upper 8 bits.
 * The lower 8 bits are stored in tx_data[1] and the upper 8 bits are stored in tx_data[0].
 *
 * @param tx_data The array to be set with the given adc_data value.
 * @param data The adc_data value to be set in the tx_data array.
 */
void set_txdata_with(uint8_t tx_data[4], const adc_data data)
{
    tx_data[1] = (uint8_t)data;
    tx_data[0] = (uint8_t)(data>>8);
}


/**
 * @brief Splits a given uint32_t value into two uint16_t values.
 *
 * This function takes a uint32_t value and splits it into two uint16_t values, where the first uint16_t value is the lower 16 bits and the second uint16_t value is the upper 16 bits.
 *
 * @param data The uint32_t value to be split.
 * @param receiver An array of two uint16_t values to store the result.
 */
void split_uint32_to_uint8_array(const uint32_t data, uint16_t receiver[2])
{
    receiver[1] = (uint16_t)data;
    receiver[0] = (uint16_t)(data>>16);
}


/**
 * @brief Combines the given MMODE register values into a single uint16_t value.
 *
 * The returned value is a bitfield of the given values, where each value is stored in the following bits:
 * - Lgain: 13-15
 * - Ngain: 11-12
 * - LNsel: 10
 * - DisHPF: 8-9
 * - Amod: 7
 * - Rmod: 6
 * - Zxcon: 4-5
 * - Pthresh: 0-3
 *
 * @param Lgain The value of the Lgain MMODE register.
 * @param Ngain The value of the Ngain MMODE register.
 * @param LNsel The value of the LNsel MMODE register.
 * @param DisHPF The value of the DisHPF MMODE register.
 * @param Amod The value of the Amod MMODE register.
 * @param Rmod The value of the Rmod MMODE register.
 * @param Zxcon The value of the Zxcon MMODE register.
 * @param Pthresh The value of the Pthresh MMODE register.
 * @return The combined uint16_t value.
 */
uint16_t get_mmode_value(uint16_t Lgain, uint16_t Ngain, uint16_t LNsel,
                         uint16_t DisHPF, uint16_t Amod, uint16_t Rmod,
                         uint16_t Zxcon, uint16_t Pthresh)
{
    return (uint16_t)(Lgain<<13 | Ngain<<11 | LNsel<<10
                      | DisHPF<<8 | Amod<<7 | Rmod<<6
                      | Zxcon<<4 | Pthresh);
}

/**
 * @brief Calculates the average of a given array of uint16_t values.
 *
 * This function takes an array of uint16_t values and calculates the average of the values in the array.
 *
 * @param data_array The array of uint16_t values to calculate the average from.
 * @param array_size The size of the data_array.
 * @return The average of the values in the data_array.
 */
uint16_t get_average_data_in_array( const uint16_t data_array[], const uint8_t array_size)
{
    uint32_t data = 0;

    for (uint8_t i = 0; i < array_size; i++)
    {
        data += data_array[i];
    }
    return data / array_size;
}

extern uint16_t Gl, Vl, Vu;

/**
 * @brief Calculates the threshold value based on the calibration parameters and the given K value.
 *
 * The threshold value is calculated as 93.2067556 * Gl * Vl * Vu * K/1000.
 *
 * @param K The value to use for calculating the threshold.
 * @return The calculated threshold value.
 */
uint16_t get_threshold(const uint8_t K)
{
    return roundf(93.2067556 * (float)(Gl * Vl *Vu * K/1000));
}

/**
 * @brief Calculates the offset value based on the measured value and the gain.
 *
 * The offset value is calculated as the measured value divided by 1000, multiplied by the gain divided by (1 << 16), and then multiplied by (1 << 8).
 *
 * @param measured_value The measured value.
 * @param gain The gain value.
 * @return The calculated offset value.
 */
uint16_t get_offset_from_measured(const uint16_t measured_value, const uint16_t gain)
{
    float value = measured_value / 1000;
    return value * ((float)gain / (1 << 16) * (1<<8));
}