#!/usr/bin/bash

echo launch build;
bash -c 'podman run -i --rm -v $(pwd):/home/dev/project -v pio_jenkins:/root pio_run_e_jenkins:1.0.0'

exit;
