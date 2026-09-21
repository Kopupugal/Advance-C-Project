/*
Name: Kopperumdevi
Date: 14.04.2024
Description : Decoding for Steganography Project
*/
#include <stdio.h>
#include "encode.h"
#include "decode.h"
#include "types.h"
#include <string.h>
#include "common.h"

/* Opening files for decoding */
Status open_files_dec(DecodeInfo *decInfo)
{
    //Stego Image file
    decInfo->fptr_src_image = fopen(decInfo->src_image_fname, "r");
    // Do Error handling
    if(decInfo->fptr_src_image == NULL)
    {
	perror("fopen");
	fprintf(stderr, "ERROR : Unable to open file %s\n", decInfo->src_image_fname);

	return e_failure;
    }

    //Secret file
    decInfo->fptr_secret = fopen(decInfo->secret_fname, "r");
    // Do Error handling
    if (decInfo->fptr_secret == NULL)
    {
	perror("fopen");
	fprintf(stderr, "ERROR: Unable to open file %s\n", decInfo->secret_fname);

	return e_failure;
    }


    // No failure return e_success
    return e_success;
}
/* File Open for Decoded .txt */
Status open_files_decoded(DecodeInfo *decInfo)
{
    // Decoded txt
    decInfo->fptr_decoded = fopen(decInfo->decoded_fname, "w");
    // Do Error handling
    if (decInfo->fptr_decoded == NULL)
    {
	perror("fopen");
	fprintf(stderr, "ERROR: Unable to open file %s\n", decInfo->decoded_fname);
	return e_failure;
    }
    return e_success;

}

/* Decode magic string*/

Status decode_magic_string(char *magic_string, DecodeInfo *decInfo)
{
    int i;
    fseek(decInfo->fptr_src_image,54,SEEK_SET);
    for (i=0;i<strlen(magic_string);i++)
    {
	decInfo->magic_string_decoded[i]=0;
	fread(decInfo->image_data,8,1,decInfo->fptr_src_image);
	decode_bit_from_lsb(decInfo->image_data,decInfo->magic_string_decoded,i);
    }
	decInfo->magic_string_decoded[i]='\0';
    return e_success;
}

/* Decode bit from lsb*/
Status decode_bit_from_lsb(char *image_data, char *data,int index)
{
    data[index] = 0;
    for (int i=0; i<8; i++)
    {
	data[index] = data[index] | (((image_data[i] ) & 1) << i);
    }
    return e_success;
}
/* Decode Secret file extn_size */
Status decode_secret_file_extn_size( DecodeInfo *decInfo)
{
    char str[32];
    decInfo->secret_file_extn_size = 0;
    fread(str,32,1,decInfo->fptr_src_image);
    for(int i=0; i<32; i++)
    {
	decInfo->secret_file_extn_size = decInfo->secret_file_extn_size | (((str[i]) &1) << i);
    }
    return e_success;
}
/* Decode secret file extn*/
Status decode_secret_file_extn (DecodeInfo *decInfo)
{
    int i=0;
    for(i=0; i<(decInfo->secret_file_extn_size); i++)
    {
	decInfo->secret_file_extn[i]=0;
	fread(decInfo->image_data,8,1,decInfo->fptr_src_image);
	decode_bit_from_lsb(decInfo->image_data,decInfo->secret_file_extn,i);
    }
	decInfo->secret_file_extn[i]='\0';
    return e_success;
}
/* Decode secret file size*/
Status decode_secret_file_size( DecodeInfo *decInfo)
{
    char str[32];
    decInfo->decoded_secret_file_size = 0;
    fread(str,32,1,decInfo->fptr_src_image);
    for(int i=0; i<32; i++)
    {
	decInfo->decoded_secret_file_size = decInfo->decoded_secret_file_size | (((str[i]) &1) << i);
    }
    return e_success;
}
/* Decoded secret  data */
Status decode_secret_data (DecodeInfo *decInfo)
{
    for(int i=0; i<(decInfo->decoded_secret_file_size); i++)
    {
	decInfo->secret_data[0]=0;
	fread(decInfo->image_data,8,1,decInfo->fptr_src_image);
	decode_bit_from_lsb(decInfo->image_data,decInfo->secret_data,0);
	fwrite(decInfo->secret_data,1,1,decInfo->fptr_decoded);
    }
    return e_success;
}

/* Decoding is here */
Status do_decoding(DecodeInfo *decInfo,char *argv[])
{
    if(open_files_dec(decInfo) ==  e_success)
    {
	printf("INFO: Open file operation is Success\n");
	if (decode_magic_string(MAGIC_STRING,decInfo) ==  e_success)
	{
	    printf("Decoded magic string is %s\n",decInfo->magic_string_decoded);
	    if(strcmp(MAGIC_STRING,decInfo->magic_string_decoded) == 0)
	    printf("INFO: Decoding Magic string is Successfull\n");
	    if(decode_secret_file_extn_size(decInfo) == e_success)
	    {
		printf("Decoded Extension size is %d\n",decInfo->secret_file_extn_size);
		printf("Decoded secret file extension size successfully\n");
		if(decode_secret_file_extn(decInfo) == e_success)
		{
		   printf("Decoded Extension is %s\n",decInfo->secret_file_extn);
		   printf("Decoded secret file extension successfully\n"); 
                   if (argv[4] != NULL)
                   {
                       decInfo->decoded_fname = argv[4];
                   }
                   else
                   {
		       char temp_decoded_fname[20] = "decoded";
                       strcat(temp_decoded_fname,decInfo->secret_file_extn);
                       decInfo->decoded_fname = temp_decoded_fname;
                   }
		   if(open_files_decoded(decInfo) == e_success)
		   {
                       printf("INFO: Output file not mentioned,Creating %s as default\n",decInfo->decoded_fname);
		       if(decode_secret_file_size(decInfo) == e_success)
		       {
			   printf("INFO: Decoded Secret File size is %d\n",decInfo->decoded_secret_file_size);
			   if(decode_secret_data(decInfo) == e_success)
			   {
			       printf("INFO: Decoded secret data is Successfull\n");
			   }
			   else
			   {
			       printf("ERROR : Decoded secret data is Failed\n");
			       return 1;
			   }
		       }
		       else
		       {
			   printf("ERROR : Decode Secret File size is Failed\n");
			   return 1;
		       }
		   }
		   else
		   {
		       printf("ERROR : Output secret file cannot be open\n");
		       return 1;
		   }

		}
		else
		{
		    printf("ERROR : Secret file extension decoding is failed\n");
		    return 1;
		}	    
	    }
	    else
	    {
		printf("ERROR : Secret file size extension decoding is failed\n");
		return 1;
	    }
	}
	else
	{
	    printf("ERROR : Magic string decoding is failed\n");
	    return 1;
	}	    
    }
    else
    {
	printf("ERROR : Open file decode is failed\n");
	return 1;
    }
    return 0;
}
/* Read and Validate cla for decodings */
Status read_and_validate_decode_args(char *argv[], DecodeInfo *decInfo)
{
    if(strcmp( (strstr(argv[2], ".")), ".bmp") == 0 ) //stego_img.bmp
    {
	decInfo->src_image_fname = argv[2];
    }
    else
    {
	return e_failure;	
    }
    if( strcmp(strstr(argv[3], "."), ".txt") == 0 )
    {
	decInfo->secret_fname = argv[3];
    }
    else
    {
	return e_failure;
    }
    return e_success;
}





