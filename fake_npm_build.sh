#!/usr/bin/sh
mkdir -p html
cp ./bin/*.nes ./html
cp ./res/*.ico ./html
cp ./res/index.html ./html
echo '<!DOCTYPE html><html><head><meta http-equiv="refresh" content="1;url=https://psywave-games.github.io/coco-battle-royale-2/"></head></html>' > ./html/404.html
