#ifndef MONGOOSE_H
#define MONGOOSE_H

#define MG_VERSION "7.9"

#ifdef __cplusplus
extern "C" {
#endif


#define MG_ARCH_CUSTOM 0
#define MG_ARCH_UNIX 1
#define MG_ARCH_WIN32 2
#define MG_ARCH_ESP32 3     
#define MG_ARCH_ESP8266 4   
#define MG_ARCH_FREERTOS 5  
#define MG_ARCH_AZURERTOS 6  
#define MG_ARCH_ZEPHYR 7  
#define MG_ARCH_NEWLIB 8  
#define MG_ARCH_RTX 9
#define MG_ARCH_TIRTOS 10
#define MG_ARCH_RP2040 11
#define MG_ARCH_CCUO 11

#if !defined(MG_ARCH)
#if defined(__unix__) || defined(__APPLE__)
#define MG_ARCH MG_ARCH_UNIX
#elif defined(_WIN32)
#define MG_ARCH MG_ARCH_WIN32
#elif defined(ICACHE_FLASH) || defined(ICACHE_RAM_ATTR)
#define MG_ARCH MG_ARCH_ESP8266
#elif defined(__ZEPHYR__)
#define MG_ARCH MG_ARCH_ZEPHYR
#elif defined(ESP_PLATFORM)
#define MG_ARCH MG_ARCH_ESP32
#elif defined(FREERTOS_IP_H)
#define MG_ARCH MG_ARCH_FREERTOS
#define MG_ENABLE_FREERTOS_TCP 1
#elif defined(AZURE_RTOS_THREADX)
#define MG_ARCH MG_ARCH_AZURERTOS
#elif defined(PICO_TARGET_NAME)
#define MG_ARCH MG_ARCH_RP2040
#endif
#endif 

#if !defined(MG_ARCH) || (MG_ARCH == MG_ARCH_CUSTOM)
#include "mongoose_custom.h"  
#endif

#if !defined(MG_ARCH)
#error "MG_ARCH is not specified and we couldn't guess it. Set -D MG_ARCH=..."
#endif


#define MG_BIG_ENDIAN (*(uint16_t *) "\0\xff" < 0x100)


#if MG_ARCH == MG_ARCH_AZURERTOS

#include <stdarg.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <time.h>

#include <fx_api.h>
#include <tx_api.h>

#include <nx_api.h>
#include <nx_bsd.h>
#include <nx_port.h>
#include <tx_port.h>

#define PATH_MAX FX_MAXIMUM_PATH
#define MG_DIRSEP '\\'

#define socklen_t int
#define closesocket(x) soc_close(x)

#undef FOPEN_MAX

#endif


#if MG_ARCH == MG_ARCH_ESP32

//#include <ctype.h>
#include <dirent.h>
#include <errno.h>
#include <fcntl.h>
#include <limits.h>
#include <netdb.h>
#include <stdarg.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <time.h>

#include <esp_timer.h>

#define MG_PATH_MAX 128

#endif


#if MG_ARCH == MG_ARCH_ESP8266

//#include <ctype.h>
#include <dirent.h>
#include <errno.h>
#include <fcntl.h>
#include <limits.h>
#include <netdb.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <sys/types.h>
#include <time.h>

#include <esp_system.h>

#define MG_PATH_MAX 128

#endif


#if MG_ARCH == MG_ARCH_FREERTOS

//#include <ctype.h>

#include <stdarg.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h> 
#include <string.h>
#include <sys/stat.h>

#include <FreeRTOS.h>
#include <task.h>

#ifndef MG_IO_SIZE
#define MG_IO_SIZE 512
#endif

#define calloc(a, b) mg_calloc(a, b)
#define free(a) vPortFree(a)
#define malloc(a) pvPortMalloc(a)
#define strdup(s) ((char *) mg_strdup(mg_str(s)).ptr)


static inline void *mg_calloc(size_t cnt, size_t size) {
  void *p = pvPortMalloc(cnt * size);
  if (p != NULL) memset(p, 0, size * cnt);
  return p;
}

#define mkdir(a, b) mg_mkdir(a, b)
static inline int mg_mkdir(const char *path, mode_t mode) {
  (void) path, (void) mode;
  return -1;
}

#endif  


#if MG_ARCH == MG_ARCH_NEWLIB
#define _POSIX_TIMERS

//#include <ctype.h>
#include <errno.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <sys/types.h>
#include <time.h>
#include <unistd.h>

#define MG_PATH_MAX 100
#define MG_ENABLE_SOCKET 0
#define MG_ENABLE_DIRLIST 0

#endif


#if MG_ARCH == MG_ARCH_RP2040
#include <errno.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include <pico/stdlib.h>
int mkdir(const char *, mode_t);
#endif


#if MG_ARCH == MG_ARCH_RTX


#if !defined MG_ENABLE_RL && (!defined(MG_ENABLE_LWIP) || !MG_ENABLE_LWIP)
#define MG_ENABLE_RL 1
#endif

#endif


#if MG_ARCH == MG_ARCH_TIRTOS

#include <stdlib.h>
//#include <ctype.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

#include <serrno.h>
#include <sys/socket.h>

#include <ti/sysbios/knl/Clock.h>

#endif


#if MG_ARCH == MG_ARCH_UNIX

#define _DARWIN_UNLIMITED_SELECT 1  

#if defined(__APPLE__)
#include <mach/mach_time.h>
#endif

#if !defined(MG_ENABLE_EPOLL) && defined(__linux__)
#define MG_ENABLE_EPOLL 1
#elif !defined(MG_ENABLE_POLL)
#define MG_ENABLE_POLL 1
#endif

#include <arpa/inet.h>
//#include <ctype.h>
#include <dirent.h>
#include <errno.h>
#include <fcntl.h>
#include <inttypes.h>
#include <limits.h>
#include <netdb.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <signal.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#if defined(MG_ENABLE_EPOLL) && MG_ENABLE_EPOLL
#include <sys/epoll.h>
#elif defined(MG_ENABLE_POLL) && MG_ENABLE_POLL
#include <poll.h>
#else
#include <sys/select.h>
#endif

#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <sys/types.h>
#include <time.h>
#include <unistd.h>

#ifndef MG_ENABLE_DIRLIST
#define MG_ENABLE_DIRLIST 1
#endif

#ifndef MG_PATH_MAX
#define MG_PATH_MAX FILENAME_MAX
#endif

#endif


#if MG_ARCH == MG_ARCH_WIN32

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

#ifndef _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#endif

#ifndef _WINSOCK_DEPRECATED_NO_WARNINGS
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#endif

//#include <ctype.h>
#include <direct.h>
#include <errno.h>
#include <fcntl.h>
#include <limits.h>
#include <signal.h>
#include <stdarg.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <time.h>

#if defined(_MSC_VER) && _MSC_VER < 1700
#define __func__ ""
typedef __int64 int64_t;
typedef unsigned __int64 uint64_t;
typedef unsigned char uint8_t;
typedef char int8_t;
typedef unsigned short uint16_t;
typedef short int16_t;
typedef unsigned int uint32_t;
typedef int int32_t;
typedef enum { false = 0, true = 1 } bool;
#else
#include <stdbool.h>
#include <stdint.h>
#include <ws2tcpip.h>
#endif

#include <process.h>
#include <winerror.h>
#include <winsock2.h>


#ifndef __cplusplus
#define snprintf _snprintf
#define vsnprintf _vsnprintf
#ifndef strdup  
#define strdup(x) _strdup(x)
#endif
#endif

#define MG_INVALID_SOCKET INVALID_SOCKET
#define MG_SOCKET_TYPE SOCKET
typedef unsigned long nfds_t;
#if defined(_MSC_VER)
#pragma comment(lib, "ws2_32.lib")
#ifndef alloca
#define alloca(a) _alloca(a)
#endif
#endif
#define poll(a, b, c) WSAPoll((a), (b), (c))
#define closesocket(x) closesocket(x)

typedef int socklen_t;
#define MG_DIRSEP '\\'

#ifndef MG_PATH_MAX
#define MG_PATH_MAX FILENAME_MAX
#endif

#ifndef SO_EXCLUSIVEADDRUSE
#define SO_EXCLUSIVEADDRUSE ((int) (~SO_REUSEADDR))
#endif

#define MG_SOCK_ERR(errcode) ((errcode) < 0 ? WSAGetLastError() : 0)

#define MG_SOCK_PENDING(errcode)                                            \
  (((errcode) < 0) &&                                                       \
   (WSAGetLastError() == WSAEINTR || WSAGetLastError() == WSAEINPROGRESS || \
	WSAGetLastError() == WSAEWOULDBLOCK))

#define MG_SOCK_RESET(errcode) \
  (((errcode) < 0) && (WSAGetLastError() == WSAECONNRESET))

#define realpath(a, b) _fullpath((b), (a), MG_PATH_MAX)
#define sleep(x) Sleep(x)
#define mkdir(a, b) _mkdir(a)

#ifndef S_ISDIR
#define S_ISDIR(x) (((x) &_S_IFMT) == _S_IFDIR)
#endif

#ifndef MG_ENABLE_DIRLIST
#define MG_ENABLE_DIRLIST 1
#endif

#endif


#if MG_ARCH == MG_ARCH_ZEPHYR

#include <zephyr/kernel.h>

//#include <ctype.h>
#include <errno.h>
#include <fcntl.h>
#include <zephyr/net/socket.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <time.h>

#define MG_PUTCHAR(x) printk("%c", x)
#ifndef strdup
#define strdup(s) ((char *) mg_strdup(mg_str(s)).ptr)
#endif
#define strerror(x) zsock_gai_strerror(x)
#define FD_CLOEXEC 0
#define F_SETFD 0
#define MG_ENABLE_SSI 0

int rand(void);
int sscanf(const char *, const char *, ...);

#endif


#if defined(MG_ENABLE_FREERTOS_TCP) && MG_ENABLE_FREERTOS_TCP

//#include <ctype.h>
#include <errno.h>
#include <limits.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <time.h>

#include <FreeRTOS.h>
#include <list.h>
#include <task.h>

#include <FreeRTOS_IP.h>
#include <FreeRTOS_Sockets.h>

#define MG_SOCKET_TYPE Socket_t
#define MG_INVALID_SOCKET FREERTOS_INVALID_SOCKET


#define IPPROTO_TCP FREERTOS_IPPROTO_TCP
#define IPPROTO_UDP FREERTOS_IPPROTO_UDP
#define AF_INET FREERTOS_AF_INET
#define SOCK_STREAM FREERTOS_SOCK_STREAM
#define SOCK_DGRAM FREERTOS_SOCK_DGRAM
#define SO_BROADCAST 0
#define SO_ERROR 0
#define SOL_SOCKET 0
#define SO_REUSEADDR 0
#define sockaddr_in freertos_sockaddr
#define sockaddr freertos_sockaddr
#define accept(a, b, c) FreeRTOS_accept((a), (b), (c))
#define connect(a, b, c) FreeRTOS_connect((a), (b), (c))
#define bind(a, b, c) FreeRTOS_bind((a), (b), (c))
#define listen(a, b) FreeRTOS_listen((a), (b))
#define socket(a, b, c) FreeRTOS_socket((a), (b), (c))
#define send(a, b, c, d) FreeRTOS_send((a), (b), (c), (d))
#define recv(a, b, c, d) FreeRTOS_recv((a), (b), (c), (d))
#define setsockopt(a, b, c, d, e) FreeRTOS_setsockopt((a), (b), (c), (d), (e))
#define sendto(a, b, c, d, e, f) FreeRTOS_sendto((a), (b), (c), (d), (e), (f))
#define recvfrom(a, b, c, d, e, f) \
  FreeRTOS_recvfrom((a), (b), (c), (d), (e), (f))
#define closesocket(x) FreeRTOS_closesocket(x)
#define gethostbyname(x) FreeRTOS_gethostbyname(x)
#define getsockname(a, b, c) mg_getsockname((a), (b), (c))
#define getpeername(a, b, c) mg_getpeername((a), (b), (c))

static inline int mg_getsockname(MG_SOCKET_TYPE fd, void *buf, socklen_t *len) {
  (void) fd, (void) buf, (void) len;
  return -1;
}

static inline int mg_getpeername(MG_SOCKET_TYPE fd, void *buf, socklen_t *len) {
  (void) fd, (void) buf, (void) len;
  return 0;
}
#endif


#if defined(MG_ENABLE_LWIP) && MG_ENABLE_LWIP
#if defined(__GNUC__)
#include <sys/stat.h>
#include <sys/time.h>
#else
struct timeval {
  time_t tv_sec;
  long tv_usec;
};
#endif

#include <lwip/sockets.h>

#if LWIP_SOCKET != 1
#error Set LWIP_SOCKET variable to 1 (in lwipopts.h)
#endif
#endif


#if defined(MG_ENABLE_RL) && MG_ENABLE_RL


#define MG_ENABLE_CUSTOM_MILLIS 1
#define closesocket(x) closesocket(x)
#define mkdir(a, b) (-1)

#define TCP_NODELAY SO_KEEPALIVE

#define MG_SOCK_ERR(errcode) ((errcode) < 0 ? (errcode) : 0)

#define MG_SOCK_PENDING(errcode)                                \
  ((errcode) == BSD_EWOULDBLOCK || (errcode) == BSD_EALREADY || \
   (errcode) == BSD_EINPROGRESS)

#define MG_SOCK_RESET(errcode) \
  ((errcode) == BSD_ECONNABORTED || (errcode) == BSD_ECONNRESET)

#define MG_SOCK_INTR(fd) 0

#define socklen_t int
#endif


#ifndef MG_ENABLE_LOG
#define MG_ENABLE_LOG 1
#endif

#ifndef MG_ENABLE_TCPIP
#define MG_ENABLE_TCPIP 0  
#endif

#ifndef MG_ENABLE_LWIP
#define MG_ENABLE_LWIP 0  
#endif

#ifndef MG_ENABLE_FREERTOS_TCP
#define MG_ENABLE_FREERTOS_TCP 0  
#endif

#ifndef MG_ENABLE_RL
#define MG_ENABLE_RL 0  
#endif

#ifndef MG_ENABLE_SOCKET
#define MG_ENABLE_SOCKET !MG_ENABLE_TCPIP
#endif

#ifndef MG_ENABLE_POLL
#define MG_ENABLE_POLL 0
#endif

#ifndef MG_ENABLE_EPOLL
#define MG_ENABLE_EPOLL 0
#endif

#ifndef MG_ENABLE_FATFS
#define MG_ENABLE_FATFS 0
#endif

#ifndef MG_ENABLE_MBEDTLS
#define MG_ENABLE_MBEDTLS 0
#endif

#ifndef MG_ENABLE_OPENSSL
#define MG_ENABLE_OPENSSL 0
#endif

#ifndef MG_ENABLE_CUSTOM_TLS
#define MG_ENABLE_CUSTOM_TLS 0
#endif

#ifndef MG_ENABLE_SSI
#define MG_ENABLE_SSI 0
#endif

#ifndef MG_ENABLE_IPV6
#define MG_ENABLE_IPV6 0
#endif

#ifndef MG_ENABLE_MD5
#define MG_ENABLE_MD5 1
#endif


#ifndef MG_ENABLE_WINSOCK
#define MG_ENABLE_WINSOCK 1
#endif

#ifndef MG_ENABLE_DIRLIST
#define MG_ENABLE_DIRLIST 0
#endif

#ifndef MG_ENABLE_CUSTOM_RANDOM
#define MG_ENABLE_CUSTOM_RANDOM 0
#endif

#ifndef MG_ENABLE_CUSTOM_MILLIS
#define MG_ENABLE_CUSTOM_MILLIS 0
#endif

#ifndef MG_ENABLE_PACKED_FS
#define MG_ENABLE_PACKED_FS 0
#endif

#ifndef MG_ENABLE_ASSERT
#define MG_ENABLE_ASSERT 0
#endif

#ifndef MG_IO_SIZE
#define MG_IO_SIZE 2048  
#endif

#ifndef MG_MAX_RECV_SIZE
#define MG_MAX_RECV_SIZE (3 * 1024 * 1024)  
#endif

#ifndef MG_DATA_SIZE
#define MG_DATA_SIZE 32  
#endif

#ifndef MG_MAX_HTTP_HEADERS
#define MG_MAX_HTTP_HEADERS 30
#endif

#ifndef MG_HTTP_INDEX
#define MG_HTTP_INDEX "index.html"
#endif

#ifndef MG_PATH_MAX
#ifdef PATH_MAX
#define MG_PATH_MAX PATH_MAX
#else
#define MG_PATH_MAX 128
#endif
#endif

#ifndef MG_SOCK_LISTEN_BACKLOG_SIZE
#define MG_SOCK_LISTEN_BACKLOG_SIZE 3
#endif

#ifndef MG_DIRSEP
#define MG_DIRSEP '/'
#endif

#ifndef MG_ENABLE_FILE
#if defined(FOPEN_MAX)
#define MG_ENABLE_FILE 1
#else
#define MG_ENABLE_FILE 0
#endif
#endif

#ifndef MG_INVALID_SOCKET
#define MG_INVALID_SOCKET (-1)
#endif

#ifndef MG_SOCKET_TYPE
#define MG_SOCKET_TYPE int
#endif

#ifndef MG_SOCKET_ERRNO
#define MG_SOCKET_ERRNO errno
#endif


#if MG_ENABLE_EPOLL
#define MG_EPOLL_ADD(c)
do {
struct epoll_event ev = {EPOLLIN | EPOLLERR | EPOLLHUP, {c}};
epoll_ctl(c->mgr->epoll_fd, EPOLL_CTL_ADD, (int) (size_t) c->fd, &ev);
} while (0)
#define MG_EPOLL_MOD(c, wr)
do {
struct epoll_event ev = {EPOLLIN | EPOLLERR | EPOLLHUP, {c}};
if (wr) ev.events |= EPOLLOUT;
epoll_ctl(c->mgr->epoll_fd, EPOLL_CTL_MOD, (int) (size_t) c->fd, &ev);
} while (0)
#include <stddef.h>
#else
#define MG_EPOLL_ADD(c)
#define MG_EPOLL_MOD(c, wr)
#endif


struct mg_str 
{
  const char *ptr;  
  size_t len;
};

#define MG_NULL_STR \
  { NULL, 0 }

#define MG_C_STR(a) \
  { (a), sizeof(a) - 1 }


#define mg_str(s) mg_str_s(s)


struct mg_str mg_str(const char *s);
struct mg_str mg_str_n(const char *s; size_t n);
int mg_lower(const char *s);
int mg_ncasecmp(const char *s1, const char *s2, size_t len);
int mg_casecmp(const char *s1, const char *s2);
int mg_vcmp(const struct mg_str *s1, const char *s2);
int mg_vcasecmp(const struct mg_str *str1, const char *str2);
int mg_strcmp(const struct mg_str str1, const struct mg_str str2);
struct mg_str mg_strstrip(struct mg_str s);
struct mg_str mg_strdup(const struct mg_str s);
const char *mg_strstr(const struct mg_str haystack, const struct mg_str needle);
bool mg_match(struct mg_str str, struct mg_str pattern, struct mg_str *caps);
bool mg_globmatch(const char *pattern, size_t plen, const char *s, size_t n);
bool mg_commalist(struct mg_str *s, struct mg_str *k, struct mg_str *v);
bool mg_split(struct mg_str *s, struct mg_str *k, struct mg_str *v, char delim);
char *mg_hex(const void *buf, size_t len, char *dst);
void mg_unhex(const char *buf, size_t len, unsigned char *to);
unsigned long mg_unhexn(const char *s, size_t len);
int mg_check_ip_acl(struct mg_str acl, uint32_t remote_ip);
int64_t mg_to64(struct mg_str str);
uint64_t mg_tou64(struct mg_str str);
char *mg_remove_double_dots(char *s);


struct mg_queue {
  char *buf;
  size_t size;
  volatile size_t tail;
  volatile size_t head;
};

void mg_queue_init(struct mg_queue *, char *, size_t);        
size_t mg_queue_book(struct mg_queue *, char **buf, size_t);  
void mg_queue_add(struct mg_queue *, size_t);                 
size_t mg_queue_next(struct mg_queue *, char **);  
void mg_queue_del(struct mg_queue *, size_t);      

typedef void (*mg_pfn_t)(char, void *);                 
typedef size_t (*mg_pm_t)(mg_pfn_t, void *, va_list *);

size_t mg_vxprintf(void (*)(char, void *), void *, const char *fmt, va_list *);
size_t mg_xprintf(void (*fn)(char, void *), void *, const char *fmt, ...);


size_t mg_vsnprintf(char *buf, size_t len, const char *fmt, va_list *ap);
size_t mg_snprintf(char *, size_t, const char *fmt, ...);
char *mg_vmprintf(const char *fmt, va_list *ap);
char *mg_mprintf(const char *fmt, ...);
size_t mg_queue_vprintf(struct mg_queue *, const char *fmt, va_list *);
size_t mg_queue_printf(struct mg_queue *, const char *fmt, ...);


size_t mg_print_ip(void (*out)(char, void *), void *arg, va_list *ap);
size_t mg_print_ip_port(void (*out)(char, void *), void *arg, va_list *ap);
size_t mg_print_ip4(void (*out)(char, void *), void *arg, va_list *ap);
size_t mg_print_ip6(void (*out)(char, void *), void *arg, va_list *ap);
size_t mg_print_mac(void (*out)(char, void *), void *arg, va_list *ap);


void mg_pfn_iobuf(char ch, void *param);  
void mg_pfn_stdout(char c, void *param);  



enum { MG_LL_NONE, MG_LL_ERROR, MG_LL_INFO, MG_LL_DEBUG, MG_LL_VERBOSE };
void mg_log(const char *fmt, ...);
bool mg_log_prefix(int ll, const char *file, int line, const char *fname);
void mg_log_set(int log_level);
void mg_hexdump(const void *buf, size_t len);
void mg_log_set_fn(mg_pfn_t fn, void *param);

#if MG_ENABLE_LOG
#define MG_LOG(level, args)                                                \
  do {                                                                     \
	if (mg_log_prefix((level), __FILE__, __LINE__, __func__)) mg_log args; \
  } while (0)
#else
#define MG_LOG(level, args) \
  do {                      \
	if (0) mg_log args;     \
  } while (0)
#endif

#define MG_ERROR(args) MG_LOG(MG_LL_ERROR, args)
#define MG_INFO(args) MG_LOG(MG_LL_INFO, args)
#define MG_DEBUG(args) MG_LOG(MG_LL_DEBUG, args)
#define MG_VERBOSE(args) MG_LOG(MG_LL_VERBOSE, args)




struct mg_timer {
  unsigned long id;         
  uint64_t period_ms;      
  uint64_t expire;          
  unsigned flags;           
#define MG_TIMER_ONCE 0     
#define MG_TIMER_REPEAT 1   
#define MG_TIMER_RUN_NOW 2  
  void (*fn)(void *);       
  void *arg;                
  struct mg_timer *next;    
};

void mg_timer_init(struct mg_timer **head, struct mg_timer *timer,
				   uint64_t milliseconds, unsigned flags, void (*fn)(void *),
				   void *arg);
void mg_timer_free(struct mg_timer **head, struct mg_timer *);
void mg_timer_poll(struct mg_timer **head, uint64_t new_ms);
bool mg_timer_expired(uint64_t *expiration, uint64_t period, uint64_t now);





enum { MG_FS_READ = 1, MG_FS_WRITE = 2, MG_FS_DIR = 4 };


struct mg_fs {
  int (*st)(const char *path, size_t *size, time_t *mtime);  
  void (*ls)(const char *path, void (*fn)(const char *, void *), void *);
  void *(*op)(const char *path, int flags);             
  void (*cl)(void *fd);                                
  size_t (*rd)(void *fd, void *buf, size_t len);        
  size_t (*wr)(void *fd, const void *buf, size_t len);  
  size_t (*sk)(void *fd, size_t offset);                
  bool (*mv)(const char *from, const char *to);         
  bool (*rm)(const char *path);                         
  bool (*mkd)(const char *path);                        
};

extern struct mg_fs mg_fs_posix;   
extern struct mg_fs mg_fs_packed;  
extern struct mg_fs mg_fs_fat;     


struct mg_fd {
  void *fd;
  struct mg_fs *fs;
};

struct mg_fd *mg_fs_open(struct mg_fs *fs, const char *path, int flags);
void mg_fs_close(struct mg_fd *fd);
char *mg_file_read(struct mg_fs *fs, const char *path, size_t *size);
bool mg_file_write(struct mg_fs *fs, const char *path, const void *, size_t);
bool mg_file_printf(struct mg_fs *fs, const char *path, const char *fmt, ...);



#if MG_ENABLE_ASSERT
#include <assert.h>
#elif !defined(assert)
#define assert(x)
#endif

void mg_random(void *buf, size_t len);
char *mg_random_str(char *buf, size_t len);
uint16_t mg_ntohs(uint16_t net);
uint32_t mg_ntohl(uint32_t net);
uint32_t mg_crc32(uint32_t crc, const char *buf, size_t len);
uint64_t mg_millis(void);

#define mg_htons(x) mg_ntohs(x)
#define mg_htonl(x) mg_ntohl(x)

#define MG_U32(a, b, c, d)                                         \
  (((uint32_t) ((a) &255) << 24) | ((uint32_t) ((b) &255) << 16) | \
   ((uint32_t) ((c) &255) << 8) | (uint32_t) ((d) &255))


#define MG_U8P(ADDR) ((uint8_t *) (ADDR))
#define MG_IPADDR_PARTS(ADDR) \
  MG_U8P(ADDR)[0], MG_U8P(ADDR)[1], MG_U8P(ADDR)[2], MG_U8P(ADDR)[3]


#define LIST_ADD_HEAD(type_, head_, elem_) \
  do {                                     \
	(elem_)->next = (*head_);              \
	*(head_) = (elem_);                    \
  } while (0)

#define LIST_ADD_TAIL(type_, head_, elem_) \
  do {                                     \
	type_ **h = head_;                     \
	while (*h != NULL) h = &(*h)->next;    \
	*h = (elem_);                          \
  } while (0)

#define LIST_DELETE(type_, head_, elem_)   \
  do {                                     \
	type_ **h = head_;                     \
	while (*h != (elem_)) h = &(*h)->next; \
	*h = (elem_)->next;                    \
  } while (0)



unsigned short mg_url_port(const char *url);
int mg_url_is_ssl(const char *url);
struct mg_str mg_url_host(const char *url);
struct mg_str mg_url_user(const char *url);
struct mg_str mg_url_pass(const char *url);
const char *mg_url_uri(const char *url);




struct mg_iobuf {
  unsigned char *buf; 
  size_t size;         
  size_t len;          
  size_t align;        
};

int mg_iobuf_init(struct mg_iobuf *, size_t, size_t);
int mg_iobuf_resize(struct mg_iobuf *, size_t);
void mg_iobuf_free(struct mg_iobuf *);
size_t mg_iobuf_add(struct mg_iobuf *, size_t, const void *, size_t);
size_t mg_iobuf_del(struct mg_iobuf *, size_t ofs, size_t len);

int mg_base64_update(unsigned char p, char *to, int len);
int mg_base64_final(char *to, int len);
int mg_base64_encode(const unsigned char *p, int n, char *to);
int mg_base64_decode(const char *src, int n, char *dst);




typedef struct {
  uint32_t buf[4];
  uint32_t bits[2];
  unsigned char in[64];
} mg_md5_ctx;

void mg_md5_init(mg_md5_ctx *c);
void mg_md5_update(mg_md5_ctx *c, const unsigned char *data, size_t len);
void mg_md5_final(mg_md5_ctx *c, unsigned char[16]);




typedef struct {
  uint32_t state[5];
  uint32_t count[2];
  unsigned char buffer[64];
} mg_sha1_ctx;

void mg_sha1_init(mg_sha1_ctx *);
void mg_sha1_update(mg_sha1_ctx *, const unsigned char *data, size_t len);
void mg_sha1_final(unsigned char digest[20], mg_sha1_ctx *);


struct mg_connection;
typedef void (*mg_event_handler_t)(struct mg_connection *, int ev,
								   void *ev_data, void *fn_data);
void mg_call(struct mg_connection *c, int ev, void *ev_data);
void mg_error(struct mg_connection *c, const char *fmt, ...);

enum {
  MG_EV_ERROR,      
  MG_EV_OPEN,       
  MG_EV_POLL,       
  MG_EV_RESOLVE,    
  MG_EV_CONNECT,    
  MG_EV_ACCEPT,    
  MG_EV_TLS_HS,     
  MG_EV_READ,      
  MG_EV_WRITE,      
  MG_EV_CLOSE,      
  MG_EV_HTTP_MSG,    
  MG_EV_HTTP_CHUNK,  
  MG_EV_WS_OPEN,    
  MG_EV_WS_MSG,     
  MG_EV_WS_CTL,    
  MG_EV_MQTT_CMD,  
  MG_EV_MQTT_MSG,   
  MG_EV_MQTT_OPEN,   
  MG_EV_SNTP_TIME,   
  MG_EV_USER         
};


struct mg_dns {
  const char *url;        
  struct mg_connection *c;  
};

struct mg_addr {
  uint16_t port;   
  uint32_t ip;     
  uint8_t ip6[16]; 
  bool is_ip6;     
};

struct mg_mgr {
  struct mg_connection *conns; 
  struct mg_dns dns4;          
  struct mg_dns dns6;          
  int dnstimeout;              
  bool use_dns6;               
  unsigned long nextid;       
  unsigned long timerid;       
  void *userdata;             
  uint16_t mqtt_id;             
  void *active_dns_requests;   
  struct mg_timer *timers;     
  int epoll_fd;                
  void *priv;                  
  size_t extraconnsize;        
#if MG_ENABLE_FREERTOS_TCP
  SocketSet_t ss;  
#endif
};

struct mg_connection {
  struct mg_connection *next; 
  struct mg_mgr *mgr;     
  struct mg_addr loc;    
  struct mg_addr rem;      
  void *fd;                 
  unsigned long id;        
  struct mg_iobuf recv;    
  struct mg_iobuf send;    
  mg_event_handler_t fn;      
  void *fn_data;               
  mg_event_handler_t pfn;     
  void *pfn_data;             
  char data[MG_DATA_SIZE];  
  void *tls;               
  unsigned is_listening : 1;  
  unsigned is_client : 1;     
  unsigned is_accepted : 1;    
  unsigned is_resolving : 1;   
  unsigned is_arplooking : 1; 
  unsigned is_connecting : 1; 
  unsigned is_tls : 1;      
  unsigned is_tls_hs : 1;     
  unsigned is_udp : 1;         
  unsigned is_websocket : 1;   
  unsigned is_mqtt5 : 1;      
  unsigned is_hexdumping : 1;  
  unsigned is_draining : 1;   
  unsigned is_closing : 1;    
  unsigned is_full : 1;      
  unsigned is_resp : 1;        
  unsigned is_readable : 1;   
  unsigned is_writable : 1;   
};

void mg_mgr_poll(struct mg_mgr *, int ms);
void mg_mgr_init(struct mg_mgr *);
void mg_mgr_free(struct mg_mgr *);

struct mg_connection *mg_listen(struct mg_mgr *, const char *url,
								mg_event_handler_t fn, void *fn_data);
struct mg_connection *mg_connect(struct mg_mgr *, const char *url,
								 mg_event_handler_t fn, void *fn_data);
struct mg_connection *mg_wrapfd(struct mg_mgr *mgr, int fd,
								mg_event_handler_t fn, void *fn_data);
void mg_connect_resolved(struct mg_connection *);
bool mg_send(struct mg_connection *, const void *, size_t);
size_t mg_printf(struct mg_connection *, const char *fmt, ...);
size_t mg_vprintf(struct mg_connection *, const char *fmt, va_list *ap);
bool mg_aton(struct mg_str str, struct mg_addr *addr);
int mg_mkpipe(struct mg_mgr *, mg_event_handler_t, void *, bool udp);


struct mg_connection *mg_alloc_conn(struct mg_mgr *);
void mg_close_conn(struct mg_connection *c);
bool mg_open_listener(struct mg_connection *c, const char *url);


struct mg_timer *mg_timer_add(struct mg_mgr *mgr, uint64_t milliseconds,
							  unsigned flags, void (*fn)(void *), void *arg);


enum { MG_IO_ERR = -1, MG_IO_WAIT = -2, MG_IO_RESET = -3 };
long mg_io_send(struct mg_connection *c, const void *buf, size_t len);
long mg_io_recv(struct mg_connection *c, void *buf, size_t len);


struct mg_http_header {
  struct mg_str name;   
  struct mg_str value;  
};

struct mg_http_message {
  struct mg_str method, uri, query, proto;             
  struct mg_http_header headers[MG_MAX_HTTP_HEADERS]; 
  struct mg_str body;                       
  struct mg_str head;                     
  struct mg_str chunk;   
  struct mg_str message;  
};


struct mg_http_serve_opts {
  const char *root_dir;     
  const char *ssi_pattern;   
  const char *extra_headers; 
  const char *mime_types;    
  const char *page404;       
  struct mg_fs *fs;     
};

struct mg_http_part {
  struct mg_str name;    
  struct mg_str filename;  
  struct mg_str body;   
};

int mg_http_parse(const char *s, size_t len, struct mg_http_message *);
int mg_http_get_request_len(const unsigned char *buf, size_t buf_len);
void mg_http_printf_chunk(struct mg_connection *cnn, const char *fmt, ...);
void mg_http_write_chunk(struct mg_connection *c, const char *buf, size_t len);
void mg_http_delete_chunk(struct mg_connection *c, struct mg_http_message *hm);
struct mg_connection *mg_http_listen(struct mg_mgr *, const char *url,
									 mg_event_handler_t fn, void *fn_data);
struct mg_connection *mg_http_connect(struct mg_mgr *, const char *url,
									  mg_event_handler_t fn, void *fn_data);
void mg_http_serve_dir(struct mg_connection *, struct mg_http_message *hm,
					   const struct mg_http_serve_opts *);
void mg_http_serve_file(struct mg_connection *, struct mg_http_message *hm,
						const char *path, const struct mg_http_serve_opts *);
void mg_http_reply(struct mg_connection *, int status_code, const char *headers,
				   const char *body_fmt, ...);
struct mg_str *mg_http_get_header(struct mg_http_message *, const char *name);
struct mg_str mg_http_var(struct mg_str buf, struct mg_str name);
int mg_http_get_var(const struct mg_str *, const char *name, char *, size_t);
int mg_url_decode(const char *s, size_t n, char *to, size_t to_len, int form);
size_t mg_url_encode(const char *s, size_t n, char *buf, size_t len);
void mg_http_creds(struct mg_http_message *, char *, size_t, char *, size_t);
bool mg_http_match_uri(const struct mg_http_message *, const char *glob);
long mg_http_upload(struct mg_connection *c, struct mg_http_message *hm,
					struct mg_fs *fs, const char *path, size_t max_size);
void mg_http_bauth(struct mg_connection *, const char *user, const char *pass);
struct mg_str mg_http_get_header_var(struct mg_str s, struct mg_str v);
size_t mg_http_next_multipart(struct mg_str, size_t, struct mg_http_part *);
int mg_http_status(const struct mg_http_message *hm);
void mg_hello(const char *url);


void mg_http_serve_ssi(struct mg_connection *c, const char *root,
					   const char *fullpath);


struct mg_tls_opts {
  const char *ca;      
  const char *crl;     
  const char *cert;      
  const char *certkey;   
  const char *ciphers;   
  struct mg_str srvname;  
  struct mg_fs *fs;      
};

void mg_tls_init(struct mg_connection *, const struct mg_tls_opts *);
void mg_tls_free(struct mg_connection *);
long mg_tls_send(struct mg_connection *, const void *buf, size_t len);
long mg_tls_recv(struct mg_connection *, void *buf, size_t len);
size_t mg_tls_pending(struct mg_connection *);
void mg_tls_handshake(struct mg_connection *);


#if MG_ENABLE_MBEDTLS
#include <mbedtls/debug.h>
#include <mbedtls/net_sockets.h>
#include <mbedtls/ssl.h>

struct mg_tls {
  char *cafile;           
  mbedtls_x509_crt ca;     
  mbedtls_x509_crt cert;    
  mbedtls_ssl_context ssl;  
  mbedtls_ssl_config conf;  
  mbedtls_pk_context pk;   
};
#endif


#if MG_ENABLE_OPENSSL

#include <openssl/err.h>
#include <openssl/ssl.h>

struct mg_tls {
  SSL_CTX *ctx;
  SSL *ssl;
};
#endif


#define WEBSOCKET_OP_CONTINUE 0
#define WEBSOCKET_OP_TEXT 1
#define WEBSOCKET_OP_BINARY 2
#define WEBSOCKET_OP_CLOSE 8
#define WEBSOCKET_OP_PING 9
#define WEBSOCKET_OP_PONG 10



struct mg_ws_message {
  struct mg_str data;  
  uint8_t flags;     
};

struct mg_connection *mg_ws_connect(struct mg_mgr *, const char *url,
									mg_event_handler_t fn, void *fn_data,
									const char *fmt, ...);
void mg_ws_upgrade(struct mg_connection *, struct mg_http_message *,
				   const char *fmt, ...);
size_t mg_ws_send(struct mg_connection *, const void *buf, size_t len, int op);
size_t mg_ws_wrap(struct mg_connection *, size_t len, int op);
size_t mg_ws_printf(struct mg_connection *c, int op, const char *fmt, ...);
size_t mg_ws_vprintf(struct mg_connection *c, int op, const char *fmt,
					 va_list *);




struct mg_connection *mg_sntp_connect(struct mg_mgr *mgr, const char *url,
									  mg_event_handler_t fn, void *fn_data);
void mg_sntp_request(struct mg_connection *c);
int64_t mg_sntp_parse(const unsigned char *buf, size_t len);





#define MQTT_CMD_CONNECT 1
#define MQTT_CMD_CONNACK 2
#define MQTT_CMD_PUBLISH 3
#define MQTT_CMD_PUBACK 4
#define MQTT_CMD_PUBREC 5
#define MQTT_CMD_PUBREL 6
#define MQTT_CMD_PUBCOMP 7
#define MQTT_CMD_SUBSCRIBE 8
#define MQTT_CMD_SUBACK 9
#define MQTT_CMD_UNSUBSCRIBE 10
#define MQTT_CMD_UNSUBACK 11
#define MQTT_CMD_PINGREQ 12
#define MQTT_CMD_PINGRESP 13
#define MQTT_CMD_DISCONNECT 14
#define MQTT_CMD_AUTH 15

enum { MQTT_OK, MQTT_INCOMPLETE, MQTT_MALFORMED };

struct mg_mqtt_opts {
  struct mg_str user;          
  struct mg_str pass;        
  struct mg_str client_id;    
  struct mg_str will_topic;   
  struct mg_str will_message;  
  uint8_t will_qos;           
  uint8_t version;            
  uint16_t keepalive;         
  bool will_retain;            
  bool clean;                 
};

struct mg_mqtt_message {
  struct mg_str topic;  
  struct mg_str data;   
  struct mg_str dgram;  
  uint16_t id;  
  uint8_t cmd;  
  uint8_t qos;  
  uint8_t ack;  
};

struct mg_connection *mg_mqtt_connect(struct mg_mgr *, const char *url,
									  const struct mg_mqtt_opts *opts,
									  mg_event_handler_t fn, void *fn_data);
struct mg_connection *mg_mqtt_listen(struct mg_mgr *mgr, const char *url,
									 mg_event_handler_t fn, void *fn_data);
void mg_mqtt_login(struct mg_connection *c, const struct mg_mqtt_opts *opts);
void mg_mqtt_pub(struct mg_connection *c, struct mg_str topic,
				 struct mg_str data, int qos, bool retain);
void mg_mqtt_sub(struct mg_connection *, struct mg_str topic, int qos);
int mg_mqtt_parse(const uint8_t *; size_t; uint8_t; struct mg_mqtt_message; *);
void mg_mqtt_send_header(struct mg_connection *, uint8_t cmd, uint8_t flags,
						 uint32_t len);
void mg_mqtt_ping(struct mg_connection *);
void mg_mqtt_pong(struct mg_connection *);
void mg_mqtt_disconnect(struct mg_connection *);



struct mg_dns_message {
  uint16_t txnid;
  bool resolved;     
  struct mg_addr addr;  
  char name[256];       

struct mg_dns_header {
  uint16_t txnid;  
  uint16_t flags;
  uint16_t num_questions;
  uint16_t num_answers;
  uint16_t num_authority_prs;
  uint16_t num_other_prs;
};


struct mg_dns_rr {
  uint16_t nlen;    
  uint16_t atype;   
  uint16_t aclass;  
  uint16_t alen;  
};

void mg_resolve(struct mg_connection *, const char *url);
void mg_resolve_cancel(struct mg_connection *);
bool mg_dns_parse(const uint8_t *buf, size_t len, struct mg_dns_message *);
size_t mg_dns_parse_rr(const uint8_t *buf, size_t len, size_t ofs,
					   bool is_question, struct mg_dns_rr *);





#ifndef MG_JSON_MAX_DEPTH
#define MG_JSON_MAX_DEPTH 30
#endif


enum { MG_JSON_TOO_DEEP = -1, MG_JSON_INVALID = -2, MG_JSON_NOT_FOUND = -3 };
int mg_json_get(struct mg_str json, const char *path, int *toklen);

bool mg_json_get_num(struct mg_str json, const char *path, double *v);
bool mg_json_get_bool(struct mg_str json, const char *path, bool *v);
long mg_json_get_long(struct mg_str json, const char *path, long dflt);
char *mg_json_get_str(struct mg_str json, const char *path);
char *mg_json_get_hex(struct mg_str json, const char *path, int *len);
char *mg_json_get_b64(struct mg_str json, const char *path, int *len);


struct mg_rpc_req {
  struct mg_rpc **head;  
  struct mg_rpc *rpc;  
  mg_pfn_t pfn;        
  void *pfn_data;       
  void *req_data;       
  struct mg_str frame;   
};

struct mg_rpc {
  struct mg_rpc *next;             
  struct mg_str method;          
  void (*fn)(struct mg_rpc_req *);  
  void *fn_data;                    
};

void mg_rpc_add(struct mg_rpc **head, struct mg_str method_pattern,
				void (*handler)(struct mg_rpc_req *), void *handler_data);
void mg_rpc_del(struct mg_rpc **head, void (*handler)(struct mg_rpc_req *));
void mg_rpc_process(struct mg_rpc_req *);

void mg_rpc_ok(struct mg_rpc_req *, const char *fmt, ...);
void mg_rpc_vok(struct mg_rpc_req *, const char *fmt, va_list *ap);
void mg_rpc_err(struct mg_rpc_req *, int code, const char *fmt, ...);
void mg_rpc_verr(struct mg_rpc_req *, int code, const char *fmt, va_list *);
void mg_rpc_list(struct mg_rpc_req *r);


struct mg_tcpip_if; 

struct mg_tcpip_driver {
  bool (*init)(struct mg_tcpip_if *);           
  size_t (*tx)(const void *, size_t, struct mg_tcpip_if *);  
  size_t (*rx)(void *buf, size_t len, struct mg_tcpip_if *);  
  bool (*up)(struct mg_tcpip_if *);                    
};
struct queue {
  uint8_t *buf;
  size_t len;
  volatile size_t tail, head;
};

struct mg_tcpip_if {
  uint8_t mac[6];
  uint32_t ip, mask, gw;         
  struct mg_str rx;                
  struct mg_str tx;              
  bool enable_dhcp_client;        
  bool enable_dhcp_server;       
  struct mg_tcpip_driver *driver;  
  void *driver_data;              
  struct mg_mgr *mgr;            
  struct queue queue;              

  uint8_t gwmac[6];
  uint64_t now;      
  uint64_t timer_1000ms; 
  uint64_t lease_expire;    
  uint16_t eport;           
  volatile uint32_t ndrop;  
  volatile uint32_t nrecv;  
  volatile uint32_t nsent;  
  volatile uint32_t nerr;   
  uint8_t state;            
#define MIP_STATE_DOWN 0    
#define MIP_STATE_UP 1    
#define MIP_STATE_READY 2  
};

void mg_tcpip_init(struct mg_mgr *, struct mg_tcpip_if *);
void mg_tcpip_free(struct mg_tcpip_if *);
void mg_tcpip_qwrite(void *buf, size_t len, struct mg_tcpip_if *ifp);
size_t mg_tcpip_qread(void *buf, struct mg_tcpip_if *ifp);

size_t mg_tcpip_driver_rx(void *buf, size_t len, struct mg_tcpip_if *ifp);

extern struct mg_tcpip_driver mg_tcpip_driver_stm32;
extern struct mg_tcpip_driver mg_tcpip_driver_w5500;
extern struct mg_tcpip_driver mg_tcpip_driver_tm4c;
extern struct mg_tcpip_driver mg_tcpip_driver_stm32h;
extern struct mg_tcpip_driver mg_tcpip_driver_imxrt1020;


struct mg_tcpip_spi {
  void *spi;          
  void (*begin)(void *);         
  void (*end)(void *);           
  uint8_t (*txn)(void *, uint8_t); 
};

#if MG_ENABLE_TCPIP
#if !defined(MG_ENABLE_DRIVER_STM32H) && !defined(MG_ENABLE_DRIVER_TM4C)
#define MG_ENABLE_DRIVER_STM32 1
#else
#define MG_ENABLE_DRIVER_STM32 0
#endif
#endif


struct mg_tcpip_driver_imxrt1020_data {
  int mdc_cr;  
};


struct mg_tcpip_driver_stm32_data {
  int mdc_cr; 
};


struct mg_tcpip_driver_stm32h_data {
  int mdc_cr;  
};


struct mg_tcpip_driver_tm4c_data {
  int mdc_cr;  
};

#ifdef __cplusplus
}
#endif
#endif  
