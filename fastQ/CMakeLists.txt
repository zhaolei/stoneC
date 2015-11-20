cmake_minimum_required(VERSION 2.8)
PROJECT(fastq)                                             #定义整个CMake的工程名
FILE(GLOB SOURCE_Q "log.c dat.c http.c fastq.c")                      #告诉CMake：工程的源文件在哪里？
ADD_EXECUTABLE(fastq log.c dat.c http.c fastq.c)  
FILE(GLOB SOURCE_P "log.c dat.c http.c conf.c fastp.c")                      #告诉CMake：工程的源文件在哪里？
ADD_EXECUTABLE(fastp log.c dat.c http.c conf.c fastp.c post.c)  
#TARGET_LINK_LIBRARIES("-levent")
#TARGET_LINK_LIBRARIES("-L/usr/local/lib")
set(CMAKE_C_FLAGS " -g -Wall ")
set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS} -g -ggdb -pg")
#set(CMAKE_LINKER "  -L/usr/local/lib -levent -Wall ")
link_directories("/usr/local/lib")
target_link_libraries(fastq event)
target_link_libraries(fastp event)
target_link_libraries(fastp curl)
target_link_libraries(fastp config)

