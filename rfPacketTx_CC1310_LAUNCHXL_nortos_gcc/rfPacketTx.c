#include <stdlib.h>
#include <unistd.h>
#include <ti/drivers/rf/RF.h>
#include <ti/drivers/PIN.h>
#include <ti/drivers/pin/PINCC26XX.h>
#include <ti/drivers/GPIO.h>
#include <ti/drivers/SPI.h>
#include <stddef.h>
#include <stdint.h>
#include <string.h>
#include <stdio.h>
#include DeviceFamily_constructPath(driverlib/rf_prop_mailbox.h)

#include "Board.h"
#include "smartrf_settings/smartrf_settings.h"

#define PAYLOAD_LENGTH      32
#define PACKET_INTERVAL     1000000  /* Set packet interval to 1s */

static RF_Object rfObject;
static RF_Handle rfHandle;
static uint8_t packet[PAYLOAD_LENGTH];

/* Uncomment to enable continuous transmission for power measurements */
//#define CONTINOUS_TX

void *mainThread(void *arg0){
    RF_Params rfParams;
    RF_Params_init(&rfParams);

    /* set the PA bias to 3.3V */
    GPIO_setConfig(Board_DIO0, GPIO_CFG_OUT_STD | GPIO_CFG_OUT_LOW);
    GPIO_write(Board_DIO0, 0);

    GPIO_setConfig(IOID_3, GPIO_CFG_OUT_STD | GPIO_CFG_OUT_LOW);
    GPIO_write(IOID_3, 0);
    sleep(1);
    GPIO_write(IOID_3, 1);
    sleep(1);
    GPIO_write(IOID_3, 0);
    sleep(1);

    #ifdef CONTINOUS_TX
        /* use unmodulated CW */
        RF_cmdTxTest.config.bUseCw = 1;
        /* Request access to the radio */
        rfHandle = RF_open(&rfObject, &RF_prop, (RF_RadioSetup*)&RF_cmdPropRadioDivSetup, &rfParams);
        /* Send CMD_FS and wait until it has completed */
        RF_runCmd(rfHandle, (RF_Op*)&RF_cmdFs, RF_PriorityNormal, NULL, 0);
        /* Activate the CMX901 PA */
        GPIO_write(Board_DIO0, 1);
        /* Send CMD_TX_TEST which sends forever */
        RF_runCmd(rfHandle, (RF_Op*)&RF_cmdTxTest, RF_PriorityNormal, NULL, 0);
        /* Should never come here */
        while (1);
    #else

    RF_cmdPropTx.pktLen = PAYLOAD_LENGTH;
    RF_cmdPropTx.pPkt = packet;
    RF_cmdPropTx.startTrigger.triggerType = TRIG_NOW;

    rfHandle = RF_open(&rfObject, &RF_prop, (RF_RadioSetup*)&RF_cmdPropRadioDivSetup, &rfParams);

    /* Set the frequency */
    RF_postCmd(rfHandle, (RF_Op*)&RF_cmdFs, RF_PriorityNormal, NULL, 0);

    while(1){
        /* Fill the packet with 1s to obtain an uninterrupted RF pulse */
        uint8_t i;
        for (i = 0; i < PAYLOAD_LENGTH; i++){
            packet[i] = 0xFF;
        }

        /* Activate the CMX901 PA */
        GPIO_write(Board_DIO0, 1);

        /* Send packet */
        RF_EventMask terminationReason = RF_runCmd(rfHandle, (RF_Op*)&RF_cmdPropTx, RF_PriorityNormal, NULL, 0);
        switch(terminationReason){
            case RF_EventLastCmdDone:
                // A stand-alone radio operation command or the last radio operation command in a chain finished.
                break;
            case RF_EventCmdCancelled:
                // Command cancelled before it was started; it can be caused by RF_cancelCmd() or RF_flushCmd().
                break;
            case RF_EventCmdAborted:
                // Abrupt command termination caused by RF_cancelCmd() or RF_flushCmd().
                break;
            case RF_EventCmdStopped:
                // Graceful command termination caused by RF_cancelCmd() or RF_flushCmd().
                break;
            default:
                // Uncaught error event
                while(1);
        }
        /* Deactivate the CMX901 PA */
        GPIO_write(Board_DIO0, 0);

        uint32_t cmdStatus = ((volatile RF_Op*)&RF_cmdPropTx)->status;
        switch(cmdStatus){
            case PROP_DONE_OK:
                // Packet transmitted successfully
                break;
            case PROP_DONE_STOPPED:
                // received CMD_STOP while transmitting packet and finished transmitting packet
                break;
            case PROP_DONE_ABORT:
                // Received CMD_ABORT while transmitting packet
                break;
            case PROP_ERROR_PAR:
                // Observed illegal parameter
                break;
            case PROP_ERROR_NO_SETUP:
                // Command sent without setting up the radio in a supported mode using CMD_PROP_RADIO_SETUP or CMD_RADIO_SETUP
                break;
            case PROP_ERROR_NO_FS:
                // Command sent without the synthesizer being programmed
                break;
            case PROP_ERROR_TXUNF:
                // TX underflow observed during operation
                break;
            default:
                // Uncaught error event - these could come from the pool of states defined in rf_mailbox.h
                while(1);
        }

        /* Power down the radio */
        RF_yield(rfHandle);



        /* Sleep for PACKET_INTERVAL us */
        usleep(PACKET_INTERVAL);
    }
    #endif
}
