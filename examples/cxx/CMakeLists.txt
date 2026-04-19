file(GLOB TEST_FILES
        #"${CMAKE_CURRENT_SOURCE_DIR}/*.cpp"
        #"${CMAKE_CURRENT_SOURCE_DIR}/*.cxx"
        ${CMAKE_CURRENT_SOURCE_DIR}/example_simple.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/example_interpolator.cpp
)

# Create a separate executable target for each test file
foreach(TEST_FILE ${TEST_FILES})
    # Get the filename without path and extension
    get_filename_component(TEST_NAME ${TEST_FILE} NAME_WE)

    # Create executable for this test
    add_executable(${TEST_NAME} ${TEST_FILE})

    target_include_directories(${TEST_NAME} PRIVATE
            ${CMAKE_SOURCE_DIR}
            ${CMAKE_CURRENT_SOURCE_DIR}
    )

    set_target_properties(${TEST_NAME} PROPERTIES
            CXX_STANDARD 20
            CXX_STANDARD_REQUIRED ON
    )

    # Link against the Randon library and GTest
    target_link_libraries(${TEST_NAME}
            PRIVATE
            AADShared
    )
endforeach ()

