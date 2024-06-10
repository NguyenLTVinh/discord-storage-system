#include "filesplitter.h"
#include <dpp/dpp.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <cstdlib>
#include <filesystem>
#include <vector>

void load_env() {
    std::ifstream env_file(".env");
    if (!env_file.is_open()) {
        std::cerr << "Error: Unable to open .env file" << std::endl;
        return;
    }

    std::string line;
    while (std::getline(env_file, line)) {
        if (line.empty() || line[0] == '#') {
            continue;
        }

        std::istringstream iss(line);
        std::string key;
        if (std::getline(iss, key, '=')) {
            std::string value;
            if (std::getline(iss, value)) {
                setenv(key.c_str(), value.c_str(), 1);
            }
        }
    }
    env_file.close();
}

// int upload_file(dpp::cluster& bot, const dpp::snowflake& channel_id, const std::string& file_path) {
//     std::filesystem::path path(file_path);
//     std::string file_name = path.filename().string();
//     dpp::message msg(channel_id, file_name);
//     msg.add_file(file_name, dpp::utility::read_file(file_path));
//     bot.message_create(msg, [](const dpp::confirmation_callback_t& callback) {
//         if (callback.is_error()) {
//             std::cerr << "Error sending message: " << callback.get_error().message << std::endl;
//             return -1;
//         } else {
//             std::cout << "Message sent successfully: " << std::get<dpp::message>(callback.value).content << std::endl;
//             return 0;
//         }
//     });
//     return 0;
// }

void upload_files(dpp::cluster& bot, const dpp::snowflake& channel_id, const std::vector<std::string>& files, size_t index) {
    if (index >= files.size()) {
        return; // All files sent
    }

    dpp::message msg(channel_id, files[index]);
    msg.add_file(files[index], dpp::utility::read_file(files[index]));

    bot.message_create(msg, [&bot, &channel_id, &files, index](const dpp::confirmation_callback_t& callback) {
        if (callback.is_error()) {
            // Handle error
        } else {
            upload_files(bot, channel_id, files, index + 1);
        }
    });
}

void download_file(const std::string& file_name, const std::string& save_directory) {
    // TODO: Download Logic.
    std::cout << "Downloading " << file_name << " to " << save_directory << std::endl;
}

int main(int argc, char* argv[]) {
    if (argc < 3) {
        std::cerr << "Usage: " << argv[0] << " upload [FILE PATH] | download [FILE NAME] --output [SAVE DIRECTORY]" << std::endl;
        return 1;
    }

    load_env();

    const char* bot_token = std::getenv("BOT_TOKEN");
    if (bot_token == nullptr) {
        std::cerr << "Error: BOT_TOKEN not set in .env file" << std::endl;
        return 1;
    }
    const char* id = std::getenv("CHANNEL_ID");
    if (id == nullptr) {
        std::cerr << "Error: CHANNEL_ID not set in .env file" << std::endl;
        return 1;
    }

    std::string operation = argv[1];
    dpp::cluster bot(bot_token);
    bot.on_log(dpp::utility::cout_logger());
    dpp::snowflake channel_id(id);
    FileSplitter file_splitter;
    std::vector<std::string> files_to_send;

    if (operation == "upload") {
        std::string file_path = argv[2];
        if (!std::filesystem::exists(file_path)) {
            std::cerr << "Error: File does not exist" << std::endl;
            return 1;
        }

        files_to_send = file_splitter.splitFile(file_path);
        // files_to_send = std::vector<std::string>{"./test.pdf.part0", "./test.pdf.part1", "./test.pdf.part2", "./test.pdf.part3"};
        bot.on_ready([&bot, &channel_id, files_to_send](const dpp::ready_t& event) {
            upload_files(bot, channel_id, files_to_send, 0);
        });

    } else if (operation == "download") {
        std::string file_name = argv[2];
        std::string save_directory = ".";
        if (argc >= 5 && std::string(argv[3]) == "--output") {
            save_directory = argv[4];
        }

        download_file(file_name, save_directory);
        return 0;

    } else {
        std::cerr << "Error: Unknown operation" << std::endl;
        return 1;
    }

    bot.start(dpp::st_wait);
    return 0;
}
