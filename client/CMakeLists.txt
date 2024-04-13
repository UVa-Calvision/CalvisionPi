file(GLOB CLIENT_HEADER_LIST CONFIGURE_DEPENDS "${CalvisionPi_SOURCE_DIR}/client/*.h")
file(GLOB CLIENT_SOURCE_LIST CONFIGURE_DEPENDS "${CalvisionPi_SOURCE_DIR}/client/*.cpp")

add_executable(client)

target_sources(client PRIVATE
    ${CLIENT_SOURCE_LIST}
    ${COMMON_SOURCE_LIST}
)

target_include_directories(client PUBLIC
    "${CalvisionPi_SOURCE_DIR}/common"
    "${CalvisionPi_SOURCE_DIR}/client"
)

target_link_libraries(client PRIVATE CppUtils::CppUtilsIO CppUtils::CppUtilsNetworking)
target_compile_features(client PUBLIC cxx_std_14)

# target_precompile_headers(client PRIVATE ${COMMON_HEADER_LIST} ${CLIENT_HEADER_LIST})

install(TARGETS client
    LIBRARY DESTINATION "lib"
    ARCHIVE DESTINATION "lib"
    RUNTIME DESTINATION "bin"
)
