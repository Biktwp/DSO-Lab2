/*
 * OPERATING SYSTEMS DESING - 16/17
 *
 * @file 	test.c
 * @brief 	Implementation of the client test routines.
 * @date	01/03/2017
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
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

	/*
	 * Create the filesystem
	 */
	ret = mkFS(DEV_SIZE);
	if (ret != 0)
	{
		fprintf(stdout, "%s%s%s%s%s", ANSI_COLOR_BLUE, "TEST mkFS ", ANSI_COLOR_RED, "FAILED\n", ANSI_COLOR_RESET);
		return -1;
	}
	fprintf(stdout, "%s%s%s%s%s", ANSI_COLOR_BLUE, "TEST mkFS ", ANSI_COLOR_GREEN, "SUCCESS\n", ANSI_COLOR_RESET);

	/*
	 * Mount the filesystem
	 */
	ret = mountFS();
	if (ret != 0)
	{
		fprintf(stdout, "%s%s%s%s%s", ANSI_COLOR_BLUE, "TEST mountFS ", ANSI_COLOR_RED, "FAILED\n", ANSI_COLOR_RESET);
		return -1;
	}
	fprintf(stdout, "%s%s%s%s%s", ANSI_COLOR_BLUE, "TEST mountFS ", ANSI_COLOR_GREEN, "SUCCESS\n", ANSI_COLOR_RESET);

	/*
	 * Test 1: Create 39 directories, in 3 loops to change the directory names and paths,
	 * creating 10 directories in the root directory and the rest inside those directories
	 */
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

	for(int i = 0; i < 9; i++){
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

	/*
	 * Test 2: create a directory with depth = 4
	 * Error expected
	 */
	ret = mkDir("/1/1/1/1");
	if (ret == 0)
	{
		fprintf(stdout, "%s%s%s%s%s", ANSI_COLOR_BLUE, "TEST create 4 depth directory ", ANSI_COLOR_RED, "FAILED\n", ANSI_COLOR_RESET);
		
	}
	else fprintf(stdout, "%s%s%s%s%s", ANSI_COLOR_BLUE, "TEST create 4 depth directory ", ANSI_COLOR_GREEN, "SUCCESS\n", ANSI_COLOR_RESET);

	/*
	 * Test 3: create a file, the number 41, more than the maximum allowed
	 * Expected: Error
	 */	
	ret = createFile("/1/1/1/cosa.txt");
	if (ret == 0)
	{
		fprintf(stdout, "%s%s%s%s%s", ANSI_COLOR_BLUE, "TEST create 41th file ", ANSI_COLOR_RED, "FAILED\n", ANSI_COLOR_RESET);
		
	}
	else fprintf(stdout, "%s%s%s%s%s", ANSI_COLOR_BLUE, "TEST create 41th file ", ANSI_COLOR_GREEN, "SUCCESS\n", ANSI_COLOR_RESET);

	/*
	 * Test 4: Remove the directory "/1" and all its children directoies and files
	 * Expected: deletion with no problems
	 */
	ret = rmDir("/1");
	if (ret != 0)
	{
		fprintf(stdout, "%s%s%s%s%s", ANSI_COLOR_BLUE, "TEST create 41 file ", ANSI_COLOR_RED, "FAILED\n", ANSI_COLOR_RESET);
	}
	else fprintf(stdout, "%s%s%s%s%s", ANSI_COLOR_BLUE, "TEST create 41 file ", ANSI_COLOR_GREEN, "SUCCESS\n", ANSI_COLOR_RESET);

	/*
	 * Test 5: list all the directories from root, to check the method and the previous test.
	 * Expected: 9 directoies, from 0 to 9 without dirrectory "1"
	 */
	ret = lsDir("/",inodesDir,namesDir);
	if (ret < 0)
	{
		fprintf(stdout, "%s%s%s%s%s", ANSI_COLOR_BLUE, "TEST ls / ", ANSI_COLOR_RED, "FAILED\n", ANSI_COLOR_RESET);
		
	}
	else fprintf(stdout, "%s%s%s%s%s", ANSI_COLOR_BLUE, "TEST ls / ", ANSI_COLOR_GREEN, "SUCCESS\n", ANSI_COLOR_RESET);

	/*
	 * Test 6: list all the directories of a directory that does not exits, to check the method and the previous test.
	 * Expected: Error
	 */
	ret = lsDir("/1",inodesDir,namesDir);
	if (ret == 0)
	{
		fprintf(stdout, "%s%s%s%s%s", ANSI_COLOR_BLUE, "TEST ls /1 ", ANSI_COLOR_RED, "FAILED\n", ANSI_COLOR_RESET);
		
	}
	else fprintf(stdout, "%s%s%s%s%s", ANSI_COLOR_BLUE, "TEST ls /1 ", ANSI_COLOR_GREEN, "SUCCESS\n", ANSI_COLOR_RESET);

	/*
	 * Test 7: Create a file, open it, write some words and wrtie an other time to ckech if it continues where it finished before
	 * after that lseek to the position 5 of the file in order to read only the second word. After that, it close the file
	 * Expected: success in all the operations done.
	 */
	ret = createFile("/cosa.txt");
	if (ret < 0)
	{
		fprintf(stdout, "%s%s%s%s%s", ANSI_COLOR_BLUE, "TEST create file for wrtie ", ANSI_COLOR_RED, "FAILED\n", ANSI_COLOR_RESET);
		
	}
	else fprintf(stdout, "%s%s%s%s%s", ANSI_COLOR_BLUE, "TEST create file for write ", ANSI_COLOR_GREEN, "SUCCESS\n", ANSI_COLOR_RESET);

	int fd = openFile("/cosa.txt");
	if (fd < 0)
	{
		fprintf(stdout, "%s%s%s%s%s", ANSI_COLOR_BLUE, "TEST open file for wrtie ", ANSI_COLOR_RED, "FAILED\n", ANSI_COLOR_RESET);
		
	}
	else fprintf(stdout, "%s%s%s%s%s", ANSI_COLOR_BLUE, "TEST open file for write ", ANSI_COLOR_GREEN, "SUCCESS\n", ANSI_COLOR_RESET);


	ret = writeFile(fd, "Hello World", 11);
	if (ret != 11)
	{
		fprintf(stdout, "%s%s%s%s%s", ANSI_COLOR_BLUE, "TEST wrtie ", ANSI_COLOR_RED, "FAILED\n", ANSI_COLOR_RESET);
		
	}
	else fprintf(stdout, "%s%s%s%s%s", ANSI_COLOR_BLUE, "TEST write ", ANSI_COLOR_GREEN, "SUCCESS\n", ANSI_COLOR_RESET);

	ret = writeFile(fd, " Bitches", 8);
	if (ret != 8)
	{
		fprintf(stdout, "%s%s%s%s%s", ANSI_COLOR_BLUE, "TEST wrtie ", ANSI_COLOR_RED, "FAILED\n", ANSI_COLOR_RESET);
		
	}
	else fprintf(stdout, "%s%s%s%s%s", ANSI_COLOR_BLUE, "TEST write ", ANSI_COLOR_GREEN, "SUCCESS\n", ANSI_COLOR_RESET);

	ret = lseekFile(fd, 0, FS_SEEK_BEGIN);
	if (ret != 0)
	{
		fprintf(stdout, "%s%s%s%s%s", ANSI_COLOR_BLUE, "TEST lseek file for read ", ANSI_COLOR_RED, "FAILED\n", ANSI_COLOR_RESET);
		
	}
	else fprintf(stdout, "%s%s%s%s%s", ANSI_COLOR_BLUE, "TEST lseek file for read ", ANSI_COLOR_GREEN, "SUCCESS\n", ANSI_COLOR_RESET);

	ret = lseekFile(fd, 6, FS_SEEK_CUR);
	if (ret != 0)
	{
		fprintf(stdout, "%s%s%s%s%s", ANSI_COLOR_BLUE, "TEST lseek file for read ", ANSI_COLOR_RED, "FAILED\n", ANSI_COLOR_RESET);
		
	}
	else fprintf(stdout, "%s%s%s%s%s", ANSI_COLOR_BLUE, "TEST lseek file for read ", ANSI_COLOR_GREEN, "SUCCESS\n", ANSI_COLOR_RESET);

	void *result = malloc(sizeof(char[BLOCK_SIZE]));
	ret = readFile(fd, result, 5);
	if (ret != 5)
	{
		fprintf(stdout, "%s%s%s%s%s", ANSI_COLOR_BLUE, "TEST read ", ANSI_COLOR_RED, "FAILED\n", ANSI_COLOR_RESET);
		
	}
	else fprintf(stdout, "%s%s%s%s%s", ANSI_COLOR_BLUE, "TEST read ", ANSI_COLOR_GREEN, "SUCCESS\n", ANSI_COLOR_RESET);

	ret = closeFile(fd);
	if (ret < 0)
	{
		fprintf(stdout, "%s%s%s%s%s", ANSI_COLOR_BLUE, "TEST close file ", ANSI_COLOR_RED, "FAILED\n", ANSI_COLOR_RESET);
		
	}
	else fprintf(stdout, "%s%s%s%s%s", ANSI_COLOR_BLUE, "TEST close file ", ANSI_COLOR_GREEN, "SUCCESS\n", ANSI_COLOR_RESET);

	///

	/*
	 * Test n: Unmount the filesystem and write all the metadata in the disk
	 * Expected: Write meta data in disk
	 */
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
