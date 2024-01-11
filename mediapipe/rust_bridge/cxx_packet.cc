#include <utility>

#include "mediacanal.h"

#include "mediapipe/framework/formats/classification.pb.h"
#include "mediapipe/framework/formats/image.h"
#include "mediapipe/framework/formats/image_frame.h"
#include "mediapipe/framework/formats/landmark.pb.h"
#include "mediapipe/framework/packet.h"

namespace mediacanal {

    namespace helper {

        mediapipe::Timestamp get_mediapipe_timestamp(const Timestamp &timestamp) {
            if (timestamp.is_unset) {
                return mediapipe::Timestamp::Unset();
            } else if (timestamp.is_unstarted) {
                return mediapipe::Timestamp::Unstarted();
            } else if (timestamp.is_prestream) {
                return mediapipe::Timestamp::PreStream();
            } else if (timestamp.is_poststream) {
                return mediapipe::Timestamp::PostStream();
            }
            return mediapipe::Timestamp::FromSeconds(timestamp.second);
        }

        Timestamp from_mediapipe_timestamp(const mediapipe::Timestamp &timestamp) {
            Timestamp result{};
            result.is_unset = timestamp == mediapipe::Timestamp::Unset();
            result.is_unstarted = timestamp == mediapipe::Timestamp::Unstarted();
            result.is_prestream = timestamp == mediapipe::Timestamp::PreStream();
            result.is_poststream = timestamp == mediapipe::Timestamp::PostStream();
            result.second = timestamp.Seconds();
            return result;
        }

        Landmark from_mediapipe_landmark(const mediapipe::Landmark &landmark) {
            Landmark result{};
            result.x = landmark.x();
            result.y = landmark.y();
            result.z = landmark.z();
            result.presence = landmark.presence();
            result.visibility = landmark.visibility();
            return result;
        }

        Landmarks from_mediapipe_landmark_list(const mediapipe::LandmarkList &landmarks) {
            Landmarks result{};
            for (const auto &landmark: landmarks.landmark()) {
                result.items.push_back(from_mediapipe_landmark(landmark));
            }
            return result;
        }

        Landmark from_mediapipe_normalized_landmark(const mediapipe::NormalizedLandmark &landmark) {
            Landmark result{};
            result.x = landmark.x();
            result.y = landmark.y();
            result.z = landmark.z();
            result.presence = landmark.presence();
            result.visibility = landmark.visibility();
            return result;
        }

        Landmarks from_mediapipe_normalized_landmark_list(const mediapipe::NormalizedLandmarkList &landmarks) {
            Landmarks result{};
            for (const auto &landmark: landmarks.landmark()) {
                result.items.push_back(from_mediapipe_normalized_landmark(landmark));
            }
            return result;
        }

        Classification from_mediapipe_classification(const mediapipe::Classification &classification) {
            Classification result{};
            result.index = classification.index();
            result.score = classification.score();
            result.label = classification.label();
            result.display_name = classification.display_name();
            return result;
        }

        Classifications from_mediapipe_classification_list(const mediapipe::ClassificationList &classifications) {
            Classifications result{};
            for (const auto &landmark: classifications.classification()) {
                result.items.push_back(from_mediapipe_classification(landmark));
            }
            return result;
        }

        ImageMemoryInfo from_image_frame(const mediapipe::ImageFrame &image) {
            assert(image.Format() == mediapipe::ImageFormat::SRGB);
            assert(image.WidthStep() * image.Height() ==
                   image.PixelDataSize());

            ImageMemoryInfo memory_info{};
            memory_info.data = image.PixelData();
            memory_info.cols = image.Width();
            memory_info.rows = image.Height();
            memory_info.step = image.WidthStep();

            return memory_info;
        }

    }// namespace helper

