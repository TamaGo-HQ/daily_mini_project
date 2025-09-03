#ifndef UART_H
#define UART_H

#include <stdint.h>

/* ------------------- Error Types ------------------- */
typedef enum
{
    UART_NO_ERROR = 0,
    UART_FRAMING_ERROR,
    UART_PARITY_ERROR, 
    UART_OVERRUN_ERROR,
    UART_BUFFER_FULL
} UART_Error_t;

/* ------------------- Data Bits ------------------- */
typedef enum
{
    UART_DATA_7_BITS = 7,
    UART_DATA_8_BITS = 8
} UART_DataBits_t;

/* ------------------- Stop Bits ------------------- */
typedef enum
{
    UART_STOP_1_BIT = 1,
    UART_STOP_2_BITS = 2
} UART_StopBits_t;

/* ------------------- Baud Rates ------------------- */
typedef enum
{
    UART_BAUD_9600   = 9600,
    UART_BAUD_19200  = 19200,
    UART_BAUD_38400  = 38400,
    UART_BAUD_57600  = 57600,
    UART_BAUD_115200 = 115200
} UART_BAUD;

/* ------------------- Parity Options ------------------- */
typedef enum
{
    UART_PARITY_NONE = 0,
    UART_PARITY_EVEN,
    UART_PARITY_ODD
} UART_PARITY;

/* ------------------- Configuration Structure ------------------- */
typedef struct
{
    UART_BAUD baud_rate;
    UART_PARITY parity;
    UART_DataBits_t data_bits;
    UART_StopBits_t stop_bits;
} UART_Config_t;

/* ------------------- Hardware-like Registers ------------------- */
typedef struct
{
    uint8_t DR;     // Data Register (read RX, write TX)
    uint8_t SR;     // Status Register (flags)
    uint8_t CR1;    // Control Register 1 (enable bits, config)
    uint8_t CR2;    // Control Register 2 (additional config)
    uint8_t BRR;    // Baud Rate Register
} UART_Registers_t;

/* ------------------- UART Handle Structure ------------------- */
typedef struct
{
    UART_Registers_t* registers;
    UART_Config_t config;
    // TODO: Add buffers, timing, interrupt flags
} UART_Handle_t;

/* Initialize UART peripheral with the specified configuration */
void UART_Init(UART_Handle_t* huart, UART_Config_t* config);

/* Transmit a single byte over UART */
void UART_SendByte(UART_Handle_t* huart, uint8_t data);

/* Receive a single byte from UART (blocking) */
uint8_t UART_ReceiveByte(UART_Handle_t* huart);

/* Check if data is available in the receive buffer */
uint8_t UART_IsDataReady(UART_Handle_t* huart);

/* Check if UART is ready to transmit a new byte */
uint8_t UART_IsTxEmpty(UART_Handle_t* huart);

/* Get the current UART error status flags */
UART_Error_t UART_GetError(UART_Handle_t* huart);

/* Clear UART error flags */
void UART_ClearError(UART_Handle_t* huart);

/*Send a buffer of bytes (non-blocking or blocking) */
void UART_SendBuffer(UART_Handle_t* huart, const uint8_t* buffer, uint16_t length);

/* Receive a buffer of bytes (non-blocking or blocking) */
uint16_t UART_ReceiveBuffer(UART_Handle_t* huart, uint8_t* buffer, uint16_t max_length);

#endif

