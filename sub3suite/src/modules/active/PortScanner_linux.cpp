/*
	TCP Syn port scanner code in C with Linux Sockets :)
*/

#include<stdio.h> //printf
#include<string.h> //memset
#include<stdlib.h> //for exit(0);
#include<sys/socket.h>
#include<errno.h> //For errno - the error number
#include<pthread.h>
#include<netdb.h>	//hostend
#include<arpa/inet.h>
#include<netinet/tcp.h>	//Provides declarations for tcp header
#include<netinet/ip.h>	//Provides declarations for ip header

void * receive_ack( void *ptr );
void process_packet(unsigned char* , int);
unsigned short csum(unsigned short * , int );
char * hostname_to_ip(char * );
int get_local_ip (char *);

struct pseudo_header    //needed for checksum calculation
{
	unsigned int source_address;
	unsigned int dest_address;
	unsigned char placeholder;
	unsigned char protocol;
	unsigned short tcp_length;
	
	struct tcphdr tcp;
};

struct in_addr dest_ip;

int main(int argc, char *argv[])
{
	//Create a raw socket
	int s = socket (AF_INET, SOCK_RAW , IPPROTO_TCP);
	if(s < 0)
	{
		printf ("Error creating socket. Error number : %d . Error message : %s \n" , errno , strerror(errno));
		exit(0);
	}
	else
	{
		printf("Socket created.\n");
	}
		
	//Datagram to represent the packet
	char datagram[4096];	
	
	//IP header
	struct iphdr *iph = (struct iphdr *) datagram;
	
	//TCP header
	struct tcphdr *tcph = (struct tcphdr *) (datagram + sizeof (struct ip));
	
	struct sockaddr_in  dest;
	struct pseudo_header psh;
	
	char *target = argv[1];
	
	if(argc < 2)
	{
		printf("Please specify a hostname \n");
		exit(1);
	}
	
	if( inet_addr( target ) != -1)
	{
		dest_ip.s_addr = inet_addr( target );
	}
	else
	{
		char *ip = hostname_to_ip(target);
		if(ip != NULL)
		{
			printf("%s resolved to %s \n" , target , ip);
			//Convert domain name to IP
			dest_ip.s_addr = inet_addr( hostname_to_ip(target) );
		}
		else
		{
			printf("Unable to resolve hostname : %s" , target);
			exit(1);
		}
	}
	
	int source_port = 43591;
	char source_ip[20];
	get_local_ip( source_ip );
	
	printf("Local source IP is %s \n" , source_ip);
	
	memset (datagram, 0, 4096);	/* zero out the buffer */
	
	//Fill in the IP Header
	iph->ihl = 5;
	iph->version = 4;
	iph->tos = 0;
	iph->tot_len = sizeof (struct ip) + sizeof (struct tcphdr);
	iph->id = htons (54321);	//Id of this packet
	iph->frag_off = htons(16384);
	iph->ttl = 64;
	iph->protocol = IPPROTO_TCP;
	iph->check = 0;		//Set to 0 before calculating checksum
	iph->saddr = inet_addr ( source_ip );	//Spoof the source ip address
	iph->daddr = dest_ip.s_addr;
	
	iph->check = csum ((unsigned short *) datagram, iph->tot_len >> 1);
	
	//TCP Header
	tcph->source = htons ( source_port );
	tcph->dest = htons (80);
	tcph->seq = htonl(1105024978);
	tcph->ack_seq = 0;
	tcph->doff = sizeof(struct tcphdr) / 4;		//Size of tcp header
	tcph->fin=0;
	tcph->syn=1;
	tcph->rst=0;
	tcph->psh=0;
	tcph->ack=0;
	tcph->urg=0;
	tcph->window = htons ( 14600 );	// maximum allowed window size
	tcph->check = 0; //if you set a checksum to zero, your kernel's IP stack should fill in the correct checksum during transmission
	tcph->urg_ptr = 0;
	
	//IP_HDRINCL to tell the kernel that headers are included in the packet
	int one = 1;
	const int *val = &one;
	
	if (setsockopt (s, IPPROTO_IP, IP_HDRINCL, val, sizeof (one)) < 0)
	{
		printf ("Error setting IP_HDRINCL. Error number : %d . Error message : %s \n" , errno , strerror(errno));
		exit(0);
	}
	
	printf("Starting sniffer thread...\n");
	char *message1 = "Thread 1";
	int  iret1;
	pthread_t sniffer_thread;

	if( pthread_create( &sniffer_thread , NULL ,  receive_ack , (void*) message1) < 0)
	{
		printf ("Could not create sniffer thread. Error number : %d . Error message : %s \n" , errno , strerror(errno));
		exit(0);
	}

	printf("Starting to send syn packets\n");
	
	int port;
	dest.sin_family = AF_INET;
	dest.sin_addr.s_addr = dest_ip.s_addr;
	for(port = 1 ; port < 100 ; port++)
	{
		tcph->dest = htons ( port );
		tcph->check = 0;	// if you set a checksum to zero, your kernel's IP stack should fill in the correct checksum during transmission
		
		psh.source_address = inet_addr( source_ip );
		psh.dest_address = dest.sin_addr.s_addr;
		psh.placeholder = 0;
		psh.protocol = IPPROTO_TCP;
		psh.tcp_length = htons( sizeof(struct tcphdr) );
		
		memcpy(&psh.tcp , tcph , sizeof (struct tcphdr));
		
		tcph->check = csum( (unsigned short*) &psh , sizeof (struct pseudo_header));
		
		//Send the packet
		if ( sendto (s, datagram , sizeof(struct iphdr) + sizeof(struct tcphdr) , 0 , (struct sockaddr *) &dest, sizeof (dest)) < 0)
		{
			printf ("Error sending syn packet. Error number : %d . Error message : %s \n" , errno , strerror(errno));
			exit(0);
		}
	}
	
	pthread_join( sniffer_thread , NULL);
	printf("%d" , iret1);
	
	return 0;
}

