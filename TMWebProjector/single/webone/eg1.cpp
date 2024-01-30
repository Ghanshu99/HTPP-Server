#include<tmwp>
#include<iostream>
using namespace std;
using namespace tmwp;
/*void doSomething(int dataCount,char *data[])
{
cout<<"great, this code got executed on server side"<<endl;
if(dataCount>0) cout<<"Data that arrived"<<endl;
for(int r=0;r<dataCount;r++)
{
cout<<data[r]<<endl;
}
}
*/
void doSomething(Request &request,Response &response)
{
string n=request.get("nm");
string c=request.get("ct");
response.write("<!DOCTYPE HTML>");
response.write("<HTML lang='en'>");
response.write("<head>");
response.write("<TITLE>Whatever</TITLE>");
response.write("<meta charset='utf-8'>");
response.write("</head>");
response.write("<body>");
response.write("Hello");
response.write(n);
response.write("<BR>");
response.write("Data Saved");
response.write("</body>");
response.write("</html>");
}
int main()
{
TMWebProjector server(8080);
server.onRequest("/register",doSomething);
server.start();
return 0;
}




// #ifndef ___TMWP
// #define ___TMWP 13
// #include<iostream>
// #include<map>
// using namespace std;
// namespace tmwp
// {
// class Request
// { 
// public:
// char *method;
// char *resource;
// char isClientSideTechnologyResource;
// char *mimeType;
// int dataCount;
// char **data;
// string forwardTo;
// map<string,string> keyValues;
// void setKeyValue(string k,string v);
// string getValue(string k);
// string get(string name);
// void forward(string forwardTo);
// };
// class Response
// {
// private:
// int clientSocketDescriptor;
// bool isClosed;
// bool headerCreated;
// void createHeader();
// public:
// Response(int clientSocketDescriptor);
// void write(const char *str);
// void write(string str);
// //you need to overload more methods for primitive types
// void Close();
// };
// class TMWebProjector
// {
// private:
// int portNumber;
// map<string, void (*)(Request &,Response &)> requestMappings;
// public:
// TMWebProjector(int portNumber);
// ~TMWebProjector();
// void start();
// void onRequest(string url,void(*ptrOnRequest)(Request &,Response &));
// };
// }
// #endif



