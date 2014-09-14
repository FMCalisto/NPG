struct hostent
{
	char *h_name; 		// official host name
	char **h_aliases; 	// alias list
	int h_addrtype; 	// host address type
	int h_length;		// length of address
	char **h_addr_list;	// list of addresses (NULL term.)
};
