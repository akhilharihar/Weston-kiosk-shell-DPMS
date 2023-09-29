project('kiosk-shell-dpms', 'c', version: '0.1', default_options: [
  'c_std=c17'
])

libweston_version = 10
dir_prefix = get_option('prefix')
dir_lib = dir_prefix / get_option('libdir')
dir_module_weston = dir_lib / 'weston'
dir_system_include = '/usr/include'

weston = dependency('libweston-@0@'.format(libweston_version), method: 'pkg-config')
wayland_server = dependency('wayland-server', method: 'pkg-config')
wayland_client = dependency('wayland-client', method: 'pkg-config')
pix = dependency('pixman-1', method : 'pkg-config')
wayland_scanner = dependency('wayland-scanner', native: true)
wayland_scanner_prog = find_program(wayland_scanner.get_variable('wayland_scanner'))

protocol_server_header = custom_target(
  'protocol_server_header',
  input: 'protocol/dpms.xml',
  output: 'kiosk-shell-dpms-interface.h',
  command: [wayland_scanner_prog, 'server-header', '@INPUT@', '@OUTPUT@']
)

protocol_code = custom_target(
  'protocol_code',
  input: 'protocol/dpms.xml',
  output: 'kiosk-shell-dpms-interface.c',
  command: [wayland_scanner_prog, 'private-code', '@INPUT@', '@OUTPUT@']
)

protocol_client_header = custom_target(
  'protocol_client_header',
  input: 'protocol/dpms.xml',
  output: 'kiosk-shell-dpms-client.h',
  command: [wayland_scanner_prog, 'client-header', '@INPUT@', '@OUTPUT@']
)

if get_option('buildtype') == 'debug'
  add_project_arguments('-DDEBUG', language: ['c', 'cpp'])
endif

shared_library(
  'kiosk-dpms',
  'src/kiosk-dpms.c', protocol_code, protocol_server_header,
  dependencies: [weston, wayland_server, pix],
  name_prefix: '',
  install: true,
  install_dir: dir_module_weston,
  include_directories: ['@0@/libweston-10'.format(dir_system_include), '@0@/pixman-1'.format(dir_system_include)]
)

executable(
  'testing',
  'src/test.c', protocol_code, protocol_client_header,
  dependencies: [wayland_client],
  name_prefix: '',
  install: false
)