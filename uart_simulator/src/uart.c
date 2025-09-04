#include"uart.h"

#define BUFFER_SIZE 64

UART_Error_t UART_Init(UART_Handle_t* huart, UART_Config_t* config){
    // Input validation
    if (huart == NULL || config == NULL){
        return UART_ERROR_NULL_POINTER;
    }

    // Copy configuration
    huart->config = *config;

    //create tx/rx buffers
    huart->tx_buffer = cb_create(BUFFER_SIZE, sizeof(uint8_t));
    if (huart->tx_buffer == NULL){
        return UART_ERROR_BUFFER_CREATE;
    }

    huart->rx_buffer = cb_create(BUFFER_SIZE, sizeof(uint8_t));
    if (huart->tx_buffer == NULL){
        cb_destroy(huart->tx_buffer);
        return UART_ERROR_BUFFER_CREATE;
    }

    huart->tx_interrupt_enabled = false;
    huart->rx_interrupt_enabled = false;
    huart->error_interrupt_enabled = false;
    huart->tx_busy = false;
    huart->rx_busy = false;
    huart->current_error = UART_NO_ERROR;

    return UART_NO_ERROR;
}

