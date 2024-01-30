#include<tmwp>
#include<iostream>
using namespace std;
using namespace tmwp;
void addStud(Request &request,Response &response)
{
class Student 
{
public:
int rollNumber;
String name;
}
File *f;
String r=request.get("rl");
String n=request.get("nm");
Student s;
s.rollNumber=(int)r;
s.name=n;
f=fopen("serverdata.dat","ab");
fwrite(&s,sizeof(Student),1,f);
fclose(f);
response.write("<DOCTYPE HTML>");
response.write("<html lang='en'>");
response.write("<head>");
response.write("<meta charset='utf-8'>");
response.write("<title>TM Web Projector</title>");
response.write("</head>");
response.write("<body>");
response.write("<h2 style='color:green'>Student added</h2>");
response.write("<a href='index.html'>ok</a>");
response.write("</body>");
response.write("</html>");
response.close();
// no need to check uniqueness of the roll number
// but be sure that you are not adding duplicate roll number
// code to add data to a file 
// put data into object , open file in append and binary mode
// write object
// using response.write send back html that says, student added
// and an link (ok) to send request for index.html
// in the end don't forget to call for close method
}
void getAllStudents(Request &request,Response &response)
{
File *f;
String rollNumber;
String name;
f=fopen("serverdata.dat","rb");
fread(&s,sizeof(Student),1,f);
fclose(f);
rollNumber=s.rollNumber;
name=s.name;
response.write("<DOCTYPE HTML>");
response.write("<html lang='en'>");
response.write("<head>");
response.write("<meta charset='utf-8'>");
response.write("<title>TM Web Projector</title>");
response.write("</head>");
response.write("<body>");
response.write("<table border='1'>");
response.write("<tr>");
response.write("<td>S.No.</td><td>Roll No.</td><td>Name</td>");
response.write("</tr>");
response.write("<tr><td>1.</td>rollNumber<td><td>name</td></td></tr>");
response.write("</body>");
response.write("</html>");
response.close();

// code to open data file
// create html with data from file and send it using response.write
// in the end don't forget to call close method
}
void sendStudentEditForm(Request &request,Response &response)
{
String r=request.get("rl");
//extract rollnumber from request rl
//file handling code to search roll number and 
//send html
response.write("<!DOCTYPE HTML>");
response.write("html lang='en'");
response.write("<head>");
response.write("<meta charset='utf-8'");
response.write("<title>ABCL School, Ujjain</title>");
response.write("</head>");
response.write("<body>");
response.write("Student(Edit Module)");
response.write("<form method='get' action='updateStudent'>");
response.write("Roll number: 101");
response.write("<input type='hidden' id='rl' name='rl' value='101'><br>");
response.write("Name <input type='text' id='nm' name='nm' value='Sameer'><br>");
response.write("<button type='submit'>Update</button>");
response.write("</form>");
response.write("<a href='index.html'>Home</a>");
response.write("</body>");
response.write("</html>");
response.close();
}
void updateStudentData(Request &request,Response &response)
{
//code to extract data against rl and nm (roll number and name)
// file handling code to search roll number and update data
// send back html
response.write("<!DOCTYPE HTML>");
response.write("html lang='en'");
response.write("<head>");
response.write("<meta charset='utf-8'");
response.write("<title>ABCL School, Ujjain</title>");
response.write("</head>");
response.write("<body>");
response.write("Student(Edit Module)");
response.write("<h3>Student updated</h3>");
response.write("<form method='get' action='index.html'>");
response.write("<button type='submit'>Ok</button>");
response.write("</form>");
response.write("</body>");
response.write("</html>");
response.close();

}
void sendStudentDeleteConfirmatinoForm(Request &request,Response &response)
{
//extract rollnumber from request rl
//file handling code to search roll number and 
//send html
response.write("<!DOCTYPE HTML>");
response.write("html lang='en'");
response.write("<head>");
response.write("<meta charset='utf-8'");
response.write("<title>ABCL School, Ujjain</title>");
response.write("</head>");
response.write("<body>");
response.write("Student(Delete Module)");
response.write("<form method='get' action='deleteStudent'>");
response.write("Roll number: 101");
response.write("Name:Sameer<br>");
response.write("<input type='hidden' id='rl' name='rl' value='101'><br>");
response.write("Name <input type='text' id='nm' name='nm' value='Sameer'><br>");
response.write("<button type='submit'>Delete</button>");
response.write("</form>");
response.write("<a href='index.html'>Home</a>");
response.write("</body>");
response.write("</html>");
response.close();
}
void deleteStudentData(Request &request,Response &response)
{
//code to extract data against rl(roll number)
// file handling code to search roll number and update data
// send back html
response.write("<!DOCTYPE HTML>");
response.write("html lang='en'");
response.write("<head>");
response.write("<meta charset='utf-8'");
response.write("<title>ABCL School, Ujjain</title>");
response.write("</head>");
response.write("<body>");
response.write("Student(Delete Module)");
response.write("<h3>Student Deleted</h3>");
response.write("<form method='get' action='index.html'>");
response.write("<button type='submit'>Ok</button>");
response.write("</form>");
response.write("</body>");
response.write("</html>");
response.close();
}

int main()
{
TMWebProjector server(8080);
server.onRequest("/addStudent",addStud);
server.onRequest("/getStudents",getAllStudents);
server.onRequest("/editStudent",sendStudentEditForm);
server.onRequest("/updateStudent",updateStudentData);
server.onRequest("/confirmDeleteStudent",sendStudentDeleteConfirmatinoForm);
server.onRequest("/deleteStudent",deleteStudent);
server.start();
return 0;
}