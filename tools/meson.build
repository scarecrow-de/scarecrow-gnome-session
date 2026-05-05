install_data(
  'scarecrow-session-custom-session',
  install_dir: session_bindir
)

deps = session_deps + [
  sm_dep,
  ice_dep
]

cflags = ['-DLOCALE_DIR="@0@"'.format(session_localedir)]

programs = [
  # name, deps, cflags, install_dir
  ['scarecrow-session-quit', deps, cflags, session_bindir],
  ['scarecrow-session-inhibit', session_deps, cflags, session_bindir]
]

if enable_systemd_session
  programs += [['scarecrow-session-ctl', session_bin_deps, cflags, session_libexecdir]]
endif

if enable_session_selector
  deps = [
    glib_dep,
    gtk_dep
  ]

  cflags += '-DGTKBUILDER_DIR="@0@"'.format(session_pkgdatadir)

  programs += [['scarecrow-session-selector', deps, cflags, session_bindir]]
endif

deps = [
  gtk_dep,
  x11_dep,
  dependency('egl'),
  dependency('glesv2')
]

cflags = '-DPKGDATADIR="@0@"'.format(session_pkgdatadir)

programs += [['scarecrow-session-check-accelerated-gles-helper', deps, cflags, session_libexecdir]]

deps = [
  glib_dep,
  x11_dep,
  dependency('gl'),
  dependency('epoxy'),
  dependency('xcomposite')
]

programs += [['scarecrow-session-check-accelerated-gl-helper', deps, cflags, session_libexecdir]]

deps += [gtk_dep]

cflags = '-DLIBEXECDIR="@0@"'.format(session_libexecdir)

programs += [['scarecrow-session-check-accelerated', deps, cflags, session_libexecdir]]

foreach program: programs
  executable(
    program[0],
    program[0] + '.c',
    include_directories: top_inc,
    dependencies: program[1],
    c_args: program[2],
    install: true,
    install_dir: program[3]
  )
endforeach
