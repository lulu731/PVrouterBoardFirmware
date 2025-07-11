#include "spi_master.h"
#include "spi_config.h"
#include "spi_functions.h"
#include "adc_adress.h"

#include "esp_log.h"

static const char TAG[] = "spi_master.c";

spi_device_handle_t meter_handle;

/***********************/
/* READ and WRITE data */
/***********************/
esp_err_t read_data(const adc_address adress, uint16_t* data)
{
    spi_transaction_t trans = {
        .addr = adress | MSB_READ_ADDR_MASK,
        .length = 24,
        .rxlength = 16,
        .flags = SPI_TRANS_USE_RXDATA,
    };

    esp_err_t ret = spi_device_polling_transmit( meter_handle, &trans);
    if (ret ==ESP_OK)
        *data = (uint16_t)trans.rx_data[0] << 8 | (uint16_t)trans.rx_data[1];

    return ret;
}

esp_err_t write_data(const adc_address adress, const uint16_t data)
{
    spi_transaction_t trans = {
        .addr = adress,
        .length = 16,
        .flags = SPI_TRANS_USE_TXDATA,
    };
    set_txdata_with(trans.tx_data, data);

    return spi_device_polling_transmit( meter_handle, &trans);
}

//todo: manage errors in writing
/***********************/
/*        END          */
/* READ and WRITE data */
/***********************/


uint16_t get_average_value_fm_adress(adc_address adress)
{
    uint8_t count = 5;
    uint16_t data_array[count];

    for (uint8_t i = 0; i < count; i++)
    {
        read_data(adress, &data_array[i]);
    }

    return get_average_value(data_array, count);
}


void write_complement_average_to_address(const adc_address source_address,
                                         const adc_address average_address)
{
    uint16_t average_value = get_average_value_fm_adress(source_address);
    write_data(average_address, ~average_value + 1);
}

void write_average_to_address(const adc_address source_address,
                              const adc_address average_address)
{
    uint16_t average_value = get_average_value_fm_adress(source_address);
    write_data(average_address, average_value);
}



/************************/
/* Metering calibration */
/************************/

void write_PL_constant() //21 - 22H
{
    const uint16_t Mc = 1000; //todo: get from nvs
    const uint16_t Un = 230;
    const uint16_t Ib = 10;
    const uint16_t Gl = 1;
    const uint16_t Vl = 167;
    const uint16_t Vu = 529;

    const float pl_float = 838860800 * (float)(Gl * Vl *Vu) / (float)(Mc * Un * Ib);
    uint32_t pl_const = pl_float;

    int8_t mod = pl_const % 4;
    if ( mod > 2)
        pl_const = pl_const + 4 - mod;
    else
        pl_const = pl_const - mod;

    uint16_t data[2];
    split_uint32_to_uint8_array(pl_const, data);

    write_data(PL_CONST_H, data[0]);
    write_data(PL_CONST_L, data[1]);
}

void write_gain_L() // 23H
{
    const uint16_t Lgain = 0; //todo: get fm nvs
    write_data(L_GAIN, Lgain);
}

void write_phi_L() // 24H
{
    const uint16_t Lphi = 0;
    write_data(L_PHI, Lphi);
}

void write_gain_N() // 25H
{
    const uint16_t Ngain = 0; //todo: get fm nvs
    write_data(N_GAIN, Ngain);
}

void write_phi_N() // 26H
{
    const uint16_t Nphi = 0;
    write_data(N_PHI, Nphi);
}


uint8_t K = 1;

void load_param()
{
    Gl =  1;
    Vl =  167;
    Vu =  529;
}

void write_PStartTh() // 27H
{
    uint16_t PStartTh = get_threshold(K);
    write_data(P_START_TH, PStartTh);
}

void write_PNolTh() // 28H
{
    uint16_t PNolTh = get_threshold(K);
    write_data(P_START_TH, PNolTh);
}

void write_QStartTh() // 29H
{
    uint16_t QStartTh = get_threshold(K);
    write_data(Q_START_TH, QStartTh);
}

void write_QNolTh() // 2AH
{
    uint16_t QNolTh = get_threshold(K);
    write_data(P_START_TH, QNolTh);
}

void write_MMODE() // 2BH
{
    //todo: get values frmo nvs
    uint8_t Lgain = 0b100;
    uint8_t Ngain = 0;
    uint8_t LNsel = 0;
    uint8_t DisHPF = 0;
    uint8_t Amod = 0;
    uint8_t Rmod = 0;
    uint8_t Zxcon = 0b10;
    uint8_t Pthresh = 0b10;
    write_data(MMODE, get_mmode_value(Lgain, Ngain, LNsel, DisHPF, Amod, Rmod, Zxcon, Pthresh));
}

void exec_metering_calibration()
{
    write_data(CAL_START, CAL_NEEDED); //start calibration

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
    adc_param cs_data;
    read_data(CS1, &cs_data);
    write_data(CS1, cs_data);

    write_data(CAL_START, CAL_END); //end calibration
}

/************************/
/*         END          */
/* Metering calibration */
/************************/


/***********************/
/* Measure calibration */
/***********************/

void write_gain( const float expected_value, const adc_address meas_value_address,
                 const adc_address gain_address)
{
    uint16_t measured_value;
    read_data(meas_value_address, &measured_value);
    uint16_t old_gain;
    read_data(gain_address, &old_gain);
    const float float_measured_value = measured_value / 100;
    write_data(gain_address, old_gain * expected_value / float_measured_value);
}

void write_Ugain() // 31H
{
    const float Un = 230.00;
    write_gain( Un, U_RMS, U_GAIN);
}

void write_IgainL() // 32H
{
    const float Ib = 10.00;
    write_gain( Ib, I_RMS, I_GAIN_L);
}

void write_IgainN() // 33H
{
    const float Ib = 10.00;
    write_gain( Ib, I_RMS_2, I_GAIN_N);
}

void write_Uoffset() // 34H
{
    write_average_to_address(U_RMS, U_OFFSET);
}

void write_IoffsetL() // 35H
{
    write_average_to_address(I_RMS, I_OFFSET_L);
}

void write_IoffsetN() // 36H
{
    write_average_to_address(I_RMS_2, I_OFFSET_N);
}

void write_PQoffsetL() // 37H 38H
{
    write_complement_average_to_address(P_MEAN, P_OFFSET_L);
    write_complement_average_to_address(Q_MEAN, Q_OFFSET_L);
}

void write_PQoffsetN()
{
    write_complement_average_to_address(P_MEAN_2, P_OFFSET_N);
    write_complement_average_to_address(Q_MEAN_2, Q_OFFSET_N);
}


void exec_measurement_calibration()
{
    write_data(ADJ_START, CAL_NEEDED); //start calibration

    //V line
    write_Ugain();
    write_Uoffset();

    //L line
    write_IgainL();
    write_IoffsetL();

    //if PoffsetL does not exist in NVS
    write_data(SMALL_P_MOD, 0xA987);
    write_PQoffsetL();
    write_PQoffsetN();
    write_data(SMALL_P_MOD, 0xA980);

    //N line
    write_IgainN();
    write_IoffsetN();

    ///update CS1 register
    adc_param cs_data;
    read_data(CS2, &cs_data);
    write_data(CS2, cs_data);

    write_data(ADJ_START, CAL_END); //end calibration
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

esp_err_t test_spi()
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

    return ESP_FAIL;//todo: to be corrected
}