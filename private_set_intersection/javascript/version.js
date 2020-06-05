/* eslint-disable @typescript-eslint/no-var-requires */
const PSI = require('./dist/combined/js/umd')
const fs = require('fs')
const package = require('../../package.json')

;(async () => {
  const psi = await PSI()
  package.version = psi.package.version
  console.log('Updating package version to:', package.version)
  const updatedPackage = JSON.stringify(package, null, 2)
  fs.writeFileSync('package.json', updatedPackage)
})()
