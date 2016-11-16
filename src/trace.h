#ifndef TRACE_H_INCLUDED
#define TRACE_H_INCLUDED

#ifdef DEBUG
    extern void _trace(const char *source, const char *fmt, ...);
    #define TRACE(fmt, ...) _trace(__PRETTY_FUNCTION__,fmt,##__VA_ARGS__)
#else
    #define TRACE(fmt, ...)
#endif

#endif
