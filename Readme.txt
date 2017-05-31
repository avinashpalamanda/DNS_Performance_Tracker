                                                                                        DNS_PERFORMANCE_TRACKER

The tool queries the DNS using Type A query and determines the corresponding round trip time or latency time of the response.

The output of the tool consist of list of domain names and their Average Latency,SD,Minimum & Maximum Query Time sorted in increasing order.

MYSQL DB was used to store the Latency Average and Standard Deviation of the latency time.

Design model is a partial version of MVC architecture where the both the Model and Controller files implement the view capability.

Files : 

/src/main.cpp                  - The file accepts the input from the user and uses the controller_query class to run the queries.
/src/controller_query          - This file acts as a interface between LDNS and SQL++.
				    --->It consist of class objects for controller_DNS_resolver class and model_DB_latency class.
				    --->The class consist of member function to run the query model serially as well as conccurrently.
				    --->The concurrent/threaded model was implemented since in some cases query to DNS such as baidu.com took long time and in this gap the system remains 				                idle.Moreover in threaded the queries are running in concurrent manner providing better performance.	
/src/controller_DNS_resolver   - This file uses the LDNS to query the DNS and return with a latency value of the query.	
/src/model_DB_latency          - This file is used to provide the access to DB.		

Example : DNS_Performance_Tracker -f 1 -c 2 -p 0 -d latency_test -s localhost -u root -k password


Tables :

DOMAINS -  
+-----------+----------------+
| domain_id | domain_name    |
+-----------+----------------+
|         2 | youtube.com    |
|         3 | facebook.com   |
|         4 | baidu.com      |
|         5 | wikipedia.org  |
|         6 | yahoo.com      |
|         7 | google.co.in   |
|         8 | reddit.com     |
|         9 | qq.com         |
|        10 | taobao.com     |
|        11 | facebook.co.in |
+-----------+----------------+

STATISTICS - 
+-----------+-----------------+----------------------------+----------------+---------------------+---------------------+--------------------+--------------------+
| domain_id | latency_average | latency_standard_deviation | number_queries | query_first         | query_last          | Minimum_query_time | Maximum_query_time |
+-----------+-----------------+----------------------------+----------------+---------------------+---------------------+--------------------+--------------------+
|         2 |         43.6667 |                    4.49691 |              3 | 2017-05-30 15:18:12 | 2017-05-30 15:18:16 |                 40 |                 50 |
|         3 |         28.3333 |                     17.442 |              3 | 2017-05-30 15:18:12 | 2017-05-30 15:18:16 |                 16 |                 53 |
|         4 |         274.333 |                    31.9618 |              3 | 2017-05-30 15:18:12 | 2017-05-30 15:18:17 |                246 |                319 |
|         5 |              24 |                          0 |              3 | 2017-05-30 15:18:12 | 2017-05-30 15:18:17 |                 24 |                 24 |
|         6 |              22 |                    4.32049 |              3 | 2017-05-30 15:18:12 | 2017-05-30 15:18:17 |                 16 |                 26 |
|         7 |         34.6667 |                    4.02768 |              3 | 2017-05-30 15:18:13 | 2017-05-30 15:18:17 |                 29 |                 38 |
|         8 |              24 |                    4.96655 |              3 | 2017-05-30 15:18:13 | 2017-05-30 15:18:17 |                 17 |                 28 |
|         9 |             272 |                     11.225 |              3 | 2017-05-30 15:18:13 | 2017-05-30 15:18:17 |                260 |                287 |
|        10 |         183.333 |                    79.3697 |              3 | 2017-05-30 15:18:13 | 2017-05-30 15:18:17 |                 90 |                284 |
+-----------+-----------------+----------------------------+----------------+---------------------+---------------------+--------------------+--------------------+


Output :

                                            Rank Table
--------------------------------------------------------------------------------------------------------
Rank      Domain Name              Average Latency(ms)      Min_Query Time(ms)       Max_Query Time(ms)
--------------------------------------------------------------------------------------------------------
1         yahoo.com                22                       16                       26                       
2         wikipedia.org            24                       24                       24                       
3         reddit.com               24                       17                       28                       
4         facebook.com             28.3333                  16                       53                       
5         google.co.in             34.6667                  29                       38                       
6         youtube.com              43.6667                  40                       50                       
7         taobao.com               183.333                  90                       284                      
8         qq.com                   272                      260                      287                      
9         baidu.com                274.333                  246                      319                      
--------------------------------------------------------------------------------------------------------
				


Note : I have also provided a extra hhelper function which can be used to update(insert/delete) domains in the DOMAINS table.



