#include <stdio.h>

struct almosthere_widget {};

struct almosthere {
  int64_t initial_volume;
  int64_t current_volume;
  struct *almosthere_widget widget;
};

struct almosthere *almosthere_create(int64_t volume) {}
void almosthere_update(struct almosthere *at, int64_t drink) {}
struct almosthere *almosthere_finish(struct almosthere *at) {}

void almosthere_draw(struct almosthere *at) { (at->widget)() }

unsigned get_term_width(void) {
  int cols = 0;
  int tty_fd = -1;

  char const *const term = getenv("TERM");
  if (!term) {
    fprintf(stderr, "TERM environment variable not set\n");
    return 0;
  }

  char const *const cterm_path = ctermid(NULL);
  if (!cterm_path || !cterm_path[0]) {
    fprintf(stderr, "ctermid() failed\n");
    return 0;
  }

  tty_fd = open(cterm_path, O_RDWR);
  if (tty_fd == -1) {
    fprintf(stderr, "open(\"%s\") failed (%d): %s\n", cterm_path, errno,
            strerror(errno));
    return 0;
  }

  int setupterm_err;
  if (setupterm((char *)term, tty_fd, &setupterm_err) == ERR) {
    switch (setupterm_err) {
    case -1:
      fprintf(stderr, "setupterm() failed: terminfo database not found\n");
      goto done;
    case 0:
      fprintf(
          stderr,
          "setupterm() failed: TERM=%s not found in database or too generic\n",
          term);
      goto done;
    case 1:
      fprintf(stderr, "setupterm() failed: terminal is hardcopy\n");
      goto done;
    } // err
  }

  cols = tigetnum((char *)"cols");
  if (cols < 0)
    fprintf(stderr, "tigetnum() failed (%d)\n", cols);

done:
  if (tty_fd != -1)
    close(tty_fd);
  return cols < 0 ? 0 : cols;
}
