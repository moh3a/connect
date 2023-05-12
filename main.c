#include <stdio.h>
#include <sql.h>
#include <sqlext.h>
#include <string.h>

#include "connect.c"

/*
This is the first handle you will need as everything else is effectively in the environment. Once you have an environment handle you can define the version of ODBC you require, enable connection pooling and allocate connection handles with SQLSetEnvAttr and SQLAllocHandle.
*/
SQLHENV environment_handle;

/*
You need one connection handle for each data source you are going to connect to. Like environment handles, connection handles have attributes which you can retrieve and set with SQLSetConnectAttr and SQLGetConnectAttr.
*/
SQLHDBC connection_handle;

/*
Once you have a connection handle and have connected to a data source you allocate statement handles to execute SQL or retrieve meta data. As with the other handles you can set and get statement attributes with SQLSetStmtAttr and SQLGetStmtAttr.
*/
SQLHSTMT statement_handle;

int main()
{
    printf("hello world");
    connect(environment_handle, connection_handle, "MYSQL_DSN");
    return 0;
}
