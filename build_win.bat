
pushd sp\src
devtools\bin\vpc /2010 /nh2 +game /mksln nightmarehouse.sln
devenv nightmarehouse.sln /Rebuild Release
popd
copy sp\game\nh2\bin\client.dll client.dll
copy sp\game\nh2\bin\server.dll server.dll
