# `Coco Battle Royale II`<br/>`ココバトル (kokobatoru) 2`

[![version](https://img.shields.io/github/v/release/psywave-games/coco-battle-royale-2?sort=semver&label=download)](https://github.com/psywave-games/coco-battle-royale-2/releases)
[![license](https://img.shields.io/github/license/psywave-games/coco-battle-royale-2)](https://github.com/psywave-games/coco-battle-royale-2/blob/master/LICENSE) 
[![status](https://img.shields.io/github/checks-status/psywave-games/coco-battle-royale-2/master)](https://github.com/psywave-games/coco-battle-royale-2/actions)
[![size](https://img.shields.io/github/repo-size/psywave-games/coco-battle-royale-2)](https://github.com/psywave-games/coco-battle-royale-2/archive/refs/heads/master.zip)
[![Engine:CC65](https://img.shields.io/badge/Engine-CC65-red)](https://cc65.github.io/) 

> homebrew game for nintendo years 80 console, distributed as free software.


## How to build

```SHELL
$ git clone --recurse-submodules https://github.com/psywave-games/coco-battle-royale-2
$ make
```

## Gameplay

| :us: :eu: Nintendo Entertainment System | :jp: Famicom |
| :-------------------------------------: | :-----------:|
| ![coco battle royale 2 gameplay](https://raw.githubusercontent.com/psywave-games/gifs/main/cocobattleroyale2.gif) | ![kokobatoru 2 gameplay](https://raw.githubusercontent.com/psywave-games/gifs/main/kokobatoru2.gif) |

### Controls ###

 * player 1 is a main players thats start the game.
 * player 3 needs a FourScore or a extra gamepad for famicom. (only jap/not usa eu consoles)
 * player 4 needs a Fourscore adapter.

| player | controls |
| :----: | :------- |
| 1 | **attack:**<kbd>A</kbd> **moves:**<kbd>&uarr;</kbd> <kbd>&larr;</kbd> <kbd>&darr;</kbd> <kbd>&rarr;</kbd><br/>**restart**:<kbd>start</kbd> **pause:**<kbd>select</kbd> |
| 2 | **attack:**<kbd>A</kbd> **moves:**<kbd>&uarr;</kbd> <kbd>&larr;</kbd> <kbd>&darr;</kbd> <kbd>&rarr;</kbd> |
| 3 | **attack:**<kbd>A</kbd> **moves:**<kbd>&uarr;</kbd> <kbd>&larr;</kbd> <kbd>&darr;</kbd> <kbd>&rarr;</kbd> |
| 4 | **attack:**<kbd>A</kbd> **moves:**<kbd>&uarr;</kbd> <kbd>&larr;</kbd> <kbd>&darr;</kbd> <kbd>&rarr;</kbd> |

### Extra Controls ###

 * player 5 & 6 must need a homemade adapter, or if you use an emulator simply configure the keys.

| player | virtual controls | logic controls |
| :----: | :--------------- | :------------- |
| 5 | **attack:**<kbd>A</kbd><br/>**moves:**<kbd>&uarr;</kbd> <kbd>&larr;</kbd> <kbd>&darr;</kbd> <kbd>&rarr;</kbd> | **attack:**<kbd>2:B</kbd><br/>**moves:**<kbd>2:select</kbd> <kbd>2:start</kbd> <kbd>3:B</kbd> <kbd>3:select</kbd> |
| 6 | **attack:**<kbd>A</kbd><br/>**moves:**<kbd>&uarr;</kbd> <kbd>&larr;</kbd> <kbd>&darr;</kbd> <kbd>&rarr;</kbd> | **attack:**<kbd>1:B</kbd><br/>**moves:**<kbd>3:start</kbd> <kbd>4:select</kbd> <kbd>4:B</kbd> <kbd>4:start</kbd> |
