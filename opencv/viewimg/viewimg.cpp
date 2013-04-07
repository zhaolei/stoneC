#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <cv.h>
#include <highgui.h>


int main(int argc, char *argv[])
{
	IplImage* img = 0; 
	int height,width, channels;
    //uchar *data;
        
    char file[100] = "a.png";
	
	if(argc<2){
	      printf("Usage: viewimg <image-file-name>\n\7");
	      exit(0);
	}
	strcpy(file, argv[1]);
	printf("%s \n", file);
		      // load an image  
	img=cvLoadImage(file);
        if(!img){
                printf("Could not load image file: %s\n", file);
                exit(0);
        }

        // get the image data
        height    = img->height;
        width     = img->width;
        //step      = img->widthStep;
        channels  = img->nChannels;
        //data      = (uchar *)img->imageData;
        
        printf("Processing a %dx%d image with %d channels\n",height,width,channels); 

        // create a window
        cvNamedWindow(file); 
        cvMoveWindow(file, 100, 100);
	    cvResizeWindow(file, 400, 400);

        // show the image

        cvShowImage(file, img );
        
	// wait for a key
        cvWaitKey(0);

        // release the image
        cvReleaseImage(&img );
        return 0;
}
