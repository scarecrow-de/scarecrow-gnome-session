desktop_plain = 'scarecrow'

desktops = [
  desktop_plain,
  'scarecrow-xorg'
]

shell_component = {
  desktop_plain: 'io.github.scarecrow_de.Shell',
}

required_components = {
  desktop_plain: [
    'io.github.scarecrow_de.SettingsDaemon.A11ySettings',
    'io.github.scarecrow_de.SettingsDaemon.Color',
    'io.github.scarecrow_de.SettingsDaemon.Datetime',
    'io.github.scarecrow_de.SettingsDaemon.Housekeeping',
    'io.github.scarecrow_de.SettingsDaemon.Keyboard',
    'io.github.scarecrow_de.SettingsDaemon.MediaKeys',
    'io.github.scarecrow_de.SettingsDaemon.Power',
    'io.github.scarecrow_de.SettingsDaemon.PrintNotifications',
    'io.github.scarecrow_de.SettingsDaemon.Rfkill',
    'io.github.scarecrow_de.SettingsDaemon.ScreensaverProxy',
    'io.github.scarecrow_de.SettingsDaemon.Sharing',
    'io.github.scarecrow_de.SettingsDaemon.Smartcard',
    'io.github.scarecrow_de.SettingsDaemon.Sound',
    'io.github.scarecrow_de.SettingsDaemon.UsbProtection',
    'io.github.scarecrow_de.SettingsDaemon.Wacom',
    'io.github.scarecrow_de.SettingsDaemon.XSettings',
  ],
}

if enable_session_selector
  desktops += 'scarecrow-custom-session'
endif

foreach name: desktops
  desktop_conf = configuration_data()
  desktop_conf.set('bindir', session_bindir)

  desktop = name + '.desktop'

  desktop_in = configure_file(
    input: desktop + '.in.in',
    output: desktop + '.in',
    configuration: desktop_conf
  )

  install_dir = join_paths(session_datadir, 'xsessions')
  # FIXME: The same target can not be copied into two directories.
  #        There is a workaround in meson_post_install.py until proper solution arises:
  #        https://groups.google.com/forum/#!topic/mesonbuild/3iIoYPrN4P0
  if name == desktop_plain
    #install_dir: [
    #  join_paths(session_datadir, 'xsessions'),
    #  join_paths(session_datadir, 'wayland-sessions')
    #]
  endif

  desktop_target = i18n.merge_file(
    type: 'desktop',
    input: desktop_in,
    output: desktop,
    po_dir: po_dir,
    install: true,
    install_dir: install_dir
  )
endforeach

sessions = [
  'scarecrow',
  'scarecrow-dummy'
]

foreach session: sessions
  session_file = session + '.session'
  desktop_conf = configuration_data()
  desktop_conf.set('libexecdir', session_libexecdir)
  desktop_conf.set('required_components', ';'.join(
    [shell_component.get(session, '')] + required_components.get(session, [])))

  desktop = session_file + '.desktop'

  desktop_in = configure_file(
    input: desktop + '.in.in',
    output: desktop + '.in',
    configuration: desktop_conf
  )

  i18n.merge_file(
    type: 'desktop',
    input: desktop_in,
    output: session_file,
    po_dir: po_dir,
    install: true,
    install_dir: join_paths(session_pkgdatadir, 'sessions')
  )
endforeach

schema_conf = configuration_data()

schema = 'io.github.scarecrow_de.SessionManager.gschema.xml'

configure_file(
  input: schema + '.in',
  output: schema,
  install: true,
  install_dir: join_paths(session_datadir, 'glib-2.0', 'schemas'),
  configuration: schema_conf
)

install_data(
  'scarecrow-session.convert',
  install_dir: join_paths(session_datadir, 'GConf', 'gsettings')
)

if enable_systemd_session
  unit_conf = configuration_data()
  unit_conf.set('libexecdir', session_libexecdir)

  systemd_service = ['scarecrow-session-manager@.service',
                     'scarecrow-session-signal-init.service',
                     'scarecrow-session-restart-dbus.service',
                     'scarecrow-session-monitor.service',
                     'scarecrow-session-failed.service']

  foreach service: systemd_service
    configure_file(
      input: service + '.in',
      output: service,
      install: true,
      install_dir: systemd_userunitdir,
      configuration: unit_conf
    )
  endforeach

  systemd_target = files('scarecrow-session-wayland@.target',
                         'scarecrow-session-wayland.target',
                         'scarecrow-session-x11@.target',
                         'scarecrow-session-x11.target',
                         'scarecrow-session-x11-services.target',
                         'scarecrow-session-x11-services-ready.target',
                         'scarecrow-session@.target',
                         'scarecrow-session.target',
                         'scarecrow-session-pre.target',
                         'scarecrow-session-manager.target',
                         'scarecrow-session-initialized.target',
                         'scarecrow-session-shutdown.target',
                         'scarecrow-session-failed.target',
                         )

  install_data(
    systemd_target,
    install_dir: systemd_userunitdir
  )

  install_data(
    'scarecrow-launched-override.scope.conf',
    rename: 'override.conf',
    install_dir : join_paths(systemd_userunitdir, 'scarecrow-launched-.scope.d')
  )

  foreach session, req_components: required_components
    wanted_targets = []
    foreach component: req_components
      wanted_targets += 'Wants=@0@.target'.format(component)
    endforeach

    configure_file(
      input: session + '.session.conf.in',
      output: session + '.session.conf',
      configuration: {
        'requires_component': shell_component[session],
        'wants_required_components': '\n'.join(wanted_targets),
      },
      install_dir: systemd_userunitdir / 'scarecrow-session@@0@.target.d'.format(
        session),
    )
  endforeach
endif

data = files('hardware-compatibility')

if enable_session_selector
  data += files('session-selector.ui')
endif

install_data(
  data,
  install_dir: session_pkgdatadir
)
