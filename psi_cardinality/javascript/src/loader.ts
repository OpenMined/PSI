import * as psiCardinality from 'psi_cardinality'

type NestedLibrary = { readonly library: psiCardinality.Library }
export type Loader = () => Promise<NestedLibrary>

/*
 * Emscripten output contains this callback (onRuntimeInitialized)
 * which fires when the library is fully initialized.
 *
 * We're simply converting this into a promise.
 */
const waitUntilReady = (src: psiCardinality.Library): Promise<void> =>
  new Promise(resolve => (src.onRuntimeInitialized = resolve))

/**
 * Export a default function which instantiates the library
 * @param {Object} bin Emscripten library to initialize
 */
export const createLoader = (
  bin: () => psiCardinality.Library
): Loader => async (): Promise<NestedLibrary> => {
  const library = bin()
  await waitUntilReady(library)
  return {
    library
  }
}
