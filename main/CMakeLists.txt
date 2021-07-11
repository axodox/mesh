idf_component_register(SRCS 
                        "graphics/colors.cpp"

                        "infrastructure/dependencies.cpp"
                        "infrastructure/error.cpp"
                        "infrastructure/event_loop.cpp"
                        "infrastructure/hasher.cpp"
                        "infrastructure/logger.cpp"
                        "infrastructure/text.cpp"

                        "networking/mime_type.cpp"
                        "networking/http_query_method.cpp"
                        "networking/http_text_stream.cpp"
                        "networking/http_query.cpp"
                        "networking/http_server.cpp"
                        "networking/ntp_client.cpp"
                        "networking/wifi.cpp"

                        "numerics/float3.cpp"

                        "peripherals/integrated_led.cpp"
                        "peripherals/ws2812_strip.cpp"

                        "storage/embedded.cpp"
                        "storage/nvs.cpp"

                        "app/angular_pages.cpp"
                        "app/integrated_led_blinker.cpp"
                        "main.cpp"
                        INCLUDE_DIRS "."
                       EMBED_FILES 
                        "www/favicon.png.gz"
                        "www/index.html.gz"
                        "www/main.js.gz"
                        "www/polyfills.js.gz"
                        "www/runtime.js.gz"
                        "www/styles.css.gz")