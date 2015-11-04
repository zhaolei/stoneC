
#define BUF_SIZE 8192
struct indexq {
    int start;
    int len;
    //char x;
};

//#define INDEX_SIZE (sizeof(struct indexq) * 2) 
#define INDEX_SIZE sizeof(struct indexq) 
//#define INDEX_SIZE 16 

