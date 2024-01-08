cc_library(
    name = "mediacanal_lib",
    srcs = [
        "cxx_generated/bridge.rs.cc",
        "mediacanal.cc",
        "cxx_graph.cc",
   	    "cxx_packet.cc",
  	],
    hdrs = [
        "cxx_generated/bridge.rs.h",
        "mediacanal.h",
    ],
    alwayslink = 1,
    visibility = ["//visibility:public"],
    deps = [
        "//mediapipe/framework:calculator_framework",
        "//mediapipe/framework/formats:image_frame",
        "//mediapipe/framework/port:file_helpers",
        "//mediapipe/framework/port:parse_text_proto",
        "//mediapipe/framework/port:status",
        "//mediapipe/util:resource_util",

        "@com_google_absl//absl/flags:flag",
        "@com_google_absl//absl/flags:parse",

        # Include most graphs and modules for maximum features
        "//mediapipe/graphs/face_detection:desktop_live_calculators",
        "//mediapipe/graphs/face_mesh:desktop_calculators",
        "//mediapipe/graphs/face_mesh:desktop_live_calculators",
        "//mediapipe/graphs/hair_segmentation:desktop_calculators",
        "//mediapipe/graphs/hand_tracking:desktop_offline_calculators",
        "//mediapipe/graphs/hand_tracking:desktop_tflite_calculators",
        "//mediapipe/graphs/holistic_tracking:holistic_tracking_cpu_graph_deps",
        "//mediapipe/graphs/iris_tracking:iris_depth_cpu_deps",
        "//mediapipe/graphs/iris_tracking:iris_tracking_cpu_deps",
        "//mediapipe/graphs/object_detection:desktop_tflite_calculators",
        "//mediapipe/graphs/pose_tracking:pose_tracking_cpu_deps",
        "//mediapipe/graphs/selfie_segmentation:selfie_segmentation_cpu_deps",
        "//mediapipe/graphs/template_matching:desktop_calculators",
        "//mediapipe/graphs/tracking:desktop_calculators",
    ],
)

cc_binary(
    name = "mediacanal",
    linkshared = 1,
    deps = [
        ":mediacanal_lib",
    ],
)
