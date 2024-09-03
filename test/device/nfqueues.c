// THIS FILE HAS BEEN AUTOGENERATED. DO NOT EDIT.

/**
 * Nefilter queue for device tplink-plug
 */

// Standard libraries
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>
#include <assert.h>
#include <signal.h>
#include <sys/time.h>
// Custom libraries
#include "nfqueue.h"
#include "packet_utils.h"
#include "rule_utils.h"
// Parsers
#include "header.h"
#include "dns.h"


/* CONSTANTS */

float DROP_PROBA = 1.0;  // Drop probability for random drop verdict mode

#define NUM_THREADS 4

/**
 * Thread-specific data.
 */
typedef struct {
    uint8_t   id;      // Thread ID
    uint32_t  seed;    // Thread-specific seed for random number generation
    pthread_t thread;  // The thread itself
} thread_data_t;

thread_data_t thread_data[NUM_THREADS];

dns_map_t *dns_map;  // Domain name to IP address mapping

#ifdef DEBUG
uint16_t dropped_packets = 0;
#endif /* DEBUG */


/**
 * @brief dns-query-tplinkapi callback function, called when a packet enters the queue.
 * 
 * @param pkt_id packet ID for netfilter queue
 * @param hash packet payload SHA256 hash (only present if LOG is defined)
 * @param timestamp packet timestamp (only present if LOG is defined)
 * @param pkt_len packet length, in bytes
 * @param payload pointer to the packet payload
 * @param arg pointer to the argument passed to the callback function
 * @return the verdict for the packet
 */
#ifdef LOG
uint32_t callback_dns_query_tplinkapi(int pkt_id, uint8_t *hash, struct timeval timestamp, int pkt_len, uint8_t *payload, void *arg)
#else
uint32_t callback_dns_query_tplinkapi(int pkt_id, int pkt_len, uint8_t *payload, void *arg)
#endif /* LOG */
{
    #ifdef DEBUG
    printf("Received packet from nfqueue 0\n");
    #endif

    // Skip layer 3 and 4 headers
    size_t skipped = get_headers_length(payload);

    // Parse payload as DNS message
    dns_message_t dns_message = dns_parse_message(payload + skipped);
    #ifdef DEBUG
    dns_print_message(dns_message);
    #endif
    uint32_t verdict = NF_ACCEPT;  // Packet verdict: ACCEPT or DROP

    /* Policy dns-query-tplinkapi */
    if (
        dns_message.header.qr == 0
        &&
        ( dns_message.header.qdcount > 0 && dns_message.questions->qtype == A )
        &&
        dns_contains_full_domain_name(dns_message.questions, dns_message.header.qdcount, "use1-api.tplinkra.com")
    ) {



        uint32_t old_verdict = verdict;

        // Binary DROP
        verdict = NF_DROP;

        #if defined LOG || defined DEBUG
        if (verdict == NF_DROP) {
            #ifdef LOG
            print_hash(hash);
            printf(",%ld.%06ld,dns-query-tplinkapi,,DROP\n", (long int)timestamp.tv_sec, (long int)timestamp.tv_usec);
            #endif /* LOG */
            #ifdef DEBUG
            printf("DROP - Policy: dns-query-tplinkapi\n");
            if (old_verdict != NF_DROP) {
                dropped_packets++;
                printf("Dropped packets: %hu\n", dropped_packets);
            }
            #endif /* DEBUG */
        }
        #endif /* LOG || DEBUG */

    }
    /* Policy dns-query-tplinkcloud */
    if (
        dns_message.header.qr == 0
        &&
        ( dns_message.header.qdcount > 0 && dns_message.questions->qtype == A )
        &&
        dns_contains_full_domain_name(dns_message.questions, dns_message.header.qdcount, "n-devs.tplinkcloud.com")
    ) {



        uint32_t old_verdict = verdict;

        // Binary DROP
        verdict = NF_DROP;

        #if defined LOG || defined DEBUG
        if (verdict == NF_DROP) {
            #ifdef LOG
            print_hash(hash);
            printf(",%ld.%06ld,dns-query-tplinkcloud,,DROP\n", (long int)timestamp.tv_sec, (long int)timestamp.tv_usec);
            #endif /* LOG */
            #ifdef DEBUG
            printf("DROP - Policy: dns-query-tplinkcloud\n");
            if (old_verdict != NF_DROP) {
                dropped_packets++;
                printf("Dropped packets: %hu\n", dropped_packets);
            }
            #endif /* DEBUG */
        }
        #endif /* LOG || DEBUG */

    }

    // Free memory allocated for parsed messages
    dns_free_message(dns_message);

    #ifdef LOG
    if (verdict != NF_DROP) {
        // Log packet as accepted
        print_hash(hash);
        printf(",%ld.%06ld,dns-query-tplinkapi,,ACCEPT\n", (long int)timestamp.tv_sec, (long int)timestamp.tv_usec);
    }
    free(hash);
    #endif /* LOG */

    return verdict;
}




