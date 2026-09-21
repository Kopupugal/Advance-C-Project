#include <stdio.h>
#include "encode.h"
#include "decode.h"
#include "types.h"
#include <string.h>
int main(int argc,char *argv[])
{
    if(argc > 3)
    {
    EncodeInfo E1;
    DecodeInfo D1;
    int res = check_operation_type(argv);
    if ( res == e_encode)
    {
	printf("INFO : Encoding is selected\n");
	if (read_and_validate_encode_args(argv, &E1) == e_success)
	{
	    printf("INFO : Read and validate is successfull\n");
	    if(do_encoding(&E1) == e_success)
	    {
		printf("INFO : Encoding is success\n");
	    }
	    else
	    {
		printf("ERROR : Encoding is failure\n");
		return 3;
	    }
	}
	else
	{
	    printf("ERROR : Failed to read and validate encode arg\n");
	    return 2;
	}
    }
    else if( res == e_decode)
    {
	printf("INFO : Decoding is selected\n");
	if (read_and_validate_decode_args(argv, &D1) == e_success)
	{
	    printf("INFO : Read and validate is successfull\n");
	    if(do_decoding(&D1,argv) == e_success)
	    {
		printf("INFO : Decoding is success\n");
	    }
	    else
	    {
		printf("ERROR : Decoding is failure\n");
		return 4;
	    }
	}
	else
	{
	    printf("ERROR : Failed to read and validate decode arg\n");
	    return 3;
	}
    }
    else
    {
	printf("ERROR : Unsupported Operation\n");
        printf("For Encoding:\n./a.out -e beautiful.bmp secret.txt [stegno.bmp]\n");
	printf("For Decoding:\n./a.out -d stegno.bmp data.txt\n");
	return 1;
    }
    }
    else
    {
	printf("ERROR: Please pass the arguments\n");
        printf("For Encoding:\n./a.out -e beautiful.bmp secret.txt [stegno.bmp]\n");
	printf("For Decoding:\n./a.out -d stegno.bmp data.txt\n");
    }
    return 0;
}
OperationType check_operation_type(char *argv[])
{
    if(strcmp(argv[1], "-e") == 0)
	return e_encode;
    else if(strcmp(argv[1], "-d") == 0)
	return e_decode;
    else
	return e_unsupported;
}


