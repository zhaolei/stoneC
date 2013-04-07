#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <cv.h>
#include <highgui.h>


int main( int argc, char** argv )
{
	IplImage* dst; 
    char file[100] = "a.png";
    
    dst = cvCreateImage( cvSize(200,200), 8, 3 );
    if(argc>1) {
        //free(dst);
	    strcpy(file, argv[1]);
	    printf("%s \n", file);
	    dst=cvLoadImage(file);
    }
    cvLine( dst, cvPoint(200,200), cvPoint(0,0), CV_RGB(200,0,0), 4);
    cvLine( dst, cvPoint(200,0), cvPoint(0,200), CV_RGB(0,200,200), 4);

    cvNamedWindow( "Source", 1 );
    cvShowImage( "Source", dst);
    //free(dst);

    cvWaitKey(0);
}
