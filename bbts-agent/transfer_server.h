#ifndef BBTS_AGENT_TRANSFER_SERVER_H
#define BBTS_AGENT_TRANSFER_SERVER_H

#include <string>
#include <vector>
#include <boost/noncopyable.hpp>
#include <boost/scoped_ptr.hpp>

#include "proto/transfer_server_types.h"
#include "proto/TransferServer.h"
#include "bbts-agent/routing.h"

namespace bbts {

/**
 * @brief
 */
class TransferServer : public Routing {
public:
    TransferServer();
    ~TransferServer();

    void request_transfer(const RequestTransferInfo &info);

private:
};

} // namespace bbts

#endif // BBTS_AGENT_TRANSFER_SERVER_H
