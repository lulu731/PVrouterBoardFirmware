#include "adc_functions.h"

#include "calibration_helpers.h"
#include "adc_rw.h"
#include "adc_registers.h"

#include <assert.h>

/**
 * @brief calculates the average of adc register values
 * @param reg pointer to adc register to read from
 * @param nbr_datas number of times to read from adc register
 * @return average of adc register values
 */
adc_data get_average_data_fm_register(struct adc_register* reg, const int nbr_datas)
{
    adc_data data_array[nbr_datas];

    for (uint8_t i = 0; i < nbr_datas; i++)
    {
        read_adc_register(reg);
        data_array[i] = (*reg).data;
    }

    return get_average_data_in_array(data_array, nbr_datas);
}

/**
 * @brief calculates the complement average of adc register values
 * @param source_reg pointer to adc register to read from
 * @param nbr_datas number of times to read from adc register
 * @return complement average of adc register values
 */
adc_data get_complement_average_fm_register(struct adc_register* source_reg, const int nbr_datas)
{
    adc_data average_value = get_average_data_fm_register(source_reg, nbr_datas);
    return ~average_value + 1;
}

extern uint16_t Mc, Un, Ib, Gl, Vl, Vu;
extern struct adc_register PL_CONST_H, PL_CONST_L;

void write_PL_constant() //21 - 22H
{
    assert(Mc != 0 && Un != 0 && Ib != 0);

    const float pl_float = 838860800.0f * (float)(Gl * Vl * Vu) / (float)(Mc * Un * Ib);
    uint32_t pl_const = pl_float;

    int8_t mod = pl_const % 4;
    if ( mod > 2)
        pl_const = pl_const + 4 - mod;
    else
        pl_const = pl_const - mod;

    uint16_t data[2];
    split_uint32_to_uint8_array(pl_const, data);

    PL_CONST_H.data = data[0];
    PL_CONST_L.data = data[1];
    write_adc_register(PL_CONST_H);
    write_adc_register(PL_CONST_L);
}

void write_gain_register(const struct adc_register gain_register)
{
    assert(gain_register.address == 0x23 || gain_register.address == 0x25);
    write_adc_register(gain_register);
}

void write_phi_register(const struct adc_register gain_register)
{
    assert(gain_register.address == 24 || gain_register.address == 26);
    write_adc_register(gain_register);
}

uint8_t K = 1;

void write_threshold_register(struct adc_register threshold_register)
{

    assert(threshold_register.address == 0x27 || threshold_register.address == 0x28 ||
           threshold_register.address == 0x29 || threshold_register.address == 0x2A);

    threshold_register.data = get_threshold(K);
    write_adc_register(threshold_register);
}

extern uint16_t Lgain, Ngain, LNsel, DisHPF, Amod, Rmod, Zxcon, Pthresh;
extern struct adc_register MMODE;
void write_MMODE() // 2BH
{
    MMODE.data = get_mmode_value(Lgain, Ngain, LNsel, DisHPF, Amod, Rmod, Zxcon, Pthresh);
    write_adc_register(MMODE);
}

/**
 * @brief write calculated gain based on measured and expected values to gain address
 *
 * @param expected_value expected value of measured value
 * @param measured_value_address address of measured value
 * @param gain_address address of gain value
 */

 /*---------------gain------------------*/
extern struct adc_register U_RMS;
 adc_data get_line_gain(const float expected_value, struct adc_register* measured_value_register,
                       struct adc_register* gain_register)
{
    read_adc_register(measured_value_register);

    read_adc_register(gain_register);
    adc_data old_gain = (*gain_register).data;

    float divider = 1000;
    if ((*measured_value_register).address == U_RMS.address)
        divider = 100;

    const float float_measured_value = (*measured_value_register).data / divider;

    return old_gain * expected_value / float_measured_value;
}


/*---------------ofsset------------------*/

adc_data get_offset(struct adc_register reg, const adc_data gain)
{
    read_adc_register(&reg);
    return ~get_offset_from_measured(reg.data, gain) + 1;
}

adc_data get_power_offset(struct adc_register* power_register)
{
    return get_average_data_fm_register(power_register, 5);
}