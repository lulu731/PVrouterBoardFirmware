#include "spi_master.h"
#include "spi_config.h"
#include "spi_functions.h"
#include "nvs_params.h"
#include "adc_registers.h"

#include "esp_log.h"

static const char TAG[] = "spi_master.c";

spi_device_handle_t meter_handle;

/***********************/
/* READ and WRITE data */
/***********************/

esp_err_t read_adc_register(struct adc_register reg)
{
    spi_transaction_t trans = {
        .addr = reg.address | MSB_READ_ADDR_MASK,
        .length = 24,
        .rxlength = 16,
        .flags = SPI_TRANS_USE_RXDATA,
    };

    esp_err_t ret = spi_device_polling_transmit( meter_handle, &trans);
    if (ret ==ESP_OK)
        reg.data = (adc_data)trans.rx_data[0] << 8 | (adc_data)trans.rx_data[1];

    return ret;
}

esp_err_t write_adc_register(const struct adc_register reg)
{
    spi_transaction_t trans = {
        .addr = reg.address,
        .length = 16,
        .flags = SPI_TRANS_USE_TXDATA,
    };
    set_txdata_with(trans.tx_data, reg.data);

    return spi_device_polling_transmit( meter_handle, &trans);
}

//todo: manage errors in writing
/***********************/
/*        END          */
/* READ and WRITE data */
/***********************/


adc_data get_average_data_fm_register(struct adc_register reg, const int nbr_datas)
{
    adc_data data_array[nbr_datas];

    for (uint8_t i = 0; i < nbr_datas; i++)
    {
        read_adc_register(reg);
        data_array[i] = reg.data;
    }

    return get_average_data_in_array(data_array, nbr_datas);
}

adc_data get_complement_average_fm_register(struct adc_register source_reg, const int nbr_datas)
{
    adc_data average_value = get_average_data_fm_register(source_reg, nbr_datas);
    return ~average_value + 1;
}


/************************/
/* Metering calibration */
/************************/

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

/// set calibration gains and angles to 0
extern struct adc_register L_GAIN, L_PHI, N_GAIN, N_PHI;

void write_gain_L() // 23H
{
    L_GAIN.data = 0;
    write_adc_register(L_GAIN);
}

void write_phi_L() // 24H
{
    L_PHI.data = 0;
    write_adc_register(L_PHI);
}

void write_gain_N() // 25H
{
    N_GAIN.data = 0;
    write_adc_register(N_GAIN);
}

void write_phi_N() // 26H
{
    N_PHI.data = 0;
    write_adc_register(N_PHI);
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

void exec_metering_calibration()
{
    extern struct adc_register CAL_START;
    extern adc_data CAL_NEEDED, CAL_END;

    CAL_START.data = CAL_NEEDED;
    write_adc_register(CAL_START); //start calibration

    write_PL_constant();

    write_gain_L();
    write_phi_L();
    write_gain_N();
    write_phi_N();

    write_PStartTh();
    write_PNolTh();
    write_QStartTh();
    write_QNolTh();

    write_MMODE();

    ///update CS1 register
    extern struct adc_register CS1;
    read_adc_register(CS1);
    write_adc_register(CS1);

    CAL_START.data = CAL_END;
    write_adc_register(CAL_START); //end calibration
}

/************************/
/*         END          */
/* Metering calibration */
/************************/


/***********************/
/* Measure calibration */
/***********************/

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
    read_adc_register(measured_value_register);

    read_adc_register(gain_register);
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
    read_adc_register(reg);
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

void exec_gain_calibration()
{
    get_Ugain(); // needs measure at 230V
    get_IgainL(); // needs measure at Ib
    get_IgainN(); // needs measure at Ib*/
}

void exec_offset_calibration() // needs measure no current
{
    get_Uoffset();
    get_IoffsetL();
    get_IoffsetN();

    extern struct adc_register SMALL_P_MOD;
    SMALL_P_MOD.data = 0xA987;
    write_adc_register(SMALL_P_MOD); // small power mode
    get_PQoffsetL();
    get_PQoffsetN();
    SMALL_P_MOD.data = 0xA980;
    write_adc_register(SMALL_P_MOD);
}

void exec_measurement_calibration()
{
    extern struct adc_register ADJ_START;
    extern adc_data CAL_NEEDED, CAL_END;

    ADJ_START.data = CAL_NEEDED;
    write_adc_register(ADJ_START); //start calibration

    #ifdef CALIBRATION_AT_IB
        exec_gain_calibration();
        save_nvs_param("Ugain", Ugain);
        save_nvs_param("IgainL", IgainL);
        save_nvs_param("IgainN", IgainN);
    #endif

    write_Ugain();
    write_IgainL();
    write_IgainN();


    #ifdef CALIBRATION_NO_CURRENT
        exec_offset_calibration(); //
        save_nvs_param("Uoffset", Uoffset);
        save_nvs_param("IoffsetL", IoffsetL);
        save_nvs_param("IoffsetN", IoffsetN);
        save_nvs_param("PoffsetL", PoffsetL);
        save_nvs_param("QoffsetL", QoffsetL);
        save_nvs_param("PoffsetN", PoffsetN);
        save_nvs_param("QoffsetN", QoffsetN);
    #endif

    write_Uoffset();
    write_IoffsetL();
    write_IoffsetN();
    write_PQoffsetL();
    write_PQoffsetN();

    ///update CS2 register
    extern struct adc_register CS2;
    read_adc_register(CS2);
    write_adc_register(CS2);

    ADJ_START.data = CAL_END;
    write_adc_register(ADJ_START); //end calibration
}

/***********************/
/*       END           */
/* Measure calibration */
/***********************/


void exec_calibration(void)
{
    exec_metering_calibration();
    exec_measurement_calibration();
}

esp_err_t calibrate_adc()
{
    esp_err_t ret;

    ESP_LOGI(TAG, "Initializing bus SPI%d...", EMETER_HOST + 1);
    //Initialize the SPI bus
    ret = spi_bus_initialize(EMETER_HOST, &buscfg, SPI_DMA_CH_AUTO);
    ESP_ERROR_CHECK(ret);


    ESP_LOGI(TAG, "Initializing meter...");
    ret = spi_bus_add_device( EMETER_HOST, &devcfg, &meter_handle);
    ESP_ERROR_CHECK(ret);

    exec_calibration();

    return ret;//todo: return error?
}