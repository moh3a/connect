#include <stdio.h>
#include <sql.h>
#include <sqlext.h>

/*
Once you have a connection handle you can connect to your ODBC driver or data source using SQLDriverConnect (ODBC 2 applications use SQLConnect but this is much less flexible). Connecting to your ODBC driver is perhaps one of the largest subjects in ODBC as SQLDriverConnect can be called in many different ways; this introductory tutorial will cover the simplest case of connecting to a named data source you have already created and further tutorials will expand on this.
*/
void connect(SQLHENV environment_handle, SQLHDBC connection_handle, char *dsn)
{
    SQLRETURN ret; /* ODBC API return status */
    SQLCHAR outstr[1024];
    SQLSMALLINT outstrlen;

    /*
    The ODBC connection string contains a series of attributes names and values separated by semi-colons
    it contain some ODBC defined attributes (there are also driver defined attributes but these vary per driver). The ODBC defined attributes are:
    - DSN - the name of the data source to connect to.
    - DRIVER - the name of the driver to connect to. You can use this in DSN-less connections.
    - FILEDSN - the name of a file containing the connection attributes.
    - UID/PWD - any username and password the database requires for authentication.
    - SAVEFILE - request the DSN attributes are saved in this file.
    */
    char connection_string[100] = "DSN=";
    strcat(connection_string, dsn);
    strcat(connection_string, ';');

    /* Allocate an environment handle */
    SQLAllocHandle(SQL_HANDLE_ENV, SQL_NULL_HANDLE, &environment_handle);
    /* We want ODBC 3 support */
    SQLSetEnvAttr(environment_handle, SQL_ATTR_ODBC_VERSION, (void *)SQL_OV_ODBC3, 0);
    /* Allocate a connection handle */
    SQLAllocHandle(SQL_HANDLE_DBC, environment_handle, &connection_handle);
    /* Connect to the provided DSN */
    ret = SQLDriverConnect(
        connection_handle,
        NULL,
        connection_string,
        SQL_NTS,
        outstr,
        sizeof(outstr),
        &outstrlen,
        /*
        Argment specifies how the driver will handle the input connection string especially if there are insufficient attributes defined to connect.
        - SQL_DRIVER_PROMPT : The driver displays a dialog box, using the values from the connection string and system information (if any) as initial values. When the user exits the dialog box, the driver connects to the data source. It also constructs a connection string from the value of the DSN or DRIVER keyword in InConnectionString and the information returned from the dialog box. It places this connection string in the OutConnectionString buffer.
        - SQL_DRIVER_COMPLETE and SQL_DRIVER_COMPLETE_REQUIRED : If the connection string contains enough information and that information is correct, the driver connects to the data source and copies InConnectionString to OutConnectionString. If any information is missing or incorrect the driver takes the same actions as it does for SQL_DRIVER_PROMPT, except that if DriverComplete is SQL_DRIVER_COMPLETE_REQUIRED, the driver disables the controls for any information not required to connect to the data source.
        - SQL_DRIVER_NOPROMPT : If the connection string contains enough information, the driver connects to the data source and copies InConnectionString to OutConnectionString. Otherwise, the driver returns SQL_ERROR for SQLDriverConnect. You would usually use this from non-interactive applications.
        */
        SQL_DRIVER_COMPLETE);
    if (SQL_SUCCEEDED(ret))
    {
        printf("Connected\n");
        printf("Returned connection string was:\n\t%s\n", outstr);
        if (ret == SQL_SUCCESS_WITH_INFO)
        {
            printf("Driver reported the following diagnostics\n");
            extract_error("SQLDriverConnect", connection_handle, SQL_HANDLE_DBC);
        }
        SQLDisconnect(connection_handle); /* disconnect from driver */
    }
    else
    {
        fprintf(stderr, "Failed to connect\n");
        extract_error("SQLDriverConnect", connection_handle, SQL_HANDLE_DBC);
    }
    /* free up allocated handles */
    SQLFreeHandle(SQL_HANDLE_DBC, connection_handle);
    SQLFreeHandle(SQL_HANDLE_ENV, environment_handle);
    /*
    output example:
    Connected
    Returned connection string was:
            DSN=fred;SERVERPORT=server:8888;TARGETDSN=test;UID=dbuser;PWD=dbpass;
                    LOGONUSER=fred;LOGONAUTH=mypass;
    Driver reported the following diagnostics

    The driver reported the following diagnostics whilst running SQLDriverConnect

    01000:1:5703:[unixODBC][NetConn: 141dbe18][Microsoft][ODBC SQL Server Driver][SQL Server]
            Changed language setting to us_english.
    01000:2:5701:[unixODBC][NetConn: 141dbe18][Microsoft][ODBC SQL Server Driver][SQL Server]
            Changed database context to 'easysoft_fred'.
    */
}