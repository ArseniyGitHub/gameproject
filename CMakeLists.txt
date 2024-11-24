# CMakeList.txt: проект CMake для gameproject; включите исходный код и определения,
# укажите здесь логику для конкретного проекта.
#
cmake_minimum_required (VERSION 3.8)

# Включение горячей перезагрузки для компиляторов MSVC, если поддерживается.
if(DEFINED ENV{VCPKG_HOME})
    
    include("$ENV{VCPKG_HOME}/scripts/buildsystems/vcpkg.cmake")
else()
    include("$ENV{VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake")
endif()
set     (VCPKG_INCLUDE_DIR "$ENV{VCPKG_HOME}/installed/${VCPKG_TARGET_TRIPLET}/include")
set         (VCPKG_LIB_DIR "$ENV{VCPKG_HOME}/installed/${VCPKG_TARGET_TRIPLET}/lib")
set             (VCPKG_BIN_DIR "$ENV{VCPKG_HOME}/installed/${VCPKG_TARGET_TRIPLET}/bin")
set (VCPKG_DEBUG_BIN_DIR "$ENV{VCPKG_HOME}/installed/${VCPKG_TARGET_TRIPLET}/debug/bin")
set (VCPKG_DEBUG_LIB_DIR "$ENV{VCPKG_HOME}/installed/${VCPKG_TARGET_TRIPLET}/debug/lib")
include_directories(VCPKG_INCLUDE_DIR)
add_subdirectory("modules/Candle")


if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

project ("gameproject")
file(GLOB_RECURSE SOURCES "src/*.cpp" "include/*.h")

find_package(spdlog CONFIG REQUIRED)
find_package(box2d CONFIG REQUIRED)
find_package(SFML COMPONENTS system window graphics CONFIG REQUIRED)
find_package(imgui CONFIG REQUIRED)
find_package(ImGui-SFML CONFIG REQUIRED)
find_package(nlohmann_json CONFIG REQUIRED)
find_package(unofficial-sqlite3 CONFIG REQUIRED)
find_package(implot CONFIG REQUIRED)

# Добавьте источник в исполняемый файл этого проекта.
add_executable (gameproject  "main.cpp" ${SOURCES})
add_executable  (thor_test    "tests/ThorTest.cpp")
add_executable (candle_test "tests/CandleTest.cpp")
add_executable(network_test_server "tests/server_data/server.cpp" "tests/server_data/server.h" "tests/server_data/main.cpp")
add_executable(network_test_client "tests/client_data/client.cpp" "tests/client_data/client.h" "tests/client_data/main.cpp" "tests/client_data/window.h" "tests/client_data/window.cpp")

target_include_directories("gameproject" PRIVATE "include")
#########################################################################################################################################################################################################
target_link_libraries(gameproject PRIVATE spdlog::spdlog)
target_link_libraries(network_test_server PRIVATE spdlog::spdlog sfml-system sfml-network sfml-graphics sfml-window nlohmann_json::nlohmann_json unofficial::sqlite3::sqlite3 imgui::imgui implot::implot ImGui-SFML::ImGui-SFML)
target_link_libraries(network_test_client PRIVATE spdlog::spdlog sfml-system sfml-network sfml-graphics sfml-window nlohmann_json::nlohmann_json unofficial::sqlite3::sqlite3 imgui::imgui implot::implot ImGui-SFML::ImGui-SFML)
target_link_libraries(gameproject PRIVATE box2d::box2d)
target_link_libraries(gameproject PRIVATE sfml-system sfml-network sfml-graphics sfml-window)
target_link_libraries(gameproject PRIVATE imgui::imgui)
target_link_libraries(gameproject PRIVATE ImGui-SFML::ImGui-SFML)
target_link_libraries(gameproject PRIVATE nlohmann_json::nlohmann_json)
target_link_libraries(gameproject PRIVATE Candle-s)
target_link_libraries(gameproject PRIVATE unofficial::sqlite3::sqlite3)
target_link_libraries(gameproject PRIVATE implot::implot)



target_link_libraries(thor_test   PRIVATE sfml-system sfml-graphics sfml-window)
target_link_libraries(candle_test PRIVATE sfml-system sfml-graphics sfml-window Candle-s)
##################################################################################################################################################################################################################################################################################################################################################################################################################

#target_link_libraries(network_test3 PRIVATE sfml-system sfml-graphics sfml-window implot::implot ImGui-SFML::ImGui-SFML nlohmann_json::nlohmann_json unofficial::sqlite3::sqlite3 imgui::imgui sfml-network)

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    target_link_libraries("gameproject" PRIVATE "${VCPKG_DEBUG_LIB_DIR}/thor-d.lib")
    target_link_libraries(thor_test PRIVATE "${VCPKG_DEBUG_LIB_DIR}/thor-d.lib")
else()
    target_link_libraries("gameproject" PRIVATE "${VCPKG_LIB_DIR}/thor.lib")
    target_link_libraries(thor_test PRIVATE "${VCPKG_LIB_DIR}/thor.lib")
endif()
set(THOR_DLL_PATH_RELEASE "${VCPKG_BIN_DIR}/thor.dll")
set(THOR_DLL_PATH_DEBUG "${VCPKG_DEBUG_BIN_DIR}/thor-d.dll")

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET gameproject PROPERTY CXX_STANDARD 23)
  set_property(TARGET thor_test PROPERTY CXX_STANDARD 23)
  set_property(TARGET candle_test PROPERTY CXX_STANDARD 23)
  set_property(TARGET network_test_server PROPERTY CXX_STANDARD 23)
  set_property(TARGET network_test_client PROPERTY CXX_STANDARD 23)
endif()

add_custom_command(TARGET gameproject POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy_if_different
    $<$<CONFIG:Debug>:${THOR_DLL_PATH_DEBUG}>
    $<$<CONFIG:Release>:${THOR_DLL_PATH_RELEASE}>
    $<TARGET_FILE_DIR:gameproject>
)

set(SOURSE_DIR "${CMAKE_SOURCE_DIR}/assets")
set(DESTINATION_DIR "${CMAKE_BINARY_DIR}")
file(COPY ${SOURSE_DIR} DESTINATION ${DESTINATION_DIR})



# TODO: Добавьте тесты и целевые объекты, если это необходимо.
