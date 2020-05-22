#!/bin/sh
set -e

# Download the install script.
curl -LO https://github.com/bazelbuild/bazel/releases/download/3.1.0/bazel-3.1.0-installer-darwin-x86_64.sh

# Run the installer.
chmod +x bazel-3.1.0-installer-darwin-x86_64.sh
sudo ./bazel-3.1.0-installer-darwin-x86_64.sh
