
#define BUF_SIZE 8192
struct indexq {
    int start;
    int len;
};

struct dataq {
    char *data;
    int len;
    int id;
};

extern int dat_fd;
extern int index_fd;

//#define INDEX_SIZE (sizeof(struct indexq) * 2) 
#define INDEX_SIZE sizeof(struct indexq) 
//#define INDEX_SIZE 16 

int writeData(int fd, void *data, int len);

int readData(int fd, void *data, int pos, int len); 

int writeIndex(int fd, void *data);

int readIndex(int fd, void *data);

int readIndexById(int fd, void *data, int id);

int writeEntity(int dfd, int ifd, void *data, int len);
char *readEntity(int dfd, int ifd, int id);

int initDat(int *dfd, int *ifd);

