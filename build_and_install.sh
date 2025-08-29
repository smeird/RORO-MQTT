#!/usr/bin/env bash
set -euo pipefail

BUILD_DIR="build"
INSTALL_PREFIX="/usr"

if [ "$(id -u)" -ne 0 ]; then
  SUDO="sudo"
else
  SUDO=""
fi

cmake -B "$BUILD_DIR" -S . -DCMAKE_INSTALL_PREFIX="${INSTALL_PREFIX}" "$@"
cmake --build "$BUILD_DIR" --parallel

pushd "$BUILD_DIR" >/dev/null
ctest --output-on-failure
popd >/dev/null

for prefix in /usr /usr/local; do
  $SUDO rm -f "$prefix/bin/indi-mqtt-universalror" \
    "$prefix/share/indi/mqtt_universalror.xml" \
    "$prefix/lib/udev/rules.d/indi-mqtt-universalror.rules" || true
done

$SUDO cmake --install "$BUILD_DIR"

echo "Installation complete. Restart Ekos/KStars to load the MQTT Universal ROR driver."
