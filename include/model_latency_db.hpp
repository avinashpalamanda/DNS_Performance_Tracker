// Copyright [2017] <Avinash Palamanda>
// The File acts as a header file for model_latency_db.cpp and defines class which helps in setting,
// connections with DB and tables(DOMAINS,STATISTICS).Class provides fucntions to query,insert and update those tables.
#ifndef DNS_TRACKER_CODE_FINAL_INCLUDE_MODEL_LATENCY_DB_HPP_
#define DNS_TRACKER_CODE_FINAL_INCLUDE_MODEL_LATENCY_DB_HPP_

#include <mysql++.h>  // SQL++ Library
#include <iostream>
#include <string>
#include <sstream>
#include <result.h>
#include <map>
#include <cmath>  // For arithemtic operations

// Class DnsDB provides us interface with of the SQL server and Tables.
// Once the instance of class is created the required tables STATISTICS and DOMAINS are created in case if they dont exists.
//  1)DOMAINS - Domain_id,Domain_name.
//  2)STATISTICS - Domain_id,average latency, latency SD,Min latency time,max latency time,First query time and Last query time.
// Member Functions :
//  1)GetDomains_() - Returns map of domain_id and domain name from the DOMAINS table.
//  2)DnsLatencyDBUpdate_ - Updates the STATISTICS table with new latency avg and Standard Deviation.
class DnsDB {
 private:
  const char* DB_name_;
  const char* server_name_;
  const char* user_name_;
  const char* password_;

  mysqlpp::Connection db_conn_;
 public:
  DnsDB(const char* Db, const char* Server, const char* User, const char* Pwd);
  std::map<int, std::string> GetDomains_();
  void DnsLatencyDBUpdate_(int latency, time_t current_ts, int dns_id);
  std::string ShowRank();  // Function to display the rank of domains.
};
#endif  // DNS_TRACKER_CODE_FINAL_INCLUDE_MODEL_LATENCY_DB_HPP_
