#include <stdio.h>
#include <string.h>
#include "types.h"
#include "decode.h"


Status read_and_validate_decode_args(char *argv[],DecodeInfo *decInfo){

    char arr[20];
    if (!(strlen(argv[2]) > 4 && (strcmp(argv[2] + strlen(argv[2]) -4 ,".bmp") == 0)))  // extn .bmp
    {
     printf("ERROR : File is not a BMP file.\n\n");   
     return d_failure;
    }
    else
    {
        decInfo->fptr_src_image_decode = fopen(argv[2], "r");  // open file
        if (decInfo->fptr_src_image_decode == NULL)
        {
            printf("ERROR : Source file not opened.\n\n");
            return d_failure;
        }

        fread(arr,1,2,decInfo->fptr_src_image_decode);  // valide B and M in file
        if(arr[0] == 'B' && arr[1] == 'M'){
            printf("INFO : Source file is .bmp file.\n\n");
            decInfo->src_image_fname_decode = argv[2];
        }
        else
        {
            printf("ERROR : Source file is not a BMP file\n\n");
            return d_failure;
        }
        
        if (argv[3] != NULL)
        {
            decInfo->magic_string = argv[3];  // save magic string
        }else
        {
            printf("ERROR : Magic String was not provided.\n\n");
            return d_failure;
        }
        
        if (argv[4] != NULL)    // if output file name given
        {
            strcpy(arr,argv[4]);
            int i =0;
            while (arr[i])
            {
                if (arr[i] == '\0'||
                arr[i] == '.')
                {
                    break;
                }
                
                i++;
                
            }
            

            arr[i] = '\0';
            strcpy(decInfo->out_fname, arr);

            printf("INFO : Output file name saved with given name.\n\n");
        }
        else
        {
            printf("INFO : Created default file name (stego) and saved.\n\n");
            strcpy(decInfo->out_fname,"stego");   // create default file
        }
        
    }
    return d_success;
}
Status do_decoding(DecodeInfo *decInfo){

    if (open_files_decode(decInfo) == d_failure)   // open all the files
    {
        printf("ERROR : File not opened.\n\n");
        return d_failure;
    }
    if (decode_magic_string(decInfo) == d_failure)    // decode the magic string
    {
        printf("ERROR : Source file does not contain secret data OR Provided Magic String was invalid.\n\n");
        return d_failure;
    }
    else{
        printf("INFO : Magic String decode successfully.\n\n");
    }        

    decode_out_file_extn_size(decInfo);

    decode_out_file_extn(decInfo);

    decode_out_file_size(decInfo);

    decode_out_file_data(decInfo);

    return d_success;

}

Status open_files_decode(DecodeInfo *decInfo){

    decInfo->fptr_src_image_decode = fopen(decInfo->src_image_fname_decode,"r");
    if (decInfo->fptr_src_image_decode == NULL)
    {
        printf("ERROR : OPEN_FILE - Source file not opened.\n\n");
    }
    return d_success;
}

Status decode_magic_string(DecodeInfo *decInfo){

    int magic_string_len = strlen(decInfo->magic_string);   // get len of string passed
    char arr[magic_string_len +1];
    arr[magic_string_len] = '\0';

    fseek(decInfo->fptr_src_image_decode,54,SEEK_SET);
    char img_data[8];

    for (int i = 0; i < magic_string_len; i++)
    {
        fread(img_data,1,8,decInfo->fptr_src_image_decode);
        arr[i] = decode_lsb_to_byte(img_data);     // decode magic string
    }
    if (strcmp(arr, decInfo->magic_string) != 0) {   // cmp magic string
        return d_failure;
    }
    return d_success;
}

Status decode_out_file_extn_size(DecodeInfo *decInfo){

    decInfo->out_extn_size = decode_size(decInfo);

    printf("INFO : Successfully decoded output file extn size.\n\n");
}

Status decode_out_file_extn(DecodeInfo *decInfo){

    char arr[8];
    int i;
    for (i = 0; i < decInfo->out_extn_size; i++)
    {
        fread(arr,1,8,decInfo->fptr_src_image_decode);
        decInfo->extn_out_file[i] = decode_lsb_to_byte(arr);
    }
    decInfo->extn_out_file[i] = '\0';

    strcat(decInfo->out_fname,decInfo->extn_out_file);

    printf("INFO : Successfully decoded extn of output file.\n\n");
}

Status decode_out_file_size(DecodeInfo *decInfo){

    decInfo->out_file_size = decode_size(decInfo);

    printf("INFO : Successfully decoded size of file.\n\n");
}

Status decode_out_file_data(DecodeInfo *decInfo){

    decInfo->fptr_out_fname = fopen(decInfo->out_fname, "w");

    char arr[8];
    char ch;

    for (int i = 0; i < decInfo->out_file_size; i++)
    {
        fread(arr,1,8,decInfo->fptr_src_image_decode);
        ch = decode_lsb_to_byte(arr);
        fwrite(&ch,1,1,decInfo->fptr_out_fname);
    }

    printf("INFO : Successfully decoded secret data.\n\n");
    
}

char decode_lsb_to_byte(char *image_buffer){

    char byte = 0;
    for (int i = 0; i < 8; i++) {  // Always process 8 bits
        byte = (byte << 1) | (image_buffer[i] & 1);  
        // Left shift the byte and add the LSB of each byte from image_buffer
    }
    return byte;
    
}

uint decode_size(DecodeInfo *decInfo){

    uint bytes = 0;
    char arr[32];
    fread(arr,1,32,decInfo->fptr_src_image_decode);
    for (int i = 0; i < 32; i++)
    {
        bytes = (bytes << 1) | (arr[i] & 1);  // Left shift the byte and add the LSB of each byte from image_buffer
    }
    return bytes;   
}
