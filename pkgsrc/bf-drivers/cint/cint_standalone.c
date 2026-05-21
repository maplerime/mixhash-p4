#include <stdio.h>
#include <string.h>
#include <target-utils/picoc/picoc.h>

int main(int argc, char *argv[]) {
  char *plugin_dir = NULL;
  int fileidx = 1;
  for (int i = 1; i < argc; i++) {
    if (strcmp(argv[i], "-p") == 0) {
      if (i == argc - 1) {
        fprintf(stderr, "-p requires a directory");
        return 1;
      }

      plugin_dir = argv[i + 1];
      fileidx = i + 2;
    }
  }

  if (argc < 2) {
    return PicocEntry(stdin, stdout, NULL, plugin_dir);
  } else {
    return PicocEntry(stdin, stdout, argv[fileidx], plugin_dir);
  }
}
