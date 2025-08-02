#include "spi_master.h"
#include "spi_config.h"
#include "adc_functions.h"

#include "esp_log.h"

static const char TAG[] = "spi_master.c";

spi_device_handle_t meter_handle;

/************************/
/* Metering calibration */
/************************/

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
    read_adc_register(&CS1);
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
    read_adc_register(&CS2);
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