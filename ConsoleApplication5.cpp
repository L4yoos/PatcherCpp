#include <iostream>
#include <fstream>
#include <sstream>
#include <cstdlib>
#include <cstring>
#include <filesystem>
#include <curl/curl.h>
#include <boost/crc.hpp>

namespace fs = std::filesystem;

size_t WriteCallback(void* contents, size_t size, size_t nmemb, std::string* output) {
    size_t totalSize = size * nmemb;
    output->append(static_cast<char*>(contents), totalSize);
    return totalSize;
}

bool fileExists(const std::string& filePath) {
    try {
        return std::filesystem::exists(filePath);
    }
    catch (const std::exception& e) {
        std::cerr << "Exception in fileExists: " << e.what() << std::endl;
        return false;
    }
}

uint32_t calculateCRC(const std::string& filename) {
    std::ifstream file(filename, std::ios::binary);
    if (!file) {
        std::cerr << "Error opening file: " << filename << std::endl;
        return 0;
    }

    boost::crc_32_type result;
    char buffer[4096];
    while (file.read(buffer, sizeof(buffer))) {
        result.process_bytes(buffer, sizeof(buffer));
    }

    result.process_bytes(buffer, file.gcount());

    return result.checksum();
}

std::streamsize getFileSize(const std::string& filename) {
    std::ifstream file(filename, std::ios::binary | std::ios::ate);
    if (!file) {
        std::cerr << "Error opening file: " << filename << std::endl;
        return -1;
    }

    return file.tellg();
}

bool downloadFile(const std::string& url, const std::string& destination) {
    try {
        fs::create_directories(fs::absolute(destination).parent_path());

        FILE* file;
        if (fopen_s(&file, destination.c_str(), "wb") != 0) {
            std::cerr << "Failed to open file for writing: " << destination << std::endl;
            return false;
        }

        CURL* curl = curl_easy_init();
        if (curl) {
            curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
            curl_easy_setopt(curl, CURLOPT_WRITEDATA, file);

            CURLcode res = curl_easy_perform(curl);
            fclose(file);
            curl_easy_cleanup(curl);

            return res == CURLE_OK;
        }
        return false;
    }
    catch (const std::exception& e) {
        std::cerr << "Exception in downloadFile: " << e.what() << std::endl;
        return false;
    }
}


int main() {
    CURL* curl;
    CURLcode res;

    curl_global_init(CURL_GLOBAL_DEFAULT);

    curl = curl_easy_init();

    if (curl) {
        const char* url = "https://example.com/patchlist.txt";
        curl_easy_setopt(curl, CURLOPT_URL, url);

        std::string responseBuffer;
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &responseBuffer);

        res = curl_easy_perform(curl);
        if (res != CURLE_OK) {
            fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
        }
        else {
            std::cout << "Contents of patchlist.txt:\n" << responseBuffer << std::endl;

            std::istringstream iss(responseBuffer);
            std::string line;
            while (std::getline(iss, line)) {

                std::istringstream lineStream(line);
                std::string filePath, crc, size;
                lineStream >> filePath >> crc >> size;
                std::cout << "File Path: " << filePath << std::endl;

                if (fileExists(filePath)) {
                    std::cout << "File exists: " << filePath << std::endl;

                    uint32_t calculatedCRC = calculateCRC(filePath);
                    std::streamsize fileSize = getFileSize(filePath);


                    if (calculatedCRC == std::stoul(crc, nullptr, 16)) {
                        std::cout << "CRC match." << std::endl;
                    }

                    else {
                        std::cout << "CRC dismatch!" << std::endl;

                        fs::remove(filePath);

                        if (downloadFile("https://example.com/" + filePath, filePath)) {
                            std::cout << "File downloaded: " << filePath << std::endl;
                        }
                        else {
                            std::cerr << "Failed to download file: " << filePath << std::endl;
                            return 0;
                        }
                    }

                    if (fileSize == std::stoll(size)) {
                        std::cout << "Size match." << std::endl;
                    }

                    else {
                        std::cout << "Size dismatch!" << std::endl;

                        fs::remove(filePath);

                        if (downloadFile("https://example.com/" + filePath, filePath)) {
                            std::cout << "File downloaded: " << filePath << std::endl;
                        }
                        else {
                            std::cerr << "Failed to download file: " << filePath << std::endl;
                            return 0;
                        }
                    }
                }
                else {
                    std::cout << "File does not exist: " << filePath << std::endl;

                    if (downloadFile("https://example.com/" + filePath, filePath)) {
                        std::cout << "File downloaded: " << filePath << std::endl;

                        uint32_t calculatedCRC = calculateCRC(filePath);
                        std::streamsize fileSize = getFileSize(filePath);

                        if (calculatedCRC == std::stoul(crc, nullptr, 16) && fileSize == std::stoll(size)) {
                            std::cout << "CRC and size match." << std::endl;
                        }
                        else {
                            std::cout << "CRC or size mismatch!" << std::endl;
                            return 0;
                        }
                    }
                    else {
                        std::cerr << "Failed to download file: " << filePath << std::endl;
                        return 0;
                    }
                }
            }
        }
        curl_easy_cleanup(curl);
    }
    curl_global_cleanup();

    return 1;
}
