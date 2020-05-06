import { CppLibrary } from './types'

export type LoaderFn = () => Promise<{ readonly library: CppLibrary }>

/*
 * Emscripten output contains this callback (onRuntimeInitialized)
 * which fires when the library is fully initialized.
 *
 * We're simply converting this into a promise.
 */
const waitUntilReady = (src: CppLibrary): Promise<void> =>
  new Promise(resolve => (src.onRuntimeInitialized = resolve))

/**
 * Export a default function which instantiates the library
 * @param {Object} bin Emscripten library to initialize
 */
export const Loader = (bin: () => CppLibrary): LoaderFn => async (): Promise<{
  readonly library: CppLibrary
}> => {
  const library = bin()
  await waitUntilReady(library)
  return {
    library
  }
}