/**
 * @brief dns-query-tplinkapi-backward callback function, called when a packet enters the queue.
 * 
 * @param pkt_id packet ID for netfilter queue
 * @param hash packet payload SHA256 hash (only present if LOG is defined)
 * @param timestamp packet timestamp (only present if LOG is defined)
 * @param pkt_len packet length, in bytes
 * @param payload pointer to the packet payload
 * @param arg pointer to the argument passed to the callback function
 * @return the verdict for the packet
 */
#ifdef LOG
uint32_t callback_dns_query_tplinkapi_backward(int pkt_id, uint8_t *hash, struct timeval timestamp, int pkt_len, uint8_t *payload, void *arg)
#else
uint32_t callback_dns_query_tplinkapi_backward(int pkt_id, int pkt_len, uint8_t *payload, void *arg)
#endif /* LOG */
{
    #ifdef DEBUG
    printf("Received packet from nfqueue 1\n");
    #endif

    // Skip layer 3 and 4 headers
    size_t skipped = get_headers_length(payload);

    // Parse payload as DNS message
    dns_message_t dns_message = dns_parse_message(payload + skipped);
    #ifdef DEBUG
    dns_print_message(dns_message);
    #endif
    uint32_t verdict = NF_ACCEPT;  // Packet verdict: ACCEPT or DROP

    /* Policy dns-query-tplinkapi-backward */
    if (
        dns_message.header.qr == 1
        &&
        ( dns_message.header.qdcount > 0 && dns_message.questions->qtype == A )
        &&
        dns_contains_full_domain_name(dns_message.questions, dns_message.header.qdcount, "use1-api.tplinkra.com")
    ) {

        // Retrieve IP addresses corresponding to the given domain name from the DNS response
        char *domain_name = NULL;
        ip_list_t ip_list = ip_list_init();
        domain_name = "use1-api.tplinkra.com";
        ip_list = dns_get_ip_from_name(dns_message.answers, dns_message.header.ancount, domain_name);

        if (ip_list.ip_count > 0) {
            // Add IP addresses to DNS map
            dns_map_add(dns_map, domain_name, ip_list);
        }

        uint32_t old_verdict = verdict;

        // Binary DROP
        verdict = NF_DROP;

        #if defined LOG || defined DEBUG
        if (verdict == NF_DROP) {
            #ifdef LOG
            print_hash(hash);
            printf(",%ld.%06ld,dns-query-tplinkapi-backward,,DROP\n", (long int)timestamp.tv_sec, (long int)timestamp.tv_usec);
            #endif /* LOG */
            #ifdef DEBUG
            printf("DROP - Policy: dns-query-tplinkapi-backward\n");
            if (old_verdict != NF_DROP) {
                dropped_packets++;
                printf("Dropped packets: %hu\n", dropped_packets);
            }
            #endif /* DEBUG */
        }
        #endif /* LOG || DEBUG */

    }
    /* Policy dns-query-tplinkcloud-backward */
    if (
        dns_message.header.qr == 1
        &&
        ( dns_message.header.qdcount > 0 && dns_message.questions->qtype == A )
        &&
        dns_contains_full_domain_name(dns_message.questions, dns_message.header.qdcount, "n-devs.tplinkcloud.com")
    ) {

        // Retrieve IP addresses corresponding to the given domain name from the DNS response
        char *domain_name = NULL;
        ip_list_t ip_list = ip_list_init();
        domain_name = "n-devs.tplinkcloud.com";
        ip_list = dns_get_ip_from_name(dns_message.answers, dns_message.header.ancount, domain_name);

        if (ip_list.ip_count > 0) {
            // Add IP addresses to DNS map
            dns_map_add(dns_map, domain_name, ip_list);
        }

        uint32_t old_verdict = verdict;

        // Binary DROP
        verdict = NF_DROP;

        #if defined LOG || defined DEBUG
        if (verdict == NF_DROP) {
            #ifdef LOG
            print_hash(hash);
            printf(",%ld.%06ld,dns-query-tplinkcloud-backward,,DROP\n", (long int)timestamp.tv_sec, (long int)timestamp.tv_usec);
            #endif /* LOG */
            #ifdef DEBUG
            printf("DROP - Policy: dns-query-tplinkcloud-backward\n");
            if (old_verdict != NF_DROP) {
                dropped_packets++;
                printf("Dropped packets: %hu\n", dropped_packets);
            }
            #endif /* DEBUG */
        }
        #endif /* LOG || DEBUG */

    }

    // Free memory allocated for parsed messages
    dns_free_message(dns_message);

    #ifdef LOG
    if (verdict != NF_DROP) {
        // Log packet as accepted
        print_hash(hash);
        printf(",%ld.%06ld,dns-query-tplinkapi-backward,,ACCEPT\n", (long int)timestamp.tv_sec, (long int)timestamp.tv_usec);
    }
    free(hash);
    #endif /* LOG */

    return verdict;
}




