#include <argp.h>

int main(int argc, char *argv[]) {
  error_t e = argp_parse(NULL, argc, argv, 0, NULL, NULL);
  return (e != 0) ? 0 : 1;
}
