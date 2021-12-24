if(${ESP_PLATFORM})

	set(EXTRA_COMPONENT_DIRS "../games/")
	
	#TODO: fix hacky solution (why does requiring main here fix issue of games not being found?)
	idf_component_register(SRCS "HLM_os.cpp" "mainMenu/mainMenu.cpp"
                    REQUIRES main games
                    INCLUDE_DIRS "../games/")

else()
	
	add_subdirectory("../games/" HLM-games)
	
	include_directories("../games/")
	
	add_library(HLM-os INTERFACE)
	
	target_sources(HLM-os INTERFACE "HLM_os.cpp" "mainMenu/mainMenu.cpp")

	target_include_directories (HLM-os INTERFACE ${CMAKE_CURRENT_SOURCE_DIR} HLM-games)
	
	target_link_libraries(HLM-os INTERFACE HLM-games)

endif()
 
