#ifndef DEBUG_H_INC
#define DEBUG_H_INC


#ifdef DEB

#define DEBUG_REGIME 1

#else

#define DEBUG_REGIME 0

#endif

#define NEED_SLEEP   0
#define NEED_LINE    0
#define NEED_PID 0


#define DEBPRINT(args...)                       \
    if(DEBUG_REGIME)                            \
    {                                           \
        if (NEED_LINE)                          \
        {                                       \
            if (NEED_PID)                       \
                fprintf(stderr, "\n|[%ld]   ", (long) getpid());    \
            else                                                    \
                fprintf(stderr, "\n|        ");                     \
            fprintf(stderr, "LINE: %d\n"                            \
                            "->", __LINE__);                        \
        }                                                           \
        fprintf(stderr, args);                                      \
    }

#define ERRCHECK_CLOSE(FD)      \
    if (close(FD) != 0)         \
    {                           \
        perror("Close #FD");    \
    }    

#define DEB_SLEEP(x, msg)                               \
    if(NEED_SLEEP)                                      \
    {                                                   \
        fprintf(stderr, msg);                           \
        fprintf(stderr, "\n\nbefore test sleep = %d\n", x);     \
        sleep(x);                                               \
        fprintf(stderr, "after test sleep = %d\n\n", x);        \
    }

#define ERR_HANDLER(msg)    \
    do { perror(msg); exit(EXIT_FAILURE); } while(0);


#endif