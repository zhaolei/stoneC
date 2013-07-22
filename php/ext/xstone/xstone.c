/*
  +----------------------------------------------------------------------+
  | PHP Version 5                                                        |
  +----------------------------------------------------------------------+
  | Copyright (c) 1997-2012 The PHP Group                                |
  +----------------------------------------------------------------------+
  | This source file is subject to version 3.01 of the PHP license,      |
  | that is bundled with this package in the file LICENSE, and is        |
  | available through the world-wide-web at the following url:           |
  | http://www.php.net/license/3_01.txt                                  |
  | If you did not receive a copy of the PHP license and are unable to   |
  | obtain it through the world-wide-web, please send a note to          |
  | z@php.net so we can mail you a copy immediately.               |
  +----------------------------------------------------------------------+
  | Author:                                                              |
  +----------------------------------------------------------------------+
*/

/* $Id: header 321634 2012-01-01 13:15:04Z felipe $ */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "php.h"
#include "php_ini.h"
#include "ext/standard/info.h"
#include "php_xstone.h"
#include "php_xstone_file.h"

#include<string.h>	//strtok
#include<stdlib.h>	//realloc
#include<sys/socket.h>	//socket
#include<netinet/in.h> //sockaddr_in
#include<arpa/inet.h>	//getsockname
#include<netdb.h>	//hostent
#include<unistd.h>	//close

/* If you declare any globals in php_xstone.h uncomment this:
ZEND_DECLARE_MODULE_GLOBALS(xstone)
*/


/* True global resources - no need for thread safety here */
static int le_xstone;

/* {{{ xstone_functions[]
 *
 * Every user visible function must have an entry in xstone_functions[].
 */
const zend_function_entry xstone_functions[] = {
	PHP_FE(confirm_xstone_compiled,	NULL)		/* For testing, remove later. */
	PHP_FE(xstone_fun,	NULL)		/*  */
	PHP_FE(xstone_printf,	NULL)		/*  */
	PHP_FE(xstone_debug,	NULL)		/*  */
	PHP_FE(xstone_time,	NULL)		/*  */
	PHP_FE(xstone_whois,	NULL)		/*  */
	PHP_FE_END	/* Must be the last line in xstone_functions[] */
};
/* }}} */

/* {{{ xstone_module_entry
 */
zend_module_entry xstone_module_entry = {
#if ZEND_MODULE_API_NO >= 20010901
	STANDARD_MODULE_HEADER,
#endif
	"xstone",
	xstone_functions,
	PHP_MINIT(xstone),
	PHP_MSHUTDOWN(xstone),
	PHP_RINIT(xstone),		/* Replace with NULL if there's nothing to do at request start */
	PHP_RSHUTDOWN(xstone),	/* Replace with NULL if there's nothing to do at request end */
	PHP_MINFO(xstone),
#if ZEND_MODULE_API_NO >= 20010901
	"0.1", /* Replace with version number for your extension */
#endif
	STANDARD_MODULE_PROPERTIES
};
/* }}} */

#ifdef COMPILE_DL_XSTONE
ZEND_GET_MODULE(xstone)
#endif

/* {{{ PHP_INI
 */
/* Remove comments and fill if you need to have entries in php.ini
PHP_INI_BEGIN()
    STD_PHP_INI_ENTRY("xstone.global_value",      "42", PHP_INI_ALL, OnUpdateLong, global_value, zend_xstone_globals, xstone_globals)
    STD_PHP_INI_ENTRY("xstone.global_string", "foobar", PHP_INI_ALL, OnUpdateString, global_string, zend_xstone_globals, xstone_globals)
PHP_INI_END()
*/
/* }}} */

/* {{{ php_xstone_init_globals
 */
/* Uncomment this function if you have INI entries
static void php_xstone_init_globals(zend_xstone_globals *xstone_globals)
{
	xstone_globals->global_value = 0;
	xstone_globals->global_string = NULL;
}
*/
/* }}} */

/* {{{ PHP_MINIT_FUNCTION
 */
