#include "ropc_search.h"

#define SEARCH_DEFAULT_OPTIONS_STRLEN 6

search_mode_e search_options_mode = SEARCH_MODE_UNDEF;
u8 search_options_byte = 0;
u16 search_options_word = 0;
u32 search_options_dword = 0;
u64 search_options_qword = 0;
r_utils_bytes_s *search_options_string = NULL;
r_utils_bytes_s *search_options_bad = NULL;
int search_options_raw = 0;
int search_options_color = 1;
const char *search_options_filename = "a.out";
int search_options_strlen = SEARCH_DEFAULT_OPTIONS_STRLEN;

void search_help(void) {
  printf("Usage : %s search [OPTIONS] [FILENAME]\n\n", PACKAGE);
  printf("OPTIONS:\n");
  printf("  --all-string, -a    [n]  Search all printable strings of at least [n] caracteres. (default is %d)\n", SEARCH_DEFAULT_OPTIONS_STRLEN);
  printf("  --byte, -b          [b]  Search the byte [b] in binary\n");
  printf("  --bad, -B           [b]  Specify bad chars in address\n");
  printf("  --dword, -d         [d]  Search the dword [d] in binary\n");
  printf("  --help, -h               Print this help message\n");
  printf("  --no-color, -n           Don't colorize output\n");
  printf("  --qword, -q         [q]  Search the qword [q] in binary\n");
  printf("  --raw, -r                Open file in raw mode (don't considere any file format)\n");
  printf("  --split-string, -s  [s]  Search a string \"splited\" in memory (which is not contiguous in memory)\n");
  printf("  --string, -S        [s]  Search a string (a byte sequence) in binary\n");
  printf("  --word, -w          [w]  Search the word [w] in binary\n");
  printf("\n");
}


/* Parse command line options */
void search_options_parse(int argc, char **argv) {
  int opt;

  const struct option opts[] = {
    {"all-string",    optional_argument, NULL, 'a'},
    {"byte",          required_argument, NULL, 'b'},
    {"bad",           required_argument, NULL, 'B'},
    {"dword",         required_argument, NULL, 'd'},
    {"help",          no_argument,       NULL, 'h'},
    {"no-color",      no_argument,       NULL, 'n'},
    {"qword",         required_argument, NULL, 'q'},
    {"raw",           no_argument,       NULL, 'r'},
    {"split-string",  required_argument, NULL, 's'},
    {"string",        required_argument, NULL, 'S'},
    {"word",          required_argument, NULL, 'w'},
    {NULL,            0,                 NULL, 0  }
  };

  while((opt = getopt_long(argc, argv, "a::b:B:d:hnq:rs:S:w:", opts, NULL)) != -1) {
    switch(opt) {

    case 'a':
      search_options_mode = SEARCH_MODE_STRING_ALL;
      if(optarg) search_options_strlen = strtoull(optarg, NULL, 0);
      break;
    case 'b':
      search_options_mode = SEARCH_MODE_BYTE;
      search_options_byte = strtoull(optarg, NULL, 0);
      break;
    case 'B':
      search_options_bad = r_utils_bytes_unhexlify(optarg);
      break;
    case 'd':
      search_options_mode = SEARCH_MODE_DWORD;
      search_options_dword = strtoull(optarg, NULL, 0);
      break;
    case 'h':
      search_help();
      exit(EXIT_FAILURE);
      break;
    case 'n':
      search_options_color = 0;
      break;
    case 'q':
      search_options_mode = SEARCH_MODE_QWORD;
      search_options_qword = strtoull(optarg, NULL, 0);
      break;
    case 'r':
      search_options_raw = 1;
      break;
    case 's':
      search_options_mode = SEARCH_MODE_STRING_SPLIT;
      search_options_string = r_utils_bytes_unhexlify(optarg);
      break;
    case 'S':
      search_options_mode = SEARCH_MODE_STRING;
      search_options_string = r_utils_bytes_unhexlify(optarg);
      break;
    case 'w':
      search_options_mode = SEARCH_MODE_WORD;
      search_options_word = strtoull(optarg, NULL, 0);
      break;

    default:
      search_help();
      exit(EXIT_FAILURE);
    }
  }

  if(optind < argc) {
    search_options_filename = argv[optind];
  }
}

void search_cmd(int argc, char **argv) {
  r_binfmt_s bin;

  search_options_parse(argc, argv);

  r_binfmt_load(&bin, search_options_filename, search_options_raw);

  switch(search_options_mode) {
  case SEARCH_MODE_STRING_ALL:
    search_print_all_string_in_bin(&bin);
    break;

  case SEARCH_MODE_STRING_SPLIT:
    search_print_split_string_in_bin(&bin, search_options_string);
    break;

  case SEARCH_MODE_STRING:
  case SEARCH_MODE_BYTE:
  case SEARCH_MODE_WORD:
  case SEARCH_MODE_DWORD:
  case SEARCH_MODE_QWORD:
    R_UTILS_ERR("This mode isn't yep supported, be patient !");
  default:
    search_help();
    R_UTILS_ERR("I don't know what do you want searching !");
  }
}
