/*
 * Copyright (c) 2023, Fabiola de las Casas Escardo <fabioladelascasas@gmail.com>
 * All rights reserved.
 * License: bsd-3-clause (see LICENSE.txt)
 * Version: 1.0
 * 
 */

/*******************************************************************************
 * Includes
 ******************************************************************************/
#include "protocol.h"
#include "crc.h"
//#include "uartDriver.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
static int add_CRC(uint8_t *buff);
static uint8_t check_CRC(uint8_t *buff);
/*******************************************************************************
 * Variables
 ******************************************************************************/

/*******************************************************************************
 * Code - private
 ******************************************************************************/
static int add_CRC(uint8_t *buff)
{
    buff[sizeof(protocol_frame_t)] = crc_calc(0, (uint8_t *)buff, sizeof(protocol_frame_t));
}

static uint8_t check_CRC(uint8_t *buff)
{
    return crc_calc(0, (uint8_t *)buff, sizeof(protocol_frame_t));
}

/*******************************************************************************
 * Code - public
 ******************************************************************************/

// Function to set the data on the frame
void protocol_setFrame(protocol_frame_t *frame, uint8_t address, uint8_t actionCode, uint8_t flagStatus, 
                        uint16_t temp, uint16_t hum) 
{
    frame->address = address;
    frame->actionCode = actionCode;
    frame->flagStatus = flagStatus;
    frame->payload_temp = temp;
    frame->payload_hum = hum;
}

// Function to build the data buffer to send
uint16_t protocol_buildFrame(uint8_t *data_buff, size_t data_buff_size, protocol_frame_t *frame)
{
    // Comprueba para asegurarse de que el búfer de datos sea lo suficientemente grande
    if (data_buff_size < sizeof(protocol_frame_t)) 
    {
        return 0;
    }

    // Copia el marco de datos al búfer de datos, teniendo en cuenta los campos bit
    memcpy(data_buff, frame, sizeof(protocol_frame_t));

    add_CRC(data_buff);

    // Devuelve la longitud del marco de datos
    return sizeof(protocol_frame_t);
}
                
crc_status_t protocol_readFrame(uint8_t *data_buff, protocol_frame_t *frame)
{
    uint8_t crc;

    // Asigna los valores de los campos a la variable de estructura
    frame->address = data_buff[0];
    frame->actionCode = data_buff[1] & 0x0F;
    frame->flagStatus = (data_buff[1] >> 4) & 0x0F;
    frame->payload_temp = (data_buff[3] << 8) | data_buff[2];
    frame->payload_hum = (data_buff[5] << 8) | data_buff[4];

    crc = check_CRC(data_buff);

    if(crc == data_buff[6])
    {
        return CRC_OK;
    }
    else
    {
        return CRC_ERROR;
    }
}