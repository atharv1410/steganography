#include <stdio.h>
#include <string.h>
#include "encode.h"
#include "types.h"

/* Function Definitions */

/*Check operation type 
 * Return Value: e_success or e_failure, on file errors
 */
OperationType check_operation_type(char *argv[])
{
	if((strcmp(argv[1],"-e")) == 0)
	{
		return e_encode;
	}
	else if((strcmp(argv[1],"-d")) == 0)
	{
		return e_decode;
	}
	else
	{
		return e_unsupported;
	}
}

/*Read and validate Encode args from argv 
 * Inputs: Src Image file, Secret file and
 * Stego Image file
 * op performed: 
 * 1. validate extn of source image file
 * 2. open source file compair first 2 bytes ("B" & "M")
 * 3. store the extn and extn size of secret file
 * 4. store the magic string char * and its size int
 * 5. if stego file provided validate or create a stego.bmp image file
 * 
 */
Status read_and_validate_encode_args(char *argv[], EncodeInfo *encInfo)
{
	char arr[20];
	if (!(strlen(argv[2]) > 4 && (strcmp(argv[2] + strlen(argv[2]) - 4, ".bmp") == 0)))  // validate extn .bmp
{
	 	printf("ERROR : INPUT FILE IS NOT A BMP FILE.\n\n");
		return e_failure;
	}
	else
	{
		encInfo -> fptr_src_image= fopen(argv[2],"r");   // open the sourc image file 
	
		if(encInfo -> fptr_src_image == NULL)
		{
			printf("ERROR : Source file was not able to open.\n\n");
		}
		else
		{
			fread(arr,1,2,encInfo -> fptr_src_image);   // copy first 2 bytes in char array
		
			if(arr[0] == 'B' && arr[1] == 'M')    // first two bytes should be B and M 
			{
				printf("INFO : Source file is .bmp file.\n\n");
				encInfo -> src_image_fname = argv[2];
			}
			else
			{
				printf("INFO : Source file is not a .bmp file.\n\n");
				return e_failure;
			}
		}
	}
	encInfo -> secret_fname = argv[3];     // char * to hold secret file name
	
	strcpy(arr,argv[3]);
	int i,j;
	for(i=0;arr[i] != '\0';i++)
	{
		if(arr[i] == '.')
		{
			for(j=0;arr[i+j] != '\0';j++)
			{
				encInfo -> extn_secret_file[j] = arr[i+j];  // store the extention of secret file char arr (ex- .txt)
			}
			encInfo	-> secret_extn_size = j;   // store the size of extn of secret file int (ex - 4)
		}

	}
	printf("INFO : Secret file name is saved.\n\n");
    if (argv[4] != NULL)
    {
        encInfo->magic_string = argv[4];      // store magic string in char *
		encInfo->magic_string_size = strlen(argv[4]);   // store size of magic string int
        printf("INFO : Magic String Saved.\n\n");
    }else {
        printf("ERROR : ENTER Magic String.\n\n");
        return e_failure;
    }
    
	if(argv[5] != NULL)
	{
		encInfo -> fptr_stego_image= fopen(argv[5],"w");   // if new file is provided open and validate
	
		if(encInfo -> fptr_stego_image == NULL)
		{
			printf("ERROR : Output file was not able to open.\n\n");
			return e_failure;
		}
		else
		{
			if((strlen(argv[5]) > 4 && (strcmp(argv[5] + strlen(argv[5]) - 4 , ".bmp")) == 0))
			{
				printf("INFO : Output file is saved as per given file name:\n\n");
				encInfo -> stego_image_fname = argv[5]; 
			}
			else
			{
				printf("INFO : Output file was not as per given format(.bmp).\n\n");
				return e_failure;
			}
		}
	}
	else
	{
		printf("INFO : Output file is saved as default file name(stego.bmp).\n\n");
		encInfo -> stego_image_fname = "stego.bmp";	   // else create stego file
	}
	return e_success;
}

/*Perform the encoding
 * Inputs: Src Image file, Secret file and
 * Stego Image file
 * op performed:
 * 1. call open_files - open files 1.source img file 2.secret data file 3.stego image file
 * 2. call check_capacity - data size * 8 should be smaller then source image file size  
 * 3. call copy_bmp_header copy 54 bytes from source file to sego file
 * 4. encoding
 * a. call encode_magic_string
 * b. call encode_secret_file_extn_size ex - 4
 * c. call encode_secret_file_extn ex - .txt
 * d. call encode_secret_file_size total char in the file in bytes
 * e. call encode_secret_file_data 
 * f. call copy_remaining_file_data
 *
 */
Status do_encoding(EncodeInfo *encInfo)
{
	if(open_files(encInfo) == e_failure)
	{
		printf("Error occured during file opening.\n");
		return e_failure;
	}
	if(check_capacity(encInfo) == e_failure)
	{
		printf("Error: Source image cant store the secret text.\n");
		return e_failure;
	}
	
	copy_bmp_header(encInfo ->fptr_src_image,encInfo ->fptr_stego_image);
	
	encode_magic_string(encInfo);
	
	encode_secret_file_extn_size(encInfo);
	
	encode_secret_file_extn(encInfo ->extn_secret_file,encInfo);
	
	encode_secret_file_size(get_file_size(encInfo -> fptr_secret),encInfo);

	encode_secret_file_data(encInfo);

	copy_remaining_img_data(encInfo -> fptr_src_image, encInfo -> fptr_stego_image);

    return e_success;
}


