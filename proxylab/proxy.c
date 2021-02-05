#include <stdio.h>


/********
 *
 *
 * *****/
#include "csapp.h"

/* Recommended max cache and object sizes */
#define MAX_CACHE_SIZE 1049000
#define MAX_OBJECT_SIZE 102400

/* You won't lose style points for including this long line in your code */
static  char *user_agent_hdr = "User-Agent: Mozilla/5.0 (X11; Linux x86_64; rv:10.0.3) Gecko/20120305 Firefox/10.0.3\r\n";

//int main()
//{
//    printf("%s", user_agent_hdr);
//    return 0;
//}

void *thread(void *vargp);
void parse_hostname(char *uri, char *hostname, char* rem_path, char *http_port);
void from_server_to_client(int fd, int server_fd);
int from_client_to_server(int fd);
void read_requesthdrs(rio_t *rp);
int parse_uri(char *uri, char *filename, char *cgiargs);
void serve_static(int fd, char *filename, int filesize);
void get_filetype(char *filename, char *filetype);
void serve_dynamic(int fd, char *filename, char *cgiargs);
void clienterror(int fd, char *cause, char *errnum, 
		 char *shortmsg, char *longmsg);

int main(int argc, char **argv) 
{
    int listenfd;
    int *connfd_p;
    char hostname[MAXLINE], port[MAXLINE];
    socklen_t clientlen;
    struct sockaddr_storage clientaddr;
    pthread_t tid;

    /* Check command line args */
    if (argc != 2) {
        fprintf(stderr, "usage: %s <port>\n", argv[0]);
        exit(1);
    }

    listenfd = Open_listenfd(argv[1]);
    
    while (1) {
        clientlen = sizeof(clientaddr);
        connfd_p = Malloc(sizeof(int));
        *connfd_p = Accept(listenfd, (SA *)&clientaddr, &clientlen); 
        Getnameinfo((SA *) &clientaddr, clientlen, hostname, MAXLINE, \
                port, MAXLINE, 0);
        Pthread_create(&tid, NULL, thread, connfd_p);
    }
}
/* $end tinymain */

/*
 * from_client_to_server - handle one HTTP request/response transaction
 */
/* $begin from_client_to_server */
int from_client_to_server(int fd) 
{
    char buf[MAXLINE], method[MAXLINE], uri[MAXLINE], version[MAXLINE];
    rio_t rio; 

    /* Read request line */
    Rio_readinitb(&rio, fd);
    if (!Rio_readlineb(&rio, buf, MAXLINE))  
        return -2;
    //printf("%s", buf);

    /* request line */
    sscanf(buf, "%s %s %s", method, uri, version);       
    if (strcasecmp(method, "GET")) {                     
        clienterror(fd, method, "501", "Not Implemented",
                    "Tiny does not implement this method");
        return -1;
    }                                                    

    /* parse hostname and port*/
    char hostname[MAXLINE], rem_path[MAXLINE], http_port[10];
    parse_hostname(uri, hostname, rem_path, http_port);

    /* connect to target server*/
    int conn_to_server_fd;
    char *port = "80";// 80 is the default HTTP port. 
    if (http_port != NULL) port = http_port; 
    printf("method: %s\nhost: %s\npath: %s\nport: %s\nversion: %s\n", \
            method, hostname, rem_path, port, version);
    conn_to_server_fd = Open_clientfd(hostname, port); 
    

    /* send rq line*/
    char *http_version = "HTTP/1.0\r\n";
    char rq_line[MAXLINE];
    strcpy(rq_line, method);
    strcat(rq_line, " ");
    strcat(rq_line, rem_path);
    strcat(rq_line, " ");
    strcat(rq_line, http_version);
    Rio_writen(conn_to_server_fd, rq_line, strlen(rq_line)); // sending request line

    /* send rq header*/
    char rq_header[MAXLINE];
    sprintf(rq_header, "Host: %s\r\n", hostname);
    Rio_writen(conn_to_server_fd, rq_header, strlen(rq_header)); // sending request header
    char rq_hdr_buf[MAXLINE];
    Rio_readlineb(&rio, rq_hdr_buf, MAXLINE);
    while (strcmp(rq_hdr_buf, "\r\n")) {
        Rio_writen(conn_to_server_fd, rq_hdr_buf, strlen(rq_hdr_buf));
        Rio_readlineb(&rio, rq_hdr_buf, MAXLINE);
    }
    Rio_writen(conn_to_server_fd, "\r\n", strlen("\r\n"));

    
    Rio_writen(conn_to_server_fd, user_agent_hdr, strlen(user_agent_hdr));
    char *connection_hdr = "Connection: close\r\n";
    Rio_writen(conn_to_server_fd, connection_hdr, strlen(connection_hdr));
    char *proxy_conn_hdr = "Proxy-Connection: close\r\n";
    Rio_writen(conn_to_server_fd, proxy_conn_hdr, strlen(proxy_conn_hdr));
    
    return conn_to_server_fd;

}
/* $end from_client_to_server */

