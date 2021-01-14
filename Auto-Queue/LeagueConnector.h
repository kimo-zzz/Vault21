#pragma once
#include <windows.h>
#include <wininet.h>
#include <iostream>
#include <conio.h>
#include <Windows.h>
#include <psapi.h>
#include <tlhelp32.h>
#include <iostream>
#include <fstream>
#include <vector>
#include <chrono>
#include <thread>
#include <string>
#include <cassert>
#include <limits>
#include <stdexcept>
#include <cctype>

#pragma comment(lib, "Wininet.lib")

class LeagueConnector
{
private:
    const char b64_table[65] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

    const char reverse_table[128] = {
       64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64,
       64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64,
       64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 62, 64, 64, 64, 63,
       52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 64, 64, 64, 64, 64, 64,
       64,  0,  1,  2,  3,  4,  5,  6,  7,  8,  9, 10, 11, 12, 13, 14,
       15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 64, 64, 64, 64, 64,
       64, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40,
       41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, 64, 64, 64, 64, 64
    };

    const std::wstring m_leagueProcessName{ L"LeagueClient.exe" };
    const std::wstring m_lockFileName{ L"lockfile" };
    const std::string m_username{ "riot" };
    const std::string m_host{ "127.0.0.1" };
    std::wstring m_lockFilePath;
    std::string m_port;
    std::string m_protocol;
    std::string m_password;
    std::string m_encodedAuth;
    std::string m_url;

    static void GetProcessIdByName(const std::wstring& pName, DWORD& outPid);
    static void GetProcessLocation(DWORD pid, std::wstring& outProcessLocation);
    static void GetFileContents(const std::wstring& filePath, std::string& outContents);
    static bool IsFileExistent(const std::wstring& fName, int retries = 0);
    std::string getPort() const { return m_port; }
    std::string getProtocol() const { return m_protocol; }
    std::string getEncodedAuth() const { return m_encodedAuth; }
    std::string getUrl() const { return m_url; }
    std::string getHost() const { return m_host; }
    friend std::ostream& operator<<(std::ostream& out, const LeagueConnector& lc);
    std::string base64_encode(const std::string& binData);
    std::string base64_decode(const std::string& ascData);

    HINTERNET hInternet;
    HINTERNET hConnection;

public:
    LeagueConnector(const std::wstring& leagueRootDir = L"");
    LeagueConnector(const LeagueConnector& lConn) = delete;
    ~LeagueConnector();

    std::string executeHttps(std::string RequestType, std::string Path, std::string Data = "");
};