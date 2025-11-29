#!/usr/bin/sh

echo running container
podman run --rm -d -v $(pwd):/home/dev/project --name jenkins_ctr ceedling_platformio:1.0.0 sleep infinity
