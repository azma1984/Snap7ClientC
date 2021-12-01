cmake_minimum_required(VERSION 3.5)
project(Snap7ClientC LANGUAGES C)

include_directories(snap7-full-1.4.2/src
                    snap7-full-1.4.2/src/core
                    snap7-full-1.4.2/src/sys
                    )

add_executable(Snap7ClientC
                            snap7-full-1.4.2/src/sys/snap_threads.h
                            snap7-full-1.4.2/src/sys/snap_threads.c
                            snap7-full-1.4.2/src/sys/snap_msgsock.h
                            snap7-full-1.4.2/src/sys/snap_msgsock.c
                            snap7-full-1.4.2/src/sys/snap_platform.h
                            snap7-full-1.4.2/src/sys/sol_threads.h
                            snap7-full-1.4.2/src/sys/snap_sysutils.h
                            snap7-full-1.4.2/src/sys/snap_sysutils.c
                            snap7-full-1.4.2/src/sys/win_threads.h

                            snap7-full-1.4.2/src/core/s7_micro_client.h
                            snap7-full-1.4.2/src/core/s7_micro_client.c
                            snap7-full-1.4.2/src/core/s7_firmware.h
                            snap7-full-1.4.2/src/core/s7_isotcp.h
                            snap7-full-1.4.2/src/core/s7_isotcp.c
                            snap7-full-1.4.2/src/core/s7_peer.h
                            snap7-full-1.4.2/src/core/s7_peer.c
                            snap7-full-1.4.2/src/core/s7_text.h
                            snap7-full-1.4.2/src/core/s7_text.c
                            snap7-full-1.4.2/src/core/s7_types.h
                            main.c
                            )

target_link_libraries(Snap7ClientC "wsock32.lib")
target_link_libraries(Snap7ClientC "Winmm.lib")
