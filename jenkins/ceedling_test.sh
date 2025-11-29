#!/usr/bin/bash

if [ "$1" == "server" ]
then
  base=/root
else
  base=/home/lulu
fi

command="$base/.platformio/penv/bin/pio pkg install -e jenkins && \
          sed -i 's|test/cJSON|jenkins/cJSON|' test/json/test_json.c && \
          ceedling"

echo launch test stage;

if [ "$1" == "server" ]
then
  podman exec jenkins_ctr /bin/bash -c "$command"
else
  /bin/bash -c "$command"
fi

exit;
