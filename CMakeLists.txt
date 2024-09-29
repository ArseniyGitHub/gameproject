# CMakeList.txt: проект CMake для gameproject; включите исходный код и определения,
# укажите здесь логику для конкретного проекта.
#
cmake_minimum_required (VERSION 3.8)

# Включение горячей перезагрузки для компиляторов MSVC, если поддерживается.
include("D:/libs/vcpkg/scripts/buildsystems/vcpkg.cmake")

if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

project ("gameproject")
file(GLOB_RECURSE SOURCES "src/*.cpp" "include/*.h")


# Добавьте источник в исполняемый файл этого проекта.
add_executable (gameproject  "main.cpp" ${SOURCES})
target_include_directories("gameproject" PRIVATE "include")
find_package(spdlog CONFIG REQUIRED)
target_link_libraries(gameproject PRIVATE spdlog::spdlog)
find_package(box2d CONFIG REQUIRED)
target_link_libraries(gameproject PRIVATE box2d::box2d)
find_package(SFML COMPONENTS system window graphics CONFIG REQUIRED)
target_link_libraries(gameproject PRIVATE sfml-system sfml-network sfml-graphics sfml-window)
#target_link_libraries(CMakeProject2 PRIVATE sfml-main)
find_package(imgui CONFIG REQUIRED)
target_link_libraries(gameproject PRIVATE imgui::imgui)
find_package(ImGui-SFML CONFIG REQUIRED)
target_link_libraries(gameproject PRIVATE ImGui-SFML::ImGui-SFML)
find_package(nlohmann_json CONFIG REQUIRED)
target_link_libraries(gameproject PRIVATE nlohmann_json::nlohmann_json)

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET gameproject PROPERTY CXX_STANDARD 20)
endif()

# TODO: Добавьте тесты и целевые объекты, если это необходимо.
