project('jsonn', 'cpp',
    version : '0.2',
    default_options : [
        'cpp_std=c++20',
        'buildtype=release',
        'c_args=-O3 -flto -fno-rtti -fvisibility=hidden -march=native',
        'cpp_args=-O3 -flto -fexceptions -fvisibility=hidden -fno-rtti -march=native',
        'c_link_args=-flto',
        'cpp_link_args=-flto -ltbb'
    ]
)

jsonn_src = files(
    'src/jsonn_serialize.cpp',
    'src/jsonn_parser.cpp',
    'src/jsonn_serialize_jsonl.cpp',
    'src/jsonn_parser_jsonl.cpp'
)

jsonn_inc = include_directories('include')

jsonn_lib = shared_library(
    'jsonn',
    jsonn_src,
    include_directories : jsonn_inc,
    version : meson.project_version(),
    install : true,                        
    install_dir : get_option('libdir')     
)

install_headers(
    'include/jsonn.h',              
    subdir : 'jsonn'                       
)
