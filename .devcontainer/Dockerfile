FROM osrf/ros:humble-desktop-full

# Add non-root user with sudo access
ARG USERNAME=vscode
ARG USER_UID=1000
ARG USER_GID=$USER_UID

# Prevent interactive prompts during package installation
ENV DEBIAN_FRONTEND=noninteractive

RUN groupadd --gid $USER_GID $USERNAME \
    && useradd --uid $USER_UID --gid $USER_GID -m $USERNAME \
    && apt-get update \
    && apt-get install -y sudo \
    && echo $USERNAME ALL=\(root\) NOPASSWD:ALL > /etc/sudoers.d/$USERNAME \
    && chmod 0440 /etc/sudoers.d/$USERNAME

RUN sudo apt-get update && sudo apt-get install -y python3-rosdep \
    && if [ -f /etc/ros/rosdep/sources.list.d/20-default.list ]; then sudo rm /etc/ros/rosdep/sources.list.d/20-default.list; fi \
    && sudo rosdep init \
    && rosdep update

# Install essential tools
RUN apt-get update && apt-get install -y \
    python3-pip \
    python3-colcon-common-extensions \
    git \
    wget \
    curl \
    vim \
    nano \
    gnupg2 \
    lsb-release \
    software-properties-common \
    # Basic OpenCV dependencies usually needed for vision tasks
    libopencv-dev \
    python3-opencv \
    # Clean up
    && rm -rf /var/lib/apt/lists/*

# Install OpenVINO
RUN wget https://apt.repos.intel.com/intel-gpg-keys/GPG-PUB-KEY-INTEL-SW-PRODUCTS.PUB \
    && cat GPG-PUB-KEY-INTEL-SW-PRODUCTS.PUB | gpg --dearmor | tee /usr/share/keyrings/intel-openvino-archive-keyring.gpg > /dev/null \
    && echo "deb [signed-by=/usr/share/keyrings/intel-openvino-archive-keyring.gpg] https://apt.repos.intel.com/openvino/2025 ubuntu22 main" | tee /etc/apt/sources.list.d/intel-openvino-2025.list \
    && apt-get update \
    && apt-get install -y openvino \
    && rm GPG-PUB-KEY-INTEL-SW-PRODUCTS.PUB \
    && rm -rf /var/lib/apt/lists/*

# Install GPU drivers (Intel Compute Runtime)
# Referencing openvino_cgn_runtime_2025.3.0.0.dockerfile
RUN apt-get update && \
    apt-get install -y --no-install-recommends ocl-icd-libopencl1 && \
    mkdir /tmp/gpu_deps && cd /tmp/gpu_deps && \
    curl -L -O https://github.com/intel/intel-graphics-compiler/releases/download/v2.2.3/intel-igc-core-2_2.2.3+18220_amd64.deb && \
    curl -L -O https://github.com/intel/intel-graphics-compiler/releases/download/v2.2.3/intel-igc-opencl-2_2.2.3+18220_amd64.deb && \
    curl -L -O https://github.com/intel/compute-runtime/releases/download/24.48.31907.7/intel-level-zero-gpu-dbgsym_1.6.31907.7_amd64.ddeb && \
    curl -L -O https://github.com/intel/compute-runtime/releases/download/24.48.31907.7/intel-level-zero-gpu_1.6.31907.7_amd64.deb && \
    curl -L -O https://github.com/intel/compute-runtime/releases/download/24.48.31907.7/intel-opencl-icd-dbgsym_24.48.31907.7_amd64.ddeb && \
    curl -L -O https://github.com/intel/compute-runtime/releases/download/24.48.31907.7/intel-opencl-icd_24.48.31907.7_amd64.deb && \
    curl -L -O https://github.com/intel/compute-runtime/releases/download/24.48.31907.7/libigdgmm12_22.5.4_amd64.deb && \
    curl -L -O https://github.com/intel/compute-runtime/releases/download/24.48.31907.7/ww48.sum && \
    sha256sum -c ww48.sum && \
    dpkg -i ./*.deb && rm -Rf /tmp/gpu_deps && \
    rm -rf /var/lib/apt/lists/*

# Add user to video and render groups for GPU access
RUN groupadd -f render \
    && usermod -aG video,render $USERNAME

# Environment updates
ENV INTEL_OPENVINO_DIR=/opt/intel/openvino
ENV InferenceEngine_DIR=/opt/intel/openvino/runtime/cmake
ENV ngraph_DIR=/opt/intel/openvino/runtime/cmake
ENV OpenVINO_DIR=/opt/intel/openvino/runtime/cmake
ENV LD_LIBRARY_PATH=/opt/intel/openvino/runtime/lib/intel64:$LD_LIBRARY_PATH
ENV PKG_CONFIG_PATH=/opt/intel/openvino/runtime/lib/intel64/pkgconfig:$PKG_CONFIG_PATH
ENV PYTHONPATH=/opt/intel/openvino/python:/opt/intel/openvino/python/python3:$PYTHONPATH


# Set up auto-source of workspace for the user
RUN echo "if [ -f /opt/ros/${ROS_DISTRO}/setup.bash ]; then source /opt/ros/${ROS_DISTRO}/setup.bash; fi" >> /home/$USERNAME/.bashrc

ENV SHELL /bin/bash
USER $USERNAME
