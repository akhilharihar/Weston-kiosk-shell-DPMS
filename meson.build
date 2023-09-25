project('kiosk-shell-dpms', 'cpp', 'c', version: '0.1')

libweston_version = 10
dir_prefix = get_option('prefix')
dir_lib = dir_prefix / get_option('libdir')
dir_module_weston = dir_lib / 'weston'
dir_system_include = '/usr/include'

weston = dependency('libweston-@0@'.format(lib_weston_version), method: 'pkg-config')
wayland = dependency('wayland-server', method: 'pkg-config')
pix = dependency('pixman-1', method : 'pkg-config')

shared_library(
  'kiosk-dpms',
  'src/kiosk-dpms.c'
  dependencies: [weston, wayland, pix],
  name_prefix: '',
  install: true,
  install_dir: dir_module_weston,
  include_directories: ['@0@/libweston-10'.format(dir_system_include), '@0@/pixman-1'.format(dir_system_include)]
)