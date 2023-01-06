#include<string>
#include<limits>
#include<mysql.h>

int checkNumber(std :: string num) {
    for(int i = 0; i < 8; i++) {
        if (num[i] > 57 || num[i] < 48) return -1;
    }
    return stoi(num);
}

double checkDouble(std :: string tempDouble) {
    int n = tempDouble.length();
    for(int i = 0; i < n; i++) {
        if(tempDouble[i] < 48 || tempDouble[i] > 57) {
            if (tempDouble[i] != '.') return std::numeric_limits<double>::quiet_NaN();
            if (n - i > 3) return std::numeric_limits<double>::quiet_NaN();
        }
    }
    return stod(tempDouble);
}

std :: string checkHash(std :: string str) { 
    std :: string fixedString = "";
    int n = str.length();
    for(int i = 0; i < n; i++) {
        if(str[i] == 39 || str[i] == 34 || str[i] == 63 || str[i] == 92) fixedString += "\\";
        fixedString += str[i];
    }
    return fixedString;
}

int checkBankingNumber(std :: string tempCardNumber){
    if (tempCardNumber.length() != 8) return -1;
    int tempNum = checkNumber(tempCardNumber);
    if (tempNum == -1) return -1;
    MYSQL *con = getConnection(); 
    MYSQL_RES *res; 
    MYSQL_ROW row;  

    res = mysql_perform_query(con, "select * from bankaccounts where card_number=" + tempCardNumber + ";");
    if (res == NULL) {
        mysql_free_result(res);
        mysql_close(con);
        return -1;
    }
    return tempNum;
}