PHP_MINIT_FUNCTION(xstone)
{
	/* If you have INI entries, uncomment these lines 
	REGISTER_INI_ENTRIES();
	*/
	//zend_printf("minit \r\n");
	return SUCCESS;
}
/* }}} */

/* {{{ PHP_MSHUTDOWN_FUNCTION
 */
PHP_MSHUTDOWN_FUNCTION(xstone)
{
	/* uncomment this line if you have INI entries
	UNREGISTER_INI_ENTRIES();
	*/
	//zend_printf("mshutdown \r\n");
	return SUCCESS;
}
/* }}} */

/* Remove if there's nothing to do at request start */
/* {{{ PHP_RINIT_FUNCTION
 */
PHP_RINIT_FUNCTION(xstone)
{
	//debug var
	zval *var;
	zval *avar;
	MAKE_STD_ZVAL(var);
	MAKE_STD_ZVAL(avar);
	ZVAL_LONG(var, 1000);
	ZVAL_LONG(avar, 100);
	ZEND_SET_SYMBOL(EG(active_symbol_table), "xstonea", avar)
	ZEND_SET_GLOBAL_VAR("xstonew", var);
	//debug var
	//zend_printf("rinit \r\n");
	return SUCCESS;
}
/* }}} */

/* Remove if there's nothing to do at request end */
/* {{{ PHP_RSHUTDOWN_FUNCTION
 */
PHP_RSHUTDOWN_FUNCTION(xstone)
{
	//zend_printf("rshutdown \r\n");
	return SUCCESS;
}
/* }}} */

/* {{{ PHP_MINFO_FUNCTION
 */
PHP_MINFO_FUNCTION(xstone)
{
	php_info_print_table_start();
	php_info_print_table_header(2, "xstone support", "enabled");
	php_info_print_table_end();

	/* Remove comments if you have entries in php.ini
	DISPLAY_INI_ENTRIES();
	*/
}
/* }}} */


/* Remove the following function when you have succesfully modified config.m4
   so that your module can be compiled into PHP, it exists only for testing
   purposes. */

/* Every user-visible function in PHP should document itself in the source */
/* {{{ proto string confirm_xstone_compiled(string arg)
   Return a string to confirm that the module is compiled in */
PHP_FUNCTION(confirm_xstone_compiled)
{
	char *arg = NULL;
	int arg_len, len;
	char *strg;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s", &arg, &arg_len) == FAILURE) {
		return;
	}

	len = spprintf(&strg, 0, "Congratulations! You have successfully modified ext/%.78s/config.m4. Module %.78s is now compiled into PHP.", "xstone", arg);
	RETURN_STRINGL(strg, len, 0);
}
/* }}} */

/* Every user-visible function in PHP should document itself in the source */
/* {{{ proto string xstone_fun(string arg)
   Return a string to confirm that the module is compiled in */
PHP_FUNCTION(xstone_fun)
{
	char *arg = NULL;
	int arg_len, len;
	char *strg;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s", &arg, &arg_len) == FAILURE) {
		return;
	}

	len = spprintf(&strg, 0, "Xstone String is: %s", arg);
	RETURN_STRINGL(strg, len, 0);
}
/* }}} */


/* Every user-visible function in PHP should document itself in the source */
/* {{{ proto string xstone_fun(string arg)
   Return a string to confirm that the module is compiled in */
PHP_FUNCTION(xstone_printf)
{
	char *arg = NULL;
	int arg_len, len;
	char *strg;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s", &arg, &arg_len) == FAILURE) {
		return;
	}
    zend_printf(arg);

	//len = spprintf(&strg, 0, "Xstone String is: %s", arg);
	//RETURN_STRINGL(strg, len, 0);
}
/* }}} */

/* {{{ proto string xstone_debug(void)
    in */
PHP_FUNCTION(xstone_debug)
{
	char *arg = NULL;
	int len,nlen;
    int num;
	char *strg;

    num = ZEND_NUM_ARGS();
	len = spprintf(&strg, 0, "Xstone File is: %s \r\n", __FILE__);
    zend_printf(strg);
    efree(strg);
	len = spprintf(&strg, 0, "Xstone Argument Num is: %d \r\n", num);
    zend_printf(strg);
	RETURN_STRINGL(strg, len, 0);
}
/* }}} */

