// Copyright [2017] <Avinash Palamanda>
// The File implements the member functions and their helper functions of class HelperFB class, which sets up connection with DB
// and provides access to tables DOMAINS and STATISTICS

#include"helper.hpp"

// Paramaterised Constructor of Class HelperDB .It sets up the connection with DB.
HelperDB::HelperDB(const char* DB_name, const char* server_name, const char* user_name, const char* Pwd) {
  try {
    DB_name_ = DB_name;
    server_name_ = server_name;
    user_name_ = user_name;
    password_ = Pwd;

    db_conn_.set_option(new mysqlpp::ReconnectOption(true));
    db_conn_.connect(DB_name_, server_name_, user_name_, password_);

    std::stringstream s;  // StringStream to create a query

    // Creates the DOMAINS TABLE if it doesnt exists
    s.str("");
    s << "CREATE TABLE IF NOT EXISTS DOMAINS (domain_id INT NOT NULL AUTO_INCREMENT PRIMARY KEY,domain_name VARCHAR(255));";
    mysqlpp::Query query = db_conn_.query(s.str());  // SQLpp Query Object to create a SQL query
    mysqlpp::SimpleResult result = query.execute();  // SQLpp Result object to know the result of execution of the query
    if (!result)  // In case the previous query execution fails
      throw std::string("Can't Create Tabel");

    s.str("");
    s << "SELECT * FROM DOMAINS;";
    query = db_conn_.query(s.str());
    mysqlpp::StoreQueryResult values = query.store();  // Object to store the result of the query execution

    // Inserts Intial 10 set of Domains if the DOMAINS  table has 0 rows
    if (values.num_rows() == 0) {  // If the DOMAINS table has 0 rows,insert new rows.
    s.str("");
    s << "INSERT INTO DOMAINS (domain_name) VALUES ('google.com'),('youtube.com'),('facebook.com'),";
    s << "('baidu.com'),('wikipedia.org'),('yahoo.com'),('google.co.in'),";
    s << "('reddit.com'),('qq.com'),('taobao.com');";
    query = db_conn_.query(s.str());
    result = query.execute();
    if (!result)  // // In case the previous query execution fails
      throw std::string("Can't Insert top Domains into table");
    }

    // Creates the STATISTICS TABLE if it doesnt exists
    s.str("");
    s << "CREATE TABLE IF NOT EXISTS STATISTICS (domain_id INT PRIMARY KEY,";
    s << "latency_average FLOAT(16) NOT NULL,";
    s << "latency_standard_deviation FLOAT(16) NOT NULL,";
    s << "number_queries INT,";
    s << "query_first TIMESTAMP NULL,";
    s << "query_last TIMESTAMP NULL,";
    s << "Minimum_query_time FLOAT(16),";
    s << "Maximum_query_time FLOAT(16),";
    s << "FOREIGN KEY (domain_id) REFERENCES DOMAINS(domain_id));";
    query = db_conn_.query(s.str());
    result = query.execute();
    if (!result)  // In case the previous query execution to create table STATISTICS fails
      throw std::string("Can't Create the STATISTICS table\n");
  }
  catch(std::string exception) {
    std::cout << exception << std::endl;
  }
  catch(std::exception& e) {
    std::stringstream es;
    es << "Can't perform DB operation due to -> " << e.what() << std::endl;
    std::cout << es.str();
  }
  catch(...) {
    std::cout << std::string("Can't perform DB operation") << std::endl;
  }
}

// Add new Domain as per user input
void HelperDB::InsertDomain(std::string domain) {
  try {
    std::stringstream s;  // StringStream to create a query
    // Creates the DOMAINS TABLE if it doesnt exists
    s.str("");
    s << "INSERT INTO DOMAINS (domain_name) VALUES ( \'" << domain <<"\');";
    mysqlpp::Query query = db_conn_.query(s.str());
    mysqlpp::SimpleResult result = query.execute();
    if (!result)  // // In case the previous query execution fails
      throw std::string("Can't Insert top Domains into table");
  }
  catch(std::string exception) {
    std::cout << exception << std::endl;
  }
  catch(std::exception& e) {
    std::stringstream es;
    es << "Can't perform DB operation due to -> " << e.what() << std::endl;
    std::cout << es.str();
  }
  catch(...) {
    std::cout << std::string("Can't perform DB operation") << std::endl;
  }
}


