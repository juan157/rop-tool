#ifndef DEF_ROPC_UTILS_H
#define DEF_ROPC_UTILS_H

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <inttypes.h>
#include <errno.h>

#include <ctype.h>
#include <getopt.h>
#include <limits.h>
#include <assert.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <sys/mman.h>

/* =========================================================================
   R_UTILS_COLOR_RESET   : a constant used to recover color state
   R_UTILS_FG_COLOR_*    : foreground colors
   R_UTILS_BG_COLOR_*    : background colors
   R_UTILS_PRINT_COLORED : a macro used to print colored (or not) strings
   ======================================================================= */

#define R_UTILS_COLOR_RESET    "\033[m"

#define R_UTILS_FG_COLOR_BLACK    "\033[30m"
#define R_UTILS_FG_COLOR_RED      "\033[31m"
#define R_UTILS_FG_COLOR_GREEN    "\033[32m"
#define R_UTILS_FG_COLOR_YELLOW   "\033[33m"
#define R_UTILS_FG_COLOR_BLUE     "\033[34m"
#define R_UTILS_FG_COLOR_MAGENTA  "\033[35m"
#define R_UTILS_FG_COLOR_CYAN     "\033[36m"
#define R_UTILS_FG_COLOR_WHITE    "\033[37m"

#define R_UTILS_BG_COLOR_BLACK    "\033[40m"
#define R_UTILS_BG_COLOR_RED      "\033[41m"
#define R_UTILS_BG_COLOR_GREEN    "\033[42m"
#define R_UTILS_BG_COLOR_YELLOW   "\033[43m"
#define R_UTILS_BG_COLOR_BLUE     "\033[44m"
#define R_UTILS_BG_COLOR_MAGENTA  "\033[45m"
#define R_UTILS_BG_COLOR_CYAN     "\033[46m"
#define R_UTILS_BG_COLOR_WHITE    "\033[47m"

#define R_UTILS_PRINT_COLORED(color,c_str,...) do {	\
    if(color) {						\
      printf(c_str);					\
      printf(__VA_ARGS__);				\
      printf(R_UTILS_COLOR_RESET);			\
    } else {						\
      printf(__VA_ARGS__);				\
    }							\
  }while(0);


#define R_UTILS_PRINT_WHITE_BG_BLACK(c,...) R_UTILS_PRINT_COLORED(c, R_UTILS_FG_COLOR_WHITE R_UTILS_BG_COLOR_BLACK, __VA_ARGS__)
#define R_UTILS_PRINT_BLACK_BG_WHITE(c,...) R_UTILS_PRINT_COLORED(c, R_UTILS_FG_COLOR_BLACK R_UTILS_BG_COLOR_WHITE, __VA_ARGS__)
#define R_UTILS_PRINT_RED_BG_BLACK(c,...) R_UTILS_PRINT_COLORED(c, R_UTILS_FG_COLOR_RED R_UTILS_BG_COLOR_BLACK, __VA_ARGS__)
#define R_UTILS_PRINT_GREEN_BG_BLACK(c,...) R_UTILS_PRINT_COLORED(c, R_UTILS_FG_COLOR_GREEN R_UTILS_BG_COLOR_BLACK, __VA_ARGS__)
#define R_UTILS_PRINT_YELLOW_BG_BLACK(c,...) R_UTILS_PRINT_COLORED(c, R_UTILS_FG_COLOR_YELLOW R_UTILS_BG_COLOR_BLACK, __VA_ARGS__)
#define R_UTILS_PRINT_BLUE_BG_WHITE(c,...) R_UTILS_PRINT_COLORED(c, R_UTILS_FG_COLOR_BLUE R_UTILS_BG_COLOR_WHITE, __VA_ARGS__)


/* =========================================================================
   WARN display an error message,
   WARNX display an error message, and the error associed to errno
   ERR display an error message & exit
   ERRX display an error message, the error associed to errno & exit
   ======================================================================= */

#define R_UTILS_WARNX(...) do {					\
    fprintf(stderr, "[-] ");					\
    fprintf(stderr, __VA_ARGS__);				\
    fprintf(stderr, " : %s\n", strerror(errno));		\
  }while(0)

#define R_UTILS_WARN(...) do {					\
    fprintf(stderr, "[-] ");					\
    fprintf(stderr, __VA_ARGS__);				\
    fprintf(stderr, "\n");					\
  }while(0)

#define R_UTILS_ERRX(...) do {					\
    R_UTILS_WARNX(__VA_ARGS__);					\
    exit(EXIT_FAILURE);						\
  }while(0)


#define R_UTILS_ERR(...) do {					\
    R_UTILS_WARN(__VA_ARGS__);					\
    exit(EXIT_FAILURE);						\
  }while(0)

#ifndef NDEBUG
#define R_UTILS_DEBUG(...) do {			\
    fprintf(stderr, "[DEBUG] ");		\
    fprintf(stderr, __VA_ARGS__);		\
    fprintf(stderr, "\n");			\
  }while(0)
#else
#define DEBUG(...)
#endif


/* =========================================================================
   ======================================================================= */

typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;

typedef u8 byte_t;
typedef u64 addr_t;
typedef u64 len_t;

void* r_utils_malloc(size_t size);
void* r_utils_calloc(size_t nmemb, size_t size);
void* r_utils_realloc(void *ptr, size_t size);
char* r_utils_strdup(const char *s);
FILE* r_utils_fopen(const char *path, const char *mode);
int r_utils_fseek(FILE *stream, long offset, int whence);
long r_utils_ftell(FILE *stream);
int r_utils_open(const char *path, int oflag);
ssize_t r_utils_read(int fd, void *buf, size_t count);
ssize_t r_utils_write(int fd, const void *buf, size_t count);
void* r_utils_mmap(void *addr, size_t len, int prot, int flags, int fildes, off_t off);
int r_utils_close(int fildes);
int r_utils_fstat(int fildes, struct stat *buf);
pid_t r_utils_fork(void);
int r_utils_execve(const char *path, char *const argv[], char *const envp[]);




int r_utils_add64(u64 *r, u64 a, u64 b);
int r_utils_add32(u32 *r, u32 a, u32 b);
int r_utils_add16(u16 *r, u16 a, u16 b);
int r_utils_mul64(u64 *r, u64 a, u64 b);
int r_utils_mul32(u32 *r, u32 a, u32 b);
int r_utils_mul16(u16 *r, u16 a, u16 b);
int r_utils_sub64(u64 *r, u64 a, u64 b);
int r_utils_sub32(u32 *r, u32 a, u32 b);
int r_utils_sub16(u16 *r, u16 a, u16 b);

/* =========================================================================
   bytes.c
   ======================================================================= */

/* Raw bytes sequence structure */
typedef struct r_utils_bytes {
  u64 len;
  u8 *bytes;
}r_utils_bytes_s;

r_utils_bytes_s* r_utils_bytes_unhexlify(const char *string);
char* r_utils_bytes_hexlify(r_utils_bytes_s *bytes);
void r_utils_free_bytes_seq(r_utils_bytes_s **bytes);
r_utils_bytes_s* r_utils_new_bytes_seq(size_t len);

/* =========================================================================
   misc.c
   ======================================================================= */

int r_utils_dec_to_hexchar(int c);
int r_utils_hexchar_to_dec(int c);
void* r_utils_memsearch(void *src, u64 src_len, void *dst, u64 dst_len);

#endif