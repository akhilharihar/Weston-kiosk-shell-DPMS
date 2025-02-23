project('weston-dpms', 'c', version: '0.2', default_options: [
  'c_std=c17'
])

if get_option('buildtype') == 'debug'
  add_project_arguments('-DDEBUG', language: ['c', 'cpp'])
endif

libweston_version = 10

dir_prefix = get_option('prefix')

dir_bin = dir_prefix / get_option('bindir')
dir_lib = dir_prefix / get_option('libdir')

dir_executable_weston = dir_bin
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
  input: 'protocol/weston-shell-dpms.xml',
  output: 'weston-shell-dpms-server-protocol.h',
  command: [wayland_scanner_prog, 'server-header', '@INPUT@', '@OUTPUT@']
)

protocol_glue_code = custom_target(
  'protocol_glue_code',
  input: 'protocol/weston-shell-dpms.xml',
  output: 'weston-shell-dpms-protocol.c',
  command: [wayland_scanner_prog, 'private-code', '@INPUT@', '@OUTPUT@']
)

protocol_client_header = custom_target(
  'protocol_client_header',
  input: 'protocol/weston-shell-dpms.xml',
  output: 'weston-shell-dpms-client-protocol.h',
  command: [wayland_scanner_prog, 'client-header', '@INPUT@', '@OUTPUT@']
)

shared_library(
  'weston-shell-dpms',
  'src/weston-shell-dpms.c', protocol_glue_code, protocol_server_header,
  dependencies: [weston, wayland_server, pix],
  name_prefix: '',
  install: true,
  install_dir: dir_module_weston,
  include_directories: ['@0@/libweston-@1@'.format(dir_system_include, libweston_version), '@0@/pixman-1'.format(dir_system_include)]
)

executable(
  'weston-dpms',
  'src/weston-dpms.c', protocol_glue_code, protocol_client_header,
  dependencies: [wayland_client],
  name_prefix: '',
  install: true,
  install_dir: dir_executable_weston
)