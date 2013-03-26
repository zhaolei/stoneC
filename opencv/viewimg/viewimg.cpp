#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <cv.h>
#include <highgui.h>


int main(int argc, char *argv[])
{
	IplImage* img = 0; 
	int height,width,step, channels;
      	uchar *data;
        
        char file[100] = "a.png";
	
	if(argc>1){
		strcpy(file, argv[1]);
		printf("%s \n", file);
	     // printf("Usage: main <image-file-name>\n\7");
	     // exit(0);
	}
		      // load an image  
	img=cvLoadImage(file);
        if(!img){
                printf("Could not load image file: %s\n", file);
                exit(0);
        }

        // get the image data
        height    = img->height;
        width     = img->width;
        step      = img->widthStep;
        channels  = img->nChannels;
        data      = (uchar *)img->imageData;
        
        printf("Processing a %dx%d image with %d channels\n",height,width,channels); 

        // create a window
        cvNamedWindow("mainWin"); 
        cvMoveWindow("mainWin", 100, 100);
	cvResizeWindow("mainWin", 400, 400);

        // show the image

        cvShowImage("mainWin", img );
        
	// wait for a key
        cvWaitKey(0);

        // release the image
        cvReleaseImage(&img );
        return 0;
}
