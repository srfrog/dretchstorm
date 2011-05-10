import os, string, sys
import SCons
import SCons.Errors


#
# set configuration options
#
opts = Variables('dstorm.conf')
opts.Add(EnumVariable('arch', 'Choose architecture to build for', 'linux-i386', allowed_values=('freebsd-i386', 'freebsd-amd64', 'linux-i386', 'linux-x86_64', 'netbsd-i386', 'opensolaris-i386', 'win32-mingw', 'darwin-ppc', 'darwin-i386')))
opts.Add(EnumVariable('warnings', 'Choose warnings level', '1', allowed_values=('0', '1', '2')))
opts.Add(EnumVariable('debug', 'Set to >= 1 to build for debug', '2', allowed_values=('0', '1', '2', '3')))
opts.Add(EnumVariable('optimize', 'Set to >= 1 to build with general optimizations', '2', allowed_values=('0', '1', '2', '3', '4', '5', '6')))
opts.Add(EnumVariable('simd', 'Choose special CPU register optimizations', 'none', allowed_values=('none', 'sse', '3dnow')))
#opts.Add(EnumVariable('cpu', 'Set to 1 to build with special CPU register optimizations', 'i386', allowed_values=('i386', 'athlon-xp', 'core2duo')))
opts.Add(BoolVariable('smp', 'Set to 1 to compile engine with symmetric multiprocessor support', 0))
opts.Add(BoolVariable('java', 'Set to 1 to compile the engine and game with Java support', 0))
opts.Add(BoolVariable('compatq3a', 'Set to 1 to compile engine with Q3A compatibility support', 0))
opts.Add(BoolVariable('experimental', 'Set to 1 to compile engine with experimental features support', 0))
#opts.Add(BoolVariable('purevm', 'Set to 1 to compile the engine with strict checking for vm/*.qvm modules in paks', 0))
opts.Add(BoolVariable('xmap', 'Set to 1 to compile the XMap(2) map compilers', 0))
#opts.Add(BoolVariable('vectorize', 'Set to 1 to compile the engine with auto-vectorization support', 0))
opts.Add(EnumVariable('curl', 'Choose http-download redirection support for the engine', 'compile', allowed_values=('none', 'compile', 'dlopen')))
opts.Add(BoolVariable('openal', 'Set to 1 to compile the engine with OpenAL support', 0))
opts.Add(BoolVariable('mumble', 'Set to 1 to compile the client with Mumble Positional Audio support', 0))
opts.Add(BoolVariable('noclient', 'Set to 1 to only compile the dedicated server', 0))
opts.Add(BoolVariable('master', 'Set to 1 to compile the master server', 0))
opts.Add(BoolVariable('package', 'Set to 1 to build package for this arch', 0))

#
# initialize compiler environment base
#
env = Environment(ENV = {'PATH' : os.environ['PATH']}, options = opts, tools = ['default'])

#
# set user-defined compiler if need be
#
if os.environ.has_key('CC'):
	env['CC'] = os.environ['CC']
if os.environ.has_key('CXX'):
	env['CXX'] = os.environ['CXX']

Help(opts.GenerateHelpText(env))

#
# set common compiler flags
#
print 'compiling for architecture ', env['arch']

if env['arch'] == 'win32-mingw':
	env['smp'] = 0
	env.Tool('crossmingw', toolpath = ['scons-tools'])
	env.PrependENVPath('PATH', os.environ['MINGW32BINPATH'])
	env.Append(CCFLAGS = '-DWIN32 -D_WIN32 -D_WINDOWS -DUSE_ICON')

# HACK: see http://www.physics.uq.edu.au/people/foster/amd64_porting.html
if env['arch'] == 'linux-x86_64':
	picLibBuilder = Builder(action = Action('$ARCOM'), emitter = '$LIBEMITTER', prefix = '$LIBPREFIX', suffix = '$LIBSUFFIX', src_suffix = '$OBJSUFFIX', src_builder = 'SharedObject')
	env['BUILDERS']['StaticLibrary'] = picLibBuilder
	env['BUILDERS']['Library'] = picLibBuilder


