find_package(Qt6 REQUIRED COMPONENTS Core)
qt_standard_project_setup()

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