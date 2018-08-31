
#include "config.h"
#include <string>
#include "path.h"

namespace bbts {


bool get_user_conf_file(int argc, char *argv[], std::string *conf_file) {
    using bbts::Path;
    if (conf_file == nullptr) {
        return false;
    }

    conf_file->clear();
    for (int i = 0; i < argc; ++i) {
        if (strncmp(argv[i], "--user-conf", strlen("--user-conf")) == 0) {
            if (i + 1 == argc) {
                fprintf(stderr, "use --user-conf, but conf file is empty!\n");
                return false;
            }
            conf_file->assign(Path::trim(Path::absolute(argv[i + 1])));
            return true;
        }
    }

    return false;
}

}