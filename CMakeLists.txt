cmake_minimum_required(VERSION 3.0)
project(freestandoom C)

set(doomgeneric_srcs_orig
    # i_main.c 
    dummy.c am_map.c doomdef.c doomstat.c dstrings.c d_event.c
    d_items.c d_iwad.c d_loop.c d_main.c d_mode.c d_net.c f_finale.c
    f_wipe.c g_game.c hu_lib.c hu_stuff.c info.c i_cdmus.c i_endoom.c
    i_joystick.c i_scale.c i_sound.c i_system.c i_timer.c memio.c
    m_argv.c m_bbox.c m_cheat.c m_config.c m_controls.c m_fixed.c
    m_menu.c m_misc.c m_random.c p_ceilng.c p_doors.c p_enemy.c
    p_floor.c p_inter.c p_lights.c p_map.c p_maputl.c p_mobj.c
    p_plats.c p_pspr.c p_saveg.c p_setup.c p_sight.c p_spec.c
    p_switch.c p_telept.c p_tick.c p_user.c r_bsp.c r_data.c
    r_draw.c r_main.c r_plane.c r_segs.c r_sky.c r_things.c
    sha1.c sounds.c statdump.c st_lib.c st_stuff.c s_sound.c tables.c
    v_video.c wi_stuff.c w_checksum.c w_file.c w_main.c w_wad.c z_zone.c
    w_file_stdc.c i_input.c i_video.c doomgeneric.c
    )

set(doomgeneric_srcs)
foreach(s ${doomgeneric_srcs_orig})
    list(APPEND doomgeneric_srcs doomgeneric/doomgeneric/${s})
endforeach()

add_executable(freestandoom
    dg.c
    module.c
    tlsf/tlsf.c
    target/heap.c
    target/dummy_system.c
    target/strdup.c
    target/ascii_ctype.c
    target/ascii_atoi.c
    target/ascii_string.c
    target/ascii_strings.c
    target/abs.c
    target/prf.c
    target/prf_stb.c
    target/dummy_sscanf.c
    ${doomgeneric_srcs})
