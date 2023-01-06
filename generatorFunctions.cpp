#include<cstdlib>
#include<mysql.h>

int generateCardNumberHelper() {
    srand((unsigned) time(NULL));
    return (rand() % 100000000);
}

std :: string generateHash(int length) {
    srand((unsigned) time(NULL));
    std :: string hash = "";
    for(int i = 0; i < length; i++) {
        hash += (char)(33 + (rand() % 94));
    }
    return hash;
}

int generateCardNumber() {
    MYSQL *con = getConnection(); 
    MYSQL_RES *res; 
    MYSQL_ROW row;  
    int tempCardNumber;
    bool collision = false;
    do { 
        tempCardNumber = generateCardNumberHelper(); 
        res = mysql_perform_query(con, "select card_number from bankaccounts;");
        while((row=mysql_fetch_row(res)) != NULL) {
            if (atoi(row[0]) == tempCardNumber) {
                collision = true;
                break;
            }
        }
        mysql_free_result(res);
    } while (collision == true);
    mysql_close(con);
    return tempCardNumber;
}