/*
	Method to sniff incoming packets and look for Ack replies
*/
void * receive_ack( void *ptr )
{
	//Start the sniffer thing
	start_sniffer();
}

int start_sniffer()
{
	int sock_raw;
	
	int saddr_size , data_size;
	struct sockaddr saddr;
	
	unsigned char *buffer = (unsigned char *)malloc(65536); //Its Big!
	
	printf("Sniffer initialising...\n");
	fflush(stdout);
	
	//Create a raw socket that shall sniff
	sock_raw = socket(AF_INET , SOCK_RAW , IPPROTO_TCP);
	
	if(sock_raw < 0)
	{
		printf("Socket Error\n");
		fflush(stdout);
		return 1;
	}
	
	saddr_size = sizeof saddr;
	
	while(1)
	{
		//Receive a packet
		data_size = recvfrom(sock_raw , buffer , 65536 , 0 , &saddr , &saddr_size);
		
		if(data_size <0 )
		{
			printf("Recvfrom error , failed to get packets\n");
			fflush(stdout);
			return 1;
		}
		
		//Now process the packet
		process_packet(buffer , data_size);
	}
	
	close(sock_raw);
	printf("Sniffer finished.");
	fflush(stdout);
	return 0;
}

void process_packet(unsigned char* buffer, int size)
{
	//Get the IP Header part of this packet
	struct iphdr *iph = (struct iphdr*)buffer;
	struct sockaddr_in source,dest;
	unsigned short iphdrlen;
	
	if(iph->protocol == 6)
	{
		struct iphdr *iph = (struct iphdr *)buffer;
		iphdrlen = iph->ihl*4;
	
		struct tcphdr *tcph=(struct tcphdr*)(buffer + iphdrlen);
			
		memset(&source, 0, sizeof(source));
		source.sin_addr.s_addr = iph->saddr;
	
		memset(&dest, 0, sizeof(dest));
		dest.sin_addr.s_addr = iph->daddr;
		
		if(tcph->syn == 1 && tcph->ack == 1 && source.sin_addr.s_addr == dest_ip.s_addr )
		{
			printf("Port %d open \n" , ntohs(tcph->source));
			fflush(stdout);
		}
	}
}

/*
 Checksums - IP and TCP
 */
unsigned short csum(unsigned short *ptr,int nbytes) 
{
	register long sum;
	unsigned short oddbyte;
	register short answer;

	sum=0;
	while(nbytes>1) {
		sum+=*ptr++;
		nbytes-=2;
	}
	if(nbytes==1) {
		oddbyte=0;
		*((u_char*)&oddbyte)=*(u_char*)ptr;
		sum+=oddbyte;
	}

	sum = (sum>>16)+(sum & 0xffff);
	sum = sum + (sum>>16);
	answer=(short)~sum;
	
	return(answer);
}

/*
	Get ip from domain name
 */
char* hostname_to_ip(char * hostname)
{
	struct hostent *he;
	struct in_addr **addr_list;
	int i;
		
	if ( (he = gethostbyname( hostname ) ) == NULL) 
	{
		// get the host info
		herror("gethostbyname");
		return NULL;
	}

	addr_list = (struct in_addr **) he->h_addr_list;
	
	for(i = 0; addr_list[i] != NULL; i++) 
	{
		//Return the first one;
		return inet_ntoa(*addr_list[i]) ;
	}
	
	return NULL;
}

/*
 Get source IP of system , like 192.168.0.6 or 192.168.1.2
 */

int get_local_ip ( char * buffer)
{
	int sock = socket ( AF_INET, SOCK_DGRAM, 0);

	const char* kGoogleDnsIp = "8.8.8.8";
	int dns_port = 53;

	struct sockaddr_in serv;

	memset( &serv, 0, sizeof(serv) );
	serv.sin_family = AF_INET;
	serv.sin_addr.s_addr = inet_addr(kGoogleDnsIp);
	serv.sin_port = htons( dns_port );

	int err = connect( sock , (const struct sockaddr*) &serv , sizeof(serv) );

	struct sockaddr_in name;
	socklen_t namelen = sizeof(name);
	err = getsockname(sock, (struct sockaddr*) &name, &namelen);

	const char *p = inet_ntop(AF_INET, &name.sin_addr, buffer, 100);

	close(sock);
}