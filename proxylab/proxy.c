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
static const char *user_agent_hdr = "User-Agent: Mozilla/5.0 (X11; Linux x86_64; rv:10.0.3) Gecko/20120305 Firefox/10.0.3\r\n";

//int main()
//{
//    printf("%s", user_agent_hdr);
//    return 0;
//}

void parse_hostname(char *uri, char *hostname, char* rem_path);
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
    int listenfd, connfd;
    int server_fd;
    char hostname[MAXLINE], port[MAXLINE];
    socklen_t clientlen;
    struct sockaddr_storage clientaddr;

    /* Check command line args */
    if (argc != 2) {
        fprintf(stderr, "usage: %s <port>\n", argv[0]);
        exit(1);
    }

    listenfd = Open_listenfd(argv[1]);
    
    while (1) {
        clientlen = sizeof(clientaddr);
        connfd = Accept(listenfd, (SA *)&clientaddr, &clientlen); //line:netp:tiny:accept
        Getnameinfo((SA *) &clientaddr, clientlen, hostname, MAXLINE, 
                        port, MAXLINE, 0);
        printf("Accepted connection from (%s, %s)\n", hostname, port);
        server_fd = from_client_to_server(connfd);
        from_server_to_client(connfd, server_fd);
        Close(connfd);             
        Close(server_fd);             
    }
}
/* $end tinymain */

/*
 * from_client_to_server - handle one HTTP request/response transaction
 */
/* $begin from_client_to_server */
int from_client_to_server(int fd) 
{
    int is_static;
    struct stat sbuf;
    char buf[MAXLINE], method[MAXLINE], uri[MAXLINE], version[MAXLINE];
    char filename[MAXLINE], cgiargs[MAXLINE];
    rio_t rio, rio_server;

    /* Read request line and headers */
    Rio_readinitb(&rio, fd);
    if (!Rio_readlineb(&rio, buf, MAXLINE))  //line:netp:from_client_to_server:readrequest
        return;
    printf("%s", buf);

    /* request line */
    sscanf(buf, "%s %s %s", method, uri, version);       //line:netp:from_client_to_server:parserequest
    if (strcasecmp(method, "GET")) {                     //line:netp:from_client_to_server:beginrequesterr
        clienterror(fd, method, "501", "Not Implemented",
                    "Tiny does not implement this method");
        return;
    }                                                    

    /* parse hostname*/
    char *host, target_server;
    Rio_readlineb(&rio, host, MAXLINE);
    //TODO parse the hostname from rq_hdrs `Host: www.xxx.com`.
    //there are some string manipulation.
    char *hostname, *rem_path;
    parse_hostname(uri, hostname, rem_path);

    /* connect to target server*/
    int conn_to_server_fd;
    // TODO determine port number after reading port number part in wirteup.
    conn_to_server_fd = Open_clientfd(hostname, 80); // 80 is the default HTTP port. 
    //Rio_readinitb(&rio_server, conn_to_server_fd);
    
    //TODO read from our client and sending it to target server
    // i need to learn from echo client code from textbook, which instruct me 
    // to send bytes to target server.
    // i also need to learn from read_requesthdrs(), which instruct me
    // read from our client

    /* send rq line*/
    char *http_version = "HTTP/1.0\r\n";
    char *rq_line;
    strcpy(rq_line, method);
    strcat(rq_line, " ");
    strcat(rq_line, rem_path);
    strcat(rq_line, " ");
    strcat(rq_line, http_version);
    Rio_writen(conn_to_server_fd, rq_line, strlen(rq_line));

    /* send rq header*/
    Rio_writen(conn_to_server_fd, host, strlen(host));
    char rq_hdr_buf[MAXLINE];
    Rio_readlineb(&rio, rq_hdr_buf, MAXLINE);
    while (strcmp(rq_hdr_buf, "\r\n")) {
        Rio_writen(conn_to_server_fd, rq_hdr_buf, strlen(rq_hdr_buf));
        Rio_readlineb(&rio, rq_hdr_buf, MAXLINE);
    }

    
    Rio_writen(conn_to_server_fd, user_agent_hdr, strlen(user_agent_hdr));
    char *connection_hdr = "Connection: close\r\n";
    Rio_writen(conn_to_server_fd, connection_hdr, strlen(connection_hdr));
    char *proxy_conn_hdr = "Proxy-Connection: close\r\n";
    Rio_writen(conn_to_server_fd, proxy_conn_hdr, strlen(proxy_conn_hdr));
    
    return conn_to_server_fd;

    /* original request header parser. deprecated */
    //read_requesthdrs(&rio);                              

    /* Parse URI from GET request */
    //is_static = parse_uri(uri, filename, cgiargs);       
    //if (stat(filename, &sbuf) < 0) {                     
    //    clienterror(fd, filename, "404", "Not found",
    //            "Tiny couldn't find this file");
    //    return;
    //}                                                    

    /* 
     * behaving like a server code as below.
     */
    //if (is_static) { /* Serve static content */          
	//if (!(S_ISREG(sbuf.st_mode)) || !(S_IRUSR & sbuf.st_mode)) { //line:netp:from_client_to_server:readable
	//    clienterror(fd, filename, "403", "Forbidden",
	//		"Tiny couldn't read the file");
	//    return;
	//}
	//serve_static(fd, filename, sbuf.st_size);        //line:netp:from_client_to_server:servestatic
    //}
    //else { /* Serve dynamic content */
	//if (!(S_ISREG(sbuf.st_mode)) || !(S_IXUSR & sbuf.st_mode)) { //line:netp:from_client_to_server:executable
	//    clienterror(fd, filename, "403", "Forbidden",
	//		"Tiny couldn't run the CGI program");
	//    return;
	//}
	//serve_dynamic(fd, filename, cgiargs);            //line:netp:from_client_to_server:servedynamic
    //}
}
/* $end from_client_to_server */

