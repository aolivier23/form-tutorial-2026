add_library(window_hit_finder OBJECT window_hit_finder.cpp)
set_target_properties(window_hit_finder PROPERTIES POSITION_INDEPENDENT_CODE ON)
target_include_directories(window_hit_finder PUBLIC ${CMAKE_SOURCE_DIR})
target_link_libraries(window_hit_finder)

#generate ROOT dicitonary for reco::hit
add_library(hit_obj OBJECT hit.cpp)
set_target_properties(hit_obj PROPERTIES POSITION_INDEPENDENT_CODE ON)
add_library(hit SHARED $<TARGET_OBJECTS:hit_obj>)
set_target_properties(hit PROPERTIES LIBRARY_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})

reflex_generate_dictionary(hit dict.h SELECTION classes_def.xml)
target_link_libraries(hit PRIVATE ROOT::RIO)
install(TARGETS hit DESTINATION lib)
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/hit_rdict.pcm DESTINATION lib)

#phlex provider and transform plugin library
add_library(window_hit_finder_alg MODULE register_window_hit_finder.cpp)
target_link_options(window_hit_finder_alg PRIVATE "LINKER:--no-as-needed")
target_link_libraries(window_hit_finder_alg PRIVATE window_hit_finder hit wire_readout phlex::module phlex::form)
install(TARGETS window_hit_finder_alg DESTINATION lib)
