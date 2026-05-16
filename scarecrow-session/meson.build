script_conf = configuration_data()
script_conf.set('libexecdir', session_libexecdir)

script = 'scarecrow-session'

configure_file(
  input: script + '.in',
  output: script,
  install: true,
  install_dir: session_bindir,
  configuration: script_conf
)

libscsmutil = static_library(
  'scsmutil',
  sources: 'scsm-util.c',
  include_directories: top_inc,
  dependencies: session_deps
)

sources = files(
  'scsm-app.c',
  'scsm-autostart-app.c',
  'scsm-client.c',
  'scsm-dbus-client.c',
  'scsm-fail-whale.c',
  'scsm-inhibitor.c',
  'scdm-log.c',
  'scsm-manager.c',
  'scsm-presence.c',
  'scsm-process-helper.c',
  'scsm-session-fill.c',
  'scsm-session-save.c',
  'scsm-shell-extensions.c',
  'scsm-shell.c',
  'scsm-store.c',
  'scsm-system.c',
  'scsm-systemd.c',
  'scsm-xsmp-client.c',
  'scsm-xsmp-server.c',
  'main.c'
)

dbus_ifaces = [
  ['io.github.scarecrow_de.SessionManager', 'ExportedManager'],
  ['io.github.scarecrow_de.SessionManager.Client', 'ExportedClient'],
  ['io.github.scarecrow_de.SessionManager.ClientPrivate', 'ExportedClientPrivate'],
  ['io.github.scarecrow_de.SessionManager.App', 'ExportedApp'],
  ['io.github.scarecrow_de.SessionManager.Inhibitor', 'ExportedInhibitor'],
  ['io.github.scarecrow_de.SessionManager.Presence', 'ExportedPresence']
]

foreach iface: dbus_ifaces
  sources += gnome.gdbus_codegen(
    iface[0],
    iface[0] + '.xml',
    interface_prefix: iface[0] + '.',
    namespace: 'Gsm',
    annotations: [iface[0], 'org.gtk.GDBus.C.Name', iface[1]]
  )
endforeach

if enable_consolekit
  sources += files('scsm-consolekit.c')
endif

cflags = [
  '-DLOCALE_DIR="@0@"'.format(session_localedir),
  '-DDATA_DIR="@0@"'.format(session_pkgdatadir),
  '-DLIBEXECDIR="@0@"'.format(session_libexecdir)
]

executable(
  meson.project_name() + '-binary',
  sources,
  include_directories: top_inc,
  dependencies: session_bin_deps,
  c_args: cflags,
  link_with: libscsmutil,
  install: true,
  install_dir: session_libexecdir
)

sources = files('scsm-fail-whale-dialog.c')

cflags = '-DLOCALE_DIR="@0@"'.format(session_localedir)

executable(
  meson.project_name() + '-failed',
  sources,
  include_directories: top_inc,
  dependencies: gtk_dep,
  c_args: cflags,
  install: true,
  install_dir: session_libexecdir
)

units = [
  ['test-inhibit', [], session_deps + [gtk_dep]],
  ['test-client-dbus', [], [gio_dep]],
  ['test-process-helper', files('scsm-process-helper.c'), [gio_dep]]
]

foreach unit: units
  executable(
    unit[0],
    [unit[0] + '.c'] + unit[1],
    include_directories: top_inc,
    dependencies: unit[2]
  )
endforeach
