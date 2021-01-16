#include "LeagueConnector.h"

LeagueConnector::LeagueConnector(const std::wstring& leagueRootDir)
{
    std::wstring finalLockFileLocation;

    if (leagueRootDir.empty())
    {
        DWORD pid{ 0 };
        while (true)
        {
            GetProcessIdByName(m_leagueProcessName, pid);

            if (pid == 0)
            {
                constexpr int timeoutInSeconds{ 5 };
                std::cout << "League process not found, checking again in " << timeoutInSeconds << " seconds\n";
                std::this_thread::sleep_for(std::chrono::seconds(timeoutInSeconds));
            }
            else
            {
                std::cout << "Process found, working...\n";
                break;
            }
        }

        std::wstring processLocation;
        GetProcessLocation(pid, processLocation);

        // so we can exclude the .exe
        const size_t lastSlashIndex{ std::wstring{ processLocation }.find_last_of(L'\\') };
        finalLockFileLocation = std::wstring{ processLocation }.substr(0, lastSlashIndex) + L'\\' + std::wstring{ m_lockFileName };
    }
    else
    {
        finalLockFileLocation = leagueRootDir + L'\\' + std::wstring{ m_lockFileName };
    }

    constexpr int retries{ 5 };
    if (!IsFileExistent(finalLockFileLocation, retries))
    {
        std::wcerr << L"File: '" << finalLockFileLocation << L"' does not exist!\n";
        throw std::exception("Lock file not found");
    }

    std::string fileContents;
    GetFileContents(finalLockFileLocation, fileContents);
    if (fileContents.empty())
    {
        throw std::exception("Lock file is empty");
    }

    std::vector<std::string> params;
    size_t last{ 0 };
    size_t next;
    while ((next = fileContents.find(':', last)) != std::string::npos)
    {
        params.push_back(fileContents.substr(last, next - last));
        last = next + 1;
    }
    params.push_back(fileContents.substr(last));

    m_port = params[2];
    m_password = params[3];
    m_protocol = params[4];
    m_encodedAuth = base64_encode(m_username + ":" + m_password);
    m_url = m_protocol + "://" + m_host + ":" + m_port;
    hInternet = InternetOpenA("InetURL/1.0", INTERNET_OPEN_TYPE_PRECONFIG, NULL, NULL, 0);
    hConnection = InternetConnectA(hInternet, m_host.c_str(), std::stoi(m_port), " ", " ", INTERNET_SERVICE_HTTP, 0, 0);
}

LeagueConnector::~LeagueConnector() {
    InternetCloseHandle(hConnection);
    InternetCloseHandle(hInternet);
}

std::ostream& operator<<(std::ostream& out, const LeagueConnector& lc)
{
    out << "Port: " << lc.m_port << '\n';
    out << "Password: " << lc.m_password << '\n';
    out << "Protocol: " << lc.m_protocol << '\n';
    out << "Encoded basic auth: " << lc.m_encodedAuth << '\n';
    return out;
}

void LeagueConnector::GetFileContents(const std::wstring& filePath, std::string& outContents)
{
    std::ifstream fStream{ filePath };
    // If we couldn't open the output file stream for reading
    if (!fStream)
    {
        // Print an error and exit
        std::wcerr << "file '" << filePath << "' could not be opened for reading!\n";
        throw std::exception("Failed opening file for reading");
    }

    const std::string lockFileContents((std::istreambuf_iterator<char>(fStream)),
        std::istreambuf_iterator<char>());

    // TODO: maybe check here?
    outContents = lockFileContents;
}

void LeagueConnector::GetProcessLocation(DWORD pid, std::wstring& outProcessLocation)
{
    wchar_t processLocationBuf[MAX_PATH];
    const HANDLE handle{ OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, pid) };
    if (!handle)
    {
        throw std::exception("Can't open handle to process");
    }

    const DWORD result{ GetModuleFileNameExW(handle, nullptr, processLocationBuf, MAX_PATH) };
    CloseHandle(handle);
    if (result == 0)
    {
        throw std::exception("Can't get process location");
    }

    outProcessLocation = processLocationBuf;
}

void LeagueConnector::GetProcessIdByName(const std::wstring& pName, DWORD& outPid)
{
    if (pName.empty())
    {
        throw std::exception("Process name is empty");
    }

    // Take a snapshot of all processes in the system.
    const HANDLE hProcessSnap{ CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0) };
    if (hProcessSnap == INVALID_HANDLE_VALUE)
    {
        throw std::exception("CreateToolhelp32Snapshot (of processes)");
    }
    PROCESSENTRY32 pe32;
    // Set the size of the structure before using it.
    pe32.dwSize = sizeof(PROCESSENTRY32);

    // Retrieve information about the first process
    if (!Process32First(hProcessSnap, &pe32))
    {
        CloseHandle(hProcessSnap);
        throw std::exception("Process32First");
    }

    do
    {
        const HANDLE hProcess{ OpenProcess(PROCESS_ALL_ACCESS, FALSE, pe32.th32ProcessID) };
        if (hProcess == nullptr)
            continue;

        const DWORD dwPriorityClass{ GetPriorityClass(hProcess) };
        if (!dwPriorityClass)
        {
            throw std::exception("GetPriorityClass");
        }
        CloseHandle(hProcess);

        if (wcscmp(pe32.szExeFile, pName.c_str()) == 0)
        {
            outPid = pe32.th32ProcessID;
            break;
        }

    } while (Process32Next(hProcessSnap, &pe32));
}

