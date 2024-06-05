#include <iostream>
#include <fstream>
#include <vector>
#include <string>

const size_t CHUNK_SIZE = 25 * 1024 * 1024; // 25MB max file volume without Discord Nitro

void splitFile(const std::string& filePath) {
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

void joinFiles(const std::string& outputFilePath, const std::vector<std::string>& chunkPaths) {
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

// int main() {
//     // std::string filePath = "./test.pdf";
//     // splitFile(filePath);
//     std::vector<std::string> chunkPaths = {
//         "./test.pdf.part0",
//         "./test.pdf.part1",
//         "./test.pdf.part2"
//     };
//     joinFiles("./testrejoined.pdf", chunkPaths);
//     return 0;
// }
