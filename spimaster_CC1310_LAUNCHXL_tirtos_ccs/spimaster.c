#include <stdlib.h>
#include <unistd.h>
#include <ti/drivers/rf/RF.h>
#include <ti/drivers/PIN.h>
#include <ti/drivers/pin/PINCC26XX.h>
#include <ti/drivers/GPIO.h>
#include DeviceFamily_constructPath(driverlib/rf_prop_mailbox.h)
#include <stddef.h>
#include <stdint.h>
#include <string.h>
#include <stdio.h>
#include <ti/drivers/SPI.h>

#include <pthread.h>
#include <semaphore.h>

#include "Board.h"

#define THREADSTACKSIZE (1024)

uint16_t masterRxBuffer[1];
uint16_t masterTxBuffer[1];

#define n_samples 1000
double voltage_readings[n_samples];

static PIN_Handle ledPinHandle;

/* Global memory storage for a PIN_Config table */
static PIN_State ledPinState;

/*
 * Initial LED pin configuration table
 *   - LEDs Board_PIN_LED0 is on.
 *   - LEDs Board_PIN_LED1 is off.
 */
PIN_Config ledPinTable[] = {
    Board_PIN_LED0 | PIN_GPIO_OUTPUT_EN | PIN_GPIO_HIGH | PIN_PUSHPULL | PIN_DRVSTR_MAX,
    Board_PIN_LED1 | PIN_GPIO_OUTPUT_EN | PIN_GPIO_LOW  | PIN_PUSHPULL | PIN_DRVSTR_MAX,
      PIN_TERMINATE
};


void send_spi_command(uint16_t command, SPI_Handle masterSpi, SPI_Transaction transaction){
    /* Upload command value */
    masterTxBuffer[0] = command;

    /* Initialize master SPI transaction structure */
    memset((void *) masterRxBuffer, 0, 1);
    transaction.count = 1;
    transaction.txBuf = (void *) masterTxBuffer;
    transaction.rxBuf = (void *) masterRxBuffer;

    /* Perform SPI transfer */
    //printf("Sending command: %i, ", masterTxBuffer[0]);
    bool transferOK;

    /*Set CSn to low */
    GPIO_write(IOID_11, 0);

    transferOK = SPI_transfer(masterSpi, &transaction);
    if (transferOK) {
        printf("Received: %i \n", masterRxBuffer[0]);
        /*Set CSn to high */
        GPIO_write(IOID_11, 1);
        usleep(100);
        return;
    }
    else {
        printf("Unsuccessful master SPI transfer");
        while(1);
    }

}

int16_t convert_channel(int channel, SPI_Handle masterSpi, SPI_Transaction transaction){
    /* CONVERT(n) command */
    masterTxBuffer[0] = 0x0000 | (uint16_t)channel << 8;

    /* Initialize master SPI transaction structure */
    memset((void *) masterRxBuffer, 0, 1);
    transaction.count = 1;
    transaction.txBuf = (void *) masterTxBuffer;
    transaction.rxBuf = (void *) masterRxBuffer;

    /* Perform SPI transfer */
    //printf("Sending: %i \n", masterTxBuffer[0]);
    bool transferOK;

    /*Set CSn to low */
    GPIO_write(IOID_11, 0);

    transferOK = SPI_transfer(masterSpi, &transaction);
    if (transferOK) {
        /*Set CSn to high */
        GPIO_write(IOID_11, 1);
        return(masterRxBuffer[0]);
    }
    else {
        printf("Unsuccessful SPI transfer");
        while(1);
    }
}

