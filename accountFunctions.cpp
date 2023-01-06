#include<mysql.h>
#include<string>

void depositMoney (std :: string cardNumber, double amount) {
    MYSQL *con = getConnection(); 
    MYSQL_RES *res; 
    MYSQL_ROW row;  

    res = mysql_perform_query(con, "select balance from bankaccounts where card_number=" + cardNumber + ";");
    row = mysql_fetch_row(res);
    std :: string currentBalance = row[0];
    double newBalance = stod(currentBalance) + amount;
    mysql_free_result(res);

    res = mysql_perform_query(con, "update bankaccounts set balance=" + std :: to_string(newBalance) + " where card_number=" + cardNumber + ";");
    mysql_free_result(res);
    mysql_close(con);
}

bool withdrawMoney (std :: string cardNumber, double amount) {
    MYSQL *con = getConnection(); 
    MYSQL_RES *res; 
    MYSQL_ROW row;  

    res = mysql_perform_query(con, "select balance from bankaccounts where card_number=" + cardNumber + ";");
    row = mysql_fetch_row(res);
    std :: string currentBalance = row[0];
    double newBalance = stod(currentBalance) - amount;
    if(newBalance < 0.0) return false;
    mysql_free_result(res);

    res = mysql_perform_query(con, "update bankaccounts set balance=" + std :: to_string(newBalance) + " where card_number=" + cardNumber + ";");
    
    mysql_free_result(res);
    mysql_close(con);
    return true;
}

bool transferMoney (std :: string cardNumber, std :: string transferAccount, double amount) {

    if(withdrawMoney(cardNumber, amount)) {
        depositMoney(transferAccount, amount);
        return true;
    }
    return false;
}
