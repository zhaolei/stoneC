#include <iostream>
#include <string>
#include "hello.pb.h"

using namespace std;

int main() {
	tutorial::Person tt;
	tutorial::Person tto;
	tt.set_name("Python");
	tt.set_email("fff@shiff.com");
	tt.set_age(18);

	std::string str1;

	tt.SerializeToString(&str1);

	tto.ParseFromString(str1);
	cout<<str1<<endl;
	cout<<tto.name()<<endl;
	cout<<tto.email()<<endl;
	cout<<tto.age()<<endl;
	return 0;
}
