#!/bin/sh

pushd sp/src
./devtools/bin/vpc /nh2 +everything /mksln everything &&
xcodebuild -project sp/src/everything.xcodeproj -configuration Release -target "client (nh2)" -target "server (nh2)"
popd
cp sp/game/nh2/bin/server.dylib .
cp sp/game/nh2/bin/client.dylib .
