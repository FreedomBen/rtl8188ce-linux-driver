#!/usr/bin/ruby

# ARGF is a cool way to handle either stdin or file input
# http://stackoverflow.com/a/273841/2062384

def is_breaking_char(char)
  ['-', '.'].include? char
end

args = []

ARGF.read.split.each do |i|
  args.push i
end

args.sort! do |a, b|
  return 0 if a == b

  # do a numeric compare on the first part of the string that is different
  # keep going on the number until reaching either a '-' or a '.'
  first_i = 0
  break_val = a.length > b.length ? a.length : b.length

  while first_i < break_val do
    break if a[first_i] != b[first_i]
    first_i += 1
  end

  # d'oh, something isn't right
  raise "Both strings look equivalent but didn't return for a == b:  a == #{a};  b == #{b}" if first_i == break_val

  new_a = a[first_i..-1].gsub(/[-.].*/, '')
  new_b = b[first_i..-1].gsub(/[-.].*/, '')

  new_a.to_i <=> new_b.to_i
end

puts args.last
