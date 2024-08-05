add_rules("mode.debug", "mode.release")
set_languages("c17", "c++23")

add_requires("uthash 2023.7.11")
add_defines("NDEBUG", "_GNU_SOURCE=1")

target("game")
    set_kind("binary")
    add_packages("uthash")
    add_files("game/src/*.c")
    remove_files("game/src/test_*.c")


-- game test
for _, file in ipairs(os.files("game/src/test_*.c")) do
    local name = path.basename(file)
    local origin = string.gsub(name, "^test_", "")
    target(name)
        set_kind("binary")
        set_default(false)
        add_packages("uthash")
        add_files("game/src/" .. name .. ".c", "game/src/" .. origin .. ".c")
        
        add_tests("default")
        add_tests("args", {runargs = {"foo", "bar"}})
        add_tests("pass_output", {trim_output = true, runargs = "foo", pass_outputs = ""})
        add_tests("fail_output", {fail_outputs = {"hello2 .*", "hello xmake"}})
end

