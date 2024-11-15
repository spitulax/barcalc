project(
  'barcalc',
  'c', 'cpp',
  version : '0.1.0',
  # license: 'MIT',
  default_options : [
    'c_std=c18',
    'cpp_std=c++23',
    'warning_level=3',
  ],
)

app_name = 'Barcalc'
app_id = 'dev.spitulax.' + app_name

cc = meson.get_compiler('c')
cxx = meson.get_compiler('cpp')

if get_option('buildtype').startswith('debug')
  add_project_arguments(['-Og', '-DDEBUG'], language: ['c', 'cpp'])
endif

add_project_arguments('-DPROG_NAME="' + meson.project_name() + '"', language: ['c', 'cpp'])
add_project_arguments('-DPROG_VERSION="' + meson.project_version() + '"', language: ['c', 'cpp'])
add_project_arguments('-DAPP_NAME="' + app_name + '"', language: ['c', 'cpp'])
add_project_arguments('-DAPP_ID="' + app_id + '"', language: ['c', 'cpp'])
add_project_arguments('-DRESOURCES_DIR="' + join_paths(meson.source_root(), 'resources') + '"', language: ['c', 'cpp'])
add_project_arguments(['-Wconversion', '-Wsign-conversion', '-Wpedantic'], language: ['c', 'cpp'])
add_project_arguments(['-Wshadow', '-Wnon-virtual-dtor'], language: 'cpp')

subdir('src')

include = [
  'src',
]

deps = [
  dependency('gtkmm-4.0', include_type : 'system')
]

executable(
  meson.project_name(),
  src,
  dependencies : deps,
  include_directories : include,
  install : true,
)
