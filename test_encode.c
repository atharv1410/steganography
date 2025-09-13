/**
 * Documentation Header
 * @file        test_encode.c
 * @brief       Main entry point for the LSB Image Steganography
 * @author      Atharv Hirapure 25002_018
 * @date        May 22, 2025
 * 
 * @project     LSB Image Steganography
 */


#include <stdio.h>
#include "encode.h"
#include "decode.h"
#include "types.h"

int main(int argc, char *argv[])
{
    if(argc<3)
    {
    	printf("ERROR : Not enough arguments passed \n");
    	printf("INFO : (For encoding)Provide Arguments like (./a.out -e Source_file_name(ex- beautiful.bmp) Secret_file_name(ex- secret) Magic_string(ex- \"#*\") output_file_name(optional))\n\n");
    	printf("INFO : (For decoding)Provide Arguments like (./a.out -d Encoded_file_name(ex stego.bmp) Magic_string(ex- \"#*\") output_file_name(optional))\n\n");
    	return 0;
    }

    EncodeInfo encInfo;
    DecodeInfo decInfo;
    if (check_operation_type(argv) == e_encode)    // Determine if we are encoding
    {
        if (argc > 4 && argc < 7)     
        {
            printf("INFO : Selected Encoding\n");

            if (read_and_validate_encode_args(argv, &encInfo) == e_success)    // read and validate the cammand line args.
            {
                if (do_encoding(&encInfo) == e_success)   // start encoding process
                {
                    printf("---------------------------\n");
				    printf("ENCODING DONE SUCCESSFULLY.\n");
				    printf("---------------------------\n");
                }else {
                    printf("ERROR : Occured during encoding process.\n\n");
                }
            
            }else {
                    printf("ERROR : Occured related to arguments passed.\n\n");
                    return 0;
            }
        }else{
            printf("ERROR : Not enough arguments passed.\n");
    		printf("INFO : (For encoding)Provide Arguments like (./a.out -e Source_file_name(ex- beautiful.bmp) Secret_file_name(ex- secret) Magic_string(ex- \"#$\") output_file_name(optional))\n\n");
    	    return 0;
        }
                        
    }else if(check_operation_type(argv) == e_decode)
    {
    	printf("INFO : Selected decoding\n\n");
	if(argc >= 3)
	{
		if(read_and_validate_decode_args(argv,&decInfo) == d_failure)
		{
			printf("ERROR : Occured related to arguments passed\n\n");
		}
		else
		{
			if(do_decoding(&decInfo) == d_failure)
			{
				printf("ERROR : Occured during Do decoding process.\n\n");
			}
			else
			{
				printf("---------------------------\n");
				printf("DECODING DONE SUCCESSFULLY.\n");
				printf("---------------------------\n");
			}
		}
	}
	else
	{
		printf("ERROR : Not enough arguments passed.\n");
		printf("INFO : (For decoding)Provide Arguments like (./a.out -d Encoded_file_name Magic_string(ex \"#*\") output_file_name(optional))\n\n");
		return 0;
	}
    }else {
        printf("ERROR : Unsupported Operation type.\n        Should be \"-e\" for Encoding \"-d\" for Decoding.\n\n");
    }
    

    return 0;
}
