# rm_detector2026

## Introduction

`rm_detector2026` is a ROS 2 package designed for the RoboMaster competition to perform real-time armor detection. It subscribes to raw camera images (`/image_raw`), performs object detection and pose estimation (identifying armor bounding boxes and keypoints), and publishes the processed results alongside an annotated visualization image to the `/detector/result` topic. Under the hood, it harnesses YOLOv10-pose models and OpenVINO to ensure fast and accurate inference.

---

## Ideas and Structure

The core concept of `rm_detector2026` is a highly modular and decoupled **Pipeline Architecture**. Because vision tasks can vary dramatically depending on the specific models and deployment hardware, the package breaks the detection process into four independent stages. Data is passed between these stages using a shared `DetectionContext` structure.

The pipeline comprises:
1. **Preprocessor:** Prepares the `cv::Mat` image for the neural network (e.g., YOLO letterbox resizing, normalization, padding).
2. **Inference Engine:** Loads the exported model and performs tensor calculations. Currently natively supports OpenVINO.
3. **Decoder:** Parses the dense tensors output by the inference engine back into meaningful representations (e.g., decoding YOLOv10-pose outputs into bounding boxes, class IDs, colors, and 4 corner keypoints + center).
4. **Postprocessor:** Refines the raw decoded results. Supports a chained structure (e.g., executing Non-Maximum Suppression (NMS) followed by rule-based geometric filtering).

Each of these components implements a corresponding interface (`IPreprocessor`, `IInferenceEngine`, `IDecoder`, `IPostprocessor`). At runtime, a master **Detector Node** uses the **Factory Design Pattern** (`PreprocessorFactory`, `EngineFactory`, etc.) to instantiate and chain the objects dynamically based on ROS 2 parameters.

---

## For User

### Prerequisites
* ROS 2 (e.g., Humble)
* OpenCV & `cv_bridge`
* OpenVINO Runtime
* Custom dependencies: `rm_interfaces`, `rm_utils`(from sentry2025/sentry_perception)

### Build the Package
Navigate to your workspace and build using `colcon`:
```bash
colcon build --packages-up-to rm_detector2026 --symlink-install
source install/setup.bash
```

### Configuration
All pipeline behaviors, thresholds, and paths are governed by `config/detector_params.yaml`.
Key parameters to modify:
* `engine.model_path`: Update this to the `absolute` path of your `.xml` OpenVINO model.
* `decoder.conf_threshold`: Adjust to dial in your bounds for detection confidence.
* `postprocessor`: Enable or disable chaining for `nms` or custom filters.

### Running the Node
Launch the detector using the provided launch file. You can configure the namespace and camera name using launch arguments.

```bash
ros2 launch rm_detector2026 detector_launch.py namespace:=d1 camera_name:=camera
```

### ROS Topics
* **Subscribes to:** `/<camera_name>/image_raw` (`sensor_msgs/msg/Image`)
* **Publishes to:** `/<camera_name>/detector/result` (`rm_interfaces/msg/Detection`) *(Contains both the parsed `Armor` lists and a visualized/annotated image for remote viewing).*

---

## For Maintainer

The package is explicitly engineered for ease of expansion as vision requirements change across competition seasons.

### Extending the Pipeline
* **Adding a New Neural Network Framework (e.g., TensorRT or ONNXRuntime):**
  1. Create a clear wrapper class in `src/pipeline/engines/` that inherits from `IInferenceEngine`.
  2. Implement the `init()` and `infer()` virtual methods.
  3. Register the new engine string handle in `src/factories/engine_factory.cpp`.
* **Adding a New Decoder or Postprocessor:**
  Identical to the above—inherit from the respective `I-` class header (`IDecoder`, `IPostprocessor`), implement the core processing logic handling the `DetectionContext`, and wire it up in the corresponding factory.

### `DetectionContext`
Be aware of the `DetectionContext` passed along the pipeline. It stores intermediate states (like the preprocessed image sizes and padding) which are often necessary downstream (for example, mapping coordinates back to the original image scale). Use `Pipeline::save_context()` to dump intermediary tensor/image outputs to disk when diagnosing alignment or scaling issues.

### Testing

Testing for this package is divided into three main components:

#### 1. ROS 2 Test (TODO)
*(TODO)* End-to-end integration tests using standard ROS 2 testing frameworks (e.g., `launch_testing`) to verify proper node initialization, topic subscriptions, and message publishing.

#### 2. Pipeline Test
To bypass ROS complexity entirely when validating model inference and the vision pipeline on static images or video feeds, there is an included Python script:
```bash
ros2 run rm_detector2026 test_image_feed.py
```

#### 3. Module Test (TODO)
*(TODO)* Unit testing for individual C++ components (preprocessors, engines, decoders, postprocessors). While some basic test binaries like `test_engines` and `test_preprocessors` exist using `ament_cmake`/`gtest` (run via `colcon test`), comprehensive module-level coverage is still to be completed.