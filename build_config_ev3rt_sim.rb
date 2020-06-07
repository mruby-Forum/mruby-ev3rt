# Usage: MRUBY_CONFIG=build_config_ev3rt.rb rake
#

#
# User Configuration
#

EV3RT_PATH = "/Users/toi/Documents/Hakoniwa/athrill-sample.work/ev3rt/ev3rt-beta7-release/asp3"


#GNU_TOOL_PREFX = "/usr/local/gcc-arm-none-eabi-4_9-2014q4/bin/arm-none-eabi-"
GNU_TOOL_PREFX = "/usr/local/bin/v850-elf-"

##GNU_TOOL_PREFX = "arm-none-eabi-"


#
# mruby Build Tasks
#
MRuby::Build.new do |conf|

  # Gets set by the VS command prompts.
  if ENV['VisualStudioVersion'] || ENV['VSINSTALLDIR']
    toolchain :visualcpp
  else
    toolchain :gcc
  end

  enable_debug

  # include the default GEMs
  conf.gembox 'default'

end

# Cross Compiling configuration for EV3RT
# http://dev.toppers.jp/trac_user/ev3pf/wiki/WhatsEV3RT (in Japanese)
#
MRuby::CrossBuild.new("EV3RT") do |conf|

  toolchain :gcc

  EV3_RT_INCLUDES = %w(include
                       arch
                       target/v850_gcc
                       target/v850_gcc/api
                       target/v850_gcc/api/include
                       target/v850_gcc/dmloader/app/include
                       target/v850_gcc/dmloader/include
                       target/v850_gcc/drivers/bluetooth/btstack/include
                       target/v850_gcc/drivers/soc/AM1808_StarterWare_1_00_03_03/include/hw
                       target/v850_gcc/drivers/bluetooth/include
                       target/v850_gcc/drivers/brick/include
                       target/v850_gcc/drivers/common/include
                       target/v850_gcc/drivers/common/virtual-linux-kernel/arch/arm/include
                       target/v850_gcc/drivers/common/virtual-linux-kernel/include
                       target/v850_gcc/drivers/console/include
                       target/v850_gcc/drivers/fatfs/include
                       target/v850_gcc/drivers/gpio/include
                       target/v850_gcc/drivers/lcd/include
                       target/v850_gcc/drivers/lcd/linux/include
                       target/v850_gcc/drivers/linux/arch/arm/include
                       target/v850_gcc/drivers/linux/arch/arm/mach-davinci/include
                       target/v850_gcc/drivers/linux/include
                       target/v850_gcc/drivers/motor/include
                       target/v850_gcc/drivers/newlib/include
                       target/v850_gcc/drivers/sensor/analog/include
                       target/v850_gcc/drivers/sensor/uart/d_uart/Linuxmod_AM1808/pru-firmware-05-31-2011-1423-v3.0/pru/hal/common/include
                       target/v850_gcc/drivers/sensor/uart/d_uart/Linuxmod_AM1808/pru-firmware-05-31-2011-1423-v3.0/pru/hal/uart/include
                       target/v850_gcc/drivers/sensor/uart/include
                       target/v850_gcc/drivers/soc/AM1808_StarterWare_1_00_03_03/include
                       target/v850_gcc/drivers/soc/include
                       target/v850_gcc/drivers/sound/include
                       target/v850_gcc/kernel_mod/include
                       target/v850_gcc/pil/include
                       target/v850_gcc/platform/include
                       target/v850_gcc/TLSF-2.4.6/include
                       sdk/common/ev3api/include
					   syssvc
                       arch/arm_gcc/am1808
                       arch/arm_gcc/common
                       arch/gcc
			. )


  conf.cc do |cc|
    cc.command = "#{GNU_TOOL_PREFX}gcc"

    cc.include_paths << EV3_RT_INCLUDES.map{|inc| File.join(EV3RT_PATH, inc)}

    cc.flags = %w(-std=gnu99 -Dgcc -mv850e2v3 -nostdlib
                  -Wall -DBUILD_EV3_PLATFORM -DCONFIG_FB_DEFERRED_IO
                  -D__KERNEL__ -D__TARGET_ARCH_ARM=5 -DTOPPERS_CFG1_OUT -DTOPPERS_EV3)


    cc.flags << %w(-O3)
    ##cc.flags << %w(-Os)
#    cc.flags << %w(-g3)
    ##cc.flags << %w(DUSE_CFG_PASS3)

    cc.compile_options = "%{flags} -o %{outfile} -c %{infile}"

    cc.defines << %w(MRB_DISABLE_STDIO) #if you dont need stdio.
    #cc.defines << %w(POOL_PAGE_SIZE=1000) #effective only for use with mruby-eval
  end

  conf.cxx do |cxx|
    cc = conf.cc
    cxx.command = cc.command.dup
    cxx.include_paths = cc.include_paths.dup
    cxx.flags = cc.flags.dup
    cxx.flags << %w(-fno-rtti -fno-exceptions)
    cxx.defines = cc.defines.dup
    cxx.compile_options = cc.compile_options.dup
  end

  conf.archiver do |archiver|
    archiver.command = "#{GNU_TOOL_PREFX}ar"
#    archiver.archive_options = 'rcs %{outfile} %{objs}'
  end

  #no executables
  conf.bins = []

  #do not build executable test
  conf.build_mrbtest_lib_only

  #disable C++ exception
  conf.disable_cxx_exception

  #gems from core
  #conf.gem :core => "mruby-sprintf"
  #conf.gem :core => "mruby-print"
  #  conf.gem :core => "mruby-math"
  #conf.gem :core => "mruby-time"
  #conf.gem :core => "mruby-struct"
  #conf.gem :core => "mruby-enum-ext"
  conf.gem :core => "mruby-string-ext"
  #conf.gem :core => "mruby-numeric-ext"
  #conf.gem :core => "mruby-array-ext"
  #conf.gem :core => "mruby-hash-ext"
  #conf.gem :core => "mruby-range-ext"
  #conf.gem :core => "mruby-proc-ext"
  #conf.gem :core => "mruby-symbol-ext"
  #conf.gem :core => "mruby-random"
  #conf.gem :core => "mruby-object-ext"
  conf.gem :core => "mruby-compiler"
  #conf.gem "mrbgems/mruby-ev3rt"
#  conf.gembox 'default'
  conf.gem "../mruby-ev3rt"
#conf.gem :github => "yamanekko/mruby-ev3rt-io"
#  conf.gem :github => "yamanekko/mruby-ev3rt"

  #light-weight regular expression
  #conf.gem :github => "masamitsu-murase/mruby-hs-regexp", :branch => "master"
end
