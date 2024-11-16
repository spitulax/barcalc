resource_files = files(
  'style.css',
)

bundled_resources = custom_target(
  'bundled-resources',
  input: resource_files,
  output: 'resources.cpp',
  command: ['python3', '@CURRENT_SOURCE_DIR@/bundle.py', '@OUTPUT0@', '@INPUT@'],
)

src += bundled_resources
