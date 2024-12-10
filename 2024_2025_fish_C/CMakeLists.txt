cmake_minimum_required(VERSION 3.25)
project(2024_2025_fish_C C)

set(CMAKE_C_STANDARD 11)
set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra")

include_directories (
        # windows
        ${CMAKE_CURRENT_SOURCE_DIR}/FishFeederGUI/customjre/include
        ${CMAKE_CURRENT_SOURCE_DIR}/FishFeederGUI/customjre/include/win32
        ${CMAKE_CURRENT_SOURCE_DIR}/Main
        ${CMAKE_CURRENT_SOURCE_DIR}/Fish
        ${CMAKE_CURRENT_SOURCE_DIR}/Menu
        ${CMAKE_CURRENT_SOURCE_DIR}/Feeder
        ${CMAKE_CURRENT_SOURCE_DIR}/DateTime
        ${CMAKE_CURRENT_SOURCE_DIR}/SystemTime
        ${CMAKE_CURRENT_SOURCE_DIR}/ScheduleTime
        ${CMAKE_CURRENT_SOURCE_DIR}/UserControl
        ${CMAKE_CURRENT_SOURCE_DIR}/File
        ${CMAKE_CURRENT_SOURCE_DIR}/Structs
)

add_executable(2024_2025_fish_C Main/main.c
        Fish/fish.c
        Fish/fish.h

        Menu/menu.c
        Menu/menu.h

        Feeder/feeder.c
        Feeder/feeder.h

        DateTime/dateTime.c
        DateTime/dateTime.h

        SystemTime/time.c
        SystemTime/time.h

        ScheduleTime/scheduleTime.c
        ScheduleTime/scheduleTime.h

        UserControl/userControl.c
        UserControl/userControl.h

        File/file.c
        File/file.h

        Structs/textStruct.h
        Structs/lineStruct.h
        Structs/dateTimeStruct.h
        Structs/FeedingScheduleStruct.h
)

target_link_libraries(2024_2025_fish_C)

target_link_libraries (
        ${PROJECT_NAME} PUBLIC

        # windows
        ${CMAKE_CURRENT_SOURCE_DIR}/FishFeederGUI/customjre/lib/jvm.lib
        ${CMAKE_CURRENT_SOURCE_DIR}/FishFeederGUI/customjre/bin/server/jvm.dll

        # IMPORTANT in addition to the lines above in CLion edit run configuration and
        # set an Environment Variable for Path to allow the jvm.dll to be found in the custom jre as follows:
        # Path=$PROJECT_DIR$\FishFeederGUI\customjre\bin\server\

        # if running on the command line in Windows set paths in terminal to find cygwin1.dll and the jvm.dll
        # set PATH=C:\path_to_project\2024_2025_fish_C\FishFeederGUI\customjre\bin\server
        # set PATH=%PATH%;C:\cygwin64\bin
)
