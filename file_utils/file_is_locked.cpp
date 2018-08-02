#include <cerrno>
#include <fcntl.h>
#include <cstdio>
#include <cstring>

#include <string>

bool lock_file(const std::string &filename) {
    int fd = open(filename.c_str(), O_WRONLY | O_CREAT, 0600);
    if (fd < 0) {
        return false;
    }
    struct flock flockbuf = { F_WRLCK, 0, SEEK_SET, 0, 0 };
    if (fcntl(fd, F_SETLK, &flockbuf) < 0) {
        return false;
    }
    return true;
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        fprintf(stderr, "usage: %s filename\n", argv[0]);
        return 1;
    }

    if (lock_file(argv[1])) {
        fprintf(stdout, "flock file %s success, is unlocked.\n", argv[1]);
        return 1;
    }
    fprintf(stderr, "flock file %s failed: %s\n", argv[1], strerror(errno));
    return 0;
}
