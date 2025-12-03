#!/usr/bin/bash

command="sed -i 's|test/cJSON|jenkins/cJSON|' test/json/test_json.c && \
         ceedling"

echo launch test stage;

podman run -u root -i --rm -v $(pwd):/home/dev/project -v pio_jenkins:/root madsciencelab:latest /bin/bash -c "$command"

exit;