/* Every user-visible function in PHP should document itself in the source */
/* {{{ proto string xstone_fun(string arg)
   Return a string to confirm that the module is compiled in */
PHP_FUNCTION(xstone_whois)
{
	char *arg = NULL;
	int arg_len, len;
	char *strg;
	char ip[100] , *data = NULL;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s", &arg, &arg_len) == FAILURE) {
		return;
	}


	get_whois(arg , &data);

	//len = spprintf(&strg, 0, "Xstone String is: %s", arg);
	len = strlen(data);
	RETURN_STRINGL(data, len, 0);
}
/* }}} */

/**
	Get the whois content of an ip
	by selecting the correct server
*/
void get_whois(char *ip , char **data)
{
	char *wch = NULL, *pch , *response = NULL;

	if(whois_query("whois.iana.org" , ip , &response))
	{
		printf("Whois query failed");
	}

	pch = strtok(response , "\n");

	while(pch != NULL)
	{
		//Check if whois line
		wch = strstr(pch , "whois.");
		if(wch != NULL)
		{
			break;
		}

		//Next line please
		pch = strtok(NULL , "\n");
	}

	if(wch != NULL)
	{
		printf("\nWhois server is : %s" , wch);
		whois_query(wch , ip , data);
	}
	else
	{
		*data = malloc(100);
		strcpy(*data , "No whois data");
	}

	return;
}

/*
 * Perform a whois query to a server and record the response
 * */
int whois_query(char *server , char *query , char **response)
{
	char ip[32] , message[100] , buffer[1500];
	int sock , read_size , total_size = 0;
	struct sockaddr_in dest;

	sock = socket(AF_INET , SOCK_STREAM , IPPROTO_TCP);

    //Prepare connection structures :)
    memset( &dest , 0 , sizeof(dest) );
    dest.sin_family = AF_INET;

//	printf("\nResolving %s..." , server);
	if(hostname_to_ip(server , ip))
	{
		printf("Failed");
		return 1;
	}
	//printf("%s" , ip);
	dest.sin_addr.s_addr = inet_addr( ip );
	dest.sin_port = htons( 43 );

	//Now connect to remote server
	if(connect( sock , (const struct sockaddr*) &dest , sizeof(dest) ) < 0)
	{
		perror("connect failed");
	}

	//Now send some data or message
	printf("\nQuerying for ... %s ..." , query);
	sprintf(message , "%s\r\n" , query);
	if( send(sock , message , strlen(message) , 0) < 0)
	{
		perror("send failed");
	}

	//Now receive the response
	while( (read_size = recv(sock , buffer , sizeof(buffer) , 0) ) )
	{
		*response = realloc(*response , read_size + total_size);
		if(*response == NULL)
		{
			printf("realloc failed");
		}
		memcpy(*response + total_size , buffer , read_size);
		total_size += read_size;
	}
	//printf("Done");
	//fflush(stdout);

	*response = realloc(*response , total_size + 1);
	*(*response + total_size) = '\0';

	close(sock);
	return 0;
}

/*
 * @brief
 * Get the ip address of a given hostname
 *
 * */
int hostname_to_ip(char * hostname , char* ip)
{
	struct hostent *he;
	struct in_addr **addr_list;
	int i;

	if ( (he = gethostbyname( hostname ) ) == NULL)
	{
		// get the host info
		herror("gethostbyname");
		return 1;
	}

	addr_list = (struct in_addr **) he->h_addr_list;

	for(i = 0; addr_list[i] != NULL; i++)
	{
		//Return the first one;
		strcpy(ip , inet_ntoa(*addr_list[i]) );
		return 0;
	}

	return 0;
}

/* The previous line is meant for vim and emacs, so it can correctly fold and 
   unfold functions in source code. See the corresponding marks just before 
   function definition, where the functions purpose is also documented. Please 
   follow this convention for the convenience of others editing your code.
*/


/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: noet sw=4 ts=4 fdm=marker
 * vim<600: noet sw=4 ts=4
 */
