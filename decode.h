#ifndef DECODE_H
#define DECODE_H

#include "types.h" // Contains user defined types

typedef struct _DecodeInfo
{
	char *src_image_fname_decode;
	FILE *fptr_src_image_decode;

	char out_fname[30];
	FILE *fptr_out_fname;
	char extn_out_file[5];
	unsigned int out_extn_size;
	unsigned long out_file_size;

    char *magic_string;

}DecodeInfo;

Status read_and_validate_decode_args(char *argv[],DecodeInfo *decInfo);

Status decode_magic_string(DecodeInfo *decInfo);

Status decode_out_file_extn_size(DecodeInfo *decInfo);

Status decode_out_file_extn(DecodeInfo *decInfo);

Status decode_out_file_size(DecodeInfo *decInfo);

Status decode_out_file_data(DecodeInfo *decInfo);

Status do_decoding(DecodeInfo *decInfo);

Status open_files_decode(DecodeInfo *decInfo);

Status decode_data_from_image(char *data, int size, FILE *fptr_src_image_decode, FILE *fptr_out_fname);

uint decode_size(DecodeInfo *decInfo);

char decode_lsb_to_byte(char *image_buffer);

#endif
