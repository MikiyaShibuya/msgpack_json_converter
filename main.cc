#include <iostream>
#include <filesystem>
#include <cxxopts.hpp>
#include <nlohmann/json.hpp>
#include <fstream>

int main(int argc, char **argv) {

    // Load opts
    cxxopts::Options opt("msgpack_json_converter", "");
    opt.add_options()("to-json", "convert msgpack to json");
    opt.add_options()("to-msg", "convert json to msgpack");
    opt.add_options()("i,input_path", "input file path", cxxopts::value<std::string>());
    opt.add_options()("o,output_path", "output file path", cxxopts::value<std::string>());

    const auto result = opt.parse(argc, argv);

    const auto to_json = result["to-json"].as<bool>();
    const auto to_msg = result["to-msg"].as<bool>();

    if ((!to_json && !to_msg) || (to_json && to_msg)) {
        std::cout << "Specify --to-json or --to-msg" << std::endl;
        return EXIT_FAILURE;
    }

    // Prepare input file
    const auto input_path = result["input_path"].as<std::string>();
    if (!std::filesystem::exists(input_path)) {
        std::cout << "input file does not exist" << std::endl;
        return EXIT_FAILURE;
    }
    std::ifstream ifs(input_path, std::ios::in | std::ios::binary);
    if (!ifs.is_open()) {
        std::cout << "cannot load the input file" << std::endl;
        return EXIT_FAILURE;
    }

    // Prepare output file
    const auto output_path = result["output_path"].as<std::string>();
    std::ofstream ofs(output_path, std::ios::out | std::ios::binary);
    if (!ofs.is_open()) {
        std::cout << "cannot open the output file" << std::endl;
        return EXIT_FAILURE;
    }

    if (to_json) {
        // Load file
        std::vector<uint8_t> load_data;
        while (true) {
            uint8_t buffer;
            ifs.read(reinterpret_cast<char *>(&buffer), sizeof(uint8_t));
            if (ifs.eof()) {
                break;
            }
            load_data.push_back(buffer);
        }
        ifs.close();
        const auto json = nlohmann::json::from_msgpack(load_data);
        ofs << std::setw(4) << json << std::endl;
    } else {
        nlohmann::json json;
        ifs >> json;
        const auto msgpack = nlohmann::json::to_msgpack(json);
        ofs.write(reinterpret_cast<const char*>(msgpack.data()), msgpack.size() * sizeof(uint8_t));
    }
    ofs.close();

    return 0;
}

