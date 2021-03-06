#include <iostream>


#include <mysql_driver.h>
#include <mysql_connection.h>
#include <cppconn/statement.h>
#include <cppconn/prepared_statement.h>
#include <cppconn/metadata.h>
#include "DBConnectionPool.h"

#define NUMOFFSET 100
#define COLNAME 200


using namespace std;
using namespace sql;

void retrieve_data_and_print(ResultSet *rs, int type, int colidx, string colname) {

    /* retrieve the row count in the result set */
    cout << "\nRetrieved " << rs->rowsCount() << " row(s)." << endl;

    cout << "\nUser_Name" << endl;
    cout << "--------" << endl;

    /* fetch the data : retrieve all the rows in the result set */
    while (rs->next()) {
        if (type == NUMOFFSET) {
            cout << rs->getString(colidx) << endl;
        } else if (type == COLNAME) {
            cout << rs->getString(colname) << endl;
        } // if-else
    } // while

    cout << endl;

} // retrieve_data_and_print()

void retrieve_dbmetadata_and_print(Connection *dbcon) {

    if (dbcon->isClosed()) {
        throw runtime_error("DatabaseMetaData FAILURE - database connection closed");
    }

    cout << "\nDatabase Metadata" << endl;
    cout << "-----------------" << endl;

    cout << boolalpha;

    /* The following commented statement won't work with Connector/C++ 1.0.5 and later */
    //auto_ptr < DatabaseMetaData > dbcon_meta (dbcon -> getMetaData());

    DatabaseMetaData *dbcon_meta = dbcon->getMetaData();

    cout << "Database Product Name: " << dbcon_meta << endl;
    cout << "Database Product Version: " << dbcon_meta->getDatabaseProductVersion() << endl;
    cout << "Database User Name: " << dbcon_meta->getUserName() << endl << endl;

    cout << "Driver name: " << dbcon_meta->getDriverName() << endl;
    cout << "Driver version: " << dbcon_meta->getDriverVersion() << endl << endl;

    cout << "Database in Read-Only Mode?: " << dbcon_meta->isReadOnly() << endl;
    cout << "Supports Transactions?: " << dbcon_meta->supportsTransactions() << endl;
    cout << "Supports DML Transactions only?: " << dbcon_meta->supportsDataManipulationTransactionsOnly() << endl;
    cout << "Supports Batch Updates?: " << dbcon_meta->supportsBatchUpdates() << endl;
    cout << "Supports Outer Joins?: " << dbcon_meta->supportsOuterJoins() << endl;
    cout << "Supports Multiple Transactions?: " << dbcon_meta->supportsMultipleTransactions() << endl;
    cout << "Supports Named Parameters?: " << dbcon_meta->supportsNamedParameters() << endl;
    cout << "Supports Statement Pooling?: " << dbcon_meta->supportsStatementPooling() << endl;
    cout << "Supports Stored Procedures?: " << dbcon_meta->supportsStoredProcedures() << endl;
    cout << "Supports Union?: " << dbcon_meta->supportsUnion() << endl << endl;

    cout << "Maximum Connections: " << dbcon_meta->getMaxConnections() << endl;
    cout << "Maximum Columns per Table: " << dbcon_meta->getMaxColumnsInTable() << endl;
    cout << "Maximum Columns per Index: " << dbcon_meta->getMaxColumnsInIndex() << endl;
    cout << "Maximum Row Size per Table: " << dbcon_meta->getMaxRowSize() << " bytes" << endl;

    cout << "\nDatabase schemas: " << endl;

    auto_ptr<ResultSet> rs(dbcon_meta->getSchemas());

    cout << "\nTotal number of schemas = " << rs->rowsCount() << endl;
    cout << endl;

    int row = 1;

    while (rs->next()) {
        cout << "\t" << row << ". " << rs->getString("TABLE_SCHEM") << endl;
        ++row;
    } // while

    cout << endl << endl;

} // retrieve_dbmetadata_and_print()

void retrieve_rsmetadata_and_print(ResultSet *rs) {

    if (rs->rowsCount() == 0) {
        throw runtime_error("ResultSetMetaData FAILURE - no records in the result set");
    }

    cout << "ResultSet Metadata" << endl;
    cout << "------------------" << endl;

    /* The following commented statement won't work with Connector/C++ 1.0.5 and later */
    //auto_ptr < ResultSetMetaData > res_meta ( rs -> getMetaData() );

    ResultSetMetaData *res_meta = rs->getMetaData();

    int numcols = res_meta->getColumnCount();
    cout << "\nNumber of columns in the result set = " << numcols << endl << endl;

    cout.width(20);
    cout << "Column Name/Label";
    cout.width(20);
    cout << "Column Type";
    cout.width(20);
    cout << "Column Size" << endl;

    for (int i = 0; i < numcols; ++i) {
        cout.width(20);
        cout << res_meta->getColumnLabel(i + 1);
        cout.width(20);
        cout << res_meta->getColumnTypeName(i + 1);
        cout.width(20);
        cout << res_meta->getColumnDisplaySize(i + 1) << endl << endl;
    }

    cout << "\nColumn \"" << res_meta->getColumnLabel(1);
    cout << "\" belongs to the Table: \"" << res_meta->getTableName(1);
    cout << "\" which belongs to the Schema: \"" << res_meta->getSchemaName(1) << "\"" << endl << endl;

} // retrieve_rsmetadata_and_print()

void testMysqlConnect() {
    DBConnectionPool *pPool = DBConnectionPool::GetInstance();
    Connection *pConnection = pPool->GetConnection();
    Statement *stmt;
    ResultSet *res;
    pConnection->setAutoCommit(0);
    cout << "\nDatabase connection\'s autocommit mode = " << pConnection->getAutoCommit() << endl;
    pConnection->setSchema("mobilework");
    retrieve_dbmetadata_and_print(pConnection);

    stmt = pConnection->createStatement();
    cout << "Executing the Query: \"SELECT * FROM sys_user\" .." << endl;

    res = stmt->executeQuery("SELECT * FROM sys_user");
    cout << "Retrieving the result set .." << endl;

    retrieve_data_and_print(res, NUMOFFSET, 5, string("USER_NAME_"));

    retrieve_rsmetadata_and_print(res);

    cout << "Demonstrating Prepared Statements .. " << endl << endl;

    pPool->ReleaseConnection(pConnection);
} // mysql connect

int main() {


//    testMysqlConnect();
    char header[1024];

    printf("------- %zu\n", sizeof(header));


    char *a = new char[1024]();

    printf("------- %zu", sizeof(*a));




    return 0;
}