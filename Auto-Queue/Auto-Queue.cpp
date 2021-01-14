#include "LCUAPI.h"

LCUAPI LCU;

void AutoQueue() {
    if (!LCU.IsConnected())
        return;

    switch (LCU.GetGameState()) {
    case GameState::NONE:
    {
        break;
    }
    case GameState::LOBBY:
    {
        break;
    }
    case GameState::MATCHMAKING:
    {
        break;
    }
    case GameState::READYCHECK:
    {
        break;
    }
    case GameState::CHAMPSELECT:
    {
        break;
    }
    case GameState::INPROGRESS:
    {
        break;
    }
    case GameState::WAITINGFORSTATS:
    {
        break;
    }
    case GameState::PREENDOFGAME:
    {
        break;
    }
    case GameState::ENDOFGAME:
    {
        break;
    }
    }
}

int main() {
    LCU.CreateLobby(GameMode::Ranked_Solo);
    //LCU.RolePicker(LCUAPI::GameRoles::Top, LCUAPI::GameRoles::Mid);
    LCU.RolePicker(GameRoles::Jungle, GameRoles::Top);

    //while (true)
    //    AutoQueue();
}