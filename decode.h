#ifndef DECODE_H
#define DECODE_H

#include "types.h" // Contains user defined types

/* 
 * Structure to store information required for
 * decoding secret file from source Image
 * Info about output and intermediate data is
 * also stored
 */

typedef struct _DecodeInfo
{
    /* Source Image info */
    char *src_image_fname;
    FILE *fptr_src_image;
    char image_data[MAX_IMAGE_BUF_SIZE];

    /* Secret File Info */
    char *secret_fname;
    FILE *fptr_secret;
    int secret_file_extn_size;
    char secret_file_extn[MAX_FILE_SUFFIX];


    /* Decoded File Info */
    char *decoded_fname;
    FILE *fptr_decoded;
    char magic_string_decoded[3];
    int decoded_secret_file_size;
    char secret_data[1];
    

} DecodeInfo;

/* Decode function, which perform decoding */
Status do_decoding(DecodeInfo *decInfo , char *argv[]);
/* Get File pointers for i/p files */
Status open_files_dec(DecodeInfo *decInfo);
/* Open File for decoding o/p files */
Status open_files_decoded(DecodeInfo *decInfo);
/*  Read and validate decode args from argv */
Status read_and_validate_decode_args(char *argv[], DecodeInfo *decInfo);
/* Decode magic string */
Status decode_magic_string(char *magic_string,DecodeInfo *decInfo);
/* Decode secret bits from lsb from image data */
Status decode_bit_from_lsb(char *image_data,char *data, int index); 
/* Decode secret file extension size */
Status decode_secret_file_extn_size( DecodeInfo *decInfo);
/* Decode secret file extension */
Status decode_secret_file_extn(DecodeInfo *decInfo);
/* Decode secret file size */
Status decode_secret_file_size( DecodeInfo *decInfo);
/* Decode secret data */
Status decode_secret_data (DecodeInfo *decInfo);

#endif
