## wan 
<p align="center">
	<img src="pol.png">
</p>
wan is a game engine that creates 2D PlayStation 1 games. You'd write the game in the source/game directory, separating the different parts of your game into different "gamestates." This separates your game code from engine code and helps keep your game code modular. Of course, you don't really have to follow what I say; just do whatever you want :)

## Requirements
- Linux environment preferably 
- [PsyQ SDK](http://www.psxdev.net/help/psyq_install.html)
- mkpsxiso

## Compilation
1. in source/engine/psx.h:
- make sure that `REGION` is set to the region you want (`NTSC_DISC` or `PAL_DISC`)
- compile for EXE or DISC by setting `EXE_BUILD` to `TRUE` or `FALSE`
2. Run make.bat
3. done

Note 1: If you edit the name of the iso/cue file in `a_build.xml` or `e_build.xml`, also apply the new name to `GAME_NAME` in `make.bat` so that the clean function works properly.

Note 2: According to this [post](http://www.psxdev.net/forum/viewtopic.php?f=53&p=20065#p20065), you can get PsyQ running on Linux using Wine and tinyxml2.

