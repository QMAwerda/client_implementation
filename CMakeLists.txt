cmake_minimum_required(VERSION 3.5)
project(NetClient)

#set(CMAKE_CXX_STANDARD 11)

# Добавляем путь к заголовочным файлам проекта Lib
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../NetCommon/src/HeaderFiles)

# Добавляем библиотеку NetCommon
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/../NetCommon build)

# Собираем исполняемый файл NetClient
add_executable(client src/SourceFiles/SimpleClient.cpp)

set_target_properties(NetCommon PROPERTIES LINKER_LANGUAGE CXX) # ставлю язык линкуемой библиотеки

# Линкуем с библиотекой NetCommon
target_link_libraries(client PUBLIC NetCommon)
