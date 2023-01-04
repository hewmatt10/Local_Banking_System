#include<iostream>
#include<fstream>
#include<mysql.h>
#include<string>
#include<stack>
#include<cstdlib>
#include<cmath>
#include<limits>
#include "databaseConnection.cpp"

int validate_number(std :: string& num) {
    for(int i = 0; i < 8; i++) {
        if (num[i] > 57 || num[i] < 48) return -1;
    }
    return stoi(num);
}

int generateCardNumber() {
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

std :: string getHashedPassword (std :: string currentPw, std :: string hash) {
    int length = currentPw.length();
    std :: string hashedPw = "";
    for(int i = 0; i < length; i++) {
        hashedPw += (char)(33 + (currentPw[i] + hash[i]) % 94);
    }
    return hashedPw;
}

std :: string displayCardNumber(int& number) {
    std :: string displayedNumber = std :: to_string(number);
    while(displayedNumber.length() < 8) displayedNumber = "0" + displayedNumber;
    return displayedNumber;
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

char getOption(int limit) {
    char op;
    do {
        char c;
        while((c=getchar()) != 10) op = c;
        if(!(op >= '1' && op <= ('1' + limit - 1))) std :: cout << "Please enter a valid option\n";
    } while (!(op >= '1' && op <= ('1' + limit - 1)));
    return op;
}


std :: string getUserString() {
    std :: string userString = "";
    char c;
    while((c=getchar()) != 10) {
        userString += c;
    }
    return userString;
}

std :: string getUserString(std :: string statement) {
    std :: cout << statement;
    std :: string userString = "";
    char c;
    while((c=getchar()) != 10) {
        userString += c;
    }
    return userString;
}

int main (int argc, char const *argv[]) {    
	MYSQL *con;	
    MYSQL_RES *res; 
    MYSQL_ROW row;	

    con = getConnection();

    bool loggedIn = false;
    int cardNumber = -1;
    while (true) {
        if(loggedIn) {
            displayUserOptions();
            char option = getOption(5);

            switch(option) {
            case '1':{
                res = mysql_perform_query(con, "select balance from bankaccounts where card_number=" + std :: to_string(cardNumber) + ";");
                row = mysql_fetch_row(res);
                std :: string currentBalance = row[0];
                std :: cout << "Balance: " << currentBalance << "\n";
                mysql_free_result(res);
                break;
            }

            case '2': {
                std :: string tempAmount = getUserString("How much are you depositing: ");
                double amount = checkDouble(tempAmount);

                if(isnan(amount)) {
                    std :: cout << "Invalid amount. \n";
                    break;
                }

                res = mysql_perform_query(con, "select balance from bankaccounts where card_number=" + std :: to_string(cardNumber) + ";");
                row = mysql_fetch_row(res);
                std :: string currentBalance = row[0];
                double newBalance = stod(currentBalance) + amount;
                mysql_free_result(res);

                res = mysql_perform_query(con, "update bankaccounts set balance=" + std :: to_string(newBalance) + " where card_number=" + std :: to_string(cardNumber) + ";");
                std :: cout << "Deposit successful. \n";
                mysql_free_result(res);
                break;
            }

            case '3':{
                std :: string tempAmount = getUserString("How much are you withdrawing: ");
                
                double amount = checkDouble(tempAmount);

                if(isnan(amount)) {
                    std :: cout << "Invalid amount. \n";
                    break;
                }

                res = mysql_perform_query(con, "select balance from bankaccounts where card_number=" + std :: to_string(cardNumber) + ";");
                row = mysql_fetch_row(res);
                std :: string currentBalance = row[0];
                double newBalance = stod(currentBalance) - amount;
                if(newBalance < 0.0) {
                    std :: cout << "Not enough balance. \n";
                    break;
                }
                mysql_free_result(res);

                res = mysql_perform_query(con, "update bankaccounts set balance=" + std :: to_string(newBalance) + " where card_number=" + std :: to_string(cardNumber) + ";");
                std :: cout << "Withdraw successful. \n";
                mysql_free_result(res);
                break;
            }
            case '4':{
                std :: string tempTransferAccount = getUserString("Enter the account number you are transfering to: ");
                //verify transfer account 

                if (tempTransferAccount.length() != 8) {
                    std :: cout << "Invalid number. \n";
                    break;
                }
                int intTempTransferAccount = validate_number(tempTransferAccount);
                if(intTempTransferAccount == -1) {
                    std :: cout << "Invalid number. \n";
                    break;
                }

                res = mysql_perform_query(con, "select balance from bankaccounts where card_number=" + std :: to_string(intTempTransferAccount) + ";");
                
                if (res == NULL) {
                    mysql_free_result(res);
                    std :: cout << "Invalid number. \n";
                    break;
                }

                row = mysql_fetch_row(res);

                std :: string tempAmount = getUserString("How much are you transferring: ");
                double amount = checkDouble(tempAmount);

                if(isnan(amount)) {
                    std :: cout << "Invalid amount. \n";
                    break;
                }

                mysql_free_result(res);

                res = mysql_perform_query(con, "select balance from bankaccounts where card_number=" + std :: to_string(cardNumber) + ";");
                row = mysql_fetch_row(res);
                std :: string currentBalance = row[0];
                double newBalance = stod(currentBalance) - amount;
                if(newBalance < 0.0) {
                    std :: cout << "Not enough balance. \n";
                    break;
                }
                mysql_free_result(res);

                res = mysql_perform_query(con, "update bankaccounts set balance=" + std :: to_string(newBalance) + " where card_number=" + std :: to_string(cardNumber) + ";");
                
                mysql_free_result(res);

                res = mysql_perform_query(con, "select balance from bankaccounts where card_number=" + std :: to_string(intTempTransferAccount) + ";");
                row = mysql_fetch_row(res);
                currentBalance = row[0];
                newBalance = stod(currentBalance) + amount;
                mysql_free_result(res);

                res = mysql_perform_query(con, "update bankaccounts set balance=" + std :: to_string(newBalance) + " where card_number=" + std :: to_string(intTempTransferAccount) + ";");
                std :: cout << "Transfer successful. \n";
                mysql_free_result(res);
                break;
            }
            case '5':
                loggedIn = false;
                cardNumber = -1;
                std :: cout << "Logged out successfully.\n";
                break;
            }
        }
        else {
            displayLoginOptions();
            char option = getOption(3);

            switch(option) {
            case '1': {
                std :: cout << "Please enter your card number: ";
                std :: string tempCardNumber; char c;
                while((c=getchar()) != 10) {
                    tempCardNumber += c;
                }
                if (tempCardNumber.length() != 8) {
                    std :: cout << "Invalid number. \n";
                    break;
                }
                int banking_number = validate_number(tempCardNumber);
                if(banking_number == -1) {
                    std :: cout << "Invalid number. \n";
                    break;
                }

                res = mysql_perform_query(con, "select name, hash, password from bankaccounts where card_number=" + std :: to_string(banking_number) + ";");
                
                if (res == NULL) {
                    mysql_free_result(res);
                    std :: cout << "Invalid number. \n";
                    break;
                }

                row = mysql_fetch_row(res);

                std :: cout << "Please enter your password: ";
                std :: string tempPassword = "";
                while((c=getchar()) != 10) {
                    tempPassword += c;
                }
                std :: string hash = row[1];
                std :: string realPassword = row[2];
                if (getHashedPassword(tempPassword, hash) != realPassword) {
                    std :: cout << "Incorrect Password.\n";
                }
                else {
                    loggedIn = true;
                    cardNumber = banking_number;
                    std :: cout << "Welcome " << row[0] << "\n";
                }

                mysql_free_result(res);
                
                break;
            }

            case '2': {
                int tempCardNumber;
                bool collision = false;
                do { 
                    tempCardNumber = generateCardNumber(); 
                    res = mysql_perform_query(con, "select card_number from bankaccounts;");
                    while((row=mysql_fetch_row(res)) != NULL) {
                        if (atoi(row[0]) == tempCardNumber) {
                            collision = true;
                            break;
                        }
                    }
                } while (collision == true);

                std :: cout << "Your assigned 8 digit card number is: " + displayCardNumber(cardNumber=tempCardNumber) + "\n";
                std :: string password = "";
                while(true) {
                    std :: string tempPassword = "";
                    while(true) {
                        std :: cout << "Please enter your password: ";
                        std :: string pw = ""; char c;
                        while((c=getchar()) != 10) {
                            pw += c;
                        }
                        if (pw.length() > 8) {tempPassword = pw; break;}
                        std :: cout << "Password too short. \n";
                    }
                    std :: cout << "Please re-enter your password: ";
                    std :: string confirmedpw = ""; char c;
                    while((c=getchar()) != 10) {
                        confirmedpw += c;
                    }
                    if (tempPassword == confirmedpw) {password = tempPassword; break;}
                    std :: cout << "Passwords do not match. \n";
                }

                std :: string hash = generateHash (password.length());
                std :: string hashedPassword = getHashedPassword (password, hash);

                std :: string userName = getUserString("Please enter your full name: ");
                std :: string userEmail = getUserString("Please enter your email: ");

                res = mysql_perform_query(con, "insert into bankaccounts (card_number, name, password, hash, email, balance) values ("
                    + std :: to_string(cardNumber) + ", "
                    + "\'" + userName + "\'" + ", "
                    + "\'" + hashedPassword + "\'" + ", "
                    + "\'" + hash + "\'" + ", "
                    + "\'" + userEmail + "\'" + ", "
                    + "0);");

                std :: cout << "Registration Successful.\n";
                loggedIn = true;

                mysql_free_result(res);

                break;
            }

            case '3':{
                std :: cout << "Exited successfully.\n";
                goto exitSequence;
                break;
            }  
            }
         }

    }
    
exitSequence:    
    mysql_close(con);
	return 0;
}