bool LeagueConnector::IsFileExistent(const std::wstring& fName, const int retries)
{
    int retryCnt{ 0 };
    do
    {
        const std::wifstream f(fName.c_str());
        if (f.good())
        {
            return true;
        }

        std::wcout << "File " << fName << " not found, retrying...\n";

        constexpr int timeoutBetweenChecks{ 5 };
        std::this_thread::sleep_for(std::chrono::seconds(timeoutBetweenChecks));

    } while (retryCnt++ < retries);

    return false;
}

std::string LeagueConnector::base64_encode(const std::string& binData)
{
    using std::string;
    using std::numeric_limits;
#pragma push_macro("max")
#undef max
    if (binData.size() > (numeric_limits<string::size_type>::max() / 4u) * 3u)
    {
        throw std::length_error("Converting too large a string to base64.");
    }
#pragma pop_macro("max")
    const size_t binLen = binData.size();
    // Use = signs so the end is properly padded.
    string retVal((((binLen + 2) / 3) * 4), '=');
    size_t outPos = 0;
    int bits_collected = 0;
    unsigned int accumulator = 0;
    const string::const_iterator binEnd = binData.end();

    for (string::const_iterator i = binData.begin(); i != binEnd; ++i) {
        accumulator = (accumulator << 8) | (*i & 0xffu);
        bits_collected += 8;
        while (bits_collected >= 6) {
            bits_collected -= 6;
            retVal[outPos++] = b64_table[(accumulator >> bits_collected) & 0x3fu];
        }
    }
    if (bits_collected > 0) { // Any trailing bits that are missing.
        assert(bits_collected < 6);
        accumulator <<= 6 - bits_collected;
        retVal[outPos++] = b64_table[accumulator & 0x3fu];
    }
    assert(outPos >= (retVal.size() - 2));
    assert(outPos <= retVal.size());
    return retVal;
}

std::string LeagueConnector::base64_decode(const std::string& ascData)
{
    using std::string;
    string retVal;
    const string::const_iterator last = ascData.end();
    int bits_collected = 0;
    unsigned int accumulator = 0;

    for (string::const_iterator i = ascData.begin(); i != last; ++i) {
        const int c = *i;
        if (isspace(c) || c == '=') {
            continue;
        }
        if ((c > 127) || (c < 0) || (reverse_table[c] > 63)) {
            throw std::invalid_argument("This contains characters not legal in a base64 encoded string.");
        }
        accumulator = (accumulator << 6) | reverse_table[c];
        bits_collected += 6;
        if (bits_collected >= 8) {
            bits_collected -= 8;
            retVal += static_cast<char>((accumulator >> bits_collected) & 0xffu);
        }
    }
    return retVal;
}

std::string LeagueConnector::executeHttps(std::string RequestType, std::string Path, std::string Data) {
    std::string total;
    DWORD bytesRead = 0;
    DWORD totalBytesRead = 0;
    char buf[0x1000];
    HINTERNET hData = HttpOpenRequestA(hConnection, RequestType.c_str(), Path.c_str(), NULL, NULL, NULL, INTERNET_FLAG_KEEP_CONNECTION | INTERNET_FLAG_SECURE, 0);
    DWORD dwFlags;
    DWORD dwBuffLen = sizeof(dwFlags);

    if (InternetQueryOption(hData, INTERNET_OPTION_SECURITY_FLAGS, &dwFlags, &dwBuffLen))
    {
        dwFlags |= SECURITY_FLAG_IGNORE_UNKNOWN_CA;
        dwFlags |= SECURITY_FLAG_IGNORE_REVOCATION;
        InternetSetOption(hData, INTERNET_OPTION_SECURITY_FLAGS, &dwFlags, sizeof(dwFlags));
    }

    const std::string headers{ "Content-Type: application/json\r\nAccept: application/json\r\nAuthorization: Basic " + getEncodedAuth() };

    if (!Data.empty())
        auto hResult = HttpSendRequestA(hData, headers.c_str(), headers.length(), (char*)Data.c_str(), Data.length());
    else
        auto hResult = HttpSendRequestA(hData, headers.c_str(), headers.length(), 0, 0);

    while (InternetReadFile(hData, buf, 0x1000 - 1, &bytesRead) && bytesRead != 0) {
        buf[bytesRead] = '\0';
        total = total + buf;
        totalBytesRead += bytesRead;
    }
    InternetCloseHandle(hData);
    return total;
}