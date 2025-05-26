/* eslint-env node */
import { dirname } from 'path';
import { fileURLToPath } from 'url';

// Resolve paths relative to this file
const __filename = fileURLToPath(import.meta.url);
const __dirname = dirname(__filename);

// Load the generated WASM module wrapper
const wasmModule = await import('./bin/psi_benchmark_wasm.js');
const benchmark = typeof wasmModule.default === 'function'
  ? await wasmModule.default()  // Modularized build
  : wasmModule.default || wasmModule; // Already initialized

// Load the loader
const { createLoader } = await import('./tsc-out/main/loader.js');

// Run
await createLoader(benchmark);
