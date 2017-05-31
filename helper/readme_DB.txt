Handler - DNS_DB_Handler :
	The main job of this handler is to connect with the DB and update the list of Domain's available in the DOMAINS table.

		1)Insert a new domain into DOMAINS table.							- 	i google.com
		2)Delete domain from DOMAINS table.This also removes the domain from the STATISTICS table.	-	d google.com
		3)Show table by rank(Sorted according to the Latency Average)					-	s


Syntax to run the Handler : 	./DNS_DB_Handler -d latency_test -s localhost -u root -k 1991@rajeswari
