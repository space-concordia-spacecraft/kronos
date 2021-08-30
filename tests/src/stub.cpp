extern "C" {

    int _open(const char *name, int flags, int mode) {
        return 0;
    }

    int _gettimeofday( struct timeval *tv, void *tzvp ) {
        return 0;
    }

}


