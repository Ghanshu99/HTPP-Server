#ifndef ___TPL
#define ___TPL
#include<tmwp>
using namespace tmwp;
void getErrorPage_TPL(Request &request,Response &response);
void registerTPLs(TMWebProjector *server)
{
server->onRequest("/ErrorPage.tpl",getErrorPage_TPL);
}
#endif