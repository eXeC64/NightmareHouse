#!/bin/bash

pushd src
./devtools/bin/vpc /nh2 +everything /mksln everything &&
#make -j8 -f everything.mak clean &&
make -j8 -f everything.mak client_nh2 server_nh2
popd
cp game/nh2/bin/{server,client}.so .
