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

Request::Request()
{
this->method=NULL;
this->resource=NULL;
this->isClientSideTechnologyResource='\0';
this->mimeType=NULL;
this->dataCount=0;
this->data=NULL;
}


// char * Request::get(string str) 
// {
// char *rollNumber;
// char *name;
// if(str=="rl")
// {
// i=0;
// while(data[0][i]!='\0')
// {
// if(data[0][i]=='=') break;
// i++;
// }
// i++;
// rollNumber=(char *)malloc(sizeof(rollNumber)*2);
// strcpy(rollNumber,request.data[0][i]);
// return rollNumber;
// }
// if(str=="nm")
// {
// i=0;
// while(data[1][i]!='\0')
// {
// if(data[1][i]=='=') break;
// i++;
// }
// i++;
// name=(char *)malloc(sizeof(char)*);
// strcpy(name,request.data[1][i]);
// return name;
// }
// }


char * Request::get(string query) //query=nm
{
char *nn=(char *)malloc(sizeof(char)*2);
nn[0]=' ';
nn[1]='\0';
char *mm;
char *parameter;
char *a;
int found,x,e,len,f;
len=query.length();
parameter=(char *)malloc(sizeof(char)*len+1);
for(f=0;query[f]!='\0';f++) parameter[f]=query[f];
parameter[f]='\0';
found=0;
for(x=0;x<dataCount;x++)
{
a=(char *)malloc(sizeof(char)*len+1);
a[0]=data[x][0];
a[1]=data[x][1];
a[2]='\0';
if(strcmp(a,parameter)==0)
{
found=1;
break;
}
}
if(found==1)
{
f=len+1;
len=strlen(data[x]);
len=len-f+1;
mm=(char *)malloc(sizeof(char)*len);
for(e=0;data[x][f]!='\0';e++,f++)  mm[e]=data[x][f];
mm[e]='\0';
return mm;
}
else
{
return nn;
}
}


