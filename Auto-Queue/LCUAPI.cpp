#include "LCUAPI.h"

bool LCUAPI::IsConnected() {
    if (conn.executeHttps("GET", "/lol-platform-config/v1/initial-configuration-complete") == "true")
        return true;
    return false;
}
GameState LCUAPI::GetGameState() {
    std::string GameStatus = conn.executeHttps("GET", "/lol-gameflow/v1/gameflow-phase");
    GameStatus = std::regex_replace(GameStatus, std::regex("\""), "");
    if (GameStatus == "None")
        return GameState::NONE;
    else if (GameStatus == "Lobby")
        return GameState::LOBBY;
    else if (GameStatus == "MatchMaking")
        return GameState::MATCHMAKING;
    else if (GameStatus == "ReadyCheck")
        return GameState::READYCHECK;
    else if (GameStatus == "ChampSelect")
        return GameState::CHAMPSELECT;
    else if (GameStatus == "InProgress")
        return GameState::INPROGRESS;
    else if (GameStatus == "WaitingForStats")
        return GameState::WAITINGFORSTATS;
    else if (GameStatus == "PreEndOfGame")
        return GameState::PREENDOFGAME;
    else if (GameStatus == "EndOfGame")
        return GameState::ENDOFGAME;
    else
        return GameState::UNKNOWN;
}
JSON_Object LCUAPI::CreatePracticeGame(unsigned short TeamSize) {
    if (TeamSize > 5)
        TeamSize = 5;
    else if (TeamSize < 1)
        TeamSize = 1;

    std::string RequestData = "{\
        \"customGameLobby\": {\
        \"configuration\": {\
            \"gameMode\": \"PRACTICETOOL\", \"gameMutator\" : \"\", \"gameServerRegion\" : \"\", \"mapId\" : 11, \"mutators\" : {\"id\": 1}, \"spectatorPolicy\" : \"AllAllowed\", \"teamSize\" : " + std::to_string(TeamSize) + "\
        },\
            \"lobbyName\" : \"Practice Mode\",\
                \"lobbyPassword\" : null\
         },\
        \"isCustom\": true\
         }";

    return JSON_Object(conn.executeHttps("POST", "/lol-lobby/v2/lobby", RequestData));
}
JSON_Object LCUAPI::CreateLobby(GameMode QueueID) {
    return JSON_Object(conn.executeHttps("POST", "/lol-lobby/v2/lobby", "{\"queueId\":" + std::to_string((int)QueueID) + "}"));
}
JSON_Object LCUAPI::JoinQueue() {
    return JSON_Object(conn.executeHttps("POST", "/lol-lobby/v2/lobby/matchmaking/search"));
}
JSON_Object LCUAPI::ReadyCheckAccept() {
    return JSON_Object(conn.executeHttps("POST", "/lol-matchmaking/v1/ready-check/accept"));
}
JSON_Object LCUAPI::ReadyCheckDecline() {
    return JSON_Object(conn.executeHttps("POST", "/lol-matchmaking/v1/ready-check/decline"));
}
JSON_Object LCUAPI::RolePicker(GameRoles Role1, GameRoles Role2) {
    std::string FirstPreference = "";
    std::string SecondPreference = "";

    switch (Role1) {
    case GameRoles::Bot:
    {
        FirstPreference = "\"firstPreference\" : \"BOTTOM\"";
        break;
    }
    case GameRoles::Jungle:
    {
        FirstPreference = "\"firstPreference\" : \"JUNGLE\"";
        break;
    }
    case GameRoles::Mid:
    {
        FirstPreference = "\"firstPreference\" : \"MIDDLE\"";
        break;
    }
    case GameRoles::Support:
    {
        FirstPreference = "\"firstPreference\" : \"UTILITY\"";
        break;
    }
    case GameRoles::Top:
    {
        FirstPreference = "\"firstPreference\" : \"TOP\"";
        break;
    }
    case GameRoles::Unselected:
    {
        FirstPreference = "\"firstPreference\" : \"UNSELECTED\"";
        break;
    }
    }

    switch (Role2) {
    case GameRoles::Bot:
    {
        SecondPreference = "\"secondPreference\" : \"BOTTOM\"";
        break;
    }
    case GameRoles::Jungle:
    {
        SecondPreference = "\"secondPreference\" : \"JUNGLE\"";
        break;
    }
    case GameRoles::Mid:
    {
        SecondPreference = "\"secondPreference\" : \"MIDDLE\"";
        break;
    }
    case GameRoles::Support:
    {
        SecondPreference = "\"secondPreference\" : \"UTILITY\"";
        break;
    }
    case GameRoles::Top:
    {
        SecondPreference = "\"secondPreference\" : \"TOP\"";
        break;
    }
    case GameRoles::Unselected:
    {
        SecondPreference = "\"secondPreference\" : \"UNSELECTED\"";
        break;
    }
    }

    JSON_Object RoleSelect("{" + FirstPreference + ", " + SecondPreference + " }");

    if (Role1 == GameRoles::Fill || Role2 == GameRoles::Fill)
        RoleSelect.Set("{\"firstPreference\" : \"FILL\"}");


    return JSON_Object(conn.executeHttps("PUT", "/lol-lobby/v2/lobby/members/localMember/position-preferences", RoleSelect.Get()));
}
JSON_Object LCUAPI::SelectChampion(GamePickSlot actid, Champions ChampionID) {
    return JSON_Object(conn.executeHttps("PATCH", "/lol-champ-select/v1/session/actions/" + std::to_string((int)actid), "{ \"championId\": " + std::to_string((int)ChampionID) + " }"));
}
JSON_Object LCUAPI::CompleteAction(GamePickSlot actid) {
    return JSON_Object(conn.executeHttps("POST", "/lol-champ-select/v1/session/actions/" + std::to_string((int)actid) + "/complete"));
}