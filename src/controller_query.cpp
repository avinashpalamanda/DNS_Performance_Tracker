// Copyright [2017] <Avinash Palamanda>
// The File implements the member functions and their helper functions of main class ControllerQuery,
// which can be used to query Domain/DNS and update latency average and SD in table STATISTICS by serial or concurrent way.
#include "../include/controller_query.hpp"

// Helper Function used to generate random string to be prepened with the domain name.
std::string RandomStringGenerator() {
  const std::string kListWords = "0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";
  srand(time(0));
  std::string prepend = "";
  // Generates a random string of length 15.
  for (int i = 0; i < 15; i++) {
    prepend += kListWords[rand() % kListWords.size()];
  }
  return prepend;
}

// Paramaterised constructor of class ControllerQuery which sets up the period,cycle and creates a instance of class DnsDB,which helps in creating interface with DB.
// Instance of class DnsDB is created to get connection with DB.
// DnsDB's constructor inturn connects with DB and creates tables STATISTICS,DOMIANS and inserts intial set of domains, in case if it doesnt exists.
// Also gets map of <int,string> containing domain_id and domain_name with the help of DnsDB's member function GetDomains_().
// Input - period,Cycle,DB name,Server name,User Name,Password.
ControllerQuery::ControllerQuery(int query_period_input, int num_cycle_input, const char* DB_name, const char* server_name, const char* user_name, const char* Pwd):DB_handler_(DB_name, server_name, user_name, Pwd) {
  query_period_ = query_period_input;
  num_cycle_ = num_cycle_input;
  if (query_period_ <= 0)  // Sets default value of period to 1.
    query_period_ = 1;
  if (num_cycle_ <= 0)  // Incase cycle is 0,the function returns to main function.
      num_cycle_ = 10;
  domain_name_list_ = DB_handler_.GetDomains_();
}

// Member function of ControllerQuery used to obtain the latency value by quering the DNS in a serial way.
// Queries the Domain serially for in specific period for particular number of cycles and obtains the latency values.
// Obtained latency is then passed to DNSDB instance to update the latency avg and SD in table.
void ControllerQuery::QuerySerial_() {
  DNSResolver ldns_handler;  // Instance of the class DNSResolver to query the DNS and obtain latency.

  std::map<int, std::string>::iterator itr;  // Iterator used to access the domain_name_list_ map.
  itr = domain_name_list_.begin();

  while (true) {  // Run a infinite loop till no. of cycles reaches 0.
    std::stringstream domain_to_query;  // Stringstream to generate the prepended DNS name.Example xxxxxxxx.google.com.
    std::string prepend = RandomStringGenerator();
    itr = domain_name_list_.begin();
    for (; itr != domain_name_list_.end(); itr++) {  // Get latency value of all domains, available in template map.
      domain_to_query.str("");
      domain_to_query << prepend << "." << itr->second;  // Genrates the prepended DNS name.
      time_t current_time = time(NULL);
      double latency = ldns_handler.QueryResolve_(domain_to_query.str());  // Get the latency value for a particular DNS.
      DB_handler_.DnsLatencyDBUpdate_(latency, current_time, itr->first);  // Update the STATISTICS table with new latency.
    }
    num_cycle_--;  // Decreases the cycle value.
    if (num_cycle_ == 0)  // If cycle reaches 0 then returns to main function.
      break;
    sleep(query_period_);  // Wait for corresponding period of time.
  }
}

// Member function to query a particular Domain and update the table STATISTICS.Used in threaded application.
// Input : Map containing domain_id and name,period and cycle.
void ControllerQuery::QueryConcurrentHandler_(std::map<int, std::string>::iterator itr, int query_period, int query_cycle) {
  while (true) {  // Run a infinite loop till no. of cycles reaches 0.
    DNSResolver ldns_handler;  // Instance of the class DNSResolver to query the DNS and obtain latency.
    std::stringstream domain_to_query;  // Stringstream to generate the prepended domain name.Example xxxxxxxx.google.com.
    std::string prepend = RandomStringGenerator();
    domain_to_query.str("");
    domain_to_query << prepend << "." << itr->second;  // Genrates the prepended DNS name.
    time_t current_time = time(NULL);
    double latency = ldns_handler.QueryResolve_(domain_to_query.str());  // Get the latency value
    DB_lock_.lock();  // Lock the DB_lock_ mutex so that no other thread utilizes it.
    DB_handler_.DnsLatencyDBUpdate_(latency, current_time, itr->first);  // Update the STATISTICS table with new latency.
    DB_lock_.unlock();  // Unlock mutex
    query_cycle--;
    if (query_cycle == 0)  // If cycle reaches 0 then returns to main function.
      break;
    sleep(query_period);  // Wait for corresponding period of time.
  }
}

// Member function which creates threads to query domains for number of cycle and period
void ControllerQuery::QueryConcurrent_() {
  std::vector<std::thread> query_threads;  // Vector containing threads which are going to run are stored here.

  std::map<int, std::string>::iterator itr = domain_name_list_.begin();
  for (; itr != domain_name_list_.end(); itr++) {  // Query each domain in threaded way.Each DNS will have each thread.
    query_threads.push_back(std::thread(&ControllerQuery::QueryConcurrentHandler_, this, itr, query_period_, num_cycle_));
  }

  for (size_t i=0; i < domain_name_list_.size(); i++)  // Access the individual thread objects.
    if (query_threads[i].joinable())  // Incase if thread is still running,join the thread.
      query_threads[i].join();
}

// Function to Display Domains by rank
void ControllerQuery::displays() {
  std::cout << DB_handler_.ShowRank();
}
