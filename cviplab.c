/* =========================================================================
*
*   Computer Vision and Image Processing Lab - Dr. Scott Umbaugh SIUE
*
* =========================================================================
*
*             File Name: CVIPlab.c
*           Description: This  is the skeleton program for the Computer Vision
*			 and Image Processing Labs
*   Initial Coding Date: April 23, 1996
*   Last update Date: Jan31, 2017
*   .NET version:     September 21, 2006
*           Portability: Standard (ANSI) C
*             Credit(s): Scott Umbaugh, Zhen Li, Kun Luo,Dejun Zhang,Wenjun(Evan) Bian, Rohini Dahal, Norsang Lama
*                        Southern Illinois University Edwardsville
*  Copyright (C) 2004 Scott Umbaugh and SIUE
*
*  Permission to use, copy, modify, and distribute this software and its
*  documentation for any non-commercial purpose and without fee is hereby
*  granted, provided that the above copyright notice appear in all copies
*  and that both that copyright notice and this permission notice appear
*  in supporting documentation.  This software is provided "as is"
*  without express or implied warranty.
*
****************************************************************************/

/*
** include header files
*/

#include "CVIPtoolkit.h"
#include "CVIPconvert.h"
#include "CVIPdef.h"
#include "CVIPimage.h"
#include "CVIPlab.h"
//#include "CVIPview.h"


#include <string.h>
#include <dirent.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>	
#include <stdio.h>

#define SAMPLES 10
#define FILES_IN_DIR 200

#define CASE_MAX 20

/* Put the command here, as VIDEO_APP, to run your image acquisition
application program */

//#define VIDEO_APP "explorer.exe"

