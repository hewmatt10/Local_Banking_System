#include<iostream>
#include<string>
#include<mysql.h>

void displayUserOptions() {
    std :: cout << "--- Banking System Demonstration ---\n" 
                << "----- Please select an option: -----\n \n"
                << "----------- 1. Balance -------------\n"
                << "----------- 2. Deposit -------------\n"
                << "----------- 3. Withdraw ------------\n"
                << "----------- 4. Transfer ------------\n"
                << "----------- 5. Logout --------------\n";
}

void displayLoginOptions() {
    std :: cout << "--- Banking System Demonstration ---\n" 
                << "----- Please select an option: -----\n \n"
                << "----------- 1. Log In --------------\n"
                << "----------- 2. Register ------------\n"
                << "----------- 3. Exit ----------------\n";
}

void displayBalance (std :: string cardNumber) {
    MYSQL *con = getConnection(); 
    MYSQL_RES *res; 
    MYSQL_ROW row;  
    res = mysql_perform_query(con, "select balance from bankaccounts where card_number=" + cardNumber + ";");
    row = mysql_fetch_row(res);
    std :: string currentBalance = row[0];
    std :: cout << "Balance: " << currentBalance << "\n";
    mysql_free_result(res);
    mysql_close(con);
}

std :: string displayCardNumber(int& number) {
    std :: string displayedNumber = std :: to_string(number);
    while(displayedNumber.length() < 8) displayedNumber = "0" + displayedNumber;
    return displayedNumber;
}