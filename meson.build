project('chip8', 'c',
  version : '0.1',
  default_options : ['warning_level=3'])


add_project_arguments('-DDEBUG', language: 'c')
sdl2_dep = dependency('sdl2')

executable('chip8', 'core.c', 'main.c', dependencies: sdl2_dep, install : true)
