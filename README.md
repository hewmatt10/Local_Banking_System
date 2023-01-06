# Local Banking System
---
## Setup Instructions

1. Download MySQL
2. Create a new user in MySQL
3. Create a new database
4. Create a new table called `bankaccounts` using the following command: `CREATE TABLE(id int AUTO_INCREMENT PRIMARY KEY, card_number int, name varchar(255), password varchar(255), hash varchar(255), email varchar(255), balance double(32, 2))`
5. Create a file called `connection_info.txt` in the same directory as `main.cpp`
6. Write your host, MySQL username, MySQL password, and database name in that order on `4` separate lines in `connection_info.txt`
7. Save and recompile `main.cpp` with the command: `g++ -o main $(mysql_config --cflags) main.cpp $(mysql_config --libs)`
