#include <sql.h>
#include <sqlext.h>

void list_data_sources(SQLHENV environment_handle)
{
    char dsn[256];
    char desc[256];
    SQLSMALLINT dsn_ret;
    SQLSMALLINT desc_ret;
    SQLUSMALLINT direction;
    SQLRETURN ret;

    SQLAllocHandle(SQL_HANDLE_ENV, SQL_NULL_HANDLE, &environment_handle);
    SQLSetEnvAttr(environment_handle, SQL_ATTR_ODBC_VERSION, (void *)SQL_OV_ODBC3, 0);

    direction = SQL_FETCH_FIRST;
    while (SQL_SUCCEEDED(ret = SQLDataSources(environment_handle, direction,
                                              dsn, sizeof(dsn), &dsn_ret,
                                              desc, sizeof(desc), &desc_ret)))
    {
        direction = SQL_FETCH_NEXT;
        printf("%s - %s\n", dsn, desc);
        if (ret == SQL_SUCCESS_WITH_INFO)
            printf("\tdata truncation\n");
    }
    /*
    output example:
    mydsn - OOB
    pdsn - Postgres
    */
}
