#include"fjorge.h"

size_t recv_response(FILE *sockfp) {
  static char abuf[BUFSIZ] = { 0x00 };
  register size_t acnt = 0, asiz = 0, alen = 0, cnln = 0, flag = 0;

  if(!sockfp) {
    fputs("*** Encountered NULL FILE pointer before attempting to read plaintext response!\n", stderr);

    exit(EX_IOERR);
  }

  while(fgets(abuf, sizeof abuf, sockfp)) {
    if(!strncasecmp(abuf, "content-length:", 15)) {
      register const char *restrict c1 = strchr(abuf, ' ');

      cnln = atoi(++c1);
    }

    if(!strcmp(abuf, "\r\n"))
      flag = 1;

    fputs(abuf, stdout);

    if(!asiz && vcmd->verbose) {
      register char *const s1 = strchr(abuf, ' ');

      if(s1) {
        register char *const s2 = strchr(s1 + 1, ' ');

        if(s2) {
          register long unsigned int acod = 0;

          *s2 = '\0';
   
          acod = strtoul(s1, NULL, 10);

          if(errno != ERANGE)
            fprintf(stderr, "*%%* HTTP response code: %lu\n", acod);
        }
      }
    }

    if(vcmd->output)
      fputs(abuf, vcmd->output);

    if(!asiz && vcmd->brief)
      break;

    acnt = strlen(abuf);
    asiz += acnt;

    if(flag) {
      alen += acnt;

      if(cnln && alen >= cnln)
        break;
    }
  }

  return asiz;
}
