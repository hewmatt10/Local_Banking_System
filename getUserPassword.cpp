#include<mysql.h>
#include<string>
#include<iostream>

std :: string getHashedPassword (std :: string currentPw, std :: string hash) {
    int length = currentPw.length();
    std :: string hashedPw = "";
    for(int i = 0; i < length; i++) {
        hashedPw += (char)(33 + (currentPw[i] + hash[i]) % 94);
    }
    return hashedPw;
}

std :: string getUserRegisteredPassword() {
    std :: string password;
    while(true) {
        std :: string tempPassword = "";
        while(true) {
            std :: string pw = getUserString("Please enter your password: ");
            if (pw.length() > 8) {tempPassword = pw; break;}
            std :: cout << "Password too short. \n";
        }
        std :: string confirmedpw = getUserString("Please re-enter your password: ");
        if (tempPassword == confirmedpw) {password = tempPassword; break;}
        std :: cout << "Passwords do not match. \n";
    }
    return password;
}

bool getUserLoginPassword (std :: string bankingNumber) {
    MYSQL *con = getConnection(); 
    MYSQL_RES *res; 
    MYSQL_ROW row;  
    res = mysql_perform_query(con, "select name, hash, password from bankaccounts where card_number=" + bankingNumber + ";");
    
    if (res == NULL) {
        mysql_free_result(res);
        std :: cout << "Invalid number. \n";
        return false;
    }

    row = mysql_fetch_row(res);

    std :: string tempPassword = getUserString("Please enter your password: ");
    std :: string hash = row[1];
    std :: string realPassword = row[2];

    mysql_free_result(res);
    mysql_close(con);

    if (getHashedPassword(tempPassword, hash) != realPassword) {
        std :: cout << "Incorrect Password.\n";
        return false;
    }
    std :: cout << "Welcome " << row[0] << "\n";
    return true;

}