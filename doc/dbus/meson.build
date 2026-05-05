ifaces_refs = []

ifaces = [
  'io.github.scarecrow_de.SessionManager',
  'io.github.scarecrow_de.SessionManager.Client',
  'io.github.scarecrow_de.SessionManager.ClientPrivate',
  'io.github.scarecrow_de.SessionManager.Inhibitor',
  'io.github.scarecrow_de.SessionManager.Presence'
]

gnome_session_dir = join_paths(meson.source_root(), 'scarecrow-session')
spec_to_docbook = files('spec-to-docbook.xsl')

foreach iface: ifaces
  iface_ref = iface + '.ref.xml'

  ifaces_refs += custom_target(
    iface_ref,
    input: files(join_paths(gnome_session_dir, iface + '.xml')),
    output: iface_ref,
    command: [xsltproc, '--output', '@OUTPUT@', spec_to_docbook, '@INPUT@']
  )
endforeach

session_conf = configuration_data()
session_conf.set('VERSION', session_version)

#session = meson.project_name()

session = 'scarecrow'

xml_in = configure_file(
  input: session + '.xml.in',
  output: session + '.xml',
  configuration: session_conf
)

config_xsl = files('config.xsl')

custom_target(
  session,
  input: xml_in,
  output: session + '.html',
  command: [find_program('xmlto'), 'xhtml-nochunks', '-o', meson.current_build_dir(), '-m', config_xsl, '@INPUT@'],
  depends: ifaces_refs,
  install: true,
  install_dir: join_paths(session_datadir, 'doc', meson.project_name(), 'dbus')
)
