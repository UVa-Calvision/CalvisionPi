file(GLOB SERVER_HEADER_LIST CONFIGURE_DEPENDS "${CalvisionPi_SOURCE_DIR}/server/*.h")
file(GLOB SERVER_SOURCE_LIST CONFIGURE_DEPENDS "${CalvisionPi_SOURCE_DIR}/server/*.cpp")

add_executable(server)

target_compile_definitions(server PUBLIC BUILD_SERVER)

target_sources(server PRIVATE
    ${SERVER_SOURCE_LIST}
    ${COMMON_SOURCE_LIST}
)

target_include_directories(server PUBLIC
    "${CalvisionPi_SOURCE_DIR}/common"
    "${CalvisionPi_SOURCE_DIR}/server"
)

target_link_directories(server PRIVATE "${CMAKE_PREFIX_PATH}/lib")
target_link_libraries(server PRIVATE fmt gpiod CppUtils::CppUtilsIO CppUtils::CppUtilsNetworking)
target_compile_features(server PUBLIC cxx_std_14)

target_precompile_headers(server PRIVATE ${COMMON_HEADER_LIST} ${SERVER_HEADER_LIST})

install(TARGETS server
    LIBRARY DESTINATION "lib"
    ARCHIVE DESTINATION "lib"
    RUNTIME DESTINATION "bin"
)
