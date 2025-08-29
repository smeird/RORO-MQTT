#!/usr/bin/env bash
set -euo pipefail

BUILD_DIR="build"

cmake -B "$BUILD_DIR" -S . "$@"
cmake --build "$BUILD_DIR" --parallel

pushd "$BUILD_DIR" >/dev/null
ctest --output-on-failure
popd >/dev/null

if [ "$(id -u)" -ne 0 ]; then
  sudo cmake --install "$BUILD_DIR"
else
  cmake --install "$BUILD_DIR"
fi

echo "Installation complete. Restart Ekos/KStars to load the MQTT Universal ROR driver."
