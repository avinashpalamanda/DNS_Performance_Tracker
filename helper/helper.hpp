// Copyright [2017] <Avinash Palamanda>
// The File implements the member functions and their helper functions of class HelperFB class, which sets up connection with DB
// and provides access to tables DOMAINS and STATISTICS
#ifndef DNS_TRACKER_CODE_FINAL_HELPER_HELPER_HPP_
#define DNS_TRACKER_CODE_FINAL_HELPER_HELPER_HPP_

#include <mysql++.h>  // SQL++ Library
#include <iostream>
#include <string>
#include <sstream>
#include <result.h>
#include <getopt.h>
#include <map>
#include <cmath>  // For arithemtic operations

// Class HelperDB provides us interface with of the SQL server and Tables.
// Inserting new domain into DOMAINS  table
// Deleting Domain if it not required
// Showing rank of the table.
class HelperDB {
 private:
  const char* DB_name_;
  const char* server_name_;
  const char* user_name_;
  const char* password_;

  mysqlpp::Connection db_conn_;
 public:
  HelperDB(const char* Db, const char* Server, const char* User, const char* Pwd);
  void InsertDomain(std::string domain);  // Used insert new Domain
  void DeleteDomain(std::string domain);  // Used delete new Domain
  std::string ShowRank();  // Function to display the rank of domains.
};
#endif  // DNS_TRACKER_CODE_FINAL_HELPER_HELPER_HPP_
