;NES 4-player controller reading - written for the ca65 assembler
;- works with FourScore on NES (and AV Famicom afaik)
;- works with Hori adapter on Famicom (up to 4 players)
;- works with expansion port controllers on Famicom (3 players)
;- should work with dual expansion port controllers on Famicom (4 players)
;- has checks in place to allow worn out standard controllers to be replaced by
;  expansion controllers on Famicom (depending on state of playerActive array);
;  This is good behavior for Famicom, a lot of commercially released Famicom
;  games do the same!

.export _playerActive,_joy1,_joy2,_joy3,_joy4

;joypad button constants
JOY_RIGHT 	= $01
JOY_LEFT	= $02
JOY_DOWN 	= $04
JOY_UP	 	= $08
JOY_START	= $10
JOY_SELECT	= $20
JOY_B		= $40
JOY_A		= $80

;you'll need to initialize this when the user selects the number of players.
;zero for active players, non-zero for inactive players
_playerActive: .res 4

;will hold state of joypads after call to updateInput (use bitwise ops with
;button constants above)
_joy1: .res 1
_joy2: .res 1
_joy3: .res 1
_joy4: .res 1

buf4016_0: .res 3
buf4016_1: .res 3
buf4017_0: .res 3
buf4017_1: .res 3
