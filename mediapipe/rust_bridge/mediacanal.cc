#include "mediacanal.h"

#include "absl/flags/declare.h"
#include "absl/flags/flag.h"

ABSL_DECLARE_FLAG(std::string, resource_root_dir);

namespace mediacanal {

void mp_throw_if_error(const absl::Status &status) {
  if (!status.ok()) {
    throw std::runtime_error{"Failed to execute mediapipe command: " +
                             status.ToString()};
  }
}

void set_resource_root_dir(rust::Str path) {
  std::string resource_path{path};
  absl::SetFlag(&FLAGS_resource_root_dir, resource_path);
}

}
