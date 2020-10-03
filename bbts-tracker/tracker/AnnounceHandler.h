#ifndef BBTS_TRACKER_ANNOUNCE_HANDLER_
#define BBTS_TRACKER_ANNOUNCE_HANDLER_

#include "tracker-protocol/Announce.h"
#include "bbts-tracker/tracker/PeerHandler.h"

namespace bbts {
namespace tracker {

class AnnounceHandler : virtual public AnnounceIf {
 public:
  AnnounceHandler() = default;

  void announce(AnnounceResponse &_return, const AnnounceRequest &_request) override;

  void StopByInfohash(BaseResponse& _return, const ControlByInfohashRequest& request) override;

  void ResumeByInfohash(BaseResponse& _return, const ControlByInfohashRequest& request) override;

  PeerHandler& get_peer_handler() {
    return peer_handler_;
  }

 private:
  void AnnounceInfo(AnnounceResponse &_return,
                    int error_code,
                    const std::string &request_tag,
                    const std::string &message);

  void AnnounceSuccess(AnnounceResponse &_return, const std::string &request_tag);

  void AnnounceError(AnnounceResponse &_return,
                     const std::string &request_tag,
                     const std::string &error_message);

  bool PreHandleRequest(const AnnounceRequest &request, PeerInfo *peer_info);

  bool ComposeResponse(const PeerList &peer_list, bool have_seed, AnnounceResponse *response);

  void ControlByInfohash(BaseResponse& _return, const ControlByInfohashRequest& request, int type);

  PeerHandler peer_handler_;
  static int MIN_INTERVAL;
};

} // namespace tracker
} // namespace bbts


#endif // BBTS_TRACKER_ANNOUNCE_HANDLER_

