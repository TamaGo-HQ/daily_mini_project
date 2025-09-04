#ifndef UART_H
#define UART_H

#include <stdint.h>
#include"circular_buffer.h"
#include<stdbool.h>

/* ------------------- Error Types ------------------- */
typedef enum
{
    UART_NO_ERROR = 0,
    UART_ERROR_NULL_POINTER,
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
} UART_Baud_t;

/* ------------------- Parity Options ------------------- */
typedef enum
{
    UART_PARITY_NONE = 0,
    UART_PARITY_EVEN,
    UART_PARITY_ODD
} UART_Parity_t;

/* ------------------- Configuration Structure ------------------- */
typedef struct
{
    UART_Baud_t baud_rate;
    UART_Parity_t parity;
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
    UART_Registers_t registers;
    UART_Config_t config;

    //TX/RX Buffers
    circular_buffer_t tx_buffer;
    circular_buffer_t rx_buffer;

    // Interrupt flags
    bool tx_interrupt_enabled;
    bool rx_interrupt_enabled;
    bool error_interrupt_enabled;

    // Status Flags
    bool tx_busy;
    bool rx_busy;
    UART_Error_t current_error;
} UART_Handle_t;

/* Initialize UART peripheral with the specified configuration */
void UART_Init(UART_Handle_t* huart, UART_Config_t* config);

/* Transmit a single byte over UART */
void UART_SendByte(UART_Handle_t* huart, uint8_t data);

/* Receive a single byte from UART */
uint8_t UART_ReceiveByte(UART_Handle_t* huart);

/* Check if data is available in the receive buffer */
uint8_t UART_IsDataReady(UART_Handle_t* huart);

/* Check if UART is ready to transmit a new byte */
uint8_t UART_IsTxEmpty(UART_Handle_t* huart);

/* Get the current UART error status flags */
UART_Error_t UART_GetError(UART_Handle_t* huart);

/* Clear UART error flags */
void UART_ClearError(UART_Handle_t* huart);

/*Send a buffer of bytes  */
void UART_SendBuffer(UART_Handle_t* huart, const uint8_t* buffer, uint16_t length);

/* Receive a buffer of bytes */
uint16_t UART_ReceiveBuffer(UART_Handle_t* huart, uint8_t* buffer, uint16_t max_length);

/* ------------------- Status Register (SR) Bit Definitions ------------------- */
typedef enum {
    SR_TXE_BIT   = 0,  // Transmit Empty
    SR_RXNE_BIT  = 1,  // Receive Not Empty  
    SR_TC_BIT    = 2,  // Transmission Complete
    SR_PE_BIT    = 3,  // Parity Error
    SR_FE_BIT    = 4,  // Framing Error
    SR_ORE_BIT   = 5   // Overrun Error
} UART_SR_Bits_t;

/* ------------------- Control Register 1 (CR1) Bit Definitions ------------------- */
typedef enum {
    CR1_UE_BIT     = 0,  // UART Enable
    CR1_RE_BIT     = 2,  // Receiver Enable
    CR1_TE_BIT     = 3,  // Transmitter Enable
    CR1_RXNEIE_BIT = 5,  // RX Not Empty Interrupt Enable
    CR1_TXEIE_BIT  = 7   // TX Empty Interrupt Enable
} UART_CR1_Bits_t;

/* ------------------- Control Register 2 (CR2) Stop Bits ------------------- */
typedef enum {
    CR2_STOP_1_BIT = 0x00,  // 1 stop bit
    CR2_STOP_2_BIT = 0x20   // 2 stop bits (bits 13:12 = 10)
} UART_CR2_StopBits_t;

#endif

