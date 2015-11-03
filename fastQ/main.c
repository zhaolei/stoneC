/*
* {next = {tqe_next = 0x0, tqe_prev = 0x804b624}, evcon = 0x804b598, flags = 1, input_headers = 0x804b578, 
  output_headers = 0x804b8a8, remote_host = 0x804b958 "127.0.0.1", remote_port = 39867, 
    host_cache = 0x804bbf0 "localhost", kind = EVHTTP_REQUEST, type = EVHTTP_REQ_GET, headers_size = 402, 
      body_size = 0, uri = 0x804b428 "/", uri_elems = 0x804b438, major = 1 '\001', minor = 1 '\001', 
        response_code = 0, response_code_line = 0x0, input_buffer = 0x804b8b8, ntoread = 0, chunked = 0, userdone = 0, 
          output_buffer = 0x804b908, cb = 0xb7fa57e0, cb_arg = 0x804b3b8, chunk_cb = 0}

*/
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <err.h>
#include <event.h>
#include <evhttp.h>

void now_handler(struct evhttp_request *req, void *arg)
{
        struct evbuffer *buf;
        char ebuf[100];
        struct evkeyvalq params;
        char *decoded_uri;
        const char *uri;

        buf = evbuffer_new();

        if (buf == NULL)
        err(1, "failed to create response buffer");

        sprintf(ebuf, "log: %s %s %s \n", req->remote_host, req->host_cache, req->uri);
        //evbuffer_add_printf(buf, "<h1>Hello World!</h1>");
        evbuffer_add_printf(buf, ebuf);
        evhttp_send_reply(req, HTTP_OK, "OK", buf);
        printf("log: %s %s %s \n", req->remote_host, req->host_cache, req->uri);

        //GET
        uri = evhttp_request_uri(req);

        decoded_uri = evhttp_decode_uri(uri);
        evhttp_parse_query(decoded_uri, &params);
        printf("GET: s=%s\n", evhttp_find_header(&params, "s"));
        /*
        sprintf(tmp, "q=%s\n", evhttp_find_header(&params, "q"));
        strcat(output, tmp);
        sprintf(tmp, "s=%s\n", evhttp_find_header(&params, "s"));
        strcat(output, tmp);
        */
        free(decoded_uri);

        //char *post_data = (char *) EVBUFFER_DATA(req->input_buffer);
        //printf("POST: post_data=%s ok\n", post_data);
        //sprintf(tmp, "post_data=%s\n", post_data);

        struct evbuffer *bufx;
        size_t len;
        char *data=malloc(1000);
        bufx=evhttp_request_get_input_buffer(req);
        len=evbuffer_copyout(bufx, data, 1000);
        printf("POST: len=%d req=%s\n", (int)len, data);
}


int mainx(int argc, char **argv)
{
    struct evhttp *httpd;

    event_init();

    httpd = evhttp_start("0.0.0.0", 6006);

    if ( httpd == NULL )
    {

        fprintf(stderr, "Start server error: %m\n");
        exit(1);
    }

    /* Set a callback for requests to "/specific". */

    /* evhttp_set_cb(httpd, "/specific", another_handler, NULL); */

 

    /* Set a callback for all other requests. */

    evhttp_set_gencb(httpd, now_handler, NULL);
    event_dispatch();

    /* Not reached in this code as it is now. */

    evhttp_free(httpd);
    return 0;
}
