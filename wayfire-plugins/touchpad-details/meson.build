shared_module(
  'touchpad-details',
  'plugin.cpp',
  name_prefix: 'lib',
  dependencies: [dep_wayfire, dep_wfconfig, dep_wlroots, dependency('libinput')],
  install: true,
  install_dir: plugindir,
)

install_data('touchpad-details.xml', install_dir: metadatadir)
