if(${ESP_PLATFORM})

#idf_component_register(SRCS "pong/pong.cpp" "tetris/tetris.cpp" "tetris/tetris_draw.cpp" "tetris/tetris_game.cpp"
#                    INCLUDE_DIRS "." "../os/")
idf_component_register(SRCS "pong/pong.cpp"
                    REQUIRES api
                    INCLUDE_DIRS "." "../api/include/")
#set(COMPONENT_SRCS font/font.c font/font0.c font/font1.c font/font2.c lcd_gui.cpp gfx/char.cpp gfx/obj.cpp gfx/rect.cpp gfx/textbox.cpp widget/textbutton.cpp gfx/textline.cpp widget/widget.cpp widget/button.cpp widget/slider.cpp widget/imagebutton.cpp widget/keyboard.cpp widget/textinput.cpp gfx/dot_field.cpp lcd_gui_textinfo.cpp gfx/image.cpp)
#	set(COMPONENT_ADD_INCLUDEDIRS ".")
#	register_component()

else()

	add_library(HLM-games INTERFACE)
	
	include_directories("../os")
	
	target_sources(HLM-games INTERFACE "pong/pong.cpp")

	target_include_directories (HLM-games INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})

endif()
 