void main_cviplab() {


	IMAGE_FORMAT  format;     	/* the input image format */
	Image* cvipImage; 	    /* pointer to the CVIP Image structure */
	Image* cvipImage5;
	Image* cvipImage7; 	

	char* outputfile;	    /* output file name */
	int		     choice;
	CVIP_BOOLEAN  done = CVIP_NO;


	char* rootdir = "C:/sandbox/test_images/";

	char* outImgPath  = "C:/sandbox/results/";
	char* outImgPath5 = "C:/sandbox/results_5/";
	char* outImgPath7 = "C:/sandbox/results_7/";

	char* filtersTag  = "F3-7-";
	char* filtersTag5 = "F5-7-";
	char* filtersTag7 = "F7-7-";
	char* outImgName;
	char* inImgName;

	int med_fil_val        = 3;
	int med_fil_val5       = 5;
	int med_fil_val7       = 7;

	int adapt_med_fil_val   = 7;
	


	int threshold_segm_val = 169;

	int num_of_files_in_dir = 12;
	char* fileNames[FILES_IN_DIR];
	char* onlyFileNamesArr[FILES_IN_DIR];
	

	struct dirent* entry;
	struct stat filestat;
	DIR* folder;
	folder = opendir(rootdir);

	
	char* outImgName5;
	char* outImgName7;

	int thresholdValue[] = { 100, 125, 150, 169, 180 };


	if (folder == NULL)
	{
		puts("Unable to read directory");
		return (1);
	}

	printf("Openning Directory: %s \n\n", rootdir);

	int fileIter = 0;
	while ((entry = readdir(folder)))
	{
		stat(entry->d_name, &filestat);

		if (S_ISDIR(filestat.st_mode))
		{
			//printf("%4s: %s\n", "Dir", entry->d_name);
		}
		else
		{
	
			onlyFileNamesArr[fileIter] = (char*)malloc((strlen(entry->d_name))*sizeof(char) + 1);
			strcpy(onlyFileNamesArr[fileIter], entry->d_name);
			
		
			inImgName = (char*)malloc( (strlen(rootdir) + strlen(entry->d_name)) *sizeof(char) + 2);
			strcpy(inImgName, rootdir);
			strcat(inImgName, entry->d_name);
		

			fileNames[fileIter] = (char*)malloc((strlen(rootdir) + strlen(entry->d_name))*sizeof(char) + 1);
			strcpy(fileNames[fileIter], inImgName);

			fileIter++;

		}


	}

	
	closedir(folder);

	
	int idx[SAMPLES];
	int rand_lower_limit = 0;
	

	time_t t;
	srand((unsigned)time(&t));
	
	for (int i = 0; i < SAMPLES; i++) {

		bool check;
		int num_selected;

		do {
			num_selected = (rand() % (FILES_IN_DIR - rand_lower_limit) + 1) + rand_lower_limit;
			check = true;

			for (int j = 0; j < i; j++) {

				if (num_selected == idx[j]) {
					check = false;
					break;
				}
			}

		} while (!check);

		idx[i] = num_selected;

	}

	char buffer[sizeof(int) * 3 + 1];

	for (int i = 0; i < SAMPLES; i++) {

		printf("\n");
		printf("[PROCESSING IMAGE] %s\n", fileNames[idx[i]]);
		printf("[ONLY NAME]        %s\n", onlyFileNamesArr[idx[i]]);
		printf("----------------------------------------------------------\n");
		printf("path: %s\n", rootdir);
		
		
		
		//cvipImage  = read_Image(fileNames[idx[i]], 1);
		//cvipImage5 = read_Image(fileNames[idx[i]], 1);
		//cvipImage7 = read_Image(fileNames[idx[i]], 1);

		


		for (int k = 0; k < 5; k++)
		{
			cvipImage  = read_Image(fileNames[idx[i]], 1);
			cvipImage5 = read_Image(fileNames[idx[i]], 1);
			cvipImage7 = read_Image(fileNames[idx[i]], 1);

			if (cvipImage == NULL)
			{
				error_CVIP("main", "could not read input image");
			}


			itoa(thresholdValue[k], buffer, 10);

			// Processing Image with Median Filter = 3
			cvipImage = (Image*)median_filter(cvipImage, med_fil_val);
			cvipImage = (Image*)adapt_median_filter(cvipImage, adapt_med_fil_val);
			cvipImage = (Image*)adapt_median_filter(cvipImage, adapt_med_fil_val);
			cvipImage = (Image*)adapt_median_filter(cvipImage, adapt_med_fil_val);
			cvipImage = (Image*)adapt_median_filter(cvipImage, adapt_med_fil_val);
			cvipImage = (Image*)threshold_segment(cvipImage, thresholdValue[k], CVIP_YES);

		
			outImgName = (char*)malloc( (strlen(outImgPath) + strlen(onlyFileNamesArr[idx[i]]) + strlen(filtersTag) + strlen(buffer)+strlen("-Image-")) * sizeof(char) + 3);

			strcpy(outImgName, outImgPath);
			strcat(outImgName, filtersTag);
			strcat(outImgName, buffer);
			strcat(outImgName, "-Image-");
			strcat(outImgName, onlyFileNamesArr[idx[i]]);
		

			write_Image(cvipImage, outImgName, 1, 0, 33, 1);

			// Processing Image with Median Filter = 5

			

			cvipImage5 = (Image*)median_filter(cvipImage5, med_fil_val5);
			cvipImage5 = (Image*)adapt_median_filter(cvipImage5, adapt_med_fil_val);
			cvipImage5 = (Image*)adapt_median_filter(cvipImage5, adapt_med_fil_val);
			cvipImage5 = (Image*)adapt_median_filter(cvipImage5, adapt_med_fil_val);
			cvipImage5 = (Image*)adapt_median_filter(cvipImage5, adapt_med_fil_val);
			cvipImage5 = (Image*)threshold_segment(cvipImage5, thresholdValue[k], CVIP_YES);

			outImgName5 = (char*)malloc((strlen(outImgPath5) + strlen(onlyFileNamesArr[idx[i]]) + strlen(filtersTag5) + strlen(buffer) + strlen("-Image-")) * sizeof(char) + 3);
			strcpy(outImgName5, outImgPath5);
			strcat(outImgName5, filtersTag5);
			strcat(outImgName5, buffer);
			strcat(outImgName5, "-Image-");
			strcat(outImgName5, onlyFileNamesArr[idx[i]]);

			write_Image(cvipImage5, outImgName5, 1, 0, 33, 1);


			// Processing Image with Median Filter = 7


			cvipImage7 = (Image*)median_filter(cvipImage7, med_fil_val7);
			cvipImage7 = (Image*)adapt_median_filter(cvipImage7, adapt_med_fil_val);
			cvipImage7 = (Image*)adapt_median_filter(cvipImage7, adapt_med_fil_val);
			cvipImage7 = (Image*)adapt_median_filter(cvipImage7, adapt_med_fil_val);
			cvipImage7 = (Image*)adapt_median_filter(cvipImage7, adapt_med_fil_val);
			cvipImage7 = (Image*)threshold_segment(cvipImage7, thresholdValue[k], CVIP_YES);

			outImgName7 = (char*)malloc((strlen(outImgPath7) + strlen(onlyFileNamesArr[idx[i]]) + strlen(filtersTag7) + strlen(buffer) + strlen("-Image-") ) * sizeof(char) + 3);
			strcpy(outImgName7, outImgPath7);
			strcat(outImgName7, filtersTag7);
			strcat(outImgName7, buffer);
			strcat(outImgName7, "-Image-");
			strcat(outImgName7, onlyFileNamesArr[idx[i]]);

			//Writing image to directory
			write_Image(cvipImage7, outImgName7, 1, 0, 33, 1);


			//delete_Image(cvipImage);
			free(outImgName);
			free(outImgName5);
			free(outImgName7);

			delete_Image(cvipImage);
			delete_Image(cvipImage5);
			delete_Image(cvipImage7);
		}

		//delete_Image(cvipImage);
		//delete_Image(cvipImage5);
		//delete_Image(cvipImage7);
		
	
	}

	for (int i = 0; i < 12; i++) {
		free(fileNames[i]);
		free(onlyFileNamesArr[i]);
	}


	/********************************************************
	*	Processing images with MEDIUM FILTER values 5 and 7
	********************************************************/
/*
	struct dirent* readerPtr;
	struct stat filestat5;
	DIR* folderNext5;
	folderNext5 = opendir(outImgPath);

	char* inImgName5;
	char* outImgName5;
	char* outImgName7;


	if (folderNext5 == NULL)
	{
		puts("Unable to read directory");
		return (1);
	}

	while ((readerPtr = readdir(folderNext5))) {
		stat(readerPtr->d_name, &filestat5);

		if (S_ISDIR(filestat5.st_mode)) {

		}
		else {
			inImgName5 = (char*)malloc((strlen(outImgPath) + strlen(readerPtr->d_name)) * sizeof(char) + 2);
			strcpy(inImgName5, outImgPath);
			strcat(inImgName5, readerPtr->d_name);
			
			cvipImage = read_Image(inImgName5, 1);
			cvipImage7 = read_Image(inImgName5, 1);

			if (cvipImage == NULL || cvipImage7 == NULL)
			{
				error_CVIP("main", "could not read input image");
			}

			// Processing Image with medium filter = 5
			cvipImage = (Image*)median_filter(cvipImage, med_fil_val5);
			cvipImage = (Image*)adapt_median_filter(cvipImage, adapt_med_fil_val);
			cvipImage = (Image*)adapt_median_filter(cvipImage, adapt_med_fil_val);
			cvipImage = (Image*)adapt_median_filter(cvipImage, adapt_med_fil_val);
			cvipImage = (Image*)adapt_median_filter(cvipImage, adapt_med_fil_val);
			cvipImage = (Image*)threshold_segment(cvipImage, threshold_segm_val, CVIP_YES);

			outImgName5 = (char*)malloc( ( strlen(outImgPath5) + strlen(readerPtr->d_name) + strlen(filtersTag5) ) * sizeof(char) + 3);
			strcpy(outImgName5, outImgPath5);
			strcpy(outImgName5, outImgPath5);
			strcat(outImgName5, filtersTag5);
			strcat(outImgName5, readerPtr->d_name);

			write_Image(cvipImage, outImgName5, 1, 0, 33, 1);



			// Processing images with medium filter = 7
			cvipImage7 = (Image*)median_filter(cvipImage7, med_fil_val7);
			cvipImage7 = (Image*)adapt_median_filter(cvipImage7, adapt_med_fil_val);
			cvipImage7 = (Image*)adapt_median_filter(cvipImage7, adapt_med_fil_val);
			cvipImage7 = (Image*)adapt_median_filter(cvipImage7, adapt_med_fil_val);
			cvipImage7 = (Image*)adapt_median_filter(cvipImage7, adapt_med_fil_val);
			cvipImage7 = (Image*)threshold_segment(cvipImage7, threshold_segm_val, CVIP_YES);

			outImgName7 = (char*)malloc((strlen(outImgPath7) + strlen(readerPtr->d_name) + strlen(filtersTag7)) * sizeof(char) + 3);
			strcpy(outImgName7, outImgPath7);
			strcpy(outImgName7, outImgPath7);
			strcat(outImgName7, filtersTag7);
			strcat(outImgName7, readerPtr->d_name);

			//Writing image to directory
			write_Image(cvipImage7, outImgName7, 1, 0, 33, 1);

			delete_Image(cvipImage7);
			free(outImgName5);
			free(outImgName7);

		}
		
	}

	closedir(folderNext5);

*/

	/**********************************************
	*	Processing images with MEDIUM FILTER = 7
	***********************************************/
/*
	struct dirent* readerPtr7;
	struct stat filestat7;
	DIR* folderNext7;
	folderNext7 = opendir(outImgPath);

	
	char* inImgName7;
	char* outImgName7;

	if (folderNext7 == NULL)
	{
		put("Unable to read directory");
		return (1);
	}

	while ((readerPtr7 = readdir(folderNext7)))
	{
		stat(readerPtr7->d_name, &filestat7);

		if (S_ISDIR(filestat7.st_mode)) {

		}
		else
		{
			inImgName7 = (char*)malloc((strlen(outImgPath) + readerPtr7->d_namlen) * sizeof(char) + 2);
			strcpy(inImgName7, outImgPath);
			strcat(inImgName7, readerPtr7->d_name);
			
			cvipImage = read_Image(inImgName7, 1);

			if (cvipImage == NULL)
			{
				error_CVIP("main", "Could not read input image for Filter 7");
			}

			// Processing Image with medium filter 7
			cvipImage = (Image*)median_filter(cvipImage, med_fil_val7);
			cvipImage = (Image*)adapt_median_filter(cvipImage, adapt_med_fil_val);
			cvipImage = (Image*)adapt_median_filter(cvipImage, adapt_med_fil_val);
			cvipImage = (Image*)adapt_median_filter(cvipImage, adapt_med_fil_val);
			cvipImage = (Image*)adapt_median_filter(cvipImage, adapt_med_fil_val);
			cvipImage = (Image*)threshold_segment(cvipImage, threshold_segm_val, CVIP_YES);

			outImgName7 = (char*)malloc((strlen(outImgPath7) + strlen(readerPtr7->d_name) + strlen(filtersTag7)) * sizeof(char) + 3);
			strcpy(outImgName7, outImgPath7);
			strcat(outImgName7, filtersTag7);
			strcat(outImgName7, readerPtr7->d_name);

			// saving processed image to directory 
			write_Image(cvipImage, outImgName7, 1, 0, 33, 1);

			delete_Image(cvipImage);
			free(outImgName7);

		}

	}

	closedir(folderNext7);
*/

}