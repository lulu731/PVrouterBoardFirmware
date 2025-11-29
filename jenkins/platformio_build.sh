#!/usr/bin/bash

if [ "$1" == "server" ]
then
  build='podman exec jenkins_ctr /bin/bash -c "/root/.platformio/penv/bin/pio run -e jenkins"'
else
  build='/home/lulu/.platformio/penv/bin/pio run -e jenkins';
fi

echo launch build;
bash -c "$build";
exit;
