#include "filesplitter.h"

std::vector<std::string> FileSplitter::splitFile(const std::string& filePath) {
    std::vector<std::string> splitted_paths{};
    std::ifstream inputFile(filePath, std::ios::binary);
    if (!inputFile) {
        std::cerr << "Error opening file: " << filePath << std::endl;
        return splitted_paths;
    }

    size_t fileCount = 0;
    std::vector<char> buffer(CHUNK_SIZE);

    while (inputFile) {
        inputFile.read(buffer.data(), buffer.size());
        std::streamsize bytesRead = inputFile.gcount();
        if (bytesRead > 0) {
            std::ofstream outputFile(filePath + ".part" + std::to_string(fileCount), std::ios::binary);
            splitted_paths.push_back(filePath + ".part" + std::to_string(fileCount));
            outputFile.write(buffer.data(), bytesRead);
            fileCount++;
        }
    }

    return splitted_paths;
}

void FileSplitter::joinFiles(const std::string& outputFilePath, const std::vector<std::string>& chunkPaths) {
    std::ofstream outputFile(outputFilePath, std::ios::binary);

    for (const auto& chunkPath : chunkPaths) {
        std::ifstream inputFile(chunkPath, std::ios::binary);
        if (!inputFile) {
            std::cerr << "Error opening file: " << chunkPath << std::endl;
            return;
        }
        outputFile << inputFile.rdbuf();
    }
}
