FROM mcr.microsoft.com/devcontainers/cpp:1-ubuntu-22.04

ARG REINSTALL_CMAKE_VERSION_FROM_SOURCE="3.28.0"
ARG INSTALL_CLANG_VERSION="17"

# Optionally install the cmake for vcpkg
COPY ./reinstall-cmake.sh /tmp/

RUN if [ "${REINSTALL_CMAKE_VERSION_FROM_SOURCE}" != "none" ]; then \
        chmod +x /tmp/reinstall-cmake.sh && /tmp/reinstall-cmake.sh ${REINSTALL_CMAKE_VERSION_FROM_SOURCE}; \
    fi \
    && rm -f /tmp/reinstall-cmake.sh

# add add-apt-repository for llvm install
RUN apt-get update && export DEBIAN_FRONTEND=noninteractive \
    && apt-get -y install --no-install-recommends software-properties-common gnuplot-nox clang-tidy
# with X: gnuplot-qt xauth

# llvm install (up to version 17). To update, wget https://apt.llvm.org/llvm.sh
# and add "-y" to line add-apt-repository ...
COPY ./llvm.sh /tmp/

RUN if [ "${INSTALL_CLANG_VERSION}" != "none" ]; then \
        chmod +x /tmp/llvm.sh && /tmp/llvm.sh ${INSTALL_CLANG_VERSION} \
        && apt-get -y install --no-install-recommends clang-tidy-${INSTALL_CLANG_VERSION}; \
    fi \
    && rm -f /tmp/llvm.sh

# [Optional] Uncomment this section to install additional vcpkg ports.
# RUN su vscode -c "${VCPKG_ROOT}/vcpkg install <your-port-name-here>"

# [Optional] Uncomment this section to install additional packages.
# RUN apt-get update && export DEBIAN_FRONTEND=noninteractive \
#     && apt-get -y install --no-install-recommends <your-package-list-here>