void *masterThread(void *arg0)
{
    SPI_Handle      masterSpi;
    SPI_Params      spiParams;
    SPI_Transaction transaction;

    /* Open SPI as master (default) */
    SPI_Params_init(&spiParams);
    spiParams.frameFormat = SPI_POL0_PHA0; //CPOL=0 and CPHA=0 required by RHD2132
    spiParams.bitRate = 4000000;
    spiParams.dataSize = 16;
    spiParams.transferMode = SPI_MODE_BLOCKING;
    masterSpi = SPI_open(Board_SPI_MASTER, &spiParams);
    if (masterSpi == NULL) {
        printf("Error initializing master SPI\n");
        while (1);
    }
    else {
        printf("SPI initialized\n");
    }

    /* Verify the SPI connection by reading registers 40 - 44 */
    //printf("Send two dummy commands to prepare the IC\n");
    send_spi_command(0b1111111100000000, masterSpi, transaction);
    send_spi_command(0b1111111100000000, masterSpi, transaction);

    //printf("Reading INTAN from registers 40-44\n");
    send_spi_command(0b1110100000000000, masterSpi, transaction);
    send_spi_command(0b1110100100000000, masterSpi, transaction);
    send_spi_command(0b1110101000000000, masterSpi, transaction);
    send_spi_command(0b1110101100000000, masterSpi, transaction);
    send_spi_command(0b1110110000000000, masterSpi, transaction);

    /* Write to Register 0: ADC Configuration and Amplifier Fast Settle */
    /* Nominal settings from datasheet */
    send_spi_command(0b1000000011011110, masterSpi, transaction);

    /* Write to Register 1:  Supply Sensor and ADC Buffer Bias Current */
    /* ADC buffer bias  =  32 because we're using the lowest sampling rate*/
    send_spi_command(0b1000000100000010, masterSpi, transaction);

    /* Write to Register 2: MUX Bias Current */
    /* Nominal settings from datasheet */
    send_spi_command(0b1000001000000100, masterSpi, transaction);

    /* Write to Register 3: MUX Load, Temperature Sensor, and Auxiliary Digital Output */
    /* Disable temperature sensor and digital output*/
    send_spi_command(0b1000001100000010, masterSpi, transaction);

    /* Write to Register 4: ADC Output Format and DSP Offset Removal */
    /* Drive MISO to 1 when idle, use 2s complement, don't use absolute value, disable DSP filtering */
    send_spi_command(0b100001001110000, masterSpi, transaction);

    /* Write to Register 5: Impedance Check Control */
    /* No electrode calibration using a DAC waveform generator */
    send_spi_command(0b1000010100000000, masterSpi, transaction);

    /* Write to Register 6: Impedance Check DAC */
    /* N/A since not using DAC */
    send_spi_command(0b1000011000000000, masterSpi, transaction);

    /* Write to Register 7: Impedance Check Amplifier Select */
    /* N/A since not using DAC */
    send_spi_command(0b1000011100000000, masterSpi, transaction);

    /* Write to Registers 8-13: On-Chip Amplifier Bandwidth Select */
    /* Upper bandwidth = 300 Hz, lower bandwidth = 1 Hz */
    send_spi_command(0b1000100000000110, masterSpi, transaction);
    send_spi_command(0b1000100100001001, masterSpi, transaction);
    send_spi_command(0b1000101000000010, masterSpi, transaction);
    send_spi_command(0b1000101100001011, masterSpi, transaction);
    send_spi_command(0b1000110000111110, masterSpi, transaction);
    send_spi_command(0b1000110100000000, masterSpi, transaction);

    /* Write to Registers 14-17: : Individual Amplifier Power */
    /* Power down amplifiers 8-31 */
    send_spi_command(0b1000111011111111, masterSpi, transaction);
    send_spi_command(0b1000111100000000, masterSpi, transaction);
    send_spi_command(0b1001000000000000, masterSpi, transaction);
    send_spi_command(0b1001000100000000, masterSpi, transaction);

    /* Calibrate */
    send_spi_command(0b0101010100000000, masterSpi, transaction);

    /* Nine dummy commands after calibration*/
    send_spi_command(0b1111111100000000, masterSpi, transaction);
    send_spi_command(0b1111111100000000, masterSpi, transaction);
    send_spi_command(0b1111111100000000, masterSpi, transaction);
    send_spi_command(0b1111111100000000, masterSpi, transaction);
    send_spi_command(0b1111111100000000, masterSpi, transaction);
    send_spi_command(0b1111111100000000, masterSpi, transaction);
    send_spi_command(0b1111111100000000, masterSpi, transaction);
    send_spi_command(0b1111111100000000, masterSpi, transaction);
    send_spi_command(0b1111111100000000, masterSpi, transaction);

    /* Wait 3s for the chip to stabilize*/
    sleep(3);

    /*Start sampling*/
    int f_sample = 500;
    int delay = (int)1000000*(1/(double)f_sample);
    #ifdef RECORD_WAVEFORM
        int i = 0;
        for(i = 0; i< n_samples; i++){
            voltage_readings[i] = 0.195*convert_channel(0, masterSpi, transaction);
            //PIN_setOutputValue(ledPinHandle, Board_PIN_LED0, 0);
            //PIN_setOutputValue(ledPinHandle, Board_PIN_LED1, 1);
            usleep(delay);
        }
    #else
        double v = 0;
        while(1){
            v = 0.195*convert_channel(0, masterSpi, transaction);
            PIN_setOutputValue(ledPinHandle, Board_PIN_LED0, 0);
            PIN_setOutputValue(ledPinHandle, Board_PIN_LED1, 1);
            if(v>1500){
                PIN_setOutputValue(ledPinHandle, Board_PIN_LED0, 1);
                PIN_setOutputValue(ledPinHandle, Board_PIN_LED1, 0);
            }
            usleep(delay);
        }
    #endif

    SPI_close(masterSpi);
    printf("Data acquisition complete \n");

    return (NULL);
}

void *mainThread(void *arg0)
{
    ledPinHandle = PIN_open(&ledPinState, ledPinTable);
    if(!ledPinHandle) {
        /* Error initializing board LED pins */
        while(1);
    }
    pthread_t           thread0;
    pthread_attr_t      attrs;
    struct sched_param  priParam;
    int                 retc;
    int                 detachState;

    GPIO_init();
    SPI_init();

    //PIN_setOutputValue(ledPinHandle, Board_PIN_LED0, 0);
    //PIN_setOutputValue(ledPinHandle, Board_PIN_LED1, 1);
    //PIN_setOutputValue(ledPinHandle, Board_PIN_PA, 0);

    printf("Initializing Intan RHD2132 \n");

    /* Create application threads */
    pthread_attr_init(&attrs);

    detachState = PTHREAD_CREATE_DETACHED;
    /* Set priority and stack size attributes */
    retc = pthread_attr_setdetachstate(&attrs, detachState);
    if (retc != 0) {
        /* pthread_attr_setdetachstate() failed */
        while (1);
    }

    retc |= pthread_attr_setstacksize(&attrs, THREADSTACKSIZE);
    if (retc != 0) {
        /* pthread_attr_setstacksize() failed */
        while (1);
    }

    /* Create master thread */
    priParam.sched_priority = 1;
    pthread_attr_setschedparam(&attrs, &priParam);

    retc = pthread_create(&thread0, &attrs, masterThread, NULL);
    if (retc != 0) {
        /* pthread_create() failed */
        while (1);
    }

    return (NULL);
}
