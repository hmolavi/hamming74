[![Component Registry](https://components.espressif.com/components/hmolavi/hamming74/badge.svg)](https://components.espressif.com/components/hmolavi/hamming74) [![Espressif](https://img.shields.io/badge/Espressif-Components-blue.svg?style=flat-square)](https://components.espressif.com/components/hmolavi/hamming74) [![PIO](https://badges.registry.platformio.org/packages/hmolavi/library/hamming74.svg)](https://registry.platformio.org/libraries/hmolavi/Hamming74) [![GitHub](https://img.shields.io/badge/GitHub-hmolavi/hamming74-blue.svg?style=flat-square)](https://github.com/hmolavi/hamming74)

# Hamming(7,4) Encoding and Decoding Library

A lightweight C library providing functions to encode and decode data bits using the Hamming(7,4) error correction scheme.

## Features

- Encodes 4-bit data blocks into 7-bit Hamming code.
- Decodes and corrects single-bit errors in encoded data.

## Getting Started

### Usage

> **Warning:** The encode function assumes you are passing in bits in multiples of 4.

1. Include the header:
    ```c
    #include "hamming.h"
    ```
2. Encode & Decode data:

    ```c
    int input_bits[4] = {1,0,1,1};
    int out_bits[7];
    hamming_encode_74(input_bits, sizeof(input_bits), out_bits);

    int decoded_bits[4];
    hamming_decode_74(out_bits, sizeof(out_bits), decoded_bits);

    /* decoded_bits will be the same as input_bits */
    for (int i = 0; i < 4; i++) {
         assert(input_bits[i] == decoded_bits[i]);
    }
    ```

## License

This project is released under the [MIT License](LICENSE). Use it freely in your own projects!