Status open_files(EncodeInfo *encInfo)
{
    // Src Image file
    encInfo->fptr_src_image = fopen(encInfo->src_image_fname, "r");
    // Do Error handling
    if (encInfo->fptr_src_image == NULL)
    {
    	perror("fopen");
    	fprintf(stderr, "ERROR: Unable to open file %s\n", encInfo->src_image_fname);

    	return e_failure;
    }

    // Secret file
    encInfo->fptr_secret = fopen(encInfo->secret_fname, "r");
    // Do Error handling
    if (encInfo->fptr_secret == NULL)
    {
    	perror("fopen");
    	fprintf(stderr, "ERROR: Unable to open file %s\n", encInfo->secret_fname);

    	return e_failure;
    }

    // Stego Image file
    encInfo->fptr_stego_image = fopen(encInfo->stego_image_fname, "w");
    // Do Error handling
    if (encInfo->fptr_stego_image == NULL)
    {
    	perror("fopen");
    	fprintf(stderr, "ERROR: Unable to open file %s\n", encInfo->stego_image_fname);

    	return e_failure;
    }

    // No failure return e_success
    return e_success;
}


Status check_capacity(EncodeInfo *encInfo)
{
	if( ((strlen(encInfo->magic_string)*8) + 32 + (strlen(strstr(encInfo ->secret_fname,"."))*8) + 32 + (get_file_size(encInfo->fptr_secret) * 8) + 54 )<=get_image_size_for_bmp(encInfo ->fptr_src_image) )
	{
		printf("INFO : Secret file can be encoded in source image.\n\n");
	}
	else
	{
		return e_failure;
	}
}

uint get_image_size_for_bmp(FILE *fptr_image)
{
    uint width, height;
    // Seek to 18th byte
    fseek(fptr_image, 18, SEEK_SET);

    // Read the width (an int)
    fread(&width, sizeof(int), 1, fptr_image);
    //printf("width = %u\n", width);

    // Read the height (an int)
    fread(&height, sizeof(int), 1, fptr_image);
    //printf("height = %u\n", height);
    // Return image capacity
    return width * height * 3;
}


uint get_file_size(FILE *fptr)
{
	int size;
	fseek(fptr,0,SEEK_END);
	size = ftell(fptr);
	return size;
}


Status copy_bmp_header(FILE *fptr_src_image, FILE *fptr_dest_image)
{
	rewind(fptr_src_image);   // get to initial of file
	rewind(fptr_dest_image);
	char arr[54];
	fread(arr,1,54,fptr_src_image);  // copy in char arr
	fwrite(arr,1,54,fptr_dest_image);  // write in dest_file
	
	printf("INFO : Successfully copied bmp header to output file.\n\n");
}


Status encode_magic_string(EncodeInfo *encInfo)
 {
 	encode_data_to_image(encInfo->magic_string,strlen(encInfo->magic_string),encInfo ->fptr_src_image, encInfo ->fptr_stego_image);
 	
	printf("INFO : Successfully Encoded magic string.\n\n");
 }


Status encode_secret_file_extn(char *file_extn, EncodeInfo *encInfo)
{
	encode_data_to_image ( file_extn , encInfo->secret_extn_size , encInfo -> fptr_src_image , encInfo -> fptr_stego_image );
	
	printf("INFO : Successfully Encoded secret file extension.\n\n");
}


Status encode_secret_file_size(long file_size, EncodeInfo *encInfo)
{
	encInfo -> secret_extn_size = file_size;
	encode_secret_file_extn_size(encInfo);
	
	printf("INFO : Successfully Encoded secret file size.\n\n");
}


Status encode_secret_file_data(EncodeInfo *encInfo)
{
	char arr[encInfo -> secret_extn_size];
	rewind(encInfo -> fptr_secret);
	for(int i=0;i<encInfo -> secret_extn_size;i++)
	{
		arr[i] = fgetc(encInfo -> fptr_secret);
	}
	encode_data_to_image(arr,encInfo ->secret_extn_size,encInfo ->fptr_src_image,encInfo -> fptr_stego_image);
	
	printf("INFO : Successfully Encoded secret data into image file.\n\n");
}



Status encode_data_to_image(char *data, int size, FILE *fptr_src_image, FILE *fptr_stego_image)
{
	char arr[8];
	for(int i=0;i<size;i++)
	{	
		fread(arr,1,8,fptr_src_image);
		encode_byte_to_lsb(data[i],arr);   // data[i] 1 byte(8 bits) char arr 8 bytes
		fwrite(arr,1,8,fptr_stego_image);
	}
}


Status encode_byte_to_lsb(char data, char *image_buffer)
{
	for(int i=0;i< 8;i++)
	{
		image_buffer[i] = (image_buffer[i] & 0xFE) | ((data >> (7-i)) & 1);    // clear last bit and get from msb and set
	} 
}


Status copy_remaining_img_data(FILE *fptr_src, FILE *fptr_dest)
{
	long int initial = ftell(fptr_src);   // get initial value
	long int end = get_file_size(fptr_src);  // get the full size of file
	fseek(fptr_src,initial,SEEK_SET);  // fseek til the initial again 
	char arr[end-initial];   // char array size of data remanig to copy 
	fread(arr,1,end-initial,fptr_src);   // read the data
	fwrite(arr,1,end-initial,fptr_dest);   // write the data
	
	printf("INFO : Successfully Copied remaining data.\n\n");
}

/*
for extention size i.e 4bytes i.e 4*8 =32
*/
Status encode_secret_file_extn_size(EncodeInfo *encInfo)
{
	char arr[32];
	fread(arr,1,32,encInfo -> fptr_src_image);
	for(int i=0;i<32;i++)
	{
		arr[i] = (arr[i] & 0xFE) | (((encInfo ->secret_extn_size) >> (31-i)) & 1);
	}	
	fwrite(arr,1,32,encInfo ->fptr_stego_image);
}
