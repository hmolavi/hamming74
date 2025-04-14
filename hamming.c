/**
 * @file hamming.c
 * @author Hossein Molavi (hmolavi@uwaterloo.ca)
 *
 * @brief Implementation of Hamming Code for error detection and correction.
 *
 * This file contains functions to encode data using Hamming code, decode
 * encoded data, and detect and correct single-bit errors.
 *
 * @note
 *  Specifically designed for use of Hamming(7,4).
 *
 * @version 1.0
 * @date 2025-04-13
 *
 * @copyright Copyright (c) 2025
 */

#include <stdlib.h>
#include "hamming.h"

/*-----------------------------------------------------------*/
/*   --------------   Helper Functions   -----------------   */
/*-----------------------------------------------------------*/

/**
 * @brief Calculate the even-parity for a given parity bit position.
 *
 * @param n The total number of bits in the encoded data.
 * @param data Pointer to the array containing the data bits.
 * @param p The index of the parity check (0 for P1, 1 for P2, etc.).
 * @return The calculated parity (0 or 1).
 */
static int parity_check(int n, int *data, int p)
{
    int mask = 1 << p;  // 1-based position of the parity bit (2^p)
    int sum = 0;
    // Iterate over all bits starting from the current parity bit's position
    for (int i = mask - 1; i < n; i++) {
        // Check if the (i+1)-th bit has the p-th bit set (1-based position)
        if (((i + 1) & mask) != 0) {
            sum ^= data[i];
        }
    }
    return sum;
}

/*-----------------------------------------------------------*/

/**
 * @brief Calculate the syndrome for the given encoded Hamming code data.
 *
 * - Dependant on parity_check()
 *
 * @param n The number of bits in the encoded_data array.
 * @param encoded_data Pointer to the array containing the encoded Hamming code data.
 * @return The calculated syndrome, indicates position of single-bit error.
 */
static int calculate_syndrome(int n, int *encoded_data)
{
    int syndrome = 0;
    int p = 0;
    // Determine the number of parity bits needed (r) such that 2^r >= n + r + 1
    // For simplicity, iterate while 2^p <= n
    while ((1 << p) <= n) {
        int parity_pos = (1 << p) - 1;  // 0-based position of parity bit
        if (parity_pos >= n) break;
        // Compute the parity check for this p
        int check = parity_check(n, encoded_data, p);
        // If the check is non-zero, set the corresponding syndrome bit
        syndrome |= (check << p);
        p++;
    }
    return syndrome;
}

/*-----------------------------------------------------------*/

/**
 * @brief Encodes a 4-bit nibble into a 7-bit Hamming(7,4) encoded array.
 *
 * @param data An integer array of size 4 containing the 4-bit input nibble.
 * @param encoded_data An integer array of size 7 that will be populated with the Hamming(7,4) encoded data.
 */
static void hamming_encode_nibble(const int data[4], int encoded_data[7])
{
    // For a 4-bit nibble, the number of parity bits is fixed at 3 (7 = 4 data bits + 3 parity bits)
    int n = 7;
    int r = 3;

    // Initialize encoded_data to 0
    for (int i = 0; i < n; i++) {
        encoded_data[i] = 0;
    }

    // Place data bits in non-parity positions
    int j = 0;
    for (int i = 0; i < n; i++) {
        // Skip positions that are powers of 2: indices 0, 1, and 3 (i+1 == 1,2,4)
        if ((i & (i + 1)) == 0) {
            continue;
        }
        encoded_data[i] = data[j];
        j++;
    }

    // Calculate and set the parity bits
    for (int p = 0; p < r; p++) {
        int parity_pos = (1 << p) - 1;  // 0-based index for parity bit
        encoded_data[parity_pos] = parity_check(n, encoded_data, p);
    }
}

/*-----------------------------------------------------------*/

/**
 * @brief Decodes a 7-bit encoded nibble into its original 4-bit data.
 *
 * The expected arrangement for Hamming(7,4) is:
 * - Data bits are located at indices 2, 4, 5, and 6 of the encoded_data array.
 *
 * @param encoded_data An integer array of length 7 containing the encoded nibble bits.
 * @param decoded_data An integer array of length 4 where the resulting decoded (and corrected)
 *                     4-bit data is stored.
 */
static void hamming_decode_nibble(int encoded_data[7], int decoded_data[4])
{
    // Calculate the syndrome to detect errors over 7 bits
    int syndrome = calculate_syndrome(7, encoded_data);

    // Correct the error if syndrome is non-zero
    if (syndrome != 0) {
        int error_pos = syndrome - 1;  // Convert to 0-based index
        if (error_pos < 7) {
            encoded_data[error_pos] ^= 1;  // Flip the incorrect bit
        }
    }

    // For Hamming(7,4), the data bits are at indices 2, 4, 5, and 6.
    decoded_data[0] = encoded_data[2];
    decoded_data[1] = encoded_data[4];
    decoded_data[2] = encoded_data[5];
    decoded_data[3] = encoded_data[6];
}

/*-----------------------------------------------------------*/
/*  -----------------   API Functions    -----------------   */
/*-----------------------------------------------------------*/

void hamming_encode_generic(const void *data, size_t data_size, int *out_bits)
{
    // Treat data as a stream of bytes.
    // Each byte contains 2 nibbles, and each nibble (4 bits) is encoded into 7 bits.
    const unsigned char *bytes = (const unsigned char*)data;
    size_t nibble_count = data_size * 2;  // Two nibbles per byte

    for (size_t i = 0; i < nibble_count; i++) {
        int block[4];
        unsigned char nibble;

        // Extract high nibble for even indices, low nibble for odd indices
        if (i % 2 == 0) {
            nibble = bytes[i / 2] >> 4;
        } else {
            nibble = bytes[i / 2] & 0x0F;
        }

        // Convert nibble to a 4-element bit array.
        block[0] = (nibble >> 3) & 1;
        block[1] = (nibble >> 2) & 1;
        block[2] = (nibble >> 1) & 1;
        block[3] = nibble & 1;

        int encoded[7];
        hamming_encode_nibble(block, encoded);

        // Place the encoded 7 bits into the output array.
        for (int j = 0; j < 7; j++) {
            out_bits[i * 7 + j] = encoded[j];
        }
    }
}

void hamming_encode_74(const int *input_bits, int total_bits, int *out_bits)
{
    // total_bits should be a multiple of 4
    for (int i = 0; i < total_bits; i += 4) {
        int block[4] = {
            input_bits[i],
            input_bits[i + 1],
            input_bits[i + 2],
            input_bits[i + 3]};
        int encoded[7];
        hamming_encode_nibble(block, encoded);
        for (int j = 0; j < 7; j++) {
            out_bits[(i / 4) * 7 + j] = encoded[j];
        }
    }
}

/*-----------------------------------------------------------*/

void hamming_decode_74(const int *in_bits, int total_bits, int *decoded_bits)
{
    for (int i = 0; i < total_bits / 4; i++) {
        int encoded[7];
        for (int j = 0; j < 7; j++) {
            encoded[j] = in_bits[i * 7 + j];
        }
        int block[4];
        hamming_decode_nibble(encoded, block);

        // Copy back 4 decoded bits
        decoded_bits[i * 4 + 0] = block[0];
        decoded_bits[i * 4 + 1] = block[1];
        decoded_bits[i * 4 + 2] = block[2];
        decoded_bits[i * 4 + 3] = block[3];
    }
}

/*-----------------------------------------------------------*/