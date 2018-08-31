#include <google/protobuf/stubs/common.h>
#include "bbts-agent/bbts_client.h"

int main(int argc, char* argv[]) {
    if (!bbts::init_download_configure(argc, argv)) {
        return 1;
    }

    GOOGLE_PROTOBUF_VERIFY_VERSION;
    int ret = bbts::process(argc, argv);
    google::protobuf::ShutdownProtobufLibrary();
    return ret;
}
