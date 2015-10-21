#!/bin/bash

pushd src
./devtools/bin/vpc /nh2 +everything /mksln everything &&
xcodebuild -project everything.xcodeproj -configuration Release -target "client (nh2)" -target "server (nh2)"
popd
cp game/nh2/bin/{client,server}.dylib .
