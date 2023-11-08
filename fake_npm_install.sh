#!/usr/bin/sh
mkdir -p html
URL_GAMEPAD=https://cdn.jsdelivr.net/npm/gamepadzilla@1/dist/gamepadzilla.js
URL_FCEUX_JS=https://cdn.jsdelivr.net/npm/em-fceux@2.2.0/dist/fceux.min.js
URL_FCEUX_WASM=https://cdn.jsdelivr.net/npm/em-fceux@2.2.0/dist/fceux.wasm
(curl -fsSL $URL_GAMEPAD || wget -qO- $URL_GAMEPAD) > html/gamepadzilla.js
(curl -fsSL $URL_FCEUX_JS || wget -qO- $URL_FCEUX_JS) > html/fceux.js
(curl -fsSL $URL_FCEUX_WASM || wget -qO- $URL_FCEUX_WASM) > html/fceux.wasm
