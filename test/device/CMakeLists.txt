# Minimum required CMake version
cmake_minimum_required(VERSION 3.20)

set(EXECUTABLE_OUTPUT_PATH ${BIN_DIR})

# Nfqueue C file for device tplink-plug
add_executable(test-tplink-plug nfqueues.c)
target_link_libraries(test-tplink-plug pthread)
IF( OPENWRT_CROSSCOMPILING )
target_link_libraries(test-tplink-plug jansson mnl nfnetlink nftnl nftables netfilter_queue netfilter_log)
ENDIF()
target_link_libraries(test-tplink-plug nfqueue packet_utils rule_utils)
target_link_libraries(test-tplink-plug header dns)
target_include_directories(test-tplink-plug PRIVATE ${INCLUDE_DIR} ${INCLUDE_PARSERS_DIR})
install(TARGETS test-tplink-plug DESTINATION ${EXECUTABLE_OUTPUT_PATH})
