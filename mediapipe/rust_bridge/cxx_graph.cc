#include "mediacanal.h"

#include <memory>
#include <utility>

#include "mediapipe/framework/calculator_framework.h"
#include "mediapipe/framework/port/parse_text_proto.h"
#include "mediapipe/framework/port/status.h"

namespace mediacanal {

std::unique_ptr<CxxGraph> new_cxx_graph(const CallbackHandler &rust_graph,
                                        rust::Str config) {
  return std::make_unique<CxxGraph>(rust_graph, std::string(config));
}

CxxGraph::CxxGraph(const CallbackHandler &rust_graph, const std::string &config)
    : rust_graph_(rust_graph), mediapipe_graph_(mediapipe::CalculatorGraph{}) {
  auto graph_config =
      mediapipe::ParseTextProtoOrDie<mediapipe::CalculatorGraphConfig>(config);

  LOG(INFO) << "Initializing the CxxGraph.";
  mp_throw_if_error(mediapipe_graph_.Initialize(graph_config));
}

CxxGraph::~CxxGraph() {
  LOG(INFO) << "Shutting down.";
  if (is_started_) {
    absl::Status status = mediapipe_graph_.CloseAllPacketSources();
    if (!status.ok()) {
      LOG(ERROR) << "Error in CloseAllPacketSources(): " << status.ToString();
    } else {

      absl::Status status = mediapipe_graph_.WaitUntilDone();
      if (!status.ok()) {
        LOG(ERROR) << "Error in WaitUntilDone(): " << status.ToString();
      }
    }
  } else {
    LOG(INFO) << "No cleanup since graph has not been started yet";
  }
}

void CxxGraph::start() {
  mp_throw_if_error(mediapipe_graph_.StartRun({}));
  is_started_ = true;
}
void CxxGraph::queue_packet(rust::Str input_id,
                            std::unique_ptr<CxxPacket> packet) {
  mp_throw_if_error(mediapipe_graph_.AddPacketToInputStream(
      std::string(input_id), packet->packet_));
}
void CxxGraph::observe_output(rust::Str output_id) {
  std::string output_id_str{output_id};
  auto out_cb = [&, output_id_clone = std::string{output_id}](const mediapipe::Packet &p) {
    // We cannot use output_id directly, because it is just a reference
    rust::Str output_id{output_id_clone};
    LOG(INFO) << "Received packet";
    auto packet = std::make_unique<CxxPacket>(p);
    rust_graph_.output_callback(output_id, std::move(packet));
    return absl::OkStatus();
  };
  mp_throw_if_error(
      mediapipe_graph_.ObserveOutputStream(output_id_str, out_cb));
}

} // namespace mediacanal
