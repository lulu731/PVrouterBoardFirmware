#include "adc_functions.h"

#include "spi_functions.h"
#include "adc_rw.h"
#include "adc_registers.h"

adc_data get_average_data_fm_register(struct adc_register reg, const int nbr_datas)
{
    adc_data data_array[nbr_datas];

    for (uint8_t i = 0; i < nbr_datas; i++)
    {
        read_adc_register(&reg);
        data_array[i] = reg.data;
    }

    return get_average_data_in_array(data_array, nbr_datas);
}

adc_data get_complement_average_fm_register(struct adc_register source_reg, const int nbr_datas)
{
    adc_data average_value = get_average_data_fm_register(source_reg, nbr_datas);
    return ~average_value + 1;
}

extern uint16_t Mc, Un, Ib, Gl, Vl, Vu;

void write_PL_constant() //21 - 22H
{
    const float pl_float = 838860800 * (float)(Gl * Vl *Vu) / (float)(Mc * Un * Ib);
    uint32_t pl_const = pl_float;

    int8_t mod = pl_const % 4;
    if ( mod > 2)
        pl_const = pl_const + 4 - mod;
    else
        pl_const = pl_const - mod;

    uint16_t data[2];
    split_uint32_to_uint8_array(pl_const, data);

    extern struct adc_register PL_CONST_H, PL_CONST_L;
    PL_CONST_H.data = data[0];
    PL_CONST_L.data = data[1];
    write_adc_register(PL_CONST_H);
    write_adc_register(PL_CONST_L);
}

void write_gain_register(const struct adc_register gain_register)
{
    write_adc_register(gain_register);
}

void write_phi_register(const struct adc_register gain_register)
{
    write_adc_register(gain_register);
}

uint8_t K = 1;

extern struct adc_register P_START_TH, P_NO_L_TH, Q_START_TH, Q_NO_L_TH;
void write_PStartTh() // 27H
{
    uint16_t PStartTh = get_threshold(K);
    P_START_TH.data = PStartTh;
    write_adc_register(P_START_TH);
}

void write_PNolTh() // 28H
{
    uint16_t PNolTh = get_threshold(K);
    P_NO_L_TH.data = PNolTh;
    write_adc_register(P_NO_L_TH);
}

void write_QStartTh() // 29H
{
    uint16_t QStartTh = get_threshold(K);
    Q_START_TH.data = QStartTh;
    write_adc_register(Q_START_TH);
}

void write_QNolTh() // 2AH
{
    uint16_t QNolTh = get_threshold(K);
    Q_NO_L_TH.data = QNolTh;
    write_adc_register(Q_NO_L_TH);
}

extern uint16_t Lgain, Ngain, LNsel, DisHPF, Amod, Rmod, Zxcon, Pthresh;
void write_MMODE() // 2BH
{
    extern struct adc_register MMODE;
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

 adc_data get_gain(const float expected_value, struct adc_register measured_value_register,
                     struct adc_register gain_register)
{
    read_adc_register(&measured_value_register);

    read_adc_register(&gain_register);
    adc_data old_gain = gain_register.data;

    extern struct adc_register U_RMS;
    float divider = 1000;
    if (measured_value_register.address == U_RMS.address)
        divider = 100;

    const float float_measured_value = measured_value_register.data / divider;

    return old_gain * expected_value / float_measured_value;
}

extern uint16_t Un, Ib;
extern uint16_t Ugain, IgainL, IgainN, Uoffset, IoffsetL, IoffsetN;
extern struct adc_register U_GAIN, I_GAIN_L, I_GAIN_N, U_OFFSET, I_OFFSET_L, I_OFFSET_N;
extern struct adc_register U_RMS, I_RMS, I_RMS_2;

/*---------------gain------------------*/

void get_Ugain()
{
    Ugain = get_gain(Un, U_RMS, U_GAIN);
}

void write_Ugain() // 31H
{
    U_GAIN.data = Ugain;
    write_adc_register(U_GAIN);
}

void get_IgainL()
{
    IgainL = get_gain(Ib, I_RMS, I_GAIN_L);
}

void write_IgainL() // 32H
{
    I_GAIN_L.data = IgainL;
    write_adc_register(I_GAIN_L);
}

void get_IgainN()
{
    IgainN = get_gain(Ib, I_RMS_2, I_GAIN_N);
}

void write_IgainN() // 33H
{
    I_GAIN_N.data = IgainN;
    write_adc_register(I_GAIN_N);
}

/*---------------ofsset------------------*/

uint16_t get_offset(struct adc_register reg, const uint16_t gain)
{
    read_adc_register(&reg);
    return ~get_offset_from_measured(reg.data, gain) + 1;
}

void get_Uoffset()
{
    Uoffset = get_offset(U_RMS, Ugain);
}

void write_Uoffset() // 34H
{
    U_OFFSET.data = Uoffset;
    write_adc_register(U_OFFSET);
}

void get_IoffsetL()
{
    IoffsetL = get_offset(I_RMS, IgainL);
}

void write_IoffsetL() // 35H
{
    I_OFFSET_L.data = IoffsetL;
    write_adc_register(I_OFFSET_L);
}

void get_IoffsetN()
{
    IoffsetN = get_offset(I_RMS_2, IgainN);
}
void write_IoffsetN() // 36H
{
    I_OFFSET_N.data = IoffsetN;
    write_adc_register(I_OFFSET_N);
}

extern uint16_t PoffsetL, QoffsetL, PoffsetN, QoffsetN;

void get_PQoffsetL() // 37H 38H
{
    extern struct adc_register P_MEAN, Q_MEAN;
    PoffsetL = get_average_data_fm_register(P_MEAN, 5);
    PoffsetN = get_average_data_fm_register(Q_MEAN, 5);
}

void write_PQoffsetL() // 37H 38H
{
    extern struct adc_register P_OFFSET_L, Q_OFFSET_L;
    P_OFFSET_L.data = PoffsetL;
    Q_OFFSET_L.data = QoffsetL;
    write_adc_register(P_OFFSET_L);
    write_adc_register(Q_OFFSET_L);
}

void get_PQoffsetN()
{
    extern struct adc_register P_MEAN_2, Q_MEAN_2;
    PoffsetN = get_average_data_fm_register(P_MEAN_2, 5);
    QoffsetN = get_average_data_fm_register(Q_MEAN_2, 5);
}

void write_PQoffsetN() // 37H 38H
{
    extern struct adc_register P_OFFSET_N, Q_OFFSET_N;
    P_OFFSET_N.data = PoffsetN;
    Q_OFFSET_N.data = QoffsetN;
    write_adc_register(P_OFFSET_N);
    write_adc_register(Q_OFFSET_N);
}
