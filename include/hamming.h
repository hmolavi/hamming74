/**
 * @file hamming.h
 * @author Hossein Molavi (hmolavi@uwaterloo.ca)

 * @brief Implementation of Hamming Code for error detection and correction.
 *
 * @note
 *  Specifically designed for use of Hamming(7,4).
 *
 * @version 1.0
 * @date 2025-04-13
 *
 * @copyright Copyright (c) 2025
 */

#ifndef __HAMMING_H__
#define __HAMMING_H__

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Encode data using Hamming(7,4) error correction.
 *
 * @param data Pointer to the array containing the data bits to be encoded.
 * @param data_bits The number of data bits in the input array.
 * @param encoded_data Pointer to the array where the complete encoded data
 *                     (including parity bits) will be stored.
 *
 * @note Ensure that the encoded_data array has been pre-allocated with
 * sufficient size to accommodate both the data and parity bits.
 */
void hamming_encode_74(const int *input_bits, int total_bits, int *out_bits);

/**
 * @brief Decode Hamming(7,4) encoded data with error detection and correction.
 *
 * @param encoded_data Pointer to the array containing the encoded data bits (data and parity).
 * @param n The total number of bits in the encoded_data array.
 * @param decoded_data Pointer to the array where the original decoded data bits will be stored.
 */
void hamming_decode_74(const int *in_bits, int total_bits, int *decoded_bits);

#ifdef __cplusplus
}
#endif

#endif  // __HAMMING_H__