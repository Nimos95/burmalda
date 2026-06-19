cmake_minimum_required(VERSION 3.20)
project(Burmalda)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(SFML_DIR ${CMAKE_CURRENT_SOURCE_DIR}/libs/SFML)
set(IMGUI_DIR ${CMAKE_CURRENT_SOURCE_DIR}/libs/imgui)

include(FetchContent)
set(JSON_BuildTests OFF CACHE INTERNAL "")
FetchContent_Declare(json
    GIT_REPOSITORY https://github.com/nlohmann/json.git
    GIT_TAG v3.11.2
)
FetchContent_MakeAvailable(json)

configure_file(config/reels.json ${CMAKE_CURRENT_BINARY_DIR}/config/reels.json COPYONLY)
configure_file(config/paytable.json ${CMAKE_CURRENT_BINARY_DIR}/config/paytable.json COPYONLY)

add_executable(burmalda_gui
    src/main_gui_sfml.cpp
    src/game_controller.cpp
    src/random_engine.cpp
    src/reel.cpp
    src/paytable.cpp
    ${IMGUI_DIR}/imgui.cpp
    ${IMGUI_DIR}/imgui_draw.cpp
    ${IMGUI_DIR}/imgui_widgets.cpp
    ${IMGUI_DIR}/imgui_tables.cpp
    ${IMGUI_DIR}/imgui-SFML.cpp
)

target_include_directories(burmalda_gui PRIVATE
    include
    ${SFML_DIR}/include
    ${IMGUI_DIR}
)

target_link_directories(burmalda_gui PRIVATE ${SFML_DIR}/lib)

target_link_libraries(burmalda_gui
    sfml-graphics
    sfml-window
    sfml-system
    nlohmann_json::nlohmann_json
    opengl32
)

add_custom_command(TARGET burmalda_gui POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        ${SFML_DIR}/bin/sfml-graphics-2.dll
        ${SFML_DIR}/bin/sfml-window-2.dll
        ${SFML_DIR}/bin/sfml-system-2.dll
        $<TARGET_FILE_DIR:burmalda_gui>
)

add_custom_command(TARGET burmalda_gui POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_CURRENT_SOURCE_DIR}/config
        $<TARGET_FILE_DIR:burmalda_gui>/config
)

add_executable(burmalda_console
    src/main_console.cpp
    src/random_engine.cpp
    src/reel.cpp
    src/paytable.cpp
)

target_include_directories(burmalda_console PRIVATE include)
target_link_libraries(burmalda_console nlohmann_json::nlohmann_json)

message(STATUS "✅ Burmalda with SFML + ImGui configured!")