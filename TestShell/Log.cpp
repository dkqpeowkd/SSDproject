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
        // ������ �� ����
        std::sort(logBackups.begin(), logBackups.end(),
                  [](const fs::directory_entry& a, const fs::directory_entry& b) {
                    return fs::last_write_time(a) < fs::last_write_time(b);
                  });

        // ���� ������ log ���ϸ� .zip���� �̸� ����
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

    // �ܼ� ���
    std::cout << logMessage << std::endl;

    // ���� ���� ũ�� Ȯ��
    std::uintmax_t size = fs::exists(logFile) ? fs::file_size(logFile) : 0;

    if (size >= 10 * 1024) {
    // ��� ���ϸ� ����
        std::string date = currentTimestamp();
        std::string backupName = "until_" + currentTimestamp() + ".log";
        fs::rename(logFile, backupName);
        archiveOldestIfNecessary();  // ����� 3�� �̻��̸� .zip ó��
    }

    // �α� ���Ͽ� �߰�
    std::ofstream out(logFile, std::ios::app);
    if (out.is_open()) {
        out << logMessage << std::endl;
    }
}
