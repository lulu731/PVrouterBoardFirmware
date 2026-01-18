#include "adc_functions.h"
#include "adc_rw.h"

/************************/
/* Metering calibration */
/************************/

extern struct adc_register L_GAIN, L_PHI, N_GAIN, N_PHI;
extern struct adc_register CAL_START;
extern adc_data CAL_NEEDED, CAL_END;
extern struct adc_register P_START_TH, P_NO_L_TH, Q_START_TH, Q_NO_L_TH;
extern struct adc_register CS1;

/**
 * @brief Executing metering calibration procedure
 *
 * This function is responsible for executing metering calibration procedure.
 * It sets calibration start flag, writes PL constant, sets gains and phases to 0,
 * writes thresholds, MMODE, updates CS1 register and sets calibration end flag.
 */
void exec_metering_calibration()
{
    CAL_START.data = CAL_NEEDED;
    write_adc_register(CAL_START); //start calibration

    write_PL_constant();

    /*L_GAIN.data = 0; // all gains are null
    N_GAIN.data = 0;
    L_PHI.data = 0;
    N_PHI.data = 0;
    write_gain_register(L_GAIN);
    write_gain_register(N_GAIN);*/
    /*write_phi_register(L_PHI);
    write_phi_register(N_PHI);*/

    /*write_threshold_register(P_START_TH);
    write_threshold_register(P_NO_L_TH);
    write_threshold_register(Q_START_TH);
    write_threshold_register(Q_NO_L_TH);*/

    write_MMODE();

    ///update CS1 register
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

extern struct adc_register U_GAIN, I_GAIN_L, I_GAIN_N;
extern struct adc_register U_RMS, I_RMS, I_RMS_2;
extern adc_data Ugain, IgainL, IgainN;
extern adc_data Un, Ib;

/// needs measure at 230V and Ib
void exec_gain_calibration()
{
    Ugain = get_line_gain(Un, &U_RMS, &U_GAIN); // needs measure at 230V
    IgainL = get_line_gain(Ib, &I_RMS, &I_GAIN_L); // needs measure at Ib
    IgainN = get_line_gain(Ib, &I_RMS_2, &I_GAIN_N); // needs measure at Ib
}

extern struct adc_register P_MEAN, Q_MEAN, P_MEAN_2, Q_MEAN_2;

/// needs measure no current
void exec_offset_calibration()
{
    extern struct adc_register SMALL_P_MOD;
    SMALL_P_MOD.data = 0xA987;
    write_adc_register(SMALL_P_MOD); // small power mode

    SMALL_P_MOD.data = 0xA980;
    write_adc_register(SMALL_P_MOD);
}

extern struct adc_register U_OFFSET, I_OFFSET_L, I_OFFSET_N;

/// needs measure no current
void exec_offset_calibration_write()
{
    write_adc_register(U_OFFSET);
    write_adc_register(I_OFFSET_L);
    write_adc_register(I_OFFSET_N);
}

extern struct adc_register P_OFFSET_L, Q_OFFSET_L;
extern struct adc_register P_OFFSET_N, Q_OFFSET_N;

/**
 * @brief Executing measurement calibration
 *
 * This function starts the measurement calibration by setting the ADJ_START register to CAL_NEEDED.
 * Depending on the CALIBRATION_AT_IB macro, it executes gain calibration and saves the results to nvs.
 * Depending on the CALIBRATION_NO_CURRENT macro, it executes offset calibration and saves the results to nvs.
 * Finally, it ends the measurement calibration by setting the ADJ_START register to CAL_END.
 */
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

    // write gains to registers
    U_GAIN.data = Ugain;
    write_adc_register(U_GAIN);
    I_GAIN_L.data = IgainL;
    write_adc_register(I_GAIN_L);
    I_GAIN_N.data = IgainN;
    write_adc_register(I_GAIN_N);

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

/**
 * @brief Executing calibration procedure
 *
 * This function is responsible for executing calibration procedure.
 * It executes metering calibration procedure and measurement calibration procedure.
 */
void exec_calibration(void)
{
    exec_metering_calibration();
    exec_measurement_calibration();
}