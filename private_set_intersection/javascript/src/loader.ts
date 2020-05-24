import * as psiCardinality from 'psi_'

export type NestedLibrary = { readonly library: psiCardinality.Library }
export type Loader = NestedLibrary

/**
 * Export a default function which instantiates the library
 * @param {Object} bin Emscripten library to initialize
 */
export const createLoader = async (
  bin: () => Promise<psiCardinality.Library>
): Promise<NestedLibrary> => ({
  library: await bin()
})
