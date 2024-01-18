cc_library(
    name = "mediacanal_lib",
    srcs = [
        "//cxx_generated:bridge.rs.cc",
        "mediacanal.cc",
        "cxx_graph.cc",
   	    "cxx_packet.cc",
  	],
    hdrs = [
        "//cxx_generated:bridge.rs.h",
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
        # Legacy graphs
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

        # Legacy? modules
        "//mediapipe/modules/face_detection:face_detection",
        "//mediapipe/modules/face_geometry:face_geometry",
        "//mediapipe/modules/face_landmark:face_landmark_cpu",
        "//mediapipe/modules/hand_landmark:hand_landmark_cpu",
        "//mediapipe/modules/hand_landmark:hand_landmark_tracking_cpu",
        "//mediapipe/modules/holistic_landmark:face_landmarks_from_pose_cpu",
        "//mediapipe/modules/holistic_landmark:face_tracking",
        "//mediapipe/modules/holistic_landmark:hand_landmarks_from_pose_cpu",
        "//mediapipe/modules/holistic_landmark:hand_landmarks_left_and_right_cpu",
        "//mediapipe/modules/holistic_landmark:hand_tracking",
        "//mediapipe/modules/holistic_landmark:holistic_landmark_cpu",
        "//mediapipe/modules/objectron:box_landmark_cpu",
        "//mediapipe/modules/objectron:objectron_cpu",
        "//mediapipe/modules/palm_detection:palm_detection_cpu",
        "//mediapipe/modules/pose_detection:pose_detection_cpu",
        "//mediapipe/modules/pose_landmark:pose_landmark_cpu",
        "//mediapipe/modules/selfie_segmentation:selfie_segmentation_cpu",

        # Tasks / vision:
        "//mediapipe/tasks/cc/vision/face_detector:face_detector",
        "//mediapipe/tasks/cc/vision/face_landmarker:face_landmarker",
        "//mediapipe/tasks/cc/vision/face_stylizer:face_stylizer",
        "//mediapipe/tasks/cc/vision/gesture_recognizer:gesture_recognizer",
        "//mediapipe/tasks/cc/vision/hand_landmarker:hand_landmarker",
        "//mediapipe/tasks/cc/vision/image_classifier:image_classifier",
        "//mediapipe/tasks/cc/vision/image_embedder:image_embedder",
        "//mediapipe/tasks/cc/vision/image_segmenter:image_segmenter",
        "//mediapipe/tasks/cc/vision/interactive_segmenter:interactive_segmenter",
        "//mediapipe/tasks/cc/vision/object_detector:object_detector",
        "//mediapipe/tasks/cc/vision/pose_landmarker:pose_landmarker",
    ],
)

cc_binary(
    name = "mediacanal",
    linkshared = 1,
    deps = [
        ":mediacanal_lib",
    ],
)
