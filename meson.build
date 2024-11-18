project(
  'barcalc',
  'c', 'cpp',
  version : '0.1.0',
  license: 'BSD-3-Clause',
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

cpp_args = []

if get_option('buildtype').startswith('debug')
  cpp_args += ['-Og', '-DDEBUG']
  cpp_args += '-DRESOURCES_DIR="' + meson.project_source_root() / 'resources' + '"'
endif
cpp_args += '-DPROG_NAME="' + meson.project_name() + '"'
cpp_args += '-DPROG_VERSION="' + meson.project_version() + '"'
cpp_args += '-DAPP_NAME="' + app_name + '"'
cpp_args += '-DAPP_ID="' + app_id + '"'
cpp_args += ['-Wconversion', '-Wsign-conversion', '-Wpedantic']
cpp_args += ['-Wshadow', '-Wnon-virtual-dtor']

src = []
subdir('src')
if get_option('buildtype') == 'release'
  subdir('resources')
endif

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
  cpp_args : cpp_args,
)
