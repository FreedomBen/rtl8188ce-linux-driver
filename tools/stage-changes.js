#!/usr/bin/env node

const { spawn } = require('child_process');

const child = spawn('/usr/bin/git', ['add', '-p']);

child.on('exit', (code, signal) => {
  console.log(`Exited!  Code: ${code} - Signal: ${signal}`)
  process.exit(code)
})

child.stdout.on('data', (data) => {
  console.log(`child stdout:\n${data}`)
  child.stdin.write("n\n")
});

child.stderr.on('data', (data) => {
  console.error(`child stderr:\n${data}`)
});

console.log('Running')
