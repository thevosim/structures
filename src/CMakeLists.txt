add_library(data_structures INTERFACE)

# Указываем путь к заголовочным файлам для тех, кто линкуется с этой библиотекой
target_include_directories(data_structures INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})
