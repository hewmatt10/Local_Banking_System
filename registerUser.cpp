#include<mysql.h>

void registerUser(std :: string cardNumber, std :: string userName, std :: string hashedPassword, std :: string hash, std :: string userEmail, std :: string message) {
    MYSQL *con = getConnection(); 
    MYSQL_RES *res; 
    MYSQL_ROW row;  

    res = mysql_perform_query(con, "insert into bankaccounts (card_number, name, password, hash, email, balance) values ("
        + cardNumber + ", "
        + "\'" + userName + "\'" + ", "
        + "\'" + hashedPassword + "\'" + ", "
        + "\'" + hash + "\'" + ", "
        + "\'" + userEmail + "\'" + ", "
        + "0);");
    mysql_free_result(res);
    mysql_close(con);
    std :: cout << message;
}