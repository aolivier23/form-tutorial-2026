#phlex-independent physics algorithm
add_library(simulate_wires OBJECT simulate_wires.cpp)
set_target_properties(simulate_wires PROPERTIES POSITION_INDEPENDENT_CODE ON)
target_link_libraries(simulate_wires)

#generate ROOT dictionary for raw::wire_readout
add_library(wire_readout_obj OBJECT wire_readout.cpp)
set_target_properties(wire_readout_obj PROPERTIES POSITION_INDEPENDENT_CODE ON)
add_library(wire_readout SHARED $<TARGET_OBJECTS:wire_readout_obj>)

reflex_generate_dictionary(wire_readout dict.h SELECTION classes_def.xml)
target_link_libraries(wire_readout PRIVATE ROOT::RIO)
install(TARGETS wire_readout DESTINATION lib)
set_target_properties(wire_readout PROPERTIES LIBRARY_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/wire_readout_rdict.pcm DESTINATION lib)

#phlex provider plugin library
add_library(simulate_wires_provider MODULE simulate_wires_provider.cpp)
target_link_options(simulate_wires_provider PRIVATE "LINKER:--no-as-needed")
target_link_libraries(simulate_wires_provider PRIVATE simulate_wires wire_readout phlex::module)
install(TARGETS simulate_wires_provider DESTINATION lib)
