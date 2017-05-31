// Copyright [2017] <Avinash Palamanda>
// The File implements the member functions and their helper functions of class HelperFB class, which sets up connection with DB
// and provides access to tables DOMAINS and STATISTICS

#include "helper.hpp"
#include <iostream>
#include <getopt.h>

// Function to show the usage of the tool
void Usage(char *s) {
  system("clear");
  std::cout << "Usage:   " << s << " [-option] [argument]" << std::endl;
  std::cout << "option:  " << "-d DB Name" << std::endl;
  std::cout << "         " << "-s SQL Server Name" << std::endl;
  std::cout << "         " << "-u SQL User Name" << std::endl;
  std::cout << "         " << "-k SQL Password Name" << std::endl;
  std::cout << "example: " << s << "-d latency_test -s localhost -u root -k password" << std::endl;
}

int main(int argc, char *argv[]) {
  char* DB_name;
  char* server_name;
  char* user_name;
  char* Pwd;

  int opt = 0;  // To get options from user
  int check = 0;  // To check whether we got required parameters
  system("clear");
  while ((opt = getopt(argc, argv, "d:s:u:k:")) != -1) {
    switch (opt) {
      case 'd':  // Get DB name
        check++;
        DB_name = (optarg);
        break;
      case 's':  // Get SQL server name
        check++;
        server_name = (optarg);
        break;
      case 'u':  // Get User name
        check++;
        user_name = (optarg);
        break;
      case 'k':  // Get SQL Password
        check++;
        Pwd = (optarg);
        break;
      default :  // If the tool queried is wrong throw error
        Usage(argv[0]);
        return 0;
        break;
    }
  }

  if ( check == 4 ) {  // Check if we have all required data then run the querying
  HelperDB helper(DB_name, server_name, user_name, Pwd);
  system("clear");
  std::cout << "1)Insert - i Domain-Name\n2)Delete - d Domain-Name\n3)Show Rank - s \n4)Exit - e\n";
  while (1) {  // Runa infinite while loop
    char choice;  // Choice of user to delete,insert a domain.
    std::string domain_name;
    std::cin >> choice;
    switch (choice) {
      case 'i':  // Insert a Domain
        std::cin >> domain_name;
        helper.InsertDomain(domain_name);
        std::cout << "Domain Added\n";
        break;
      case 'd':  // Delete a Domain
        std::cin >> domain_name;
        helper.DeleteDomain(domain_name);
        std::cout << "Domain Deleted\n";
        break;
      case 's':  // Delete a Domain
        std::cout << helper.ShowRank();
        break;
      case 'e':  // Exit from Program
        system("clear");
        return 0;
      default:
        system("clear");
        std::cout << "1)Insert - i Domain-Name\n2)Delete - d Domain-Name\n3)Show Rank - s \n4)Exit - e\n";
      }
    }
  }
  else
    Usage(argv[0]);
}
