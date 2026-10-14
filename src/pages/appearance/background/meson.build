cc_background_deps = [
  dep_gtk,
  dep_adw,
  dep_desktop_schemas,
  dependency('gnome-bg-4'),
  dependency('gnome-desktop-4'),
  dependency('libxml-2.0'),
]

cc_background_sources = files(
  'bg-recent-source.c',
  'bg-source.c',
  'bg-wallpapers-source.c',
  'cc-background-chooser.c',
  'cc-background-item.c',
  'cc-background-paintable.c',
  'cc-background-xml.c',
)

cc_background_sources += gnome.mkenums_simple(
  'cc-background-enum-types',
  sources: files('cc-background-item.h', 'cc-background-paintable.h'),
)

cc_background_sources += gnome.mkenums_simple(
  'gdesktop-enums-types',
  sources: dep_desktop_schemas.get_variable(pkgconfig: 'prefix') / 'include' / 'gsettings-desktop-schemas' / 'gdesktop-enums.h',
  identifier_prefix: 'GDesktop',
  symbol_prefix: 'g_desktop',
)

cc_background_sources += configure_file(
  output: 'config.h',
  configuration: {'GETTEXT_PACKAGE': '"unity-settings"'},
)

cc_background_lib = static_library(
  'cc-background',
  cc_background_sources,
  dependencies: cc_background_deps,
  c_args: ['-DGNOME_DESKTOP_USE_UNSTABLE_API', '-Wno-deprecated-declarations'],
  override_options: ['warning_level=1'],
)

unity_settings_deps += declare_dependency(
  link_with: cc_background_lib,
  include_directories: include_directories('.'),
  dependencies: cc_background_deps,
  compile_args: ['-DGNOME_DESKTOP_USE_UNSTABLE_API'],
)
