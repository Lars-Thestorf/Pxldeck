if(${ESP_PLATFORM})

	idf_component_register(SRCS "pong/pong.cpp" "Snake/snake_game.cpp" "Snake/snake.cpp" "Snake/effects.cpp" "Snake/point.h" "tetris/tetris.cpp" "tetris/tetris_draw.cpp" "tetris/tetris_game.cpp" "sokoban/sokoban.cpp" "sokoban/level.cpp"
                    REQUIRES api
                    INCLUDE_DIRS "." "../api/include/")

else()

	add_library(HLM-games INTERFACE)
	
	include_directories("../os")
	
	target_sources(HLM-games INTERFACE "pong/pong.cpp" "Snake/snake_game.cpp" "Snake/snake.cpp" "Snake/effects.cpp" "Snake/point.h" "tetris/tetris.cpp" "tetris/tetris_draw.cpp" "tetris/tetris_game.cpp" "sokoban/sokoban.cpp" "sokoban/level.cpp")

	target_include_directories (HLM-games INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})

endif()
 
