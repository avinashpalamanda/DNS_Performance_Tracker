// Copyright [2017] <Avinash Palamanda>
// The File is the main function which passes the corresponding paramters and passes it to the controller part.

#include"../include/main.hpp"

// Function to show the usage of the tool
void Usage(char *s) {
  system("clear");
  std::cout << "Usage:   " << s << " [-option] [argument]" << std::endl;
  std::cout << "option:  " << "-f Period to wait before Querying " << std::endl;
  std::cout << "         " << "-c Number of cycles to query" << std::endl;
  std::cout << "         " << "-p Concurrent Querying" << std::endl;
  std::cout << "         " << "-d DB Name" << std::endl;
  std::cout << "         " << "-s SQL Server Name" << std::endl;
  std::cout << "         " << "-u SQL User Name" << std::endl;
  std::cout << "         " << "-k SQL Password Name" << std::endl;
  std::cout << "example: " << s << " -f 1 -c 2 -p 0 -d latency_test -s localhost -u root -k password" << std::endl;
}

int main(int argc, char *argv[]) {
  int period = 0;  // Period before querying next domain
  int cycle = 0;  // Number of times domain to be queried
  int concurrent = 0;  // Should the tool query threaded/concurrently
  char* DB_name;
  char* server_name;
  char* user_name;
  char* Pwd;

  int opt = 0;  // To get the options from getopt
  int check = 0;  // to check whether we have recieved DB parameters
  system("clear");
  while ((opt = getopt(argc, argv, "f:c:p:d:s:u:k:")) != -1) {  // Get the Options -f,-c,-p,-d,-s,-k,-u
    switch (opt) {
      case 'f':  // Get Period value
        period = atoi(optarg);
        break;
      case 'c':  // Get Number of Cycles
        cycle = atoi(optarg);
        break;
      case 'p':  // Get whether the code should concurrently
        concurrent = atoi(optarg);
        break;
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
      default :  // If the tool queried is wrong throw usage function
        Usage(argv[0]);
        return 0;
        break;
    }
  }

  if ( check == 4 ) {  // Check if we have all required data if so, then run the querying process
  ControllerQuery query(period, cycle, DB_name, server_name, user_name, Pwd);
  if ( concurrent == 0 )  // If user wants to run query in serial way.
    query.QuerySerial_();
  else  // Concurrent Running
    query.QueryConcurrent_();
  query.displays();
  }
  else
    Usage(argv[0]);  // Didnt recieve all required parameters
}
