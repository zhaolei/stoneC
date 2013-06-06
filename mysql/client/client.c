#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <readline/readline.h>
#include <readline/history.h>
#include "mysql.h"
 
MYSQL *mysql;
MYSQL *mysqlr;
MYSQL_RES *results;
MYSQL_ROW record;
 
static char *server_options[] = { "mysql_test", "--defaults-file=/etc/mysql/my.cnf" };
int num_elements = sizeof(server_options)/ sizeof(char *);
 
static char *server_groups[] = { "libmysqld_server", "libmysqld_client" };
 
int main(void)
{
//   int msi = mysql_server_init(num_elements, server_options, server_groups);
//   printf("mysql_server_init: %d \r\n", msi);
   mysql = mysql_init(NULL);
//   mysql_options(mysql, MYSQL_READ_DEFAULT_GROUP, "libmysqld_client");
//   mysql_options(mysql, MYSQL_OPT_USE_EMBEDDED_CONNECTION, NULL);
 
   unsigned int num = 0;
   int i = 0;
   char *sql;
   MYSQL_FIELD *field;
   mysqlr = mysql_real_connect(mysql, "127.0.0.1","root","1", "test", 3306,NULL,0);
   if(mysqlr == NULL) {
        printf("error\r\n");
        printf("mysql_real_connect failed: %s\r\n", mysql_error(mysql));
        exit(1);
   }
 
   while(sql = readline(">>")) {
    //mysql_query(mysql, "select * from fixtable limit 10");
        mysql_query(mysql, sql);
        free(sql);
        results = mysql_store_result(mysql);
 
        num = mysql_num_fields(results);
        for(i=0;i<num;i++) {
            field = mysql_fetch_field_direct(results, i);
            printf("%s  |", field->name);
        }
        printf("\r\n");
        while((record = mysql_fetch_row(results))) {
            for(i=0;i<num;i++) {
                printf("%s |", record[i]);
            }
            printf("\r\n----------------------\r\n");
        }
        
        mysql_free_result(results);
   }
 
 
   mysql_close(mysql);
   mysql_server_end();
 
   return 0;
}
