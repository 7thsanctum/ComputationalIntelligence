add_executable(classifier Main.cpp)

file(GLOB_RECURSE data_files *.txt)
foreach(data_file ${data_files})
    file(COPY ${data_files} DESTINATION ${PROJECT_BINARY_DIR})
endforeach(data_file)