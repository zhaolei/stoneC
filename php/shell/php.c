#include <sapi/embed/php_embed.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <stdio.h>

int php_shell(int argc, char * argv[]){
    PHP_EMBED_START_BLOCK(argc,argv);
    char *ch;
    char *read = "This is my PHP shell v 0.1\r\n";

    fputs(read, stdout);
    while((ch = readline(">>"))) {
        zend_eval_string(ch, NULL,"shell" TSRMLS_CC);
        fputs("\r\n", stdout);
        add_history(ch);
        free(ch);
    }
    
    PHP_EMBED_END_BLOCK();
    return 0;
}
