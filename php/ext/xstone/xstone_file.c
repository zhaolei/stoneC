//#include "xstonephp.h"
#include <time.h>
#include "php.h"
#include "php_xstone_file.h"


/* {{{ proto string xstone_time(void)
    in */
PHP_FUNCTION(xstone_time)
{
	time_t t1,t2;

	t2 = time(&t1);

	RETURN_LONG(t2);
}
/* }}} */