env.Append(CCFLAGS = '-pipe -fsigned-char')

if env['warnings'] == '1':
	env.Append(CCFLAGS = '-Wall -Wno-unused-parameter')
elif env['warnings'] == '2':
	env.Append(CCFLAGS = '-Wall -Werror')

if env['debug'] != '0':
	if env['arch'] == 'win32-mingw':
		env['optimize'] = 0
		env.Append(CCFLAGS = '-g -O0')
	else:
		env.Append(CCFLAGS = '-ggdb${debug} -D_DEBUG -DDEBUG')
else:
	env.Append(CCFLAGS = '-DNDEBUG')

if env['optimize'] != '0':
	if env['arch'] == 'win32-mingw':
		env.Append(CCFLAGS = '-O3 -march=i586 -fomit-frame-pointer -ffast-math -falign-loops=2 -falign-jumps=2 -falign-functions=2 -fstrength-reduce')
	else:
		env.Append(CCFLAGS = '-O${optimize} -ffast-math') # -fno-strict-aliasing -funroll-loops')

#if env['cpu'] == 'athlon-xp':
#	env.Append(CCFLAGS = '-march=athlon-xp') # -msse -mfpmath=sse')
#elif env['cpu'] == 'core2duo':
#	env.Append(CCFLAGS = '-march=prescott')

#if env['arch'] == 'linux-i386' and env['vectorize'] == 1:
#	env.Append(CCFLAGS = '-ftree-vectorize -ftree-vectorizer-verbose=1')

if env['arch'] == 'freebsd-i386':
        env.Append(CCFLAGS='-I/usr/local/include -I/usr/local/include/SDL -fPIC -L/usr/local/lib/gcc44 -Wl,-rpath /usr/local/lib/gcc44')
        env.Append(LIBPATH=['/usr/local/lib'])
        env.Append(LDFLAGS='-fPIC -L/usr/local/lib/gcc44 -Wl,-rpath /usr/local/lib/gcc44')

if env['arch'] == 'freebsd-amd64':
        env.Append(CCFLAGS='-I/usr/local/include -I/usr/local/include/SDL -fPIC -L/usr/local/lib/gcc44 -Wl,-rpath /usr/local/lib/gcc44')
        env.Append(LIBPATH=['/usr/local/lib'])
        env.Append(LDFLAGS='-fPIC -L/usr/local/lib/gcc44 -Wl,-rpath /usr/local/lib/gcc44')

if env['simd'] == 'sse':
	env.Append(CCFLAGS = '-DSIMD_SSE -msse')
elif env['simd'] == '3dnow':
	env.Append(CCFLAGS = '-DSIMD_3DNOW')

conf = Configure(env)
env = conf.Finish()

#
# save options
#
opts.Save('dstorm.conf', env)

#
# compile targets
#
Export('env')

if env['noclient'] == 0:
	SConscript('SConscript_dstorm', variant_dir='build/dstorm', duplicate=0)
	SConscript('SConscript_rendererGL', variant_dir='build/rendererGL', duplicate=0)
	SConscript('SConscript_base_game', variant_dir='build/base/game', duplicate=0)
	SConscript('SConscript_base_cgame', variant_dir='build/base/cgame', duplicate=0)
	SConscript('SConscript_base_ui', variant_dir='build/base/ui', duplicate=0)
	
SConscript('SConscript_dstormded', variant_dir='build/dstormded', duplicate=0)

if env['noclient'] == 1:
	SConscript('SConscript_base_game', variant_dir='build/base/game', duplicate=0)

if env['xmap'] == 1:
	SConscript('SConscript_dsxmap', variant_dir='build/dsxmap', duplicate=0)

if env['master'] == 1:
	SConscript('SConscript_xrealmaster', variant_dir='build/xmass', duplicate=0)

if env['package'] == 1:
	SConscript('SConscript_package', variant_dir='build/package', duplicate=0)

