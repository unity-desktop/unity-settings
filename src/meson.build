unity_settings_deps = [
  dep_gtk,
  dep_adw,
  dep_desktop_schemas,
]

unity_settings_sources = files('main.c')

unity_settings_includes = include_directories('.', 'window')

subdir('window')

unity_settings_sources += gnome.compile_resources(
  'unity-settings-resources',
  'unity-settings.gresource.xml',
  source_dir: ['.', meson.project_source_root() / 'data'],
  c_name: 'unity_settings',
)

executable(
  'unity-settings',
  unity_settings_sources,
  dependencies: unity_settings_deps,
  include_directories: unity_settings_includes,
  install: true,
)
