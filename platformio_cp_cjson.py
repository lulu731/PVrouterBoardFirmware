Import("env")

libs_dir = env["PROJECT_LIBDEPS_DIR"]
command = "cp -l --update=older " + libs_dir + "/" + env["PIOENV"] + "/cJSON/cJSON.* -t lib/json/"

env.Execute(command)