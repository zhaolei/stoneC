all:
	protoc -I . --cpp_out=. hello.proto
	g++ -c -o hello.o hello.pb.cc
	g++ -o tt hello.o main.cpp `pkg-config --libs protobuf` 
clean:
	rm *.o *.pb.h *.pb.cc tt
