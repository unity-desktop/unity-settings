project(
  'unity-settings',
  ['c', 'cpp'],
  version: '26',
  license: 'GPL-3.0-or-later',
  meson_version: '>= 1.0',
  default_options: [
    'c_std=gnu11',
    'cpp_std=c++17',
    'warning_level=2',
  ],
)

dep_gtk             = dependency('gtk4', version: '>= 4.22')
dep_adw             = dependency('libadwaita-1', version: '>= 1.9')
dep_desktop_schemas = dependency('gsettings-desktop-schemas', version: '>= 47')
dep_wayfire         = dependency('wayfire')
dep_wfconfig        = dependency('wf-config')
dep_wlroots         = dependency('wlroots-0.20')

cc  = meson.get_compiler('c')
cpp = meson.get_compiler('cpp')

add_project_arguments(
  cc.get_supported_arguments(['-Wno-unused-parameter']),
  '-DG_LOG_DOMAIN="unity-settings"',
  language: 'c',
)

add_project_arguments(
  '-I' + dep_wayfire.get_variable(pkgconfig: 'includedir') / 'wayfire',
  language: 'cpp',
)

gnome           = import('gnome')

plugindir   = get_option('libdir') / 'wayfire'
metadatadir = get_option('datadir') / 'wayfire' / 'metadata'

subdir('data')
subdir('src')
subdir('wayfire-plugins' / 'night-light')
subdir('wayfire-plugins' / 'touchpad-details')

gnome.post_install(
  gtk_update_icon_cache: true,
  update_desktop_database: true,
)
