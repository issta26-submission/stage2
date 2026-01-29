#include <cstddef>
#include <pcap/bluetooth.h>
#include <pcap/can_socketcan.h>
#include <pcap/compiler-tests.h>
#include <pcap/dlt.h>
#include <pcap/funcattrs.h>
#include <pcap/ipnet.h>
#include <pcap/nflog.h>
#include <pcap/pcap-inttypes.h>
#include <pcap/pcap.h>
#include <pcap/sll.h>
#include <pcap/socket.h>
#include <pcap/usb.h>
#include <pcap/vlan.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <vector>
#include <fstream>
#include <iostream>
#include <sstream>
#include <cstring>
#include <fcntl.h>
#include <cassert>

int test_api_sequence() {
    // Step 1: Declarations / Initialize
    char errbuf[PCAP_ERRBUF_SIZE];
    memset(errbuf, 0, sizeof(errbuf));
    
    const char *device = "lo";
    bpf_u_int32 net = 0;
    bpf_u_int32 mask = 0;
    int lookup_ret = pcap_lookupnet(device, &net, &mask, errbuf);
    pcap_t *handle = pcap_create(device, errbuf);
    assert(handle != NULL); 
    // Step 2: Configure
    int snap_ret = pcap_set_snaplen(handle, 65535);
    assert(snap_ret == 0);
    int promisc_ret = pcap_set_promisc(handle, 1);
    assert(promisc_ret == 0);
    int timeout_ret = pcap_set_timeout(handle, 1000);
    assert(timeout_ret == 0);
    int buf_ret = pcap_set_buffer_size(handle, 2 * 1024 * 1024);
    assert(buf_ret == 0);
    int activate_ret = pcap_activate(handle);
    assert(activate_ret >= 0); 
    // Step 3: Operate / Validate
    int snap = pcap_snapshot(handle);
    assert(snap > 0);
    int dlt = pcap_datalink(handle);
    assert(dlt >= 0);
    struct bpf_program prog;
    memset(&prog, 0, sizeof(prog));
    const char *filter_exp = "ip";
    int compile_ret = pcap_compile(handle, &prog, filter_exp, 1, net);
    assert(compile_ret == 0);
    assert(prog.bf_insns != NULL);
    int setfilter_ret = pcap_setfilter(handle, &prog);
    assert(setfilter_ret == 0);
    const char* dump_file = "/tmp/test_libpcap_api_sequence.pcap";
    pcap_dumper_t *dumper = pcap_dump_open(handle, dump_file);
    assert(dumper != NULL);
    struct pcap_pkthdr hdr;
    memset(&hdr, 0, sizeof(hdr));
    hdr.caplen = (bpf_u_int32)snap;
    hdr.len = (bpf_u_int32)snap;
    hdr.ts.tv_sec = 1735689600;
    hdr.ts.tv_usec = 0;
    memset(packet, 0xAA, sizeof(packet));
    pcap_dump((u_char *)dumper, &hdr, packet);
    int flush_ret = pcap_dump_flush(dumper);
    assert(flush_ret == 0);
    long file_pos = pcap_dump_ftell(dumper);
    assert(file_pos > 24); 
    // Step 4: Cleanup
    pcap_dump_close(dumper);
    pcap_freecode(&prog);
    pcap_close(handle);
    FILE* f = fopen(dump_file, "r");
    assert(f != NULL);

    return 66;
}

int main() {
    // Only running once to keep output clean, but loop is fine for stress testing
    for (int i = 0; i < 1; i++) {
        int result = test_api_sequence();
        if (result != 66) {
            std::cerr << "API sequence test failed with error code: " << result << std::endl;
            exit(100);
        }
    }
    printf("Test completed successfully.\n");
    return 0;
}