/**
 * @brief wan-https-to-domain-tplinkapi callback function, called when a packet enters the queue.
 * 
 * @param pkt_id packet ID for netfilter queue
 * @param hash packet payload SHA256 hash (only present if LOG is defined)
 * @param timestamp packet timestamp (only present if LOG is defined)
 * @param pkt_len packet length, in bytes
 * @param payload pointer to the packet payload
 * @param arg pointer to the argument passed to the callback function
 * @return the verdict for the packet
 */
#ifdef LOG
uint32_t callback_wan_https_to_domain_tplinkapi(int pkt_id, uint8_t *hash, struct timeval timestamp, int pkt_len, uint8_t *payload, void *arg)
#else
uint32_t callback_wan_https_to_domain_tplinkapi(int pkt_id, int pkt_len, uint8_t *payload, void *arg)
#endif /* LOG */
{
    #ifdef DEBUG
    printf("Received packet from nfqueue 10\n");
    #endif

    uint32_t dst_addr = get_ipv4_dst_addr(payload);  // IPv4 destination address, in network byte order
    uint32_t verdict = NF_ACCEPT;  // Packet verdict: ACCEPT or DROP

    /* Policy wan-https-to-domain-tplinkapi */
    if (
        ( dns_entry_contains(dns_map_get(dns_map, "use1-api.tplinkra.com"), (ip_addr_t) {.version = 4, .value.ipv4 = dst_addr}) )
    ) {



        uint32_t old_verdict = verdict;

        // Binary DROP
        verdict = NF_DROP;

        #if defined LOG || defined DEBUG
        if (verdict == NF_DROP) {
            #ifdef LOG
            print_hash(hash);
            printf(",%ld.%06ld,wan-https-to-domain-tplinkapi,,DROP\n", (long int)timestamp.tv_sec, (long int)timestamp.tv_usec);
            #endif /* LOG */
            #ifdef DEBUG
            printf("DROP - Policy: wan-https-to-domain-tplinkapi\n");
            if (old_verdict != NF_DROP) {
                dropped_packets++;
                printf("Dropped packets: %hu\n", dropped_packets);
            }
            #endif /* DEBUG */
        }
        #endif /* LOG || DEBUG */

    }
    /* Policy wan-https-to-domain-tplinkcloud */
    if (
        ( dns_entry_contains(dns_map_get(dns_map, "n-devs.tplinkcloud.com"), (ip_addr_t) {.version = 4, .value.ipv4 = dst_addr}) )
    ) {



        uint32_t old_verdict = verdict;

        // Binary DROP
        verdict = NF_DROP;

        #if defined LOG || defined DEBUG
        if (verdict == NF_DROP) {
            #ifdef LOG
            print_hash(hash);
            printf(",%ld.%06ld,wan-https-to-domain-tplinkcloud,,DROP\n", (long int)timestamp.tv_sec, (long int)timestamp.tv_usec);
            #endif /* LOG */
            #ifdef DEBUG
            printf("DROP - Policy: wan-https-to-domain-tplinkcloud\n");
            if (old_verdict != NF_DROP) {
                dropped_packets++;
                printf("Dropped packets: %hu\n", dropped_packets);
            }
            #endif /* DEBUG */
        }
        #endif /* LOG || DEBUG */

    }


    #ifdef LOG
    if (verdict != NF_DROP) {
        // Log packet as accepted
        print_hash(hash);
        printf(",%ld.%06ld,wan-https-to-domain-tplinkapi,,ACCEPT\n", (long int)timestamp.tv_sec, (long int)timestamp.tv_usec);
    }
    free(hash);
    #endif /* LOG */

    return verdict;
}




