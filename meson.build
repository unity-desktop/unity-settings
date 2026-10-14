project(
  'unity-settings',
  'c',
  version: '26',
  license: 'GPL-3.0-or-later',
  meson_version: '>= 1.0',
  default_options: [
    'c_std=gnu11',
    'warning_level=2',
  ],
)

dep_gtk             = dependency('gtk4', version: '>= 4.22')
dep_adw             = dependency('libadwaita-1', version: '>= 1.9')
dep_desktop_schemas = dependency('gsettings-desktop-schemas', version: '>= 47')

cc = meson.get_compiler('c')

add_project_arguments(
  cc.get_supported_arguments(['-Wno-unused-parameter']),
  '-DG_LOG_DOMAIN="unity-settings"',
  language: 'c',
)

gnome = import('gnome')

subdir('data')
subdir('src')

gnome.post_install(
  gtk_update_icon_cache: true,
  update_desktop_database: true,
)
