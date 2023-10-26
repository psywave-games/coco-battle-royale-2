#!/usr/bin/env python3
from base64 import b64encode as b64
import sys

title = 'Coco Battle Royale II'
input = './bin/cocobattleroyale.nes'
output = './html/usa.html'

if '--jap' in sys.argv:
    title = 'Kokobatoru 2'
    input = './bin/kokobatoru.nes'
    output = './html/jap.html'

rom = b64(open(input, "rb").read()).decode()
html = open("./res/index.html", "r").read()
html = html.replace(r'{{game}}', title).replace(r'{{rom}}', rom)

open(output, "w").write(html)
print(title)
