#ifndef BIT_OPS_H
#define BIT_OPS_H

#include <stdint.h>
#include <stdbool.h>

/*============================================================================*
 *                              BASIC BIT OPERATIONS                          *
 *============================================================================*/

/*
 * @brief Set a specific bit in a value
 * @param value The value to modify
 * @param position The bit position to set (0-based indexing)
 * @return Value with the specified bit set to 1
 * 
 * Example: SET_BIT(0x05, 1) returns 0x07 (101 -> 111)
 */
#define SET_BIT(value, position)    ((value) | (1U << (position)))

/*
 * @brief Clear a specific bit in a value
 * @param value The value to modify  
 * @param position The bit position to clear (0-based indexing)
 * @return Value with the specified bit set to 0
 * 
 * Example: CLEAR_BIT(0x07, 1) returns 0x05 (111 -> 101)
 */
#define CLEAR_BIT(value, position)  ((value) & ~(1U << (position)))

/*
 * @brief Toggle a specific bit in a value
 * @param value The value to modify
 * @param position The bit position to toggle (0-based indexing)  
 * @return Value with the specified bit flipped
 * 
 * Example: TOGGLE_BIT(0x05, 1) returns 0x07 (101 -> 111)
 */
#define TOGGLE_BIT(value, position) ((value) ^ (1U << (position)))

/*
 * @brief Check if a specific bit is set
 * @param value The value to check
 * @param position The bit position to check (0-based indexing)
 * @return Non-zero if bit is set, 0 if bit is clear
 * 
 * Example: CHECK_BIT(0x05, 2) returns 4 (non-zero, bit is set)
 *          CHECK_BIT(0x05, 1) returns 0 (bit is clear)
 */
#define CHECK_BIT(value, position)  ((value) & (1U << (position)))

/*
 * @brief Check if a specific bit is set (normalized to 0/1)
 * @param value The value to check
 * @param position The bit position to check (0-based indexing)
 * @return 1 if bit is set, 0 if bit is clear
 * 
 * Example: IS_BIT_SET(0x05, 2) returns 1 (bit is set)
 *          IS_BIT_SET(0x05, 1) returns 0 (bit is clear)
 */
#define IS_BIT_SET(value, position) (((value) & (1U << (position))) != 0)

#endif /* BIT_OPS_H */