/*
 * parse_uri - parse URI into filename and CGI args
 *             return 0 if dynamic content, 1 if static
 */
/* $begin parse_uri */
int parse_uri(char *uri, char *filename, char *cgiargs) 
{
    char *ptr;

    if (!strstr(uri, "cgi-bin")) {  /* Static content */ //line:netp:parseuri:isstatic
	strcpy(cgiargs, "");                             //line:netp:parseuri:clearcgi
	strcpy(filename, ".");                           //line:netp:parseuri:beginconvert1
	strcat(filename, uri);                           //line:netp:parseuri:endconvert1
	if (uri[strlen(uri)-1] == '/')                   //line:netp:parseuri:slashcheck
	    strcat(filename, "home.html");               //line:netp:parseuri:appenddefault
	return 1;
    }
    else {  /* Dynamic content */                        //line:netp:parseuri:isdynamic
	ptr = index(uri, '?');                           //line:netp:parseuri:beginextract
	if (ptr) {
	    strcpy(cgiargs, ptr+1);
	    *ptr = '\0';
	}
	else 
	    strcpy(cgiargs, "");                         //line:netp:parseuri:endextract
	strcpy(filename, ".");                           //line:netp:parseuri:beginconvert2
	strcat(filename, uri);                           //line:netp:parseuri:endconvert2
	return 0;
    }
}
/* $end parse_uri */

/* $begin clienterror */
void clienterror(int fd, char *cause, char *errnum, 
		 char *shortmsg, char *longmsg) 
{
    char buf[MAXLINE];

    /* Print the HTTP response headers */
    sprintf(buf, "HTTP/1.0 %s %s\r\n", errnum, shortmsg);
    Rio_writen(fd, buf, strlen(buf));
    sprintf(buf, "Content-type: text/html\r\n\r\n");
    Rio_writen(fd, buf, strlen(buf));

    /* Print the HTTP response body */
    sprintf(buf, "<html><title>Tiny Error</title>");
    Rio_writen(fd, buf, strlen(buf));
    sprintf(buf, "<body bgcolor=""ffffff"">\r\n");
    Rio_writen(fd, buf, strlen(buf));
    sprintf(buf, "%s: %s\r\n", errnum, shortmsg);
    Rio_writen(fd, buf, strlen(buf));
    sprintf(buf, "<p>%s: %s\r\n", longmsg, cause);
    Rio_writen(fd, buf, strlen(buf));
    sprintf(buf, "<hr><em>The Tiny Web server</em>\r\n");
    Rio_writen(fd, buf, strlen(buf));
}

void from_server_to_client(int client_fd, int server_fd) {
    char buf_from_server_to_client[MAXLINE];
    memset(buf_from_server_to_client, 0, sizeof(buf_from_server_to_client));
    rio_t rio;
    Rio_readinitb(&rio, server_fd);
    while (strcmp(buf_from_server_to_client, "\r\n")) {
        Rio_readlineb(&rio, buf_from_server_to_client, MAXLINE);
        Rio_writen(client_fd, buf_from_server_to_client, \
                    strlen(buf_from_server_to_client));
    }
    Rio_writen(client_fd, "\r\n", strlen("\r\n"));
    return ;
    
}
void parse_hostname(char *uri, char *hostname, char *rem_path, char *http_port) {
    int cnt = 0;
    size_t uri_len = strlen(uri);
    char *st_pos = NULL, *end_pos=NULL;
    char *port_ptr = NULL;
    
    for (size_t i = 0; i < uri_len; ++i) {
        if (uri[i] == ':' && cnt == 2) {
            port_ptr = &uri[i+1];
            continue;
        }
        if (uri[i] == '/') {
            if (cnt < 1) {
            } else if (cnt == 1) {
                st_pos = uri+i+1;
            } else if (cnt == 2) {
                end_pos = uri+i;
                if (!port_ptr) {
                    strncpy(hostname, st_pos, end_pos - st_pos - 11);
                    http_port = NULL;
                } else {
                    strncpy(hostname, st_pos, port_ptr - st_pos - 1); // assign hostname
                    strncpy(http_port, port_ptr, end_pos - (port_ptr)); // assign port
                }
            }
            cnt++;
        }
    }
    
    strncpy(rem_path, end_pos, (uri+uri_len-1) - end_pos + 1);
    return ;
}

void *thread(void *vargp) {
    int connfd = *((int *)vargp);
    int server_fd = 0;
    Pthread_detach(pthread_self());
    Free(vargp);
    /* do */
    if ((server_fd = from_client_to_server(connfd)) > 0) {
        printf("proxy successful\n");
        from_server_to_client(connfd, server_fd);
    }
    if (server_fd > 0) 
        Close(server_fd);
    Close(connfd);
    return NULL;
}
