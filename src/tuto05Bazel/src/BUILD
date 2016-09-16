cc_binary(
    name = "libNativeVTK.so",
    srcs = ["jni/main.cxx"],
    copts = ["-Iexternal/deps/include","-Iexternal/deps/include/vtk-6.3/","--verbose"],
    linkopts = [
        "-landroid",
        "-llog",
        ],
    deps = [
        "@deps//:vtk",
        "@deps//:native_app_glue", 
        "@deps//:gnustl"
        ],
    linkshared = 1,
)

cc_library(
    name = "maintest",
    srcs = ["libNativeVTK.so"],
)


android_binary(
    name = "NativeVTK_Bazel",
    custom_package = "com.jdev2015.NativeVTK",
    srcs = glob(["src/com/jdev2015/NativeVTK/*.java"]),
    manifest = "AndroidManifest.xml",
    legacy_native_support = 1,
    resource_files = glob(["res/**"]),
    deps = ["maintest",
        "@deps//:vtk",
        "@deps//:native_app_glue", 
        "@deps//:gnustl",
        "@deps//:xml",
        "@deps//:jpeg", 
        "@deps//:expat",
        "@deps//:freetype",
        "@deps//:png", 
        "@deps//:tiff",
        "@deps//:iconv",
        "@deps//:charset",
        ],
)