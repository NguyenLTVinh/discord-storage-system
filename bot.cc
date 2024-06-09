// #include "filesplitter.h"
#include <dpp/dpp.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <cstdlib>

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

int main() {
    load_env();

    const char* bot_token = std::getenv("BOT_TOKEN");

    std::cout << bot_token;
    if (bot_token == nullptr) {
        std::cerr << "Error: BOT_TOKEN not set in .env file" << std::endl;
        return 1;
    }
    const char* id = std::getenv("CHANNEL_ID");
    if (id == nullptr) {
        std::cerr << "Error: CHANNEL_ID not set in .env file" << std::endl;
        return 1;
    }
    dpp::cluster bot(bot_token);
    bot.on_log(dpp::utility::cout_logger());
    dpp::snowflake channel_id(id);

    bot.on_ready([&bot, &channel_id](const dpp::ready_t& event) {
        dpp::message msg(channel_id, "Take this code file!");
        msg.add_file("main.cc", dpp::utility::read_file("./main.cc"));
        bot.message_create(msg, [](const dpp::confirmation_callback_t& callback) {
            if (callback.is_error()) {
                std::cerr << "Error sending message: " << callback.get_error().message << std::endl;
                exit(1);
            } else {
                std::cout << "Message sent successfully: " << std::get<dpp::message>(callback.value).content << std::endl;
                exit(0);
            }
        });
    });
    
    bot.start(dpp::st_wait);
    return 0;
}