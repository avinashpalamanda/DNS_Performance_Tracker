// Copyright [2017] <Avinash Palamanda>
// The File has basic class called ControllerQuery which contains various member to access the DNSDB and DNSResolver classes.
#ifndef DNS_TRACKER_CODE_FINAL_INCLUDE_CONTROLLER_QUERY_HPP_
#define DNS_TRACKER_CODE_FINAL_INCLUDE_CONTROLLER_QUERY_HPP_

#include<iostream>
#include<string>
#include<sstream>
#include<map>
#include<thread>
#include<mutex>
#include<iterator>

#include "controller_DNS_resolver.hpp"  // To get access of DNSResolver class
#include"model_latency_db.hpp"  // To get access of DNSDB class

// The class consist of basic data members which is fed to other DNSDB and DNSResolver class constructors.
// DB_name, Server Name, Password and User Name to access DB.DB_handler_ to create instance of DnsDB.
// Map<int,string> to store domains and domain id.
// Member function to query domains serially and concurrently.
class ControllerQuery {
 private:
    int query_period_;
    int num_cycle_;
    const char* DB_name_;
    const char* server_name_;
    const char* user_name_;
    const char* password_;
    std::map<int, std::string> domain_name_list_;
 public:
    std::mutex DB_lock_;
    DnsDB DB_handler_;
    ControllerQuery(int query_period_input, int num_cycle_input, const char* DB_name, const char* server_name, const char* user_name, const char* Pwd);
    void displays();
    void QuerySerial_();  // Serial Querying
    void QueryConcurrent_();  // Concurrent Querying
    void QueryConcurrentHandler_(std::map<int, std::string>::iterator itr,
                                   int query_period, int query_cycle);  //
};

#endif  // DNS_TRACKER_CODE_FINAL_INCLUDE_CONTROLLER_QUERY_HPP_
