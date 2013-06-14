CC = gcc
#CFLAGS = -I/usr/include/opencv/ \
#	 -L /usr/lib 
CFLAGS = -I/usr/include/opencv/ 
LDFLAGS = -lm -lopencv_core -lopencv_highgui -Wall -g  -W 
ALL:
	$(CC) -g -o cam  main.cpp $(CFLAGS) $(LDFLAGS)

#    -I/usr/include/opencv  -lopencv_core -lopencv_imgproc -lopencv_highgui -lopencv_ml -lopencv_video -lopencv_features2d -lopencv_calib3d -lopencv_objdetect -lopencv_contrib -lopencv_legacy -lopencv_flann 
#pkg-config opencv --libs --cflags opencv
