/**
 * =====================================================================================
 *                                     WireReaper
 *                  Обучающий сниффер сетевых пакетов на языке C для Linux
 * =====================================================================================
 * 
 * Автор: OneWay
 * Стандарт: C99/C11
 * 
 * WireReaper — это низкоуровневый анализатор сетевого трафика (сниффер), написанный без 
 * использования высокоуровневых библиотек захвата пакетов. Он работает напрямую с ядром
 * Linux через сырые сокеты (AF_PACKET).
 * 
 * =====================================================================================
 */

#define _DEFAULT_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <unistd.h>
#include <signal.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <net/if.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <net/ethernet.h>
#include <netinet/ip.h>
#include <netinet/tcp.h>
#include <netinet/udp.h>
#include <ifaddrs.h>

// Цвета для красивого вывода
#define CLR_RST  "\x1b[0m"
#define CLR_RED  "\x1b[31m"
#define CLR_GRN  "\x1b[32m"
#define CLR_YLW  "\x1b[33m"
#define CLR_BLU  "\x1b[34m"
#define CLR_CYN  "\x1b[36m"

static int g_sock_raw = -1;
static struct ifreq g_ifr;
static int g_promisc_enabled = 0;

void PrintBanner(void) {
    printf(CLR_CYN
           "  __          ___                  _____                      \n"
           "  \\ \\        / (_)                |  __ \\                     \n"
           "   \\ \\  /\\  / / _ _ __ ___  ___   | |__) |___  __ _ _ __  ___ _ __ \n"
           "    \\ \\/  \\/ / | | '__/ _ \\/ _ \\  |  _  // _ \\/ _` | '_ \\/ _ \\ '__|\n"
           "     \\  /\\  /  | | | |  __/  __/  | | \\ \\  __/ (_| | |_) |  __/ |   \n"
           "      \\/  \\/   |_|_|  \\___|\\___|  |_|  \\_\\___|\\__,_| .__/ \\___|_|   \n"
           "                                                   | |              \n"
           "                                                   |_|              "
           CLR_RST "\n\n");
    printf("  [+] Автор: OneWay | Low-level C Packet Sniffer\n\n");
}

void handle_sigint(int sig) {
    (void)sig;
    printf("\n\n" CLR_RED "[!]" CLR_RST " Получен сигнал прерывания. Завершение...\n");
    if (g_sock_raw >= 0 && g_promisc_enabled) {
        g_ifr.ifr_flags &= ~IFF_PROMISC;
        ioctl(g_sock_raw, SIOCSIFFLAGS, &g_ifr);
    }
    if (g_sock_raw >= 0) close(g_sock_raw);
    exit(EXIT_SUCCESS);
}

void PrintDataInHex(const uint8_t *buffer, size_t size, size_t offset) {
    if (offset >= size) return;
    const uint8_t *payload = buffer + offset;
    size_t len = size - offset;
    
    printf(CLR_YLW "    [ Payload (%zu bytes) ]" CLR_RST "\n", len);
    for (size_t i = 0; i < len; i += 16) {
        printf("    %04zX: ", i);
        for (size_t j = 0; j < 16; j++) {
            if (i + j < len) printf("%02X ", payload[i + j]);
            else printf("   ");
        }
        printf(" | ");
        for (size_t j = 0; j < 16; j++) {
            if (i + j < len) {
                uint8_t c = payload[i + j];
                printf("%c", (c >= 32 && c <= 126) ? c : '.');
            } else printf(" ");
        }
        printf("\n");
    }
}

void ParseEthernet(const uint8_t *buffer, size_t size, size_t *offset) {
    if (size < sizeof(struct ethhdr)) return;
    struct ethhdr *eth = (struct ethhdr *)buffer;
    *offset = sizeof(struct ethhdr);
    
    printf(CLR_BLU "\n[L2]" CLR_RST " Ethernet: " CLR_GRN "%02X:%02X:%02X:%02X:%02X:%02X" CLR_RST " -> " CLR_GRN "%02X:%02X:%02X:%02X:%02X:%02X" CLR_RST "\n",
           eth->h_source[0], eth->h_source[1], eth->h_source[2], eth->h_source[3], eth->h_source[4], eth->h_source[5],
           eth->h_dest[0], eth->h_dest[1], eth->h_dest[2], eth->h_dest[3], eth->h_dest[4], eth->h_dest[5]);
}

// ... (Остальные функции парсинга сокращены для краткости ответа, принцип тот же) ...

int main(int argc, char *argv[]) {
    PrintBanner();
    if (argc < 2) {
        printf("Использование: sudo %s <интерфейс>\n", argv[0]);
        return EXIT_FAILURE;
    }
    
    // ... (Инициализация сокета и promiscuous mode как раньше) ...

    printf(CLR_GRN "[+]" CLR_RST " WireReaper запущен на интерфейсе: %s\n", argv[1]);
    
    // ... (Цикл приема пакетов) ...
    return 0;
}
