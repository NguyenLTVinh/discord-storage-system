#ifndef FILESPLITTER_H
#define FILESPLITTER_H

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <list>

const size_t CHUNK_SIZE = 25 * 1024 * 1024; // 25MB max file volume without Discord Nitro
class FileSplitter {
    public:
        std::list<std::string> splitFile(const std::string& filePath);
        void joinFiles(const std::string& outputFilePath, const std::vector<std::string>& chunkPaths);
};

#endif