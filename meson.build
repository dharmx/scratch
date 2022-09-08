project('scratch', 'c',
  version: '0.1.0',
  meson_version: '>= 0.59.0',
  default_options: [ 'warning_level=2', 'werror=false', 'c_std=gnu11' ]
)

gtkdep =  dependency('gtk4')
gnome = import('gnome')

resources = gnome.compile_resources('resources', 'src/scratch.gresource.xml')
sourcefiles = files('src/main.c', 'src/scratch-textview.c')

executable('scratch', sourcefiles, resources, dependencies: gtkdep)

# vim:filetype=meson
