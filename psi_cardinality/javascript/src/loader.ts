import bazel from 'bazel-psi-cardinality'

type NestedBazelLibrary = { readonly library: bazel.Library }
export type Loader = () => Promise<NestedBazelLibrary>

/*
 * Emscripten output contains this callback (onRuntimeInitialized)
 * which fires when the library is fully initialized.
 *
 * We're simply converting this into a promise.
 */
const waitUntilReady = (src: bazel.Library): Promise<void> =>
  new Promise(resolve => (src.onRuntimeInitialized = resolve))

/**
 * Export a default function which instantiates the library
 * @param {Object} bin Emscripten library to initialize
 */
export const createLoader = (
  bin: () => bazel.Library
): Loader => async (): Promise<NestedBazelLibrary> => {
  const library = bin()
  await waitUntilReady(library)
  return {
    library
  }
}
