#include <stdio.h>
#include "sbuf.h"
#include "csapp.h"

#define SBUFSIZE 16
#define NTHREADS 4

sbuf_t sbuf;
static  char *user_agent_hdr = "User-Agent: Mozilla/5.0 (X11; Linux x86_64; rv:10.0.3) Gecko/20120305 Firefox/10.0.3\r\n";
static const char *conn_hdr = "Connection: close\r\n";
static const char *prox_hdr = "Proxy-Connection: close\r\n";
static const char *host_hdr_format = "Host: %s\r\n";
static const char *requestlint_hdr_format = "GET %s HTTP/1.0\r\n";
static const char *endof_hdr = "\r\n";

static const char *connection_key = "Connection";
static const char *user_agent_key= "User-Agent";
static const char *proxy_connection_key = "Proxy-Connection";
static const char *host_key = "Host";

void from_client_to_server(int fd);
void clienterror(int fd, char *cause, char *errnum, 
		 char *shortmsg, char *longmsg);
void* thread(void *vargp);
void parse_hostname(char *uri, char *hostname, char path[], int *http_port);
void build_http_header(char *http_header,char *hostname,char *path, rio_t *client_rio);


void parse_hostname(char *uri, char *hostname, char path[], int *http_port) {
    char *pos = strstr(uri, "//");
    path[0] = '/';
    path[1] = '\0';
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
        from_client_to_server(connfd);
        Close(connfd);
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
void from_client_to_server(int fd) 
{
    char buf[MAXLINE], method[MAXLINE], uri[MAXLINE], version[MAXLINE];
    rio_t rio, server_rio;

    /* Read request line */
    Rio_readinitb(&rio, fd);
    if (!Rio_readlineb(&rio, buf, MAXLINE))  
        return ;
    //printf("%s", buf);

    /* request line */
    sscanf(buf, "%s %s %s", method, uri, version);       
    if (strcasecmp(method, "GET")) {                     
        clienterror(fd, method, "501", "Not Implemented",
                    "Tiny does not implement this method");
        return ;
    }                                                    

    /* parse hostname and port*/
    char hostname[MAXLINE], rem_path[MAXLINE];
    int http_port;
    char server_header[MAXLINE];
    parse_hostname(uri, hostname, rem_path, &http_port);
    build_http_header(server_header, hostname, rem_path, &rio);

    /* connect to target server*/
    int conn_to_server_fd;
    printf("method: %s\nhost: %s\npath: %s\nport: %d\nversion: %s\n", \
            method, hostname, rem_path, http_port, version);
    char port[10];
    sprintf(port, "%d", http_port);
    conn_to_server_fd = Open_clientfd(hostname, port); 
    Rio_readinitb(&server_rio, conn_to_server_fd);
    Rio_writen(conn_to_server_fd, server_header, strlen(server_header));

    size_t n;
    memset(buf, 0, sizeof(buf));
    while((n=Rio_readlineb(&server_rio,buf,MAXLINE))!=0)
    {
        printf("proxy received %ld bytes,then send\n",n);
        Rio_writen(fd,buf,n);
    }
    Close(conn_to_server_fd);
    return ;
}
    
void build_http_header(char *http_header,char *hostname,char *path, rio_t *client_rio)
{
    char buf[MAXLINE],request_hdr[MAXLINE],other_hdr[MAXLINE],host_hdr[MAXLINE];
    /*request line*/
    sprintf(request_hdr,requestlint_hdr_format,path);
    /*get other request header for client rio and change it */
    while(Rio_readlineb(client_rio,buf,MAXLINE)>0)
    {
        if(strcmp(buf,endof_hdr)==0) break;/*EOF*/

        if(!strncasecmp(buf,host_key,strlen(host_key)))/*Host:*/
        {
            strcpy(host_hdr,buf);
            continue;
        }

        if(!strncasecmp(buf,connection_key,strlen(connection_key))
                &&!strncasecmp(buf,proxy_connection_key,strlen(proxy_connection_key))
                &&!strncasecmp(buf,user_agent_key,strlen(user_agent_key)))
        {
            strcat(other_hdr,buf);
        }
    }
    if(strlen(host_hdr)==0)
    {
        sprintf(host_hdr,host_hdr_format,hostname);
    }
    sprintf(http_header,"%s%s%s%s%s%s%s",
            request_hdr,
            host_hdr,
            conn_hdr,
            prox_hdr,
            user_agent_hdr,
            other_hdr,
            endof_hdr);

    return ;
}
