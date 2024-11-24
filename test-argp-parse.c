#include <argp.h>
#include <assert.h>
#include <stdbool.h>

const char *argp_program_version = "test-argp-parse 1.0";

const char *argp_program_bug_address = "<bug-test-argp-parse@example.com>";

/* Program documentation. */
static char doc[] =
    "test-argp-parse -- Test the argp API with features useful for us.";

static const struct argp_option options[] = {
    {"flag", 'f', NULL, 0, "Set the flag", 0},
    {"include", 'I', "DIR", 0, "Look for include files in DIR", 0},
    {NULL, 0, NULL, 0, NULL, 0}};

struct cli_arguments {
  bool flag;
  char *include_dir;
};

void print_cli_arguments(const struct cli_arguments *const cli) {
  printf("CLI arguments @%p:\n", (void *)cli);
  printf("  * include_dir: %s\n", cli->include_dir);
}

static error_t parse_opt(int key, char *arg, struct argp_state *state) {
  assert(state != NULL);
  struct cli_arguments *cli = state->input;
  assert(cli != NULL);
  switch (key) {

  case 'f':
    cli->flag = true;
    break;

  case 'I':
    cli->include_dir = arg;
    break;

  default:
    return ARGP_ERR_UNKNOWN;
  }

  return 0;
}

/* Our argument parser.  The options, parser, and
   args_doc fields are zero because we have neither options or
   arguments; doc and argp_program_bug_address will be
   used in the output for ‘--help’, and the ‘--version’
   option will print out argp_program_version. */
static struct argp argp = {
    options,   /* argp_options */
    parse_opt, /* arg_parser_t parser function */
    NULL,      /* argp_doc aka usage lines */
    doc,       /* doc */
    NULL,      /* argp_child children */
    NULL,      /* help_filter function */
    NULL,      /* argp_domain for translations */
};

int main(int argc, char *argv[]) {
  printf("Original command line rguments:\n");
  for (int i = 0; i < argc; i++) {
    printf("  %d. %s\n", i, argv[i]);
  }

  struct cli_arguments cli = {false, NULL};

  int first_unparsed = 0;
  error_t ap_err = argp_parse(&argp, argc, argv, 0, &first_unparsed, &cli);

  if (ap_err != 0) {
    switch (ap_err) {
    case EINVAL:
      fprintf(stderr, "Error: Invalid command line found trying to parse the "
                      "command line.\n");
      return 2;
    case ENOMEM:
      fprintf(stderr, "Error: Not enough memory to parse the command line.\n");
      return 2;
    default:
      fprintf(stderr,
              "Error: Unknown error %d trying to parse the command line.\n",
              ap_err);
      return 2;
    }
  }

  printf("Reordered command line rguments after calling argp_parse():\n");
  for (int i = 0; i < argc; i++) {
    printf("  %d. %s\n", i, argv[i]);
  }

  print_cli_arguments(&cli);

  printf("first_unparsed: %d\n", first_unparsed);
  if (first_unparsed < argc) {
    printf("Unparsed CLI arguments:\n");
    for (int i = first_unparsed; i < argc; ++i) {
      printf("  %d. %s\n", i, argv[i]);
    }
  }

  return 0;
}
