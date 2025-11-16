add_rules("mode.debug", "mode.release")
set_languages("c11", "c++17")

add_requires("uthash 2023.7.11", "lua ~5.4.0", "raylib ~5.5.0")
add_defines("NDEBUG", "_GNU_SOURCE=1")
if is_mode("debug") then
    add_cflags("-g", "-fsanitize=address")
    add_ldflags("-fsanitize=address")
end

target("game")
    set_kind("binary")
    add_packages("uthash")
    add_files("game/src/*.c")
    remove_files("game/src/test_*.c")
    if is_plat("macosx") then
        add_syslinks("pthread")
    end


-- game test
target("test_worker")
    set_kind("binary")
    set_default(false)
    add_packages("uthash")
    add_files("game/src/test_worker.c", "game/src/worker.c", "game/src/ring_queue.c", "game/src/logger.c")
    if is_plat("macosx") then
        add_syslinks("pthread")
    end
    
    add_tests("default")
    -- add_tests("args", {runargs = {"foo", "bar"}})
    -- add_tests("pass_output", {trim_output = true, runargs = "foo", pass_outputs = ""})
    -- add_tests("fail_output", {fail_outputs = {"hello2 .*", "hello xmake"}})


target("lua")
    set_kind("binary")
    add_files("lua/src/*.c")
    add_packages("lua")


target("ray")
    set_kind("binary")
    add_files("ray/src/*.c")
    add_packages("raylib")
