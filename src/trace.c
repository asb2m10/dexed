#ifdef DEBUG
#include <stdio.h>
#include <stdarg.h>
void _trace(const char *source, const char *fmt, ...) {
    char output[1024];
    va_list argptr;
    va_start(argptr, fmt);
    vsnprintf(output, 1023, fmt, argptr);
    va_end(argptr);
#ifdef FILETRACE
    FILE *fp;
    fp=fopen("/tmp/dexed.log", "a");
    fputs(source, fp);
    fputs(": ", fp);
    fputs(output, fp);
    fputs("\n", fp);
    fclose(fp);
#else
    printf("%s: %s\n",source,output);
#endif
}
#endif
