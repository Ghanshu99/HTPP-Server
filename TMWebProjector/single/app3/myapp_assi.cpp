#include<tmwp>
#include<iostream>
using namespace std;
using namespace tmwp;
void One(Request &request,Response &response)
{
cout<<"one got called"<<endl;
// void request.setInt("abcd",123);
request.forward("xyz.html");
}
void Two(Request &request,Response &response)
{
cout<<"two got called"<<endl;
request.forward("/three");	
}
void Three(Request &request,Response &response)
{
cout<<"three got called"<<endl;
}
int main()
{
TMWebProjector server(8080);
server.onRequest("/one",One);
server.onRequest("/two",Two);
server.onRequest("/three",Three);
server.start();
return 0;
}