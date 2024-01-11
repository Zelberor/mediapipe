// Taken from https://github.com/julesyoungberg/mediapipe/tree/master

// Copyright 2019 The MediaPipe Authors.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//      http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//
// An example of sending OpenCV webcam frames into a MediaPipe graph.

#ifndef MEDIACANAL_H
#define MEDIACANAL_H

// We need to define this class before bridge.rs.h, otherwise it will not be
// found
namespace mediacanal {
    class CxxPacket;
    class CxxGraph;
}// namespace mediacanal

#include <cstdint>
#include <cstdlib>
#include <string>

#include "absl/status/status.h"
#include "mediapipe/framework/calculator_framework.h"

#include "cxx_generated/bridge.rs.h"

// For converting exceptions to rust results
namespace rust::behavior {
    template<typename Try, typename Fail>
    static void trycatch(Try &&func, Fail &&fail) noexcept try {
        func();
    } catch (const std::exception &e) {
        fail(e.what());
    }
}// namespace rust::behavior

namespace mediacanal {

    void mp_throw_if_error(const absl::Status &status);
    void set_resource_root_dir(rust::Str path);

    std::unique_ptr<CxxPacket> packet_new_image_frame(const ImageMemoryInfo &memory_info);
    std::unique_ptr<CxxPacket> packet_new_image(const ImageMemoryInfo &memory_info);
    std::unique_ptr<CxxPacket> packet_new_int(int32 value);
    std::unique_ptr<CxxPacket> packet_new_bool(bool value);

    class CxxPacket {
    public:
        const mediapipe::Packet packet_;
        explicit CxxPacket(mediapipe::Packet packet);
        CxxPacket(const CxxPacket &other) = default;
        CxxPacket(CxxPacket &&other) = default;
        CxxPacket &operator=(const CxxPacket &other) = default;
        CxxPacket &operator=(CxxPacket &&other) = default;

        std::unique_ptr<CxxPacket> clone() const;
        std::unique_ptr<CxxPacket> at(const Timestamp &timestamp) const;
        Timestamp timestamp() const;
        ImageMemoryInfo get_image_frame_memory_info() const;
        ImageMemoryInfo get_image_memory_info() const;
        Landmarks get_landmarks() const;
        rust::Vec<Landmarks> get_landmarks_list() const;
        Landmarks get_normalized_landmarks() const;
        rust::Vec<Landmarks> get_normalized_landmarks_list() const;
        Classifications get_classifications() const;
        rust::Vec<Classifications> get_classifications_list() const;
        int32 get_int() const;
        bool get_bool() const;
    };

    // Create and initialize using provided config
    // throws runtime exception if initialization failed
    std::unique_ptr<CxxGraph> new_cxx_graph(const CallbackHandler &rust_graph, rust::Str config, rust::Slice<const SidePacket> side_packets);

    class CxxGraph {
    private:
        const CallbackHandler &rust_graph_;
        mediapipe::CalculatorGraph mediapipe_graph_;
        bool is_started_ = false;

    public:
        CxxGraph(const CallbackHandler &rust_graph, const std::string &config, const std::map<std::string, mediapipe::Packet> &side_packets);
        CxxGraph(const CxxGraph &other) = delete;
        CxxGraph(CxxGraph &&other) = delete;
        CxxGraph &operator=(const CxxGraph &other) = delete;
        CxxGraph &operator=(CxxGraph &&other) = delete;
        ~CxxGraph();

        void start();
        void queue_packet(rust::Str input_id, std::unique_ptr<CxxPacket> packet);
        void observe_output(rust::Str output_id);
    };

}// namespace mediacanal

#endif
