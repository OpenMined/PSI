import * as psi from 'psi_'

export type NestedLibrary = { readonly library: psi.Library }
export type Loader = NestedLibrary

/**
 * Export a default function which instantiates the library
 * @param {Object} bin Emscripten library to initialize
 */
export const createLoader = async (
  bin: () => Promise<psi.Library>
): Promise<NestedLibrary> => ({
  library: await bin()
})
