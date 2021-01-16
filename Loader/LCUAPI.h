#pragma once
#include "LeagueConnector.h"
#include <regex>

struct JSON_Object {
    JSON_Object(std::string pData = "") {
        Data = pData;
    }
    void Set(std::string pData) {
        this->Data = pData;
    }
    std::string Get() {
        return this->Data;
    }
private:
    std::string Data;
};

enum class GameState {
    UNKNOWN,
    NONE,
    LOBBY,
    MATCHMAKING,
    READYCHECK,
    CHAMPSELECT,
    INPROGRESS,
    WAITINGFORSTATS,
    PREENDOFGAME,
    ENDOFGAME
};
enum class GameMode {
    Draft_Pick = 400,           // 5v5 Draft Pick games
    Ranked_Solo = 420,          // 5v5 Ranked Solo games
    Blind_Pick = 430,           // 5v5 Blind Pick games
    Ranked_Flex = 440,          // 5v5 Ranked Flex games
    ARAM = 450,                 // 5v5 ARAM games
    Bot_Intro = 830,            // Co-op vs. AI Intro Bot games
    Bot_Beginner = 840,         // Co-op vs. AI Beginner Bot game
    Bot_Intermediate = 850     // Co-op vs. AI Intermediate Bot games
};
enum class GameRoles {
    Top,
    Mid,
    Bot,
    Support,
    Jungle,
    Fill,
    Unselected,
};
enum class GamePickSlot {
    First = 1,
    Second,
    Third,
    Fourth,
    Fifth
};
enum class Champions {
    Annie = 1,
};

/*  To-Do List:
    - Proper JSON handler json->structs.
    - Client State checks to avoid calling any function we aren't suppose to outside of the scope of where it should be called.
    - add all possible RPC calls.
    - Add a GUI.
    - Add saveable config file.
*/

class LCUAPI {
public:
    bool IsConnected();
    GameState GetGameState();
    JSON_Object CreatePracticeGame(unsigned short TeamSize = 1);
    JSON_Object CreateLobby(GameMode QueueID);
    JSON_Object JoinQueue();
    JSON_Object ReadyCheckAccept();
    JSON_Object ReadyCheckDecline();
    JSON_Object RolePicker(GameRoles Role1, GameRoles Role2 = GameRoles::Unselected);
    JSON_Object SelectChampion(GamePickSlot actid, Champions ChampionID);
    JSON_Object CompleteAction(GamePickSlot actid);

private:
    LeagueConnector conn{ L"C:\\Riot Games\\League of Legends" };
};