/**
 * @brief wan-https-to-domain-tplinkapi-backward callback function, called when a packet enters the queue.
 * 
 * @param pkt_id packet ID for netfilter queue
 * @param hash packet payload SHA256 hash (only present if LOG is defined)
 * @param timestamp packet timestamp (only present if LOG is defined)
 * @param pkt_len packet length, in bytes
 * @param payload pointer to the packet payload
 * @param arg pointer to the argument passed to the callback function
 * @return the verdict for the packet
 */
#ifdef LOG
uint32_t callback_wan_https_to_domain_tplinkapi_backward(int pkt_id, uint8_t *hash, struct timeval timestamp, int pkt_len, uint8_t *payload, void *arg)
#else
uint32_t callback_wan_https_to_domain_tplinkapi_backward(int pkt_id, int pkt_len, uint8_t *payload, void *arg)
#endif /* LOG */
{
    #ifdef DEBUG
    printf("Received packet from nfqueue 11\n");
    #endif

    uint32_t src_addr = get_ipv4_src_addr(payload);  // IPv4 source address, in network byte order
    uint32_t verdict = NF_ACCEPT;  // Packet verdict: ACCEPT or DROP

    /* Policy wan-https-to-domain-tplinkapi-backward */
    if (
        ( dns_entry_contains(dns_map_get(dns_map, "use1-api.tplinkra.com"), (ip_addr_t) {.version = 4, .value.ipv4 = src_addr}) )
    ) {



        uint32_t old_verdict = verdict;

        // Binary DROP
        verdict = NF_DROP;

        #if defined LOG || defined DEBUG
        if (verdict == NF_DROP) {
            #ifdef LOG
            print_hash(hash);
            printf(",%ld.%06ld,wan-https-to-domain-tplinkapi-backward,,DROP\n", (long int)timestamp.tv_sec, (long int)timestamp.tv_usec);
            #endif /* LOG */
            #ifdef DEBUG
            printf("DROP - Policy: wan-https-to-domain-tplinkapi-backward\n");
            if (old_verdict != NF_DROP) {
                dropped_packets++;
                printf("Dropped packets: %hu\n", dropped_packets);
            }
            #endif /* DEBUG */
        }
        #endif /* LOG || DEBUG */

    }
    /* Policy wan-https-to-domain-tplinkcloud-backward */
    if (
        ( dns_entry_contains(dns_map_get(dns_map, "n-devs.tplinkcloud.com"), (ip_addr_t) {.version = 4, .value.ipv4 = src_addr}) )
    ) {



        uint32_t old_verdict = verdict;

        // Binary DROP
        verdict = NF_DROP;

        #if defined LOG || defined DEBUG
        if (verdict == NF_DROP) {
            #ifdef LOG
            print_hash(hash);
            printf(",%ld.%06ld,wan-https-to-domain-tplinkcloud-backward,,DROP\n", (long int)timestamp.tv_sec, (long int)timestamp.tv_usec);
            #endif /* LOG */
            #ifdef DEBUG
            printf("DROP - Policy: wan-https-to-domain-tplinkcloud-backward\n");
            if (old_verdict != NF_DROP) {
                dropped_packets++;
                printf("Dropped packets: %hu\n", dropped_packets);
            }
            #endif /* DEBUG */
        }
        #endif /* LOG || DEBUG */

    }


    #ifdef LOG
    if (verdict != NF_DROP) {
        // Log packet as accepted
        print_hash(hash);
        printf(",%ld.%06ld,wan-https-to-domain-tplinkapi-backward,,ACCEPT\n", (long int)timestamp.tv_sec, (long int)timestamp.tv_usec);
    }
    free(hash);
    #endif /* LOG */

    return verdict;
}



