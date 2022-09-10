project('scratch', 'c',
  version: '0.1.0',
  meson_version: '>= 0.59.0',
  default_options: [ 'warning_level=3', 'werror=false', 'c_std=gnu11' ]
)

gnome = import('gnome')

gtkdep = dependency('gtk4')
x11dep = dependency('x11')

resources = gnome.compile_resources('resources', 'src/scratch.gresource.xml')
sourcefiles = files('src/main.c', 'src/scratch-textview.c', 'src/scratch-wm.c')

executable('scratch', sourcefiles, resources, dependencies: [ x11dep, gtkdep ])

# vim:filetype=meson
