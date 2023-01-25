cmake_minimum_required (VERSION 3.0) 
project(OnlineJudge)

# 配置编译选项
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -std=c++11")

#设置可执行文件最终存储的路径
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)
#设置库文件路径，类似-L
link_directories(./lib)

aux_source_directory(./src SRC_LIST)
aux_source_directory(./src/db DB_LIST)
aux_source_directory(./src/tinymuduo/base TINYMUDUO_LIST)
aux_source_directory(./src/tinymuduo/net TINYMUDUO_LIST)

add_executable(OnlineJudge ${SRC_LIST} ${DB_LIST} ${TINYMUDUO_LIST} ${TINYMUDUO_LIST})

include_directories(${PROJECT_SOURCE_DIR}/include)

# 表示server这个目标程序，需要链接的库
target_link_libraries(OnlineJudge pthread jsoncpp ctemplate mysqlclient)