/**
 * @brief SIGINT handler, flush stdout and exit.
 *
 * @param arg unused
 */
void sigint_handler(int arg) {
    fflush(stdout);
    exit(0);
}


/**
 * @brief Print program usage.
 * 
 * @param prog program name
 */
void usage(char* prog) {
    fprintf(stderr, "Usage: %s [-s DNS_SERVER_IP] [-p DROP_PROBA]\n", prog);
}


/**
 * @brief Program entry point
 * 
 * @param argc number of command line arguments
 * @param argv list of command line arguments
 * @return exit code, 0 if success
 */
int main(int argc, char *argv[]) {

    // Initialize variables
    int ret;
    char *dns_server_ip = "8.8.8.8";  // Default DNS server: Google Quad8

    // Setup SIGINT handler
    signal(SIGINT, sigint_handler);


    /* COMMAND LINE ARGUMENTS */
    int opt;
    while ((opt = getopt(argc, argv, "hp:s:")) != -1)
    {
        switch (opt)
        {
        case 'h':
            /* Help */
            usage(argv[0]);
            exit(EXIT_SUCCESS);
        case 'p':
            /* Random verdict mode: drop probability (float between 0 and 1) */
            DROP_PROBA = atof(optarg);
            break;
        case 's':
            /* IP address of the network gateway */
            dns_server_ip = optarg;
            break;
        default:
            usage(argv[0]);
            exit(EXIT_FAILURE);
        }
    }
    #ifdef DEBUG
    printf("Drop probability for random verdict mode: %f\n", DROP_PROBA);
    #endif /* DEBUG */


    #ifdef LOG
    // CSV log file header
    printf("hash,timestamp,policy,state,verdict\n");
    #endif /* LOG */


    /* GLOBAL STRUCTURES INITIALIZATION */

    // Initialize variables for DNS
    dns_map = dns_map_create();
    dns_message_t dns_response;
    ip_list_t ip_list;
    dns_entry_t *dns_entry;

    // Open socket for DNS
    int sockfd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (sockfd < 0) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    // Server address: network gateway
    struct sockaddr_in server_addr;
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(53);
    server_addr.sin_addr.s_addr = inet_addr(dns_server_ip);

    // Add addresses for domain use1-api.tplinkra.com to DNS map
    ret = dns_send_query("use1-api.tplinkra.com", sockfd, &server_addr);
    if (ret == 0) {
        ret = dns_receive_response(sockfd, &server_addr, &dns_response);
        if (ret == 0) {
            ip_list = dns_get_ip_from_name(dns_response.answers, dns_response.header.ancount, "use1-api.tplinkra.com");
            dns_map_add(dns_map, "use1-api.tplinkra.com", ip_list);
            #ifdef DEBUG
            // Check DNS map has been correctly updated
            dns_entry = dns_map_get(dns_map, "use1-api.tplinkra.com");
            dns_entry_print(dns_entry);
            #endif /* DEBUG */
        }
    }

    // Add addresses for domain n-devs.tplinkcloud.com to DNS map
    ret = dns_send_query("n-devs.tplinkcloud.com", sockfd, &server_addr);
    if (ret == 0) {
        ret = dns_receive_response(sockfd, &server_addr, &dns_response);
        if (ret == 0) {
            ip_list = dns_get_ip_from_name(dns_response.answers, dns_response.header.ancount, "n-devs.tplinkcloud.com");
            dns_map_add(dns_map, "n-devs.tplinkcloud.com", ip_list);
            #ifdef DEBUG
            // Check DNS map has been correctly updated
            dns_entry = dns_map_get(dns_map, "n-devs.tplinkcloud.com");
            dns_entry_print(dns_entry);
            #endif /* DEBUG */
        }
    }

    

    /* NFQUEUE THREADS LAUNCH */

    // Create threads
    uint8_t i = 0;

    /* dns-query-tplinkapi */
    // Setup thread-specific data
    thread_data[i].id = i;
    thread_data[i].seed = time(NULL) + i;
    thread_arg_t thread_arg_dns_query_tplinkapi = {
        .queue_id = 0,
        .func = &callback_dns_query_tplinkapi,
        .arg = &(thread_data[i].id)
    };
    ret = pthread_create(&(thread_data[i++].thread), NULL, nfqueue_thread, (void *) &thread_arg_dns_query_tplinkapi);
    assert(ret == 0);
    
    /* dns-query-tplinkapi-backward */
    // Setup thread-specific data
    thread_data[i].id = i;
    thread_data[i].seed = time(NULL) + i;
    thread_arg_t thread_arg_dns_query_tplinkapi_backward = {
        .queue_id = 1,
        .func = &callback_dns_query_tplinkapi_backward,
        .arg = &(thread_data[i].id)
    };
    ret = pthread_create(&(thread_data[i++].thread), NULL, nfqueue_thread, (void *) &thread_arg_dns_query_tplinkapi_backward);
    assert(ret == 0);
    
    /* wan-https-to-domain-tplinkapi */
    // Setup thread-specific data
    thread_data[i].id = i;
    thread_data[i].seed = time(NULL) + i;
    thread_arg_t thread_arg_wan_https_to_domain_tplinkapi = {
        .queue_id = 10,
        .func = &callback_wan_https_to_domain_tplinkapi,
        .arg = &(thread_data[i].id)
    };
    ret = pthread_create(&(thread_data[i++].thread), NULL, nfqueue_thread, (void *) &thread_arg_wan_https_to_domain_tplinkapi);
    assert(ret == 0);
    
    /* wan-https-to-domain-tplinkapi-backward */
    // Setup thread-specific data
    thread_data[i].id = i;
    thread_data[i].seed = time(NULL) + i;
    thread_arg_t thread_arg_wan_https_to_domain_tplinkapi_backward = {
        .queue_id = 11,
        .func = &callback_wan_https_to_domain_tplinkapi_backward,
        .arg = &(thread_data[i].id)
    };
    ret = pthread_create(&(thread_data[i++].thread), NULL, nfqueue_thread, (void *) &thread_arg_wan_https_to_domain_tplinkapi_backward);
    assert(ret == 0);
    
    // Wait forever for threads
    for (i = 0; i < NUM_THREADS; i++) {
        pthread_join(thread_data[i++].thread, NULL);
    }


    /* FREE MEMORY */

    // Free DNS map
    dns_map_free(dns_map);

    return 0;
}
