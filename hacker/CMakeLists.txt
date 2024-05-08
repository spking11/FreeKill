find_package(Qt6 REQUIRED COMPONENTS Core)

qt_add_executable(record_master_md5 
    record_master_md5.cpp
)
target_include_directories(record_master_md5 PRIVATE 
    .
)
target_link_libraries(record_master_md5 PRIVATE
    Qt6::Core
    git2
)

set(HACKER_FLIST ${CMAKE_BINARY_DIR}/hacker_flist.txt)

add_custom_command(
    OUTPUT ${HACKER_FLIST}
    COMMAND $<TARGET_FILE:record_master_md5>
    DEPENDS record_master_md5
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
)

add_custom_target(HACKER_FLIST_TARGET DEPENDS ${HACKER_FLIST})
add_dependencies(FreeKill HACKER_FLIST_TARGET)

target_compile_definitions(FreeKill PRIVATE -DHACKER_FLIST="${HACKER_FLIST}")
