#include "LAGameMode.h"
#include "LAPlayer.h"

ALAGameMode::ALAGameMode()
{
    // Set the default pawn class to your custom LAPlayer character
    DefaultPawnClass = ALAPlayer::StaticClass();
}

