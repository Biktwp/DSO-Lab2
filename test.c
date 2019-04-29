/*
 * OPERATING SYSTEMS DESING - 16/17
 *
 * @file 	test.c
 * @brief 	Implementation of the client test routines.
 * @date	01/03/2017
 */

#include <stdio.h>
#include <string.h>
#include "include/filesystem.h"
#include "include/metadata.h"

// Color definitions for asserts
#define ANSI_COLOR_RESET "\x1b[0m"
#define ANSI_COLOR_RED "\x1b[31m"
#define ANSI_COLOR_GREEN "\x1b[32m"
#define ANSI_COLOR_BLUE "\x1b[34m"

#define N_BLOCKS 25					  // Number of blocks in the device
#define DEV_SIZE N_BLOCKS *BLOCK_SIZE // Device size, in bytes

int main()
{
	int ret;
	//int fd;
	int inodesDir[10];
	char namesDir[10][33];	
	//char *cosa = "123456789";
	//char prueba[10];

	///////

	ret = mkFS(DEV_SIZE);
	if (ret != 0)
	{
		fprintf(stdout, "%s%s%s%s%s", ANSI_COLOR_BLUE, "TEST mkFS ", ANSI_COLOR_RED, "FAILED\n", ANSI_COLOR_RESET);
		return -1;
	}
	fprintf(stdout, "%s%s%s%s%s", ANSI_COLOR_BLUE, "TEST mkFS ", ANSI_COLOR_GREEN, "SUCCESS\n", ANSI_COLOR_RESET);

	///////

	ret = mountFS();
	if (ret != 0)
	{
		fprintf(stdout, "%s%s%s%s%s", ANSI_COLOR_BLUE, "TEST mountFS ", ANSI_COLOR_RED, "FAILED\n", ANSI_COLOR_RESET);
		return -1;
	}
	fprintf(stdout, "%s%s%s%s%s", ANSI_COLOR_BLUE, "TEST mountFS ", ANSI_COLOR_GREEN, "SUCCESS\n", ANSI_COLOR_RESET);


	for(int i = 0; i < 10; i++){
		char cosa[99];
		sprintf(cosa,"/%d",i);
		ret = mkDir(cosa);
		if (ret != 0)
		{
			fprintf(stdout, "%s%s%s%s%s", ANSI_COLOR_BLUE, "TEST creating 10 directories 1 ", ANSI_COLOR_RED, "FAILED\n", ANSI_COLOR_RESET);
			//return -1;
		}
	}
	fprintf(stdout, "%s%s%s%s%s", ANSI_COLOR_BLUE, "TEST creating 10 directories 1 ", ANSI_COLOR_GREEN, "SUCCESS\n", ANSI_COLOR_RESET);

	for(int i = 0; i < 10; i++){
		char cosa[99];
		sprintf(cosa,"/1/%d",i);
		ret = mkDir(cosa);
		if (ret != 0)
		{
			fprintf(stdout, "%s%s%s%s%s", ANSI_COLOR_BLUE, "TEST creating 10 directories 2 ", ANSI_COLOR_RED, "FAILED\n", ANSI_COLOR_RESET);
			//return -1;
		}
	}
	fprintf(stdout, "%s%s%s%s%s", ANSI_COLOR_BLUE, "TEST creating 10 directories 2 ", ANSI_COLOR_GREEN, "SUCCESS\n", ANSI_COLOR_RESET);

	for(int i = 0; i < 10; i++){
		char cosa[99];
		sprintf(cosa,"/1/1/%d",i);
		ret = mkDir(cosa);
		if (ret != 0)
		{
			fprintf(stdout, "%s%s%s%s%s", ANSI_COLOR_BLUE, "TEST creating 10 directories 3 ", ANSI_COLOR_RED, "FAILED\n", ANSI_COLOR_RESET);
			//return -1;
		}
	}
	fprintf(stdout, "%s%s%s%s%s", ANSI_COLOR_BLUE, "TEST creating 10 directories 3 ", ANSI_COLOR_GREEN, "SUCCESS\n", ANSI_COLOR_RESET);

	for(int i = 0; i < 8; i++){
		char cosa[99];
		sprintf(cosa,"/1/2/%d",i);
		ret = mkDir(cosa);
		if (ret != 0)
		{
			fprintf(stdout, "%s%s%s%s%s", ANSI_COLOR_BLUE, "TEST creating 10 directories 4 ", ANSI_COLOR_RED, "FAILED\n", ANSI_COLOR_RESET);
			//return -1;
		}
	}
	fprintf(stdout, "%s%s%s%s%s", ANSI_COLOR_BLUE, "TEST creating 10 directories 4 ", ANSI_COLOR_GREEN, "SUCCESS\n", ANSI_COLOR_RESET);

	ret = mkDir("/1/1/1/1");
	if (ret != 0)
	{
		fprintf(stdout, "%s%s%s%s%s", ANSI_COLOR_BLUE, "TEST create 4 depth directory ", ANSI_COLOR_RED, "FAILED\n", ANSI_COLOR_RESET);
		
	}
	else fprintf(stdout, "%s%s%s%s%s", ANSI_COLOR_BLUE, "TEST create 4 depth directory ", ANSI_COLOR_GREEN, "SUCCESS\n", ANSI_COLOR_RESET);
	
	ret = createFile("/1/1/1/cosa.txt");
	if (ret != 0)
	{
		fprintf(stdout, "%s%s%s%s%s", ANSI_COLOR_BLUE, "TEST create 41 file ", ANSI_COLOR_RED, "FAILED\n", ANSI_COLOR_RESET);
		
	}
	else fprintf(stdout, "%s%s%s%s%s", ANSI_COLOR_BLUE, "TEST create 41 file ", ANSI_COLOR_GREEN, "SUCCESS\n", ANSI_COLOR_RESET);

	ret = rmDir("/1");
	if (ret != 0)
	{
		fprintf(stdout, "%s%s%s%s%s", ANSI_COLOR_BLUE, "TEST create 41 file ", ANSI_COLOR_RED, "FAILED\n", ANSI_COLOR_RESET);
		
	}
	else fprintf(stdout, "%s%s%s%s%s", ANSI_COLOR_BLUE, "TEST create 41 file ", ANSI_COLOR_GREEN, "SUCCESS\n", ANSI_COLOR_RESET);

	ret = lsDir("/",inodesDir,namesDir);
	if (ret < 0)
	{
		fprintf(stdout, "%s%s%s%s%s", ANSI_COLOR_BLUE, "TEST ls / ", ANSI_COLOR_RED, "FAILED\n", ANSI_COLOR_RESET);
		
	}
	else fprintf(stdout, "%s%s%s%s%s", ANSI_COLOR_BLUE, "TEST ls / ", ANSI_COLOR_GREEN, "SUCCESS\n", ANSI_COLOR_RESET);

	ret = lsDir("/1",inodesDir,namesDir);
	if (ret < 0)
	{
		fprintf(stdout, "%s%s%s%s%s", ANSI_COLOR_BLUE, "TEST ls /1 ", ANSI_COLOR_RED, "FAILED\n", ANSI_COLOR_RESET);
		
	}
	else fprintf(stdout, "%s%s%s%s%s", ANSI_COLOR_BLUE, "TEST ls /1 ", ANSI_COLOR_GREEN, "SUCCESS\n", ANSI_COLOR_RESET);

	
	
	ret = unmountFS();
	if (ret != 0)
	{
		fprintf(stdout, "%s%s%s%s%s", ANSI_COLOR_BLUE, "TEST unmountFS ", ANSI_COLOR_RED, "FAILED\n", ANSI_COLOR_RESET);
		return -1;
	}

	fprintf(stdout, "%s%s%s%s%s", ANSI_COLOR_BLUE, "TEST unmountFS ", ANSI_COLOR_GREEN, "SUCCESS\n", ANSI_COLOR_RESET);

	///////

	return 0;
}
