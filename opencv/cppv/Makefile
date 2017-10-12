CC = g++
#CFLAGS = -I/usr/include/opencv/ \
#	 -L /usr/lib 
CFLAGS = -I/usr/include/opencv/ 
LDFLAGS = `pkg-config --libs-only-l opencv` -lm -Wall -g  -W 
ALL:
	$(CC) -o cppv main.cpp $(CFLAGS) $(LDFLAGS)
cvt:cvt.cpp
	$(CC) -o cvt cvt.cpp $(CFLAGS) $(LDFLAGS)
kk:kmeans.cpp
	$(CC) -o kk kmeans.cpp $(CFLAGS) $(LDFLAGS)
t1:t1.cpp
	$(CC) -o t1 t1.cpp $(CFLAGS) $(LDFLAGS)
t2:t2.cpp
	$(CC) -o t2 t2.cpp $(CFLAGS) $(LDFLAGS)


#    -I/usr/include/opencv  -lopencv_core -lopencv_imgproc -lopencv_highgui -lopencv_ml -lopencv_video -lopencv_features2d -lopencv_calib3d -lopencv_objdetect -lopencv_contrib -lopencv_legacy -lopencv_flann 
#pkg-config opencv --libs --cflags opencv
