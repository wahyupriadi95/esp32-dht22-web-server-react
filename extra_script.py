Import("env")
import os

def before_build_littlefs(source, target, env):
    print("Building React App...")
    env.Execute("cd web && yarn build")
    print("React App built!")

    print("Removing old littlefs image...")
    env.Execute("rm -rf data")

    print("Copying React App to littlefs...")
    env.Execute("cp -r web/dist data")    

env.AddPreAction("$BUILD_DIR/littlefs.bin", before_build_littlefs)