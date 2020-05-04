/*
 * Emscripten output contains this callback (onRuntimeInitialized)
 * which fires when the library is fully initialized.
 *
 * We're simply converting this into a promise.
 */
const waitUntilReady = src =>
  new Promise(resolve => (src.onRuntimeInitialized = resolve))

/**
 * Export a default function which instantiates the library
 *
 * @param {String} path Path to the build output from emscripten
 * @returns {Object} library
 */
export default async path => {
  const source = (await import(path)).default
  const library = source()
  await waitUntilReady(library)
  return {
    library
  }
}