/*
 * read_requesthdrs - read HTTP request headers
 */
/* $begin read_requesthdrs */
void read_requesthdrs(rio_t *rp) 
{
    char buf[MAXLINE];

    Rio_readlineb(rp, buf, MAXLINE);
    printf("%s", buf);
    while(strcmp(buf, "\r\n")) {          //line:netp:readhdrs:checkterm
	Rio_readlineb(rp, buf, MAXLINE);
	printf("%s", buf);
    }
    return;
}
/* $end read_requesthdrs */

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

/*
 * serve_static - copy a file back to the client 
 */
/* $begin serve_static */
void serve_static(int fd, char *filename, int filesize)
{
    int srcfd;
    char *srcp, filetype[MAXLINE], buf[MAXBUF];

    /* Send response headers to client */
    get_filetype(filename, filetype);    //line:netp:servestatic:getfiletype
    sprintf(buf, "HTTP/1.0 200 OK\r\n"); //line:netp:servestatic:beginserve
    Rio_writen(fd, buf, strlen(buf));
    sprintf(buf, "Server: Tiny Web Server\r\n");
    Rio_writen(fd, buf, strlen(buf));
    sprintf(buf, "Content-length: %d\r\n", filesize);
    Rio_writen(fd, buf, strlen(buf));
    sprintf(buf, "Content-type: %s\r\n\r\n", filetype);
    Rio_writen(fd, buf, strlen(buf));    //line:netp:servestatic:endserve

    /* Send response body to client */
    srcfd = Open(filename, O_RDONLY, 0); //line:netp:servestatic:open
    srcp = Mmap(0, filesize, PROT_READ, MAP_PRIVATE, srcfd, 0); //line:netp:servestatic:mmap
    Close(srcfd);                       //line:netp:servestatic:close
    Rio_writen(fd, srcp, filesize);     //line:netp:servestatic:write
    Munmap(srcp, filesize);             //line:netp:servestatic:munmap
}

/*
 * get_filetype - derive file type from file name
 */
void get_filetype(char *filename, char *filetype) 
{
    if (strstr(filename, ".html"))
	strcpy(filetype, "text/html");
    else if (strstr(filename, ".gif"))
	strcpy(filetype, "image/gif");
    else if (strstr(filename, ".png"))
	strcpy(filetype, "image/png");
    else if (strstr(filename, ".jpg"))
	strcpy(filetype, "image/jpeg");
    else
	strcpy(filetype, "text/plain");
}  
/* $end serve_static */

/*
 * serve_dynamic - run a CGI program on behalf of the client
 */
/* $begin serve_dynamic */
void serve_dynamic(int fd, char *filename, char *cgiargs) 
{
    char buf[MAXLINE], *emptylist[] = { NULL };

    /* Return first part of HTTP response */
    sprintf(buf, "HTTP/1.0 200 OK\r\n"); 
    Rio_writen(fd, buf, strlen(buf));
    sprintf(buf, "Server: Tiny Web Server\r\n");
    Rio_writen(fd, buf, strlen(buf));
  
    if (Fork() == 0) { /* Child */ //line:netp:servedynamic:fork
	/* Real server would set all CGI vars here */
	setenv("QUERY_STRING", cgiargs, 1); //line:netp:servedynamic:setenv
	Dup2(fd, STDOUT_FILENO);         /* Redirect stdout to client */ //line:netp:servedynamic:dup2
	Execve(filename, emptylist, environ); /* Run CGI program */ //line:netp:servedynamic:execve
    }
    Wait(NULL); /* Parent waits for and reaps child */ //line:netp:servedynamic:wait
}
/* $end serve_dynamic */

/*
 * clienterror - returns an error message to the client
 */
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
    
}
void parse_hostname(char *uri, char *hostname, char *rem_path) {
    int cnt = 0;
    size_t uri_len = strlen(uri);
    char *st_pos, *end_pos;
    
    for (size_t i = 0; i < uri_len; ++i) {
        if (uri[i] == '/') {
            if (cnt < 1) {
            } else if (cnt == 1) {
                st_pos = uri+i+1;
            } else if (cnt == 2) {
                end_pos = uri+i;
                strncpy(hostname, st_pos, end_pos-st_pos);
            }
            cnt++;
        }
    }
    strncpy(rem_path, end_pos, (uri+uri_len-1) - end_pos + 1);
    return ;
}
