UPDATE LOG:

FIXME:
- !!! Player position is not centered on screen

22.08.25
- added infinitely looping background as preparation for spawning trees randomly
- fixed issues with unused variables
    Tomorrow:
        - implement random trees spawning
        - implement functionality to make them disappear when interacted with

21.08.25
- added camera view centered on the player
- did some preparation for tilemap implementation

20.08.25
- position of portal placement and player placement relative to portal after teleport is now correct

19.08.25
- animations are now again played correctly 

12.08.25
- cleanup of code and comments

10.08.25
- both portals do now have animations, not only the last placed one
- fixed problem where portal color was labeled wrong
- changed structure of how animations are stored

09.08.25
- added resourceManager and changed structure of how animations are loaded
- added animations to portals (spin)

08.08.25
- fixed player bounds
- fixed end position after teleport with sprite center offset

07.08.25
- added portals and portal logic
- started creating sprites for portals

05.08.25
- added background and animations to player

03.08.25
- new Game class for better abstraction
- Clickable Buttons with Lambdas as onClick

Todo:
- add asset folder to repo for easy access
- correct player collision bounds

Plans:
- Make Player Customizable by adding UI to change appearance (color, clothing skins, texture)
- Create inventory and make a portal gun which has to be equipped in order to place portals
- Animate portal creation, create sprites for portal and portal gun
- ??? Local Multiplayer with customizable keybinds
- ??? AI Enemies with movement
