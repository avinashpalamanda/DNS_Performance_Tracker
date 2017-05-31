// Copyright [2017] <Avinash Palamanda>
// The File implements the member functions and their helper functions of class DNSResolver,
// which can be used to query Domain and obtain the latency value.

#ifndef DNS_TRACKER_CODE_FINAL_INCLUDE_CONTROLLER_DNS_RESOLVER_HPP_
#define DNS_TRACKER_CODE_FINAL_INCLUDE_CONTROLLER_DNS_RESOLVER_HPP_

#include <iostream>
#include <string>
#include <sstream>

#include </usr/include/ldns/ldns.h>

// Class used to run DNS query and obtain the latency time.
class DNSResolver{
 public:
  double QueryResolve_(std::string input);
};
void GetTimestamp(struct timespec* temp);  // Helper function to get current system timestamp.

#endif  // DNS_TRACKER_CODE_FINAL_INCLUDE_CONTROLLER_DNS_RESOLVER_HPP_