Response::Response()
{
this->clientSocketDescriptor=0;
this->serverSocketDescriptor=0;
}
void Response::write(string message)
{
strcat(str,message.c_str());
}
void Response::close()
{
char responseBuffer[5000];
sprintf(responseBuffer,"HTTP/1.1 200 OK\nContent-Type: text/html\nContent-Length: %d\n\n",strlen(str));
strcat(responseBuffer,str);
send(clientSocketDescriptor,responseBuffer,strlen(responseBuffer),0);
responseBuffer[0]='\0';
str[0]='\0';
//closesocket(serverSocketDescriptor);
}
int extensionEquals(const char *left,const char * right)
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
if(extensionEquals(resource+lastIndexOfDot+1,"html"))
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
data=(char **)malloc(sizeof(char*)*dataCount);
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
if(this->url) delete[] this->url;
}
void TMWebProjector::onRequest(const char *url,void(*ptrOnRequest)(Request &,Response &))
{                  //onRequest("/addStudent",addStud);
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
int rc,i,length;
char g;
int bytesExtracted;
char responseBuffer[1024];
char requestBuffer[8192];
struct sockaddr_in serverSocketInformation;
struct sockaddr_in clientSocketInformation;
int serverSocketDescriptor;
int clientSocketDescriptor;
int len;
int successCode;
serverSocketDescriptor=socket(AF_INET,SOCK_STREAM,0);
if(serverSocketDescriptor<0)
{
printf("Unable to create socket\n");
return ;
}
char message[101];
serverSocketInformation.sin_family=AF_INET;
serverSocketInformation.sin_port=htons(this->portNumber);
serverSocketInformation.sin_addr.s_addr=htonl(INADDR_ANY);
successCode=bind(serverSocketDescriptor,(struct sockaddr *)&serverSocketInformation,sizeof(serverSocketInformation));
if(successCode<0)
{
sprintf(message,"Unable to bind socket to port %d",this->portNumber);
printf("%s\n",message);
return ;
}
listen(serverSocketDescriptor,10);
while(1)
{
sprintf(message,"TMServer is ready to accept request on port %d",this->portNumber);
printf("%s\n",message);
len=sizeof(clientSocketInformation);
clientSocketDescriptor=accept(serverSocketDescriptor,(struct sockaddr *)&clientSocketInformation,(socklen_t*)&len);
if(clientSocketDescriptor<0)
{
printf("Unable to accept client connection\n");
close(serverSocketDescriptor);
return ;
}
bytesExtracted=recv(clientSocketDescriptor,requestBuffer,8192,0);
if(bytesExtracted<0)
{
// what to do is yet to be decided
}
else
{
if(bytesExtracted==0)
{
// what to do is yet to be decided
}
else
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
strcpy(responseBuffer,"HTTP/1.1 200 OK\nContent-Type: text/html\nContent-Length: 163\nConnection: close\n\n<DOCKTYPE HTML><html lang='en'><head><meta charset='utf-8'><title>TMWeb Projector</title></head><body><h2 style='color:red'>Resource / not found</h2></body></html>");
successCode=send(clientSocketDescriptor,responseBuffer,strlen(responseBuffer),0);
printf("Sending 404 page\n");
}
else
{
fseek(f,0,2);
length=ftell(f);
fseek(f,0,0);
sprintf(responseBuffer,"HTTP/1.1 200 OK\nContent-Type: text/html\nContent-Length: %d\nConnection: close\n\n",length);
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
if(f==NULL)
{
char tmp[501];
printf("Sending 404 page\n");
sprintf(tmp,"<DOCKTYPE HTML><html lang='en'><head><meta charset='utf-8'><title>TMWeb Projector</title></head><body><h2 style='color:red'>Resource /%s not found</h2></body></html>",request->resource);
sprintf(responseBuffer,"HTTP/1.1 200 OK\nContent-Type: text/html\nContent-Length: %d\n\n",strlen(tmp));
strcat(responseBuffer,tmp);
send(clientSocketDescriptor,responseBuffer,strlen(responseBuffer),0);
}
else
{
fseek(f,0,2);
length=ftell(f);
fseek(f,0,0);
sprintf(responseBuffer,"HTTP/1.1 200 OK\nContent-Type: text/html\nContent-Length: %d\nConnection: close\n\n",length);
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
else
{
// more changes required in context to server side resource
if(this->url==NULL || this->ptrOnRequest==NULL)
{
printf("Sending 404 page\n");
char tmp[501];
sprintf(tmp,"<DOCKTYPE HTML><html lang='en'><head><meta charset='utf-8'><title>TMWeb Projector</title></head><body><h2 style='color:red'>Resource /%s not found</h2></body></html>",request->resource);
sprintf(responseBuffer,"HTTP/1.1 200 OK\nContent-Type: text/html\nContent-Length: %d\nConnection: close\n\n",strlen(tmp));
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
char tmp[501];
printf("Sending 404 page\n");
sprintf(tmp,"<DOCKTYPE HTML><html lang='en'><head><meta charset='utf-8'><title>TMWeb Projector</title></head><body><h2 style='color:red'>Resource /%s not found</h2></body></html>",request->resource);
sprintf(responseBuffer,"HTTP/1.1 200 OK\nContent-Type: text/html\nContent-Length: %d\nConnection: close\n\n",strlen(tmp));
strcat(responseBuffer,tmp);
send(clientSocketDescriptor,responseBuffer,strlen(responseBuffer),0);
}
}
}
}
}
}// the infinite loop related to accept method ends here

strcpy(responseBuffer,"Welcome to Thinking Machines");
successCode=send(clientSocketDescriptor,responseBuffer,strlen(responseBuffer)+1,0);
if(successCode>0)
{
printf("Response sent\n");
}
else
{
printf("Unable to sent response\n");
}
close(clientSocketDescriptor);
close(serverSocketDescriptor);
return ;
}