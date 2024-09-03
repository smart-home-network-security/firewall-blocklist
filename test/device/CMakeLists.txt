# Minimum required CMake version
cmake_minimum_required(VERSION 3.20)

set(EXECUTABLE_OUTPUT_PATH ${BIN_DIR})

# Nfqueue C file for device tplink-plug
add_executable(tplink-plug nfqueues.c)
target_link_libraries(tplink-plug pthread)
IF( OPENWRT_CROSSCOMPILING )
target_link_libraries(tplink-plug jansson mnl nfnetlink nftnl nftables netfilter_queue netfilter_log)
ENDIF()
target_link_libraries(tplink-plug nfqueue packet_utils rule_utils)
target_link_libraries(tplink-plug header dns)
target_include_directories(tplink-plug PRIVATE ${INCLUDE_DIR} ${INCLUDE_PARSERS_DIR})
install(TARGETS tplink-plug DESTINATION ${EXECUTABLE_OUTPUT_PATH})