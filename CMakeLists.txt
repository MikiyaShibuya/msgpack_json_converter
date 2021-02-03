cmake_minimum_required(VERSION 3.1)
project(msgpack_json_converter)

# find Eigen
find_package(Eigen3 3.3 REQUIRED NO_MODULE)
# find JSON
find_package(nlohmann_json REQUIRED)
find_package(cxxopts REQUIRED)

add_executable(main main.cc)

target_compile_features(main PRIVATE "cxx_std_17")

target_include_directories(main
        PRIVATE
        ${BA_INCLUDE_DIR})
target_link_libraries(main
        PUBLIC
        Eigen3::Eigen
        nlohmann_json::nlohmann_json
        cxxopts::cxxopts)

