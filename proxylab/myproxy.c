#include <stdio.h>
#include "sbuf.h"
#include "csapp.h"

#define SBUFSIZE 16
#define NTHREADS 4

sbuf_t sbuf;
static  char *user_agent_hdr = "User-Agent: Mozilla/5.0 (X11; Linux x86_64; rv:10.0.3) Gecko/20120305 Firefox/10.0.3\r\n";

int from_client_to_server(int fd);
void from_server_to_client(int client_fd, int *server_fd);
void clienterror(int fd, char *cause, char *errnum, 
		 char *shortmsg, char *longmsg);
void* thread(void *vargp);
void parse_hostname(char *uri, char *hostname, char *path, int *http_port);


void parse_hostname(char *uri, char *hostname, char *path, int *http_port) {
    char *pos = strstr(uri, "//");
    *path = "/\0";
    pos = pos != NULL?pos+2:uri;
    *http_port = 80;
    
    
    char *pos2 = strstr(pos,":");
    
    if (pos2 != NULL) {
        *pos2 = '\0';
        sscanf(pos, "%s", hostname);
        sscanf(pos2+1, "%d%s", http_port, path);
    } else {
        if ((pos2 = strstr(pos, "/")) != NULL) {
            *pos2 = '\0';
            sscanf(pos, "%s", hostname);
            *pos2 = '/';
            sscanf(pos2, "%s", path);
        } else {
            sscanf(pos, "%s", hostname);
        }
        
    }
    return ;
}
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
void* thread(void *vargp) {
    Pthread_detach(pthread_self());
    while(1) {
        int connfd = sbuf_remove(&sbuf);
        int *serverfd = malloc(sizeof(int));
        *serverfd = from_client_to_server(connfd);
        from_server_to_client(connfd, serverfd);
        Close(connfd);
        Close(serverfd);
    }
}

int main(int argc, char** argv) {
    int i, listenfd, connfd;
    socklen_t clientlen;
    struct sockaddr_storage clientaddr;
    pthread_t tid;
    
    if (argc != 2) {
        fprintf(stderr, "error\n");
        exit(-1);
    }

    listenfd = Open_listenfd(argv[1]);

    sbuf_init(&sbuf, SBUFSIZE);
    for(i = 0; i< NTHREADS;i++) {
        Pthread_create(&tid, NULL, thread, NULL);
    }
    while (1) {
        clientlen = sizeof(struct sockaddr_storage);
        connfd = Accept(listenfd, (SA *)&clientaddr, &clientlen);
        sbuf_insert(&sbuf, connfd);
    }
}
void from_server_to_client(int client_fd, int *server_fd) {
    char buf_from_server_to_client[MAXLINE];
    memset(buf_from_server_to_client, 0, sizeof(buf_from_server_to_client));
    rio_t rio;
    Rio_readinitb(&rio, *server_fd);
    Rio_readlineb(&rio, buf_from_server_to_client, MAXLINE);
    while (strcmp(buf_from_server_to_client, "\r\n")) {
        Rio_readlineb(&rio, buf_from_server_to_client, MAXLINE);
        Rio_writen(client_fd, buf_from_server_to_client, \
                    strlen(buf_from_server_to_client));
    }
    Rio_writen(client_fd, "\r\n", strlen("\r\n"));
    return ;
    
}
int from_client_to_server(int fd) 
{
    char buf[MAXLINE], method[MAXLINE], uri[MAXLINE], version[MAXLINE];
    rio_t rio, server_rio; 

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
    char hostname[MAXLINE], rem_path[MAXLINE];
    int http_port;
    parse_hostname(uri, hostname, rem_path, &http_port);

    /* connect to target server*/
    int conn_to_server_fd;
    printf("method: %s\nhost: %s\npath: %s\nport: %d\nversion: %s\n", \
            method, hostname, rem_path, http_port, version);
    char port[10];
    sprintf(port, "%d", http_port);
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
    
    /* send keep-alive header*/
    Rio_writen(conn_to_server_fd, user_agent_hdr, strlen(user_agent_hdr));
    char *connection_hdr = "Connection: close\r\n";
    Rio_writen(conn_to_server_fd, connection_hdr, strlen(connection_hdr));
    char *proxy_conn_hdr = "Proxy-Connection: close\r\n";
    Rio_writen(conn_to_server_fd, proxy_conn_hdr, strlen(proxy_conn_hdr));

    //char rq_hdr_buf[MAXLINE];
    //Rio_readinitb(&server_rio, conn_to_server_fd);
    //Rio_readlineb(&server_rio, rq_hdr_buf, MAXLINE);
    //while (strcmp(rq_hdr_buf, "\r\n")) {
    //    Rio_writen(fd, rq_hdr_buf, strlen(rq_hdr_buf));
    //    Rio_readlineb(&rio, rq_hdr_buf, MAXLINE);
    //}
    //Rio_writen(fd, "\r\n", strlen("\r\n"));

    
    
    return conn_to_server_fd;

}
