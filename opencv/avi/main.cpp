#include"cv.h"
#include"highgui.h"
#include<ctype.h>
#include<stdio.h>
int main(int argc,char** argv)
{
	IplImage* frame;
	CvCapture* capture=0;

	//capture=cvCaptureFromCAM(0);
	capture=cvCreateFileCapture("/mnt/shared/t1.avi");


	if(!capture)
	{
		fprintf(stderr,"Could not initialize capturing...\n");
		return -1;
	}
	frame = cvCreateImage(cvSize(320, 240), IPL_DEPTH_16U, 3);
	cvNamedWindow("Window 0", 0);

	//循环捕捉,知道用户跳出循环
	while(capture)
	{
		frame=cvQueryFrame(capture);
		if(!frame) {
			break;
		}
		cvShowImage("Window 0",frame);

		if(cvWaitKey(1)==27)
			break;
	}
	cvReleaseCapture(&capture);
	cvDestroyWindow("Window 0");

	return 0;
}
