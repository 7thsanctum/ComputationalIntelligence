add_executable(TSPSolver Main.cpp Point.cpp Point.h)

file(GLOB_RECURSE data_files *.tsp)
foreach(data_file ${data_files})
    file(COPY ${data_files} DESTINATION ${PROJECT_BINARY_DIR})
endforeach(data_file)