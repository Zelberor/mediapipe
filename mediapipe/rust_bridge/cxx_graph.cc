#include "mediacanal.h"

#include <memory>
#include <utility>

#include "mediapipe/framework/calculator_framework.h"
#include "mediapipe/framework/port/parse_text_proto.h"
#include "mediapipe/framework/port/status.h"

namespace mediacanal {

    std::unique_ptr<CxxGraph> new_cxx_graph(const CallbackHandler &rust_graph, rust::Str config, rust::Slice<const SidePacket> side_packets) {
        std::map<std::string, mediapipe::Packet> s_p_map{};
        for (auto &side_packet : side_packets) {
            s_p_map[std::string(side_packet.input_id)] = side_packet.packet->packet_;
        }
        return std::make_unique<CxxGraph>(rust_graph, std::string(config), s_p_map);
    }

    CxxGraph::CxxGraph(const CallbackHandler &rust_graph, const std::string &config, const std::map<std::string, mediapipe::Packet> &side_packets)
        : rust_graph_(rust_graph), mediapipe_graph_(mediapipe::CalculatorGraph{}) {
        auto graph_config = mediapipe::ParseTextProtoOrDie<mediapipe::CalculatorGraphConfig>(config);

        LOG(INFO) << "Initializing the CxxGraph.";
        mp_throw_if_error(mediapipe_graph_.Initialize(graph_config, side_packets));
        mediapipe_graph_.SetGraphInputStreamAddMode(mediapipe::CalculatorGraph::GraphInputStreamAddMode::WAIT_TILL_NOT_FULL);
    }

    CxxGraph::~CxxGraph() {
        LOG(INFO) << "Shutting down the CxxGraph.";
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
    void CxxGraph::set_input_stream_max_queue_size(rust::Str input_id, int32 size) {
        mp_throw_if_error(mediapipe_graph_.SetInputStreamMaxQueueSize(std::string(input_id), static_cast<int>(size)));
    }
    void CxxGraph::queue_packet(rust::Str input_id, std::unique_ptr<CxxPacket> packet) {
        mp_throw_if_error(mediapipe_graph_.AddPacketToInputStream(std::string(input_id), packet->packet_));
    }
    void CxxGraph::observe_output(rust::Str output_id) {
        std::string output_id_str{output_id};
        auto out_cb = [&, output_id_clone = std::string{output_id}](const mediapipe::Packet &p) {
            // We cannot use output_id directly, because it is just a reference
            rust::Str output_id{output_id_clone};
            auto packet = std::make_unique<CxxPacket>(p);
            rust_graph_.output_callback(output_id, std::move(packet));
            return absl::OkStatus();
        };
        mp_throw_if_error(mediapipe_graph_.ObserveOutputStream(output_id_str, out_cb));
    }

}// namespace mediacanal
