#include<iostream>
#include<fstream>
#include<mysql.h>
#include<string>
#include<cstdlib>
#include<cmath>
#include "databaseConnection.cpp"
#include "getUserInput.cpp"
#include "displayOptions.cpp"
#include "registerUser.cpp"
#include "generatorFunctions.cpp"
#include "validationFunctions.cpp"
#include "getUserPassword.cpp"
#include "accountFunctions.cpp"


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
                displayBalance(std :: to_string(cardNumber));
                break;
            }

            case '2': {
                double amount = checkDouble(getUserString("How much are you depositing: "));
                if(isnan(amount)) {
                    std :: cout << "Invalid amount. \n";
                    break;
                }
                depositMoney(std :: to_string(cardNumber), amount);    
                std :: cout << "Deposit successful. \n";
                break;
            }

            case '3':{
                double amount = checkDouble(getUserString("How much are you withdrawing: "));
                if(isnan(amount)) {
                    std :: cout << "Invalid amount. \n";
                    break;
                }
                if(withdrawMoney(std :: to_string(cardNumber), amount)) std :: cout << "Withdraw successful. \n";
                else std :: cout << "Withdraw unsuccessful, not enough balance. \n";
                break;
            }
            case '4':{
                int transferAccount = checkBankingNumber(getUserString("Enter the account number you are transfering to: "));
                
                if(transferAccount == -1) {
                    std :: cout << "Invalid account number. \n";
                    break;
                }

                double amount = checkDouble(getUserString("How much are you transferring: "));
                if(isnan(amount)) {
                    std :: cout << "Invalid amount. \n";
                    break;
                }
                if(transferMoney(std :: to_string(cardNumber), std :: to_string(transferAccount), amount)) std :: cout << "Transfer successful. \n";
                else std :: cout << "Transfer unsuccessful, not enough balance. \n";
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
                int banking_number = checkBankingNumber(getUserString("Please enter your card number: "));

                if(banking_number == -1) {
                    std :: cout << "Invalid account number. \n";
                    break;
                }

                loggedIn = getUserLoginPassword(std :: to_string(banking_number));
                if(loggedIn) cardNumber = banking_number;
                
                break;
            }

            case '2': {
                std :: cout << "Your assigned 8 digit card number is: " + displayCardNumber(cardNumber=generateCardNumber()) + "\n";
                std :: string password = getUserRegisteredPassword();

                std :: string hash = generateHash(password.length());
                std :: string hashedPassword = getHashedPassword (password, hash);

                registerUser(std :: to_string(cardNumber), getUserString("Please enter your full name: "), checkHash(hashedPassword), checkHash(hash), getUserString("Please enter your email: "), "Registration Successful.\n");

                loggedIn = true;

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