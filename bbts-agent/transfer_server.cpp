#include "bbts-agent/transfer_server.h"

#include <boost/shared_ptr.hpp>
#include <thrift/protocol/TBinaryProtocol.h>
#include <thrift/Thrift.h>
#include <thrift/transport/TBufferTransports.h>
#include <thrift/transport/TSocket.h>

#include "common/log.h"
#include "bbts-agent/process_info.h"

using std::string;
using std::vector;
using boost::shared_ptr;

using apache::thrift::protocol::TProtocol;
using apache::thrift::protocol::TBinaryProtocol;
using apache::thrift::transport::TSocket;
using apache::thrift::transport::TTransport;
using apache::thrift::transport::TBufferedTransport;
using apache::thrift::TException;

namespace bbts {

TransferServer::TransferServer() {
    set_service_name("transfer");
}



void TransferServer::request_transfer(const RequestTransferInfo &info) {
    string machine_room = g_process_info->machine_room();
    if (!load_conf(routing_conf(), machine_room)) {
        if (!load_conf(g_process_info->root_dir() + "/conf/routing.conf", machine_room)) {
            WARNING_LOG("load transfer conf({}/conf/routing.conf) failed",
                    g_process_info->root_dir().c_str());
            return;
        }
    }

    int max_retry_times = 1;
    bool success = false;
    NodeVector nodes;
    get_nodes(info.infohash, &nodes);
    for (NodeVector::const_iterator it = nodes.begin(); it != nodes.end() && !success; ++it) {
        shared_ptr<TSocket> socket(new TSocket(it->first, it->second));
        socket->setConnTimeout(3000);
        socket->setSendTimeout(3000);
        socket->setRecvTimeout(5000);
        shared_ptr<TBufferedTransport> transport(new TBufferedTransport(socket));
        shared_ptr<TBinaryProtocol> protocol(new TBinaryProtocol(transport));
        TransferServerClient client(protocol);
        try {
            transport->open();
            NOTICE_LOG("[open transfer:{}:{}][success]", it->first.c_str(), it->second);
        } catch (TException &tx) {
            WARNING_LOG("[open transer:{}:{}][fail:{}]", it->first.c_str(), it->second, tx.what());
            continue;
        }

        int retry = 0;
        while (retry <= max_retry_times) {
            try {
                GeneralResponse response;
                client.request_transfer(response, info);
                TRACE_LOG("transfer server[{}:{}] said:{}",
                        it->first.c_str(), it->second, response.message.c_str());
                success = true;
                break;
            }
            catch (TException &e) {
                WARNING_LOG("get transfer server failed {}: {}", ++retry, e.what());
                sleep(1);
            }
        }  // while

        try {
            transport->close();
        } catch (TException &tx) {
            WARNING_LOG("[close transfer:{}:{}][fail:{}]",
                    it->first.c_str(), it->second, tx.what());
        }
    } // for
}

} // namespace bbts
