// Copyright [2017] <Avinash Palamanda>
// The File implements the member functions and their helper functions of class DNSResolver,
// which can be used to query Domain and obtain the latency value.

#include"../include/controller_DNS_resolver.hpp"

// Function is used to obtain the current system timestamp.
void GetTimestamp(struct timespec* temp) {
  if (timespec_get(temp, CLOCK_MONOTONIC) == -1) {  // In case if the timestamp is not able to be obtained, the code exits.
      throw std::string("Error in Getting clock\n");
      exit(EXIT_FAILURE);
  }
}

// Member Function of the class DNSResolver.
// The function send the Type A DNS query to the particular domain/dns system and returns the latency of query.
// Latency - (Time at which query is sent - Time at which the answer for the query is recieved)
// Function utilizes LDNS
// Input Domain , example "google.com"
// Output Latency taken to query the domain
double DNSResolver::QueryResolve_(std::string domain_name) {
  try {
  struct timespec start, stop;
  ldns_resolver *res;
  ldns_rdf *domain;
  ldns_pkt *p;
  ldns_status s;

  p = NULL;
  domain = NULL;
  res = NULL;

  // Converts the given input string to appropirate domain name.
  domain = ldns_dname_new_frm_str(domain_name.c_str());
  if (!domain) {  // In case if we are not able to convert to appopirate domain name.
    throw std::string("Error in Domain Name");
  }

  /* create a new resolver from /etc/resolv.conf */
  s = ldns_resolver_new_frm_file(&res, NULL);

  if (s != LDNS_STATUS_OK) {  // Check LDNS status if not OK, then it throws exception.
    throw std::string("LDNS Status Not Ok");
  }

  // Gets the start and stop timestamp ,queries the domain with a type A query message.
  GetTimestamp(&start);
  p = ldns_resolver_query(res, domain, LDNS_RR_TYPE_A, LDNS_RR_CLASS_IN, LDNS_RD);
  GetTimestamp(&stop);

  double latency = double((stop.tv_sec-start.tv_sec)*1000+(double)(stop.tv_nsec-start.tv_nsec)/1000000);  // Calculates latency in ms.

  ldns_rdf_deep_free(domain);
  ldns_pkt_free(p);
  ldns_resolver_deep_free(res);
  return latency;
  }
  catch(std::string exception) {
    std::cout << exception << std::endl;
  }
  catch(std::exception& e) {
    std::stringstream es;
    es << "Can't Query DNS - " << e.what() << std::endl;
    std::cout <<  es.str();
  }
  catch(...) {
    std::cout << std::string("Can't Query DNS") << std::endl;
  }
}
