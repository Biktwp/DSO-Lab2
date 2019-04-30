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
	 * Expexted: Create all the paths successfully.
	 */
	for(int i = 0; i < 10; i++){
		char cosa[99];
		sprintf(cosa,"/%d",i);
		ret = mkDir(cosa);
		if (ret != 0)fprintf(stdout, "%s%s%s%s%s", ANSI_COLOR_BLUE, "TEST 1 creating 10 directories 1 ", ANSI_COLOR_RED, "FAILED\n", ANSI_COLOR_RESET);
	}
	fprintf(stdout, "%s%s%s%s%s", ANSI_COLOR_BLUE, "TEST 1 creating 10 directories 1 ", ANSI_COLOR_GREEN, "SUCCESS\n", ANSI_COLOR_RESET);

	for(int i = 0; i < 10; i++){
		char cosa[99];
		sprintf(cosa,"/1/%d",i);
		ret = mkDir(cosa);
		if (ret != 0) fprintf(stdout, "%s%s%s%s%s", ANSI_COLOR_BLUE, "TEST 1 creating 10 directories 2 ", ANSI_COLOR_RED, "FAILED\n", ANSI_COLOR_RESET);
	}
	fprintf(stdout, "%s%s%s%s%s", ANSI_COLOR_BLUE, "TEST 1 creating 10 directories 2 ", ANSI_COLOR_GREEN, "SUCCESS\n", ANSI_COLOR_RESET);

	for(int i = 0; i < 10; i++){
		char cosa[99];
		sprintf(cosa,"/1/1/%d",i);
		ret = mkDir(cosa);
		if (ret != 0)fprintf(stdout, "%s%s%s%s%s", ANSI_COLOR_BLUE, "TEST 1 creating 10 directories 3 ", ANSI_COLOR_RED, "FAILED\n", ANSI_COLOR_RESET);
	}
	fprintf(stdout, "%s%s%s%s%s", ANSI_COLOR_BLUE, "TEST creating 10 directories 3 ", ANSI_COLOR_GREEN, "SUCCESS\n", ANSI_COLOR_RESET);

	for(int i = 0; i < 9; i++){
		char cosa[99];
		sprintf(cosa,"/1/2/%d",i);
		ret = mkDir(cosa);
		if (ret != 0)fprintf(stdout, "%s%s%s%s%s", ANSI_COLOR_BLUE, "TEST 1 creating 10 directories 4 ", ANSI_COLOR_RED, "FAILED\n", ANSI_COLOR_RESET);
	}
	fprintf(stdout, "%s%s%s%s%s", ANSI_COLOR_BLUE, "TEST 1 creating 10 directories 4 ", ANSI_COLOR_GREEN, "SUCCESS\n", ANSI_COLOR_RESET);

	/*
	 * Test 2: create a directory with depth = 4.
	 * Error expected.
	 */
	ret = mkDir("/1/1/1/1");
	if (ret == 0) fprintf(stdout, "%s%s%s%s%s", ANSI_COLOR_BLUE, "TEST 2 create 4 depth directory ", ANSI_COLOR_RED, "FAILED\n", ANSI_COLOR_RESET);
	else fprintf(stdout, "%s%s%s%s%s", ANSI_COLOR_BLUE, "TEST 2 create 4 depth directory ", ANSI_COLOR_GREEN, "SUCCESS\n", ANSI_COLOR_RESET);

	/*
	 * Test 3: create a file, the number 41, more than the maximum allowed.
	 * Expected: Error.
	 */	
	ret = createFile("/1/1/1/cosa.txt");
	if (ret == 0) fprintf(stdout, "%s%s%s%s%s", ANSI_COLOR_BLUE, "TEST 3 create 41th file ", ANSI_COLOR_RED, "FAILED\n", ANSI_COLOR_RESET);
	else fprintf(stdout, "%s%s%s%s%s", ANSI_COLOR_BLUE, "TEST 3 create 41th file ", ANSI_COLOR_GREEN, "SUCCESS\n", ANSI_COLOR_RESET);

	/*
	 * Test 4: Remove the directory "/1/1/1" and all its children directoies and files.
	 * Expected: deletion with no problems.
	 */
	ret = rmDir("/1/1/1");
	if (ret != 0)
	{
		fprintf(stdout, "%s%s%s%s%s", ANSI_COLOR_BLUE, "TEST 4 remove /1/1/1 directory ", ANSI_COLOR_RED, "FAILED\n", ANSI_COLOR_RESET);
	}
	else fprintf(stdout, "%s%s%s%s%s", ANSI_COLOR_BLUE, "TEST 4 remove /1/1/1 directory ", ANSI_COLOR_GREEN, "SUCCESS\n", ANSI_COLOR_RESET);
	/*
	 * Test 5: Create the eleventh directory.
	 * Error expected.
	 */
	ret = mkDir("/11");
	if (ret == 0) fprintf(stdout, "%s%s%s%s%s", ANSI_COLOR_BLUE, "TEST 5  Create the eleventh directory ", ANSI_COLOR_RED, "FAILED\n", ANSI_COLOR_RESET);
	else fprintf(stdout, "%s%s%s%s%s", ANSI_COLOR_BLUE, "TEST 5  Create the eleventh directory ", ANSI_COLOR_GREEN, "SUCCESS\n", ANSI_COLOR_RESET);

	/*
	 * Test 6: Create the eleventh file.
	 * Expected: Error .
	 */	
	ret = createFile("/cosa.txt");
	if (ret == 0) fprintf(stdout, "%s%s%s%s%s", ANSI_COLOR_BLUE, "TEST 6 Create the eleventh file ", ANSI_COLOR_RED, "FAILED\n", ANSI_COLOR_RESET);
	else fprintf(stdout, "%s%s%s%s%s", ANSI_COLOR_BLUE, "TEST 6 Create the eleventh file ", ANSI_COLOR_GREEN, "SUCCESS\n", ANSI_COLOR_RESET);

	/*
	 * Test 7: Remove the directory "/1" and all its children directoies and files
	 * Expected: deletion with no problems.
	 */
	ret = rmDir("/1");
	if (ret != 0) fprintf(stdout, "%s%s%s%s%s", ANSI_COLOR_BLUE, "TEST 7 create 41 file ", ANSI_COLOR_RED, "FAILED\n", ANSI_COLOR_RESET);
	else fprintf(stdout, "%s%s%s%s%s", ANSI_COLOR_BLUE, "TEST 7 create 41 file ", ANSI_COLOR_GREEN, "SUCCESS\n", ANSI_COLOR_RESET);

	/*
	 * Test 8: list all the directories from root, to check the method and the previous test.
	 * Expected: 9 directories, from 0 to 9 without directory "1".
	 */
	ret = lsDir("/",inodesDir,namesDir);
	if (ret < 0) fprintf(stdout, "%s%s%s%s%s", ANSI_COLOR_BLUE, "TEST 8 ls / ", ANSI_COLOR_RED, "FAILED\n", ANSI_COLOR_RESET);
	else fprintf(stdout, "%s%s%s%s%s", ANSI_COLOR_BLUE, "TEST 8 ls / ", ANSI_COLOR_GREEN, "SUCCESS\n", ANSI_COLOR_RESET);
	
	/*
	 * Test 9: list all the directories of a directory that does not exits, to check the method and the previous test.
	 * Expected: Error.
	 */
	ret = lsDir("/1",inodesDir,namesDir);
	if (ret == 0)fprintf(stdout, "%s%s%s%s%s", ANSI_COLOR_BLUE, "TEST 9 ls /1 ", ANSI_COLOR_RED, "FAILED\n", ANSI_COLOR_RESET);
	else fprintf(stdout, "%s%s%s%s%s", ANSI_COLOR_BLUE, "TEST 9 ls /1 ", ANSI_COLOR_GREEN, "SUCCESS\n", ANSI_COLOR_RESET);

	/*
	 * Test 10: Create a directory in a non-existent directory.
	 * Expected: Error.
	 */
	ret = mkDir("/1/2");
	if (ret != 0) fprintf(stdout, "%s%s%s%s%s", ANSI_COLOR_BLUE, "TEST 10 creating a directory in a non-existent one ", ANSI_COLOR_GREEN, "SUCCESS\n", ANSI_COLOR_RESET);
	else fprintf(stdout, "%s%s%s%s%s", ANSI_COLOR_BLUE, "TEST 10 creating a directory in a non-existent one ", ANSI_COLOR_RED, "FAILED\n", ANSI_COLOR_RESET);
	
	/*
	 * Test 11: Create a directory with 33 characters in its name.
	 * Expected: Error.
	 */
	ret = mkDir("/2/012345678901234567890123456789012");
	if (ret != 0) fprintf(stdout, "%s%s%s%s%s", ANSI_COLOR_BLUE, "TEST 11 creating a 33 name directory ", ANSI_COLOR_GREEN, "SUCCESS\n", ANSI_COLOR_RESET);
	else fprintf(stdout, "%s%s%s%s%s", ANSI_COLOR_BLUE, "TEST 11 creating a 33 name directory ", ANSI_COLOR_RED, "FAILED\n", ANSI_COLOR_RESET);
	
	/*
	 * Test 12: Create a file with 33 characters in its name.
	 * Expected: Error.
	 */
	ret = createFile("/3/012345678901234567890123456789.txt");
	if (ret != 0) fprintf(stdout, "%s%s%s%s%s", ANSI_COLOR_BLUE, "TEST 12 creating a 33 name file ", ANSI_COLOR_GREEN, "SUCCESS\n", ANSI_COLOR_RESET);
	else fprintf(stdout, "%s%s%s%s%s", ANSI_COLOR_BLUE, "TEST 12 creating a 33 name file ", ANSI_COLOR_RED, "FAILED\n", ANSI_COLOR_RESET);
	/*
	 * Test 13: Create a directory with 32 characters in its name depth 0.
	 * Expected: Create the directory successfully.
	 */
	ret = mkDir("/01234567890123456789012345678901");
	if (ret != 0) fprintf(stdout, "%s%s%s%s%s", ANSI_COLOR_BLUE, "TEST 13 creating a 32 name directory depth 0 ", ANSI_COLOR_RED, "FAILED\n", ANSI_COLOR_RESET);
	else fprintf(stdout, "%s%s%s%s%s", ANSI_COLOR_BLUE, "TEST 13 creating a 32 name directory depth 0 ", ANSI_COLOR_GREEN, "SUCCESS\n", ANSI_COLOR_RESET);

	/*
	 * Test 14: Create a directory with 32 characters in its name depth 1.
	 * Expected: Create the directory successfully.
	 */
	ret = mkDir("/01234567890123456789012345678901/01234567890123456789012345678901");
	if (ret != 0) fprintf(stdout, "%s%s%s%s%s", ANSI_COLOR_BLUE, "TEST 14 creating a 32 name directory depth 1 ", ANSI_COLOR_RED, "FAILED\n", ANSI_COLOR_RESET);
	else fprintf(stdout, "%s%s%s%s%s", ANSI_COLOR_BLUE, "TEST 14 creating a 32 name directory depth 1 ", ANSI_COLOR_GREEN, "SUCCESS\n", ANSI_COLOR_RESET);

	/*
	 * Test 15: Create a directory with 32 characters in its name depth 2.
	 * Expected: Create the directory successfully.
	 */
	ret = mkDir("/01234567890123456789012345678901/01234567890123456789012345678901/01234567890123456789012345678901");
	if (ret != 0)fprintf(stdout, "%s%s%s%s%s", ANSI_COLOR_BLUE, "TEST 15 creating a 32 name directory depth 2 99 length path ", ANSI_COLOR_RED, "FAILED\n", ANSI_COLOR_RESET);
	else fprintf(stdout, "%s%s%s%s%s", ANSI_COLOR_BLUE, "TEST 15 creating a 32 name directory depth 2 and 99 length path ", ANSI_COLOR_GREEN, "SUCCESS\n", ANSI_COLOR_RESET);
	
	/*
	 * Test 16: Create a directory with a path of 100 characters.
	 * Expected: Error.
	 */
	ret = mkDir("/01234567890123456789012345678901/01234567890123456789012345678901/012345678901234567890123456789012");
	if (ret != 0) fprintf(stdout, "%s%s%s%s%s", ANSI_COLOR_BLUE, "TEST 16 creating a directory with 100 ", ANSI_COLOR_GREEN, "SUCCESS\n", ANSI_COLOR_RESET);
	else fprintf(stdout, "%s%s%s%s%s", ANSI_COLOR_BLUE, "TEST 16 creating a directory with 100 ", ANSI_COLOR_RED, "FAILED\n", ANSI_COLOR_RESET);
	
	/*
	 * Test 17: Create a file with a name of 32 characters and depth 3.
	 * Expected: Create the file.
	 */
	ret = createFile("/01234567890123456789012345678901/01234567890123456789012345678901/01234567890123456789012345678901/012345678901234567890123456789.t");
	if (ret != 0) fprintf(stdout, "%s%s%s%s%s", ANSI_COLOR_BLUE, "TEST 17 creating a 32 name file depth 3 ", ANSI_COLOR_RED, "FAILED\n", ANSI_COLOR_RESET);
	else fprintf(stdout, "%s%s%s%s%s", ANSI_COLOR_BLUE, "TEST 17 creating a 32 name file depth 3 ", ANSI_COLOR_GREEN, "SUCCESS\n", ANSI_COLOR_RESET);

	/*
	 * Test 18: Creating a file with a path with a lenght of 132.
	 * Expected: Create the file.
	 */
	ret = createFile("/01234567890123456789012345678901/01234567890123456789012345678901/01234567890123456789012345678901/012345678901234567890123456789.e");
	if (ret != 0)fprintf(stdout, "%s%s%s%s%s", ANSI_COLOR_BLUE, "TEST 18 creating a file with a path with a lenght of 132 ", ANSI_COLOR_RED, "FAILED\n", ANSI_COLOR_RESET);
	else fprintf(stdout, "%s%s%s%s%s", ANSI_COLOR_BLUE, "TEST 18 creating a file with a path with a lenght of 132 ", ANSI_COLOR_GREEN, "SUCCESS\n", ANSI_COLOR_RESET);
	
	/*
	 * Test 19: Creating a file with a path of length equal 133.
	 * Expected: Error.
	 */
	ret = createFile("/01234567890123456789012345678901/01234567890123456789012345678901/01234567890123456789012345678901/012345678901234567890123456789.ess");
	if (ret != 0)fprintf(stdout, "%s%s%s%s%s", ANSI_COLOR_BLUE, "TEST 19 creating a file with path length 133 ", ANSI_COLOR_GREEN, "SUCCESS\n", ANSI_COLOR_RESET);
	else fprintf(stdout, "%s%s%s%s%s", ANSI_COLOR_BLUE, "TEST 19 creating a file with path length 133 ", ANSI_COLOR_RED, "FAILED\n", ANSI_COLOR_RESET);

	/*
	 * Test 20: Removing a file with a name of 32 characters and depth 3.
	 * Expected: Remove the file.
	 */
	ret = removeFile("/01234567890123456789012345678901/01234567890123456789012345678901/01234567890123456789012345678901/012345678901234567890123456789.t");
	if (ret != 0) fprintf(stdout, "%s%s%s%s%s", ANSI_COLOR_BLUE, "TEST 20 removing a file of 32 name length and depth 3 ", ANSI_COLOR_RED, "FAILED\n", ANSI_COLOR_RESET);
	else fprintf(stdout, "%s%s%s%s%s", ANSI_COLOR_BLUE, "TEST 20 removing a file of 32 name length and depth 3 ", ANSI_COLOR_GREEN, "SUCCESS\n", ANSI_COLOR_RESET);
	 
	/*
	 * Test 21: Removing a file with a path of length equal 132.
	 * Expected: Remove the file.
	 */
	ret = removeFile("/01234567890123456789012345678901/01234567890123456789012345678901/01234567890123456789012345678901/012345678901234567890123456789.e");
	if (ret != 0) fprintf(stdout, "%s%s%s%s%s", ANSI_COLOR_BLUE, "TEST 21 removing a file with path length 132 depth 3 ", ANSI_COLOR_RED, "FAILED\n", ANSI_COLOR_RESET);
	else fprintf(stdout, "%s%s%s%s%s", ANSI_COLOR_BLUE, "TEST 21 removing a file with path length 132 depth 3 ", ANSI_COLOR_GREEN, "SUCCESS\n", ANSI_COLOR_RESET);

	/*
	 * Test 22: Removing a directory with a path of length equal 99.
	 * Expected: Remove the directory.
	 */
	ret = rmDir("/01234567890123456789012345678901/01234567890123456789012345678901/01234567890123456789012345678901");
	if (ret != 0) fprintf(stdout, "%s%s%s%s%s", ANSI_COLOR_BLUE, "TEST 22 removing a directory with path length 99 ", ANSI_COLOR_RED, "FAILED\n", ANSI_COLOR_RESET);
	else fprintf(stdout, "%s%s%s%s%s", ANSI_COLOR_BLUE, "TEST 22 removing a directory with path length 99 ", ANSI_COLOR_GREEN, "SUCCESS\n", ANSI_COLOR_RESET);

	/*
	 * Test 23: Removing a directory with a name of 32 character.
	 * Expected: Remove the directory.
	 */
	ret = rmDir("/01234567890123456789012345678901");
	if (ret != 0) fprintf(stdout, "%s%s%s%s%s", ANSI_COLOR_BLUE, "TEST 23 removing a directory with length name 32 ", ANSI_COLOR_RED, "FAILED\n", ANSI_COLOR_RESET);
	else fprintf(stdout, "%s%s%s%s%s", ANSI_COLOR_BLUE, "TEST 23 removing a directory with length name 32 ", ANSI_COLOR_GREEN, "SUCCESS\n", ANSI_COLOR_RESET);

	/*
	 * Test 24: List an 100 path directory
	 * Expected: Error.
	 */
	ret = lsDir("/01234567890123456789012345678901/01234567890123456789012345678901/012345678901234567890123456789012",inodesDir,namesDir);
	if (ret == 0)fprintf(stdout, "%s%s%s%s%s", ANSI_COLOR_BLUE, "TEST 24 ls /01234567890123456789012345678901/01234567890123456789012345678901/012345678901234567890123456789012 ", ANSI_COLOR_RED, "FAILED\n", ANSI_COLOR_RESET);
	else fprintf(stdout, "%s%s%s%s%s", ANSI_COLOR_BLUE, "TEST 24 ls /01234567890123456789012345678901/01234567890123456789012345678901/012345678901234567890123456789012 ", ANSI_COLOR_GREEN, "SUCCESS\n", ANSI_COLOR_RESET);

	/*
	 * Test 25: Create a file.
	 * Expected: Create the file.
	 */
	ret = createFile("/file.txt");
	if (ret < 0)fprintf(stdout, "%s%s%s%s%s", ANSI_COLOR_BLUE, "TEST 25 create file /file.txt ", ANSI_COLOR_RED, "FAILED\n", ANSI_COLOR_RESET);
	else fprintf(stdout, "%s%s%s%s%s", ANSI_COLOR_BLUE, "TEST 25 create file /file.txt ", ANSI_COLOR_GREEN, "SUCCESS\n", ANSI_COLOR_RESET);

	/*
	 * Test 26: List in a file.
	 * Expected: Error.
	 */
	ret = lsDir("/file.txt",inodesDir,namesDir);
	if (ret == 0)fprintf(stdout, "%s%s%s%s%s", ANSI_COLOR_BLUE, "TEST 26 ls /file.txt ", ANSI_COLOR_RED, "FAILED\n", ANSI_COLOR_RESET);
	else fprintf(stdout, "%s%s%s%s%s", ANSI_COLOR_BLUE, "TEST 26 /file.txt ", ANSI_COLOR_GREEN, "SUCCESS\n", ANSI_COLOR_RESET);

	/*
	 * Test 27: Create a file in a file.
	 * Expected: Error.
	 */
	ret = createFile("/file.txt/hello.txt");
	if (ret == 0)fprintf(stdout, "%s%s%s%s%s", ANSI_COLOR_BLUE, "TEST 27 create file /file.txt/hello.txt ", ANSI_COLOR_RED, "FAILED\n", ANSI_COLOR_RESET);
	else fprintf(stdout, "%s%s%s%s%s", ANSI_COLOR_BLUE, "TEST 27 create a file /file.txt/hello.txt ", ANSI_COLOR_GREEN, "SUCCESS\n", ANSI_COLOR_RESET);

	/*
	 * Test 28: Create a directory in a file.
	 * Expected: Error.
	 */
	ret = mkDir("/file.txt/hello");
	if (ret == 0)fprintf(stdout, "%s%s%s%s%s", ANSI_COLOR_BLUE, "TEST 28 create a directory /file.txt/hello ", ANSI_COLOR_RED, "FAILED\n", ANSI_COLOR_RESET);
	else fprintf(stdout, "%s%s%s%s%s", ANSI_COLOR_BLUE, "TEST 28 create a directory /file.txt/hello ", ANSI_COLOR_GREEN, "SUCCESS\n", ANSI_COLOR_RESET);

	/*
	 * Test 29: Remove file.
	 * Expected: Error.
	 */
	ret = rmDir("/file.txt");
	if (ret == 0)fprintf(stdout, "%s%s%s%s%s", ANSI_COLOR_BLUE, "TEST 29 remove /file.txt ", ANSI_COLOR_RED, "FAILED\n", ANSI_COLOR_RESET);
	else fprintf(stdout, "%s%s%s%s%s", ANSI_COLOR_BLUE, "TEST 29 remove /file.txt/hello ", ANSI_COLOR_GREEN, "SUCCESS\n", ANSI_COLOR_RESET);

	/*
	 * Test 30: Remove non-existent file.
	 * Expected: Error.
	 */
	ret = removeFile("/2/file.txt");
	if (ret == 0)fprintf(stdout, "%s%s%s%s%s", ANSI_COLOR_BLUE, "TEST 30 remove  /2/file.txt ", ANSI_COLOR_RED, "FAILED\n", ANSI_COLOR_RESET);
	else fprintf(stdout, "%s%s%s%s%s", ANSI_COLOR_BLUE, "TEST 30 remove  /2/file.tx ", ANSI_COLOR_GREEN, "SUCCESS\n", ANSI_COLOR_RESET);

	/*
	 * Test 31: Remove a directory.
	 * Expected: Error.
	 */
	ret = removeFile("/2");
	if (ret == 0)fprintf(stdout, "%s%s%s%s%s", ANSI_COLOR_BLUE, "TEST 31 remove /2 ", ANSI_COLOR_RED, "FAILED\n", ANSI_COLOR_RESET);
	else fprintf(stdout, "%s%s%s%s%s", ANSI_COLOR_BLUE, "TEST 31 remove /2 ", ANSI_COLOR_GREEN, "SUCCESS\n", ANSI_COLOR_RESET);

	/*
	 * Test 32: Remove root.
	 * Expected: Error.
	 */
	ret = removeFile("/");
	if (ret == 0)fprintf(stdout, "%s%s%s%s%s", ANSI_COLOR_BLUE, "TEST 32 remove / ", ANSI_COLOR_RED, "FAILED\n", ANSI_COLOR_RESET);
	else fprintf(stdout, "%s%s%s%s%s", ANSI_COLOR_BLUE, "TEST 32 remove / ", ANSI_COLOR_GREEN, "SUCCESS\n", ANSI_COLOR_RESET);

	/*
	 * Test 32: Remove root.
	 * Expected: Error.
	 */
	ret = rmDir("/");
	if (ret == 0)fprintf(stdout, "%s%s%s%s%s", ANSI_COLOR_BLUE, "TEST 32 remove / ", ANSI_COLOR_RED, "FAILED\n", ANSI_COLOR_RESET);
	else fprintf(stdout, "%s%s%s%s%s", ANSI_COLOR_BLUE, "TEST 32 remove / ", ANSI_COLOR_GREEN, "SUCCESS\n", ANSI_COLOR_RESET);

	/*
	 * Test 33: Remove root.
	 * Expected: Error.
	 */
	ret = openFile("/2");
	if (ret == 0)fprintf(stdout, "%s%s%s%s%s", ANSI_COLOR_BLUE, "TEST 33 file /2 ", ANSI_COLOR_RED, "FAILED\n", ANSI_COLOR_RESET);
	else fprintf(stdout, "%s%s%s%s%s", ANSI_COLOR_BLUE, "TEST 33 open file /2 ", ANSI_COLOR_GREEN, "SUCCESS\n", ANSI_COLOR_RESET);

	/*
	 * Test 34: Open a non-existent file.
	 * Expected: Error.
	 */
	ret = openFile("/2/file.txt");
	if (ret == 0)fprintf(stdout, "%s%s%s%s%s", ANSI_COLOR_BLUE, "TEST 34 open file /2/file.txt ", ANSI_COLOR_RED, "FAILED\n", ANSI_COLOR_RESET);
	else fprintf(stdout, "%s%s%s%s%s", ANSI_COLOR_BLUE, "TEST 34 open file /2/file.txt ", ANSI_COLOR_GREEN, "SUCCESS\n", ANSI_COLOR_RESET);



	/*
	 * Test 35: Create a file, open it, write some words and wrtie an other time to ckech if it continues where it finished before
	 * after that lseek to the position 5 of the file in order to read only the second word. After that, it close the file
	 * Expected: success in all the operations done.
	 */
	int fd = openFile("/file.txt");
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

	ret = writeFile(fd, " Bitches", 8);
	if (ret != 8)
	{
		fprintf(stdout, "%s%s%s%s%s", ANSI_COLOR_BLUE, "TEST 35 wrtie ", ANSI_COLOR_RED, "FAILED\n", ANSI_COLOR_RESET);
		
	}
	else fprintf(stdout, "%s%s%s%s%s", ANSI_COLOR_BLUE, "TEST 35 write ", ANSI_COLOR_GREEN, "SUCCESS\n", ANSI_COLOR_RESET);

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

	void *result = malloc(sizeof(char[BLOCK_SIZE]));
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
