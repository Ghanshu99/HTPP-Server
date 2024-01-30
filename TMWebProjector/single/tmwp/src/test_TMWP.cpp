#include<tmwp>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<unistd.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
using namespace tmwp;
typedef struct _request
{
char *method;
char *resource;
char isClientSideTechnologyResource;
char *mimeType;	
int dataCount;
char **data;
}REQUEST;
int extensionEquals(const char *left,const char *right)
{
char a,b;
while(*left && *right)
{
a=*left;
b=*right;
if(a>=65 && a<=90) a+=32;
if(b>=65 && b<=90) b+=32;
if(a!=b) return 0;
left++;
right++;
}
return *left==*right;
}


char *getMIMEType(char *resource)
{
char *mimeType;
mimeType=NULL;
int len=strlen(resource);
if(len<4) return mimeType;
int lastIndexOfDot=len-1;
while(lastIndexOfDot>0 && resource[lastIndexOfDot]!='.') lastIndexOfDot--;
if(lastIndexOfDot==0) return mimeType;
if(extensionEquals(resource+lastIndexOfDot+1,"html"));
{
mimeType=(char *)malloc(sizeof(char)*10);
strcpy(mimeType,"text/html");
}
if(extensionEquals(resource+lastIndexOfDot+1,"css"))
{
mimeType=(char *)malloc(sizeof(char)*9);
strcpy(mimeType,"text/css");
}
if(extensionEquals(resource+lastIndexOfDot+1,"js"))
{
mimeType=(char *)malloc(sizeof(char)*16);
strcpy(mimeType,"text/javascript");
}
if(extensionEquals(resource+lastIndexOfDot+1,"jpg"))
{
mimeType=(char *)malloc(sizeof(char)*11);
strcpy(mimeType,"image/jpeg");
}
if(extensionEquals(resource+lastIndexOfDot+1,"jpeg"))
{
mimeType=(char *)malloc(sizeof(char)*11);
strcpy(mimeType,"image/jpeg");
}
if(extensionEquals(resource+lastIndexOfDot+1,"png"))
{
mimeType=(char *)malloc(sizeof(char)*10);
strcpy(mimeType,"image/png");
}
if(extensionEquals(resource+lastIndexOfDot+1,"ico"))
{
mimeType=(char *)malloc(sizeof(char)*13);
strcpy(mimeType,"image/x-icon");
}
return mimeType;
}


char isClientSideResource(char *resource)
{
int i;
for(i=0;resource[i]!='\0' && resource[i]!='.';i++);
if(resource[i]=='\0') return 'N';  
return 'Y';
}


REQUEST *parseRequest(char *bytes)
{
char method[11];
char resource[1001];
int i,j;
for(i=0;bytes[i]!=' ';i++) method[i]=bytes[i];
method[i]='\0';
i+=2;

char **data=NULL;
int dataCount=0;
if(strcmp(method,"GET")==0)
{
//whatever?rl=23&nm=rohit
for(j=0;bytes[i]!=' ';j++,i++)
{
if(bytes[i]=='?') break;
resource[j]=bytes[i];
}
resource[j]='\0';
if(bytes[i]=='?')
{
i++;
int si=i;
dataCount=0;
while(bytes[i]!=' ')
{
if(bytes[i]=='&') dataCount++;
i++; 
}
dataCount++;
data=(char **)malloc(sizeof(char *)*dataCount);
int *pc=(int *)malloc(sizeof(int)*dataCount);
i=si;
int j=0;
while(bytes[i]!=' ')
{
if(bytes[i]=='&')
{
pc[j]=i;
j++;
}
i++;
}
pc[j]=i;
i=si;
j=0;
int howManyToPick;
while(j<dataCount)
{
howManyToPick=pc[j]-i;
data[j]=(char *)malloc(sizeof(char)*(howManyToPick+1));
strncpy(data[j],bytes+i,howManyToPick);
data[j][howManyToPick]='\0';
i=pc[j]+1;
j++;
}
}
}//method is of GET type
printf("Request arrived for %s\n",resource);
REQUEST *request=(REQUEST *)malloc(sizeof(REQUEST));
request->dataCount=dataCount;
request->data=data;
request->method=(char *)malloc((sizeof(char)*strlen(method))+1);
strcpy(request->method,method);
if(resource[0]=='\0')
{
request->resource=NULL;
request->isClientSideTechnologyResource='Y';
request->mimeType=NULL;
}
else
{
request->resource=(char *)malloc((sizeof(char)*strlen(resource))+1);
strcpy(request->resource,resource);
request->isClientSideTechnologyResource=isClientSideResource(resource);
request->mimeType=getMIMEType(resource);
}
return request;
}


