#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include "mysql.h"
 
 
 
int main(void)
{
    MYSQL *mysql;
    MYSQL *mysqlr;
    MYSQL_RES *results;
    MYSQL_ROW record;
    
    mysql = mysql_init(NULL);
 
   unsigned int num = 0;
   int i = 0;
   MYSQL_FIELD *field;
   char sql[100] = "select * from fixtable limit 10";
   mysqlr = mysql_real_connect(mysql, "127.0.0.1","root","1", "test", 3306,NULL,0);
   if(mysqlr == NULL) {
        printf("error\r\n");
        printf("mysql_real_connect failed: %s\r\n", mysql_error(mysql));
        exit(1);
   }
 
   while(1) {
    mysql_query(mysql, sql);
 
    results = mysql_store_result(mysql);
 
    num = mysql_num_fields(results);
    for(i=0;i<num;i++) {
        field = mysql_fetch_field_direct(results, i);
        printf("%10s|", field->name);
    }
    printf("\r\n");
    while((record = mysql_fetch_row(results))) {
        for(i=0;i<num;i++) {
            printf("%10s|", record[i]);
        }
        printf("\r\n---------------------------------------------------------------------\r\n");
    }
    printf("SQL: %s \r\n", sql);
    
    printf("\r\nsql>");
    fgets(sql, 100, stdin);
   }
   mysql_free_result(results);
   mysql_close(mysql);
   mysql_server_end();
 
   return 0;
}
