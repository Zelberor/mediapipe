#include <utility>

#include "mediacanal.h"

#include "mediapipe/framework/formats/image_frame.h"
#include "mediapipe/framework/packet.h"
#include "mediapipe/framework/port/parse_text_proto.h"

namespace mediacanal {

std::unique_ptr<CxxPacket>
packet_from_image_data(const ImageMemoryInfo &memory_info) {
  if (memory_info.data == nullptr) {
    throw std::runtime_error("Input image_data is nullptr");
  }

  auto data_size_bytes = memory_info.rows * memory_info.step;
  auto packet_data = new uint8[data_size_bytes];
  std::memcpy((void *)packet_data, memory_info.data, data_size_bytes);
  auto packet = mediapipe::MakePacket<mediapipe::ImageFrame>(
      mediapipe::ImageFormat::SRGB, memory_info.cols, memory_info.rows,
      memory_info.step, packet_data);
  auto cxx_packet = std::make_unique<CxxPacket>(packet);
  return cxx_packet;
}

CxxPacket::CxxPacket(mediapipe::Packet packet) : packet_(std::move(packet)) {}
std::unique_ptr<CxxPacket> CxxPacket::at(int64 timestamp) const {
  auto packet = packet_.At(mediapipe::Timestamp(timestamp));
  auto cxx_packet = std::make_unique<CxxPacket>(packet);
  return cxx_packet;
}
ImageMemoryInfo CxxPacket::get_image_memory_info() const {
  auto& output_frame = packet_.Get<mediapipe::ImageFrame>();

  assert(output_frame.Format() == mediapipe::ImageFormat::SRGB);
  assert(output_frame.WidthStep() * output_frame.Height() == output_frame.PixelDataSize());

  ImageMemoryInfo memory_info{};
  memory_info.data = output_frame.PixelData();
  memory_info.cols = output_frame.Width();
  memory_info.rows = output_frame.Height();
  memory_info.step = output_frame.WidthStep();

  return memory_info;
}
LandmarkList CxxPacket::get_landmarks() const { return LandmarkList(); }
rust::Vec<LandmarkList> CxxPacket::get_landmarks_list() const {
  return rust::Vec<LandmarkList>();
}

} // namespace mediacanal
