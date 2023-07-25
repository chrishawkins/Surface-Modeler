load("//glob_defs.bzl", "subdir_glob")

cxx_library(
    name="SurfaceModelerCore",
    compiler_flags=["-std=c++20"],
    header_namespace="SurfaceModeler",
    exported_headers=subdir_glob([
        ('core', '**/*.h'),
    ]),
    srcs=glob([
        'core/**/*.cpp',
    ]),
    link_style="static",
    tests=[":test"]
)

cxx_binary(
    name="SurfaceModelerWindows",
    headers=glob([
        'windows/**/*.h',
    ]),
    srcs=glob([
        'windows/**/*.cpp',
    ]),
    linker_flags=[
        'kernel32.lib',
        'user32.lib',
        'gdi32.lib',
        'winspool.lib',
        'comdlg32.lib',
        'advapi32.lib',
        'shell32.lib',
        'ole32.lib',
        'oleaut32.lib',
        'uuid.lib',
        'odbc32.lib',
        'odbccp32.lib',
    ],
    link_style="static",
)

cxx_test(
    name='test',
    compiler_flags=["-std=c++20"],
    deps=[
        "//third-party:gtest",
        ":SurfaceModelerCore",
    ],
    srcs=[
        'tests/VectorTests.cpp',
    ],
)
