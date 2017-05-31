// Copyright [2017] <Avinash Palamanda>
// The File implements the member functions and their helper functions of class DnsDB, which sets up connection with DB
// and provides access to tables DOMAINS and STATISTICS

#include"../include/model_latency_db.hpp"

// Paramaterised Constructor of Class DnsDB .It sets up the connection with DB.
// Creates Tables DOMAINS and STATISTICS in case they are not created already.
// Also inserts intial set of 10 Domains into the DOMAINS table,if the number of entries in DOMAINS table is 0.
// Input - DataBase Name,Server Name,User Name and Password of the SQL server.
DnsDB::DnsDB(const char* DB_name, const char* server_name, const char* user_name, const char* Pwd) {
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

// The fucntion is a member of DnsDB class.
// Function runs the Select query on DOMAINS table and returns a map of domain_id and domain_name
// Input - Nothing
// Output - Map<int, string> consisting of domain_id and domain_name example - <1,"google.com">
std::map<int, std::string> DnsDB::GetDomains_() {
  std::map<int, std::string> domain_list;  // The output variable of the function.
  try {
    std::stringstream s;  // StringStream to create a query.

    s.str(" ");
    s << "SELECT * FROM DOMAINS;";
    mysqlpp::Query query = db_conn_.query(s.str());
    mysqlpp::StoreQueryResult values = query.store();

    if (!values)  // In case the previous query could not get the data from DOMAINS table.
      throw std::string("Issue in obtaining Domains from Table DOMAINS\n");

    if (values) {  // If the values are obtained from the DOMAINS table create map<int,string>
      for (size_t i = 0; i < values.num_rows(); ++i) {
        domain_list.insert(std::make_pair(int(values[i]["domain_id"]), std::string(values[i]["domain_name"])));
      }
    }
    return domain_list;
  }

  catch(std::string exception) {
    std::cout << exception << std::endl;
  }
  catch(std::exception& e) {
    std::stringstream es;
    es << "Can't perform DB operation due to " << e.what() << std::endl;
    std::cout <<  es.str();
  }
  catch(...) {
    std::cout << "Can't perform DB operation at DOMAINS table\n";
  }
}

// Function finds the Standard Deviation on addition of new data from previously available mean,SD and number_queries.
// Input - Old Mean,OLD SD,New Value to be added and Number of previous entries
// Output - Vector consisting new mean and SD
std::vector<double> GetNewStandardDeviation(int n, double old_mean, double old_SD, double new_value) {
  std::vector<double> mean_SD;  // Vector to store the return values(mean and SD)

  // Operations to calculate new mean and SD
  double new_mean = ((n*old_mean)+new_value)/(n+1);
  double old_variance = pow(old_SD, 2);
  double new_variance = n*old_variance;
  new_variance += pow((new_value-new_mean), 2);
  new_variance += (n)*pow((old_mean-new_mean), 2);
  new_variance = new_variance/double(n+1);
  new_variance = sqrt(new_variance);

  mean_SD.push_back(new_mean);
  mean_SD.push_back(new_variance);

  return mean_SD;
}

// Member Function of class DnsDB
// First the function runs the SELECT command to get the previous entries for the dns_id
// If the dns_id is not present in the Table,it inserts a new row for the particular DNS/DOMAIN with the latency and timestamp value recieved.
// Else if there is entry available,it extracts those data and calculates the new mean & SD.
// On calculating the new value it updates the latency_average/mean,latency SD,Last timestamp,number of queries.
// Input - New latency time,Queried timestamp and Domain/Dns id.

void DnsDB::DnsLatencyDBUpdate_(int latency, time_t current_ts, int dns_id) {
  try {
    std::stringstream s;  // Stringstream to prepare the query.

    // Queries to select the previous entries in the table
    s.str("");
    s << "SELECT latency_average,latency_standard_deviation,number_queries,Minimum_query_time,Maximum_query_time";
    s << " FROM STATISTICS WHERE domain_id=" << dns_id << ";";
    mysqlpp::Query query = db_conn_.query(s.str());
    mysqlpp::StoreQueryResult values = query.store();

    if (values.num_rows() == 0) {  // No entries for that particular domain/dns id,hence inserts a new row.
      s.str("");
      s << "INSERT INTO STATISTICS (domain_id,latency_average,latency_standard_deviation,number_queries,";
      s << "query_first,query_last,Minimum_query_time,Maximum_query_time) VALUES(";
      s << dns_id << "," << latency << ",0,1,FROM_UNIXTIME(" << current_ts << "),FROM_UNIXTIME(" << current_ts << ")," << latency << "," << latency << ");";
      query = db_conn_.query(s.str());
      mysqlpp::SimpleResult result = query.execute();
      if (!result)  // In case the previous query could not insert data to STATISTICS table.
        throw std::string("Error in inserting new values STATISTICS table");
      return;  // Since the new data is entered,function exits
    }

    // Temp variable to store old avg latency, old SD, Min Query time and Max query Time from table STATISTICS
    double old_latency_average = 0;
    double old_SD = 0;
    double min = 0;
    double max = 0;
    int n = 0;
    std::vector<double> new_latency;
    if (values) {  // Get the values of old_latency_average average,old_SD,min query latency and max query latency
      old_latency_average = values[0]["latency_average"];
      old_SD = values[0]["latency_standard_deviation"];
      n = values[0]["number_queries"];
      new_latency = GetNewStandardDeviation(n, old_latency_average, old_SD, latency);
      min = values[0]["Minimum_query_time"];  // Minimum time taken to query that particular domain.
      max = values[0]["Maximum_query_time"];  // Maximum time taken to query that particular domain.
      if (min > latency)  // In case the new latency time is the minimum time taken to query DNS,update min.
        min = latency;
      if (max < latency)  // In case the new latency time is the maximum time taken to query DNS,update max.
        max = latency;
    }

    // Update the table with new values.
    s.str("");
    s << "UPDATE STATISTICS SET latency_average=" << new_latency[0];
    s << ",latency_standard_deviation=" << new_latency[1] << ",number_queries=";
    s << n+1 << ",query_last=FROM_UNIXTIME(" << current_ts << "),Minimum_query_time=" << min;
    s << ",Maximum_query_time=" << max << " WHERE domain_id=" << dns_id << ";";
    query = db_conn_.query(s.str());
    mysqlpp::SimpleResult result = query.execute();
    if (!result)  // In case if the update query fails.
      throw std::string("Error in updating new values STATISTICS table");
  }

  catch(std::string exception) {
    std::cout <<  exception << std::endl;
  }
  catch(std::exception& e) {
    std::stringstream es;
    es << "Can't create DnsDbHandler() -> " << e.what() << std::endl;
    std::cout <<  es.str();
  }
  catch(...) {
    std::cout << "Issue in DataBase Connection\n";
  }
}

// Member Function used to display the rank of the current table STATISTICS.
// Returns a string consisting of rank list.
// Queries both table with help of the foreign key domain_id.
std::string DnsDB::ShowRank() {
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
        for (size_t i = 0; i < values.size(); i++) {
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
