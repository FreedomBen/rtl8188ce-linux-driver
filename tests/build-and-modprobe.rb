#!/usr/bin/ruby

# %x{} returns a string of the output
# system() returns a true/false/nil

require 'colorize'

modstotest = %w{ rtl8188ee rtl8192ce rtl8192cu rtl8192de rtl8192ee rtl8192se rtl8723ae rtl8723be rtl8723com rtl8821ae }

def die(message)
    puts "[ERROR]: ".light_red + message.red
    abort("")
end

kernel_ver = %x{uname -r}.chomp
kernel_ver.empty? && die("Could not get current kernel version")

$results = Hash.new {|h,k| h[k] = Array.new }

def failure(mod, message)
    # $results[mod] ||= []
    $results[mod].push(message)
end

unless %x{id -u}.chomp == "0"
    die "Must be root to run this test"
end

unless File.exist?('functions.sh')
    die "This script must be run from the root directory"
end

puts "You should not run this on a production machine! It will delete backups, build, install, and modprobe all the drivers for the current kernel version".yellow
puts "Press enter to run the tests".yellow
gets


# Delete any backups by deleting the whole backup directory
BACKUP_DIR = "#{Dir.home}/.rtlwifi-backup"
BACKUP_FILE = "#{BACKUP_DIR}/#{%x{uname -r}.chomp}.tar.gz"
system("rm -rf #{BACKUP_DIR}")

system("yes 'y' | make") || die("Error building")
system("yes 'y' | make install") || die("Error installing ")

# Make sure the backups now exist
unless Dir.exist? BACKUP_DIR
  failure("backup", "The backup directory was not recreated by the build!")
end

unless File.exist? BACKUP_FILE
  failure("backup", "The backup file was not created during the build")
end

modstotest.each do |mod|
    # First make sure it built and installed correctly
    # if Dir.glob("/lib/modules/#{kernel_ver}/kernel/drivers/net/wireless/rtlwifi/#{mod}/#{mod}.ko*").empty?
    expected_file = "/lib/modules/#{kernel_ver}/kernel/drivers/net/wireless/rtlwifi/#{mod}/#{mod}.ko"
    unless File.exists?(expected_file)
        failure(mod, "#{mod}.ko was not found.  Did it get built and then installed?")
        next # same as continue in other languages
    end

    # Now modprobe and make sure it worked
    # First unload all existing modules to make sure we are loading in ours
    if system("lsmod | grep '^rtl' >/dev/null")
        command = "lsmod | grep '^rtl' | awk '{print $1}' | xargs modprobe -r "
    else
        command = "true"
    end

    unless system(command)
        failure(mod, "Could not remove existing kernel modules so test could not be run")
        next
    end

    modprobe_res = %x{modprobe #{mod}}.chomp
    unless $? == 0
        failure(mod, "Unable to modprobe in the built modules: #{modprobe_res}")
        next
    end

    # Double check that they are there
    unless system("lsmod | grep '^rtl' | head -1 | awk '{print $1}'")
        failure(mod, "Modprobe appeared to succeed but module did not show up in lsmod")
        next
    end

    # Look for "Benjamin Porter" in the output of modinfo to make sure we can detect the new driver
    unless system("modinfo #{mod} | grep 'Benjamin Porter' >/dev/null")
        failure(mod, "New module will not be detected because Benamin Porter did not show up in modinfo")
        next
    end

    # Now remove the module
    system("modprobe -r #{mod}")
end

# Now print out results
puts "Results:".yellow
modstotest.insert(0, "backup")
modstotest.each do |mod|
    result = $results[mod]
    puts "#{mod} - Status:  " + (result.empty? ? "Pass".green : "Fail".red)

    result.each do |message|
        puts "    #{message}".yellow
    end
    puts ""
end


