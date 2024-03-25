file(GLOB CLIENT_HEADER_LIST CONFIGURE_DEPENDS "${CalvisionPi_SOURCE_DIR}/client/*.h")
file(GLOB CLIENT_SOURCE_LIST CONFIGURE_DEPENDS "${CalvisionPi_SOURCE_DIR}/client/*.cpp")

add_executable(client ${COMMON_HEADER_LIST} ${CLIENT_HEADER_LIST})

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
