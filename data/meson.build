desktop_plain = 'gnome'

desktops = [
  desktop_plain,
  'gnome-xorg'
]

shell_component = {
  desktop_plain: 'io.github.scarecrow_deShell',
}

required_components = {
  desktop_plain: [
    'io.github.scarecrow_deSettingsDaemon.A11ySettings',
    'io.github.scarecrow_deSettingsDaemon.Color',
    'io.github.scarecrow_deSettingsDaemon.Datetime',
    'io.github.scarecrow_deSettingsDaemon.Housekeeping',
    'io.github.scarecrow_deSettingsDaemon.Keyboard',
    'io.github.scarecrow_deSettingsDaemon.MediaKeys',
    'io.github.scarecrow_deSettingsDaemon.Power',
    'io.github.scarecrow_deSettingsDaemon.PrintNotifications',
    'io.github.scarecrow_deSettingsDaemon.Rfkill',
    'io.github.scarecrow_deSettingsDaemon.ScreensaverProxy',
    'io.github.scarecrow_deSettingsDaemon.Sharing',
    'io.github.scarecrow_deSettingsDaemon.Smartcard',
    'io.github.scarecrow_deSettingsDaemon.Sound',
    'io.github.scarecrow_deSettingsDaemon.UsbProtection',
    'io.github.scarecrow_deSettingsDaemon.Wacom',
    'io.github.scarecrow_deSettingsDaemon.XSettings',
  ],
}

if enable_session_selector
  desktops += 'gnome-custom-session'
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
  'gnome',
  'gnome-dummy'
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

schema = 'io.github.scarecrow_deSessionManager.gschema.xml'

configure_file(
  input: schema + '.in',
  output: schema,
  install: true,
  install_dir: join_paths(session_datadir, 'glib-2.0', 'schemas'),
  configuration: schema_conf
)

install_data(
  'gnome-session.convert',
  install_dir: join_paths(session_datadir, 'GConf', 'gsettings')
)

if enable_systemd_session
  unit_conf = configuration_data()
  unit_conf.set('libexecdir', session_libexecdir)

  systemd_service = ['gnome-session-manager@.service',
                     'gnome-session-signal-init.service',
                     'gnome-session-restart-dbus.service',
                     'gnome-session-monitor.service',
                     'gnome-session-failed.service']

  foreach service: systemd_service
    configure_file(
      input: service + '.in',
      output: service,
      install: true,
      install_dir: systemd_userunitdir,
      configuration: unit_conf
    )
  endforeach

  systemd_target = files('gnome-session-wayland@.target',
                         'gnome-session-wayland.target',
                         'gnome-session-x11@.target',
                         'gnome-session-x11.target',
                         'gnome-session-x11-services.target',
                         'gnome-session-x11-services-ready.target',
                         'gnome-session@.target',
                         'gnome-session.target',
                         'gnome-session-pre.target',
                         'gnome-session-manager.target',
                         'gnome-session-initialized.target',
                         'gnome-session-shutdown.target',
                         'gnome-session-failed.target',
                         )

  install_data(
    systemd_target,
    install_dir: systemd_userunitdir
  )

  install_data(
    'gnome-launched-override.scope.conf',
    rename: 'override.conf',
    install_dir : join_paths(systemd_userunitdir, 'gnome-launched-.scope.d')
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
      install_dir: systemd_userunitdir / 'gnome-session@@0@.target.d'.format(
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
