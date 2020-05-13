#!/bin/sh
set -e

# Get Bazel release key and add it to APT sources.
sudo apt-get update && sudo apt-get install curl
curl https://bazel.build/bazel-release.pub.gpg | sudo apt-key add -
echo "deb [arch=amd64] https://storage.googleapis.com/bazel-apt stable jdk1.8" | sudo tee /etc/apt/sources.list.d/bazel.list

# Step 3: Install Bazel
sudo apt-get update && sudo apt-get install bazel