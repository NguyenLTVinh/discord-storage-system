#include "filesplitter.h"

void FileSplitter::splitFile(const std::string& filePath) {
    std::ifstream inputFile(filePath, std::ios::binary);
    if (!inputFile) {
        std::cerr << "Error opening file: " << filePath << std::endl;
        return;
    }

    size_t fileCount = 0;
    std::vector<char> buffer(CHUNK_SIZE);

    while (inputFile) {
        inputFile.read(buffer.data(), buffer.size());
        std::streamsize bytesRead = inputFile.gcount();
        if (bytesRead > 0) {
            std::ofstream outputFile(filePath + ".part" + std::to_string(fileCount), std::ios::binary);
            outputFile.write(buffer.data(), bytesRead);
            fileCount++;
        }
    }
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
