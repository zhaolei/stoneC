#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
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
 
   mysqlr = mysql_real_connect(mysql, "127.0.0.1","root","111111", "stone", 3306,NULL,0);
   if(mysqlr == NULL) {
        printf("error\r\n");
        printf("mysql_real_connect failed: %s\r\n", mysql_error(mysql));
        exit(1);
   }
 
   mysql_query(mysql, "select * from fixtable limit 10");
 
   results = mysql_store_result(mysql);
 
   while((record = mysql_fetch_row(results))) {
      printf("%s - %s - %s \n", record[0], record[1], record[2]);
   }
 
   mysql_free_result(results);
   mysql_close(mysql);
   mysql_server_end();
 
   return 0;
}
