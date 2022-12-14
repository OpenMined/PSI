/**
 * This file is strictly used to update the version value inside package.json
 *
 * It calls the direct output from the emscripten build and therefore
 * does not use the wrappers in TS. This is not ideal, however
 * it is designed to update the version at build time in order to trigger
 * the need for a commit before bundling.
 */
import PSI from './bin/psi_wasm_node.js'
import { writeFileSync } from 'fs'
import npmPackage from '../../package.json'
import npmPackageLock from '../../package-lock.json'
;(async () => {
  const psi = await PSI()
  // Ensure we don't null this field.
  const version = psi.Package.version() || npmPackage.version
  // eslint-disable-next-line no-undef
  console.log('Updating package version to:', version)
  npmPackage.version = version
  npmPackageLock.version = version
  const updatedPackage = JSON.stringify(npmPackage, null, 2) + '\n'
  const updatedPackageLock = JSON.stringify(npmPackageLock, null, 2)
  writeFileSync('package.json', updatedPackage)
  writeFileSync('package-lock.json', updatedPackageLock)
})()
