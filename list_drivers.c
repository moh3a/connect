#include <sql.h>
#include <sqlext.h>

void list_drivers(SQLHENV environment_handle)
{
    char driver[256];
    char attr[256];
    SQLSMALLINT driver_ret;
    SQLSMALLINT attr_ret;
    SQLUSMALLINT direction;
    SQLRETURN ret;

    SQLAllocHandle(SQL_HANDLE_ENV, SQL_NULL_HANDLE, &environment_handle);
    SQLSetEnvAttr(environment_handle, SQL_ATTR_ODBC_VERSION, (void *)SQL_OV_ODBC3, 0);

    direction = SQL_FETCH_FIRST;
    while (SQL_SUCCEEDED(ret = SQLDrivers(environment_handle, direction,
                                          driver, sizeof(driver), &driver_ret,
                                          attr, sizeof(attr), &attr_ret)))
    {
        direction = SQL_FETCH_NEXT;
        printf("%s - %s\n", driver, attr);
        if (ret == SQL_SUCCESS_WITH_INFO)
            printf("\tdata truncation\n");
    }
    /*
    output example:
    OOB - Easysoft ODBC-ODBC Bridge
    PostgreSQL - Postgres SQL Driver
    */
}
