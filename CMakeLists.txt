# CMakeLists.txt for hamming-c-lib ESP-IDF component

idf_component_register(
                    SRCS 
                        "hamming.c"
                    INCLUDE_DIRS 
                        "include"
                    )