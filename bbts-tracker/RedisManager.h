#ifndef  BBTS_REDIS_MANAGER_H_
#define  BBTS_REDIS_MANAGER_H_

#include <map>

#include <boost/noncopyable.hpp>
#include <boost/thread.hpp>
#include <hiredis/hiredis.h>

#include "common/LazySingleton.hpp"
#include "bbts-tracker/ThreadSingleton.hpp"

namespace bbts {

// max number of servers in a single redis cluster
#define  MAX_REDIS_SERVERS 16


struct RedisServer {
    std::string hostname;
    int port;
};

struct ContextManager {
    // context used for charging redis command, pipeline mode used
    redisContext *charge_context{nullptr};
    // context used for pipeline release
    redisContext *release_context{nullptr};
    // how many commands were charged
    int charged_commands{0};
    int release_commands{0};
    boost::mutex charge_mutex;
    boost::mutex release_mutex;
    bool flag{false};
};

struct SyncContextManager {
    // context used for direct interaction(synchronized)
    redisContext *execute_context{nullptr};
    bool flag{false};
};

class RedisConf;
/**
 * class for redis manager
 **/
class RedisManager: private boost::noncopyable {
public:
    RedisManager() = default;
    ~RedisManager() = default;
    // @brief start redis manager. Tasks including:
    //   1. mark all redis-cluster flags to 'disabled'
    //   2. initialize SyncedQueue of redis records
    //   3. call RedisConf class to load related configurtion
    //   4. set max-queue/database/passwd
    //   5. copy redis server-list from RedisConf, if cluster
    //      is empty it will be set to diabled, or set to enabled
    //   6. create hashmap for host2redis, empty content
    //   7. build connections for all redis servers and authenticate
    //      with each server, once an authentification failed,
    //      this cluster would be marked as diabled
    //   8. clean redis-command number in pipeline
    bool Start(const RedisConf &redis_conf);

    // pipeline command to redis, will not wait
    bool PipelineCommand(const std::string &redis_key, const std::string &command);

    bool ExecuteCommand(const std::string &redis_key, const std::string &command, redisReply **reply);

    void FreeReplyObject(redisReply **reply);

    // mark the flag to be terminated so that
    //   execution threads will be notified to stop
    void Stop();

private:
    // @brief periodically connect failed servers
    // @param [in]:
    void CheckAndConnectRedis();

    void DoConnection();

    void SendToRedis(ContextManager *manager);

    // authentication with redis server, including setting timeouts,
    // selecting db, athentication
    bool ConnectRedis(const RedisServer &server, redisContext **context);

    // free a redis connection
    void Disconnect(redisContext **c);

    // send_all redis commands pipelined in @context
    bool FlushPipeline(ContextManager *context);

    uint32_t GetRedisIndex(const std::string &key);

    bool AppendCommand(ContextManager *context, const std::string &command);

    bool ExcuteCommandInternal(const RedisServer &server, SyncContextManager *context,
                               const std::string &command, redisReply **reply);

private:
    int redis_num_{0};

    // redis server config
    RedisServer redis_servers_[2][MAX_REDIS_SERVERS];

    // constructed from redis servers, muiltiple servers here
    // just in case redis server crash
    ContextManager context_managers_[2][MAX_REDIS_SERVERS];

    // connection params for redis server
    std::string database_;
    std::string passwd_;
    int timeout_{3};

    boost::thread_group threads_;

    bool terminated_{false};
};

// way to get a singleton object of RedisAManager
#define g_pRedisManager LazySingleton<RedisManager>::instance()

}  // namespace bbts
#endif  // BBTS_REDIS_MANAGER_H_
