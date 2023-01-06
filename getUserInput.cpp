#include<string>

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