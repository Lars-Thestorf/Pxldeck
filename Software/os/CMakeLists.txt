if(${ESP_PLATFORM})

set(EXTRA_COMPONENT_DIRS "../games/")
idf_component_register(SRCS "HLM_os.cpp"
                    REQUIRES main games
                    INCLUDE_DIRS "." "../games/")
	#set(COMPONENT_SRCS font/font.c font/font0.c font/font1.c font/font2.c lcd_gui.cpp gfx/char.cpp gfx/obj.cpp gfx/rect.cpp gfx/textbox.cpp widget/textbutton.cpp gfx/textline.cpp widget/widget.cpp widget/button.cpp widget/slider.cpp widget/imagebutton.cpp widget/keyboard.cpp widget/textinput.cpp gfx/dot_field.cpp lcd_gui_textinfo.cpp gfx/image.cpp)
	#set(COMPONENT_ADD_INCLUDEDIRS ".")
	#register_component()
	

else()
	
	add_subdirectory("../games/" HLM-games)
	
	include_directories("../games/")
	
	add_library(HLM-os INTERFACE)
	
	target_sources(HLM-os INTERFACE HLM_os.cpp)

	target_include_directories (HLM-os INTERFACE ${CMAKE_CURRENT_SOURCE_DIR} HLM-games)
	
	target_link_libraries(HLM-os INTERFACE HLM-games)

endif()
 