    std::unique_ptr<CxxPacket> packet_new_image_frame(const ImageMemoryInfo &memory_info) {
        if (memory_info.data == nullptr) {
            throw std::runtime_error("Input image_data is nullptr");
        }

        auto data_size_bytes = memory_info.rows * memory_info.step;
        auto packet_data = new uint8[data_size_bytes];
        std::memcpy((void *) packet_data, memory_info.data, data_size_bytes);
        auto packet = mediapipe::MakePacket<mediapipe::ImageFrame>(mediapipe::ImageFormat::SRGB, static_cast<int>(memory_info.cols), static_cast<int>(memory_info.rows), static_cast<int>(memory_info.step), packet_data);
        auto cxx_packet = std::make_unique<CxxPacket>(packet);
        return cxx_packet;
    }
    std::unique_ptr<CxxPacket> packet_new_image(const ImageMemoryInfo &memory_info) {
        if (memory_info.data == nullptr) {
            throw std::runtime_error("Input image_data is nullptr");
        }

        auto data_size_bytes = memory_info.rows * memory_info.step;
        auto packet_data = new uint8[data_size_bytes];
        std::memcpy((void *) packet_data, memory_info.data, data_size_bytes);
        auto image_frame = std::make_shared<mediapipe::ImageFrame>(mediapipe::ImageFormat::SRGB, static_cast<int>(memory_info.cols), static_cast<int>(memory_info.rows), static_cast<int>(memory_info.step), packet_data);
        auto packet = mediapipe::MakePacket<mediapipe::Image>(image_frame);
        auto cxx_packet = std::make_unique<CxxPacket>(packet);
        return cxx_packet;
    }
    std::unique_ptr<CxxPacket> packet_new_int(int32 value) {
        auto packet = mediapipe::MakePacket<int>(static_cast<int>(value));
        auto cxx_packet = std::make_unique<CxxPacket>(packet);
        return cxx_packet;
    }
    std::unique_ptr<CxxPacket> packet_new_bool(bool value) {
        auto packet = mediapipe::MakePacket<bool>(value);
        auto cxx_packet = std::make_unique<CxxPacket>(packet);
        return cxx_packet;
    }

    CxxPacket::CxxPacket(mediapipe::Packet packet) : packet_(std::move(packet)) {}
    std::unique_ptr<CxxPacket> CxxPacket::clone() const {
        return std::make_unique<CxxPacket>(*this);
    }
    std::unique_ptr<CxxPacket> CxxPacket::at(const Timestamp &timestamp) const {
        auto packet = packet_.At(helper::get_mediapipe_timestamp(timestamp));
        auto cxx_packet = std::make_unique<CxxPacket>(packet);
        return cxx_packet;
    }
    Timestamp CxxPacket::timestamp() const {
        auto timestamp = packet_.Timestamp();
        return helper::from_mediapipe_timestamp(timestamp);
    }
    ImageMemoryInfo CxxPacket::get_image_frame_memory_info() const {
        auto &output_frame = packet_.Get<mediapipe::ImageFrame>();
        return helper::from_image_frame(output_frame);
    }
    ImageMemoryInfo CxxPacket::get_image_memory_info() const {
        auto &output_image = packet_.Get<mediapipe::Image>();
        auto output_frame = output_image.GetImageFrameSharedPtr();
        return helper::from_image_frame(*output_frame.get());
    }
    Landmarks CxxPacket::get_landmarks() const {
        auto &landmarks = packet_.Get<mediapipe::LandmarkList>();
        return helper::from_mediapipe_landmark_list(landmarks);
    }
    rust::Vec<Landmarks> CxxPacket::get_landmarks_list() const {
        auto &landmarks_list = packet_.Get<std::vector<mediapipe::LandmarkList>>();
        rust::Vec<Landmarks> result{};
        for (auto &landmarks: landmarks_list) {
            result.push_back(helper::from_mediapipe_landmark_list(landmarks));
        }
        return result;
    }
    Landmarks CxxPacket::get_normalized_landmarks() const {
        auto &landmarks = packet_.Get<mediapipe::NormalizedLandmarkList>();
        return helper::from_mediapipe_normalized_landmark_list(landmarks);
    }
    rust::Vec<Landmarks> CxxPacket::get_normalized_landmarks_list() const {
        auto &landmarks_list = packet_.Get<std::vector<mediapipe::NormalizedLandmarkList>>();
        rust::Vec<Landmarks> result{};
        for (auto &landmarks: landmarks_list) {
            result.push_back(helper::from_mediapipe_normalized_landmark_list(landmarks));
        }
        return result;
    }
    Classifications CxxPacket::get_classifications() const {
        auto &classifications = packet_.Get<mediapipe::ClassificationList>();
        return helper::from_mediapipe_classification_list(classifications);
    }
    rust::Vec<Classifications> CxxPacket::get_classifications_list() const {
        auto &classifications_list = packet_.Get<std::vector<mediapipe::ClassificationList>>();
        rust::Vec<Classifications> result{};
        for (auto &classifications: classifications_list) {
            result.push_back(helper::from_mediapipe_classification_list(classifications));
        }
        return result;
    }
    int32 CxxPacket::get_int() const {
        auto output = packet_.Get<int>();
        return static_cast<int32>(output);
    }
    bool CxxPacket::get_bool() const {
        auto output = packet_.Get<bool>();
        return output;
    }

}// namespace mediacanal