TMWebProjector::TMWebProjector(int portNumber)
{
this->portNumber=portNumber;
this->url=NULL;
this->ptrOnRequest=NULL;
}
TMWebProjector::~TMWebProjector()
{
if(this->url) delete [] this->url;
}
void TMWebProjector::onRequest(const char *url,void (*ptrOnRequest)(int,char *[]))
{
if(this->url) delete [] this->url;
this->url=NULL;
this->ptrOnRequest=NULL;
if(url==NULL || ptrOnRequest==NULL) return;
this->url=new char[strlen(url)+1];
strcpy(this->url,url);
this->ptrOnRequest=ptrOnRequest;
}

void TMWebProjector::start()
{
FILE *f;
int length;
int i,rc;
char g;
char responseBuffer[1024];
char requestBuffer[8192];
int bytesExtracted;
int serverSocketDescriptor;
int clientSocketDescriptor;
struct sockaddr_in serverSocketInformation;
struct sockaddr_in clientSocketInformation;
int successCode;
int len;
serverSocketDescriptor=socket(AF_INET,SOCK_STREAM,0);
if(serverSocketDescriptor<0)
{
printf("Unable to create socket\n");
return;
}
serverSocketInformation.sin_family=AF_INET;
serverSocketInformation.sin_port=htons(this->portNumber);
serverSocketInformation.sin_addr.s_addr=htonl(INADDR_ANY);
successCode=bind(serverSocketDescriptor,(struct sockaddr *)&serverSocketInformation,sizeof(serverSocketInformation));
char message[101];
if(successCode<0)
{
sprintf(message,"Unable to bind at port %d\n",this->portNumber);
printf("%s\n",message);
return;
}
listen(serverSocketDescriptor,10);
len=sizeof(clientSocketInformation);
while(1)
{
sprintf(message,"TMServer is ready to accept request on port %d\n",this->portNumber);
printf("%s\n",message);
clientSocketDescriptor=accept(serverSocketDescriptor,(struct sockaddr *)&clientSocketInformation,(socklen_t*)&len);
if(clientSocketDescriptor<0)
{
printf("Unable to accept client connection\n");
return;
}
bytesExtracted=recv(clientSocketDescriptor,requestBuffer,8192,0);
if(bytesExtracted<0)
{

}else
if(bytesExtracted==0)
{

}else
{
requestBuffer[bytesExtracted]='\0';
REQUEST *request=parseRequest(requestBuffer);
if(request->isClientSideTechnologyResource=='Y')
{
if(request->resource==NULL)
{
f=fopen("index.html","rb");
if(f!=NULL) printf("Sending index.html\n");
if(f==NULL) 
{
f=fopen("index.htm","rb");
if(f!=NULL) printf("Sending index.htm\n");
}
if(f==NULL)
{
strcpy(responseBuffer,"HTTP/1.1 200 0K\nContent-type:text/html\nContent-Length:163\nConnection: close\n\n<DOCTYPE HTML><html lang='en'><head><meta charset='utf-8'><title>TM Web Projector</title></head><body><h2 style='color:red'>Resource / not found</h2></body><html>");
send(clientSocketDescriptor,responseBuffer,strlen(responseBuffer),0);
printf("Sending 404 page\n");
}
else
{
fseek(f,0,2);
length=ftell(f);
fseek(f,0,0);
sprintf(responseBuffer,"HTTP/1.1 200 OK\nContent-Type:%s\nContent-Length:%d\nConnection: close\n\n",request->mimeType,length);
send(clientSocketDescriptor,responseBuffer,strlen(responseBuffer),0);
i=0;
while(i<length)
{
rc=length-i;
if(rc>1024) rc=1024;
fread(&responseBuffer,rc,1,f);
send(clientSocketDescriptor,responseBuffer,rc,0);
i+=rc;
}
fclose(f);
close(clientSocketDescriptor);
}
}
else
{
f=fopen(request->resource,"rb");
if(f!=NULL) printf("Sending %s\n",request->resource);
if(f==NULL)
{
//something is need to done over here
printf("Sending 404 page\n");
char tmp[501];
sprintf(tmp,"<DOCTYPE HTML><html lang='en'><head><meta charset='utf-8'><title>TM Web Projector</title></head><body><h2  style='color:red'>Resource / not found</h2></body></html>");	//done done
sprintf(responseBuffer,"HTTP/1.1 200 0K\nContent-Type:text/html\nContent-Length:%d\nConnection: close\n\n",strlen(tmp));
strcat(responseBuffer,tmp);
send(clientSocketDescriptor,responseBuffer,strlen(responseBuffer),0);
}
else
{
fseek(f,0,2);
length=ftell(f);
fseek(f,0,0);
sprintf(responseBuffer,"HTTP/1.1 200 OK\nContent-Type:%s\nContent-Length:%d\nConnection: close\n\n",request->mimeType,length);
send(clientSocketDescriptor,responseBuffer,strlen(responseBuffer),0);
i=0;
while(i<length)
{
rc=length-i;
if(rc>1024) rc=1024;
fread(&responseBuffer,rc,1,f);
send(clientSocketDescriptor,responseBuffer,rc,0);
i+=rc;
}
fclose(f);
close(clientSocketDescriptor);
}
}
}
else  //server side technology resource
{
//more changes required in context to server side resource
if(this->url==NULL || this->ptrOnRequest==NULL)
{
printf("Sending 404 page\n");
char tmp[501];
sprintf(tmp,"<DOCTYPE HTML><html lang='en'><head><meta charset='utf-8'><title>TM Web Projector</title></head><body><h2 style='color:red'>Resource /%s not found</h2></body></html>",request->resource);
sprintf(responseBuffer,"HTTP/1.1 200 OK\nContent-Type:text/html\nContent-Length:%d\nConnection: close\n\n",strlen(tmp));
strcat(responseBuffer,tmp);
send(clientSocketDescriptor,responseBuffer,strlen(responseBuffer),0);
}	
else
{   
int ii=0;
if(this->url[0]=='/') ii=1;
if(strcmp(this->url+ii,request->resource)==0)
{
Request rq;
Response rs;
rq.dataCount=request->dataCount;
rq.data=request->data;
rq.mimeType=request->mimeType;
rq.isClientSideTechnologyResource=request->isClientSideTechnologyResource;
rq.resource=request->resource;
rq.method=request->method;
rs.clientSocketDescriptor=clientSocketDescriptor;
rs.serverSocketDescriptor=serverSocketDescriptor;
this->ptrOnRequest(rq,rs);
if(request->data!=NULL)
{
for(int k=0;k<request->dataCount;k++) free(request->data[k]);
free(request->data);
}
printf("Sending processd page\n");
char tmp[501];
sprintf(tmp,"<DOCKTYPE HTML><html lang='en'><head><meta charset='utf-8'><title>TMWeb Projector</title></head><body><h2 style='color:red'>Resource /%s processed</h2></body></html>",request->resource);
sprintf(responseBuffer,"HTTP/1.1 200 OK\nContent-Type: text/html\nContent-Length: %d\nConnection: close\n\n",strlen(tmp));
strcat(responseBuffer,tmp);
send(clientSocketDescriptor,responseBuffer,strlen(responseBuffer),0);
}
else
{
printf("Sending 404 page\n");
char tmp[501];
sprintf(tmp,"<DOCTYPE HTML><html lang='en'><head><meta charset='utf-8'><title>TM Web Projector</title></head><body><h2 style='color:red'>Resource /%s not found</h2></body></html>",request->resource);
sprintf(responseBuffer,"HTTP/1.1 200 OK\nContent-Type:text/html\nContent-Length:%d\nConnection: close\n\n",strlen(tmp));
strcat(responseBuffer,tmp);
send(clientSocketDescriptor,responseBuffer,strlen(responseBuffer),0);
}
}
}
}
}
return;
}
