#phlex-independent physics algorithm
add_library(simulate_wires OBJECT simulate_wires.cpp)
target_link_libraries(simulate_wires)

#generate ROOT dictionary for raw::wire_readout
add_library(wire_readout_obj OBJECT wire_readout.cpp)
set_target_properties(wire_readout_obj PROPERTIES POSITION_INDEPENDENT_CODE ON)
add_library(wire_readout SHARED $<TARGET_OBJECTS:wire_readout_obj>)

reflex_generate_dictionary(wire_readout dict.h SELECTION classes_def.xml)
target_link_libraries(wire_readout PRIVATE ROOT::RIO)

#phlex provider plugin library
add_library(simulate_wires_provider MODULE simulate_wires_provider.cpp)
target_link_libraries(simulate_wires_provider PRIVATE simulate_wires wire_readout phlex::module form)
install(TARGETS simulate_wires_provider DESTINATION lib)
