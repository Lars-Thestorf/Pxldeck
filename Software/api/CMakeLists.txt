
if(${ESP_PLATFORM})

	idf_component_register(INCLUDE_DIRS "include/")

else()

	add_library(HLM-api INTERFACE)

	target_include_directories(HLM-api INTERFACE "include/")

endif()
 
