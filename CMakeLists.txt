# CMakeLists.txt for hamming-c-lib ESP-IDF component

idf_component_register(
                    SRCS 
                        "hamming_code.c"
                    INCLUDE_DIRS 
                        "include"
                    )