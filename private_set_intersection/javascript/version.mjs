import { readFileSync, writeFileSync } from 'fs';
import { dirname, resolve } from 'path';
import { fileURLToPath } from 'url';

const __filename = fileURLToPath(import.meta.url);
const __dirname = dirname(__filename);

const readJson = (relPath) =>
  JSON.parse(readFileSync(resolve(__dirname, relPath), 'utf8'));

const npmPackage = readJson('../../package.json');
const npmPackageLock = readJson('../../package-lock.json');

const psiModule = await import('./bin/psi_wasm_node.js');
const PSI = psiModule.default || psiModule;

const psi = typeof PSI === 'function' ? await PSI() : PSI;
const version = psi.Package.version() || npmPackage.version;

console.log('Updating package version to:', version);

npmPackage.version = version;
npmPackageLock.version = version;

writeFileSync('package.json', JSON.stringify(npmPackage, null, 2) + '\n');
writeFileSync('package-lock.json', JSON.stringify(npmPackageLock, null, 2));
