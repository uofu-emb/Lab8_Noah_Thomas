# This is the main binary. List your C files here.
add_executable(can_development_rx
    can_development_rx.c
    )

add_executable(can_development_tx
    can_development_tx.c
    )

add_executable(can_development_server
    can_development_server.c
    )

add_executable(can_development_server_lp
    can_development_server_lp.c
    )

pico_set_program_name(can_development_rx "test")
pico_set_program_version(can_development_rx "0.1")

pico_set_program_name(can_development_tx "test")
pico_set_program_version(can_development_tx "0.1")

pico_set_program_name(can_development_server "test")
pico_set_program_version(can_development_server "0.1")

pico_set_program_name(can_development_server_lp "test")
pico_set_program_version(can_development_server_lp "0.1")

# Modify the below lines to enable/disable output over UART/USB.
# These are mutually exclusive, only set one.
pico_enable_stdio_uart(can_development_rx 0)
pico_enable_stdio_usb(can_development_rx 1)
pico_enable_stdio_uart(can_development_tx 0)
pico_enable_stdio_usb(can_development_tx 1)
pico_enable_stdio_uart(can_development_server 0)
pico_enable_stdio_usb(can_development_server 1)
pico_enable_stdio_uart(can_development_server_lp 0)
pico_enable_stdio_usb(can_development_server_lp 1)

# Add the libraries to the build
target_link_libraries(can_development_rx PRIVATE
  pico_stdlib
  pico_multicore
  pico_async_context_freertos
  FreeRTOS-Kernel-Heap4
  can2040
)

target_link_libraries(can_development_tx PRIVATE
  pico_stdlib
  pico_multicore
  pico_async_context_freertos
  FreeRTOS-Kernel-Heap4
  can2040
)

target_link_libraries(can_development_server PRIVATE
  pico_stdlib
  pico_multicore
  pico_async_context_freertos
  FreeRTOS-Kernel-Heap4
  can2040
)

target_link_libraries(can_development_server_lp PRIVATE
  pico_stdlib
  pico_multicore
  pico_async_context_freertos
  FreeRTOS-Kernel-Heap4
  can2040
)

# The CYW43 is the Wifi/Bluetooth module. If the board is set to pico_w, this
# variable will be set and the wireless libraries added.
if(PICO_CYW43_SUPPORTED)
    # For led support on pico_w
    target_link_libraries(can_development_rx PRIVATE
        pico_cyw43_arch_none
    )

    # For led support on pico_w
    target_link_libraries(can_development_tx PRIVATE
        pico_cyw43_arch_none
    )

    # For led support on pico_w
    target_link_libraries(can_development_server PRIVATE
        pico_cyw43_arch_none
    )

    # For led support on pico_w
    target_link_libraries(can_development_server_lp PRIVATE
        pico_cyw43_arch_none
    )
endif()

# Another pico macro which generates the uf2 file in addition to the elf
pico_add_extra_outputs(can_development_rx)
pico_add_extra_outputs(can_development_tx)
pico_add_extra_outputs(can_development_server)
pico_add_extra_outputs(can_development_server_lp)

# add_custom_target(flash picotool load -f $<TARGET_FILE:can_development_rx> DEPENDS can_development_rx)
