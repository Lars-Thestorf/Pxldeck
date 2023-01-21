idf_component_register(SRCS "server.cpp" "websocket/websocket.c" "websocket/websocket_server.c"
    INCLUDE_DIRS "websocket/include"
    REQUIRES lwip mbedtls
    EMBED_FILES "server/error.html" "server/style.css" "server/test.js" "server/index.html")
