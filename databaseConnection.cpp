#include<mysql.h>

struct connection_details {
	const char *server, *user, *password, *database;
};

MYSQL* mysql_connection_setup(struct connection_details mysql_details){
    MYSQL *connection = mysql_init(NULL); 
    if(!mysql_real_connect(connection, mysql_details.server, mysql_details.user, mysql_details.password, mysql_details.database, 0, NULL, 0)){
        std::cout << "Connection Error: " << mysql_error(connection) << std::endl;
        exit(1); 
    }

    return connection;
}

MYSQL* getConnection () {
    std :: ifstream connectionInfo; connectionInfo.open("connection_info.txt");

    struct connection_details mysqlD;
    if(connectionInfo.is_open()) {
        std :: cout << "IN!\n";
        std :: string svr, usr, pw, db;
        std :: getline(connectionInfo, svr);
        mysqlD.server = svr.c_str(); 
        std :: getline(connectionInfo, usr);
        mysqlD.user = usr.c_str(); 
        std :: getline(connectionInfo, pw);
        mysqlD.password = pw.c_str(); 
        std :: getline(connectionInfo, db);
        mysqlD.database = db.c_str(); 
    }
    return mysql_connection_setup(mysqlD);    
}

MYSQL_RES* mysql_perform_query(MYSQL *connection, const char *sql_query){
    if(mysql_query(connection, sql_query)){
        std::cout << "MySQL Query Error: " << mysql_error(connection) << std::endl;
        exit(1);
    }

    return mysql_use_result(connection);
}

MYSQL_RES* mysql_perform_query(MYSQL *connection, std :: string query){
    if(mysql_query(connection, query.c_str())){
        std::cout << "MySQL Query Error: " << mysql_error(connection) << std::endl;
        exit(1);
    }

    return mysql_use_result(connection);
}

