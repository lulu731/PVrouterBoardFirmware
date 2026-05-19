#!/usr/bin/bash

command="ceedling"

echo launch test stage;

podman run -u root -i --rm -v $(pwd):/home/dev/project -v pio_jenkins:/root madsciencelab:latest /bin/bash -c "$command"

exit;
