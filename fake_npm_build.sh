#!/usr/bin/sh
mkdir -p html
cp ./res/index.html ./html
cp ./bin/kokobatoru.nes ./html
cp ./bin/cocobattleroyale.nes ./html
echo '<!DOCTYPE html><html><head><meta http-equiv="refresh" content="1;url=https://psywave-games.github.io/coco-battle-royale-2/"></head></html>' > ./html/404.html
