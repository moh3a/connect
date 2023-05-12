#include <stdio.h>
#include <sql.h>
#include <sqlext.h>

/*
When you start working with ODBC you will inevitably find differences between ODBC drivers. ODBC defines a set of informational types you can retrieve which describe the ODBC driver, driver manager and data source. You use SQLGetInfo to retrieve this information.

There are basically 3 types of information returned - strings, bitmasks and integer values. String information types are sometimes binary values represented by 'Y' and 'N'. To retrieve an informational type, look it up in the ODBC specification to find out what type it is and then use one of the following:
*/
void get_driver_information(SQLHENV environment_handle, SQLHDBC connection_handle)
{
    SQLRETURN ret; /* ODBC API return status */

    /* Allocate an environment handle */
    SQLAllocHandle(SQL_HANDLE_ENV, SQL_NULL_HANDLE, &environment_handle);
    /* We want ODBC 3 support */
    SQLSetEnvAttr(environment_handle, SQL_ATTR_ODBC_VERSION, (void *)SQL_OV_ODBC3, 0);
    /* Allocate a connection handle */
    SQLAllocHandle(SQL_HANDLE_DBC, environment_handle, &connection_handle);
    /* Connect to the DSN mydsn */
    ret = SQLDriverConnect(connection_handle, NULL, "DSN=web;", SQL_NTS,
                           NULL, 0, NULL,
                           SQL_DRIVER_COMPLETE);
    if (SQL_SUCCEEDED(ret))
    {
        SQLCHAR dbms_name[256], dbms_ver[256];
        SQLUINTEGER getdata_support;
        SQLUSMALLINT max_concur_act;
        SQLSMALLINT string_len;

        printf("Connected\n");
        /*
         *  Find something out about the driver.
         */
        SQLGetInfo(connection_handle, SQL_DBMS_NAME, (SQLPOINTER)dbms_name,
                   sizeof(dbms_name), NULL);
        SQLGetInfo(connection_handle, SQL_DBMS_VER, (SQLPOINTER)dbms_ver,
                   sizeof(dbms_ver), NULL);
        SQLGetInfo(connection_handle, SQL_GETDATA_EXTENSIONS, (SQLPOINTER)&getdata_support,
                   0, 0);
        SQLGetInfo(connection_handle, SQL_MAX_CONCURRENT_ACTIVITIES, &max_concur_act, 0, 0);

        printf("DBMS Name: %s\n", dbms_name);
        printf("DBMS Version: %s\n", dbms_ver);
        if (max_concur_act == 0)
        {
            printf("SQL_MAX_CONCURRENT_ACTIVITIES - no limit or undefined\n");
        }
        else
        {
            printf("SQL_MAX_CONCURRENT_ACTIVITIES = %u\n", max_concur_act);
        }
        if (getdata_support & SQL_GD_ANY_ORDER)
            printf("SQLGetData - columns can be retrieved in any order\n");
        else
            printf("SQLGetData - columns must be retrieved in order\n");
        if (getdata_support & SQL_GD_ANY_COLUMN)
            printf("SQLGetData - can retrieve columns before last bound one\n");
        else
            printf("SQLGetData - columns must be retrieved after last bound one\n");

        SQLDisconnect(connection_handle); /* disconnect from driver */
    }
    else
    {
        fprintf(stderr, "Failed to connect\n");
    }
    /* free up allocated handles */
    SQLFreeHandle(SQL_HANDLE_DBC, connection_handle);
    SQLFreeHandle(SQL_HANDLE_ENV, environment_handle);

    /*
    output example:
    Connected
    DBMS Name: Microsoft SQL Server
    DBMS Version: 08.00.0760
    SQL_MAX_CONCURRENT_ACTIVITIES = 1
    SQLGetData - columns must be retrieved in order
    SQLGetData - columns must be retrieved after last bound one
    */
}