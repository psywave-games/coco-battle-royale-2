#!/usr/bin/env python3
from base64 import b64encode as b64
import requests
import sys

title = 'Coco Battle Royale II'
input = './bin/cocobattleroyale.nes'
output = './html/usa.html'

if '--jap' in sys.argv:
    title = 'Kokobatoru 2'
    input = './bin/kokobatoru.nes'
    output = './html/jap.html'

if '--download' in sys.argv:
    for file_name in ['nes.min.js']:
        cdn = 'https://cdn.jsdelivr.net/gh/takahirox/nes-js@v0.0.1/build'
        with requests.get(f'{cdn}/{file_name}', stream=True) as download:
            file_name = file_name.replace(r'.min.js', '.js')
            open(f'./html/{file_name}', "w").write(download.text)

rom = b64(open(input, "rb").read()).decode()
html = open("./res/index.html", "r").read()
html = html.replace(r'{{game}}', title).replace(r'{{rom}}', rom)
open(output, "w").write(html)

print(title)
