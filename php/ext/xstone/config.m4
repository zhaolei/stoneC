dnl $Id$
dnl config.m4 for extension xstone

dnl Comments in this file start with the string 'dnl'.
dnl Remove where necessary. This file will not work
dnl without editing.

dnl If your extension references something external, use with:

PHP_ARG_WITH(xstone, for xstone support,
dnl Make sure that the comment is aligned:
[  --with-xstone             Include xstone support])

dnl Otherwise use enable:

PHP_ARG_ENABLE(xstone, whether to enable xstone support,
dnl Make sure that the comment is aligned:
[  --enable-xstone           Enable xstone support])
PHP_XSTONE=yes

dnl PHP_NEW_EXTENSION(xstone, xstone.c      \
dnl        xstone_file.c, $ext_shared)
if test "$PHP_XSTONE" != "no"; then
  dnl Write more examples of tests here...

  dnl # --with-xstone -> check with-path
  dnl SEARCH_PATH="/usr/local /usr"     # you might want to change this
  dnl SEARCH_FOR="/include/xstone.h"  # you most likely want to change this
  dnl if test -r $PHP_XSTONE/$SEARCH_FOR; then # path given as parameter
  dnl   XSTONE_DIR=$PHP_XSTONE
  dnl else # search default path list
  dnl   AC_MSG_CHECKING([for xstone files in default path])
  dnl   for i in $SEARCH_PATH ; do
  dnl     if test -r $i/$SEARCH_FOR; then
  dnl       XSTONE_DIR=$i
  dnl       AC_MSG_RESULT(found in $i)
  dnl     fi
  dnl   done
  dnl fi
  dnl
  dnl if test -z "$XSTONE_DIR"; then
  dnl   AC_MSG_RESULT([not found])
  dnl   AC_MSG_ERROR([Please reinstall the xstone distribution])
  dnl fi

  dnl # --with-xstone -> add include path
  dnl PHP_ADD_INCLUDE($XSTONE_DIR/include)

  dnl # --with-xstone -> check for lib and symbol presence
  dnl LIBNAME=xstone # you may want to change this
  dnl LIBSYMBOL=xstone # you most likely want to change this 

  dnl PHP_CHECK_LIBRARY($LIBNAME,$LIBSYMBOL,
  dnl [
  dnl PHP_ADD_LIBRARY_WITH_PATH($LIBNAME, $XSTONE_DIR/lib, XSTONE_SHARED_LIBADD)
  dnl     AC_DEFINE(HAVE_XSTONELIB,1,[ ])
  dnl ],[
  dnl   AC_MSG_ERROR([wrong xstone lib version or lib not found])
  dnl ],[
  dnl   -L$XSTONE_DIR/lib -lm
  dnl ])
  
  dnl PHP_SUBST(XSTONE_SHARED_LIBADD)
  dnl PHP_SUBST(XSTONE_SHARED_LIBADD)

  dnl EXT_SOURCE="xstone.c xstone_file.c"  
  dnl PHP_NEW_EXTENSION(xstone, xstone.c xstone_file.c, $ext_shared)
  PHP_NEW_EXTENSION(xstone,xstone.c	     xstone_file.c, $ext_shared)
  dnl ifdef([PHP_INSTALL_HEADERS], [
  dnl	PHP_INSTALL_HEADERS(ext/xstone, php_xstone.h)
  dnl ])
fi
