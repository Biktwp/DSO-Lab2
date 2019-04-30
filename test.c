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
#include "include/auxiliary.h"

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
	//void *result = malloc(sizeof(char[BLOCK_SIZE]));
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
    
    /* Test 1: Create, ls and remove a directory*/
    /* Test 2: Create, ls and remove a file*/
	/*
	 * Test 3: Create a file, open it, write some words and write another time to cech if it continues where it finished before
	 * after that lseek to the position 5 of the file in order to read only the second word. After that, it close the file
	 * Expected: success in all the operations done.
     * Test 3: Create a file write some content 
	 */
    /*
    ret = createFile("/file.txt");
	if (ret < 0)fprintf(stdout, "%s%s%s%s%s", ANSI_COLOR_BLUE, "TEST 25 create file /file.txt ", ANSI_COLOR_RED, "FAILED\n", ANSI_COLOR_RESET);
	else fprintf(stdout, "%s%s%s%s%s", ANSI_COLOR_BLUE, "TEST 25 create file /file.txt ", ANSI_COLOR_GREEN, "SUCCESS\n", ANSI_COLOR_RESET);
*/
    ret = mkDir("/1");
	if (ret < 0)fprintf(stdout, "%s%s%s%s%s", ANSI_COLOR_BLUE, "TEST 25 create /1 ", ANSI_COLOR_RED, "FAILED\n", ANSI_COLOR_RESET);
	else fprintf(stdout, "%s%s%s%s%s", ANSI_COLOR_BLUE, "TEST 25 create /1 ", ANSI_COLOR_GREEN, "SUCCESS\n", ANSI_COLOR_RESET);

    ret = mkDir("/1/1");
	if (ret < 0)fprintf(stdout, "%s%s%s%s%s", ANSI_COLOR_BLUE, "TTEST 25 create /1/1 ", ANSI_COLOR_RED, "FAILED\n", ANSI_COLOR_RESET);
	else fprintf(stdout, "%s%s%s%s%s", ANSI_COLOR_BLUE, "TTTEST 25 create /1/1 ", ANSI_COLOR_GREEN, "SUCCESS\n", ANSI_COLOR_RESET);

    ret = mkDir("/1/1/1");
	if (ret < 0)fprintf(stdout, "%s%s%s%s%s", ANSI_COLOR_BLUE, "TTEST 25 create /1/1/1 ", ANSI_COLOR_RED, "FAILED\n", ANSI_COLOR_RESET);
	else fprintf(stdout, "%s%s%s%s%s", ANSI_COLOR_BLUE, "TTEST 25 create /1/1/1 ", ANSI_COLOR_GREEN, "SUCCESS\n", ANSI_COLOR_RESET);

    ret = createFile("/1/1/1/file.txt");
	if (ret < 0)fprintf(stdout, "%s%s%s%s%s", ANSI_COLOR_BLUE, "TTEST 25 create /1/1/1/file.txt ", ANSI_COLOR_RED, "FAILED\n", ANSI_COLOR_RESET);
	else fprintf(stdout, "%s%s%s%s%s", ANSI_COLOR_BLUE, "TTEST 25 create /1/1/1/file.txt ", ANSI_COLOR_GREEN, "SUCCESS\n", ANSI_COLOR_RESET);

    ret = mkDir("/2");
	if (ret < 0)fprintf(stdout, "%s%s%s%s%s", ANSI_COLOR_BLUE, "TEST 25 create /2 ", ANSI_COLOR_RED, "FAILED\n", ANSI_COLOR_RESET);
	else fprintf(stdout, "%s%s%s%s%s", ANSI_COLOR_BLUE, "TEST 25 create /2 ", ANSI_COLOR_GREEN, "SUCCESS\n", ANSI_COLOR_RESET);

    ret = mkDir("/2/1");
	if (ret < 0)fprintf(stdout, "%s%s%s%s%s", ANSI_COLOR_BLUE, "TEST 25 create /2/1 ", ANSI_COLOR_RED, "FAILED\n", ANSI_COLOR_RESET);
	else fprintf(stdout, "%s%s%s%s%s", ANSI_COLOR_BLUE, "TEST 25 create /2/1 ", ANSI_COLOR_GREEN, "SUCCESS\n", ANSI_COLOR_RESET);

    ret = mkDir("/2/1/1");
	if (ret < 0)fprintf(stdout, "%s%s%s%s%s", ANSI_COLOR_BLUE, "TEST 25 create /2/1/1 ", ANSI_COLOR_RED, "FAILED\n", ANSI_COLOR_RESET);
	else fprintf(stdout, "%s%s%s%s%s", ANSI_COLOR_BLUE, "TEST 25 create /2/1/1 ", ANSI_COLOR_GREEN, "SUCCESS\n", ANSI_COLOR_RESET);

    ret = createFile("/2/1/1/file.txt");
	if (ret < 0)fprintf(stdout, "%s%s%s%s%s", ANSI_COLOR_BLUE, "TEST 25 create /2/1/1/file.txt ", ANSI_COLOR_RED, "FAILED\n", ANSI_COLOR_RESET);
	else fprintf(stdout, "%s%s%s%s%s", ANSI_COLOR_BLUE, "TEST 25 create /2/1/1/file.txt ", ANSI_COLOR_GREEN, "SUCCESS\n", ANSI_COLOR_RESET);
    
    
    ret = lsDir("/",inodesDir,namesDir);
	if (ret < 0) fprintf(stdout, "%s%s%s%s%s", ANSI_COLOR_BLUE, "TEST 8 ls / ", ANSI_COLOR_RED, "FAILED\n", ANSI_COLOR_RESET);
	else fprintf(stdout, "%s%s%s%s%s", ANSI_COLOR_BLUE, "TEST 8 ls / ", ANSI_COLOR_GREEN, "SUCCESS\n", ANSI_COLOR_RESET);
	namei();
    /*
	fd = openFile("/file.txt");
	if (fd < 0)
	{
		fprintf(stdout, "%s%s%s%s%s", ANSI_COLOR_BLUE, "TEST 35 open file for wrtie ", ANSI_COLOR_RED, "FAILED\n", ANSI_COLOR_RESET);
		
	}
	else fprintf(stdout, "%s%s%s%s%s", ANSI_COLOR_BLUE, "TEST open file for write ", ANSI_COLOR_GREEN, "SUCCESS\n", ANSI_COLOR_RESET);


	ret = writeFile(fd, "Hello World", 11);
	if (ret != 11)
	{
		fprintf(stdout, "%s%s%s%s%s", ANSI_COLOR_BLUE, "TEST 35 wrtie ", ANSI_COLOR_RED, "FAILED\n", ANSI_COLOR_RESET);
		
	}
	else fprintf(stdout, "%s%s%s%s%s", ANSI_COLOR_BLUE, "TEST 35 write ", ANSI_COLOR_GREEN, "SUCCESS\n", ANSI_COLOR_RESET);

	ret = writeFile(fd, " Anthoni", 8);
	if (ret != 8)
	{
		fprintf(stdout, "%s%s%s%s%s", ANSI_COLOR_BLUE, "TEST 35 wrtie ", ANSI_COLOR_RED, "FAILED\n", ANSI_COLOR_RESET);
		
	}
	else fprintf(stdout, "%s%s%s%s%s", ANSI_COLOR_BLUE, "TEST 35 write ", ANSI_COLOR_GREEN, "SUCCESS\n", ANSI_COLOR_RESET);

    ret = readFile(fd, result, 5);
	if (ret != 5)
	{
		fprintf(stdout, "%s%s%s%s%s", ANSI_COLOR_BLUE, "TEST 35 read ", ANSI_COLOR_RED, "FAILED\n", ANSI_COLOR_RESET);
		
	}
	else fprintf(stdout, "%s%s%s%s%s", ANSI_COLOR_BLUE, "TEST 35 read ", ANSI_COLOR_GREEN, "SUCCESS\n", ANSI_COLOR_RESET);

	ret = lseekFile(fd, 0, FS_SEEK_BEGIN);
	if (ret != 0)
	{
		fprintf(stdout, "%s%s%s%s%s", ANSI_COLOR_BLUE, "TEST 35 lseek file for read ", ANSI_COLOR_RED, "FAILED\n", ANSI_COLOR_RESET);
		
	}
	else fprintf(stdout, "%s%s%s%s%s", ANSI_COLOR_BLUE, "TEST 35 lseek file for read ", ANSI_COLOR_GREEN, "SUCCESS\n", ANSI_COLOR_RESET);

	ret = lseekFile(fd, 6, FS_SEEK_CUR);
	if (ret != 0)
	{
		fprintf(stdout, "%s%s%s%s%s", ANSI_COLOR_BLUE, "TEST 35 lseek file for read ", ANSI_COLOR_RED, "FAILED\n", ANSI_COLOR_RESET);
		
	}
	else fprintf(stdout, "%s%s%s%s%s", ANSI_COLOR_BLUE, "TEST 35 lseek file for read ", ANSI_COLOR_GREEN, "SUCCESS\n", ANSI_COLOR_RESET);

	ret = readFile(fd, result, 5);
	if (ret != 5)
	{
		fprintf(stdout, "%s%s%s%s%s", ANSI_COLOR_BLUE, "TEST 35 read ", ANSI_COLOR_RED, "FAILED\n", ANSI_COLOR_RESET);
		
	}
	else fprintf(stdout, "%s%s%s%s%s", ANSI_COLOR_BLUE, "TEST 35 read ", ANSI_COLOR_GREEN, "SUCCESS\n", ANSI_COLOR_RESET);

	ret = closeFile(fd);
	if (ret < 0)
	{
		fprintf(stdout, "%s%s%s%s%s", ANSI_COLOR_BLUE, "TEST 35 close file ", ANSI_COLOR_RED, "FAILED\n", ANSI_COLOR_RESET);
		
	}
	else fprintf(stdout, "%s%s%s%s%s", ANSI_COLOR_BLUE, "TEST 35 close file ", ANSI_COLOR_GREEN, "SUCCESS\n", ANSI_COLOR_RESET);

    
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
/*
    ret = mountFS();
	if (ret != 0)
	{
		fprintf(stdout, "%s%s%s%s%s", ANSI_COLOR_BLUE, "TEST mountFS ", ANSI_COLOR_RED, "FAILED\n", ANSI_COLOR_RESET);
		return -1;
	}
	fprintf(stdout, "%s%s%s%s%s", ANSI_COLOR_BLUE, "TEST mountFS ", ANSI_COLOR_GREEN, "SUCCESS\n", ANSI_COLOR_RESET);
	
    ret = lsDir("/",inodesDir,namesDir);
	if (ret < 0) fprintf(stdout, "%s%s%s%s%s", ANSI_COLOR_BLUE, "TEST 8 ls / ", ANSI_COLOR_RED, "FAILED\n", ANSI_COLOR_RESET);
	else fprintf(stdout, "%s%s%s%s%s", ANSI_COLOR_BLUE, "TEST 8 ls / ", ANSI_COLOR_GREEN, "SUCCESS\n", ANSI_COLOR_RESET);
	
	ret = unmountFS();
	if (ret != 0)
	{
		fprintf(stdout, "%s%s%s%s%s", ANSI_COLOR_BLUE, "TEST unmountFS ", ANSI_COLOR_RED, "FAILED\n", ANSI_COLOR_RESET);
		return -1;
	}

	fprintf(stdout, "%s%s%s%s%s", ANSI_COLOR_BLUE, "TEST unmountFS ", ANSI_COLOR_GREEN, "SUCCESS\n", ANSI_COLOR_RESET);

    fd = openFile("/file.txt");
	if (fd < 0)
	{
		fprintf(stdout, "%s%s%s%s%s", ANSI_COLOR_BLUE, "TEST 35 open file for wrtie ", ANSI_COLOR_RED, "FAILED\n", ANSI_COLOR_RESET);
		
	}
	else fprintf(stdout, "%s%s%s%s%s", ANSI_COLOR_BLUE, "TEST open file for write ", ANSI_COLOR_GREEN, "SUCCESS\n", ANSI_COLOR_RESET);

    ret = readFile(fd, result, 5);
	if (ret != 5)
	{
		fprintf(stdout, "%s%s%s%s%s", ANSI_COLOR_BLUE, "TEST 35 read ", ANSI_COLOR_RED, "FAILED\n", ANSI_COLOR_RESET);
		
	}
	else fprintf(stdout, "%s%s%s%s%s", ANSI_COLOR_BLUE, "TEST 35 read ", ANSI_COLOR_GREEN, "SUCCESS\n", ANSI_COLOR_RESET);


	ret = lseekFile(fd, 0, FS_SEEK_BEGIN);
	if (ret != 0)
	{
		fprintf(stdout, "%s%s%s%s%s", ANSI_COLOR_BLUE, "TEST 35 lseek file for read ", ANSI_COLOR_RED, "FAILED\n", ANSI_COLOR_RESET);
		
	}
	else fprintf(stdout, "%s%s%s%s%s", ANSI_COLOR_BLUE, "TEST 35 lseek file for read ", ANSI_COLOR_GREEN, "SUCCESS\n", ANSI_COLOR_RESET);

	ret = lseekFile(fd, 6, FS_SEEK_CUR);
	if (ret != 0)
	{
		fprintf(stdout, "%s%s%s%s%s", ANSI_COLOR_BLUE, "TEST 35 lseek file for read ", ANSI_COLOR_RED, "FAILED\n", ANSI_COLOR_RESET);
		
	}
	else fprintf(stdout, "%s%s%s%s%s", ANSI_COLOR_BLUE, "TEST 35 lseek file for read ", ANSI_COLOR_GREEN, "SUCCESS\n", ANSI_COLOR_RESET);

	ret = readFile(fd, result, 5);
	if (ret != 5)
	{
		fprintf(stdout, "%s%s%s%s%s", ANSI_COLOR_BLUE, "TEST 35 read ", ANSI_COLOR_RED, "FAILED\n", ANSI_COLOR_RESET);
		
	}
	else fprintf(stdout, "%s%s%s%s%s", ANSI_COLOR_BLUE, "TEST 35 read ", ANSI_COLOR_GREEN, "SUCCESS\n", ANSI_COLOR_RESET);

	ret = closeFile(fd);
	if (ret < 0)
	{
		fprintf(stdout, "%s%s%s%s%s", ANSI_COLOR_BLUE, "TEST 35 close file ", ANSI_COLOR_RED, "FAILED\n", ANSI_COLOR_RESET);
		
	}
	else fprintf(stdout, "%s%s%s%s%s", ANSI_COLOR_BLUE, "TEST 35 close file ", ANSI_COLOR_GREEN, "SUCCESS\n", ANSI_COLOR_RESET);

*/
	return 0;
}