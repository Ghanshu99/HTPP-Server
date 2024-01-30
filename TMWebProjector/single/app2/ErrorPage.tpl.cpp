#include<tmwp>
using namespace tmwp;
void getErrorPage_TPL(Request &request,Response &response)
{
response.write("<!DOCTYPE html>");
response.write("<html lang='en'>");
response.write("<head>");
response.write("<meta charset='utf-8'>");
response.write("<title>The Clock</title>");
response.write("</head>");
response.write("<body>");
response.write("<h1>");
response.write(request.getValue("error"));
response.write("</h1>");
response.write("<br>");
response.write("<a href='index.html'>Home</a>");
response.write("</body>");
response.write("</html> ");
}