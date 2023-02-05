if(${ESP_PLATFORM})

	idf_component_register(
        SRCS "inputEvents.cpp" "defaultInputEvents.cpp"
        REQUIRES api
        INCLUDE_DIRS "include/")

else()

	add_library(HLM-libs INTERFACE)

    target_sources(HLM-libs INTERFACE "inputEvents.cpp" "defaultInputEvents.cpp")

	target_include_directories(HLM-libs INTERFACE "include/")

endif()