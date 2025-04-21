#pragma once

#define OUTPUT_FILE_NAME "ssd_output.txt"
#define NAND_FILE_NAME "ssd_nand.txt"
#define COMMAND_BUFFER_FOLDER_NAME "buffer"
#define ZERO_PATTERN "0x00000000"
#define ERROR_PATTERN "ERROR"
#define FAIL_BUFFER_READ_MESSAGE "DATA_IS_NOT_IN_BUFFER"

#define MAX_LBA 99
#define MIN_LBA 0
#define LBA_SIZE 4 // 4Byte
#define MAX_BUFFER_SIZE 5