// Delete Domain as per user requirement
void HelperDB::DeleteDomain(std::string domain) {
  try {
    int domain_id;
    std::stringstream s;  // StringStream to create a query
    // Creates the DOMAINS TABLE if it doesnt exists
    s.str("");
    s << "SELECT domain_id FROM DOMAINS WHERE domain_name=\'" << domain <<"\';";
    mysqlpp::Query query = db_conn_.query(s.str());
    mysqlpp::StoreQueryResult values = query.store();
    if (values.size() == 0)
      return;
    if (values) {
      domain_id = values[0]["domain_id"];
    }
    s.str("");
    s << "DELETE FROM STATISTICS where domain_id=" << domain_id << ";";
    query = db_conn_.query(s.str());
    mysqlpp::SimpleResult result = query.execute();

    if (!result)
      throw std::string("Cannot Delete entry");

    s.str("");
    s << "DELETE FROM DOMAINS where domain_id=" << domain_id << ";";
    query = db_conn_.query(s.str());
    result = query.execute();

    if (!result)
      throw std::string("Cannot Delete entry");
  }
  catch(std::string exception) {
    std::cout << exception << std::endl;
  }
  catch(std::exception& e) {
    std::stringstream es;
    es << "Can't perform DB operation due to -> " << e.what() << std::endl;
    std::cout << es.str();
  }
  catch(...) {
    std::cout << std::string("Can't perform DB operation") << std::endl;
  }
}

// Member Function used to display the rank of the current table STATISTICS.
// Returns a string consisting of rank list.
// Queries both table with help of the foreign key domain_id.
std::string HelperDB::ShowRank() {
  try {
    std::stringstream s;  // Stringstream to prepare the query.

    // Queries to select the previous entries in the table
    s.str("");
    s << "SELECT d.domain_name,s.latency_average,s.Minimum_query_time,s.Maximum_query_time from DOMAINS d, STATISTICS s";
    s << " where d.domain_id=s.domain_id order by s.latency_average;";
    mysqlpp::Query query = db_conn_.query(s.str());
    mysqlpp::StoreQueryResult values = query.store();

    if (values) {
      s.str("");
      s << std::setw(55) << "Rank Table\n";
      s << "--------------------------------------------------------------------------------------------------------\n";
      s << std::left << std::setw(10) << "Rank" << std::setw(25) << "Domain Name" << std::setw(25) << "Average Latency(ms)" << std::setw(25) << "Min_Query Time(ms)";
      s << std::setw(15) << "Max_Query Time(ms)\n";
      s << "--------------------------------------------------------------------------------------------------------\n";
      if (values) {  // Get the values of old_latency_average average,old_SD,min query latency and max query latency
        for (size_t i=0; i < values.size(); i++) {
          s << std::left << std::setw(10) << i+1 << std::setw(25) << values[i]["domain_name"] << std::setw(25) << values[i]["latency_average"];
          s << std::setw(25) << values[i]["Minimum_query_time"] << std::setw(25) << values[i]["Maximum_query_time"] << "\n";
        }
      }
      s << "--------------------------------------------------------------------------------------------------------\n";
      return s.str();
    }
  }
  catch(std::string exception) {
    std::cout <<  exception << std::endl;
    exit(0);
  }
  catch(std::exception& e) {
    std::stringstream es;
    es << "Can't create DnsDbHandler() -> " << e.what() << std::endl;
    std::cout <<  es.str();
    exit(0);
  }
  catch(...) {
    std::cout << "Issue in DataBase Connection" << std::endl;
    exit(0);
  }
}
