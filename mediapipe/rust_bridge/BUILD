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

        "@com_google_absl//absl/flags:parse",
    ],
)

cc_binary(
    name = "mediacanal",
    linkshared = 1,
    deps = [
        ":mediacanal_lib",
    ],
)
