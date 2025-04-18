#include "Log.h"


std::string Logger::currentTimestamp() {
    std::time_t now = std::time(nullptr);
    std::tm* timeinfo = std::localtime(&now);
    char buffer[20];
    std::strftime(buffer, sizeof(buffer), "%y.%m.%d %H.%M.%S", timeinfo);
    return std::string(buffer);
}

std::string Logger::formatLogMessage(const std::string& classname, const std::string& message) {
    std::ostringstream oss;
    char timeBuffer[20];
    std::time_t now = std::time(nullptr);
    std::strftime(timeBuffer, sizeof(timeBuffer), "%y.%m.%d %H.%M", std::localtime(&now));
 
    oss << "[" << timeBuffer << "] ";
    oss << std::left << std::setw(30) << classname << ":";
    oss << message;
    return oss.str();
}

void Logger::archiveOldestIfNecessary() {
    std::vector<fs::directory_entry> logBackups;

    for (const auto& entry : fs::directory_iterator(".")) {
        if (entry.is_regular_file()) {
            std::string name = entry.path().filename().string();

            if (entry.path().extension() == ".log") {
                logBackups.push_back(entry);
            }
        }
    }
    if (logBackups.size() >= 2) {
        // 오래된 순 정렬
        std::sort(logBackups.begin(), logBackups.end(),
                  [](const fs::directory_entry& a, const fs::directory_entry& b) {
                    return fs::last_write_time(a) < fs::last_write_time(b);
                  });

        // 가장 오래된 log 파일만 .zip으로 이름 변경
        const auto& oldestUnzipped = logBackups.front();

        fs::path oldPath = logBackups.front().path();
        fs::path newPath = oldPath;
        newPath.replace_extension(".zip");
        fs::rename(oldPath, newPath);
    }
}

void Logger::log(const std::string& classname, const std::string& message) {
    std::string logFile = "latest.log";
    std::string logMessage = formatLogMessage(classname, message);

    // 콘솔 출력
    std::cout << logMessage << std::endl;

    // 현재 파일 크기 확인
    std::uintmax_t size = fs::exists(logFile) ? fs::file_size(logFile) : 0;

    if (size >= 10 * 1024) {
    // 백업 파일명 생성
        std::string date = currentTimestamp();
        std::string backupName = "until_" + currentTimestamp() + ".log";
        fs::rename(logFile, backupName);
        archiveOldestIfNecessary();  // 백업이 3개 이상이면 .zip 처리
    }

    // 로그 파일에 추가
    std::ofstream out(logFile, std::ios::app);
    if (out.is_open()) {
        out << logMessage << std::endl;
    }
}
