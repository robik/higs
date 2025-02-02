add_executable(higs-cli main.cpp)
target_link_libraries(higs-cli PRIVATE
    Hermes::Hermes
    Boost::pfr
    Boost::program_options
    higs
)