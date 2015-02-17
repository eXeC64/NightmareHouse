#!/bin/bash

pushd sp/src
./devtools/bin/vpc /nh2 +everything /mksln everything
make -j8 -f everything.mak client_nh2 server_nh2
popd
cp sp/game/nh2/bin/{server,client}.so .
